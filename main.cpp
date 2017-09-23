#include <SDL2/SDL.h>
#include "gameframework.h"

using namespace std;

class GameRC : public GameFramework
{
public:
	using GameFramework::GameFramework;

	virtual bool OnCreate()
		{
			return true;
		}
	
	virtual bool OnUpdate(float elapsed_time)
		{
			SDL_Renderer *renderer = get_renderer();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
			SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
			SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
			SDL_RenderPresent(renderer);
			
			return true;
		}
};
	
int main(int argc, char *argv[])
{
	GameRC game(800, 600);
	game.start();

    return 0;
}
