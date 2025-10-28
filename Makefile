# Makefile for Matrix Calculator

# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# 目录设置
SRCDIR = src
BUILDDIR = build
INCDIR = include

# 目标文件
TARGET = $(BUILDDIR)/MatrixCalculator.exe

# 源文件
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# 默认目标
all: $(TARGET)

# 创建 build 目录
$(BUILDDIR):
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)

# 链接目标文件
$(TARGET): $(BUILDDIR) $(OBJECTS)
	@echo 正在链接...
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo 编译成功! 可执行文件: $(TARGET)

# 编译源文件
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo 编译: $<
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理编译文件
clean:
	@if exist $(BUILDDIR)\*.o del /Q $(BUILDDIR)\*.o
	@if exist $(TARGET) del /Q $(TARGET)
	@echo 清理完成

# 运行程序
run: $(TARGET)
	@cd $(BUILDDIR) && MatrixCalculator.exe

.PHONY: all clean run
