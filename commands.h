/*
 * commands.h
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *
 *
 */



#ifndef ANOMALYDETECTOR_H__COMMANDS_H_
#define ANOMALYDETECTOR_H__COMMANDS_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include  <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <string>
#include "HybridAnomalyDetector.h"
using namespace std;

struct anomalyRange {
  int startingTime;
  int endingTime;
};




class DefaultIO {
 public:
  // used to read one message like OK
  virtual string read() = 0;
  // used to read one float and put it in value
  virtual void read(float *value) = 0;
  // used to write text to the client
  virtual void write(string text) = 0;
  // used to write floats to the client
  virtual void write(float f) = 0;
  // used to read from a user given file into the fileName path
  void readAndSaveFile (string outputPath)  {
  //Create a new ofstream for writing
    ofstream outputStream;
  // open the source file and the dest file
    outputStream.open(outputPath);
  // start the loop of read/write
    string line;
    line = this->read();
    do {
      outputStream << line << "\n";
      line = this->read();
    } while (line != "done");
//Close the stream
    outputStream.close();
  }
  // destructor
  virtual ~DefaultIO() {}
};



// Console IO
class CIO: public DefaultIO {
 public:
  // reads one line from console
  string read() override {
    string str;
    cin >> str;
    return str;
  }

  void read(float *value) override {
    float tmp;
    cin >> tmp;
    *value = tmp;
    return;
  }

  // writes one string to console
  void write(string text) override {
    cout << text;
  }
  // writes one float to console
  void write(float f) override {
    cout << f;
  }



};

// socket IO
class SIO: public DefaultIO {
  int connection;
 public:
  SIO(int connection): DefaultIO() {
    this->connection = connection;
  }
  // reads one line from socket
  string read() override {
    string userInput="";
    char c=0;
    ::read(connection,&c,sizeof(char));
    while(c!='\n'){
      userInput+=c;
      ::read(connection,&c,sizeof(char));
    }
    return userInput;
  }

  void read(float *value) override {
    string userInput= read();
    float tmp = stof(userInput);
    *value = tmp;
    return;
  }

  // writes one string to user
  void write(string text) override {
    ::write(connection,text.c_str(),text.length());
    ::write(connection,"\n",1);
  }
  // writes one float to user
  void write(float f) override {
    string str = to_string(f);
    write(str);
  }
};



//A struct to share the data of the current state.
// notice we have a username for each ip-port connection
struct Data {
    string userName;
    float threshold;
    vector<AnomalyReport> anomalyReport;
Data() {
    threshold = 0.9;
    userName = "";
}
};

// the command class
class Command {
protected:
    DefaultIO* dio;
    string description;
public:
    // constructor
    Command(DefaultIO* dio):dio(dio){}
    virtual void execute(Data* data)=0;
    string getDescription() {
      return description;
    }
    virtual ~Command(){}
};




//A class for the first command which is "Upload".
class UPLOAD: public Command {
public:
  UPLOAD(DefaultIO* dio):Command(dio) {
    this->description = "upload a time series csv file";
  }
  virtual void execute(Data* data) {
    dio->write("Please upload your local train CSV file.\n");
    string savePath = "anomalyTrain" + data->userName + ".csv";
    dio->readAndSaveFile(savePath);
    dio->write("Upload complete.\n");
    dio->write("Please upload your local test CSV file.\n");
   savePath = "anomalyTest" + data->userName + ".csv";
    dio->readAndSaveFile(savePath);
    dio->write("Upload complete.\n");
    }
};

//A class for the second command which is "Set new threshold".
class THRESHOLD: public Command {
public:
  THRESHOLD(DefaultIO* dio): Command(dio) {
    this->description = "algorithm settings";
    }
  virtual void execute(Data* data) {
    bool thresholdGiven = false;
    dio->write("The current correlation threshold is ");
    dio->write(data->threshold);
    dio->write("\nType a new threshold\n");
    float value;
    dio->read(&value);
    while (!thresholdGiven) {
        if (value > 0 && value <= 1) {
            data->threshold = value;
            thresholdGiven = true;
        } else {
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(&value);
        }
    }
}
};

//A class for the third command which is "Detect anomalies".
class DETECT: public Command {
public:
  DETECT(DefaultIO* dio): Command(dio) {
    this->description = "detect anomalies";
}
  virtual void execute(Data* data) {
    // get paths
    string trainPath = "anomalyTrain" + data->userName + ".csv";
    string testPath = "anomalyTest" + data->userName + ".csv";
    // we use c_str since the builder of TS takes const char* and not string
    TimeSeries train(trainPath.c_str());
    TimeSeries test(testPath.c_str());
    // create the anomaly detector
    HybridAnomalyDetector ad;
    ad.setThreshold(data->threshold);
    // learn the normal model
    ad.learnNormal(train);
    // detect anomalies
    data->anomalyReport = ad.detect(test);
    dio->write("anomaly detection complete.\n");
}
};

//A class for the fourth command which is "Displaying results".
class DISPLAY_RESULTS: public Command {
public:
  DISPLAY_RESULTS(DefaultIO* dio): Command(dio) {
    this->description = "display results";
}
  virtual void execute(Data* data) {
    vector<AnomalyReport> anomalyReport = data->anomalyReport;
    for(AnomalyReport ar : anomalyReport) {
        string timeStep = std::to_string(ar.timeStep);
        dio->write(timeStep + "\t" + ar.description + "\n");
    }
    dio->write("Done.\n");
}
};

