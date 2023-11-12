##
## Project : Cevy
## File : Makefile
##

all:	build

build:
	cmake -S . -B ./build
	make --no-print-directory -C build

test:
	cmake -DTESTS=on -S . -B ./build
	make --no-print-directory -C build tests-run

.PHONY: all build test
