#pragma once

#include "base.h"
#include "entities.h"
#include "objects.h"

#include <unordered_map>

enum {DAY=true, NIGHT=false};
enum {WEREWOLVES=1, VAMPIRES=2};
enum {NORTH, SOUTH, EAST, WEST};

/*
   Game map class
*/
class Game {
	private:
		int width;
		int height;
		std::unordered_map<Point, Object*> map;

		bool timeOfDay = DAY;
		int werewolves = 0;
		int vampires = 0;

		Player* player;
		std::string playerTeam;

		Point getEmptyPos();
		bool isValidPosition(Point position);

	public:
		Game(int _width, int _height, std::string _playerTeam);

		std::vector<Point> getPossibleMoves(Point position);

		std::unordered_map<Point, Object*> getObjects();
		std::vector<Entity*> getEntities(Point position);

		void moveEntities();
		void interactEntities();

		void movePlayer(int direction);
		std::string getPlayerTeam() {return playerTeam;};

		int update();

		int getWidth() {return width;};
		int getHeight() {return height;};

		void print();
};
