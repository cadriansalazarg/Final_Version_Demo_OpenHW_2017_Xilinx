#

# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#               (dependencies are added to end of Makefile)
#

# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -g

LIBS = -lm

DIRECTIVE = -DNative_IT -DNative_IW -DNative_IT

# define the C++ source files
SRCS = infoli.cpp  infoli_tb.cpp

# define the C++ object files 

OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = FinalProgram


.PHONY: depend clean

all:	$(MAIN)


$(MAIN):	$(OBJS) 
		$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS) $(DIRECTIVE)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
		
	$(CC) $(CFLAGS)  -c $<  -o $@ 
		
clean:
	$(RM) *.o *~ $(MAIN)
	rm  -f InferiorOlive_Output.txt
	rm  -f InferiorOlive_OutputVdend.txt
	

# DO NOT DELETE THIS LINE -- make depend needs it
