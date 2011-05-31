
###########################################################################
## Adjust the following two variables to match your project.             ##
###########################################################################

# Only specify source files, not header files.
# Lists of files can be denoted by white-space separated filenames across
# one or more lines.  Use a backslash (\) at the end of a line if the list
# continues to the next line, e.g.
#
# SOURCE_FILES  =       \
#                       myfile1.cpp myfile2.cpp \
#                       myfile3.cpp      <- No backslash on last line!

SOURCE_FILES	=	\
			main.cpp	\
			cfg.cpp

BINARY_FILE	=	mycompiler



###########################################################################
## You should not need to change anything below this line.               ##
###########################################################################

ROSE_INSTALL = /class/cse756/rose-0.9.4a
CXX = g++
LDFLAGS = -lrose -L$(ROSE_INSTALL)/rose/lib -L$(ROSE_INSTALL)/boost/lib -g -Wl,-rpath $(ROSE_INSTALL)/rose/lib -Wl,-rpath $(ROSE_INSTALL)/boost/lib
CXXFLAGS = -I$(ROSE_INSTALL)/rose/include -I$(ROSE_INSTALL)/boost/include/boost-1_37 -g 

TEMP0_FILES = $(SOURCE_FILES:.cpp=.o)
TEMP1_FILES = $(TEMP0_FILES:.C=.o)
OBJECT_FILES = $(TEMP1_FILES:.cc=.o)

all		:	$(BINARY_FILE)

$(BINARY_FILE)	:	$(OBJECT_FILES)
			$(CXX) -o $(BINARY_FILE) $(LDFLAGS) $(OBJECT_FILES)
                        
.cpp.o          :
			$(CXX) -c $(CXXFLAGS) -o $@ $<
.C.o            :
			$(CXX) -c $(CXXFLAGS) -o $@ $<
.cc.o           :
			$(CXX) -c $(CXXFLAGS) -o $@ $<

clean		:
			rm -f *.o
			rm -f out*.c
			rm $(BINARY_FILE)

cleantest:
	rm -f out*.c

cfg.dot: mycompiler cfg_test.c
	./mycompiler cfg_test.c

cfg.png: cfg.dot
	dot -Tpng cfg.dot > cfg.png

commitpj4: pretty.cpp
	cp pretty.cpp intermediate1.cpp
	submit c756aa lab4 intermediate1.cpp
	diff pretty.cpp intermediate1.cpp

commitpj5: pretty.cpp
	cp pretty.cpp intermediate2.cpp
	submit c756aa lab5 intermediate2.cpp
	diff pretty.cpp intermediate2.cpp

commitpj6: cfg.cpp
	submit c756aa lab6 cfg.cpp

