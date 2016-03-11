//
//  FTPClient.cpp
//  
//
//  Created by Artem Kovtunenko on 3/5/16.
//
//

#include "FTPClient.h"

FTPClient:: FTPClient()
{
    clientSocket = new Socket();
    
}
FTPClient:: ~FTPClient()
{
    delete clientSocket;
}
FTPClient:: FTPClient(const char * serverURL)
{
    while(true)
    {
        char buffer[BUFFERSIZE];
        bzero(buffer, sizeof(buffer));
        url = serverURL;
        clientSocket =new Socket();
        int clientSD = clientSocket->openCientSocket(serverURL, 21);
        //cout << serverURL << endl;
        clientSocket->readBuffer(buffer, sizeof(buffer));
        int c = getCode(buffer);
        if(getCode(buffer) == 220)
        {
            cout << buffer;
            break;
        }
    }
    
    
    
}
int FTPClient:: openConnection()
{
    return 0;
}
char* FTPClient:: strTochar(char data[], string str)
{
    //char data[BUFFERSIZE];
    strncpy(data, str.c_str(), str.length());
    return data;
}
void FTPClient:: userName()
{
    while(true)
    {
        cout << "Name (" << url << ":" << getlogin() << "): ";
        char userName[BUFFERSIZE], cmdUser[BUFFERSIZE], temp[BUFFERSIZE];
        
        
        
        bzero(userName, sizeof(userName));
        bzero(cmdUser, sizeof(cmdUser));
        bzero(temp, sizeof(temp));
        
        string name;
        cin >> name;
        strTochar(userName, name);
        strTochar(cmdUser, "USER ");
        sendMessage(cmdUser, userName);
        
        clientSocket->readBuffer(temp, sizeof(temp));
        if(getCode(temp) == 331)
        {
            cout << temp;
            break;
        }
        //cout << endl;
    }
}
void FTPClient:: passwd()
{
    //while(true)
    //{
        cout << "Password: ";
    char passwd[BUFFERSIZE], cmdPass[BUFFERSIZE];//, temp[20000];
    char *temp;
  
        bzero(passwd, sizeof(passwd));
        bzero(cmdPass, sizeof(cmdPass));
        //bzero(temp, sizeof(temp));
        string password;
        cin >> password;
        
        
        strTochar(passwd, password);
        strTochar(cmdPass, "PASS ");
        sendMessage(cmdPass, passwd);
   // sleep(2);
        //clientSocket->readFrom(databuf, sizeof(databuf));
        cout << clientSocket->readBuffer(databuf, sizeof(databuf));
    syst();
    //cout << endl;
    //cout << temp;
    //sleep(3);
        /*if(getCode(temp) == 230)
        {
            cout << temp;
            break;
        }*/
    //sleep(1);
       // cout << databuf;
    //}
    
    

}
int FTPClient:: sendMessage1(char* cmd, char *message)
{
    char temp[BUFFERSIZE];
    bzero(temp, sizeof(temp));
    strcpy(temp, cmd);
    strcat(temp, message);
    strcat(temp, "\r\n\0");
    //return clientSocket->writeTo(temp, strlen(temp));
    //cout << clientSocket->readBuffer(databuf, sizeof(databuf));
    clientSocket->writeTo(temp, strlen(temp));
    cout << clientSocket->readBuffer(databuf, sizeof(databuf));
    return 1;
}
int FTPClient:: sendMessage(char* cmd, char *message)
{
    char temp[BUFFERSIZE];
    bzero(temp, sizeof(temp));
    strcpy(temp, cmd);
    strcat(temp, message);
    strcat(temp, "\r\n\0");
    return clientSocket->writeTo(temp, strlen(temp));
    
}
bool FTPClient:: syst()
{
    
    char systCMD[BUFFERSIZE], empty[1];
    bzero(systCMD, sizeof(systCMD));
    strcat(systCMD, "SYST");
    sendMessage(systCMD, empty);
    bzero(databuf, sizeof(databuf));
    clientSocket->readBuffer(databuf, sizeof(databuf));
    cout << databuf;
    //cout << endl;
    
    return true;
    
}
int FTPClient:: getCode(char buf[])
{
    char code[3];
    strncpy(code, buf, 3);
    return atoi(code);
}
void FTPClient:: passiveOpen()
{
    char buffer[BUFFERSIZE], empty[1];
    char *newIP;
    bzero(buffer, sizeof(buffer));
    bzero(empty, sizeof(empty));
    strcat(buffer, "PASV");
    sendMessage(buffer, empty);
    bzero(databuf, sizeof(databuf));
    clientSocket->readBuffer(databuf, sizeof(buffer));
    cout << databuf;
    calculatePASSV(databuf);
}
void FTPClient:: calculatePASSV(char buffer[])
{
    vector<int> values;
    //char finalBuf[6];
    char temp[25];
    //bzero(finalBuf, sizeof(finalBuf));
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
void FTPClient:: lsCMD()
{
    passiveOpen();
    char buffer[BUFFERSIZE], empty[0];
    char *temp;
    bzero(buffer, sizeof(buffer));
    //bzero(buf, sizeof(buf)); ////////////////////////
    strcat(buffer, "LIST");
    temp = clientSocket->readBuffer(databuf, sizeof(databuf));
    //int c = getCode(temp);
    while(getCode(buffer) != 150)
    {
        temp = clientSocket->readBuffer(databuf, sizeof(databuf));
        //cout << buffer;
    }
    cout << temp;
    sendMessage(buffer, empty);
    //cout << clientSocket->readBuffer(databuf, sizeof(databuf));
    
    //bzero(databuf, sizeof(databuf));
    cout << clientSocket->readBuffer(databuf, sizeof(databuf));
    
    
    bzero(databuf, sizeof(databuf));
    //dataSocket->readFrom(databuf, sizeof(databuf)); //read from dataSocket
    dataSocket->readData(cout);
    cout << dataSocket->readBuffer(databuf, sizeof(databuf));
    
    bzero(databuf, sizeof(databuf));
    
    dataSocket->closeSD();

    cout << clientSocket->readBuffer(databuf, sizeof(databuf));
    
}

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
    cout << clientSocket->readBuffer(databuf, sizeof(databuf));
    //cout << endl;
    
}







