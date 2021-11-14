/*
#include <iostream>
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include <fstream>
bool wrong(float val, float expected){
    return val<expected-0.001 || val>expected+0.001;
}
int main () {
    int  i =6;
    float b =11;
    std::cout << "hello world" << std::endl;
    float setX [5] = { 5, 12, 18, 23, 45 };
    float setY [5] = {2, 8, 18, 20, 28};
    float setZ [5] = {1, 6, 25, 51, 109};
    //test2
    std::cout << "COV TEST" << std::endl;
    std::cout << "Expected: 116.88" << std::endl;
    std::cout << "Test got: " << cov(setX, setY, 5) << std::endl;
    std::cout << "VAR TEST" << std::endl;
    std::cout << "Expected: 1554.24" << std::endl;
    std::cout << "Test got: " << var(setZ, 5) << std::endl;
    std::cout << "PEARS TEST" << std::endl;
    std::cout << "Expected: 0.936601" << std::endl;
    std::cout << "Test got: " << pearson(setX, setY, 5) << std::endl;
    const int N=10;
    float x[]={1,2,3,4,5,6,7,8,9,10};
    float y[]={2.1,4.2,6.1,8.1,10.3,12.2,14.4,16.1,18.2,20.3};
    Point* ps[N];
    for(int i=0;i<N;i++)
        ps[i]=new Point(x[i],y[i]);
    Line l=linear_reg(ps,N);
    Point p(4,8);
    float v[]={var(x,N),cov(x,y,N),pearson(x,y,N),l.a,l.b,l.f(4),dev(p,l)};
    float e[]={8.25,16.63,0.999,2.015,0.113,8.176,0.176};
    for(int i=0;i<7;i++)
        if(wrong(v[i],e[i]))
            std::cout<<"error for check "<<i<<" (-14)"<<std::endl;
    for(int i=0;i<N;i++)
        delete ps[i];
    std::cout<<"very naise"<< std::endl;
    TimeSeries test("monkey.csv");
    std::cout << test;
    return 0;
}*/