# SCGI-server
## Описание
Реализация асинхронного `SCGI` сервера с использованием `boost::asio::spawn`.

Структура проекта:
* **main.cpp** - основной файл;
* **CMakeLists.txt** - параметры сборки;
* **async_server.hpp** -  описание и реализация класса сервера *async_server*;
* **session.hpp** - описание класса сессии *session*;
* **session.cpp** - реализация класса сессии *session*;
* **parser.hpp** - описание структур для хранения параметров запросов и ответов и функций парсинга;
* **parser.cpp** - реализация функций парсинга запросов и формирования ответов сервера.

Сервер настроен под сервер `nginx` с конфигурацией:

    server {
          location / {
            include   scgi_params;
            scgi_pass localhost:8080;
        }
    }

Требуемые параметры командной строки при запуске: число портов, размер буфера, число потоков.

## Установка
Для успешной работы реализации требуется **последняя** версия `Boost` (на момент написания реализации - **1.71.0**).

Инструкция по установке:
1. cmake .
2. make
3. ./SCGI-server

## Ссылки
* Официальная спецификация протокола `SCGI`: https://python.ca/scgi/protocol.txt
* Документация `Boost:Asio`: https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio.html

