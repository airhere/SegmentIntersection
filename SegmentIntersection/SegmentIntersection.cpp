// SegmentIntersection.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FindIntersect.h"
#include <SFML/Graphics.hpp>

float inte[NUM_INTE][2];
int inteCount;
const int lineNum = 10;


float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void initLines(std::vector<std::array<sf::Vertex, 2> > & lines, int num) {
	for (int i = 0; i < num; ++i) {
		lines.push_back({ {
			sf::Vertex(sf::Vector2f(RandomFloat(0,600), RandomFloat(0,600))),
			sf::Vertex(sf::Vector2f(RandomFloat(0,600), RandomFloat(0,600)))
		} });
	}
	
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Segments Intersection");
	sf::Keyboard::setVirtualKeyboardVisible(false);

	std::vector<std::array<sf::Vertex, 2> > lines;

	srand(time(0));
	initLines(lines, lineNum);
	findIntersection(lines);

	std::vector<sf::CircleShape*> squares;
	for (int i = 0; i < inteCount; ++i) {
		sf::CircleShape *cs = new sf::CircleShape(4);
		cs->setPosition(inte[i][0]-2, inte[i][1]-2);
		cs->setFillColor(sf::Color(255, 0, 0));
		squares.push_back(cs);
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();

		// Draw the squares
		for (auto &s : squares) {
			window.draw(*s);
		}

		// Draw the lines
		for (auto &l : lines) {
			window.draw(l.data(), 2, sf::Lines);
		}

		window.display();
	}

    return 0;
}

