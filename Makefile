CPPFILES := $(shell find . -name "*.cpp")
OBJFILES := $(subst src,obj,$(CPPFILES:.cpp=.o))

all: bin/steve

bin/steve: $(OBJFILES)
	mkdir -p bin
	g++ -o bin/steve $(OBJFILES)

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ -c $< -o $@
