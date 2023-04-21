#pragma once
#include<iostream>
#include<iomanip>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>
#include<string>
#include<cstring>
#include<thread>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#define SERVER_IP "127.0.0.1"
#define PORT 50400
#define ERROR_S "ERROR:"
#define CLIENT_END_CONECTION '#'

namespace logging = boost::log;

namespace keywords = boost::log::keywords;

using namespace std;

class AcceptedSocket{
    private:
        int acceptedSock; //сам сокет
        struct sockaddr_in address;//его адресс
        int error;// ошибка в случае сбоя
        bool acceptedSuccessfully;//показатель успешного подключения
        string name;//имя пользователья ПОКА ХЗ КУДА И КАК ПРИМЕНИТЬ НО ОЧЕНЬ НАДО!

    public:
        AcceptedSocket()
        {

        }

        AcceptedSocket(int sockFD, struct sockaddr_in sock_address)
        {
            this->acceptedSock = sockFD;
            this->address = sock_address;
            this->acceptedSuccessfully = sockFD>0;
            if(!this->acceptedSuccessfully)
                this->error = sockFD;
        }

        ~ AcceptedSocket()
        {

        }

        string GetName()
        {
            return this->name;
        }

        void SetName(string clientName)
        {
            this->name = clientName;
        }

        int GetSock()
        {
            return this->acceptedSock;
        }

};

void DialogIncoming(AcceptedSocket *pSocket);

void StartAcceptIncomingConnections(int server);

void DialogOnSeparateThread(AcceptedSocket *pSocket);

void sendReceivedMessageToALL(char *buffer, int sockFD);

AcceptedSocket acceptIncomingConnection(int server);

bool client_end(const char* nsg);

void startListenMessagesAndPrint(int client);

void ListenAndPrint(int client);

void initServerLogging();

void inintClientLogging();
