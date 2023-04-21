#include"serverclient.h"

namespace logging = boost::log;
auto now = chrono::system_clock::now();
auto in_time_t = chrono::system_clock::to_time_t(now);
stringstream logDate;

void initServerLogging()
{
    logDate << put_time(localtime(&in_time_t), "%Y-%m-%d.%X");

    logging::add_file_log(
        keywords::file_name = "../logs/" + logDate.str() + " server.log",     
        keywords::auto_flush       = true,            
        keywords::rotation_size = 10 * 1024 * 1024,                      
        keywords::format = "%LineID%. [%TimeStamp%][%ThreadID%] [%Severity%] %Message%");

    logging::add_common_attributes();
}

void inintClientLogging()
{

    logDate << put_time(localtime(&in_time_t), "%Y-%m-%d.%X");

    logging::add_file_log(
        keywords::file_name        ="../logs/" + logDate.str() + " client.log",    
        keywords::auto_flush       = true,               
        keywords::rotation_size    = 10* 1024 * 1024,                     
        keywords::format = "%LineID%. [%TimeStamp%][%ThreadID%] [%Severity%] - %Message%");

    logging::add_common_attributes();
}

AcceptedSocket* acceptedClients = new AcceptedSocket[SOMAXCONN];
int acceptedClientsCount = 0;
AcceptedSocket *client;

AcceptedSocket acceptIncomingConnection(int server){
    struct sockaddr_in client_address;
    socklen_t c_addlen = sizeof(struct sockaddr_in);
    int clientSocket = accept(server, reinterpret_cast<struct sockaddr*>(&client_address), &c_addlen);
    AcceptedSocket acceptedSocket = AcceptedSocket(clientSocket, client_address);
    if(clientSocket > -1)
    {
        cout<<"Client successfully connected!" << endl;
        BOOST_LOG_TRIVIAL(info) << "Клиентский сокет принят";
    }
    else
        BOOST_LOG_TRIVIAL(error) << "Клиентский сокет не может подключиться";   
    

    return acceptedSocket;
}

void StartAcceptIncomingConnections(int server){

    char dialog[1024];
    char privet[1024] = "Здоров! Наш супер-час-сервер приветствует тебя! Как вас звать-величать? ";
    char vernis[1024] = "С возвращением, ";
    char online[1024] = "Сейчас не молчат: ";
    char exit[1024] = "Если хочешь покинуть чат пиши #";
    while(true)
    {
        BOOST_LOG_TRIVIAL(debug) << "Вызываем функцию acceptIncomingConnection";
        acceptedClients[acceptedClientsCount] = acceptIncomingConnection(server);
        send(acceptedClients[acceptedClientsCount].GetSock(), privet, 1024, 0);
        recv(acceptedClients[acceptedClientsCount].GetSock(), dialog,1024,0);
        acceptedClients[acceptedClientsCount].SetName(dialog);
        client = &acceptedClients[acceptedClientsCount];
        cout<< "Client nickname - " << acceptedClients[acceptedClientsCount].GetName()<< endl;
        strcat(vernis, client->GetName().c_str());
        strcat(vernis, " ! в моем замечательном сервере!!");
        send(acceptedClients[acceptedClientsCount].GetSock(), vernis, 1024,0);
        acceptedClientsCount++;
        cout << "Connected Sockets: " << acceptedClientsCount << endl;
        strcpy(vernis, "С возвращением, ");

        for(int i = 0; i < acceptedClientsCount; i++)
        {

            if(acceptedClientsCount == 1)
            {
                BOOST_LOG_TRIVIAL(info) << "В данный момент нет других подключенных пользователей";
                strcat(online, "В данный момент никого нет в сети");
            }

            if(i < acceptedClientsCount - 1)
            {
                BOOST_LOG_TRIVIAL(info) << "В данный момент онлайн есть";
                strcat(online,acceptedClients[i].GetName().c_str());
                strcat(online, " ");
            }
            else
                continue;
            
        }

        send(acceptedClients[acceptedClientsCount - 1].GetSock(), online, 1024,0);
        send(acceptedClients[acceptedClientsCount - 1].GetSock(), exit, 1024, 0);
        strcpy(online,"Сейчас не молчат: ");
        BOOST_LOG_TRIVIAL(debug) << "Переводим клиента на отдельный поток для дальнейшей переписки  вызвывем DialogOnSeparateThread";
        DialogOnSeparateThread(client); //НАЧИНАЕМ ДИАЛОГ
        
    }
    
    
}

