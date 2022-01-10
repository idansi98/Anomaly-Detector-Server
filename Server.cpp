/*
 * Server.cpp
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *
 *
 */
#include "Server.h"

// Constructor
Server::Server(int port) noexcept(false) {
  this->port = port;
  this->t = 0;
}

// Call admitClients() on a new thread
void Server::start(ClientHandler& ch) noexcept(false){
  // Remember port and ClientHandler
  auto *ti = new threadInfo();
  ti->port = port;
  ti->ch = &ch;
  // Pass them into the pthread
  pthread_create(&this->t,nullptr, &admitClients, (void*)ti);
}

// stops the admitClient thread
void Server::stop() const{
  pthread_cancel(this->t);
}

// Refreshes the alarm
static void refreshAlarm(int signum) {
  alarm(ALARM_WAIT_LENGTH);
}

// TCP port setup + the main loop of admitting clients into our server
[[noreturn]] void* Server::admitClients(void * args) {
  // unpack values
  auto *tf = (threadInfo *) args;
  int port = tf->port;
  auto ch = tf->ch;
  // free unused args
  delete (threadInfo *) args;
  
  // Create socket
  struct sockaddr_in address{};
  address.sin_family =AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port= htons(port);

  // create socket
  int sockFD = socket(AF_INET, SOCK_STREAM, 0);
  if (sockFD < 0)
    perror("Couldn't get a socket");

  // bind socket
  if(bind(sockFD, (struct sockaddr *) &address, sizeof(address)) < 0)
    perror("Couldn't bind");

  // listen
  if (listen(sockFD, 4)<0)
    perror("Couldn't listen");

  // set signal
  signal(SIGALRM, refreshAlarm);
  // Client accepting loop
  while (true) {
    int newSocket;
    // set up a timeout for accepting
    alarm(ALARM_WAIT_LENGTH);
    // accept a client
    if ((newSocket = accept(sockFD , (struct sockaddr*) nullptr, (socklen_t *) nullptr))<0)
      perror("Couldn't accept client");
    // handle the client
    ch->handle(newSocket);
  }
}



Server::~Server() = default;


