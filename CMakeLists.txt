cmake_minimum_required(VERSION 3.16)
project(anomalydetector)

set(CMAKE_CXX_STANDARD 11)

add_executable(anomalydetector MainTrain.cpp anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h
        SimpleAnomalyDetector.h timeseries.cpp timeseries.h SimpleAnomalyDetector.cpp tests.cpp)
