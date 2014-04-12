### eqmake4 was here ###
TEMPLATE = lib

CONFIG -= qt

SOURCES += \
    wrap.cpp

#DEFINES += NUMPY_EXTRACT

INCLUDEPATH += "/usr/include/python2.7/"
INCLUDEPATH += "/usr/include/boost/"

LIBS += -lpython2.7 -lboost_python-2.7

QMAKE_CXXFLAGS += -fPIC

TARGET = mm

HEADERS += \
    u_bond.h \
    ndarray.h

QMAKE_CXXFLAGS+=-std=c++11

OTHER_FILES += \
    test_script.py
