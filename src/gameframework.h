#ifndef __GAMEFRAMEWORK_H__
#define __GAMEFRAMEWORK_H__

#include <exception>
#include <chrono>
#include <SDL2/SDL.h>

using namespace std;

class GameException : public std::exception
{
public:
	GameException(const char *m) : mesg(m) {}

	virtual const char* what() const throw()
		{
			return mesg;
		}

private:
	const char *mesg;
};

class GameFramework
{
public:
	GameFramework(int width, int height) : w(width), h(height)
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
			{
				throw GameException("Failed to init SDL");
			}

			window = SDL_CreateWindow( get_name(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
			if (!window)
			{
				throw GameException("Failed to create SDL window");
			}

			renderer = SDL_CreateRenderer(window, -1, 0);
			if (!renderer)
			{
				throw GameException("Failed to create SDL renderer");
			}
		}

	virtual bool OnCreate() = 0;
	virtual bool OnUpdate(float elapsed_time) = 0;
	virtual void OnDestroy() = 0;

	void start()
		{
			if (!OnCreate())
			{
				throw GameException("Failed to create game framework");
			}

			bool quit = false;
			SDL_Event e;

			auto tp1 = chrono::system_clock::now();
			auto tp2 = chrono::system_clock::now();

			while (!quit)
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if ( e.type == SDL_KEYUP )
					{
						if ( e.key.keysym.sym == SDLK_ESCAPE )
						{
							quit = true;
						}
					}
				}
				
				tp2 = chrono::system_clock::now();
				chrono::duration<float> elapsed_time = tp2 - tp1;
				tp1 = tp2;

				char title[256];
				snprintf(title, 256, "%8.2f fps - %s", 1.0 / elapsed_time.count(), get_name());
				SDL_SetWindowTitle(window, title);
				
				if (!OnUpdate(elapsed_time.count()))
				{
					throw GameException("Failed on game update");
				}
			}

			OnDestroy();
		}

	virtual ~GameFramework()
		{
			if (renderer)
			{
				SDL_DestroyRenderer(renderer);
			}
			if (window)
			{
				SDL_DestroyWindow(window);
			}

			SDL_Quit();
		}

protected:
	virtual const char* get_name() { return "GameFramework"; }
	
	SDL_Window* get_window() { return window; }
	SDL_Renderer* get_renderer() { return renderer; }
	int width() const { return w; }
	int height() const { return h; }

private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	int w = 0;
	int h = 0;
};

#endif
