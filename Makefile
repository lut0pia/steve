ifndef config
  config=debug
endif

CXXFLAGS=-Wall -Wextra
ifeq ($(config),debug)
	CXXFLAGS+=-g3 -D_DEBUG
	OBJDIR=obj/dbg
	EXENAME=steve_dbg
else
	CXXFLAGS+=-O3
	OBJDIR=obj/rls
	EXENAME=steve
endif

CPPFILES := $(shell find . -name "*.cpp")
OBJFILES := $(subst src,$(OBJDIR),$(CPPFILES:.cpp=.o))

all: bin/steve

bin/steve: $(OBJFILES)
	rm -rf bin
	mkdir -p bin
	g++ $(CXXFLAGS) -o bin/$(EXENAME) $(OBJFILES)

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) -c $< -o $@
