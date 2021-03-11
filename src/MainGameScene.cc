#include "MainGameScene.h"

MainGameScene::MainGameScene(int startingLevel) {
    
}

void MainGameScene::update(float totalTime, float dt, SheetMap &sm) {
    handleInput(sm);
    sm.directionArrow->angle += dt * 100 * sm.directionInput.x;

    //===== COLLISION BEGINS
    sm.playerBodyRect.x = sm.player->pos->x;
    sm.playerBodyRect.y = sm.player->pos->y;
    sm.playerBodyRect.w = sm.player->currentFrameRect->w;
    sm.playerBodyRect.h = sm.player->currentFrameRect->h;
    sm.playerCollision = checkCollision(sm.collidableTiles, sm.player);
    //===== COLLISION ENDS

    //===== LIMIT VELOCITY BEGINS
    sm.player->vel->x = clamp(sm.player->vel->x, -maximumSpeed, maximumSpeed);
    sm.player->vel->y = clamp(sm.player->vel->y, -maximumSpeed, maximumSpeed);
    // normalizing (TODO move this to function)
    float vUnit = sqrt(pow(sm.player->vel->x, 2) + pow(sm.player->vel->y, 2));
    vUnit = vUnit == 0 ? 1 : vUnit;
    sm.player->vel->x = (sm.player->vel->x / vUnit) * maximumSpeed;
    sm.player->vel->y = (sm.player->vel->y / vUnit) * maximumSpeed;
    //===== LIMIT VELOCITY ENDS

    //===== MOVING BEGINS
    if (sm.playerIsFlying) {
        if (sm.playerCollision) {
            printf("collision!\n");
            // playerIsFlying = false;

            // player->vel->x = 0;
            // player->vel->y = 0;
            sm.player->vel->x *= 0.05f;
            sm.player->vel->y *= 0.05f;
        }
    } else {
        // moving with platform here ...
    }
    //===== MOVING ENDS

    sm.player->pos->x += sm.player->vel->x;
    sm.player->pos->y += sm.player->vel->y;
    sm.player->pos->x = clamp(sm.player->pos->x, 0, SCREEN_WIDTH);
    sm.player->pos->y = clamp(sm.player->pos->y, 0, SCREEN_HEIGHT);
    sm.player->frame = 0;
    sm.player->update(dt);

    sm.directionArrow->pos->x = sm.player->pos->x + 4;
    sm.directionArrow->pos->y = sm.player->pos->y;
    sm.directionArrow->frameTime += dt;
    if (sm.directionArrow->frameTime > sm.directionArrow->frameRate) {
        sm.directionArrow->frameTime = 0;
        sm.directionArrow->frame = (sm.directionArrow->frame + 1) % sm.directionArrow->totalFrames;
    }
    sm.directionArrow->update(dt);

    for (auto obj : sm.levelObjects) {
    //   cout << x.first << " " << x.second << endl; 
        obj.second->pos->x += obj.second->vel->x;
        obj.second->pos->y += obj.second->vel->y;
        obj.second->pos->x = clamp(obj.second->pos->x, 0, SCREEN_WIDTH);
        obj.second->pos->y = clamp(obj.second->pos->y, 0, SCREEN_HEIGHT);
        obj.second->frameTime += dt;
        if (obj.second->frameTime > obj.second->frameRate) {
            obj.second->frameTime = 0;
            obj.second->frame = (obj.second->frame + 1) % obj.second->totalFrames;
        }
        obj.second->update(dt);
    }
}

void MainGameScene::render(SDL_Renderer* mainRenderer, TextHandler* textHandler) {
    textHandler->render(mainRenderer, "Game started!", 58, 10);
}

bool MainGameScene::isFinished() {
    return finished;
}

void MainGameScene::handleInput(SheetMap &sm) {
    if (sm.input->getKeyDown(Input::ACTION)) {
        printf("starting to fly\n");
        sm.playerIsFlying = true;
        sm.player->vel->x = cos(sm.directionArrow->angle * PI / 180.0);
        sm.player->vel->y = sin(sm.directionArrow->angle * PI / 180.0);
    }

    sm.directionInput.y = 
        (sm.input->getKeyDown(INPUT::DOWN)) ? 1 :
        (sm.input->getKeyDown(INPUT::UP) ? -1 : 0);
    sm.directionInput.x = 
        (sm.input->getKeyDown(INPUT::RIGHT)) ? 1 :
        (sm.input->getKeyDown(INPUT::LEFT) ? -1 : 0);
    

    if (sm.directionInput.x != 0) {
        printf("oh my gaaaaaaaaaaaaaaaaa\n");
    }
}

bool MainGameScene::checkCollision(std::vector<std::vector<int>> level, float sx, float sy, float ex, float ey) {
    bool collision = false;
    int tileSize = 8;
    for (int i = sx; i <= ex; i += tileSize) {
        if (collision) {
            break;
        }
        for (int k = sy; k <= ey; k += tileSize) {
            if (collision) {
                break;
            }
            int px = i / tileSize;
            int py = k / tileSize;
            if (level[px][py]) {
                collision = true;
                if (sgn(i - SheetMap::getInstance().player->pos->x) == sgn(SheetMap::getInstance().player->vel->x)) {
                    SheetMap::getInstance().player->vel->x = SheetMap::getInstance().player->vel->x * -1.0f;
                }
                if (sgn(k - SheetMap::getInstance().player->pos->y) == sgn(SheetMap::getInstance().player->vel->y)) {
                    SheetMap::getInstance().player->vel->y = SheetMap::getInstance().player->vel->y * -1.0f;
                }
            }
        }
    }
    return collision;
}

bool MainGameScene::checkCollision(std::vector<std::vector<int>> tiles, Entity* player) {
    float sx = player->outputFrameRect->x + player->vel->x;
    float sy = player->outputFrameRect->y + player->vel->y;
    float ex = sx + player->outputFrameRect->w;
    float ey = sy + player->outputFrameRect->h;
    return checkCollision(tiles, sx, sy, ex, ey);
}

void MainGameScene::resetScene() {
    finished = false;
}