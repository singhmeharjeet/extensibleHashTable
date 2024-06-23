#compile header first

all: compile run

compile: 
	g++ -std=c++17 -Wno-sign-compare -Wall -g ./src/*.cpp  -o ./build/main

run:
	./build/main

clean:
	rm -rf ./build/*
	@echo "Clean done."
