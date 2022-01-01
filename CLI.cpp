/*
 * CLI.cpp
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *
 *
 */
#include "CLI.h"
#include "commands.h"


CLI::CLI(DefaultIO* dio) {
    //Initialize a vector of commands.
    this->dio = dio;
    commands.push_back(new UPLOAD(dio));
    commands.push_back(new THRESHOLD(dio));
    commands.push_back(new DETECT(dio));
    commands.push_back(new DISPLAY_RESULTS(dio));
    commands.push_back(new UPLOAD_AND_ANALYZE(dio));
    commands.push_back(new Exit(dio));
}

//This method starts the menu using switch case.
void CLI::start() {
    // init data
    Data *data = new Data();
    // create the menu
    int counter = 1;

    string menu = "Welcome to the Anomaly Detection Server.\n"
                  "Please choose an option:\n";
    for (Command* cmd : commands) {
      // for each command, adds it to the menu
      menu += to_string(counter) + "." + cmd->getDescription() + "\n";
      counter ++;
    }

  // show the menu
    dio->write(menu);
    // let the user pick an option
    float num = 0;
    dio->read(&num);
    int choice = num - 1;

  while (choice != 5) {
    // execute the command
        commands[choice]->execute(data);
    // show menu again
        dio->write(menu);
        // let the user pick again
        dio->read(&num);
        choice = num - 1;
    }
    // here if user picked to exit
  commands[5]->execute(data);
    }

CLI::~CLI() {
}
