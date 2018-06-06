#-------------------------------------------------
#
# Project created by QtCreator 2018-05-12T18:38:22
#
#-------------------------------------------------

QT       += core gui sql \
            multimedia \
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hockey
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    puck.cpp \
    striker.cpp \
    game.cpp \
    wall.cpp \
    goal.cpp \
    field.cpp \
    accelerator.cpp \
    box.cpp \
    vectorxy.cpp \
    score.cpp \
    narrator.cpp \
    dialog.cpp \
    login.cpp \
    mainwindow.cpp \
    registro.cpp \
    game_window.cpp \
    get_string.cpp \
    bot.cpp \
    controller.cpp \
    secret_question.cpp \
    help.cpp

HEADERS += \
    game.h \
    puck.h \
    striker.h \
    wall.h \
    goal.h \
    field.h \
    accelerator.h \
    box.h \
    vectorxy.h \
    score.h \
    narrator.h \
    dialog.h \
    login.h \
    mainwindow.h \
    registro.h \
    game_window.h \
    get_string.h \
    bot.h \
    controller.h \
    secret_question.h \
    help.h

RESOURCES += \
    images.qrc \
    sound.qrc

FORMS += \
    dialog.ui \
    login.ui \
    mainwindow.ui \
    registro.ui \
    game_window.ui \
    get_string.ui \
    secret_question.ui \
    help.ui


CONFIG += resources_big