void DialogOnSeparateThread(AcceptedSocket *pSocket){
    BOOST_LOG_TRIVIAL(debug) << "Создаем поток для клиентского сокета и переводим в режим переписки и вызываем DialogIncoming";
    thread th(DialogIncoming,pSocket);
    th.detach();
}

void DialogIncoming(AcceptedSocket *pSocket){
    BOOST_LOG_TRIVIAL(debug)<<"Начинаем диалог";
    char buffer [1024];
    char message [1024];
    char disconect[1024];
    char podcluchilca[1024];
    strcpy(podcluchilca, pSocket->GetName().c_str());
    strcat(podcluchilca, " подключился к чату!");
    BOOST_LOG_TRIVIAL(debug)<<"Сообщаем всем о подключении вызывая функцию sendReceivedMessageToAll";
    sendReceivedMessageToALL(podcluchilca,pSocket->GetSock());
    while(true)
    {
        ssize_t Recived = recv(pSocket->GetSock(), buffer, 1024, 0);
        if(Recived > 0)
        {
            BOOST_LOG_TRIVIAL(info)<<"Клиент написал сообщение";
            cout << pSocket->GetSock() << endl;
            buffer[Recived] = 0;
            cout <<pSocket->GetName() << ": " << buffer << endl;
            strcpy(message, pSocket->GetName().c_str());
            strcat(message, ": ");
            strcat(message, buffer);
            BOOST_LOG_TRIVIAL(debug)<<"Рассылаем всем сообщение вызывая функцию sendReceivedMessageToAll";
            sendReceivedMessageToALL(message,pSocket->GetSock());
        } 
        else if (Recived == 0)
        {
                BOOST_LOG_TRIVIAL(info)<<"Клиент покинул покидает чат";
                cout << pSocket->GetSock() << endl;
                strcpy(disconect, pSocket->GetName().c_str());
                strcat(disconect, " покинул чат!");
                cout << disconect << endl;
                BOOST_LOG_TRIVIAL(debug)<<"Сообщаем всем об отключении вызывая функцию sendReceivedMessageToAll";
                sendReceivedMessageToALL(disconect, pSocket->GetSock());
                cout << "Client disconected " << acceptedClientsCount << endl;
                break;
        }
           
    }
    BOOST_LOG_TRIVIAL(debug)<<"Закрываем сокет (close)";
    close(pSocket->GetSock());
    //cout << pSocket->Getaccept()<<endl;
    cout << pSocket->GetSock() << endl;
    acceptedClientsCount --;
    cout<< acceptedClientsCount << endl;
}

void sendReceivedMessageToALL(char *buffer, int sockFD){
    for(int i = 0; i < acceptedClientsCount; i++)
    {
        
        if(acceptedClients[i].GetSock() != sockFD)
            send(acceptedClients[i].GetSock(), buffer, strlen(buffer), 0);
    }
        
}

bool client_end(const char* nsg)
{
    BOOST_LOG_TRIVIAL(debug) << "Проверка сообщения";
    for(int i = 0; i < strlen(nsg); ++i)
    {
        if(nsg[i] == CLIENT_END_CONECTION)
            return true;
        
    }
    return false;
}

void startListenMessagesAndPrint(int client){
    BOOST_LOG_TRIVIAL(debug) << "Создаем потоки для печати сообщений и передаем функцию ListenAndPrint";
    thread th(ListenAndPrint, client);
    th.detach();   
}

void ListenAndPrint(int client){
    char buffer [1024];
    while(true)
    {
        ssize_t Recived = recv(client, buffer, 1024, 0);

        if(Recived > 0)
        {
            buffer[Recived] = 0;
            cout << buffer << endl;
        } 

        if(Recived == 0)
            break;
    }
     //close(client);
    
}