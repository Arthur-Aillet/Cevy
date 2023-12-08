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
	make --no-print-directory -C build tests-run-cevy

doc:
	cd docs/doxygen && doxygen

.PHONY: all build test
