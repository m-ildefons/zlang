CC=gcc
CFLAGS= -O0 -g -march=native -mtune=native
C_DEBUG_FLAGS= -O0 -g -fsanitize=address -fno-omit-frame-pointer
C_WARN_FLAGS= -Wall \
			-Wextra \
			-Wpedantic \
			-Winline \
			-Wundef \
			-Wshadow \
			-Winit-self \
			-Wformat-nonliteral \
			-Wcast-qual \
			-Wcast-align \
			-Wsign-conversion \
			-Wpointer-arith \
			-Wbad-function-cast \
			-Wmissing-prototypes \
			-Wmissing-include-dirs \
			-Wmissing-declarations \
			-Wstrict-prototypes \
			-Wstrict-overflow=5 \
			-Wnested-externs \
			-Wwrite-strings \
			-Wno-unused-parameter \
			-Wfloat-equal \
			-Wdisabled-optimization \
			-Wformat=2 \
			-Wno-format-extra-args \
			-Wformat-security \
			-Wformat-nonliteral \
			-Wformat-signedness \
			-Wlogical-op \
			-Wredundant-decls \
			-Wswitch-default \
			-Wunused-function \
			-Wunused-label \
			-Wunused-value \
			-Wunused-variable \
			-Wnull-dereference
			#-ansi
LDFLAGS= -L/usr/lib/x86_64-linux-gnu/
LD_DEBUG_FLAGS= -lasan -fno-omit-frame-pointer -fsanitize=address
OUT=../bin/boa
OUT_DEBUG=../bin/boa_debug

SRC=$(notdir $(wildcard ./*.c))
HEADER=$(notdir $(wildcard ./*.h))
OBJ=$(SRC:%.c=%.o)
OBJ_DEBUG=$(SRC:%.c=%_debug.o)

.PHONY: all debug clean clean_bin clean_obj clean_after_build lint

all: $(OUT) clean_after_build

debug: $(OUT_DEBUG) clean_after_build

$(OUT): $(OBJ)
	$(CC) -g $(OBJ) $(LDFLAGS) -o $(OUT)

$(OUT_DEBUG): $(OBJ_DEBUG)
	$(CC) -g $(OBJ_DEBUG) $(LD_DEBUG_FLAGS) -o $(OUT_DEBUG)

%.o: %.c
	@echo "$(CC) compiling $<"
	@$(CC) $(C_WARN_FLAGS) $(CFLAGS) -c $<

%_debug.o: %.c
	@echo "$(CC) debug compiling $<"
	@$(CC) $(C_WARN_FLAGS) $(C_DEBUG_FLAGS) -c $< -o $@

clean_bin:
	rm -f ../bin/*

clean_obj:
	rm -f ./*.o

clean: clean_bin clean_obj

clean_after_build:
	$(MAKE) clean_obj

lint:
	splint $(SRC) $(HEADER)

