@echo off

set script_dir=%~dp0

set programmer=STM32_Programmer_CLI.exe

where /q %programmer%
if %errorlevel% neq 0 (
	echo Cannot find STM32_Programmer_CLI.exe command
	exit /b 1
)

if "%2" neq "" (
	echo Usage: flash.bat [probe_index]
	exit /b 2
)

for /f %%l in ('%programmer% --list ^| find "ST-Link Probe"') do (
	set /a num_probes+=1
)

set probe=0
if "%1" neq "" (
	set probe=%1
) else if %num_probes% gtr 1 (
	echo More than one ST-Link probe detected, please specify the index of the probe to use.
	echo The available probes can be listed with: %programmer% --list.
	echo Usage: flash.bat ^<probe_index^>
	exit /b 3
)

%programmer% -c port=SWD index=%probe% -w "%script_dir%\build\nucleo_ispu_test_header.bin" 0x8000000 -s 0x8000000

