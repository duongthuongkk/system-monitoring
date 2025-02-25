
all: source/device_info.o main.o
	gcc -o device_info source/device_info.o main.o

main: main.c include/device_info.h
	gcc -o main.o main.c -I ./include
cpu_info: source/device_info.c include/device_info.h
	gcc -o device_info.o ./source/device_info.c -I ./include

clean:
	rm -rf cpu main.o source/device_info.o
