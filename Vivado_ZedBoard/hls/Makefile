
#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.

#Copyright <YEAR> <COPYRIGHT HOLDER>

#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.




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
