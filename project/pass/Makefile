LLVM_CONFIG=llvm-config

CXX=clang++
CXXFLAGS=`$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS=`$(LLVM_CONFIG) --ldflags` -Wl,-znodelete

OBJECTS = pass.o dataflow.o

all: pass.so

pass.so: $(OBJECTS)
	$(CXX) -dylib -shared $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.so *.ll *.bc
