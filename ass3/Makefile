EXE = a3searchSPIMI
OBJS = porter2_stemmer.o
CC = g++ -std=c++11 -Wall -pedantic -I. -g
CCOPTS = -O3
LINKER = g++ -std=c++11 -Wall -pedantic -I. -g

all: $(EXE)

$(EXE): $(OBJS) a3searchSPIMI.cpp
	$(LINKER) a3searchSPIMI.cpp -o $@ $(OBJS)

porter2_stemmer.o: porter2_stemmer.h porter2_stemmer.cpp
	$(CC) $(CCOPTS) -c porter2_stemmer.cpp -o $@

clean:
	rm -f *.o $(EXE)
