TARGET = tp_data
TEMPLATE = lib

DEFINES += TDP_DATA_LIBRARY

SOURCES += src/Globals.cpp
HEADERS += inc/tp_data/Globals.h

SOURCES += src/AbstractMember.cpp
HEADERS += inc/tp_data/AbstractMember.h

SOURCES += src/AbstractMemberFactory.cpp
HEADERS += inc/tp_data/AbstractMemberFactory.h

SOURCES += src/Collection.cpp
HEADERS += inc/tp_data/Collection.h

SOURCES += src/CollectionFactory.cpp
HEADERS += inc/tp_data/CollectionFactory.h

#-- Members ----------------------------------------------------------------------------------------
SOURCES += src/members/StringMember.cpp
HEADERS += inc/tp_data/members/StringMember.h
