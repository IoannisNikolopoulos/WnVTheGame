#include "entities.h"

/*
   Entity methods
*/
Entity::Entity(Point _position): Object(_position) {
	strength = rand()%3+1;
	armor = rand()%2+1;
	medKits = rand()%3;
};

bool Entity::damage(int strength) {
	/*
	   Damage entity based on attacker strength
	*/

	health -= armor-strength;
	if (health <= 0) return true;
	return false;
}

void Entity::heal() {
	/*
	   Heal entity
	*/

	health++;
}

bool Entity::interact(Entity* other) {
	/*
	   Interact with another entity on the map
	*/

	// Chose whether to interact with the other entity or not
	int interact = rand()%2;
	if (!interact) return false;

	// If other entity is friendly, heal it if there are med kits available
	if (team == other->team && medKits > 0) {
		other->heal();
		medKits--;
		return false;
	}

	// If it's not friendly attack if it has lower or same strength
	if (strength >= other->strength) {
		return other->damage(strength);
	}

	return false;
}


/*
   Vampire Methods
*/
void Vampire::print() {
	/*
	   Display Vampire
	*/
	std::cout << "v";
}

Point Vampire::move(std::vector<Point> moves) {
	/*
	   Move Vampire according to a list of possible moves
	*/

	// Chose random position to move to from possible moves
	position = moves[rand()%moves.size()];

	// Return new position
	return position;
}


/*
   Werewolf Methods
*/
void Werewolf::print() {
	/*
	   Display Werewolf
	*/
	std::cout << "w";
}

Point Werewolf::move(std::vector<Point> moves) {
	/*
	   Move Werewolf according to a list of possible moves
	*/

	// From possible moves remove the diagonals
	std::vector<Point>::iterator it = moves.begin();
	while (it != moves.end()) {
		if (abs(position.x - it->x) + abs(position.y - it->y) > 1) {
			it = moves.erase(it);
		} else {
			++it;
		}
	}

	// Chose random position to move to from possible moves
	position = moves[rand()%moves.size()];

	// Return new position
	return position;
}


/*
   Player Methods
*/
void Player::print() {
	/*
	   Display Player
	*/

	if (team == "Werewolves") {
		std::cout << "W";
	} else if (team == "Vampires") {
		std::cout << "V";
	}
}

Point Player::move(std::vector<Point> moves) {
	/*
	   Move Player according to a list of possible moves
	*/
	position = moves[0];

	return position;
}

void Player::healTeam(std::vector<Entity *> entities) {
	for (auto& e : entities) {
		e->heal();
	}
	medKits--;
}
