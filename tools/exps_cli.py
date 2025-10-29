"""
Compiles exps files within a .skytemple folder into a ROM.
usage: python3 exps_cli.py [rom] [-q] [scripts]
Script paths should be relative to the .skytemple/SCRIPT folder. You may provide as many script paths as you like, or provide no paths to compile all exps files within the SCRIPT folder.
A -q argument anywhere after the rom path will silence all logs.

Examples of valid syntax:
* Compile unionall.exps: python3 tools/exps_cli.py rom.nds COMMON/unionall.exps
* Compile all scripts: python3 tools/exps_cli.py rom.nds
* Compile partner.exps and partner2.exps: python3 tools/exps_cli.py rom.nds S00P01A/partner.exps S00P01A/partner2.exps
* Compile partner.exps and partner2.exps (no output): python3 tools/exps_cli.py rom.nds S00P01A/partner.exps -q S00P01A/partner2.exps
* Compile all scripts (no logs): python3 tools/exps_cli.py rom.nds -q

Required packages (pip install):
- skytemple-files
"""

from __future__ import annotations

import os
import sys

from ndspy.rom import NintendoDSRom

from skytemple_files.script.ssb.script_compiler import ScriptCompiler
from skytemple_files.script.ssb.writer import SsbWriter
from skytemple_files.common import string_codec
from skytemple_files.common.util import get_ppmdu_config_for_rom
from skytemple_files.common.ppmdu_config.data import Pmd2Data


class SimpleExpsCompiler:
    def __init__(self, rom_name: str, flags: []):
        self.rom_name = rom_name
        self.rom = NintendoDSRom.fromFile(rom_name)
        self.flags = flags
        self.config = get_ppmdu_config_for_rom(self.rom)
        if self.config.string_encoding == string_codec.PMD2_STR_ENCODER:
            string_codec.init()  # Needed to prevent a LookupError in write()
        self.compiler = ScriptCompiler(self.config)
        dst_path = f"{rom_name}.skytemple"  # dst = ".skytemple"
        self.dst_script_path = os.path.join(dst_path, "SCRIPT")
        dst_macro_path = os.path.join(dst_path, "Macros")
        self.dst_abs_macro_path = os.path.abspath(dst_macro_path)
        self.log("Initialized SimpleExpsCompiler!")

    def log(self, message: str):
        if "q" not in self.flags:
            print(message)

    def compile_single_script(self, script_path: str):
        full_script_path = os.path.join(self.dst_script_path, script_path)
        if not os.path.exists(full_script_path):
            print(f"{script_path} not found.", file=sys.stderr)
            exit(1)
        with open(full_script_path, "r") as file:
            script = file.read()
        ssb_model, _ = self.compiler.compile_explorerscript(
            script, "", lookup_paths=[self.dst_abs_macro_path]
        )
        compiled_ssb = SsbWriter(ssb_model, self.compiler.rom_data).write()
        self.rom.setFileByName(
            os.path.join("SCRIPT", script_path.replace(".exps", ".ssb")), compiled_ssb
        )
        self.log(f"Compiled {script_path}")

    def compile_multiple_scripts(self, scripts: []):
        if len(scripts) == 0:
            scripts = self.get_all_exps()
        for script in scripts:
            self.compile_single_script(script)

    def get_all_exps(self):
        exps_files = []
        for root, _, files in os.walk(self.dst_script_path):
            for file in files:
                if file.endswith(".exps"):
                    full_path = os.path.join(root, file)
                    relative_path = os.path.relpath(
                        full_path, start=self.dst_script_path
                    )
                    exps_files.append(relative_path)
        return exps_files

    def save(self, filename: str | None = None):
        if filename is None:
            filename = self.rom_name
        self.rom.saveToFile(filename)
        self.log(f"Saved to {filename}!")


def main(rom_name: str, scripts: [], flags: []):
    simple_compiler = SimpleExpsCompiler(rom_name, flags)
    simple_compiler.compile_multiple_scripts(scripts)
    simple_compiler.save()


if __name__ == "__main__":
    if len(sys.argv) <= 1:
        print("Please provide a ROM.", file=sys.stderr)
        exit(1)
    if not os.path.exists(sys.argv[1]):
        print(f"ROM {sys.argv[1]} not found.", file=sys.stderr)
        exit(1)
    rom_name = sys.argv.pop(1)
    # Sort everything past the first arg into either flags or scripts
    flags = []
    scripts = []
    for x in sys.argv[1:]:
        if x.startswith("-") and len(x) == 2:
            flags.append(x[1])
        else:
            scripts.append(x)
    main(rom_name, scripts, flags)
