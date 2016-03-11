//
//  main.cpp
//  Program5
//
//  Created by Artem Kovtunenko on 3/2/16.
//  Copyright (c) 2016 432. All rights reserved.
//

#ifndef SocketCS_Socket_h
#define SocketCS_Socket_h

#include <fstream>
#include <sstream>
#include <sys/poll.h>
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
#include <sys/poll.h>   
#include <iostream>

using namespace std;

class Socket
{
private:
    int clientSD;

public:
    Socket();
    ~Socket();
    int openCientSocket(const char*, int);
    int pollRecvFrom(int);
    void closeSD();
    void shutDownSD();
    int writeTo(char *, int);
    int readFrom(char *, int);
    char* readBuffer(char *, int);
    void readData(ostream&);
    //int sendTo(char msg[], int length);
    //int recvFrom(char msg[], int length);
};


#endif

