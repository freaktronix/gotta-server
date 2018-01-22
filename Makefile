all: src/protobuf realmd

src/protobuf:
	protoc-c --c_out=./src protobuf/auth.proto
	${CC} ./src/protobuf/auth.pb-c.c -c -I./src -lprotobuf-c

realmd:
	${CC} ./src/realmd.c -o realmd

clean:
	rm -rf ./src/protobuf ./auth.pb-c.o ./realmd
