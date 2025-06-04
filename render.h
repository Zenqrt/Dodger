#include <SDL3/SDL.h>
#include "entity.h"

void render_player(SDL_Renderer *renderer, Player *player, const int w_width, const int w_height);
void render_enemy(SDL_Renderer *renderer, Enemy *enemy, const int w_width, const int w_height);
void render_bonus_food(SDL_Renderer *renderer, BonusFood *bonus_food, const int w_width, const int w_height);
void render_game_area(SDL_Renderer *renderer, const int width, const int height, const int w_width, const int w_height);
void render_score(SDL_Renderer *renderer, int score);
void render_game_over(SDL_Renderer *renderer, int final_score, const int w_width, const int w_height);