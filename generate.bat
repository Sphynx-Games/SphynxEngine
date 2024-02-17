@echo off

pushd %~dp0
call cmake .
popd

pause