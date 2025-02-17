#include <pmdsky.h>
#include <cot.h>

void CreateRoomInCell (int topLeftX, int topLeftY, int botRightX, int botRightY, int roomId, void* roomStuff, int bitflags);
void GenerateMazeBacktrackingFloor (int gridSizexX, int gridSizeY, struct floor_properties *floor_props);
void GenerateSidewinderFloor (int gridSizexX, int gridSizeY, struct floor_properties *floor_props);