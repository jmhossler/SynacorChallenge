debugger: debugger.o virMach.o stack.o mem.o
	g++ -Wall -std=c++0x -o debugger debugger.o virMach.o stack.o mem.o -lpanel -lncurses

debugger.o: ../stack.h ../virMach.h ../mem.h
	g++ -Wall -std=c++0x -c debugger.cpp

virMach.o: ../virMach.h ../stack.h ../mem.h
	g++ -Wall -std=c++0x -c ../virMach.cpp -lncurses

stack.o: ../stack.h
	g++ -Wall -std=c++0x -c ../stack.cpp

mem.o: ../mem.h
	g++ -Wall -std=c++0x -c ../mem.cpp

clean:
	rm *.o debugger

test:
	debugger ../../challenge.bin
