//
//  Socket.cpp
//  
//
//  Created by Artem Kovtunenko on 3/4/16.
//
//

#include "Socket.h"

Socket:: Socket()
{
    clientSD = -1;
}
Socket:: ~Socket()
{
    if(clientSD != -1)
    {
        close(clientSD);
    }
}
int Socket:: openCientSocket(const char* name, int port)
{
    struct hostent *host = gethostbyname(name);
    
    /*
     * Creating an address
     */
    sockaddr_in sendSocketAddress;
    bzero((char *)&sendSocketAddress, sizeof(sendSocketAddress));
    sendSocketAddress.sin_family = AF_INET;
    sendSocketAddress.sin_addr.s_addr = inet_addr(inet_ntoa( *(struct in_addr*) (*host->h_addr_list) ));
    sendSocketAddress.sin_port = htons(port);
    
    /*
     *  Opening socket and connecting to a host
     */
    int clientSD = socket(AF_INET, SOCK_STREAM, 0);
    int rc = connect(clientSD, (struct sockaddr *)&sendSocketAddress, sizeof(sendSocketAddress));
    if (rc < 0)
    {
        cerr << "Connection Failed" << endl;
        close(clientSD);
        return -1;
    }
    this->clientSD = clientSD;
    return this->clientSD;
    
}

int Socket::pollRecvFrom(int time)
{
    struct pollfd pfd[1];
    pfd[0].fd = clientSD;             // declare I'll check the data availability of sd
    pfd[0].events = POLLRDNORM; // declare I'm interested in only reading from sd
    
    // check it immediately and return a positive number if sd is readable,
    // otherwise return 0 or a negative number
    int returnValue = poll( pfd, 1, 1000 );
    if(returnValue > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void Socket:: closeSD()
{
    close(clientSD);
}

void Socket:: shutDownSD()
{
    shutdown(clientSD, SHUT_WR);
}

int Socket::writeTo(char msg[], int length)
{
    return write(clientSD, msg, length);
}

/*int Socket::readFrom(char msg[], int length)
{
    

    struct pollfd ufds;
    
    ufds.fd = clientSD;                     // a socket descriptor to exmaine for read
    ufds.events = POLLIN;             // check if this sd is ready to read
    ufds.revents = 0;                 // simply zero-initialized
    int val = poll( &ufds, 1, 1000 ); // poll this socket for 1000msec (=1sec)
    int numBytes = 0, totalBytes = 0, count = 0;
    while ( val > 0 )
    {                  // the socket is ready to read
        //char buf[1500];
        count++;
        numBytes = read( clientSD, msg, length ); // guaranteed to return from read
        if((msg[numBytes] == '\0' && msg[numBytes - 1] == '\n' && msg[numBytes -2 ] == '\r') || count == 10000)
        {
            return numBytes;
            break;
        }
        totalBytes += numBytes;
        val = poll( &ufds, 1, 1000 );
    }
    return 1;
 
    //return 1;
}*/
char* Socket::readBuffer(char msg[], int length)
{
    struct pollfd pfd[1];
    pfd[0].fd = clientSD;             // declare I'll check the data availability of sd
    pfd[0].events = POLLRDNORM; // declare I'm interested in only reading from sd
    char buf[1500];
    bzero(buf, sizeof(buf));
    int numOfChars = 0;
    while(poll(pfd, 1, 1000) > 0)
    {
        numOfChars = read( clientSD, msg, length );
        if(numOfChars == 0)
        {
            break;
        }
        buf[numOfChars] = '\0';
        strcat(buf, msg);
        
    }
    //bzero(msg, sizeof(msg));
    //strcpy(msg, buf);
    return msg;
    
}
void Socket:: readData(ostream& stream)
{
    char message[1500];
    struct pollfd ufds;
    ufds.fd = clientSD;                     // a socket descriptor to exmaine for read
    ufds.events = POLLIN;             // check if this sd is ready to read
    ufds.revents = 0;
    int p = poll(&ufds, 1, 1000);
    while (p > 0) {
        int totalReads = read(clientSD, message, sizeof(message)); // guaranteed to return from read
        if (totalReads == 0)
        {
            break;
        }
        message[totalReads] = '\0';
        string strMessage(message);
        stream << strMessage;
        p = poll(&ufds, 1, 1000);
    }
}






