# 指定编译器
CXX = g++

# 编译器标志
#CXXFLAGS = -Wall -g

# 链接库
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# 项目目标文件
TARGET = TOUHOU_BUPT

# 所有源文件
SRC = main.cpp FO.cpp Game.cpp

# 头文件目录
INCLUDE = -I./include

# 对应的目标文件
OBJ = $(SRC:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接生成可执行文件，使用库链接选项 $(LDFLAGS)
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# 生成每个源文件的目标文件，包含头文件目录选项 $(INCLUDE)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# 清理目标和中间文件
clean:
	rm -f $(OBJ) $(TARGET)

# 伪目标，防止和文件同名
.PHONY: all clean
