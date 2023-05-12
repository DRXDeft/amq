CPP=g++
CPPFLAGS=-std=c++11  -I../include/libbloom -I../include/BBHash -I ~/suffix_array/lib/include/
LIB=-L../include/libbloom/build/ -lbloom -lpthread -L ~/suffix_array/lib/lib -lsdsl -ldivsufsort -ldivsufsort64 -DNDEBUG
all: task1 task2 task3

task1: task1.o
	$(CPP) -o task1 task1.o $(LIB)

task2: task2.o
	$(CPP) -o task2 task2.o $(LIB)


task3: task3.o
	$(CPP) -o task3 task3.o $(LIB)


%.o: %.cpp 
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	rm *.o task1 task2 task3