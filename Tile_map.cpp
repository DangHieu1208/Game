#include "Tile_map.h"

void Map::loadMap(SDL_Renderer* ren, const char* file_name) {
    ifstream file(file_name);
    int n;

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            file >> tiles[i][j];
        }
    }

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j <32; j++) {
            tile[i][j].x = j*80;
            tile[i][j].y = i*80;
            tile[i][j].w = 80;
            tile[i][j].h = 80;
        }
    }

    mapTile[0].loadTex("graphic/tiles.png", ren);
    mapTile[0].setSrc(0, 16, 32, 32);

    mapTile[1].loadTex("graphic/tiles.png", ren);
    mapTile[1].setSrc(0, 160, 32, 32);

    Enemy enemy0;
    enemy0.loadTex("Uyen.jpg", ren);
    enemy0.setSrc(0, 0, 1152, 2048);
    enemy0.setDst(500, 500, 100, 100);
    enemies.push_back(enemy0);
}

void Map::renderMap(SDL_Renderer* ren, Player& player) {

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            SDL_Rect renderQuad = {tile[i][j].x - player.camera.x, tile[i][j].y - player.camera.y, 80, 80};
            SDL_Rect wallRect = {tile[i][j].x, tile[i][j].y, 80, 80};
            if (tiles[i + offSetY][j + offSetX] == 1) {
                mapTile[0].render(ren, renderQuad);
                if (player.checkCollision(wallRect)) {
                    player.solveCollision(wallRect);
                }
            } else {
                mapTile[1].render(ren, renderQuad);
            }
        }
    }

    for (size_t i = 0; i < enemies.size(); i++) {
        enemies[i].update(SDL_GetTicks(), player, tiles, tile);
        enemies[i].renderEnemy(ren, player.camera);
    }
}

void Map::print() {
 for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 32; j++) {
        cout << tiles[i][j] << " ";
    }
    cout << endl;
 }
}


