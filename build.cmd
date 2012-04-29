@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
devenv markdown-json.sln /Build Release
xcopy /Y /D Release\*.exe .\
