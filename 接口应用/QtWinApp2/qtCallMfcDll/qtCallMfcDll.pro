QT       += core gui
QT   += winextras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += User32.LIB
LIBS += Gdi32.LIB
CONFIG += c++17
INCLUDEPATH += ../../../src/opencv/include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += _AFXDLL
SOURCES += \
    dyna.cxx \
    main.cpp \
    mainwindow.cpp \
    procInterface.cpp \
    tinyxml2.cpp

HEADERS += \
    critsect.h \
    dyna.h \
    mainwindow.h \
    platform.h \
    procInterface.h \
    tinyxml2.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../x64/release/ -lopencv_world500
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../x64/debug/ -lopencv_world500d
else:unix: LIBS += -L$$PWD/../../../x64/ -lopencv_world500d

INCLUDEPATH += $$PWD/../../../src/opencv/include
DEPENDPATH += $$PWD/../../../src/opencv/include
