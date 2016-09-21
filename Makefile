#note that any variable declared in this Makefile can be overwritten in the call to make
#For example we could use the command: make CXX=gcc to use a different compiler.

#do make GL_LOAD=gl_core_4_<x> to use a different version of opengl
GL_LOAD = gl_core_4_1
GL_LOAD_SRC = $(SRCDIR)/$(GL_LOAD).c
GL_LOAD_OBJ = $(OBJDIR)/$(GL_LOAD).o

INCDIR = ./inc
SRCDIR = ./src
OBJDIR = ./obj
LIBDIR = ./lib

#system include directory.
#note this is for Mac OS X El Capitan. For Unix distros try /usr/local/include
#on windows you may have to add specific items for glfw, glm, soil2, and assimp
#or "install" these headers to a common location.
SYSTEMINC = /opt/local/include

INCLUDE = -I$(INCDIR) -I$(SYSTEMINC)

#grab a list the c++ source files (the gl_core_4_x.c is compiled separately and added to objects list)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

#give the library a name
LIBNAME = balrog

#complier, flags, and options
CXX = g++
CXXFLAGS = -Wall -std=c++11
OPTIONS = -O2

#patsubst - pattern substitution $(patsubst pattern_to_replace, replace_with_this, text)
# i.e. results in OBJ = (./src/*.cpp) -> ./obj/*.o
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
OBJ += $(GL_LOAD_OBJ)

#addprefix and add addsuffix are built in functions and do exactly waht they say on the tin
#in unix systems library filenames have to be prefixed with lib and end in a .a extension
LIB = $(addprefix $(LIBDIR)/lib, $(addsuffix .a, $(LIBNAME)))



all: $(GL_LOAD_OBJ) $(LIB)

$(GL_LOAD_OBJ): $(GL_LOAD_SRC)
		$(CXX) $(CXXFLAGS) $(OPTIONS) $(INCLUDE) -c $< -o $@

#this target actually makes the library using the archive command 'ar'
#it requires that the object files exist and are up-to-date, if not go to that target.
#the options rcs mean (r)eplace object files with the same name, (c)reate the archive.
#the s flag is equivalent to running ranlib on the archive.
#$@ refers to the target. In this context $^ means all object files found in the object directory.
$(LIBDIR)/lib$(LIBNAME).a: $(OBJ)
	ar rcs $@ $^

#here the pipe command '|' means check on the existance of the library directory and
#do this before you try to make the library.
#if it doesn't exist go to the target to make the directory else ignore.
#note the -p option in mkdir command is thus belt and braces
$(LIB): | $(LIBDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)

#this target complies the source files into object files.
#The % symbol is a wildcard meaning do this for each source file found.
#The $< means the first prerequisite i.e. the source file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTIONS) $(INCLUDE) -c $< -o $@

#note this is the same syntax as for the code that checks for and makes the library directory
#but in this case is for the object directory.
$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

#clean doesn't actually make anything, it just removes files, so must be "declared" phony.
#we do this as if there is a file called "clean" within this directory then make may get confused.
#the @ symbol suppressed the normal 'echo' response of the command it prefixes.
.PHONY: clean
clean:
	@rm -f $(OBJ) $(LIB) $(SRCDIR)/*~ $(INCDIR)/*~ ./*~
