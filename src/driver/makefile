driver: driver.o virMach.o stack.o mem.o
	g++ -Wall -std=c++0x -o driver driver.o virMach.o stack.o mem.o -lncurses

driver.o: ../stack.h ../virMach.h ../mem.h
	g++ -Wall -std=c++0x -c driver.cpp

virMach.o: ../virMach.h ../stack.h ../mem.h
	g++ -Wall -std=c++0x -c ../virMach.cpp -lncurses

stack.o: ../stack.h
	g++ -Wall -std=c++0x -c ../stack.cpp

mem.o: ../mem.h
	g++ -Wall -std=c++0x -c ../mem.cpp

clean:
	rm *.o driver

test:
	driver ../../challenge.bin
