# gmailc version
VERSION = 0.2

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I/usr/include/libxml2
LIBS = -lcurl -lxml2

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\"
# CFLAGS   = -g -std=c11 -pedantic -Wall -O0 ${INCS} ${CPPFLAGS}
CFLAGS   = -std=c11 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# Solaris
# CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
# LDFLAGS = ${LIBS}

# compiler and linker
CC = clang

all: options gmailc

options:
	@echo gmailc build options
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

clean:
	@rm gmailc
	@rm curl_handler.o
	@rm xml_handler.o

gmailc: curl_handler.o xml_handler.o
