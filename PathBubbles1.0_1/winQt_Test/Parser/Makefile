# libsvl makefile for Macintosh
# Mike Erwin, October 2010

TARGETS = libphyParser.a

OBJECTS = TreeNameParser.o GraphIndexParser2.o

CXXFLAGS = -Os -I.

libphyParser.a: ${OBJECTS}
	ar -ru $@ ${OBJECTS}

	#libtool -static -o $@ ${OBJECTS}
# -- older form --

depend:
	${CXX} ${CXXFLAGS} -MM *.cpp

tidy:
	rm -f *.o

clean:
	rm -f ${TARGETS} *.o

# -- from 'make depend' --

TreeNameParser.o: TreeNameParser.cpp TreeNameParser.h
#TreeNameParser2.o: TreeNameParser2.cpp TreeNameParser2.h
GraphIndexParser2.o: GraphIndexParser2.cpp GraphIndexParser2.h
