//
//  FTPClient.cpp
//  
//
//  Created by Artem Kovtunenko on 3/5/16.
//
//

#include "FTPClient.h"

//default constructor
FTPClient:: FTPClient()
{
    clientSocket = new Socket();
    
}
//default destructor
FTPClient:: ~FTPClient()
{
    delete clientSocket;
}

//constructor that takes a server's URL to initiate a socket.
//This URL will be passed to a Socket class.
FTPClient:: FTPClient(const char * serverURL)
{
    while(true)
    {
        bzero(databuf, sizeof(databuf));
        url = serverURL;
        clientSocket =new Socket();
        int clientSD = clientSocket->openCientSocket(serverURL, 21);
        clientSocket->readFrom(databuf, sizeof(databuf));
        if(getCode(databuf) == 220) //check if message received starts with code 220
        {
            cout << databuf;
            break;
        }
    }
}

//this method takes a string as a given argument/parameter and
//convers it to char buffer (char data[] parameter comes enpty at first)
char* FTPClient:: strTochar(char data[], string str)
{
    
    strncpy(data, str.c_str(), str.length());
    return data;
}

//take a user name and pass it to FTP server.For as long as you get message that
//starts with 331 code, we good.
void FTPClient:: userName()
{
    while(true)
    {
        cout << "Name (" << url << ":" << getlogin() << "): ";
        char userName[BUFFERSIZE], cmdUser[BUFFERSIZE];
        
        bzero(userName, sizeof(userName));
        bzero(cmdUser, sizeof(cmdUser));
        bzero(databuf, sizeof(databuf));
        
        string name;
        cin >> name;
        strTochar(userName, name);
        strTochar(cmdUser, "USER ");
        sendMessage(cmdUser, userName);
        
        clientSocket->readFrom(databuf, sizeof(databuf));
        if(getCode(databuf) == 331)
        {
            cout << databuf;
            break;
        }
    }
}
//take a password from user and send it to server, if server
//authenticates it send a message to user that starts with code 230;
//otherwise, system prompts to enter a pass again
void FTPClient:: passwd()
{
    int code = 0;
    do
    {
        cout << "Password: ";
        char passwd[BUFFERSIZE], cmdPass[BUFFERSIZE];//, temp[20000];
        bzero(databuf, sizeof(databuf));
  
        bzero(passwd, sizeof(passwd));
        bzero(cmdPass, sizeof(cmdPass));
        
        string password;
        cin >> password;
        
        strTochar(passwd, password);
        strTochar(cmdPass, "PASS ");
        sendMessage(cmdPass, passwd);
   
        
        clientSocket->readBuffer(databuf, sizeof(databuf));
        code = getCode(databuf);
        if(getCode(databuf) == 501)
        {
            cout << databuf;
        }
   }
    while(code != 230);
    cout << databuf;
    syst();

}

//takes two arguments: command and massage from user,
//if message is empty, it sends command only such as "ls".
//Bufore we do that we create empty buffer (even though we can use
//cmd's buffer space) and copy cmd to that buffer space.
//Then we concatenate cmd and message. Finally, we end our
//buffer data with "\r\n\0" to make sure that server sees end of a file
int FTPClient:: sendMessage(char* cmd, char *message)
{
    char temp[BUFFERSIZE];
    bzero(temp, sizeof(temp));
    strcpy(temp, cmd);
    strcat(temp, message);
    strcat(temp, "\r\n\0");
    if(commandReques == "close")
    {
        return dataSocket->writeTo(temp, strlen(temp));
    }
    else
    {
        return clientSocket->writeTo(temp, strlen(temp));
    }
    
}
//syst request.
bool FTPClient:: syst()
{
    
    char systCMD[BUFFERSIZE], empty[1];
    bzero(systCMD, sizeof(systCMD));
    strcat(systCMD, "SYST");
    sendMessage(systCMD, empty);
    bzero(databuf, sizeof(databuf));
    clientSocket->readBuffer(databuf, sizeof(databuf));
    if(getCode(databuf) == 215)
    {
        cout << databuf;
    }
    
    return true;
    
}

