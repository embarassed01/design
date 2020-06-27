#-------------------------------------------------
#
# Project created by QtCreator 2020-04-18T10:00:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-design
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    my_qt_common.cpp \
    algo111.cpp \
    crypt/base64.cpp \
    crypt/sha1.cpp \
    algo200.cpp \
    algo300.cpp \
    algo400.cpp \
    crypt/md5.cpp \
    algo500.cpp \
    algo1100.cpp \
    crypt/md4.cpp \
    algo1711.cpp \
    crypt/sha512.cpp \
    crypt/sha384.cpp \
    runcrack.cpp \
    maskprocessor.cpp \
    algo0.cpp \
    algo10.cpp \
    algo20.cpp \
    algo30.cpp \
    algo50.cpp \
    crypt/hmac_md5.cpp \
    crypt/sha256.cpp \
    algo1400.cpp \
    crypt/sha224.cpp \
    crypt/hmac_sha1.cpp \
    algo150.cpp \
    crypt/hmac_sha256.cpp \
    algo1450.cpp \
    crypt/hmac_sha224.cpp \
    crypt/hmac_sha512.cpp \
    crypt/hmac_sha384.cpp \
    crypt/hmac_md4.cpp \
    crypt/pbkdf2_sha256.cpp \
    algo12900.cpp \
    crypt/pbkdf2_md4.cpp \
    crypt/pbkdf2_md5.cpp \
    crypt/pbkdf2_sha1.cpp \
    crypt/pbkdf2_sha224.cpp \
    crypt/pbkdf2_sha384.cpp \
    crypt/pbkdf2_sha512.cpp \
    stl/types.cpp

HEADERS += \
        mainwindow.h \
    my_qt_common.h \
    algo111.h \
    crypt/base64.h \
    crypt/sha1.h \
    algo200.h \
    algo300.h \
    algo400.h \
    crypt/md5.h \
    algo500.h \
    algo1100.h \
    crypt/md4.h \
    algo1711.h \
    crypt/sha512.h \
    crypt/sha384.h \
    runcrack.h \
    maskprocessor.h \
    interface_crack.h \
    algo0.h \
    algo10.h \
    algo20.h \
    algo30.h \
    algo50.h \
    crypt/hmac_md5.h \
    crypt/sha256.h \
    algo1400.h \
    crypt/sha224.h \
    crypt/hmac_sha1.h \
    algo150.h \
    crypt/hmac_sha256.h \
    algo1450.h \
    crypt/hmac_sha224.h \
    crypt/hmac_sha512.h \
    crypt/hmac_sha384.h \
    crypt/hmac_md4.h \
    crypt/pbkdf2_sha256.h \
    algo12900.h \
    crypt/pbkdf2_md4.h \
    crypt/pbkdf2_md5.h \
    crypt/pbkdf2_sha1.h \
    crypt/pbkdf2_sha224.h \
    crypt/pbkdf2_sha384.h \
    crypt/pbkdf2_sha512.h \
    stl/types.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
