TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lmysqlclient
LIBS += -lpthread
INCLUDEPATH +=./include/
INCLUDEPATH +=./src/
INCLUDEPATH +=./hero/
SOURCES += \
        croommanger.cpp \
        gamekernel.cpp \
        player.cpp \
        src/Mysql.cpp \
        src/TCPKernel.cpp \
        src/TCPNet.cpp \
        src/Thread_pool.cpp \
        src/err_str.cpp \
        src/main.cpp

HEADERS += \
    croommanger.h \
    gamekernel.h \
    hero/Ccaocao.h \
    hero/Cliubei.h \
    hero/Csunquan.h \
    hero/hero.h \
    include/Mysql.h \
    include/TCPKernel.h \
    include/TCPNet.h \
    include/Thread_pool.h \
    include/err_str.h \
    include/packdef.h \
    player.h

DISTFILES += \
    src/Mysql.cpp.bak \
    src/makefile
