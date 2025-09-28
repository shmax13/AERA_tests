@echo off

REM Adjust these paths to your install locations:
set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set OPENCPPCOVERAGE_PATH="C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe"

%MSBUILD_PATH% "C:\Users\shmax\source\repos\AERA_tests\AERA_tests.sln" /p:Configuration=Debug /m /t:Rebuild

%OPENCPPCOVERAGE_PATH% ^
  --sources "C:\Users\shmax\source\repos\AERA_tests" ^
  --export_type html:"C:\Users\shmax\source\repos\AERA_tests\shmax_tests\coverage_report" ^
  -- "C:\Users\shmax\source\repos\AERA_tests\Debug\shmax_tests.exe"

start "" "C:\Users\shmax\source\repos\AERA_tests\shmax_tests\coverage_report\index.html"

exit /b %errorlevel%
