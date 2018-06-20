#pragma once
#include <SFML/Graphics.hpp>
class  Point
{
public:
	Point(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Point(sf::Vector2f p, int linenum, int etype = 0) {
		this->x = p.x;
		this->y = p.y;
		line = linenum;
		eType = etype;
	}

	Point() {}
	~Point() {}

	bool operator < (const Point& p) const
	{
		if (y > p.y) {
			return 1;
		}
		else if (y == p.y && x > p.x) {
			return 1;
		}
		else return 0;
	}

	float x, y;
	int line;
	int interline;
	int eType; //0 upper 1 lowwer 2 intersection
};

class  Line
{
public:
	Line(Point x, Point y) {
		this->x = x;
		this->y = y;
	}
	~Line() {}

private:
	Point x, y;
};