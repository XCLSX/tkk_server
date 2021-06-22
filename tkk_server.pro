TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lmysqlclient
LIBS += -lpthread
INCLUDEPATH +=./include/
INCLUDEPATH +=./src/
SOURCES += \
        cardmanger.cpp \
        croommanger.cpp \
        src/Mysql.cpp \
        src/TCPKernel.cpp \
        src/TCPNet.cpp \
        src/Thread_pool.cpp \
        src/err_str.cpp \
        src/main.cpp

HEADERS += \
    cardmanger.h \
    croommanger.h \
    include/Mysql.h \
    include/TCPKernel.h \
    include/TCPNet.h \
    include/Thread_pool.h \
    include/err_str.h \
    include/packdef.h

DISTFILES += \
    src/Mysql.cpp.bak \
    src/makefile
