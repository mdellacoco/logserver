/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Margarita E. DElla Sera
 *
 * Created on March 30, 2016, 5:40 PM
 */

#include <cstdlib>
#include "LoggerServer.h"
#include "Daemon.h"
#include "ConfigFile.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    //Read config File
  
    config* pConfigFile;
    
    ConfigFile confFileParams;
    confFileParams.ReadConfigParams();
    pConfigFile = confFileParams.getConfigParams();
   
    //Start Daemon
    
   // Daemon myDaemon(daemonDir, daemonPidFile);
    Daemon myDaemon(pConfigFile->_daemonDir.c_str(), 
                    pConfigFile->_daemonPidFile.c_str());
    
    myDaemon.daemonize();
     
    //Start Logger
    LoggerServer* pMyLoggerServer;
    
    pMyLoggerServer = LoggerServer::Instance(pConfigFile->_logFile.c_str(),
                                             atoi(pConfigFile->_portNo.c_str()));
    
    pMyLoggerServer->Run();
     
    pMyLoggerServer->StopInstance();

    return 0;
}

