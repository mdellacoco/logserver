/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LoggerServer.h
 * Author: Margarita E. DElla Sera
 *
 * Created on March 30, 2016, 5:43 PM
 */

#ifndef LOGGERSERVER_H
#define LOGGERSERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;

class LoggerServer {
public:
    static LoggerServer* Instance(const string& logFile, int _portNo);
    void StopInstance();
    void Run();
    int makeAcceptSock(const char *servspec);
    void acceptLoop (const char *servspec);
    ~LoggerServer();
private:
    LoggerServer();
    LoggerServer(const LoggerServer& orig){};
    LoggerServer& operator = (const LoggerServer& orig){};
    void error(const char *msg);
   // int makeAcceptSock(const char *servspec);
    void newConnection (int sock);
    bool isclosed (int sock);
    
private:
    bool                 active;
    int                  porNumber;
    string               filename;
    //Singleton
    static LoggerServer* instance;

};

#endif /* LOGGERSERVER_H */

