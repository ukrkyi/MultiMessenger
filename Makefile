all:
	@test -d build/ || mkdir build
	cd build && qmake .. && make

clean:
	rm -rf build

install:
	cd build && make install