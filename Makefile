PWD=$(shell pwd)
ZLANG=$(PWD)/bin/zlang
export
BIN_DIR=bin
MKDIR_P=mkdir -p

.PHONY: src \
	clean \
	test \
	lint \
	directories

all: directories src

debug: directories src_debug

test: directories src
	$(MAKE) -C test

clean: clean_src

src: clean_src
	$(MAKE) -C src

src_debug: clean_src
	$(MAKE) debug -j -C src

clean_src:
	$(MAKE) clean -C src

lint: clean_src
	$(MAKE) lint -i -C src

directories: ${BIN_DIR}

${BIN_DIR}:
	${MKDIR_P} ${BIN_DIR}
