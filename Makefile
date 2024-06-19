target:= wLog

SRC_PATH += .
SRC_PATH += src
SOURCES := $(foreach dir, $(SRC_PATH), $(wildcard $(dir)/*))
CPP_SOURCES := $(filter %.cpp, $(SOURCES))

$(target):clean
	g++ $(CPP_SOURCES) -I./include -o $@
clean:
	rm -rf $(target)