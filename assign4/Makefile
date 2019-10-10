LLVM_CONFIG=llvm-config

CXX=clang++
CXXFLAGS=`$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS=`$(LLVM_CONFIG) --ldflags` -Wl,-znodelete

all: lva.so

lva.so: lva.o
	$(CXX) -shared lva.o -o lva.so $(LDFLAGS)

lva.o: lva.cpp
	$(CXX) -c lva.cpp -o lva.o $(CXXFLAGS)

clean:
	rm -f *.o *.so
