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
			map += "################################";
			map += "#..............................#";
			map += "#..............................#";
			map += "#.......########################";
			map += "#..............................#";
			map += "#..............................#";
			map += "#..............#################";
			map += "#...#####.........#....#.......#";
			map += "#...#...###.......#....#.......#";
			map += "#...#...#.##......#....#.......#";
			map += "#...#...#..##.....#....#.......#";
			map += "#...#...#...##....#....#.......#";
			map += "#...#...#....##........#.......#";
			map += "#.......#.....##.......#####...#";
			map += "#.......#......##..............#";
			map += "#########.......##.............#";
			map += "#................#.....#########";
			map += "#..............................#";
			map += "#......############### ........#";
			map += "#..............................#";
			map += "#......#.............#######...#";
			map += "#......#...#######...#....#....#";
			map += "#......#...#.....#...#...###...#";
			map += "#......#...#.....#...#..##.....#";
			map += "#......#...#.....#...#...##....#";
			map += "#......#...#.....#...#....##...#";
			map += "#......#...#.....#...#.....#...#";
			map += "#......#...#.....#...#.....#...#";
			map += "#......#.............#.....#...#";
			map += "#......#...#######...#..####...#";
			map += "#..............................#";
			map += "################################";

			SDL_Renderer *renderer = get_renderer();
			tex_ceil = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 1, 256);
			tex_floor = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 1, 256);
			tex_wall[0] = loadTexture("res/wall_1.bmp");
			tex_wall[1] = loadTexture("res/wall_2.bmp");
			tex_wall[2] = loadTexture("res/wall_3.bmp");
			tex_wall[3] = loadTexture("res/wall_4.bmp");
			tex_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, map_w, map_h);
			if (!tex_ceil || !tex_floor || !tex_wall[0] || !tex_wall[1] || !tex_wall[2] || !tex_wall[3] || !tex_map)
			{
				cerr << "Failed to create textures.\n";
				return false;
			}

			unsigned int data_ceil[256];
			unsigned int data_floor[256];
			auto _to_rgba = [](float r, float g, float b)
			{
				return (static_cast<int>(255 * r) & 0xff) << 24 |
				(static_cast<int>(255 * g) & 0xff) << 16 |
				(static_cast<int>(255 * b) & 0xff) << 8 | 0xff;
			};
			for (unsigned int y=0; y<256; y++)
			{
				float inv_f = static_cast<float>(255-y)/255.0f;
				data_ceil[y] = _to_rgba(0.43f * inv_f, 0.71f * inv_f, 0.71f * inv_f);

				float f = static_cast<float>(y)/255.0f;
				data_floor[y] = _to_rgba(0.78f * f, 0.39f * f, 0.20f * f);
			}

			unsigned int data_map[map_h * map_w];
			for (int y=0; y<map_h; y++)
			{
				for (int x=0; x<map_w; x++)
				{
					if (get_map(x, y) != '#')
						data_map[(map_h - y - 1) * map_w + x] = _to_rgba(0.8f, 0.8f, 1.0f);
					else
						data_map[(map_h - y - 1) * map_w + x] = _to_rgba(0.0f, 0.0f, 0.0f);
				}
			}

			SDL_Rect rect = { 0, 0, 1, 256 };
			SDL_Rect rect_map = { 0, 0, map_w, map_h };
			if (SDL_UpdateTexture(tex_ceil, &rect, data_ceil, sizeof(unsigned int)) ||
				SDL_UpdateTexture(tex_floor, &rect, data_floor, sizeof(unsigned int)) ||
				SDL_UpdateTexture(tex_map, &rect_map, data_map, sizeof(unsigned int) * map_w))
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
				player_a -= player_rot * elapsed_time;
			}
			if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
			{
				player_a += player_rot * elapsed_time;
			}
			if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
			{
				float tmp_x = player_x + sinf(player_a) * player_mov * elapsed_time;
				float tmp_y = player_y + cosf(player_a) * player_mov * elapsed_time;

				if (get_map(static_cast<int>(tmp_x), static_cast<int>(tmp_y)) != '#')
				{
					player_x = tmp_x;
					player_y = tmp_y;
				}
			}
			if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
			{
				float tmp_x = player_x - sinf(player_a) * player_mov * elapsed_time;
				float tmp_y = player_y - cosf(player_a) * player_mov * elapsed_time;

				if (get_map(static_cast<int>(tmp_x), static_cast<int>(tmp_y)) != '#')
				{
					player_x = tmp_x;
					player_y = tmp_y;
				}
			}
			if (((state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) &&
				 (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])) ||
				(state[SDL_SCANCODE_Q]))
			{
                float tmp_x = player_x - cosf(player_a) * player_mov * elapsed_time;
				float tmp_y = player_y + sinf(player_a) * player_mov * elapsed_time;

				if (get_map(static_cast<int>(tmp_x), static_cast<int>(tmp_y)) != '#')
				{
					player_x = tmp_x;
					player_y = tmp_y;
				}
			}
			if (((state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) &&
				 (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])) ||
				(state[SDL_SCANCODE_E]))
			{
                float tmp_x = player_x + cosf(player_a) * player_mov * elapsed_time;
				float tmp_y = player_y - sinf(player_a) * player_mov * elapsed_time;

				if (get_map(static_cast<int>(tmp_x), static_cast<int>(tmp_y)) != '#')
				{
					player_x = tmp_x;
					player_y = tmp_y;
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
				float ray_angle = (player_a - fov / 2.0f) + (static_cast<float>(x)  / static_cast<float>(width())) * fov;
				float distance_to_wall = 0.0f;
				bool hitwall = false;

				float eyex = sinf(ray_angle);
				float eyey = cosf(ray_angle);

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

				int ceiling = static_cast<float>(height() / 2.0f) - height() / distance_to_wall;
				int floor = height() - ceiling;
				
				int shade = 255.0f * sample_x;
				
				// Wall
				SDL_Rect rect_src = { shade, 0, 1, 256 };
				SDL_Rect rect_dst = { x, ceiling, 1, floor - ceiling };
				if (distance_to_wall < 8.0f)
					SDL_RenderCopy(renderer, tex_wall[0], &rect_src, &rect_dst);
				else if (distance_to_wall < 12.0f)
					SDL_RenderCopy(renderer, tex_wall[1], &rect_src, &rect_dst);
				else if (distance_to_wall < 14.0f)
					SDL_RenderCopy(renderer, tex_wall[2], &rect_src, &rect_dst);
				else
					SDL_RenderCopy(renderer, tex_wall[3], &rect_src, &rect_dst);
			}

			// Draw map on top left
			SDL_Rect rect_map = { 0, 0, map_w, map_h };
			SDL_Rect rect_dst = { 0, 0, map_w * 3, map_h * 3 };
			SDL_RenderCopy(renderer, tex_map, &rect_map, &rect_dst);

			SDL_SetRenderDrawColor(renderer, 0xff, 0x3f, 0x3f, 0xff);
			int pos_x = static_cast<int>(player_x) * 3;
			int pos_y = (map_h - 1) * 3 - static_cast<int>(player_y) * 3;
			SDL_Rect rect = { pos_x, pos_y, 3, 3 };
			SDL_RenderFillRect(renderer, &rect);

			SDL_RenderPresent(renderer);
			
			return true;
		}

	virtual void OnDestroy()
	{
		SDL_DestroyTexture(tex_ceil);
		SDL_DestroyTexture(tex_floor);
		for (int i=0; i<4; i++)
			SDL_DestroyTexture(tex_wall[i]);
		SDL_DestroyTexture(tex_map);
	}

private:
	SDL_Texture* loadTexture(const char *filename)
	{
			SDL_Renderer *renderer = get_renderer();
			SDL_Surface *surf = SDL_LoadBMP(filename);
			if (!surf)
			{
				cerr << "Failed to load texture: " << filename << " : " <<  SDL_GetError() << endl;
				return NULL;
			}
			SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
			if (!tex)
			{
				cerr << "Failed to create texture from surface: " << filename << " : " <<  SDL_GetError() << endl;
				return NULL;
			}
			SDL_FreeSurface(surf);
			return tex;
	}

	char get_map(int x, int y) const { return map.c_str()[(map_h - y - 1) * map_w + x]; }

	string map;
	int map_w = 32;
	int map_h = 32;
	float depth = 32.0f;

	float player_x = 10.0f;
	float player_y = 10.0f;
	float player_a = 0.0f;

	float base_fov = 2.5f;
	float fov = base_fov / 3.14159f;

	float player_rot = 1.25f;
	float player_mov = 5.0f;

	SDL_Texture* tex_ceil;
	SDL_Texture* tex_floor;
	SDL_Texture* tex_wall[4];
	SDL_Texture* tex_map;
};

#endif
