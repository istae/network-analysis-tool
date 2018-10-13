INC = -I C:/libraries/include
all:
	gcc -g -O3 -std=c11 -DDEBUG=0 main.c $(INC) -o demo
debug:
	gcc -Wall -g -O3 -std=c11 -DDEBUG=1 main.c $(INC) -o demo
profile:
	gcc -Wall -pg -O3 -std=c11 -DDEBUG=0 main.c $(INC) -o prof
clean-profile:
	rm analysis.txt
	rm prof.exe
	rm gmon.out
#prof -f old.txt < commands.txt && gprof prof.exe > analysis.txt
