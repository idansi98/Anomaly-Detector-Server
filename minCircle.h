//
// Created by idans on 11/12/2021.
//

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
// Function to check whether a point lies inside or on the boundaries of the circle.
bool is_Inside(const Circle& c, const Point& p);
// Helper method to get a circle defined by 3 points.
Point get_Circle_Center(float ax, float ay, float bx, float by);
// Function to return a unique circle that intersects three points.
Circle get_Circle_From3(const Point& a, const Point& b, const Point& c);
// Function to return the smallest circle that intersects 2 points.
Circle get_Circle_From2(const Point& a, const Point& b);
// Function to check whether a circle encloses the given points.
bool is_Valid_Circle(const Circle& c, const std::vector<Point>& P);
// Function to return the minimum enclosing circle for N <= 3.
Circle min_Circle_Trivial(std::vector<Point>& points);
Circle min_Circle(Point** points, std::vector<Point> vector, int n);
Circle get_Min_Circle(Point** points, int size);

#endif //ANOMALYDETECTOR_MINCIRCLE_H
