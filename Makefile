
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
			error.cpp	\
			pretty.cpp

BINARY_FILE	=	mycompiler



###########################################################################
## You should not need to change anything below this line.               ##
###########################################################################

ROSE_INSTALL = /class/cse756/rose-0.9.4a
CXX = g++
LDFLAGS = -lrose -L$(ROSE_INSTALL)/rose/lib -L$(ROSE_INSTALL)/boost/lib -g -O3 -Wl,-rpath $(ROSE_INSTALL)/rose/lib -Wl,-rpath $(ROSE_INSTALL)/boost/lib
CXXFLAGS = -I$(ROSE_INSTALL)/rose/include -I$(ROSE_INSTALL)/boost/include/boost-1_37 -g -O3

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
			rm $(BINARY_FILE)


