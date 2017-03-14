CMAKELISTS=	CMakeLists.txt \
		clib/CMakeLists.txt

all: build/Makefile
	(cd build && make)

build/Makefile: ${CMAKELISTS}
	mkdir -p build
	(cd build && cmake ..)
