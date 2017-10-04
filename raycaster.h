#include <SDL2/SDL.h>
#include <string>
#include <cmath>
#include <iostream>
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
			map += "#....#.....#...#";
			map += "#....#.....#...#";
			map += "#....#...###...#";
			map += "#....##.##.....#";
			map += "#.....#.#......#";
			map += "#.....###......#";
			map += "#..............#";
			map += "################";

			SDL_Renderer *renderer = get_renderer();
			tex_ceil = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 1, 256);
			tex_floor = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 1, 256);
			if (!tex_ceil || !tex_floor)
			{
				cerr << "Failed to create textures.\n";
				return false;
			}

			unsigned int data_ceil[256];
			unsigned int data_floor[256];
			for (unsigned int i=0; i<256; i++)
			{
				data_ceil[i] = (255-i) << 8 | 0xff;
				data_floor[i] = i << 24 | 0xff;
			}

			SDL_Rect rect = { 0, 0, 1, 256 };

			if (SDL_UpdateTexture(tex_ceil, &rect, data_ceil, sizeof(unsigned int)) ||
				SDL_UpdateTexture(tex_floor, &rect, data_floor, sizeof(unsigned int)))
			{
				cerr << "Failed to update textures.\n";
				return false;
			}

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
			SDL_Rect rect_src = { 0, 0, 1, 256 };
			SDL_Rect rect_ceil = { 0, 0, width(), height() / 2 };
			SDL_Rect rect_floor = { 0, height() / 2, width(), height() / 2 };

			// Clear screen by drawing ceiling and floor
			SDL_RenderCopy(renderer, tex_ceil, &rect_src, &rect_ceil);
			SDL_RenderCopy(renderer, tex_floor, &rect_src, &rect_floor);
			
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

				// Wall
				SDL_SetRenderDrawColor(renderer, shade, shade, shade, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer, x, ceiling, x, floor);
			}

			SDL_RenderPresent(renderer);
			
			return true;
		}

	virtual void OnDestroy()
	{
		SDL_DestroyTexture(tex_ceil);
		SDL_DestroyTexture(tex_floor);
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

	SDL_Texture* tex_ceil;
	SDL_Texture* tex_floor;
	SDL_Texture* tex_wall;
};
