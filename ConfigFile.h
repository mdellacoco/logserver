/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConfigFile.h
 * Author: Margarita E. Della Sera
 *
 * Created on April 4, 2016, 12:53 PM
 */

#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef struct {
    const std::string FILENAME = "/etc/logserver/logserver.conf";
    std::string DELIM = "=";
    const int MAX_BUFFER = 1024;
    int NUM_LINES = 4;    
}fileProperties;

typedef struct {
        std::string _daemonDir = "";
        std::string _daemonPidFile = "";
        std::string _logFile = "";
        std::string _portNo = "";
    }config;

class ConfigFile {
public:
    ConfigFile();
    ConfigFile(const ConfigFile& orig);
    virtual ~ConfigFile();
    
    void ReadConfigParams();
    config* getConfigParams(){return &_config;}
private:
    fileProperties _fileProperties;
    config         _config;   
};

#endif /* CONFIGFILE_H */

