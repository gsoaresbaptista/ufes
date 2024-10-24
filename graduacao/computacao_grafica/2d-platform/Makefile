PROJ_NAME := trabalhocg
SOURCE := $(shell find source/ -type f -name '*.cpp')
HEADER := $(shell find source/ -type f -name '*.hpp')
OBJECT := $(addprefix objects/,$(subst .cpp,.o,$(subst source/,,$(SOURCE))))

CXX := g++
STD_FLAGS := -W -Wall -std=c++11 -o3
GLUT_FLAGS := -lGL -lGLU -lglut

all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJECT)
	$(CXX) $^ $(STD_FLAGS) $(GLUT_FLAGS) -o $@

objects/%.o: source/%.cpp
	$(CXX) $< -c -o $@

objects/%.o: source/models/%.cpp source/models/%.hpp
	$(CXX) $< -c -o $@

objects/%.o: source/views/%.cpp source/views/%.hpp
	$(CXX) $< -c -o $@

objects/%.o: source/controllers/%.cpp source/controllers/%.hpp
	$(CXX) $< -c -o $@

objects/%.o: source/utils/libs/%.cpp source/utils/libs/%.hpp
	$(CXX) $< -c -o $@

objects/%.o: source/utils/shapes/%.cpp source/utils/shapes/%.hpp
	$(CXX) $< -c -o $@

objects/%.o: source/utils/%.cpp source/utils/%.hpp
	$(CXX) $< -c -o $@

objFolder:
	@ mkdir -p objects
	@ mkdir -p objects/models
	@ mkdir -p objects/views
	@ mkdir -p objects/controllers
	@ mkdir -p objects/utils
	@ mkdir -p objects/utils/libs
	@ mkdir -p objects/utils/shapes

clean:
	@ rm -rf ./objects/*.o $(PROJ_NAME)
	@ rm -rf ./objects/models
	@ rm -rf ./objects/views
	@ rm -rf ./objects/controllers
	@ rm -rf ./objects/utils/libs
	@ rm -rf ./objects/utils/shapes
	@ rm -rf ./objects/utils
	@ rmdir objects

run: all
	./$(PROJ_NAME)
