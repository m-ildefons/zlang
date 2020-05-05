CC=$(ZLANG)
DIR=$(sort $(dir $(wildcard */)))
SUBDIR=$(DIR:%/=%_subdir)
SRC=$(notdir $(wildcard ./*.x))
RES=$(SRC:%.x=%.res)
OUT=$(SRC:%.x=%.out)
ASM=$(SRC:%.x=%.s)
BIN=$(SRC:%.x=%.bin)
TEST=$(SRC:%.x=%_test)

.PHONY: all clean

all: $(TEST) $(SUBDIR) clean
	@echo "$(shell pwd) \e[92mALL SUCCESS\e[39m"

%_subdir:
	@$(MAKE) -C $(@:%_subdir=%)

%_test: %.out %.res
	@diff -q $?; \
	if [ $$? -eq 0 ]; then \
		echo "\e[92mSUCCESS \e[39m($@)"; \
	else \
		echo "\e[91mFAIL \e[39m($@)"; \
	fi

%.out: %.bin
	@./$< > /dev/null; \
	echo $$? > $@

%.bin: %.s
	gcc $< -o $@;

%.s: %.x
	@$(CC) $< > /dev/null; \

clean: $(OUT) $(BIN) $(ASM)
	@rm $?
