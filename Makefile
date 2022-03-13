name = demo
#-finstrument-functions is for traceback along with traceback.cpp
#traceback.cpp needs to be compiled like: g++ -c -fPIC traceback.cpp
compile_flags = -Wall -Wextra -Werror -std=c++17 #-finstrument-functions
GLEW = /goinfre/edavid/.brew/Cellar/glew/2.2.0_1
GLFW = /goinfre/edavid/.brew/Cellar/glfw/3.3.6
link_flags = -L$(GLEW)/lib -lGLEW -L$(GLFW)/lib -lglfw -framework OpenGL
compiler = g++
pwd = $(shell pwd)
# source_files = $(foreach file,$(shell find sources -type f -not -name "*.h"),$(pwd)/$(file))
header_directory = $(pwd)/headers
source_directory = sources
object_directory = objects
source_files = $(foreach file,$(shell find sources -type f -not -name "*.h"),$(pwd)/$(file))
object_files = $(subst $(source_directory),$(object_directory),$(source_files:.cpp=.o))
header_files = $(subst $(source_directory),$(header_directory),$(source_files:.cpp=.hpp))

#first:
#	echo $(source_files)
#	echo $(object_files)
#	echo $(header_files)

$(name): $(subst $(source_directory),$(object_directory),$(source_files:.cpp=.o))
	$(compiler) $(link_flags) -o $@ $^

# main.cpp
#$(pwd)/$(object_directory)/main.o: $(source_directory)/main.cpp $(header_files)
#	cd $(object_directory) && $(compiler) $(compile_flags) -I$(header_directory) -I$(GLEW)/include -I$(GLFW)/include -c ../$<

.SECONDEXPANSION:
%.o: $$(subst $(object_directory),$(source_directory), $$(subst .o,.cpp,$$@))
	cd $(dir $@) && $(compiler) $(compile_flags) -I$(header_directory) -I$(GLEW)/include -I$(GLFW)/include -c $^

# for .cpp files that has their own .hpp
#$(pwd)/%.o: # $(header_directory)/%.hpp $(header_directory)/%.h
#	cd $(dir $@) && $(compiler) $(compile_flags) -I$(header_directory) -I$(GLEW)/include -I$(GLFW)/include -c $^ $(subst $(object_directory),$(source_directory),$(patsubst %.o,%.cpp,$@))

.PHONY: all clean re fclean bonus
all:
	make $(name)
clean:
	find $(object_directory) -name "*.o" -delete
	rm -f imgui.ini
fclean: clean
	rm -f $(name)
re: fclean
	make $(name)
