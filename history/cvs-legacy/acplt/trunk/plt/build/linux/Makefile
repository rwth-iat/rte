CXX_EXTRA_FLAGS = -I../../include -DPLT_HAS_BOOL=1
CXX = g++
CXXFLAGS = -g
all: thandle

thandle.o:	../../tests/handle.cpp ../../include/plt/handle.hpp
	$(CXX) $(CXX_EXTRA_FLAGS) $(CXXFLAGS) -c -o thandle.o $<

thandle:	thandle.o
	$(CXX) -o thandle $<

clean:
	rm -f thandle.o thandle