CC=g++
EXTENSION := cpp
SOURCE_FILES := $(wildcard src/*.${EXTENSION})
OBJ_FILES := $(addprefix obj/,$(notdir $(SOURCE_FILES:.${EXTENSION}=.o)))
LD_FLAGS := -lSDL2 -lSDL2_image
CC_FLAGS := -Wall -MMD -g -std=c++11
TARGET=Console.out

all: directories ${TARGET}

$(TARGET): $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.${EXTENSION}
	$(CC) $(CC_FLAGS) -c -o $@ $<

.PHONY: directories clean
directories:
	mkdir -p obj 

clean:
	rm -f $(TARGET) $(OBJ_FILES) $(OBJ_FILES:.o=.d)

# Automatic dependency graph generation with -MMD
-include $(OBJ_FILES:.o=.d)
