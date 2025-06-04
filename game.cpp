#include <vector>
#include <cstdlib>
#include <iostream>
#include "render.h"
#include "game.h"
#include "sounds.h"

using namespace std;

const int PLAYER_SIZE = 20;
const double PLAYER_SPEED = 300;

Player player;
vector<Enemy> enemies;
vector<BonusFood> bonus_food;
int next_spawn_tick;
int next_food_tick;
int score;
enum GameState state;

static bool is_in_bounds_x(const double x, const double width)
{
    return x - (width / 2) >= 0 && x + (width / 2) <= AREA_WIDTH;
}

static bool is_in_bounds_y(const double y, const double height)
{
    return y - (height / 2) >= 0 && y + (height / 2) <= AREA_HEIGHT;
}

static double get_distance(const double x1, const double y1, const double x2, const double y2)
{
    return SDL_sqrt(
        SDL_pow(SDL_abs(x2 - x1), 2) + SDL_pow(SDL_abs(y2 - y1), 2));
}

static double bound(const double d, const double min, const double max)
{
    if (d < min)
    {
        return min;
    }
    else if (d > max)
    {
        return max;
    }
    else
    {
        return d;
    }
}

void initialize_game()
{
    player.x = AREA_WIDTH / 2;
    player.y = AREA_HEIGHT / 2;
    player.width = 20;
    player.height = 20;
    player.health = 1;

    next_spawn_tick = 0;
    next_food_tick = 0;
    score = 0;

    state = LIMBO;
}

void start_game()
{
    state = ACTIVE;
    next_food_tick = SDL_GetTicks() + 16000;

    play_music();
}

void pause_game(bool paused)
{
    if (paused)
    {
        state = PAUSED;
    }
    else
    {
        state = ACTIVE;
    }
}

void reset_game()
{
    next_spawn_tick = 0;
    score = 0;

    if (enemies.size() != 0)
    {
        enemies.clear();
    }

    if (bonus_food.size() != 0)
    {
        bonus_food.clear();
    }

    player.x = AREA_WIDTH / 2;
    player.y = AREA_HEIGHT / 2;
    player.health = 1;
}

void end_game()
{
    state = ENDED;

    player.forward = false;
    player.backward = false;
    player.left = false;
    player.right = false;

    play_sfx(DEATH);
    stop_music();
}

static bool is_colliding(Player player, Entity entity)
{
    double p_radius = player.width / 2;
    double e_radius = entity.width / 2;

    double distance = get_distance(player.x, player.y, entity.x, entity.y);

    return distance < p_radius + e_radius;
}

static bool is_colliding(Entity entity, int p_lower_x, int p_higher_x, int p_lower_y, int p_higher_y)
{
    int e_lower_x = entity.x - (entity.width / 2);
    int e_higher_x = entity.x + (entity.width / 2);
    int e_lower_y = entity.y - (entity.height / 2);
    int e_higher_y = entity.y + (entity.height / 2);

    if ((p_lower_x > e_lower_x && p_lower_x < e_higher_x) || (p_higher_x > e_lower_x && p_higher_x < e_higher_x))
    {
        if ((p_lower_y > e_lower_y && p_lower_y < e_higher_y) || (p_higher_y > e_lower_y && p_higher_y < e_higher_y))
        {
            return true;
        }
    }

    return false;
}

void handle_collisions()
{
    int p_lower_x = player.x - (player.width / 2);
    int p_higher_x = player.x + (player.width / 2);
    int p_lower_y = player.y - (player.height / 2);
    int p_higher_y = player.y + (player.height / 2);

    for (Enemy enemy : enemies)
    {
        int e_lower_x = enemy.x - (enemy.width / 2);
        int e_higher_x = enemy.x + (enemy.width / 2);
        int e_lower_y = enemy.y - (enemy.height / 2);
        int e_higher_y = enemy.y + (enemy.height / 2);

        // if (is_colliding(enemy, p_lower_x, p_higher_x, p_lower_y, p_higher_y))
        if (is_colliding(player, enemy))
        {
            player.health--;

            if (player.health <= 0)
            {
                end_game();
                return;
            }
            break;
        }
    }

    for (int i = 0; i < bonus_food.size(); i++)
    {
        BonusFood bonus = bonus_food[i];

        // if (is_colliding(bonus, p_lower_x, p_higher_x, p_lower_y, p_higher_y))
        if (is_colliding(player, bonus))
        {
            score += 10;
            play_sfx(BONUS);

            bonus_food.erase(bonus_food.begin() + i);
        }
    }
}

