#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <functional>

inline bool RUNNING = true;
inline bool PAUSED = false;

struct Point {
	/*
	   Point struct used store coordinates on the map
	*/

	int x;
	int y;

	// Default constructor
	Point() {
		x = 0;
		y = 0;
	}
	// Main constructor
	Point(int _x, int _y): x(_x), y(_y){};
	// Copy constructor
	Point(const Point& old) {
		x = old.x;
		y = old.y;
	};

	// Equality operator
	bool operator==(const Point& other) const noexcept {
		return (x == other.x && y == other.y);
	}

	// << operator so that a Point can be easily printed on screen
	friend std::ostream& operator<<(std::ostream& os, const Point& obj) {
		return os << obj.x << "," << obj.y;
	}
};

/*
   Hash function for Point class so it can be used as a key in an unordered_map
*/
template <>
struct std::hash<Point> {
	std::size_t operator()(const Point& p) const {
		std::size_t h1 = std::hash<int>()(p.x);
		std::size_t h2 = std::hash<int>()(p.y);
		return (h1 ^ (h2 << 1));
	}
};

/*
   Basic Object class
*/
class Object {
	protected:
		Point position;

	public:
		Object() = default;
		virtual ~Object() = default;
		Object(Point _position): position(_position) {};
		virtual void print() = 0;
		Point getPos() {return position;};
};

/*
   Utility function to clear terminal screen
*/
inline void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
