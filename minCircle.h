/*
 * minCircle.h
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *
 *
 */

#ifndef ANOMALYDETECTOR_MINCIRCLE_H
#define ANOMALYDETECTOR_MINCIRCLE_H
#include "anomaly_detection_util.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

class Circle {
public:
    Point center;
    float radius;
    Circle(Point c,float r) : center(c), radius(r) {}
};
//Function to check whether a point lies inside or on the boundaries of the circle.
bool isInside(const Circle& c, const Point& p);
//Return the distance between two points.
float dist(Point a, Point b);
//Helper method to get a circle defined by 3 points.
Point getCircleCenter(float ax, float ay, float bx, float by);
//Function to return a unique circle that intersects three points.
Circle findCircleFrom3(const Point& a, const Point& b, const Point& c);
//Function to return the smallest circle that intersects 2 points.
Circle findCircleFrom2(const Point& a, const Point& b);
//Function to check whether a circle encloses the given points.
bool isValidCircle(const Circle& c, const std::vector<Point>& P);
//Function to return the minimum enclosing circle for N <= 3.
Circle findMinCircleTrivial(std::vector<Point>& points);
//Returns the MEC using Welzl's algorithm.
Circle minCircle(Point** points, std::vector<Point> vector, size_t n);
//Get the returned MEC using min_Circle.
Circle findMinCircle(Point** points, size_t size);

#endif //ANOMALYDETECTOR_MINCIRCLE_H
