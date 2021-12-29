cmake_minimum_required(VERSION 3.16)
project(anomalydetector)

set(CMAKE_CXX_STANDARD 11)

set(HEADER anomaly_detection_util.h AnomalyDetector.h SimpleAnomalyDetector.h timeseries.h)
add_executable(anomalydetector anomaly_detection_util.cpp timeseries.cpp SimpleAnomalyDetector.cpp tests.cpp
        MainTrain.cpp minCircle.cpp minCircle.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h commands.h CLI.h CLI.cpp)
