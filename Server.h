/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <thread>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<signal.h>
#include "commands.h"
#include "CLI.h"
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
 public:
  virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
typedef struct threadInfo{
  ClientHandler *ch;
  int port;
} threadInfo;

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
 public:
  virtual void handle(int clientID){
    //TODO: fill this out so it gets clientID = newSocket = accept()...
    SIO dio = SIO(clientID);
    CLI cli = CLI(&dio);
    cli.start();
  }
};


// implement on Server.cpp
class Server {
  pthread_t t; // the thread to run the start() method in
  int port;

  // you may add data members

 public:
  Server(int port) throw (const char*);
  virtual ~Server();
  void start(ClientHandler& ch)throw(const char*);
  void stop();
 private:
  static void* admitClients(void * args);
};

#endif /* SERVER_H_ */
