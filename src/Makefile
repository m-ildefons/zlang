CC=gcc
CFLAGS= -march=native -mtune=native
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
			-Wno-unused \
			-Wnull-dereference
			#-ansi
LDFLAGS= -L/usr/lib/x86_64-linux-gnu/
OUT=../bin/boa

SRC=$(notdir $(wildcard ./*.c))
HEADER=$(notdir $(wildcard ./*.h))
OBJ=$(SRC:%.c=%.o)

.PHONY: all clean clean_bin clean_obj clean_after_build lint

all: $(OUT) clean_after_build

$(OUT): $(OBJ)
	$(CC) -g $(OBJ) $(LDFLAGS) -o $(OUT)

%.o: %.c
	@echo "$(CC) compiling $<"
	@$(CC) -O0 -g $(C_WARN_FLAGS) $(CFLAGS) -c $<

clean_bin:
	rm -f ../bin/*

clean_obj:
	rm -f ./*.o

clean: clean_bin clean_obj

clean_after_build: $(OUT)
	$(MAKE) clean_obj

lint:
	splint $(SRC) $(HEADER)

