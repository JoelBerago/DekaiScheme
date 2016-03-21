all: main

main : main.o linkedlist.o 
	g++ $^ -o $@

doc:
	doxygen doxygen.config

%.o : %.cpp
	g++ -c $< -o $@

clean :
	rm -rf *.o main
