all: solution test

solution: main.cpp network.cpp route.cpp
	g++ -lstdc++ -std=c++23 -O2 $^ -o $@

test: test.cpp network.cpp
	g++ -lstdc++ -std=c++23 -O2 $^ -o $@

clean:
	rm -f solution
	rm -f test
