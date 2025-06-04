#include <iostream>
#include <bits/stdc++.h>
#include <SDL3/SDL.h>
#include "app.h"
#include "game.h"
#include "sounds.h"

using namespace std;

const float TICK_RATE = 1.0f / 140.0f;

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_CreateWindowAndRenderer("Dodger", AREA_WIDTH + 200, AREA_HEIGHT + 200, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_SetWindowMinimumSize(window, AREA_WIDTH, AREA_HEIGHT);

    initialize_sounds();
    Application app(window);

    SDL_Event event;
    float accumulator = 0.0f;
    float last_tick = SDL_GetTicks() / 1000.0f;

    while (true)
    {
        if (SDL_PollEvent(&event))
        {
            if (SDL_EVENT_QUIT == event.type)
            {
                break;
            }
        }

        float current_tick = SDL_GetTicks() / 1000.0f;
        float frame_time = current_tick - last_tick;
        last_tick = current_tick;

        accumulator += frame_time;

        while (accumulator >= TICK_RATE)
        {
            app.fixed_update(TICK_RATE);
            accumulator -= TICK_RATE;
        }

        app.update(event);
        app.draw(renderer);
    }

    SDL_RenderClear(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}