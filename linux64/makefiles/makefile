##############################################################################
#									     #
#   makefile of MCFSolve						     #
#									     #
#                                VERSION 1.20				     #
#                	        31 - 01 - 2001				     #
#									     #
# 		               Implementation by:			     #
#									     #
#			       Antonio Frangioni			     #
#									     #
#   			   Operations Research Group			     #
#			  Dipartimento di Informatica			     #
#   			     Universita' di Pisa			     #
#									     #
##############################################################################

# module name
NAME = MCFSolve

# switches
SW = -O3 -g

# libreries
LIB = -lm

# compiler
CC = g++

# default target- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

default: $(NAME)

# clean - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean::
	rm -f *.o *~ $(NAME)

# main module (linking phase) - - - - - - - - - - - - - - - - - - - - - - - -

OBJ =   MCFSimplex.o Main.o

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIB) $(SW)

# dependencies: every .o from its .C + every recursively included .h- - - - -

MCFSimplex.o: MCFSimplex.cpp MCFSimplex.h MCFClass.h OPTUtils.h
	$(CC) -c $*.cpp -o $@ $(SW)

Main.o: Main.cpp MCFSimplex.h MCFClass.h OPTUtils.h
	$(CC) -c $*.cpp -o $@ $(SW)

############################ End of makefile #################################
