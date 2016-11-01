QT += widgets
CONFIG += c++11

QMAKE_CXX = clang
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -pedantic -pedantic-errors
QMAKE_STRIP = true

SOURCES = qnotify.cpp

target.path = /usr/bin

INSTALLS += target
