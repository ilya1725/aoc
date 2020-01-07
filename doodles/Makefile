all:	doodles ex2 ex3 memgoble file-search path
.PHONY : all

doodles:
	g++ -lstdc++ -pthread -std=c++17 -o doodles src/doodles.cpp src/MapCheck.cpp

ex2:
	g++ -O3 -Wall -Wconversion -lstdc++ -pthread -std=c++17 -o ex2 src/ex2.cpp

ex3:
	g++ -O0 -Wall -Wconversion -lstdc++ -pthread -std=c++17 -o ex3 src/ex3.cpp

memgoble:
	g++ -Wall -Wconversion -lstdc++ -pthread -std=c++17 -o memgoble src/memgoble.cpp

file-search:
	g++ -Wall -Wconversion -lstdc++ -pthread -std=c++17 -o file-search src/file-search.cpp

path:
	g++ -Wall -Wconversion -lstdc++ -pthread -std=c++17 -o obstacle-path src/obstacle-path.cpp

clean:
	rm -f memgoble
	rm -f ex2
	rm -f ex3
	rm -f doodles
	rm -f file-search
	rm -f obstacle-path
