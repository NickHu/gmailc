# gmailc version
VERSION = 0.3

# Customize below to fit your system

# compiler and linker
CC = gcc

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I/usr/include/libxml2
LIBS = -lcurl -lxml2

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\"
CFLAGS   = -std=c11 -pedantic ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# enable all warnings
CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

# GCC warnings that Clang doesn't provide:
# ifeq ($(CC),gcc)
# 	CFLAGS += -Wjump-misses-init -Wlogical-op
# endif

# Solaris
# CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
# LDFLAGS = ${LIBS}

all: CFLAGS += -Wjump-misses-init -Wlogical-op
all: CFLAGS += -O2
all: options gmailc

debug: CC = clang
debug: CFLAGS += -g -O0
debug: options gmailc

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
