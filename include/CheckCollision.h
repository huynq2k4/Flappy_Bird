#pragma once

#include <SDL.h>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Point {
public:
	double x;
	double y;
	Point(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

//SDL_bool CheckCollision(SDL_Rect* rect1, double angle1, Point center1, SDL_Rect* rect2, double angle2, Point center2);

SDL_bool CheckCollision2(vector<Point> shape1, double angle, Point center1, SDL_Rect* shape2);
