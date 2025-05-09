# eos-archipelago-patches
This repository contains all ROMside custom code used in the Pokémon Mystery Dungeon: Explorers of Sky [Archipelago APWorld](https://github.com/CrypticMonkey33/ArchipelagoExplorersOfSky).

## Just testing?
Most of the below info doesn't apply to you. Just go check out the installation guide for your OS ([Linux](https://github.com/Chesyon/eos-archipelago-patches/blob/main/install_linux.md)) ([Windows](https://github.com/Chesyon/eos-archipelago-patches/blob/main/install_windows.md)) ([MacOS](https://github.com/Chesyon/eos-archipelago-patches/blob/main/install_macos.md)). Once the repository is set up, run `make everything+b` to get a `archipelago-base.bsdiff` that you can drop into the [APWorld](https://github.com/CrypticMonkey33/ArchipelagoExplorersOfSky/tree/main/worlds/pmd_eos/data).

## Makefile and the ROMs:
We use Makefile to make compiling way easier. There are three NDS files in this repo, each with different purposes:
* vanilla.nds: This needs to be provided by you. This should be an unmodified EU ROM for Pokémon Mystery Dungeon: Explorers of Sky. You will not modify this file.
* rom.nds: The input file for c-of-time. Any changes that need to be made in SkyTemple or with external programs that aren't c-of-time should be done on this ROM.
* out.nds: The output file from c-of-time. This is the final-ish ROM (minus the [world-specific data that gets written while patching](https://github.com/CrypticMonkey33/ArchipelagoExplorersOfSky/blob/main/worlds/pmd_eos/Rom.py#L44)), that Archipelago players will be using. This has all the C/ASM code from the repository applied. **Avoid making changes here, since they will be overwritten when re-applying c-of-time code!**

Now, we have a bunch of Makefile commands for the sake of applying code and patches. There are seven base commands, and a bunch of combo commands.
### Base commands
* `make out`: Applies code from the CoT repo to rom.nds to generate out.nds.
* `make clean`: Removes unwanted files. Generally, this needs to be run before running `make out`.
* `make rom`: Get the latest rom.nds by applying unpatched-base.xdelta to vanilla.nds.
* `make xdelta`: Make a new unpatched-base.xdelta from rom.nds.
* `make bsdiff`: Make an output archipelago-base.bsdiff from out.nds.
* `make asmdump`: Shows the ASM for the compiled C code.
* `make headers`: No clue what this one does, honestly. You probably won't need it!
### Combo commands:
* `make out-c`: Cleans up unwanted files and then applies CoT code. (All below combos use `out-c` when applying CoT code, so don't worry about running `make clean` for them!)
* `make everything`: Apply the xdelta and apply CoT code.
* `make everything+x`: Make an xdelta and apply CoT code.
* `make everything-x`: Don't apply or make an xdelta, and apply CoT code. (This is the same as `out-c`, but I'm just covering all my bases.)
* `make everything+b`: Apply the xdelta, apply CoT code, and make a bsdiff.
* `make everything+x+b`: Make an xdelta, apply CoT code, and make a bsdiff.
* `make everything-x+b`: Don't apply or make an xdelta, apply CoT code, and make a bsdiff.

Below is the readme for c-of-time, which this repository is a fork of.
# c-of-time

![c-of-time logo by Irdkwia](./cot-logo.png)
*Logo by [Irdkwia](https://github.com/irdkwia)*

An environment for hooking and linking to Pokémon Mystery Dungeon: Explorers of Sky.

## Credits
This project is loosely based on [EternalCode's template](https://github.com/EternalCode/Empty-Template). The build configuration is based on scripts provided by [devkitPro](https://devkitpro.org). The patch format was inspired by [Starlight](https://github.com/shadowninja108/Starlight).

Special thanks to [UsernameFodder](https://github.com/UsernameFodder) for the [pmdsky-debug](https://github.com/UsernameFodder/pmdsky-debug) project, [Frostbyte](https://github.com/Frostbyte0x70) for the *ExtraSpace* patch and [irdkwia](https://github.com/irdkwia) for their research on item, move and special process effects.

## Rust subsystem
**NOTE: The `main` branch does currently not contain the Rust subsystem anymore**, as it's support
for symbols for `pmdsky-debug` is outdated and we eventually want to split the Rust subsytem
off so we can keep `c-of-time` up-to-date with `pmdsky-debug` more easily. Use the `rust` branch
if you want to use the Rust subsystem.

c-of-time can also be used with Rust projects. If you want to use Rust (including mixed Rust + C projects),
continue reading the `README.md` in the `rust` directory.

If you want to build pure C projects, continue below.

## Project setup

### Preparing the ROM

You need a US, EU, or JP ROM of Pokémon Mystery Dungeon: Explorers of Sky. The ROM must be patched with the [`ExtraSpace` patch by Frostbyte](https://github.com/Frostbyte0x70/EoS-asm-patches/blob/main/src/ExtraSpace.asm). You can apply the patch with [SkyTemple](https://skytemple.org):
  1. Open the ROM in SkyTemple
  2. Click *ASM Patches* (*Patches > ASM* in SkyTemple 1.4+) and switch to the *Utility* tab
  3. Select the *ExtraSpace* patch and click *Apply*

### Installing dependencies

Refer to the setup guide for your platform:
- [Windows](./install_windows.md)
- [Linux](./install_linux.md)
- [macOS](./install_macos.md)

## Building
To build the project, run `make patch`. This command will build your code, inject it into an overlay in the provided ROM and apply the patches in the `patches` directory. The output ROM will be saved as `out.nds` by default.

If you want to check the generated assembly, run `make asmdump`. A file `out.asm` will be generated, which contains an assembly listing annotated with the corresponding source code lines.

## Usage
Patches can be added to `.asm` files inside the `patches` directory. These patch files contain offsets into functions that should be patched and assembly instructions, which allow calling into custom code. See `src/main.c` and `patches/patches.asm` for examples.

### Logging and assertions
You can use the logging macros `COT_LOG`, `COT_WARN` and `COT_ERROR`. To view the logs, open the ROM in the SkyTemple debugger and check "Game Internal" in the log window. A macro for assertions `COT_ASSERT(expr)` is also available.

To disable assertions and logging globally and save some performance, change `RELEASE_CONFIG` in `Makefile`.

### Custom move/item effects and special processes
To create custom special processes, add them into the `switch` statement in `CustomScriptSpecialProcessCall`. This function is only called for special process ID 100 and greater for compatibility with existing patches.

You can add custom item or move effects in `CustomApplyItemEffect` and `CustomApplyMoveEffect`.

Please note that custom move effects are currently *not* handled by the *Metronome* move.

#### Compatiblity with existing patches

**Note: ROMs patched with c-of-time currently experience crashes with the `ExtractItemCode` and `ExtractMoveCode` patch.**

You can work around the crash by removing `.open "overlay29.bin", overlay29_start` and all following lines in `patches/internal.asm`.
However, doing so will cause custom effects written in C to have no effect.

Please reach out to us [on Discord](https://discord.gg/skytemple) for potential workarounds if you need to use the aforementioned patches while also adding effects via c-of-time.

### Custom script engine instructions

Custom script engine instructions are a more complex, but more powerful alternative to special processes.

Advantages over special processes include:
- No frame delay (especially beneficial when building complex minigames or other real-time interactions)
- Custom instructions can be used inside targeted routines, while `ProcessSpecial` doesn't work
- Cleaner script code overall without resorting to macros
- Performing entity-specific operations

Disadvantages:
- At the moment, routines cannot conditionally hang on custom instructions (as opposed to special processes, which can hang the routine and loop its code upon returning -1)
- No built-in support in SkyTemple (workaround provided below)

Like special processes, custom instructions are capable of returning a value that can then be checked with a switch-statement. For an example, see the custom instruction `CheckInputStatus` in `ground_instructions.c`.

Custom instructions are disabled by default. To enable support for custom instructions in c-of-time, open the file `include/cot/custom_instructions.h` and change the line `#define CUSTOM_GROUND_INSTRUCTIONS 0` to `#define CUSTOM_GROUND_INSTRUCTIONS 1`. You can now add your own instructions to the `CUSTOM_INSTRUCTIONS` array in `ground_instructions.c`.

#### Accessing custom script engine instructions in SkyTemple

SkyTemple will not recognize custom script engine instructions by default.
Trying to save a script that references a custom instruction will lead to an error, and scripts containing custom instructions won't decompile.
To use custom instructions in SkyTemple, open the folder where SkyTemple is installed, edit the file
`<SkyTemple installation directory>/skytemple_files/_resources/ppmdu_config/pmd2scriptdata.xml` and add the following lines under the `<OpCodes>` tag:

```xml
  <!-- Custom instructions start at ID 0x1000 -->
  <OpCode id="0x1000" name="SetDialogueBoxAttributes"              params="6"  stringidx="-1" unk2="0"  unk3="0"   >
    <Argument id="0" type="uint" name="offset_x"/>
    <Argument id="1" type="uint" name="offset_y"/>
    <Argument id="2" type="uint" name="width"/>
    <Argument id="3" type="uint" name="height"/>
    <Argument id="4" type="uint" name="screen"/>
    <Argument id="5" type="uint" name="frame"/>
  </OpCode>
  <OpCode id="0x1001" name="CheckInputStatus"                      params="1"  stringidx="-1" unk2="0"  unk3="0"   >
    <Argument id="0" type="uint" name="mode"/>
  </OpCode>
  (add additional instructions here...)
</OpCodes>
```

We're planning to provide a SkyTemple plug-in that will make this process easier in the future.

### Custom script menus

Custom script engine menus are a method of creating new, complex menus that would otherwise be inefficient with special processes or custom script engine instructions. They also serve as a more powerful alternative to the script engine's existing `message_SwitchMenu` and `message_SwitchMenu2` instructions, which do allow for simple menus, but are limited in functionality.

By default, the first new menu ID is 80; this can be changed in `src/cot/menu_hooks.c`. To call a custom script menu, simply use the `message_Menu` instruction in a script; custom menus also support return values, which can be checked with a switch-statement in ExplorerScript like:

```c
switch(message_Menu(81)) {
    case 0:
      debug_Print("ACCESS GRANTED.");
      break;
    default:
      debug_Print("INCORRECT PASSWORD. CARD EJECTION IMMINENT.");
}
```

Generally, custom script menus are more complicated to create than special processes or instructions. Instead of a single handler being defined for each menu, each menu has three functions to handle its creation, maintenance, and destruction. See `include/cot/menus.h` and `src/menus.c` for information regarding these functions, as well as the definition of a global menu information structure.

Despite the name, a custom script "menu" could technically be used for anything that needs to follow this general outline:
1. An initial phase that calls a function once, meant to allocate or create certain structures
2. An update phase that calls a function every frame until it finally returns `true`, meant to continuously check the status of anything created in the initial phase
3. A final phase that calls a function once (only run after the update phase is complete), meant to deallocate any structures that were created in the initial phase

Additionally, keep in mind that when a script calls `message_Menu`, the current routine will hang, waiting for the menu to complete the three aforementioned phases in order.

Custom script menus are disabled by default. To enable support for custom menus in c-of-time, open the file `include/cot/menus.h` and change the line `#define CUSTOM_SCRIPT_MENUS 0` to `#define CUSTOM_SCRIPT_MENUS 1`. You can now add your own menus to the `CUSTOM_SCRIPT_MENUS` array in `menus.c`.

## Updating symbol definitions and headers
To update symbol data from `pmdsky-debug`, run `git submodule foreach git pull origin master`,
then clean the build with `make clean`.

## Adding custom symbols
If you've found symbols that are currently missing, consider contributing them to [pmdsky-debug](https://github.com/UsernameFodder/pmdsky-debug). You can find instructions in the repository's [contribution docs](https://github.com/UsernameFodder/pmdsky-debug/blob/master/docs/contributing.md).

For quick testing, you can also add symbols to `symbols/custom_[region].ld` (`symbols/generated_[region].ld` is auto-generated and should not be modified). You need to specify the file each symbol belongs to in comments:

```
/* !file arm9 */
MyCoolFunction = 0x200DABC;

/* !file overlay29 */
SomeDungeonFunction = 0x22DEABC;
SomeOtherDungeonFunction = 0x22DEEFF;
```

## Code size constraints

The built code gets injected into the custom overlay 36. The entire overlay is 228 KB big, most of which is reserved for common patches provided by SkyTemple. Your code will be placed in the last 32 KB, which are considered the "common area" . If the binary is larger than 32 KB, you will get the following linker error: 
```
error "section '.text' will not fit in region 'out'"
```

### Expanding the available space
To work around this issue, you can extend the space allocated in the overlay. **If you decide to extend the space, you do so at your own risk. Be careful since this space might be used by future patches!** Check the [list of assigned areas](https://docs.google.com/document/d/1Rs4icdYtiM6KYnWxMkdlw7jpWrH7qw5v6LOfDWIiYho) to find out if patches used in your ROM are affected.

The value of `ORIGIN` must a multiple of 16 (end with 0 in hexadecimal). Therefore, the amount of bytes added to `LENGTH` must also be a multiple of 16.

To extend the allocated space, open `linker.ld` and edit the following line:
```
out     : ORIGIN = 0x23D7FF0, LENGTH = 0x8010
```

Subtract the amount of additional bytes you want to allocate from `ORIGIN` and add them to `LENGTH`. Next, open `patches/patch.py` and set `START_ADDRESS` of the top of the file to the same value as `ORIGIN` in the linker script.

### Optimizing for size
You can also change the compiler flags to optimize for size instead of speed. To do so, set `OPT_LEVEL := Os` in `Makefile`. Effectiveness varies per project.

## Licensing
- Build scripts (everything under the `tools`) are licensed under GPLv3. Review the file `LICENSE_GPLv3` for more information.
- All other code is licensed under MIT. Review the file `LICENSE_MIT` for more information.