//A class for the fifth command which is "Upload And Analyze".
class UPLOAD_AND_ANALYZE: public Command {
public:
  UPLOAD_AND_ANALYZE(DefaultIO* dio): Command(dio) {
    this->description = "upload anomalies and analyze results";
}
  virtual void execute(Data* data) {
    dio->write("Please upload your local anomalies file.\n");
    string path = "userAnomalyReport" + data->userName + ".csv";
    dio->readAndSaveFile(path);
    dio->write("Upload complete.\n");
    auto anomalyRanges = getListOfUniqueAnomalies(data);
    // a vector for user's anomaly ranges
    vector<anomalyRange> userRanges;
    // starts read the user sent anomaly file
    string line;
    ifstream readStream;
    readStream.open(path);
    // add pairs program gets to done
    while (getline(readStream,line) && line!= "done") {
      anomalyRange range;
      int commaPos = line.find(',');
      range.startingTime = stoi(line.substr(0, commaPos));
      range.endingTime = stoi(line.substr(commaPos+1, line.length()));
      userRanges.push_back(range);
    }
    // out P
    int positiveCount = userRanges.size();
    int sumUserRanges = 0;
    for (anomalyRange ar : userRanges) {
      sumUserRanges += (ar.endingTime - ar.startingTime);
    }
    // our N
    int negativeCount = getNumOfLines(data) - sumUserRanges;
    int truePositiveCounter = 0;
    for (anomalyRange ar : anomalyRanges)  {
      if (isAnomalyInRanges(ar, userRanges)) {
        truePositiveCounter++;
      }
    }
    int falsePositiveCount = anomalyRanges.size() - truePositiveCounter;
    // calculate true positive and false positive rates
    float tpRate =(float) truePositiveCounter / (float) positiveCount;
    float fpRate = (float) falsePositiveCount / (float) negativeCount;
    dio->write("True Positive Rate: " + getStringForm(tpRate) + "\n");
    dio->write("False Positive Rate: " + getStringForm(fpRate) +  "\n");



}
 private:
  // we normalize to up to 3 digits after the dot
  string getStringForm(float number) {
    string temp = to_string(number);
    int dotPosition = temp.find('.');
    int cutOffPosition = dotPosition+3;
    // remove trailing 0's
    while (temp[cutOffPosition] == '0') {
      cutOffPosition--;
    }
    // if the number is a natural number - remove the dot
    if (cutOffPosition == dotPosition) {
      cutOffPosition--;
    }
    return temp.substr(0, cutOffPosition+1);
}
  vector<anomalyRange> getListOfUniqueAnomalies(Data* data) {
    // initialize the unique anomalies vector
    vector<anomalyRange> anomalyRanges;
    // get our anomaly report
    vector<AnomalyReport> anomalyReport = data->anomalyReport;
    bool firstPair = true;
    long int lastTime = -1;
    long int thisTime;
    string lastDescription = "\n";
    string thisDescription;
    anomalyRange currentRange;
    for(AnomalyReport ar : anomalyReport) {
      thisTime = ar.timeStep;
      thisDescription = ar.description;
      // new pair completely
      if ((thisDescription != lastDescription) || thisTime != (lastTime+1)) {
        if (!firstPair) {
          // save previous pair
          currentRange.endingTime = lastTime;
          anomalyRanges.push_back(currentRange);
        }
        // start counting for this pair
        currentRange.startingTime = thisTime;
        firstPair = false;
      }
      lastTime = thisTime;
      lastDescription = thisDescription;
    }
      // for the last value
      currentRange.endingTime = lastTime;
      anomalyRanges.push_back(currentRange);
      return anomalyRanges;
}
// gets the amount of lines in the file
  int getNumOfLines(Data *data) {
    string path = "anomalyTest" + data->userName + ".csv";
    int rows = 0;
    ifstream testPath(path);
    string line;
    while (getline(testPath,line)) {
      rows++;
    }
    // first line is names and last line is a blank space
    return rows-2;
}

  bool isAnomalyFitting(anomalyRange ar1, anomalyRange ar2) {
    // if they dont have any shared time in common -> not fitting
    if (ar1.startingTime > ar2.endingTime || ar1.endingTime < ar2.startingTime) {
      return false;
    }
    // here if both times share some period
    return true;
  }
// this tries to find a match between 1 anomaly and a vector of anomalies
  bool isAnomalyInRanges(anomalyRange ar1, vector<anomalyRange> anomalies) {
    for (anomalyRange ar2 : anomalies) {
      if (isAnomalyFitting(ar1,ar2)) {
        return true;
      }
    }
    return false;
  }

};
//A class for the sixth command which is "Exit".
class Exit: public Command {
public:
  Exit(DefaultIO* dio): Command(dio) {
        this->description = "exit";
}
// here the end of the transaction is simple, so no more actions required
  virtual void execute(Data* data) {
}
};
#endif //ANOMALYDETECTOR_H__COMMANDS_H_
