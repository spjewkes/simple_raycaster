#ifndef __RAYCASTER_H__
#define __RAYCASTER_H__

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
			tex_wall = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 256, 256);
			if (!tex_ceil || !tex_floor || !tex_wall)
			{
				cerr << "Failed to create textures.\n";
				return false;
			}

			unsigned int data_ceil[256];
			unsigned int data_floor[256];
			unsigned int data_wall[65536];
			for (unsigned int y=0; y<256; y++)
			{
				data_ceil[y] = (255-y) << 8 | 0xff;
				data_floor[y] = y << 24 | 0xff;
				for (unsigned int x=0; x<256; x++)
				{
					if (y % 16 == 0)
					{
						data_wall[y * 256 + x] = 0;
					}
					else
					{
						data_wall[y * 256 + x] = x << 24 | x << 16 | y << 8 | 0xff;
					}
				}
			}

			SDL_Rect rect = { 0, 0, 1, 256 };
			SDL_Rect rect_wall = { 0, 0, 256, 256 };
			if (SDL_UpdateTexture(tex_ceil, &rect, data_ceil, sizeof(unsigned int)) ||
				SDL_UpdateTexture(tex_floor, &rect, data_floor, sizeof(unsigned int)) ||
				SDL_UpdateTexture(tex_wall, &rect_wall, data_wall, sizeof(unsigned int) * 256))
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
			if (((state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) &&
				 (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])) ||
				(state[SDL_SCANCODE_Q]))
			{
                player_x -= cosf(player_a) * 5.0f * elapsed_time;
				player_y += sinf(player_a) * 5.0f * elapsed_time;

				int x = static_cast<int>(player_x);
				int y = static_cast<int>(player_y);
				if (get_map(x, y) == '#')
				{
					player_x += cosf(player_a) * 5.0f * elapsed_time;
					player_y -= sinf(player_a) * 5.0f * elapsed_time;
				}
			}
			if (((state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) &&
				 (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])) ||
				(state[SDL_SCANCODE_E]))
			{
                player_x += cosf(player_a) * 5.0f * elapsed_time;
				player_y -= sinf(player_a) * 5.0f * elapsed_time;

				int x = static_cast<int>(player_x);
				int y = static_cast<int>(player_y);
				if (get_map(x, y) == '#')
				{
					player_x -= cosf(player_a) * 5.0f * elapsed_time;
					player_y += sinf(player_a) * 5.0f * elapsed_time;
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

				float sample_x = 0.0f;

				while (!hitwall && distance_to_wall < depth)
				{
					distance_to_wall += 0.01f;

					float test_x = player_x + eyex * distance_to_wall;
					float test_y = player_y + eyey * distance_to_wall;

					if (test_x < 0.0f || test_x >= static_cast<float>(map_w) ||
						test_y < 0.0f || test_y >= static_cast<float>(map_h))
					{
						hitwall = true;
						distance_to_wall = depth;
					}
					else
					{
						if (get_map(static_cast<int>(test_x), static_cast<int>(test_y)) == '#')
						{
							hitwall = true;

							// Determine where ray has hit the wall.
							float block_mid_x = std::floor(test_x) + 0.5f;
							float block_mid_y = std::floor(test_y) + 0.5f;

							float test_angle = std::atan2f(test_y - block_mid_y, test_x - block_mid_x);
							double intpart;
							if (test_angle >= -3.14195f * 0.25f && test_angle < 3.14195f * 0.25f)
							{
								sample_x = std::modf(test_y, &intpart);
							}
							else if (test_angle >= 3.14195f * 0.25f && test_angle < 3.14195f * 0.75f)
							{
								sample_x = std::modf(test_x, &intpart);
							}
							else if (test_angle >= -3.14195f * 0.75f && test_angle < -3.14195f * 0.25f)
							{
								sample_x = std::modf(test_x, &intpart);
							}
							else if (test_angle >= 3.14195f * 0.75f || test_angle < -3.14195f * 0.75f)
							{
								sample_x = std::modf(test_y, &intpart);
							}
						}
					}
				}

				int ceiling = static_cast<float>(height() / (base_fov / 2.0f)) - height() / distance_to_wall;
				int floor = height() - ceiling;
				
				int shade = 255.0f * sample_x;
				
				// Wall
				SDL_Rect rect_src = { shade, 0, 1, 256 };
				SDL_Rect rect_dst = { x, ceiling, 1, floor - ceiling };
				SDL_RenderCopy(renderer, tex_wall, &rect_src, &rect_dst);
			}

			SDL_RenderPresent(renderer);
			
			return true;
		}

	virtual void OnDestroy()
	{
		SDL_DestroyTexture(tex_ceil);
		SDL_DestroyTexture(tex_floor);
		SDL_DestroyTexture(tex_wall);
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

#endif
