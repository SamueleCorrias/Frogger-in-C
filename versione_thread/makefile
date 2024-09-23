frogger: main.o grafica.o rana.o fiume.o macchine.o global.o
	gcc main.o grafica.o rana.o fiume.o macchine.o global.o -w -o frogger -lpthread -lncurses 

main.o: main.c header.h global.h
	gcc -w -c main.c 

grafica.o: grafica.c header.h global.h
	gcc -c grafica.c 

rana.o: rana.c header.h global.h
	gcc -c rana.c 

fiume.o: fiume.c global.c header.h
	gcc -c fiume.c

macchine.o: macchine.c global.c header.h
	gcc -c macchine.c

global.o: global.c header.h
	gcc -c global.c

clean: 
	rm *.o frogger