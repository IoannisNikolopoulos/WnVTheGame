#include "gui.h"
#include <SDL2/SDL_events.h>

/*
	CONSTRUCTOR - DESTRUCTOR
*/

Gui::Gui(Game* _game, const int width, const int height): game(_game){
	/*
	Initialise SDL components (window, renderer) + Font loading
	*/
	window = nullptr;

	Uint32 windowFlags = SDL_WINDOW_SHOWN;
	Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		throw guiException("SDL could not initialize!", SDLFAIL);
	}

	if(TTF_Init() == -1){
		throw guiException("SDL_ttf could not initialize!", TTFFAIL);
	}

	font = TTF_OpenFont("res/arial.ttf", 50);

	if(font == NULL) {
		throw guiException("Could not open font!", TTFFAIL);
	}

	if(!(window = SDL_CreateWindow("Chess Engine",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					width,
					height,
					windowFlags))
	) throw guiException("Window could not be created!", 0);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	if(!(renderer = SDL_CreateRenderer(window, -1, rendererFlags)))
		throw guiException("Failed to create renderer!", SDLFAIL);

	mapWidth = game->getWidth();
	mapHeight = game->getHeight();

	prepareScene(BLACK);
	presentScene();
}

Gui::~Gui(void){
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}


/*
	RENDERING TO THE SCREEN
*/

void Gui::prepareScene(SDL_Color bg){
	/*
	Clear screen and set backgroud color
	*/
	SDL_SetRenderDrawColor(renderer, bg.r, bg.b, bg.g, bg.a);
	SDL_RenderClear(renderer);
}

void Gui::presentScene(void){
	SDL_RenderPresent(renderer);
}

void Gui::fillRect(SDL_Rect rect, SDL_Color c){
	/*
	Fill the area defined by rect with color c
	*/
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(renderer, &rect);
}

void Gui::outlineRect(SDL_Rect rect, SDL_Color c){
	/*
	Outline the area defined by rect with color c
	*/
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawRect(renderer, &rect);
}

void Gui::drawText(const std::string text, SDL_Rect rect, SDL_Color color){
	/*
		Render the text with color on a new surface, create a texture from the surface
		and render to the screen at the area defined by rect
	*/
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect distrect = {rect.x + rect.w/2, rect.y + rect.h/2, 4*rect.w/5, 4*rect.h/5};
	distrect.x -= distrect.w/2;
	distrect.y -= distrect.h/2;
	SDL_RenderCopy(renderer, texture, NULL, &distrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

/*
	DRAW MAP
*/

/* SDL_Color earthColor = {112, 46, 15}; */
SDL_Color earthColor = {79, 42, 25};
SDL_Color waterColor = {19, 124, 181};
SDL_Color treeColor = {3, 150, 47};

SDL_Color vampireColor = {255, 0, 0};
SDL_Color werewolfColor = {145, 5, 255};
SDL_Color playerColor = {0, 255, 0};

void Gui::drawGame(void){
	std::unordered_map<Point, Object*> objects = game->getObjects();

	int windowSize[2];
	SDL_GetWindowSize(window, &windowSize[0], &windowSize[1]);

	int xDivisions = windowSize[0] / mapWidth;
	int yDivisions = windowSize[1] / mapHeight;

	int squareSize = std::min(xDivisions, yDivisions);

	int xGap = floor(windowSize[0] - mapWidth*squareSize);
	int yGap = floor(windowSize[1] - mapHeight*squareSize);

	int xOffset = xGap/2;
	int yOffset = yGap/2;

	for(int y=0; y<mapHeight; y++){
		for(int x=0; x<mapWidth; x++){
			Point position = Point(x, y);

			SDL_Rect squareRect = {
				x*squareSize + xOffset,
				y*squareSize + yOffset,
				squareSize,
				squareSize
			};

			SDL_Color color = earthColor;
			Object* o = nullptr;
			try {
				o = objects.at(position);

				if (typeid(*o) == typeid(Tree)) color = treeColor;
				else if (typeid(*o) == typeid(Water)) color = waterColor;
			}
			catch (std::exception &e) {}

			fillRect(squareRect, color);

			if (o == nullptr) continue;

			Entity* e = dynamic_cast<Entity*>(o);

			if (e == nullptr) continue;

			SDL_Rect objectRect = {
				(int) (squareRect.x + 0.1*squareRect.w),
				(int) (squareRect.y + 0.1*squareRect.h),
				(int) (0.8 * squareRect.w),
				(int) (0.8 * squareRect.h)
			};

			std::string text = "";
			if (typeid(*e) == typeid(Werewolf)) {
				color = werewolfColor;
				text = "W";
			}
			else if (typeid(*e) == typeid(Vampire)) {
				color = vampireColor;
				text = "V";
			}
			else if (typeid(*e) == typeid(Player)) {
				color = playerColor;
				text = game->getPlayerTeam().substr(0, 1);
			}

			drawText(text, objectRect, color);
		}
	}
}

/*
	INPUT HANDLING
*/

void Gui::events(void){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				RUNNING = false;
				break;
			case SDL_KEYDOWN:
				SDL_Keycode key = event.key.keysym.sym;
				switch (key) {
					case SDLK_UP:
						game->movePlayer(NORTH);
						break;
					case SDLK_DOWN:
						game->movePlayer(SOUTH);
						break;
					case SDLK_LEFT:
						game->movePlayer(WEST);
						break;
					case SDLK_RIGHT:
						game->movePlayer(EAST);
						break;
					case SDLK_SPACE:
						PAUSED = !PAUSED;
						break;
					default:
						break;
				}
				break;
		}
	}
}
