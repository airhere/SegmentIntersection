#include "stdafx.h"
#include "windows.h"
#include "FindIntersect.h"
#include <queue>
#include <vector>
#include <cmath>
#include <set>
#include <SFML/Graphics.hpp>

std::set<std::pair<int, int> >hashSet;

//Calculate the coordinate of an intersection point by the coordinates of upper and lower points of two lines.
int intePt(float xa1, float ya1, float xa0, float ya0, float xb1, float yb1, float xb0, float yb0, float scan_y)
{
	float s1, s2, s3, s4;
	float a, b;
	if (ya1 < ya0) {
		std::swap(xa1, xa0);
		std::swap(ya1, ya0);
	}
	if (yb1 < yb0) {
		std::swap(xb1, xb0);
		std::swap(yb1, yb0);
	}
	//See if points a1 and a0 are at two sides of line [b1,b0].
	s1 = (xb0 - xa0)*(yb1 - ya0) - (xb1 - xa0)*(yb0 - ya0);
	s2 = (xb0 - xa1)*(yb1 - ya1) - (xb1 - xa1)*(yb0 - ya1);
	if (s1*s2 <= 0)
	{
		//One of a1 and a0 on line [b1,b0] is acceptable.
		//See if points b1 and b0 are at two sides of line [a1,a0].
		s3 = (xa1 - xb0)*(ya0 - yb0) - (xa0 - xb0)*(ya1 - yb0);
		s4 = s2 - s1 + s3;
		if (s3*s4 <= 0)
		{
			//One of b1 and b2 on line [a1,a0] is acceptable.
			//Calculate the coordinate of the intersection point.

			if (xa0 == xa1 || xb0 == xb1)
			{
				if (xa0 == xa1)
				{
					b = (yb0 - yb1) / (xb0 - xb1);
					inte[inteCount][0] = xa0;
					inte[inteCount][1] = b * (inte[inteCount][0] - xb1) + yb1;
				}
				else {
					a = (ya0 - ya1) / (xa0 - xa1);
					inte[inteCount][0] = xb0;
					inte[inteCount][1] = a * (inte[inteCount][0] - xa1) + ya1;
				}
			}
			else {
				a = (ya0 - ya1) / (xa0 - xa1);
				b = (yb0 - yb1) / (xb0 - xb1);
				inte[inteCount][0] = (a*xa1 - b * xb1 - ya1 + yb1) / (a - b);
				inte[inteCount][1] = a * (inte[inteCount][0] - xa1) + ya1;
			}
			if (inte[inteCount][1] < scan_y) {
				return 0;
			}

			inteCount++;
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

//Check whether the two "left" and "right" lines intersect or not. 
//If two lines intersect, insert the inte-point into event queue,
void checkInte(std::vector<std::array<sf::Vertex, 2> >& lines, int l1, int l2, std::priority_queue<Point> &equeue, float scan_y)
{
	if (l1 == l2 || (hashSet.find(std::pair<int, int>(l1, l2)) != hashSet.end() ||
		hashSet.find(std::pair<int, int>(l2, l1)) != hashSet.end()))
		return;

	if (intePt(lines[l1][0].position.x, lines[l1][0].position.y, lines[l1][1].position.x, lines[l1][1].position.y,
		lines[l2][0].position.x, lines[l2][0].position.y, lines[l2][1].position.x, lines[l2][1].position.y, scan_y))
	{
		Point p(inte[inteCount - 1][0], inte[inteCount - 1][1]);
		p.line = l1;
		p.interline = l2;
		p.eType = 2;

		hashSet.insert(std::pair<int, int>(l1, l2));
		equeue.push(p);
	}
}

float XOfLine(std::array<sf::Vertex, 2> &line, float scanx, float scany) {
	sf::Vector2f pt1 = line[0].position;
	sf::Vector2f pt2 = line[1].position;
	if (pt1.y == pt2.y) {
		return scanx;
	}
	else {
		float k = (pt1.x - pt2.x) / (pt1.y - pt2.y);
		float b = pt1.x - k * pt1.y;
		return k * scany + b;
	}
}

//The main function of the sweep algorithm.
void findIntersection(std::vector<std::array<sf::Vertex, 2> > lines)
{
	int i, j;

	std::priority_queue<Point> equeue;
	inteCount = 0;

	for (int i = 0; i < lines.size(); ++i) {
		if (lines[i][0].position.y > lines[i][1].position.y) {
			equeue.push(Point(lines[i][0].position, i, 1));
			equeue.push(Point(lines[i][1].position, i, 0));
		}
		else {
			equeue.push(Point(lines[i][0].position, i, 0));
			equeue.push(Point(lines[i][1].position, i, 1));
		}
	}

	std::vector<int> scanlist;
	scanlist.clear();

	while (!equeue.empty())
	{
		Point ep = equeue.top();
		equeue.pop();
		if (ep.eType == 0)
		{
			//For an upper point:

			int index = 0;
			//Add it into scan tree.
			if (scanlist.size() == 0) {
				scanlist.push_back(ep.line);
			}
			else {				
				while (index < scanlist.size()) {
					if (XOfLine(lines[scanlist[index]], ep.x, ep.y) >= XOfLine(lines[ep.line] ,ep.x, ep.y)) {
						break;
					}
					++index;
				}
				scanlist.insert(scanlist.begin()+index, ep.line);
			}

			//Check intersection points.
			if (index != 0) {
				checkInte(lines, scanlist[index - 1], scanlist[index], equeue, ep.y);
			}
			if(index + 1 < scanlist.size())
				checkInte(lines, scanlist[index], scanlist[index + 1], equeue, ep.y);

		}
		else if (ep.eType == 1) {
			//For a lower point:

			//Delete the upper point.
			std::vector<int>::iterator iter = scanlist.begin();
			while (iter != scanlist.end()) {
				if (*iter == ep.line) {
					break;
				}
				++iter;
			}
			iter = scanlist.erase(iter);
			// find intersect in neighbors
			if (iter != scanlist.begin() && iter != scanlist.end()) {
				checkInte(lines, *(iter-1), *iter, equeue, ep.y);
			}
		}
		else if (ep.eType == 2) {
			//For an intersection point:

			//Check for inte-points between its left line and its right neighbor,
			//and between its right line and its left neighbor.

			int line1 = ep.line;
			int line2 = ep.interline;
			hashSet.erase(std::pair<int, int>(line1, line2));
			hashSet.erase(std::pair<int, int>(line2, line1));
			int index;
			for (int i = 0; i < scanlist.size() - 1; ++i) {
				if (scanlist[i] == line1 && scanlist[i + 1] == line2 ||
					scanlist[i] == line2 && scanlist[i + 1] == line1) {
					index = i;
					int tmp = scanlist[i];
					scanlist[i] = scanlist[i + 1];
					scanlist[i + 1] = tmp;
				}
			}

			if (index != 0) {
				checkInte(lines, scanlist[index - 1], scanlist[index], equeue, ep.y);
			}
			if (index + 2 < scanlist.size())
				checkInte(lines, scanlist[index + 1], scanlist[index + 2], equeue, ep.y);

		}
	}
}
