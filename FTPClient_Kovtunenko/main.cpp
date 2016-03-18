//
//  main.cpp
//  Program5
//
//  Created by Artem Kovtunenko on 3/2/16.
//  Copyright (c) 2016 432. All rights reserved.
//


#include <string>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Socket.h"
#include "FTPClient.h"


using namespace std;


int main(int argc, const char * argv[])
{
    FTPClient ftp(argv[1]);
    
    ftp.userName();
    ftp.passwd();

    while(true)
    {
        cout << "ftp> ";
        string command;
        cin >> command;
        if(command == "syst")
        {
            ftp.setCommandRequest("syst");
            ftp.syst();
        }
        if(command == "ls")
        {
            ftp.setCommandRequest("ls");
            ftp.lsCMD();
        }
        if(command == "cd")
        {
            ftp.setCommandRequest("cd");
            ftp.cdCMD();
        }
        if(command == "close")
        {
            ftp.setCommandRequest("close");
            ftp.closeConnection();
            //break;
        }
        if(command == "quit")
        {
            ftp.setCommandRequest("quit");
            ftp.quitCMD();
            break;
        }
        if(command == "get")
        {
            ftp.setCommandRequest("get");
            ftp.getCMD();
        }
        if(command == "put")
        {
            ftp.setCommandRequest("put");
            ftp.putCDM();
        }
    }
    return 0;
}




