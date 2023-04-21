#include<iostream>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <thread>
#include"serverclient.h"


int main(){
    setlocale(LC_ALL, "Rus");

    inintClientLogging();
    
    BOOST_LOG_TRIVIAL(debug) << "Создаем сокет клиента! (socket)";
    int client = socket(AF_INET, SOCK_STREAM, 0);
    
    if(client < 0){
        BOOST_LOG_TRIVIAL(error) << "Не может создать сокет клиента!";
        cout << ERROR_S << "establishing socket error" << endl;
        exit(0);
    }
    else
        BOOST_LOG_TRIVIAL(info) << "Сокет успешно создан";

    BOOST_LOG_TRIVIAL(debug) << "Задаем параметры адреса сокета к которому будем подключаться";
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &address.sin_addr);

    BOOST_LOG_TRIVIAL(debug) << "Запрашиваем разрешение на подключение у сокета сервера (connect)"; 
    int ret = connect(client, reinterpret_cast<struct sockaddr*>(&address), sizeof(address));
    if(ret == 0){
        cout<< "Client connected to server successful" << endl;
        BOOST_LOG_TRIVIAL(info) << "Клиент успешно подключился";
    }
    else if (ret < 0)
        BOOST_LOG_TRIVIAL(error) << "Ошибка подключения к серверу"; 
    
    char privet[1024];
    char buffer[1024];

    char *name = NULL;
    size_t nameSize = 0;
    BOOST_LOG_TRIVIAL(debug) << "Получаем сообщение от сервера на запрос имени";
    recv(client, privet, 1024,0);
    cout << privet << endl;
    size_t nameCount = getline(&name, &nameSize, stdin);
    name[nameCount - 1] = 0;
    BOOST_LOG_TRIVIAL(debug) << "Отправляем имя серверу";
    send(client, name, nameSize, 0);
    BOOST_LOG_TRIVIAL(debug) << "Получаем сообщение приветствия";
    recv(client, buffer, 1024,0);
    cout << buffer << endl;
    BOOST_LOG_TRIVIAL(debug) << "Получаем сообщение об онлайне";
    recv(client, buffer, 1024,0);
    cout << buffer << endl;
    recv(client, buffer, 1024,0);
    cout << buffer << endl;
    char* line = NULL;
    size_t lineSize = 0;
    char* nicknames;
    
    BOOST_LOG_TRIVIAL(debug) << "Вызываем startListenMessagesAndPrint";
    startListenMessagesAndPrint(client);

    

    while(true)  
    {
        size_t charCount = getline(&line, &lineSize, stdin);

        line[charCount - 1] = 0;

        if(charCount > 0)
        {
            BOOST_LOG_TRIVIAL(info) << "Сообщение написано";
            if(client_end(line))
            {
                BOOST_LOG_TRIVIAL(info) << "Клиент решил покинуть чат";
                break;
            }
            ssize_t sended = send(client, line, strlen(line), 0);
        }
        
    }

    cout << "Пока, будем ждать слудеющей встречи!"<< endl;
    BOOST_LOG_TRIVIAL(debug) << "Закрываем клиентский сокет (close)";
    close(client);
    
}
