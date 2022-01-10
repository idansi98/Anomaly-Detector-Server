/*
 * Server.h
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *
 *
 */

#ifndef SERVER_H_
#define SERVER_H_

#define ALARM_WAIT_LENGTH 5
#include <thread>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <csignal>
#include "commands.h"
#include "CLI.h"
using namespace std;

// a general client handler
class ClientHandler{
 public:
  virtual void handle(int clientID)=0;
};


// A struct for moving information to the thread
typedef struct threadInfo{
  ClientHandler *ch;
  int port;
} threadInfo;

// The anomaly detection handler class
class AnomalyDetectionHandler:public ClientHandler{
 public:
  void handle(int clientID) override{
    // create a new server input output DefaultIO
    SIO dio = SIO(clientID);
    // create a new CLI with dio
    CLI cli = CLI(&dio);
    // start interacting with the user
    cli.start();
  }
};


// Server class
class Server {
  // the thread to run the start() method in
  pthread_t t{};
  int port;
 public:
  // Constructor
  explicit Server(int port) noexcept(false);
  // Destructor
  virtual ~Server();
  // create a thread to start admitClients()
  void start(ClientHandler& ch) noexcept(false);
  // stop getting new clients
  void stop() const;
 private:
  // The main loop of getting new clients
  [[noreturn]] static void* admitClients(void * args);
};

#endif /* SERVER_H_ */
