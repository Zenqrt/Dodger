#include <SDL3/SDL.h>

class Entity
{
public:
    double x;
    double y;
    int health;
    int width;
    int height;
};

class Enemy : public Entity
{
public:
    double x_speed;
    double y_speed;
    int r, g, b;
};

class BonusFood : public Entity
{
};

class Player : public Entity
{
public:
    bool forward;
    bool backward;
    bool right;
    bool left;

    void shoot(double x_dir, double y_dir);
    void update_movement();
    void handle_inputs(SDL_Scancode scancode, bool pressed);
};