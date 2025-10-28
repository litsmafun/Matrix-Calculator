# Matrix Calculator 编译脚本
# 使用方法: .\build.ps1

Write-Host "开始编译 Matrix Calculator..." -ForegroundColor Green

# 创建 build 目录
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# 设置编译器和编译选项
$CXX = "g++"
$CXXFLAGS = "-std=c++17 -Wall -Wextra -Iinclude"
$SRCDIR = "src"
$BUILDDIR = "build"
$TARGET = "MatrixCalculator.exe"

# 获取所有源文件
$sources = Get-ChildItem -Path $SRCDIR -Filter "*.cpp" | ForEach-Object { $_.FullName }

# 编译所有源文件到目标文件
Write-Host "正在编译源文件..." -ForegroundColor Cyan
$objects = @()

foreach ($src in $sources) {
    $basename = [System.IO.Path]::GetFileNameWithoutExtension($src)
    $obj = Join-Path $BUILDDIR "$basename.o"
    $objects += $obj
    
    Write-Host "  编译: $src" -ForegroundColor Yellow
    & $CXX $CXXFLAGS.Split() -c $src -o $obj
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "编译失败: $src" -ForegroundColor Red
        exit 1
    }
}

# 链接所有目标文件
Write-Host "正在链接..." -ForegroundColor Cyan
$targetPath = Join-Path $BUILDDIR $TARGET
& $CXX $objects -o $targetPath

if ($LASTEXITCODE -eq 0) {
    Write-Host "编译成功! 可执行文件: $targetPath" -ForegroundColor Green
    Write-Host "运行程序: cd build; .\$TARGET" -ForegroundColor Magenta
} else {
    Write-Host "链接失败!" -ForegroundColor Red
    exit 1
}
