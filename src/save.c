#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

int SaveMoreData(int* file_offset, void* buf, int size, int new_file_offset, bool force_save_flag) {
  int save_status = WriteSaveFile(file_offset, buf, size);
  if(save_status == 0 || force_save_flag) {
    *file_offset = new_file_offset;
    save_status = WriteSaveFile(file_offset, &CUSTOM_SAVE_AREA, sizeof(CustomSaveArea));
  }
  return save_status;
}

int LoadMoreData(int* file_offset, void* buf, int size, int new_file_offset) {
  int load_status = ReadSaveFile(file_offset, buf, size);
  if(load_status == 0) {
    *file_offset = new_file_offset;
    load_status = ReadSaveFile(file_offset, &CUSTOM_SAVE_AREA, sizeof(CustomSaveArea));
  }
  return load_status;
}

__attribute((used)) void CustomClearAdventureLogStruct() {
  ClearAdventureLogStruct();
  MemZero(&CUSTOM_SAVE_AREA, sizeof(CustomSaveArea));
}

__attribute((used)) int SaveMoreDataWrapper1(int* file_offset, void* buf, int size, int new_file_offset) { return SaveMoreData(file_offset, buf, size, 0xB7, false); }

__attribute((used)) int SaveMoreDataWrapper2(int* file_offset, void* buf, int size, int new_file_offset) { return SaveMoreData(file_offset, buf, size, 0x17F, true); }

__attribute((used)) int SaveMoreDataWrapper3(int* file_offset, void* buf, int size, int new_file_offset) { return SaveMoreData(file_offset, buf, size, 0x17F, false); }

__attribute((used)) int SaveMoreDataWrapper4(int* file_offset, void* buf, int size, int new_file_offset) { return SaveMoreData(file_offset, buf, size, 0xB7, true); }

__attribute((used)) int LoadMoreDataWrapper1(int* file_offset, void* buf, int size, int new_file_offset) { return LoadMoreData(file_offset, buf, size, 0xB7); }

__attribute((used)) int LoadMoreDataWrapper2(int* file_offset, void* buf, int size, int new_file_offset) { return LoadMoreData(file_offset, buf, size, 0x17F); }
