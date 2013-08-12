TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
   # echo/echo_client.cpp \
   # echo/echo_server.cpp

HEADERS += \
    socket.hpp \
    dns.hpp \
    unix_error.hpp

