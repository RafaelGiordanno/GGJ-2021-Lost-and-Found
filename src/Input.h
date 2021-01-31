#ifndef INPUT
#define INPUT

#include <SDL2/SDL.h>

class Input {
public:
    static const int UP = 0;
    static const int DOWN = 1;
    static const int LEFT = 2;
    static const int RIGHT = 3;

    static const int ACTION = 4;
    static const int CANCEL = 5;

    static const int ACCEPT = 6;
    static const int CHECKPOINT = 7;

    static const int PAUSE = 8;
    static const int QUIT = 9;

    // primary keys
    static const SDL_Keycode PK_UP = SDLK_w;
    static const SDL_Keycode PK_DOWN = SDLK_s;
    static const SDL_Keycode PK_LEFT = SDLK_a;
    static const SDL_Keycode PK_RIGHT = SDLK_d;

    static const SDL_Keycode PK_ACTION = SDLK_k;
    static const SDL_Keycode PK_CANCEL = SDLK_l;

    static const SDL_Keycode PK_ACCEPT = SDLK_RETURN;
    static const SDL_Keycode PK_CHECKPOINT = SDLK_r;

    // secondary keys
    static const SDL_Keycode SK_UP = SDLK_UP;
    static const SDL_Keycode SK_DOWN = SDLK_DOWN;
    static const SDL_Keycode SK_LEFT = SDLK_LEFT;
    static const SDL_Keycode SK_RIGHT = SDLK_RIGHT;

    static const SDL_Keycode SK_ACTION = SDLK_z;
    static const SDL_Keycode SK_CANCEL = SDLK_x;

    Input();
    void update();
    bool getKeyDown(int button);
    void setKeyDown(SDL_Keycode, bool);

private:
    bool buttonsPressed[16];
    bool buttonsDown[16];
    bool buttonsUp[16];

    void updateButtonStatus(int button, bool oldState, bool newState);
};

#endif // INPUT