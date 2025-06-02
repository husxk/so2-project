all: build/server build/client

build/server: | build
	g++ -o build/server tcp_server.cpp

build/client: | build
	g++ -o build/client tcp_client.cpp

build:
	mkdir -p build

clean:
	rm -rf build/server build/client

.PHONY: all clean build

