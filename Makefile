OBJECTS += $(patsubst %.c,%.o,$(wildcard src/*.c))

CPPFLAGS += -Iinclude -I../
CFLAGS += -fPIC

include Makedefs
