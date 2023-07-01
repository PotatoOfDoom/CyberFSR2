+@echo off
setlocal
set "origin=%cd%\"
set "buildPath=..\external\FidelityFX-FSR2\build"
set "slnDX12=FSR2_Sample_DX12.sln"
set "slnVK=FSR2_Sample_VK.sln"

echo Script started on %date% %time%
echo Current working directory: %origin%

REM Clear old artifacts
echo Clearing old artifacts...
cd /d "%origin%%buildPath%"
if exist "DX12" (
    echo Removing old DX12 artifacts...
    rd /s /q "DX12" > nul 2>&1
    if errorlevel 1 (
        echo Failed to remove old DX12 artifacts.
    ) else (
        echo Successfully removed old DX12 artifacts.
    )
)
if exist "VK" (
    echo Removing old VK artifacts...
    rd /s /q "VK" > nul 2>&1
    if errorlevel 1 (
        echo Failed to remove old VK artifacts.
    ) else (
        echo Successfully removed old VK artifacts.
    )
)

REM Regenerate artifacts
echo Regenerating artifacts...
cd /d "%origin%%buildPath%"
call GenerateSolutions.bat > nul 2>&1
if errorlevel 1 (
    echo Failed to regenerate artifacts.
) else (
    echo Successfully regenerated artifacts.
)

REM Build FSR2_Sample_DX12.sln
call :BuildSolution "%origin%%buildPath%\DX12\%slnDX12%" "Debug" "DX12"
call :BuildSolution "%origin%%buildPath%\DX12\%slnDX12%" "RelWithDebInfo" "DX12"

REM Build FSR2_Sample_VK.sln
call :BuildSolution "%origin%%buildPath%\VK\%slnVK%" "Debug" "VK"
call :BuildSolution "%origin%%buildPath%\VK\%slnVK%" "RelWithDebInfo" "VK"

cd /d "%origin%"
goto :endofBuildFSR2

REM Function to build a solution file
:BuildSolution
set "solutionPath=%~1"
set "configuration=%~2"
if exist "%solutionPath%" (
    echo Building %solutionPath%...
    cd /d "%origin%%buildPath%\%~3"
    MSBUILD.exe "%solutionPath%" /t:Build /p:Configuration=%configuration%
    if errorlevel 1 (
        echo Failed to build %solutionPath%.
    ) else (
        echo Successfully built %solutionPath%.
    )
) else (
    echo %solutionPath% not found. Skipping build.
)
exit /b

:endofBuildFSR2
echo FSR2 build-script finished on %date% %time%
endlocal
