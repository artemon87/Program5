//
//  main.cpp
//  Program5
//
//  Created by Artem Kovtunenko on 3/2/16.
//  Copyright (c) 2016 432. All rights reserved.
//



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
    //ftp.syst();
    //ftp.passiveOpen();
    while(true)
    {
        cout << "ftp> ";
        string command;
        cin >> command;
        //cout << endl;
        if(command == "ls")
        {
            ftp.lsCMD();
            //case 'ls'   : ftp.lsCMD();
            //case 'close': ftp.closeConnection();
            //case 'quit' : ftp.quitCMD();
        }
        if(command == "cd")
        {
            ftp.cdCMD();
        }
    }

    
    
    
    return 0;
}



