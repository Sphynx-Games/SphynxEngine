@echo off

pushd %~dp0
call cmake -S . -B build
popd

pause