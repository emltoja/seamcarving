build: main.c
	gcc -Wall -Wextra -ggdb -o main main.c bitarray.c -lm 

run: 
	make build && ./main ./testimages/jurlewicz.jpg ./outimages/jurlewiczmarked1.jpg 10