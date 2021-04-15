PROJECT = libtween
CXX = arm-none-eabi-g++
AR = arm-none-eabi-ar
CXXFLAGS = -g -Wall -pedantic -std=c++11 -fno-rtti -fno-exceptions

INCLUDES = -Iinclude/
SOURCES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/arm/%.o)

TEST_CXX = g++
TEST_AR = ar
TEST_CXXFLAGS = -g -Wall -pedantic -std=c++11 -fno-rtti -fno-exceptions -DTESTING
TEST_OBJECTS = $(SOURCES:src/%.cpp=build/test/%.o)

.PHONY: all dir clean test

all: dir $(PROJECT).a

test: dir $(PROJECT)-test.a

dir:
	@mkdir -p build/arm/equations
	@mkdir -p build/arm/paths
	@mkdir -p build/test/equations
	@mkdir -p build/test/paths
	@mkdir -p lib

$(PROJECT).a: $(OBJECTS)
	$(AR) rvs lib/$@ $^
$(PROJECT)-test.a: $(TEST_OBJECTS)
	$(TEST_AR) rvs lib/$@ $^

clean:
	@rm -rf build
	@rm -rf lib
	@echo "Successfully cleaned."

build/arm/%.o: src/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
	$(CXX) -MM $< > build/arm/$*.d

build/test/%.o: src/%.cpp
	$(TEST_CXX) $(INCLUDES) $(TEST_CXXFLAGS) -c $< -o $@
	$(TEST_CXX) -MM $< > build/test/$*.d
