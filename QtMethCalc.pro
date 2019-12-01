QT       += core gui network xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkit webkitwidgets printsupport

TARGET = appCalcShortCircuit
TEMPLATE = app


HEADERS += \
    cwindows.h \
    view/CInterface.h \
    view/CInterface3.h \
    buff/h_struct1.h \
    model/ccalculation.h \
    ccruelty.h \
    cwdgt.h

SOURCES += \
    main.cpp \
    cwindows.cpp \
    view/CInterface.cpp \
    view/CInterface3.cpp \
    model/ccalculation.cpp \
    ccruelty.cpp \
    cwdgt.cpp

RESOURCES += \
    dockwidgets.qrc

QMAKE_CXXFLAGS += -std=gnu++11
