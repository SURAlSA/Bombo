CXX = g++

TARGET = new.exe

SRCS = src/main.cpp src/util.cpp src/newfile.cpp src/newfile.cpp src/newfile.cpp

OBJS = obj/main.o obj/util.o obj/newfile.o obj/newfile.o obj/newfile.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

obj/%.o: src/%.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(TARGET) obj/*.o
