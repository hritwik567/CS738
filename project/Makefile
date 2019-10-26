OBJECTS=main.o context.o context_transition_table.o

all: main
main: $(OBJECTS)
	g++ -o main $(OBJECTS)

.cpp.o:
	g++ -c $< -o $@