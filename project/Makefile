LLVM_CONFIG=llvm-config
OBJECTS = main.o context.o context_transition_table.o
CXX = clang++

CXXFLAGS = -c -Iinclude/ `$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS = `$(LLVM_CONFIG) --ldflags` -Wl,-znodelete


all: main virtual

virtual: inter_procedural_analysis.o forward_inter_procedural_analysis.o

main: $(OBJECTS)
	$(CXX) -o main $(OBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o main
