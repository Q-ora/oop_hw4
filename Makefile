all: test

test: main.cpp Point.cpp Line.cpp ConvexHull.cpp
	g++ main.cpp Point.cpp Line.cpp ConvexHull.cpp -o test

clean: test
	rm test