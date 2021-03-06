#include <chrono>
#include <random>
#include <thread>
#include "global.h" 
#include <vector>
std::vector<int> wormLoc;
const int lvlsz = 1600;
int size = 3;
char direction = 0;
int id[lvlsz];
int lifespan[lvlsz];
int idbuf[lvlsz];
int lifespanbuf[lvlsz];
PAINTSTRUCT verf;
RECT rect;
HBRUSH kwast;
int random(int x) {
    std::uniform_int_distribution<int> dist(0, x);
    int time = std::chrono::high_resolution_clock().now().time_since_epoch().count();
    std::default_random_engine rand(time);
    int randf = dist(rand);
    return randf;
}
HDC init_game(HWND app, int food) {
    memset(id, 0, lvlsz * 4);
    memset(lifespan, 0, lvlsz * 4);
    memset(idbuf, 0, lvlsz * 4);
    memset(lifespanbuf, 0, lvlsz * 4);
    id[food] = 3;
    for (int i = 0; i < wormLoc.size(); i++) {
        id[wormLoc[i]] = 1;
    }
    return BeginPaint(app, &verf);
}
void snake_mov(int itt, int dir) {
    int nwpcs = itt + dir;
    if (itt % 40 == 39 && nwpcs % 40 == 0) {
        return;
    }
    if (itt % 40 == 0 && nwpcs % 40 == 39) {
        return;
    }
    if (id[nwpcs] == 1 || id[nwpcs] == 2) {
        return;
    }
    if (id[nwpcs] == 3) {
        idbuf[new_food()] = 3;
        for (int i = 0; i < lvlsz; i++) {
            if (lifespan[i] == size) {
                lifespanbuf[i] = size + 1;
            }
        }
        size += 1;
        lifespanbuf[nwpcs] = size;
        idbuf[nwpcs] = 1;
        return;
    }
    idbuf[nwpcs] = 1;
    lifespanbuf[nwpcs] = size;
}
void the_game(HDC pixel) {
    memcpy(lifespanbuf, lifespan, lvlsz * 4);
    memcpy(idbuf, id, lvlsz * 4);
    newWorm();
    socket_data(direction);
    for (int i = 0; i < wormLoc.size(); i++) {
        switch (wormDir[i]) {
        case 0:
            snake_mov(wormLoc[i],1);
            wormLoc[i] += 1;
            break;
        case 1:
            snake_mov(wormLoc[i], -1);
            wormLoc[i] -= 1;
            break;
        case 2:
            snake_mov(wormLoc[i], 40);
            wormLoc[i] += 40;
            break;
        case 3:
            snake_mov(wormLoc[i], -40);
            wormLoc[i] -= 40;
            break;
        }
    }
    for (int i = 0; i < lvlsz; i++) {
        if (lifespan[i] > 0) {
            lifespanbuf[i] = lifespan[i] - 1;
            if (lifespanbuf[i] == 0) {
                idbuf[i] = 0;
            }
        }
    }
    memcpy(lifespan, lifespanbuf, lvlsz * 4);
    memcpy(id, idbuf, lvlsz * 4);
    rect = { 0,0,1600,1600 };
    FillRect(pixel, &rect, CreateSolidBrush(RGB(255, 255, 255)));
    for (int i = 0; i < lvlsz; i++) {
        if (id[i] != 0) {
            switch (id[i]) {
            case 1:
                kwast = CreateSolidBrush(RGB(0, 0, 0));
                break;
            case 2:
                kwast = CreateSolidBrush(RGB(0, 255, 0));
                break;
            case 3:
                kwast = CreateSolidBrush(RGB(255, 0, 0));
                break;
            }
            int offsety = i * 20 % 800;
            int offsetx = i / 40 * 20;
            rect = { offsety,offsetx ,offsety + 20,offsetx + 20 };
            FillRect(pixel, &rect, kwast);
        }
    }
    if (GetKeyState(0x44) < 0) {
        direction = 0;
    }
    if (GetKeyState(0x41) < 0) {
        direction = 1;
    }
    if (GetKeyState(0x53) < 0) {
        direction = 2;
    }
    if (GetKeyState(0x57) < 0) {
        direction = 3;
    }
    Sleep(100);
}
