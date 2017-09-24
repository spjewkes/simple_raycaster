#include <SDL2/SDL.h>
#include <string>
#include <cmath>
#include "gameframework.h"

using namespace std;

class GameRC : public GameFramework
{
public:
	using GameFramework::GameFramework;

	virtual bool OnCreate()
		{
			map += "################";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "#..............#";
			map += "################";

			return true;
		}
	
	virtual bool OnUpdate(float elapsed_time)
		{
			const Uint8 *state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
			{
				player_a -= (0.25f * elapsed_time);
			}
			if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
			{
				player_a += (0.25f * elapsed_time);
			}

			SDL_Renderer *renderer = get_renderer();
			
			for (int x=0; x<width(); x++)
			{
				float rayAngle = (player_a - fov / 2.0f) + (static_cast<float>(x)  / static_cast<float>(width())) * fov;
				float distance_to_wall = 0.0f;
				bool hitwall = false;

				float eyex = sinf(rayAngle);
				float eyey = cosf(rayAngle);

				while (!hitwall && distance_to_wall < depth)
				{
					distance_to_wall += 0.1f;

					int test_x = static_cast<int>(player_x + eyex * distance_to_wall);
					int test_y = static_cast<int>(player_y + eyey * distance_to_wall);

					if (test_x < 0 || test_x >= map_w || test_y < 0 || test_y >= map_h)
					{
						hitwall = true;
						distance_to_wall = depth;
					}
					else
					{
						if (map.c_str()[test_y * map_w + test_x] == '#')
						{
							hitwall = true;
						}
					}
				}

				int ceiling = static_cast<float>(height() / 2.0f) - height() / distance_to_wall;
				int floor = height() - ceiling;
				
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer, x, 0, x, ceiling);

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer, x, ceiling, x, floor);

				SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer, x, floor, x, height());
			}

			SDL_RenderPresent(renderer);
			
			return true;
		}

private:
	string map;
	int map_w = 16;
	int map_h = 16;
	float depth = 16.0f;

	float player_x = 8.0f;
	float player_y = 8.0f;
	float player_a = 0.0f;

	float fov = 4.0 / 3.14159;
};
