SOURCE_FILES := $(wildcard *.cpp)
TARGETS := $(SOURCE_FILES:.cpp=.out)

examples: ${TARGETS}

%.out: %.cpp
	g++ -Wall -std=c++11 -lSDL2 -lSDL2_image -lhalfling -o $@ $^

clean:
	rm -f ${TARGETS}
