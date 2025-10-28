# Makefile for Matrix Calculator

# ����������
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Ŀ¼����
SRCDIR = src
BUILDDIR = build
INCDIR = include

# Ŀ���ļ�
TARGET = $(BUILDDIR)/MatrixCalculator.exe

# Դ�ļ�
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# Ĭ��Ŀ��
all: $(TARGET)

# ���� build Ŀ¼
$(BUILDDIR):
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)

# ����Ŀ���ļ�
$(TARGET): $(BUILDDIR) $(OBJECTS)
	@echo ��������...
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo ����ɹ�! ��ִ���ļ�: $(TARGET)

# ����Դ�ļ�
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo ����: $<
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ��������ļ�
clean:
	@if exist $(BUILDDIR)\*.o del /Q $(BUILDDIR)\*.o
	@if exist $(TARGET) del /Q $(TARGET)
	@echo �������

# ���г���
run: $(TARGET)
	@cd $(BUILDDIR) && MatrixCalculator.exe

.PHONY: all clean run
