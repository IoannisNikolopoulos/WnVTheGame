#pragma once

#include "base.h"

/*
   Entity class
   Is the base for Werewolf, Vampire, Player
*/
class Entity: public Object {
	protected:
		int strength = 0;
		int health = 10;
		int armor = 0;
		int medKits = 0;

		std::string team;

	public:
		Entity() = default;
		virtual ~Entity() = default;

		Entity(Point _position);

		virtual Point move(std::vector<Point> moves) = 0;

		bool interact(Entity* other);
		bool damage(int amount);
		void heal();

		std::string getTeam() {return team;};
};


/*
   Werewolf class
   Based on Entity
*/
class Werewolf: public Entity {
	public:
		Werewolf(Point _position): Entity(_position) {
			team = "Werewolves";
		};
		void print();

		Point move(std::vector<Point> moves);
};

/*
   Vampire class
   Based on Entity
*/
class Vampire: public Entity {
	public:
		Vampire(Point _position): Entity(_position) {
			team = "Vampires";
		};
		void print();

		Point move(std::vector<Point> moves);
};

/*
   Player class
   Based on Entity
*/
class Player: public Entity {
	public:
		Player(Point _position, std::string _team): Entity(_position) {
			team = _team;
			medKits = 1;
		};
		void print();

		Point move(std::vector<Point> moves);

		void healTeam(std::vector<Entity*> entities);
};
