/*
 * minCircle.cpp
 *
 * Authors:     206821258 Idan Simai
 *              206534299 Ido Tziony
 *
 *
 */

#include "minCircle.h"
#include "anomaly_detection_util.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;


// Function to check whether a point lies inside or on the boundaries of the circle.
bool isInside(const Circle& c, const Point& p)
{
    return (distance(c.center, p) <= c.radius);
}


// Helper method to get a circle defined by 3 points
Point getCircleCenter(float ax, float ay, float bx, float by)
{
    float A = ax * ax + ay * ay;
    float B = bx * bx + by * by;
    float C = ax * by - ay * bx;
    return Point(((by * A - ay * B) / (2 * C)), ((ax * B - bx * A) / (2 * C)));
}

// Function to return a unique circle that intersects three points.
Circle findCircleFrom3(const Point& a, const Point& b, const Point& c)
{
    Point d = getCircleCenter(b.x - a.x, b.y - a.y,c.x - a.x, c.y - a.y);
    d.x += a.x;
    d.y += a.y;
    return { d, distance(d, a) };
}

// Function to return the smallest circle that intersects 2 points.
Circle findCircleFrom2(const Point& a, const Point& b) {
    float x = ((a.x + b.x) / 2);
    float y = ((a.y + b.y) / 2);
    float r = ((distance(a, b)) / 2);
    return {{x, y}, r};
}

// Function to check whether a circle encloses the given points.
bool isValidCircle(const Circle& c, const vector<Point>& P)
{
// Iterating through all the point to check  whether the point lie inside the circle or not.
    for (const Point& p : P)
        if (!isInside(c, p))
            return false;
    return true;
}

// Function to return the minimum enclosing circle for N <= 3.
Circle findMinCircleTrivial(vector<Point>& points)
{
    if(points.size() == 0) {
        return {{0, 0}, 0};
    }
    else if(points.size() == 1) {
        return {points[0], 0};
    }
    else if (points.size() == 2) {
        return findCircleFrom2(points[0], points[1]);
    }
//Check if MEC can be determined by 2 points only.
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = findCircleFrom2(points[i], points[j]);
            if (isValidCircle(c, points))
                return c;
        }
    }
    return findCircleFrom3(points[0], points[1], points[2]);
}

// Returns the MEC using Welzl's algorithm Takes a set of input points and a vector of points on the circle boundary.
// n represents the number of points in points that are not yet processed.
Circle minCircle(Point** points, vector<Point> vector, size_t n) {
//Check if there are no more points to check or |vector| = 3.
    if (n == 0 || vector.size() == 3) {
        return findMinCircleTrivial(vector);
    }
//Pick a random point.
    int random = rand() % n;
    Point p (points[random]->x, points[random]->y);
//Put the picked point at the end of since it's more efficient than deleting from the middle of the vector.
    swap(points[random], points[n - 1]);
//Get the MEC circle c from the set of points \ {p}.
    Circle c = minCircle(points, vector , (n - 1));
//If c contains point, return c.
    if (isInside(c, p)) {
        return c;
    }
//Otherwise, must be on the boundary of the MEC.
    vector.push_back(p);

//Return the MEC for points - {p} and vector U {p}.
    return minCircle(points, vector, (n - 1));
}

//Return the MEC.
Circle findMinCircle(Point** points, size_t size)
{
    return minCircle(points, {}, size);
}
