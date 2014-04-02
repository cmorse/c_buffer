
CC=$(CXX)
CC=clang++

CFLAGS=-c -Wall

LIBS=-lboost_unit_test_framework

OBJS=test.o

PROGRAM=test

all: $(PROGRAM)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGRAM): $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $(PROGRAM)
	sh ./runtests.sh

clean:
	@rm -f *.o
	@rm $(PROGRAM)

