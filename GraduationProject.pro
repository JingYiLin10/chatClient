#-------------------------------------------------
#
# Project created by QtCreator 2018-03-13T15:04:14
#
#-------------------------------------------------

QT       += core gui\
        network\
        sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraduationProject
TEMPLATE = app


SOURCES += main.cpp\
        chatsoft.cpp \
    logininterfase.cpp \
    chatinterfase.cpp \
    mylineedit.cpp

HEADERS  += chatsoft.h \
    logininterfase.h \
    chatinterfase.h \
    mylineedit.h

FORMS    += chatsoft.ui

DISTFILES += \
    img/loginInterfasebg.jpg \
    img/tencentIcon.png \
    img/rightImg.png \
    img/chatInterfaseTitle.png \
    img/add11.png \
    img/add22.png \
    img/user11.png \
    img/user22.png \
    img/close.png \
    img/headIcon.png \
    img/mini.png \
    img/setting.png

RC_FILE += exeicon.rc
