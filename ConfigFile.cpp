/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConfigFile.cpp
 * Author: Margarita E. Della Sera
 * 
 * Created on April 4, 2016, 12:53 PM
 */

#include <string>

#include "ConfigFile.h"

ConfigFile::ConfigFile() {
}

ConfigFile::ConfigFile(const ConfigFile& orig) {
}

ConfigFile::~ConfigFile() {
}

void ConfigFile::ReadConfigParams(){
    
    int fd;
    std::filebuf fb;
    std::string ff[5] = {"", "", "", "", ""};
  
    char part1[256];
    char part2[256];

    int i = 0;   
    
    if (fb.open (_fileProperties.FILENAME.c_str() ,std::ios::in)) {
        std::istream is(&fb);
 
        while (!is.eof()) {  

            is.getline(part1, 256, ' ');
            is.getline(part2, 256);

            ff[i] = part2;
            i++;          
        }
      _config._daemonDir = ff[0];
      _config._daemonPidFile = ff[1];
      _config._logFile = ff[2];
      _config._portNo = ff[3];
        
    }    
}

