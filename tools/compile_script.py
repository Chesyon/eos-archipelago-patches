"""
Compiles a single exps script into a ROM, given the rom name and path to the script within the .skytemple SCRIPT folder.

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
    def __init__(self, rom_name: str):
        # Get config
        self.rom_name = rom_name
        self.rom = NintendoDSRom.fromFile(rom_name)
        self.config = get_ppmdu_config_for_rom(self.rom)
        if self.config.string_encoding == string_codec.PMD2_STR_ENCODER:
            string_codec.init() # Needed to prevent a LookupError in write()
        self.compiler = ScriptCompiler(self.config)
        dst_path = f"{rom_name}.skytemple" # dst = ".skytemple"
        self.dst_script_path = os.path.join(dst_path, "SCRIPT")
        dst_macro_path = os.path.join(dst_path, "Macros")
        self.dst_abs_macro_path = os.path.abspath(dst_macro_path)
        print("Initialized SimpleExpsCompiler!")

    def compile_single_script(self, script_path: str):
        print(f"Compiling {script_path}...")
        # TODO: make sure path exists first!
        with open(os.path.join(self.dst_script_path, script_path), 'r') as file:
            script = file.read()
        ssb_model, _ = self.compiler.compile_explorerscript(script, "", lookup_paths=[self.dst_abs_macro_path])
        compiled_ssb = SsbWriter(ssb_model, self.compiler.rom_data).write()
        self.rom.setFileByName(os.path.join("SCRIPT", script_path.replace(".exps", ".ssb")), compiled_ssb)
        print(f"Compiled {script_path}")

    def save(self, filename: str | None = None):
        if filename is None:
            filename = self.rom_name
        self.rom.saveToFile(filename)
        print(f"Saved to {filename}!")

def main(rom_name, script_path):
    simple_compiler = SimpleExpsCompiler(rom_name)
    simple_compiler.compile_single_script(script_path)
    simple_compiler.save()

if __name__ == "__main__":
    if len(sys.argv) == 2:
        print("Please provide a script path.", file=sys.stderr)
        exit(1)
    if len(sys.argv) == 1:
        print("Please provide a ROM and a script path.", file=sys.stderr)
        exit(1)
    if not os.path.exists(sys.argv[1]):
        print(f"ROM {sys.argv[1]} not found.", file=sys.stderr)
        exit(1)
    main(sys.argv[1], sys.argv[2])
