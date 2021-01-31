#include "SheetMap.h"

SheetMap::SheetMap() {

}

SheetMap& SheetMap::getInstance() {
    static SheetMap sheetMap;
    return sheetMap;
}