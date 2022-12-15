#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"

#define WHITE {255, 255, 255, 255}
#define BLACK {0, 0, 0, 255}

enum {SDLFAIL, TTFFAIL};

class Gui {
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font *font;

		Game* game;

		int mapWidth;
		int mapHeight;

	public:
		Gui(Game* _game, const int width, const int height);
		~Gui(void);
		void prepareScene(const SDL_Color bg);
		void presentScene(void);

		void fillRect(const SDL_Rect rect, const SDL_Color c);
		void outlineRect(const SDL_Rect rect, const SDL_Color c);
		void drawText(const std::string text, SDL_Rect rect, SDL_Color color);

		void drawStartup(void);
		void drawGame(void);

		void events(void);
};

class guiException : public std::exception{
	private:
		std::string msg;
		int type;

	public:
		guiException(const std::string _msg, const int _type): msg(_msg), type(_type){}
		int getType(){
			return type;
		}

	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
};
