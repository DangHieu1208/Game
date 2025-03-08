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

    wall.loadTex("graphic/tiles.png", ren);
    wall.setSrc(0, 16, 32, 32);

    floor.loadTex("graphic/tiles.png", ren);
    floor.setSrc(0, 160, 32, 32);
}

void Map::createMap(SDL_Renderer* ren, Player& player) {
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            SDL_Rect renderQuad = {tile[i][j].x - player.camera.x, tile[i][j].y - player.camera.y, 80, 80};
            if (tiles[i + offSetY][j + offSetX] == 1) {
                //wall.setDst(tile[i][j].x - player.camera.x, tile[i][j].y - player.camera.y, 80, 80);
                wall.render(ren, renderQuad);
            } else {
                //floor.setDst(tile[i][j].x - player.camera.x, tile[i][j].y - player.camera.y, 80, 80);
                floor.render(ren, renderQuad);
            }
        }
    }
}


void Map::scrollMap(Player& player, SDL_Event& e) {
    if (player.dst.x > 1280/2) {
        offSetX++;
    }
    if (offSetX > 16) {
        offSetX = 16;
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

//void Map::renderMap(SDL_Renderer* ren) {
  //  for (int i = 0; i < 9; i++) {
    //    for (int j = 0; j < 16; j++) {
      //      SDL_RenderCopy()
        //}
    //}
//}
