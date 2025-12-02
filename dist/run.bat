@echo off
cd /d "%~dp0"
start http://localhost:3000
serve -s . -l 3000
pause
