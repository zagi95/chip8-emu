#include "display.h"
#include "SDL3/SDL.h"
#include <stdint.h>
#include "stdio.h"

#define WINDOW_TITLE "chip 8 emulator"
#define SCALE 10
#define WINDOW_WIDTH (64 * SCALE)
#define WINDOW_HEIGHT (64 * SCALE)

static SDL_Window   *window = NULL;
static SDL_Renderer *renderer = NULL;

void display_init()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                                &window, &renderer);
}
// TODO: sredi update i handle events na sdl3

void display_update(const uint8_t *framebuffer)
{
    SDL_FRect rect = {
        .w = SCALE,
        .h = SCALE
    };

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < 32; i++) {
        for(int j = 0; j < 64; j++) {
            int idx = i * 64 + j;
            if(framebuffer[idx] == 1) {
                printf("\nblablabla: %d, %d", j, i);
                rect.x = j * SCALE;
                rect.y = i * SCALE;
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderFillRect(renderer, &rect);
    
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

void display_close()
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

int display_handle_events(uint8_t *key)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT)
            return 0;
        if (ev.type == SDL_EVENT_KEY_DOWN ||
            ev.type == SDL_EVENT_KEY_UP) {
            int pressed = (ev.type == SDL_EVENT_KEY_DOWN);
            switch (ev.key.key) {
            case SDLK_1: key[0x1] = pressed; break;
            case SDLK_2: key[0x2] = pressed; break;
            case SDLK_3: key[0x3] = pressed; break;
            case SDLK_4: key[0xC] = pressed; break;
            case SDLK_Q: key[0x4] = pressed; break;
            case SDLK_W: key[0x5] = pressed; break;
            case SDLK_E: key[0x6] = pressed; break;
            case SDLK_R: key[0xD] = pressed; break;
            case SDLK_A: key[0x7] = pressed; break;
            case SDLK_S: key[0x8] = pressed; break;
            case SDLK_D: key[0x9] = pressed; break;
            case SDLK_F: key[0xE] = pressed; break;
            case SDLK_Y: key[0xA] = pressed; break;
            case SDLK_X: key[0x0] = pressed; break;
            case SDLK_C: key[0xB] = pressed; break;
            case SDLK_V: key[0xF] = pressed; break;
            default: break;
            }
        }
    }
    return 1;
}
