
TSAN_FLAGS = "verbosity=3 detect_deadlocks=1 halt_on_error=true "
BUG_SOLVER = setarch $(uname -m) -R

all: build/ build/main
	./build/main 5

all2: build/
	TSAN_OPTIONS=${TSAN_FLAGS} ${BUG_SOLVER} ./build/main

build/:
	./scripts/build.sh

build/main:
	./scripts/build.sh

clean:
	rm -rf build/

.PHONY: all clean all2

