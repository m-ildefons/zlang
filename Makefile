PWD=$(shell pwd)
BOA=$(PWD)/bin/boa
export
BIN_DIR=bin
MKDIR_P=mkdir -p

.PHONY: src \
	clean \
	test \
	lint \
	directories

all: directories src
test: directories src
	$(MAKE) -C test
clean: clean_src
src: clean_src
	$(MAKE) -C src
clean_src:
	$(MAKE) clean -C src

lint: clean_src
	$(MAKE) lint -i -C src

directories: ${BIN_DIR}

${BIN_DIR}:
	${MKDIR_P} ${BIN_DIR}
