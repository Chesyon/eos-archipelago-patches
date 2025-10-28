"""
Finds exps files in the .skytemple folder that compile to scripts functionally identical to their vanilla counterparts.
Note that this compares what is IN THE ROM to vanilla. Any exps modifications that are in the file but NOT in the ROM will not be detected by this script.

Required packages (pip install):
- skytemple-files
- tqdm
"""

from __future__ import annotations

import asyncio
import logging
import os
import sys
import traceback
import warnings
from concurrent.futures.thread import ThreadPoolExecutor
from threading import Lock

from ndspy.rom import NintendoDSRom
from tqdm import tqdm

from skytemple_files.common.util import (
    get_files_from_rom_with_extension,
    get_ppmdu_config_for_rom,
)
from skytemple_files.script.ssb.handler import SsbHandler

poison_container = [False]
loop = asyncio.new_event_loop()


progress_lock = Lock()
poison_lock = Lock()
folder_files = []
vanilla_candidates = []

async def main(executor):
    print("Finding exps in .skytemple folder...")
    skytemple_path = "rom.nds.skytemple/" # SKYTEMPLE FOLDER IS DEFINED HERE
    for root, _, files in os.walk(skytemple_path):
        for file in files:
            if file.endswith('.exps'):
                full_path = os.path.join(root, file)
                relative_path = os.path.relpath(full_path, start=skytemple_path)
                folder_files.append(relative_path)
    print("Preparing to check scripts in ROMs...")
    rom = NintendoDSRom.fromFile("rom.nds") # ROM IS DEFINED HERE
    vanilla = NintendoDSRom.fromFile("vanilla.nds") # VANILLA ROM IS DEFINED HERE
    static_data = get_ppmdu_config_for_rom(rom)
    vanilla_static_data = get_ppmdu_config_for_rom(vanilla)
    awaitables = []
    for i, file_name in enumerate(get_files_from_rom_with_extension(rom, "ssb")):
        # Run multiple in parallel with asyncio executors.
        awaitables.append(loop.run_in_executor(executor, process_single, file_name, static_data, rom, vanilla_static_data, vanilla))

    print("Checking scripts in ROMs...")
    pending = awaitables
    with tqdm(total=len(pending)) as pbar:
        while len(pending) > 0:
            len_pending_before = len(pending)
            done, pending = await asyncio.wait(pending, return_when=asyncio.FIRST_COMPLETED)
            pbar.update(len_pending_before - len(pending))
            # to raise exceptions of tasks back to main loop:
            for fut in done:
                try:
                    fut.result()
                except Exception:
                    loop.stop()
                    with poison_lock:
                        poison_container[0] = True
                    raise
    print("Done!")
    print("Found " + str(len(vanilla_candidates)) + " files in .skytemple folder whose SsbScripts are identical to those of vanilla:")
    for output_file in vanilla_candidates:
        print(output_file)


def process_single(filename, static_data, rom, vanilla_static_data, vanilla):
    exps_filename = filename.replace(".ssb", ".exps")
    if exps_filename in folder_files: # Does the file have an EXPS in the .skytemple folder? If not, this is guaranteed vanilla and we can ignore it entirely because we don't need to remove something that isn't there. 
        if vanilla.filenames.idOf(filename) is not None: # If it does, does vanilla have a matching script file? If not, this is guaranteed custom and we want to keep it.
            try:
                ssbbin = rom.getFileByName(filename)
                ssb = SsbHandler.deserialize(ssbbin, static_data)
                explorer_script, _ = ssb.to_ssb_script()
            except BaseException as ex:
                raise RuntimeError(f"Error decompiling {filename} from ROM.") from ex
            try:
                vanilla_ssbbin = vanilla.getFileByName(filename)
                vanilla_ssb = SsbHandler.deserialize(vanilla_ssbbin, vanilla_static_data)
                vanilla_explorer_script, _ = vanilla_ssb.to_ssb_script()
            except BaseException as ex:
                raise RuntimeError(f"Error decompiling {filename} from vanilla.") from ex
            if explorer_script == vanilla_explorer_script:
                vanilla_candidates.append(exps_filename)


def handle_exception(loop, context):
    # context["message"] will always be there; but context["exception"] may not
    print("Exception while scanning.", file=sys.stderr)
    if "exception" in context:
        ex = context["exception"]
        print(
            "".join(traceback.format_exception(etype=type(ex), value=ex, tb=ex.__traceback__)),
            file=sys.stderr,
        )
    else:
        print(f"AsyncIO fatal error: {context['message']}", file=sys.stderr)
    loop.stop()


if __name__ == "__main__":
    logging.basicConfig(level=logging.ERROR)
    warnings.filterwarnings("ignore") 

    executor = ThreadPoolExecutor(max_workers=max(2, os.cpu_count() - 2))
    loop.set_exception_handler(handle_exception)
    try:
        loop.run_until_complete(main(executor))
    finally:
        loop.close()
        executor.shutdown()
