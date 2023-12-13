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

clean:
	rm -rf ./build/*

fclean: clean
	rm -rf ./lib/*
	rm -rf ./bin/*

re: fclean build

.PHONY: all build test clean fclean re
