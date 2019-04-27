newrecordfile.o: newrecordfile.cpp newrecordfile.h
	g++ -c -std=c++11 -pedantic-errors $<
final.o:
	g++ -c -std=c++11 -pedantic-errors final.cpp
main: final.o newrecordfile.o
	g++ $^ -o $@
run: main
	./main
clean: 
	rm *.o
.PHONY: run clean
