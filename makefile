# ID: *****1536
# EMAIL: yairjacob620@gmail.com

FLAGS = -Wall
O_FILES = building_slot.o land.o board.o player.o catan.o

catan: main
	./main 0 6 0 6 0 6 0 6 0 6 0 6 1

main: $(O_FILES) main.o
	clang++ $(FLAGS) $(O_FILES) main.o -o main

valgrind: main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main

test: $(O_FILES) tests.o
	clang++ $(FLAGS) $(O_FILES) tests.o -o tests
	./tests 

building_slot.o: building_slot.cpp building_slot.hpp
	clang++ $(FLAGS) -c building_slot.cpp

land.o: land.cpp land.hpp
	clang++ $(FLAGS) -c land.cpp

board.o: board.cpp board.hpp
	clang++ $(FLAGS) -c board.cpp

player.o: player.cpp player.hpp
	clang++ $(FLAGS) -c player.cpp

catan.o: catan.cpp catan.hpp
	clang++ $(FLAGS) -c catan.cpp

tests.o: tests.cpp doctest.h
	clang++ $(FLAGS) -c tests.cpp

main.o: main.cpp
	clang++ $(FLAGS) -c main.cpp

clean:
	rm *.o tests main
