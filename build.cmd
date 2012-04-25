@echo off
call "%VS100COMNTOOLS%vsvars32.bat"
devenv markdown.sln /Build Release
xcopy /Y /D Release\*.exe .\
