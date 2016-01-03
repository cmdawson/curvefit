# SOURCES := $(wildcard *.cpp)
SOURCES := $(swap.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
INCLUDE := -I../levmar-2.6 -I.
LDFLAGS := -lblas -llapack -lboost_date_time

#main: main.o $(OBJECTS) liblevmar.a
#	g++ $(LDFLAGS) -o $@ $^

test: test.o bond.o curvefitter.o liblevmar.a
	g++ $(LDFLAGS) -o $@ $^

%.o: %.cpp
	g++ $(INCLUDE) -c -o $@ $<

clean:
	rm *.o
