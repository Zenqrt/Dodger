#include <SDL3/SDL.h>

class Application
{
private:
    SDL_Window *window;

public:
    Application(SDL_Window *window);
    void update(SDL_Event event);
    void fixed_update(float delta_time);
    void draw(SDL_Renderer *renderer);
};