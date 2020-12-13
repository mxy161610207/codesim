XX = g++
CFLAGS = -g 

INCLUDE = $(wildcard ./inlcude/*.h) 
SOURCES = $(wildcard ./src/*.cpp)
INCLUDE_DIRS = -I./include

TARGET = codesim
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET) : $(OBJECTS)
	$(XX) $(CFLAGS) $^ -o $@ $(CLIBS)
$(OBJECTS) : %.o : %.cpp
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

clean:
	rm -rf $(TARGET) $(OBJECTS)  
