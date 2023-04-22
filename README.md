# tcpchat
## Груповой сервер-клиент чат
### Описание
Разработано серверное приложение, которое взаимодействует с клиентами через tcp-соединения. Клиенты подключаются к серверу на его порт, проходят легкую авторизацию, после чего предоставляется полный доступ к потоку сообщений.
### Установка
Для успешной работы приложения необходимо подлючить библиотеку Boost. 
1. После успешного скачивания проекта, переходим в папку builds
2. Пишем cmake ../scr 
3. После успешного создания Makefile в этой же директории запускаем его
4. В случае успешной компиляции появятся 3 файла для запуска server client test
### Технологии
С++
Boost
