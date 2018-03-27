#!/usr/bin/fish
g++ src/main.c -I src/ -I include/ -o bin/assignment1 -lGL -lGLU -lm -lglut
if test $status -eq 0
    bin/assignment1
else
    echo Compilation failed
end