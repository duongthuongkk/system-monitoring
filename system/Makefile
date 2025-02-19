
all: source/cpu_info.o main.o
	gcc -o cpu source/cpu_info.o main.o

main: main.c include/cpu_info.h
	gcc -o main.o main.c -I ./include
cpu_info: source/cpu_info.c include/cpu_info.h
	gcc -o cpu_info.o ./source/cpu_info.c -I ./include

clean:
	rm -rf cpu
