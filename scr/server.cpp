#include"serverclient.h"
  

int main(){
    
    initServerLogging();
 
    int server = socket(AF_INET, SOCK_STREAM, 0);
    BOOST_LOG_TRIVIAL(debug) << "Создаем сокет сервера (socket)";
    if(server < 0){
        cout << ERROR_S << "Can't create a socket" << endl;
        BOOST_LOG_TRIVIAL(error) << "Не может создать сокет сервера!";
        return -1;
    }
    else
        BOOST_LOG_TRIVIAL(info) << "Сокет успешно создан";

    BOOST_LOG_TRIVIAL(debug) << "Задаем параметры сокета";
    struct sockaddr_in server_address;
    inet_pton(AF_INET, "0.0.0.0", &server_address.sin_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    socklen_t s_addlen = sizeof(server_address);

    int ret = bind(server, reinterpret_cast<struct sockaddr*>(&server_address), s_addlen);  
    BOOST_LOG_TRIVIAL(debug) << "Привязываем сокет к адресу (bind)"; 
    if(ret < 0){
        cout<< ERROR_S << "binding conection" << endl;
        BOOST_LOG_TRIVIAL(error) << "Ошибка привязки к адресу";
        return -2;
    }
    else
    {
        cout << "SERVER START SUCCESSFULY" << endl;
        BOOST_LOG_TRIVIAL(info) << "Сокет сервера успешно запущен";
    }

    int listenRes = listen(server, SOMAXCONN);
    BOOST_LOG_TRIVIAL(debug) << "Сокет сервера начинает прослушку (listen)";
    if(listenRes < 0)
    {
        cout<< ERROR_S << "Can't listen " << endl;
        BOOST_LOG_TRIVIAL(error) << "Сокет не может слушать сокеты клиентов";
        return -3;
    }

    BOOST_LOG_TRIVIAL(debug) << "Сокет сервера начинает принимать клиентов (Вызываем функцию StartAcceptIncommingConnections)";
    StartAcceptIncomingConnections(server);
    BOOST_LOG_TRIVIAL(debug) << "Сокет сервера выключается (shutdown)";
    shutdown(server, SHUT_RDWR);
 
    return 0;
    
}