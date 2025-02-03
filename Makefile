SRC_DIR = ./src

OBJ_DIR = ./build

OUT_DIR = ./bin

NASM = nasm -f elf64

LINK = ld -m elf_x86_64

HELLO_SRC = $(SRC_DIR)/hello.asm
HELLO_CMD =

.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: build-hello
build-hello: $(HELLO_SRC) ## Builds the hello cmd
	@$(NASM) $(HELLO_SRC) -o $(OBJ_DIR)/hello.o
	@$(LINK) $(OBJ_DIR)/hello.o -o $(OUT_DIR)/hello

.PHONY: run-hello
run-hello: build-hello ## Builds and runs the Hello cmd
	@$(OUT_DIR)/hello
