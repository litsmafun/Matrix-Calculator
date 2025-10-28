@echo off
REM Matrix Calculator 编译脚本 (批处理版本)

echo 开始编译 Matrix Calculator...

REM 创建 build 目录
if not exist build mkdir build

REM 编译所有源文件
echo 正在编译源文件...

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/Matrix.cpp -o build/Matrix.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/DenseMatrix.cpp -o build/DenseMatrix.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/SparseMatrix.cpp -o build/SparseMatrix.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/BasicMatrixOperations.cpp -o build/BasicMatrixOperations.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/BasicMatrixFactory.cpp -o build/BasicMatrixFactory.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/ConsoleUI.cpp -o build/ConsoleUI.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/App.cpp -o build/App.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/Localization.cpp -o build/Localization.o
if errorlevel 1 goto error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/main.cpp -o build/main.o
if errorlevel 1 goto error

REM 链接所有目标文件
echo 正在链接（静态链接）...
g++ build/Matrix.o build/DenseMatrix.o build/SparseMatrix.o build/BasicMatrixOperations.o build/BasicMatrixFactory.o build/ConsoleUI.o build/App.o build/Localization.o build/main.o -static -static-libgcc -static-libstdc++ -o build/MatrixCalculator.exe
if errorlevel 1 goto error

echo.
echo 编译成功! 可执行文件: build\MatrixCalculator.exe
echo 运行程序: cd build ^&^& MatrixCalculator.exe
goto end

:error
echo.
echo 编译失败!
exit /b 1

:end
