@echo off

pushd bin
cl /diagnostics:caret /Zi ..\parse.c
popd