//gets first few bytes from a buffer to get its code number
//and returns code as an int value
int FTPClient:: getCode(char buf[])
{
    char code[2];
    strncpy(code, buf, 3);
    return atoi(code);
}

//passive open request. We send message to server with PASV cmd
//and get new ip address from server that gets saved in buffer.
//We then send this buffer to calculatePASSV method that
//would take buffer and converts whats inside to IP address
void FTPClient:: passiveOpen()
{
    char buffer[BUFFERSIZE], empty[1];
    bzero(buffer, sizeof(buffer));
    bzero(empty, sizeof(empty));
    strcat(buffer, "PASV");
    sendMessage(buffer, empty);
    bzero(databuf, sizeof(databuf));
    clientSocket->readBuffer(databuf, sizeof(buffer));
    if(getCode(databuf) == 227)
    {
        cout << databuf;
        calculatePASSV(databuf);
    }
    else
    {
        cerr << "Error occurred from retrieving command" << endl;
        cout << databuf;
    }
    
}

//convers whats inside buffer to an ip address.
//There are couple of ways to do that but I used vector to store
//individual numbers and then used stringstream to make an ip like data from it
void FTPClient:: calculatePASSV(char buffer[])
{
    vector<int> values;
    char temp[25];
    for(int i = 0; i < strlen(buffer); i++)
    {
        if(buffer[i] == '(')
        {
            strncpy(temp, buffer + i, 24);
            stringstream stm(temp);
            bzero(temp, sizeof(temp));
            stm.get();
            for(int i = 0; i < 6; i++)
            {
                stm.getline(temp, 5, ',');
                values.push_back(atoi(temp));
            }
            
            break;
        }
    }
    int port = (int)values[4] * 256 + (int)values[5];
    stringstream newIP;
    newIP << values[0];
    for(int i = 1; i < 4; i ++)
    {
        newIP << '.' << values[i];
    }
    dataSocket = new Socket();
    dataFD = dataSocket->openCientSocket(newIP.str().c_str(), port);
}

//LS command. Bahaves similar to any other command, except that it uses readDate
//function from Socket class (recall that readDate function is used with this
//command only). readData can read enormous amount of data.
//Commented out area was intended to be used first, but child was behaving
//weird in some cases; therefore I commented it out
void FTPClient:: lsCMD()
{
    passiveOpen();
    char buffer[BUFFERSIZE], empty[0];
    bzero(buffer, sizeof(buffer));
    strcat(buffer, "LIST");

    sendMessage(buffer, empty);
    
    clientSocket->readFrom(databuf, sizeof(databuf));
    cout << databuf;
    
    dataSocket->readData(cout);
    
    clientSocket->readFrom(databuf, sizeof(databuf));
    cout << databuf;
    
    dataSocket->closeSD();
    /*
    if(fork() == 0)
    {
        sendMessage(buffer, empty);
        
        clientSocket->readFrom(databuf, sizeof(databuf));
        cout << databuf;
        
        dataSocket->readData(cout);
        
        clientSocket->readFrom(databuf, sizeof(databuf));
        cout << databuf;
    }

    else
    {
        wait(0);
    }
    dataSocket->closeSD();
    */
    
}

//change directory function.
//prompts user to give directory address.
//Going back works too
void FTPClient:: cdCMD()
{
    char buffer[10], dirBuffer[50];
    bzero(buffer, sizeof(buffer));
    bzero(dirBuffer, sizeof(dirBuffer));
    string dir;
    cin >> dir;
    strTochar(dirBuffer, dir);
    strTochar(buffer, "CWD ");
    sendMessage(buffer, dirBuffer);
    bzero(databuf, sizeof(databuf));
    clientSocket->readBuffer(databuf, sizeof(databuf));
    cout << databuf;
   
    
}
//quit command
//Sends command, reads message from server, and finally closes socket
void FTPClient:: quitCMD()
{
    char buffer[BUFFERSIZE], empty[0];
    bzero(buffer, sizeof( buffer));
    strcat(buffer, "QUIT");
    sendMessage(buffer, empty);
    clientSocket->readFrom(databuf, sizeof(databuf));
    cout << databuf;
    clientSocket->closeSD();
}

//close command
//It closes dataSD. You can still operate but.
//All it does is it closes data SD so that no
//data is being sent
void FTPClient:: closeConnection()
{
    char buffer[BUFFERSIZE], empty[0];
    bzero(buffer, sizeof( buffer));
    strcat(buffer, "QUIT");
    sendMessage(buffer, empty);
    cout << "221 Goodbye..." << endl;
    dataSocket->closeSD();
    //clientSocket->shutDownSD();
}

//get command.
//initiates request (TYPE I -> RETR), opens file to write
//and create a file that it will write to. We call readData that
//will read buffer (file from a server) and at the same time writes
//to ofstream. Finally we close file and close dataSocket
void FTPClient:: getCMD()
{
    string remoteFile;
    cin >> remoteFile;
    
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int file = open( remoteFile.c_str(), O_WRONLY | O_CREAT, mode );
    
    passiveOpen();
    
    char buffer[BUFFERSIZE], empty[0], fileBuffer[50];
    bzero(buffer, sizeof(buffer));
    bzero(fileBuffer, sizeof(fileBuffer));
    strcat(buffer, "TYPE I");
    sendMessage(buffer, empty);
    
    clientSocket->readFrom(databuf, sizeof(databuf));
    cout << databuf;
    
    bzero(buffer, sizeof(buffer));
    strTochar(fileBuffer, remoteFile);
    strcat(buffer, "RETR ");
    sendMessage(buffer, fileBuffer);
    
    ofstream stream;
    stream.open(remoteFile.c_str(), ofstream:: out | ofstream:: binary);
    dataSocket->readData(stream);
    stream.close();
    
    clientSocket->readFrom(databuf, sizeof(databuf));
    cout << databuf;
    
    dataSocket->closeSD();
    
    
}

//put command
//Behaves similar to get cmmand.
//We open file, if doesnt extst we create it. Then
//we read content of a file and write to a file that we will
//store on a server.
void FTPClient:: putCDM()
{
    cout << "(local-file) ";
    string localFile;
    cin >> localFile;
    cout << "(remote-file) ";
    string remoteFile;
    cin >> remoteFile;
    
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int file = open( localFile.c_str(), O_WRONLY | O_CREAT, mode );
    
    passiveOpen();
    
    char buffer[BUFFERSIZE], empty[0], fileBuffer[50];
    bzero(buffer, sizeof(buffer));
    bzero(fileBuffer, sizeof(fileBuffer));
    strcat(buffer, "TYPE I");
    sendMessage(buffer, empty);
    
    clientSocket->readFrom(databuf, sizeof(databuf));
    cout << databuf;
    
    bzero(buffer, sizeof(buffer));
    strTochar(fileBuffer, remoteFile);
    strcat(buffer, "STOR ");
    sendMessage(buffer, fileBuffer);
    
    ifstream stream;
    stream.open(localFile.c_str(), ifstream:: in | ofstream:: binary);
    
    while(!stream.eof())
    {
        stream.read(buffer, sizeof(buffer));
        dataSocket->writeTo(buffer, strlen(buffer));
    }
    
    clientSocket->readFrom(databuf, sizeof(databuf));
    cout << databuf;
    
    dataSocket->closeSD();
    
}

//sets the name of command passed (used with sendMessage function)
void FTPClient:: setCommandRequest(string name)
{
    commandReques = name;
}








