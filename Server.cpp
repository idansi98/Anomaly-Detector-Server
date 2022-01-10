
#include "Server.h"



Server::Server(int port)throw (const char*) {
  this->port = port;
}

void Server::start(ClientHandler& ch)throw(const char*){
  //std::ref(ch), port
  threadInfo ti;
  ti.port = port;
  ti.ch = &ch;
  pthread_create(&this->t,NULL, &admitClients, (void*)&ti);
}

void Server::stop(){
  pthread_cancel(this->t);
}

static void testFunc(int signum) {
  //nothing
}

void* Server::admitClients(void * args) {
  threadInfo *tf = (threadInfo *) args;
  int port = tf->port;
  auto ch = tf->ch;

  struct sockaddr_in address;
  address.sin_family =AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port= htons(port);

  // create socket
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  if (sockfd <0) {
    cout<< "Couldn't get a socket\n";
  }
  cout << "created socket\n";

  // bind
  if(bind(sockfd,(struct sockaddr *) &address, sizeof(address)) < 0) {
    cout<< "Couldn't bind\n";

  }
  cout << "binded socket\n";

  // listen
  if (listen(sockfd,4)<0) {
    cout<< "Couldn't listen\n";
  }
  cout << "listenning to socket\n";

  //
  while (true) {
    struct sockaddr clientaddress;
    int adress_len;
    // accept 1 client
    //TODO: add socket timeout
    int newSocket;
    signal(SIGALRM, testFunc);
    //alarm(1);
    cout << "Now waiting\n";

    if((newSocket = accept(sockfd ,(struct sockaddr*) 0, (socklen_t *) 0))<0) {
      cout<< "Couldn't accept client\n";
    }
    cout << "New user :" <<  to_string(newSocket) << "\n";
    //alarm(0);
    ch->handle(newSocket);
  }

}



Server::~Server() {
}


