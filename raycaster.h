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
			map += "#####..#####...#";
			map += "#......#.......#";
			map += "#......#..##...#";
			map += "#......#.......#";
			map += "#..............#";
			map += "######...###...#";
			map += "#....#.........#";
			map += "#....#.........#";
			map += "#....#....#....#";
			map += "#....#...#.....#";
			map += "#.....#.#......#";
			map += "#......#.......#";
			map += "#..............#";
			map += "################";

			return true;
		}

	virtual bool OnUpdate(float elapsed_time)
		{
			const Uint8 *state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
			{
				player_a -= (0.75f * elapsed_time);
			}
			if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
			{
				player_a += (0.75f * elapsed_time);
			}
			if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
			{
				player_x += sinf(player_a) * 5.0f * elapsed_time;
				player_y += cosf(player_a) * 5.0f * elapsed_time;

				int x = static_cast<int>(player_x);
				int y = static_cast<int>(player_y);
				if (get_map(x, y) == '#')
				{
					player_x -= sinf(player_a) * 5.0f * elapsed_time;
					player_y -= cosf(player_a) * 5.0f * elapsed_time;
				}
			}
			if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
			{
				player_x -= sinf(player_a) * 5.0f * elapsed_time;
				player_y -= cosf(player_a) * 5.0f * elapsed_time;

				int x = static_cast<int>(player_x);
				int y = static_cast<int>(player_y);
				if (get_map(x, y) == '#')
				{
					player_x += sinf(player_a) * 5.0f * elapsed_time;
					player_y += cosf(player_a) * 5.0f * elapsed_time;
				}
			}

			SDL_Renderer *renderer = get_renderer();
			
			for (int x=0; x<width(); x++)
			{
				float rayAngle = (player_a - fov / (base_fov / 2.0f)) + (static_cast<float>(x)  / static_cast<float>(width())) * fov;
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
						if (get_map(test_x, test_y) == '#')
						{
							hitwall = true;
						}
					}
				}

				int ceiling = static_cast<float>(height() / (base_fov / 2.0f)) - height() / distance_to_wall;
				int floor = height() - ceiling;
				
				int shade = 255 - static_cast<int>(16.0 * (distance_to_wall < 15.0f ? distance_to_wall : 15.0f));

				// Ceiling
				for (int y=0; y<ceiling; y++)
				{
					int col = 255 - (y < 255 ? y : 255);
					SDL_SetRenderDrawColor(renderer, col, 0, 0, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawPoint(renderer, x, y);
				}

				// Wall
				SDL_SetRenderDrawColor(renderer, shade, shade, shade, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer, x, ceiling, x, floor);

				// Floor
				for (int y=floor; y<height(); y++)
				{
					int col = 255 - ((height() - y) < 255 ? height() - y : 255);
					SDL_SetRenderDrawColor(renderer, 0, 0, col, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}

			SDL_RenderPresent(renderer);
			
			return true;
		}

private:
	char get_map(int x, int y) const { return map.c_str()[(map_h - y - 1) * map_w + x]; }

	string map;
	int map_w = 16;
	int map_h = 16;
	float depth = 16.0f;

	float player_x = 8.0f;
	float player_y = 8.0f;
	float player_a = 0.0f;

	float base_fov = 4.0f;
	float fov = base_fov / 3.14159f;
};
