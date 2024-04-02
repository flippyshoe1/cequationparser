DEBUG_FLAGS = -Wall -Wextra -pedantic
TARGET = cequationparse
COMPILER_FLAGS = $(DEBUG_FLAGS)
LINKER_FLAGS =

all: $(TARGET)

$(TARGET): object
	gcc -o $@ ./*.o $(LINKER_FLAGS)

object:
	gcc $(COMPILER_FLAGS) -I include/ -c src/*.c

clean:
	rm ./*.o $(TARGET)
