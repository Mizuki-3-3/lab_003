CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Itemplates

TARGET = test

SRCS = tests/assertions.cpp \
       tests/test_concat.cpp \
       tests/test_find.cpp \
       tests/test_find_subsequence.cpp \
       tests/test_get.cpp \
       tests/test_inssert.cpp \
       tests/test_map.cpp \
       tests/test_push_back.cpp \
       tests/test_push_front.cpp \
       tests/test_reduce.cpp \
       tests/test_sort.cpp \
       tests/test_subdeque.cpp \
       tests/test_where.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) tests/test_main.cpp -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	cmd.exe /c start cmd.exe /k $(TARGET).exe

clean:
	del tests\*.o test.exe
rerun: clean all run

.PHONY: all clean run rerun