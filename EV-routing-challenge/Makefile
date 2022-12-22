all: solution test

solution: main.cpp network.cpp route.cpp
	g++ -lstdc++ -std=c++17 -O1 -O1 $^ -o $@

test: test.cpp network.cpp
	g++ -lstdc++ -std=c++17 -O1 $^ -o $@

clean:
	rm -f solution
	rm -f test
