@echo off
REM Matrix Calculator ����ű� (������汾)

echo ��ʼ���� Matrix Calculator...

REM ���� build Ŀ¼
if not exist build mkdir build

REM ��������Դ�ļ�
echo ���ڱ���Դ�ļ�...

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

REM ��������Ŀ���ļ�
echo �������ӣ���̬���ӣ�...
g++ build/Matrix.o build/DenseMatrix.o build/SparseMatrix.o build/BasicMatrixOperations.o build/BasicMatrixFactory.o build/ConsoleUI.o build/App.o build/Localization.o build/main.o -static -static-libgcc -static-libstdc++ -o build/MatrixCalculator.exe
if errorlevel 1 goto error

echo.
echo ����ɹ�! ��ִ���ļ�: build\MatrixCalculator.exe
echo ���г���: cd build ^&^& MatrixCalculator.exe
goto end

:error
echo.
echo ����ʧ��!
exit /b 1

:end
