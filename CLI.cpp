/*
 * CLI.cpp
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *  TODO: Create a read method which deals with int in a class that implements DefaultIO.
 *  TODO: and after we have read(int* int) cancel the conversion from float to int.
 */
#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO* dio) {
    //Initialize a vector of commands.
    this->dio = dio;
    vector<Command*> coms;
    UPLOAD* first = new UPLOAD(dio);
    coms.push_back(first);
    THRESHOLD* second = new THRESHOLD(dio);
    coms.push_back(second);
    DETECT* third = new DETECT(dio);
    coms.push_back(third);
    DISPLAY_RESULTS* fourth = new DISPLAY_RESULTS(dio);
    coms.push_back(fourth);
    UPLOAD_AND_ANALYZE* fifth = new UPLOAD_AND_ANALYZE(dio);
    coms.push_back(fifth);
    Exit* sixth = new Exit(dio);
    coms.push_back(sixth);
    commands = coms;
}

//This method starts the menu using switch case.
void CLI::start() {
    Data *data = new Data();
    string menu = "Welcome to the Anomaly Detection Server.\n"
                  "Please choose an option:\n1.upload a time series csv file\n"
                  "2.algorithm settings\n3.detect anomalies\n4.display results\n"
                  "5.upload anomalies and analyze results\n6.exit\n";
    float num = 0;
    dio->read(&num);
    while (num != 6) {
        dio->write(menu);
        switch ((int) num) {
            case 1: {
                commands[0]->execute(data);
                break;
            }
            case 2: {
                commands[1]->execute(data);
                break;
            }
            case 3: {
                commands[2]->execute(data);
                break;
            }
            case 4: {
                commands[3]->execute(data);
                break;
            }
            case 5: {
                commands[4]->execute(data);
                break;
            }
            case 6: {
                commands[5]->execute(data);
            }
            default: {
                break;
            }
        }
                dio->read(&num);
        }
    }

CLI::~CLI() {
}
