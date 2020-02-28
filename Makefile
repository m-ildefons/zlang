PWD=$(shell pwd)
BOA=$(PWD)/bin/boa
export

.PHONY: src clean test lint
all: src
test: src
	$(MAKE) -C test
clean: clean_src
src: clean_src
	$(MAKE) -C src
clean_src:
	$(MAKE) clean -C src

lint: clean_src
	$(MAKE) lint -i -C src
