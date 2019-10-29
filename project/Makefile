OBJECTS = main.o context.o context_transition_table.o
CXX = g++
CFLAGS = -c -Iinclude/

all: main virtual

virtual: inter_procedural_analysis.o

main: $(OBJECTS)
	$(CXX) -o main $(OBJECTS)

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f *.o main
