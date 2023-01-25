#This is the makefile for this experimentation section of the OPENGL project.
#Makefile template used from CS70 Assignment Template
TARGETS = OPENGL.exe Triangle.exe ExampleOPENGL.exe

#Flags
OPTFLAGS = 
DBGFLAGS =    

#compiler choice
CXX = g++ 

#Other Flags
CXXFLAGS = -O0 -g3 -Wall -c -fmessage-length=0  $(DBGFLAGS) $(OPTFLAGS)
CPPFLAGS =                  # Used for -D, -isystem and -I preprocessor options
LDFLAGS  =  -lglu32 -lopengl32 -lfreeglut  # Used for linking-related options (-l, etc.)

all: $(TARGETS)

#
#
#
#Rules for linking and creating the executable
#
#
#


#Equivalent command without fancy syntactic sugar
# g++ -o OPENGL OPENGL.o -lglu32 -lopengl32 -lfreeglut
OPENGL.exe: OPENGL.o
	$(CXX) -o $@ $^ $(LDFLAGS)

Triangle.exe: Triangle.o
	$(CXX) -o $@ $^ $(LDFLAGS)

ExampleOPENGL.exe: ExampleOPENGL.o
	$(CXX) -o $@ $^ $(LDFLAGS)



#
#
#
#Rules for creating the object files
#
#
#

#Equivalent command without fancy syntactic sugar
# g++ -O0 -g3 -Wall -c -fmessage-length=0 -o OPENGL.o "OPENGL.cpp"
OPENGL.o: OPENGL.cpp
	$(CXX) $(CXXFLAGS) -o OPENGL.o "OPENGL.cpp"

Triangle.o: Triangle.cpp
	$(CXX) $(CXXFLAGS) -o Triangle.o "Triangle.cpp"

ExampleOPENGL.o: ExampleOPENGL.cpp
	$(CXX) $(CXXFLAGS) -o ExampleOPENGL.o "ExampleOPENGL.cpp"


#rule to clean everything (Needs to be fixed)
clean:
	rm -rf *.exe $(TARGETS)

