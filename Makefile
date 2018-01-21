all: realmd

realmd:
	${CC} src/realmd.c -o realmd

clean:
	rm -rf realmd
