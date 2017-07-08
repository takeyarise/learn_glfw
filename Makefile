TARGET_EXEC ?= a.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

#SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

ETC_INC := $()

#INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d -name '.git' -prune -o -type d  -print)
#INC_DIRS := $(shell find ./include -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LDFLAGS := $(shell pkg-config --libs gl glew glfw3)

CXXFLAGS := -std=c++14 -Wall -Wextra -g -DDEBUG #-DGLFW_INCLUDE_NONE

CPPFLAGS ?= $(INC_FLAGS) $(shell pkg-config --cflags gl glew glfw3) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(ETC_INC)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ $(ETC_INC)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)/*

-include $(DEPS)
