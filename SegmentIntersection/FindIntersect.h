#pragma once
/*
*	Author: John Hany
*	Website: http://johnhany.net/
*	Source code updates: https://github.com/johnhany/SegmentsIntersection
*	If you have any advice, you could contact me at: johnhany@163.com
*
*/

#ifndef __FIND_INTE_H
#define __FIND_INTE_H

#define NUM_INTE 500

//save the intersection points
extern float inte[NUM_INTE][2];
//number of intersection points found at present
extern int inteCount;

void findIntersection(std::vector<std::array<sf::Vertex, 2> > lines);

#endif
