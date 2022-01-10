/*
 * anomaly_detection_util.cpp
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *
 *
 *
 */

#include "anomaly_detection_util.h"


// returns the sum of an array of floats
float sum(const float* x, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++)
        sum += x[i];
    return sum;
}
//returns the distance between two points
float distance(Point a, Point b) {
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}
// returns the mean of an array of floats
float mean(const float* x, int size) {
    return sum(x, size) / (float) size;
}

// returns the variance of X and Y
float var(const float* x, const int size) {
    float meanX = mean(x, size);
    float array [size];
    for (int i = 0; i < size; i++)
        array[i] = static_cast<float>((x[i] - meanX) * (x[i] - meanX));
    return mean(array, size);
}

// returns the covariance of X and Y
float cov(const float* x, const float* y, int size) {
    float meanX = mean(x, size);
    float meanY = mean(y, size);
    float array1[size];
  for (int i = 0; i < size; ++i) {
    array1[i] = (x[i]-meanX) * (y[i]-meanY);
  }
    float covariance = mean(array1,size);
    return covariance;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(const float* x, const float* y, int size) {
    return cov(x, y, size) / (std::sqrt(var(x, size)) * std::sqrt(var(y, size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float x[size];
    float y[size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = ((cov(x, y, size)) / (var(x, size)));
    float b = ((mean(y, size)) -(a * mean(x, size)));
    // calls the constructor of return value -> Point
    return {a, b};
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
    Line line(linear_reg(points, size));
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    return std::abs(p.y - l.f(p.x));
}

