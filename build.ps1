# Matrix Calculator ����ű�
# ʹ�÷���: .\build.ps1

Write-Host "��ʼ���� Matrix Calculator..." -ForegroundColor Green

# ���� build Ŀ¼
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# ���ñ������ͱ���ѡ��
$CXX = "g++"
$CXXFLAGS = "-std=c++17 -Wall -Wextra -Iinclude"
$SRCDIR = "src"
$BUILDDIR = "build"
$TARGET = "MatrixCalculator.exe"

# ��ȡ����Դ�ļ�
$sources = Get-ChildItem -Path $SRCDIR -Filter "*.cpp" | ForEach-Object { $_.FullName }

# ��������Դ�ļ���Ŀ���ļ�
Write-Host "���ڱ���Դ�ļ�..." -ForegroundColor Cyan
$objects = @()

foreach ($src in $sources) {
    $basename = [System.IO.Path]::GetFileNameWithoutExtension($src)
    $obj = Join-Path $BUILDDIR "$basename.o"
    $objects += $obj
    
    Write-Host "  ����: $src" -ForegroundColor Yellow
    & $CXX $CXXFLAGS.Split() -c $src -o $obj
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "����ʧ��: $src" -ForegroundColor Red
        exit 1
    }
}

# ��������Ŀ���ļ�
Write-Host "��������..." -ForegroundColor Cyan
$targetPath = Join-Path $BUILDDIR $TARGET
& $CXX $objects -o $targetPath

if ($LASTEXITCODE -eq 0) {
    Write-Host "����ɹ�! ��ִ���ļ�: $targetPath" -ForegroundColor Green
    Write-Host "���г���: cd build; .\$TARGET" -ForegroundColor Magenta
} else {
    Write-Host "����ʧ��!" -ForegroundColor Red
    exit 1
}
