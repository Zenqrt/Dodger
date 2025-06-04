#include "game.h"
#include "render.h"

int map_coordinates(int w_coordinate, int w_size, int a_size)
{
    double distance = (w_size / 2.0) - (a_size / 2.0);
    return (int)(w_coordinate + distance);
}

void render_player(SDL_Renderer *renderer, Player *player, const int w_width, const int w_height)
{
    int x = map_coordinates(player->x, w_width, AREA_WIDTH);
    int y = map_coordinates(player->y, w_height, AREA_HEIGHT);

    int width = player->width;
    int height = player->height;

    SDL_FRect rect;
    rect.x = x - (width / 2);
    rect.y = y - (height / 2);
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
}

void render_enemy(SDL_Renderer *renderer, Enemy *enemy, const int w_width, const int w_height)
{
    int x = map_coordinates(enemy->x, w_width, AREA_WIDTH);
    int y = map_coordinates(enemy->y, w_height, AREA_HEIGHT);

    int width = enemy->width;
    int height = enemy->height;

    SDL_FRect rect;
    rect.x = x - (width / 2);
    rect.y = y - (height / 2);
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(renderer, enemy->r, enemy->g, enemy->b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
}

void render_bonus_food(SDL_Renderer *renderer, BonusFood *bonus_food, const int w_width, const int w_height)
{
    int x = map_coordinates(bonus_food->x, w_width, AREA_WIDTH);
    int y = map_coordinates(bonus_food->y, w_height, AREA_HEIGHT);

    int width = bonus_food->width;
    int height = bonus_food->height;

    SDL_FRect rect;
    rect.x = x - (width / 2);
    rect.y = y - (height / 2);
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(renderer, 70, 135, 240, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
}

void render_game_area(SDL_Renderer *renderer, const int width, const int height, const int w_width, const int w_height)
{
    // Render border only if the window size does not equal the game area
    if (w_width != width || w_height != height)
    {
        SDL_FRect area_rect;
        area_rect.w = width;
        area_rect.h = height;
        area_rect.x = (w_width / 2) - (width / 2);
        area_rect.y = (w_height / 2) - (height / 2);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderRect(renderer, &area_rect);
    }
}

void render_score(SDL_Renderer *renderer, int score)
{
    SDL_SetRenderScale(renderer, 3, 3);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugTextFormat(renderer, 5, 10, "Score: %i", score);
    SDL_SetRenderScale(renderer, 1, 1);
}

float get_center_text_x(float x_scale, const int w_width, char *arr)
{
    float size = (w_width - (SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * x_scale * strlen(arr))) / 2.0;
    return size / x_scale;
}

float get_center_text_y(float y_scale, const int w_height, int lines)
{
    return (float)((w_height - (SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * y_scale * lines)) / 2) / y_scale;
}

void render_game_over(SDL_Renderer *renderer, int final_score, const int w_width, const int w_height)
{
    // Ik this is scuffed...
    float scale = 4;
    SDL_SetRenderScale(renderer, scale, scale);

    char game_over_text[] = "GAME OVER!";
    char final_score_text[] = "Final Score: ";
    char reset_text[] = "Press <Space> to restart";

    float ideal_y = get_center_text_y(scale, w_height, 6);

    SDL_SetRenderDrawColor(renderer, 214, 49, 49, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, get_center_text_x(scale, w_width, game_over_text), ideal_y, game_over_text);

    SDL_SetRenderDrawColor(renderer, 255, 249, 135, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugTextFormat(renderer, get_center_text_x(scale, w_width, final_score_text), ideal_y + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE + 10, "%s%i", final_score_text, final_score);

    SDL_SetRenderScale(renderer, 2, 2);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, get_center_text_x(2, w_width, reset_text), get_center_text_y(2, w_height, 1) + 50, reset_text);

    SDL_SetRenderScale(renderer, 1, 1);
}
