QT       += core gui
QT       += network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    connection.cpp \
    connectwindow.cpp \
    createroomwindow.cpp \
    main.cpp \
    menuwindow.cpp \
    paintscene.cpp \
    paintwindow.cpp \
    peermanager.cpp \
    player.cpp \
    resultrecord.cpp \
    resultwindow.cpp \
    roomrecord.cpp \
    server.cpp

HEADERS += \
    client.h \
    connection.h \
    connectwindow.h \
    createroomwindow.h \
    menuwindow.h \
    paintscene.h \
    paintwindow.h \
    peermanager.h \
    player.h \
    resultrecord.h \
    resultwindow.h \
    roomrecord.h \
    server.h

FORMS += \
    connectwindow.ui \
    createroomwindow.ui \
    menuwindow.ui \
    paintwindow.ui \
    resultwindow.ui \
    roomrecord.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
