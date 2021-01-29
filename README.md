# GGJ-2021-Lost-and-Found

Public repo for our GGJ 2021 game.

To build this repo, you'll need SDL2.

Create a `build` folder and don't forget to add specific targets for the game:
- `build/win` for Windows executables
- `build/linux` for Linux builds
- `build/web` for emscripten compiled HTML5 builds

After creating all the interested build folders you can run:
- `make win` for Windows builds
- `make` for Linux builds
- `make web` for HTML5 Web builds

