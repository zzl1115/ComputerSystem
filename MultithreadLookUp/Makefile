all: program1

program1: program1.c
	gcc -o program1 -pthread program1.c

run: program1
	./program1 test_data2.txt 2 4 1



run_valgrind: program1
	valgrind -v --tool=memcheck --leak-check=yes --num-callers=20 --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./program1 test_data2.txt 2 4 1

clean:
	rm program1