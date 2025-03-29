PREFIX?=/usr/local
DESTDIR?=
INSTALL ?= install
CFLAGS += $(shell pkg-config --cflags libpng)
LDFLAGS += $(shell pkg-config --libs libpng)

dumppng:	dumppng.c png_utils.o
	$(CC) ${CFLAGS} -o dumppng dumppng.c png_utils.o ${LDFLAGS}

png_utils.o:	png_utils.h png_utils.c
	$(CC) ${CFLAGS} -c png_utils.c

clean:
	rm -f png_utils.o dumppng

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${PREFIX}/man/man1
	${INSTALL} -m 755 ./dumppng ${DESTDIR}${PREFIX}/bin/dumppng 
	${INSTALL} -m 644 ./dumppng.1 ${DESTDIR}${PREFIX}/man/man1/dumppng.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dumppng
	rm -f ${DESTDIR}${PREFIX}/man/man1/dumppng.1

