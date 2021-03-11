#ifndef GLOBALS
#define GLOBALS

#include <algorithm>

#define ASSETS_FOLDER "assets/"
#define PI 3.14159265
#ifndef DEBUG
#define DEBUG false
#endif

#define INDEX_MAIN_MENU 0
#define INDEX_INTRO_CUTSCENE 1
#define INDEX_MAIN_GAME 2
#define INDEX_ENDING 3

int gameScale = 4;

const int SCREEN_WIDTH = 192;
const int SCREEN_HEIGHT = 112;

//===== MATH BEGINS
float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// MATH ENDS =======

#endif // GLOBALS