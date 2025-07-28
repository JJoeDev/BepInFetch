@echo off

cls

cmake -G Ninja .
ninja

.\bin\BepFetch.exe
