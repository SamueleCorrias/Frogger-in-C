frogger: main.o grafica.o auto.o fiume.o rana.o
	gcc main.o grafica.o auto.o fiume.o rana.o -o frogger -lncurses

main.o: main.c header.h
	gcc -c main.c 

grafica.o: grafica.c header.h
	gcc -c grafica.c 

auto.o: auto.c header.h 
	gcc -c auto.c 

fiume.o: fiume.c header.h 
	gcc -c fiume.c 

rana.o: rana.c header.h 
	gcc -c rana.c 

clean: 
	rm *.o frogger
