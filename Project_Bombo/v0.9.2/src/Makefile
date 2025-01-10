CXX = g++

TARGET = test_enviroment/Bombo

SRCS = Main.cpp Bombo.cpp Util.cpp Generator.cpp

OBJS = Main.o Bombo.o Util.o Generator.o

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)
	
%.o: %.cpp
	$(CXX) -c $<
	
clean:
	rm -f $(TARGET) *.o

