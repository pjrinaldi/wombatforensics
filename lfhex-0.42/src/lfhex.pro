# $Id: lfhex.pro,v 1.2 2008-09-11 01:49:00 salem Exp $
TARGET  = lfhex
VERSION = 0.42
TEMPLATE = app
SOURCES = reader.cpp \
          save.cpp \
          translate.cpp \
          hexEditor.cpp \
          hexGui.cpp \
          grid.cpp \
          compareDlg.cpp \
          conversionDlg.cpp \
          cursor.cpp \
          delta.cpp \
          offsetConstraint.cpp \
          driver.cpp
HEADERS = box.hpp \
          compareDlg.hpp \
          conversionDlg.hpp \
          cursor.hpp \
          delta.hpp \
          grid.hpp \
          hexEditor.hpp \
          hexGui.hpp \
          offsetConstraint.hpp \
          reader.hpp \
          save.hpp \
          translate.hpp \
          expr.h \
          local.h \
          mappings.h

# The following defines are used to select large file offsets.
# If your OS does not support this comment it out.
DEFINES += _LARGEFILE_SOURCE _FILE_OFFSET_BITS=64
# If your machine is big endian then uncomment the following, otherwise the
# offset labels will be in little endian (which some find hard to read).
# DEFINES += WORDS_BIGENDIAN
lfhex.path  = /usr/local/bin
lfhex.files = lfhex
INSTALLS += lfhex
RESOURCES = images.qrc
YACCSOURCES += expr.y
LEXSOURCES += expr.l
QMAKE_LEX  = flex
QMAKE_YACC = bison
QMAKE_YACCFLAGS = -d -b expr -p expr
DISTFILES += ../README \
             ../README.install \
             ../COPYING \
             $$YACCSOURCES \
             $$LEXSOURCES \
             img/exit.xbm \
             img/first.xbm \
             img/last.xbm \
             img/next.xbm \
             img/prev.xbm
# Uncomment this to compile in debug mode
# CONFIG += debug

