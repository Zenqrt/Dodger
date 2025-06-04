#include <SDL3/SDL.h>

const int AREA_WIDTH = 600;
const int AREA_HEIGHT = 600;

enum GameState
{
    LIMBO = 0,
    ACTIVE = 1,
    PAUSED = 2,
    ENDED = 3,
};

void initialize_game();
void start_game();
void pause_game(bool paused);
void reset_game();
void handle_collisions();
void handle_spawning();
void handle_player_inputs(SDL_Scancode scancode, bool pressed);
void update_movement(float delta_time);
void render_game(SDL_Renderer *renderer);

GameState get_game_state();
