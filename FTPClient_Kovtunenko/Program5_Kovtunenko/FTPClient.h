//
//  FTPClient.h
//  
//
//  Created by Artem Kovtunenko on 3/5/16.
//
//

#ifndef ____FTPClient__
#define ____FTPClient__

#include <vector>
#include <sys/poll.h>
#include <fstream>
#include <sstream>
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
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

const int BUFFERSIZE = 1500;
class FTPClient
{
public:
    FTPClient();
    ~FTPClient();
    FTPClient(const char *);
    void closeConnection();
    void passiveOpen();
    void userName();
    void passwd();
    int sendMessage(char*, char*);
    int receiveMessgae(char*);
    //char* receive(char *);
    void cdCMD();
    void lsCMD();
    void getCMD();
    void putCDM();
    void quitCMD();
    char* strTochar(char [], string);
    bool syst();
    int getCode(char []);
    void calculatePASSV(char[]);
    void setCommandRequest(string);

private:
    string commandReques;
    Socket *clientSocket;
    Socket *dataSocket;
    int dataFD;
    const char *url;
    char databuf[BUFFERSIZE];

};


#endif /* defined(____FTPClient__) */
