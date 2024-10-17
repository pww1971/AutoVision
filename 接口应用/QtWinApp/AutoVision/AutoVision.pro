QT       += core gui
QT   += winextras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += User32.LIB
LIBS += Gdi32.LIB

CONFIG += c++17
INCLUDEPATH += ../../../src/opencv/include \
../../../src/procSrc  \
../../../src/deviceSrc    \
../../../src/mysql_include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../../src/deviceSrc/automove.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



unix|win32: LIBS += -lmodbus

unix|win32: LIBS += -ldog_windows_x64_3159259

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../x64/release/ -lopencv_world500
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../x64/debug/ -lopencv_world500d
else:unix: LIBS += -L$$PWD/../../../x64/ -lopencv_world500d

INCLUDEPATH += $$PWD/../../../src/opencv/include
DEPENDPATH += $$PWD/../../../src/opencv/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../x64/release/ -lprocLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../x64/debug/ -lprocLib
else:unix: LIBS += -L$$PWD/../../../x64/ -lprocLib

INCLUDEPATH += $$PWD/../../../src/procSrc/include
DEPENDPATH += $$PWD/../../../src/procSrc/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../x64/release/ -lmvlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../x64/debug/ -lmvlib
else:unix: LIBS += -L$$PWD/../../../x64/ -lmvlib

INCLUDEPATH += $$PWD/../../../src/deviceSrc/include
DEPENDPATH += $$PWD/../../../src/deviceSrc/include