void handle_spawning()
{
    Uint64 tick = SDL_GetTicks();

    if (tick >= next_spawn_tick)
    {
        score += enemies.size();

        Enemy enemy;
        enemy.width = 10;
        enemy.height = 10;
        enemy.x_speed = ((rand() % 501) + 500) / 4.0;
        enemy.y_speed = ((rand() % 501) + 500) / 4.0;
        enemy.r = 100;
        enemy.g = 100;
        enemy.b = 100;

        do
        {
            enemy.x = (rand() % (AREA_WIDTH - 2 * enemy.width - 20)) + enemy.width + 10;
            enemy.y = (rand() % (AREA_HEIGHT - 2 * enemy.height - 20)) + enemy.height + 10;

        } while (get_distance(enemy.x, enemy.y, player.x, player.y) < 250);

        enemies.push_back(enemy);

        next_spawn_tick = tick + 5000;
    }

    if (tick >= next_food_tick)
    {
        BonusFood bonus;
        bonus.width = 15;
        bonus.height = 15;
        bonus.x = (rand() % (AREA_WIDTH - 2 * bonus.width - 20)) + bonus.width + 10;
        bonus.y = (rand() % (AREA_HEIGHT - 2 * bonus.height - 20)) + bonus.height + 10;

        bonus_food.push_back(bonus);
        next_food_tick = tick + 16000;
    }
    else if (next_food_tick - tick <= 8000 && bonus_food.size() != 0)
    {
        bonus_food.clear();
    }
}

void handle_player_inputs(SDL_Scancode scancode, bool pressed)
{
    if (scancode == SDL_SCANCODE_W || scancode == SDL_SCANCODE_UP)
    {
        player.forward = pressed;
    }

    if (scancode == SDL_SCANCODE_A || scancode == SDL_SCANCODE_LEFT)
    {
        player.left = pressed;
    }

    if (scancode == SDL_SCANCODE_S || scancode == SDL_SCANCODE_DOWN)
    {
        player.backward = pressed;
    }

    if (scancode == SDL_SCANCODE_D || scancode == SDL_SCANCODE_RIGHT)
    {
        player.right = pressed;
    }
}

void update_player_movement(float delta_time)
{
    double dx = 0;
    double dy = 0;

    if (player.forward)
    {
        dy = -PLAYER_SPEED;
    }

    if (player.backward)
    {
        dy = PLAYER_SPEED;
    }

    if (player.left)
    {
        dx = -PLAYER_SPEED;
    }

    if (player.right)
    {
        dx = PLAYER_SPEED;
    }

    const int x_max = AREA_WIDTH - (player.width / 2);
    const int x_min = player.width / 2;
    const int y_max = AREA_HEIGHT - (player.height / 2);
    const int y_min = player.height / 2;

    if (dx != 0)
    {
        player.x = bound(player.x + dx * delta_time, x_min, x_max);
    }

    if (dy != 0)
    {
        player.y = bound(player.y + dy * delta_time, y_min, y_max);
    }
}

void update_enemy_movement(float delta_time)
{
    for (Enemy &enemy : enemies)
    {
        double x_updated = enemy.x + enemy.x_speed * delta_time;
        double y_updated = enemy.y + enemy.y_speed * delta_time;

        if (is_in_bounds_x(x_updated, enemy.width))
        {
            enemy.x = x_updated;
        }
        else
        {
            enemy.x_speed = -enemy.x_speed;
            enemy.x += enemy.x_speed * delta_time;

            play_sfx(WALL_HIT);
        }

        if (is_in_bounds_y(y_updated, enemy.height))
        {
            enemy.y = y_updated;
        }
        else
        {
            enemy.y_speed = -enemy.y_speed;
            enemy.y += enemy.y_speed * delta_time;

            play_sfx(WALL_HIT);
        }
    }
}

void update_movement(float delta_time)
{
    update_player_movement(delta_time);
    update_enemy_movement(delta_time);
}

void render_game(SDL_Renderer *renderer)
{
    int w_width, w_height;

    if (!SDL_GetCurrentRenderOutputSize(renderer, &w_width, &w_height))
    {
        SDL_LogError(1, "Failed to find window size");
    }

    for (Enemy enemy : enemies)
    {
        render_enemy(renderer, &enemy, w_width, w_height);
    }

    for (BonusFood bonus : bonus_food)
    {
        render_bonus_food(renderer, &bonus, w_width, w_height);
    }

    render_player(renderer, &player, w_width, w_height);
    render_game_area(renderer, AREA_WIDTH, AREA_HEIGHT, w_width, w_height);
    render_score(renderer, score);

    if (state == ENDED)
    {
        render_game_over(renderer, score, w_width, w_height);
    }
}

GameState get_game_state()
{
    return state;
}
