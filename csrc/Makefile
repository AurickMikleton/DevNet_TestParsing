CLANG = clang

CFLAGS = -g -Wall

SOURCE = banlist.c hashtable.c main.c sort.c

all:
	$(CLANG) $(CFLAGS) $(SOURCE) -o stat.exe
	$(CLANG) $(CFLAGS) parse.c -o parse.exe

clean:
	del *.o *.exe *.ilk *.pdb
