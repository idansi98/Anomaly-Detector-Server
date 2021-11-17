/*
 * timeseries.cpp
 *
 * Authors:  206821258 Idan Simai
 *           206534299 Ido Tziony
 *
 */
//TODO: Add try/catch/exceptions
// compile only once
#pragma once
#include "timeseries.h"
#include <vector>
#include <string>



// constructor
TimeSeries::TimeSeries(const char *csvFileName) : fileReader(), featureMap(), features() {
    fileReader.open(csvFileName);


    if (!fileReader.is_open()) {
        std::cout << "Unable to open file \n";
    }
    rowCount = 0;
    features = readLine(true);
    columnCount = features.size();
    // initializer functions
    initMap();
    initVectors();
}


std::vector<std::string> TimeSeries::readLine(bool firstTime) {
    std::vector<std::string> words;

    std::string row;
    // gets a line from fileReader and puts it in row
    getline(fileReader, row);
    if (row.empty()) {
        // if empty -> return an empty vector
        return {};
    }
    // std::cout << row << "\n";
    // separates the line into tokens
    size_t pos = 0;
    std::string token;
    while ((pos = row.find(',')) != std::string::npos) {
        token = row.substr(0, pos);
        // push token into vector of words
        words.push_back(token);
        row.erase(0, pos + 1);
    }
    words.push_back(row);
  if (words.size() != columnCount && !firstTime) {
    std::cout << "wrong amount of elements in line " << rowCount + 1 << ", expected: " << columnCount<< " got: " << words.size() << std::endl;
    exit(1);
  }
    return words;
}

/* initializes the feature map to be
   FEATURE1:EMPTY VECTOR
   FEATURE2:EMPTY VECTOR
   etc
*/
void TimeSeries::initMap() {
    for (const std::string& str: features) {
        std::vector<float> vec;
        featureMap.insert(std::make_pair(str, vec));
    }
}

void TimeSeries::initVectors() {
    std::vector<std::string> line = readLine(false);
    while (!line.empty()) {
        rowCount++;
        int counter = 0;
        for (const std::string& str: line) {
            // get the column we need to add at
            // NOTE: need direct access so can't use getColumn()
            std::vector<float> column = featureMap.at(features.at(counter));
            // add value to column
            column.push_back(stod(str));
            featureMap.erase(features[counter]);
            featureMap.insert(std::make_pair(features.at(counter), column));
            counter++;
        }
        if (counter != columnCount) {
            // if we scanned less values than amount of columns
            std::cout << "insufficient values at row: " << rowCount;
            // handle it
        }
        // get next line
        line = readLine(false);
    }
}

std::ostream& operator <<(std::ostream &out, const TimeSeries &time_series) {
    for(std::string str: time_series.features) {
        out << "FEATURE: " << str ;
        const std::vector<float> values = time_series.getColumn(str);
        const int sizeOfColumn = values.size();
        out << " (" << sizeOfColumn <<")" << std::endl;
        for(double value: values) {
            out << value << std::endl;
        }
        out << "END OF VALUES" << std::endl;
    }
    out << "END OF FEATURES" << std::endl;
    return out;
}




/*
TimeSeries::TimeSeries(const char* csvFileName) {
    ifstream input(csvFileName);
    string string1;
    input >> string1;
    string feature;
    stringstream file(string1);
    while(getline(file, feature, ',')) {
        vector<float> vector;
        featuresTable[feature] = vector;
        featuresNames.push_back(feature);
    }
    while(!(input.eof())){
        string line;
        input >> line;
        string value;
        stringstream stringstream1(line);
        int i = 0;
        while(getline(stringstream1, value, ',')){
            featuresTable[featuresNames[i]].push_back(stof(value));
            i++;
        }
    }
    input.close();
    this->colCounter = featuresTable[featuresNames[0]].size();

}
  */

