# Compiler settings
CXX = g++
LIBS = -lpthread 

# Names of executable files
EXECUTABLES = arrivals.out full_schedule.out airplane.out update.out

# Library file
LIBRARY = libUtility.so

all: $(EXECUTABLES)

# Rule to build an executable
%.out: %.cpp %.h $(LIBRARY) libUtility.h
	$(CXX) -g $< -o $@  -L. -lUtility $(LIBS)

# Rule to build the library
$(LIBRARY): libUtility.cpp libUtility.h
	$(CXX) -g $(LIBS) -fPIC -shared -o $@ $^

# Rule to remove all executables and the library
clean:
	rm -f $(EXECUTABLES) $(LIBRARY)



# Disable implicit rules
.SUFFIXES:
