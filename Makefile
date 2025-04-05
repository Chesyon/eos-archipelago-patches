# This file is based off DevkitARM's Makefile template and the included DevkitARM make files.
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# the prefix on the compiler executables
#---------------------------------------------------------------------------------
PREFIX          :=      arm-none-eabi-

export CC       :=      $(PREFIX)gcc
export CXX      :=      $(PREFIX)g++
export AS       :=      $(PREFIX)as
export AR       :=      $(PREFIX)gcc-ar
export OBJCOPY  :=      $(PREFIX)objcopy
export STRIP    :=      $(PREFIX)strip
export NM       :=      $(PREFIX)gcc-nm
export RANLIB   :=      $(PREFIX)gcc-ranlib

#---------------------------------------------------------------------------------
%.o: %.c
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -DREGION_$(REGION) -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.m
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(OBJCFLAGS) -DREGION_$(REGION) -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.s
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(ASFLAGS) -DREGION_$(REGION) -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.S
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(ASFLAGS) -DREGION_$(REGION) -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.elf:
	echo linking $(notdir $@)
	$(LD)  $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------

#             <-- Change to EU or JP if required
REGION := EU
ROM := rom.nds
ROM_OUT := out.nds

TARGET		:=	out
BUILD		:=	build
SOURCES		:=	src src/cot
INCLUDES	:=	include pmdsky-debug/headers
OPT_LEVEL := -Os

# Change to "RELEASE_CONFIG := -DNDEBUG" for release builds without asserts and logs
RELEASE_CONFIG := -DDEBUG

PYTHON := python3

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-marm -mno-thumb-interwork

CFLAGS	:=	-g -Wall $(OPT_LEVEL) $(RELEASE_CONFIG) $(SP_EFFECT_COMPAT) \
 			-march=armv5te -mtune=arm946e-s -fomit-frame-pointer -fno-short-enums \
			-ffast-math -fno-builtin \
			-fmacro-prefix-map=$(realpath $(CURDIR)/..)=. \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9

# Those are to be set by command line arguments.
CFLAGS  +=  $(EXTRA_CFLAGS)

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-T $(CURDIR)/../symbols/generated_$(REGION).ld \
			-T $(CURDIR)/../symbols/custom_$(REGION).ld -T $(CURDIR)/../linker.ld \
			-g $(ARCH) -Wl,-Map,$(notdir $*.map) -Xlinker -no-enum-size-warning -nostdlib  -Xlinker --no-check-sections

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= 
 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	
 
#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))
 
#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(BINFILES:.bin=.o) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
 
#---------------------------------------------------------------------------------
.PHONY: $(BUILD)
$(BUILD): symbols/generated_$(REGION).ld
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

.PHONY: buildobjs
buildobjs:
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile buildobjs
 
#---------------------------------------------------------------------------------
.PHONY: clean
clean:
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).asm $(ROM_OUT).nds symbols/generated_*.ld
	@echo Clean!
 
#---------------------------------------------------------------------------------
else
 
DEPENDS	:=	$(OFILES:.o=.d)
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).elf	:	$(CURDIR)/../linker.ld $(OFILES)

.PHONY: buildobjs
buildobjs: $(OFILES)

-include $(DEPENDS)
 
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------

symbols/generated_$(REGION).ld:
	$(PYTHON) scripts/generate_linkerscript.py $(REGION)

.PHONY: out
out: build
	$(PYTHON) scripts/patch.py $(REGION) $(ROM) $(OUTPUT).elf $(ROM_OUT)

.PHONY: asmdump
asmdump: build
	arm-none-eabi-objdump -S -d $(OUTPUT).elf > $(OUTPUT).asm

.PHONY: headers
headers:
	cd pmdsky-debug/headers && $(PYTHON) augment_headers.py --aliases --deprecate-aliases --docstrings
	
.PHONY: rom
rom:
	xdelta3 -d -f -s vanilla.nds unpatched-base.xdelta rom.nds

.PHONY: xdelta
xdelta:
	xdelta3 -e -f -s vanilla.nds rom.nds unpatched-base.xdelta
	
.PHONY: bsdiff
bsdiff:
	@echo "\e[1;33mThis part takes a while, please be patient! \e[0m"
	bsdiff4 vanilla.nds out.nds archipelago-base.bsdiff
	@echo "\e[1;32mDone! \e[0m"
	
.PHONY: overlay36-location
.SILENT: overlay36-location
overlay36-location:
	echo "Position of Overlay36 (Decimal):"
	LANG=C grep -a -b -P -U -o "\x0d\xf0\xad\xba" out.nds | cut -d ":" -f1

.PHONY: out+c
out+c:
	make clean && make out

# Apply the xdelta and apply CoT code.
.PHONY: everything
everything: rom out+c overlay36-location

# Make an xdelta and apply CoT code.
.PHONY: everything+x
everything+x: xdelta out+c overlay36-location

# Don't apply or make an xdelta, and apply CoT code. (same as out-c)
.PHONY: everything-x
everything-x: out+c overlay36-location

# Apply the xdelta, apply CoT code, and make a bsdiff.
.PHONY: everything+b
everything+b: rom out+c bsdiff overlay36-location

# Make an xdelta, apply CoT code, and make a bsdiff.
.PHONY: everything+x+b
everything+x+b: xdelta out+c bsdiff overlay36-location

# Don't apply or make an xdelta, apply CoT code, and make a bsdiff.
.PHONY: everything-x+b 
everything-x+b: out+c bsdiff overlay36-location
