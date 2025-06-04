#include <SDL3/SDL.h>
#include "app.h"
#include "game.h"

Application::Application(SDL_Window *window)
{
    this->window = window;
    initialize_game();
}

void Application::update(SDL_Event event)
{
    enum GameState state = get_game_state();

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_F11 || event.key.scancode == SDL_SCANCODE_F)
        {
            Uint32 window_flags = SDL_GetWindowFlags(this->window);
            SDL_SetWindowFullscreen(this->window, !(window_flags & SDL_WINDOW_FULLSCREEN));
        }
    }

    if (state == ACTIVE)
    {
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            if (event.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                pause_game(true);
            }
            else
            {
                handle_player_inputs(event.key.scancode, true);
            }
        }

        if (event.type == SDL_EVENT_KEY_UP)
        {
            handle_player_inputs(event.key.scancode, false);
        }
    }
    else if (state == PAUSED)
    {
        if (event.type = SDL_EVENT_KEY_UP)
        {
            if (event.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                pause_game(false);
            }
        }
    }
    else if (state == ENDED)
    {
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_SPACE)
        {
            reset_game();
            start_game();
        }
    }
    else if (state == LIMBO)
    {
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_SPACE)
        {
            start_game();
        }
    }
}

void Application::fixed_update(float delta_time)
{
    enum GameState state = get_game_state();

    if (state != LIMBO)
    {
        update_movement(delta_time);
    }

    if (state == ACTIVE)
    {
        handle_spawning();
        handle_collisions();
    }
}

void Application::draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    render_game(renderer);

    SDL_RenderPresent(renderer);
}
