LLVM_CONFIG = llvm-config
OBJECTS = main.o context.o context_transition_table.o forward_inter_procedural_analysis.o sign_analysis.o inter_procedural_analysis.o
CXX = clang++

CXXFLAGS = -c -Iinclude/ `$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS = `$(LLVM_CONFIG) --ldflags` -Wl,-znodelete

all: main.so

main.so: $(OBJECTS)
	$(CXX) -shared $(OBJECTS) -o main.so $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.so
