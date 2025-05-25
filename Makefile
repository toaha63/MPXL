# ==============================================
# MPXL (Multiple Precision eXtended Library)
# Makefile - Builds libmpxl.a without keeping .o files
# ==============================================

# Compiler Settings
CC       = gcc
CFLAGS   = -O3 -Wall -pedantic -fPIC
AR       = ar
ARFLAGS  = rcs
RM       = rm -f

# Library Info
LIB_NAME = mpxl
STATIC_LIB = lib$(LIB_NAME).a

# Source Files (explicitly list MPXL.c, ignore Demo.c)
LIB_SRC  = src/MPXL.c
LIB_OBJ  = $(LIB_SRC:.c=.o)
HEADER   = src/MPXL.h

# ==============================================
# Build Rules
# ==============================================

all: $(STATIC_LIB)

$(STATIC_LIB): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RM) $(LIB_OBJ)  # Delete .o after building

$(LIB_OBJ): $(LIB_SRC) $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# ==============================================
# Utility Rules
# ==============================================

.PHONY: clean install test

clean:
	$(RM) $(STATIC_LIB) src/*.o

install: $(STATIC_LIB)
	@mkdir -p /usr/local/lib /usr/local/include
	cp $(STATIC_LIB) /usr/local/lib/
	cp $(HEADER) /usr/local/include/

test: $(STATIC_LIB)
	$(CC) $(CFLAGS) -Isrc src/Demo.c -L. -l$(LIB_NAME) -o demo
	./demo
