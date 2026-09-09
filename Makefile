CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Isrc
LDFLAGS = -lopengl32 -lglu32 -lgdi32 -luser32

SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/DHMT_OpenGL.exe

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/Window.cpp $(SRC_DIR)/Camera.cpp $(SRC_DIR)/Shapes.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir $(BIN_DIR)

clean:
	del /Q $(SRC_DIR)\*.o $(BIN_DIR)\*.exe 2>nul || exit 0

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
