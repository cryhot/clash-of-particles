_GUI = $(if $(NOGUI),,-D GUI)
CC = gcc
CFLAGS = -g -std=c99 -Wall -I ./include $(_GUI)
LDFLAGS = -lm -lSDL

.PHONY: clean doc

doc:
	doxygen conf/doxygen.conf

%.o: ./src/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

check-syntax: disc.o snow.o

read-file: read-file.o
	$(CC) $(CFLAGS) -o $@ $^

write-fact: write-fact.o
	$(CC) $(CFLAGS) -o $@ $^

snow: disc.o snow.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

add-files-svn:
	svn add --force src/*.c include/*.h data/*.txt --auto-props --parents --depth infinity -q

clean:
	- rm -f *.o *.csv fact.txt read-file write-fact snow
