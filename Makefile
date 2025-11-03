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

# Helper variables
VANILLA_ROM := vanilla.nds
XDELTA_BASE := unpatched-base.xdelta
BSDIFF_OUT := archipelago-base.bsdiff

TARGET		:=	out
BUILD		:=	build
SOURCES		:=	src src/cot
INCLUDES	:=	include pmdsky-debug/headers
OPT_LEVEL := -O2

# Change to "RELEASE_CONFIG := -DNDEBUG" for release builds without asserts and logs
RELEASE_CONFIG := -DDEBUG

PYTHON := python3
XDELTA := xdelta3
BSDIFF := bsdiff4f

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

export CUSTOMSYMBOLS	:= 	$(wildcard symbols/custom_*.ld)
export GENERATEDSYMBOLS :=	$(foreach sym_file,$(CUSTOMSYMBOLS),\
							$(patsubst symbols/custom_%.ld,symbols/generated_%.ld,$(sym_file)))
 
#---------------------------------------------------------------------------------
.PHONY: $(BUILD)
$(BUILD): $(GENERATEDSYMBOLS)
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
	@echo "\e[1;36mClean! \e[0m"
 
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

# Create the desired ld file for a region.
symbols/generated_%.ld:
	$(PYTHON) scripts/generate_linkerscript.py $(patsubst generated_%.ld,%,$(@F))

.PHONY: out
out: build
	$(PYTHON) scripts/patch.py $(REGION) $(ROM) $(OUTPUT).elf $(ROM_OUT)

.PHONY: asmdump
asmdump: build
	arm-none-eabi-objdump -S -d $(OUTPUT).elf > $(OUTPUT).asm

.PHONY: headers
headers:
	cd pmdsky-debug/headers && $(PYTHON) augment_headers.py --aliases --deprecate-aliases --docstrings
	
.PHONY: backup
backup: 
	@if [ -f "$(ROM)" ]; then \
		$(XDELTA) -e -f -s "$(VANILLA_ROM)" "$(ROM)" backups/backup-"$$(date +%s)".xdelta; \
	fi
	
.PHONY: rom
rom: backup
	$(XDELTA) -d -f -s $(VANILLA_ROM) $(XDELTA_BASE) $(ROM)

.PHONY: xdelta
xdelta:
	$(XDELTA) -e -f -s $(VANILLA_ROM) $(ROM) $(XDELTA_BASE)
	
.PHONY: bsdiff
bsdiff:
	@echo "\e[1;33mThis part takes up to a minute, please be patient! \e[0m"
	$(BSDIFF) $(VANILLA_ROM) $(ROM_OUT) $(BSDIFF_OUT)
	@echo "\e[1;32mDone! \e[0m"

.PHONY: scripts
scripts:
	$(PYTHON) tools/exps_cli.py $(ROM)

.PHONY: offsets
.SILENT: offsets
offsets:
	echo "Position of overlay36 (Decimal):"
	LANG=C grep -a -b -P -U -o "\x0d\xf0\xad\xba" $(ROM_OUT) | cut -d ":" -f1
	echo "Position of hintables.bin (Decimal):"
	LANG=C grep -a -b -P -U -o "\xe0\x51\x5b\xad" $(ROM_OUT) | cut -d ":" -f1
