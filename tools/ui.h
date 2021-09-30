#ifndef UI_H
#define UI_H

#if USE_SDL

#include <SDL.h>

#include "data_structures/clist.h"

#define UI_WIDTH 900
#define UI_HEIGHT 500

void ui_init();

int ui_run();

void ui_exit();

void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius);

#endif // !USE_SDL

#endif // !UI_H