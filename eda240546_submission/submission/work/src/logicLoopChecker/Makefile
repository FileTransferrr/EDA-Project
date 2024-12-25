CXX = g++

CXXFLAGS = -Wall -g -std=c++11 -pthread

TARGET = logicLoopChecker

SRCS = main.cpp \
	   loopFinder.cpp \
	   oscillationSCC.cpp \
	   parser.cpp \
	   printResult.cpp \
	   sccInfo.cpp \
	   tarjan.cpp 

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	rm -f $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) result_*.txt

.PHONY: all clean
