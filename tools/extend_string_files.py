"""
Expands the string files for rom.nds by n strings.
Usage: python3 tools/extend_string_files.py n

Required packages (pip install):
- skytemple-files
"""

import sys
import ndspy.rom
from skytemple_files.data.str.handler import StrHandler
from skytemple_files.common.util import get_ppmdu_config_for_rom
from skytemple_files.common.ppmdu_config.data import GAME_REGION_EU, GAME_REGION_US, GAME_REGION_JP

def main(num_strings_to_add):
    rom_name = 'rom.nds'

    rom = ndspy.rom.NintendoDSRom.fromFile(rom_name)
    config = get_ppmdu_config_for_rom(rom)
    files = []

    if(config.game_region == GAME_REGION_EU):
        files = ['e', 'f', 'g', 'i', 's']
    elif(config.game_region == GAME_REGION_JP):
        files = ['j']
    else:
        files = ['e']

    for filename in files:
        # get file
        string_filepath = 'MESSAGE/text_'+ filename + '.str'
        bin_before = rom.getFileByName(string_filepath)
        strings_deserialized = StrHandler.deserialize(bin_before, string_encoding=config.string_encoding)
        # do stuff to strings
        for i in range(num_strings_to_add):
            strings_deserialized.strings.append("New string " + str(i))
        # save changes
        bin_after = StrHandler.serialize(strings_deserialized)
        rom.setFileByName(string_filepath, bin_after)
        print("Expanded text_" + filename + ".str by " + str(num_strings_to_add))
    rom.saveToFile('rom.nds')
    print("Saved changes.")
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Please provide a number to extend the lists by.", file=sys.stderr)
        exit(1)

    main(int(sys.argv[1]))
