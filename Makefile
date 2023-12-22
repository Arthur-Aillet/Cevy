##
## Project : Cevy
## File : Makefile
##

SRC_DIR	=	src

all:	build

build:
	cmake -S . -B ./build
	make -j --no-print-directory -C build

test:
	cmake -DTESTS=on -S . -B ./build
	make -j --no-print-directory -C build tests-run-cevy

doc:
	cd docs/doxygen && doxygen

format:
	@for src in $(shell find $(SRC_DIR) -name "*.cpp" -o -name "*.hpp") ; do \
		echo "Formatting [$$src]..." ;  			\
		clang-format -i "$$src" -style=file ; 		\
	done
	@echo "Done"

clean:
	rm -rf ./build/*

fclean: clean
	rm -rf ./lib/*
	rm -rf ./bin/*

re: fclean build

.PHONY: all build test clean fclean re
