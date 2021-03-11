#include "SheetMap.h"

SheetMap::SheetMap() {
    input = new Input();
}

SheetMap& SheetMap::getInstance() {
    static SheetMap sheetMap;
    return sheetMap;
}