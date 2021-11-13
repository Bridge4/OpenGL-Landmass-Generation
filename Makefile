EXE=final

# Main target
all: $(EXE)

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lglfw3 -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglfw3 -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
final.o: final.cpp CSCIx229.h compileshaders.h

#  Create archive
CSCIx229.a:final.o projection.o compileshaders.o
	ar -rcs $@ $^
projection.a: final.o CSCIx229.o
compileshaders.a: final.o CSCIx229.o
# Compile rules
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
final:final.o   CSCIx229.a
	g++ $(CFLG) -o $@ $^  $(LIBS)
projection:projection.o final.o compileshaders.o CSCIx229.a 
	g++ $(CFLG) -o $@ $^  $(LIBS)

compileshaders:compileshaders.o projection.o final.o CSCIx229.a
	g++ $(CFLG) -o $@ $^  $(LIBS)
#  Clean
clean:
	$(CLEAN)