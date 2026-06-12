# Installation paths
CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio2211/cplex
CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio2211/concert

# Compilation flags
CCOPT         = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD

# Header directories
CPLEXINCDIR   = $(CPLEXDIR)/include
CONCERTINCDIR = $(CONCERTDIR)/include

# Static library directories
CPLEXLIBDIR   = $(CPLEXDIR)/lib/x86-64_linux/static_pic
CONCERTLIBDIR = $(CONCERTDIR)/lib/x86-64_linux/static_pic

# Linking flags
CCLNFLAGS     = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread -ldl

# Find all .cpp files in the current folder.
SOURCES = $(wildcard *.cpp)
# Define the name of the executable files by removing the ".cpp" extension.
TARGETS = $(SOURCES:.cpp=)

# Rule to type only 'make'
all: $(TARGETS)

# Rule for transforming any .cpp file into an executable file.
%: %.cpp
	g++ $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) $< -o $@ $(CCLNFLAGS)

# Rule for clearing the folder.
clean:
	rm -f $(TARGETS)
