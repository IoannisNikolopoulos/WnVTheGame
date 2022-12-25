#include "game.h"
#include "gui.h"

int getWidth() {
	/*
	   Get board width from user
	*/

	int width;

	clearScreen();
	std::cout << "Give width: ";
	std::cin >> width;

	return width;
}

int getHeight() {
	/*
	   Get board height from user
	*/

	int height;

	clearScreen();
	std::cout << "Give height: ";
	std::cin >> height;

	return height;
}

std::string getTeam() {
	/*
	   Get player team from user
	*/

	std::string team;

	do {
		clearScreen();
		std::cout << "Select team [Werewolves/Vampires]: ";
		std::cin >> team;
	} while (!(team == "Werewolves" || team == "Vampires"));

	return team;
}


SDL_Color bg = BLACK;

int main(int argc, char* argv[]) {
	srand(time(NULL)); // SRAND IS USED TO MAKE SURE GENERATION OF GAME IS RANDOM

	int width = 16;
	int height = 9;
	std::string playerTeam = "Werewolves";

	width = getWidth();
	height = getHeight();
	playerTeam = getTeam();

	Game* game = new Game(width, height, playerTeam); // GAME CLASS CONTAINS ALL MECHANISMS 

	Gui* gui = nullptr; // STARTS AS NULL DUE TO PRESENTLY UNDEFINED NATURE
	try {
		gui = new Gui(game, 856, 480);
	} catch (guiException& e) {
		std::cout << e.what();

		switch (e.getType()) { // CHECK FOR ERRORS DURING GUI CREATION
			case SDLFAIL:
				std::cout << " SDL_Error: " << SDL_GetError();
				break;
			case TTFFAIL:
				std::cout << " TTF_Error: " << TTF_GetError();
				break;
		}

		std::cout << std::endl;
		return EXIT_FAILURE;
	}

	while (RUNNING) { // GAME RUNS AS LONG AS WHILE IS RUNNING
		gui->events();

		if (!PAUSED) {
			gui->prepareScene(bg); // RENDERS BLACK BACKGROUND

			int result = game->update(); // UPDATES ONE TICK OF GAME PER SECOND
											// RESULT CONCLUDES GAME DATA, DECIDING WINNER
			if (result == WEREWOLVES) {
				std::cout << "Werewolves win" << std::endl;
				RUNNING = false;
			} else if (result == VAMPIRES) {
				std::cout << "Vampires win" << std::endl;
				RUNNING = false;
			}

			gui->drawGame(); // RESPONSIBLE FOR PRINTING MAP AND ENTITIES ON SCREEN

			gui->presentScene(); // SAME AS DRAW GAME
		}

		SDL_Delay(80);
	}

	return EXIT_SUCCESS;
}
