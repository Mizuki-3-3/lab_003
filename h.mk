CXX = g++
CXXFLAGS = -Iinclude -Itemplates -Wall -Wextra -std=c++20 -I"C:/FTXUI/include" -Wno-unused-parameter
LDFLAGS = -L"C:/FTXUI/lib"
LDLIBS = -lftxui-component -lftxui-dom -lftxui-screen

TARGET = main
SRC = main.cpp interface.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	del $(TARGET).exe

run: main.exe
	cmd.exe /c start cmd.exe /k main.exe

rebuild: clean all run

.PHONY: all clean rebuild