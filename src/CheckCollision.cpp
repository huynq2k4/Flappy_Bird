#include "CheckCollision.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const double PI = atan(1) * 4;

class Point {
public:
	double x;
	double y;
	Point(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
	
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 - p, q and r are collinear
// 1 - Clockwise
// 2 - Counterclockwise
int orientation(Point p, Point q, Point r)
{

	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0; // collinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

Point rotate(Point src, Point center, double angle) {
	angle *= PI / 180;
	src.x = center.x + (src.x - center.x) * cos(angle) - (src.y - center.y) * sin(angle);
	src.y = center.y + (src.x - center.x) * sin(angle) + (src.y - center.y) * cos(angle);
	return src;
}

SDL_bool CheckCollision(SDL_Rect* rect1, double angle1, SDL_Rect* rect2, double angle2)
{
	//Convert integer coordinate to double coordinate
	double rect1x = rect1->x;
	double rect1y = rect1->y;
	double rect1w = rect1->w;
	double rect1h = rect1->h;

	double rect2x = rect2->x;
	double rect2y = rect2->y;
	double rect2w = rect2->w;
	double rect2h = rect2->h;

	//Store new rect after rotation
	vector<Point> newRect1;
	Point newRect1center = { rect1x + rect1w / 2, rect1y + rect1h / 2 };


	newRect1.push_back(rotate({ rect1x, rect1y }, newRect1center, angle1));
	newRect1.push_back(rotate({ rect1x, rect1y + rect1h }, newRect1center, angle1));
	newRect1.push_back(rotate({ rect1x + rect1w, rect1y }, newRect1center, angle1));
	newRect1.push_back(rotate({ rect1x + rect1w, rect1y + rect1h }, newRect1center, angle1));

	vector<Point> newRect2;
	Point newRect2center = { rect2x + rect2w / 2, rect2y + rect2h / 2 };
	newRect2.push_back(rotate({ rect2x, rect2y }, newRect2center, angle2));
	newRect2.push_back(rotate({ rect2x, rect2y + rect2h }, newRect2center, angle2));
	newRect2.push_back(rotate({ rect2x + rect2w, rect2y }, newRect2center, angle2));
	newRect2.push_back(rotate({ rect2x + rect2w, rect2y + rect2h }, newRect2center, angle2));

	//Collide if any pair of segments intersect
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (doIntersect(newRect1[i % 4], newRect1[(i + 1) % 4], newRect2[j % 4], newRect2[(j + 1) % 4])) return SDL_TRUE;
		}
	}
	return SDL_FALSE;
}
