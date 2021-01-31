#include "Input.h"

Input::Input() {

}

void Input::update() {

}

bool Input::getKeyDown(int button) {
    return buttonsPressed[button];
}

void Input::setKeyDown(SDL_Keycode button, bool status) {
    bool b = status;
    if (button == Input::PK_UP || button == Input::SK_UP) {
        updateButtonStatus(Input::UP, buttonsPressed[Input::UP], status);
        buttonsPressed[Input::UP] = status;
    } else if (button == Input::PK_DOWN || button == Input::SK_DOWN) {
        updateButtonStatus(Input::DOWN, buttonsPressed[Input::DOWN], status);
        buttonsPressed[Input::DOWN] = status;
    } else if (button == Input::PK_LEFT || button == Input::SK_LEFT) {
        updateButtonStatus(Input::LEFT, buttonsPressed[Input::LEFT], status);
        buttonsPressed[Input::LEFT] = status;
    } else if (button == Input::PK_RIGHT || button == Input::SK_RIGHT) {
        updateButtonStatus(Input::RIGHT, buttonsPressed[Input::RIGHT], status);
        buttonsPressed[Input::RIGHT] = status;
    } else if (button == Input::PK_ACTION || button == Input::SK_ACTION) {
        updateButtonStatus(Input::ACTION, buttonsPressed[Input::ACTION], status);
        buttonsPressed[Input::ACTION] = status;
    } else if (button == Input::PK_CANCEL || button == Input::SK_CANCEL) {
        updateButtonStatus(Input::CANCEL, buttonsPressed[Input::CANCEL], status);
        buttonsPressed[Input::CANCEL] = status;
    } else if (button == Input::PK_ACCEPT) {
        updateButtonStatus(Input::ACCEPT, buttonsPressed[Input::ACCEPT], status);
        buttonsPressed[Input::ACCEPT] = status;
    } else if (button == Input::PK_CHECKPOINT) {
        updateButtonStatus(Input::CHECKPOINT, buttonsPressed[Input::CHECKPOINT], status);
        buttonsPressed[Input::CHECKPOINT] = status;
    }
}

void Input::updateButtonStatus(int button, bool oldState, bool newState) {
    buttonsUp[button] = false;
    buttonsDown[button] = false;
    if (oldState != newState) {
        if (oldState) {
            buttonsUp[button] = true;
            buttonsDown[button] = false;
        } else {
            buttonsUp[button] = false;
            buttonsDown[button] = true;
        }
    }
}