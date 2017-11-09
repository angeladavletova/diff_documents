TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x -pthread
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
LIBS += -pthread

QMAKE_CXXFLAGS_RELEASE += -ffast-math
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

SOURCES += ./src/main.cpp  ./src/diff_searcher.cpp  ./src/document_structure.cpp \
           ./src/io.cpp  ./src/text_parser.cpp
HEADERS += ./src/diff_searcher.h  ./src/document_structure.h  ./src/document_structure.h \
           ./src/io.h  ./src/text_parser.h  ./src/max_subsequence.h
