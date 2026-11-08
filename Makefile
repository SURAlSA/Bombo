CXX = g++

TARGET = test_environment/Bombo.exe

SRCS = src/main.cpp src/util.cpp src/ProjectManager.cpp src/UserManager.cpp src/BackupManager.cpp src/Generator.cpp src/Bombo.cpp src/CommandDispatcher.cpp src/FileManager.cpp src/KeyManager.cpp src/Prompter.cpp src/CompileManager.cpp src/Variables.cpp

OBJS = obj/main.o obj/util.o obj/ProjectManager.o obj/UserManager.o obj/BackupManager.o obj/Generator.o obj/Bombo.o obj/CommandDispatcher.o obj/FileManager.o obj/KeyManager.o obj/Prompter.o obj/CompileManager.o obj/Variables.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

obj/%.o: src/%.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(TARGET) obj/*.o
