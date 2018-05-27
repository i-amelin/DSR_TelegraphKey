all: libmorse.a

libmorse.a: morse.c
	@printf " BUILDING THE MORSE LIBRARY\n"
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o morse.o -c morse.c
	$(Q)$(AR) rcs libmorse.a morse.o

install:
	ln -sf libmorse.a ../ws_4/libmorse.a

BUILD_HOME=../../..

include $(BUILD_HOME)/dsr_workshop/Makefile.include
