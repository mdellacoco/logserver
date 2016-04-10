/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LoggerServer.cpp
 * Author: Margarita E. DElla Sera
 * 
 * Created on March 30, 2016, 5:43 PM
 */

#include "LoggerServer.h"

LoggerServer* LoggerServer::instance = 0;

LoggerServer::LoggerServer() {
}

LoggerServer* LoggerServer::Instance(const string& logFile, int _portNo){
 if(!instance) {
     instance = new LoggerServer;
     instance->active = true;
     instance->porNumber = _portNo;
     instance->filename = logFile;   
 }   
 return instance;
}

LoggerServer::~LoggerServer() {
    if(instance->instance)
        delete instance->instance;
}

void LoggerServer::StopInstance(){
    instance->active = false;
}

void LoggerServer::Run(){
   int sockfd, newsockfd;
   socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;
   
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
     
   if (sockfd < 0) 
      error("ERROR opening socket");
   
   int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
         error("setsockopt(SO_REUSEADDR) failed");
     
   bzero((char *) &serv_addr, sizeof(serv_addr));
     
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(instance->porNumber);
     
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
      error("ERROR on binding");
     
   listen(sockfd,5);
   
   for(;;){
      clilen = sizeof(cli_addr);
     
      newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
     
      if (newsockfd < 0) 
          error("ERROR on accept");
   
      std::thread t(&LoggerServer::newConnection, this, newsockfd);
      t.detach();
   }
   /* 
   bzero(buffer,256);
     
   n = read(newsockfd,buffer,255);
     
   if (n < 0) error("ERROR reading from socket");
     
   printf("Here is the message: %s\n",buffer);
   
   ofstream logfile;
   
   logfile.open(instance->filename.c_str(), std::ios_base::app);
     
   if(logfile.is_open()) {
       logfile << buffer << "\n";
   }
   else
     error("Your file is close");
   
   n = write(newsockfd,"I got your message",18);
     
   if (n < 0) error("ERROR writing to socket");
     
   close(newsockfd);
     
   close(sockfd);
   logfile.close();
       */
}

int LoggerServer::makeAcceptSock(const char *servspec){
    
    const int one = 1;
    struct addrinfo hints = {};
    struct addrinfo *res = 0, *ai = 0, *ai4 = 0;
    char *node = strdup(servspec);
    char *service = strrchr(node, ':');
    int sock;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
   // hints.ai_flags = 0;
 //   hints.ai_protocol = 0;          /* Any protocol */
    
    getaddrinfo(NULL, "3389", &hints, &res);
    free(node);
    
    for (ai = res; ai; ai = ai->ai_next) {
        if (ai->ai_family == PF_INET6) break;
        else if (ai->ai_family == PF_INET) ai4 = ai;
    }
    ai = ai ? ai : ai4;
    
    sock = socket(ai->ai_family, SOCK_STREAM, 0);
    
    if (sock < 0) 
      error("ERROR opening socket");
    
    
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    bind(sock, ai->ai_addr, ai->ai_addrlen);
    listen(sock, 256);
    
    int new_sock = accept(sock, NULL, NULL);
    
    printf("new sock is %i\n", new_sock);
    
    std::thread t(&LoggerServer::newConnection, this, new_sock);
    t.detach();
    
    freeaddrinfo(res);

    return sock; 
}

void LoggerServer::acceptLoop (const char *servspec){
    printf("Estoy aqui .. \n");
     int sock = instance->makeAcceptSock("");
     printf("Estoy aqui 0.. %i\n", sock);
     
     struct sockaddr_in cli_addr;
     
     socklen_t clilen;
     clilen = sizeof(cli_addr);
     
     for (;;) {
         printf("Estoy aqui 1.. %i\n", sock);
        int new_sock = accept(sock, NULL, NULL);
       // int new_sock = accept(sock,(struct sockaddr *) &cli_addr, &clilen);
        if (new_sock < 0) 
          error("ERROR on accept");
        //std::thread t(newConnection, new_sock);
        printf("Estoy aqui 1.. \n");
        std::thread t(&LoggerServer::newConnection, this, new_sock);
        t.detach();
    }
}

void LoggerServer::newConnection (int sock) {
    
    ssize_t r;
    char buffer[256];
    
    while (!isclosed(sock)) {
       // r = send(sock, ".\n", 2, 0);
       bzero(buffer,256);
     
       r = read(sock,buffer,255);
     
       if (r < 0) error("ERROR reading from socket");
     
      //  printf("Here is the message: %s\n",buffer);
        
        ofstream logfile;
   
        logfile.open(instance->filename.c_str(), std::ios_base::app);
     
        if(logfile.is_open()) 
            logfile << buffer << "\n";
        //if (r < 0) break;
        logfile.close();
        sleep(1);
    }
    close(sock);
}
    

bool LoggerServer::isclosed (int sock){
    
    char x;
interrupted:
    ssize_t r = ::recv(sock, &x, 1, MSG_DONTWAIT|MSG_PEEK);
    if (r < 0) {
        switch (errno) {
        case EINTR:     goto interrupted;
        case EAGAIN:    break; /* empty rx queue */
        case ETIMEDOUT: break; /* recv timeout */
        case ENOTCONN:  break; /* not connected yet */
        default:        throw(errno);
        }
    }
    return r == 0;
    
}

void LoggerServer::error(const char *msg)
{
    perror(msg);
    exit(1);
}

