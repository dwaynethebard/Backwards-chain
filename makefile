.SUFFIXES: .h .o .cpp

CCMAIN = main.cpp
CC = g++ -g  -std=c++11
CCOPTS = -w
OBJS = main.o
CCEXEC = Backwards

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile 
	@echo Linking $@ . . .
	$(CC) $(CCOPTS) $(OBJS) -o $@ 
 

%.o:	%.cpp
	@echo Compiling $< . . .
	$(CC) -c $(CCOPTS) $<

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)




	