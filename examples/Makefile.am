examples: logo.out hello.out

%.out: %.cpp
	g++ -Wall -std=c++11 -lSDL2 -lSDL2_image -lhalfling -o $@ $^

clean:
	rm -f logo.out
