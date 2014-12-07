#
# GNU Makefile for the picking project
#
# Michael Shafae
# mshafae at fullerton.edu
#
# This makefile has been edited by Brian Mitzel for use in CPSC 486
# Assignment 2 - Picking
#
# It depends on the config.guess program that is in the config directory
# to guess what configuration to use for the build.
#

SHELL = /bin/sh
SYSTEM ?= $(shell config/config.guess | cut -d - -f 3 | sed -e 's/[0-9\.]//g;')
SYSTEM.SUPPORTED = $(shell test -f config/Makefile.$(SYSTEM) && echo 1)

ifeq ($(SYSTEM.SUPPORTED), 1)
include config/Makefile.$(SYSTEM)
else
$(error "Platform '$(SYSTEM)' not supported")
endif


TARGET = vfculling
# C++ Files
CXXFILES =   vfculling.cpp Camera.cpp Model.cpp PlyModel.cpp Point3.cpp Ray.cpp Scene.cpp Vec3.cpp Vec4.cpp VecMath.cpp
CFILES =  
# Headers
HEADERS =  Camera.h GLSLShader.h Model.h PlyModel.h Point3.h Ray.h Scene.h Vec3.h Vec4.h VecMath.h

OBJECTS = $(CXXFILES:.cpp=.o) $(CFILES:.c=.o)

DEP = $(CXXFILES:.cpp=.d) $(CFILES:.c=.d)

default all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LLDLIBS)

-include $(DEP)

%.d: %.cpp
	set -e; $(CXX) -MM $(CFLAGS) $< \
	| sed 's/($*).o[ :]*/.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.d: %.c
	set -e; $(CXX) -MM $(CFLAGS) $< \
	| sed 's/($*).o[ :]*/.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

%.o: %.c
	$(CXX) $(CFLAGS) -c $<

clean:
	-rm -f $(OBJECTS) core $(TARGET).core *~

spotless: clean
	-rm -f $(TARGET) $(DEP)
