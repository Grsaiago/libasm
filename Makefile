SRC_DIR = src
OUT_DIR = objects

SRCS = $(wildcard $(SRC_DIR)/*.s)
OBJS = $(patsubst $(SRC_DIR)/%.s, $(OUT_DIR)/%.o, $(SRCS))

NASM = nasm -f elf64

.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

# Build target depends on the output directory and object files
.PHONY: build
build: $(OUT_DIR) $(OBJS)

# Rule to create the output directory
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Rule to create object files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.s
	$(NASM) $< -o $@

# Run tests
.PHONY: run-tests
t: build ## Runs all tests using criterion
	@gcc -lcriterion main.c $(wildcard $(OUT_DIR)/*.o) -o tests
	@./tests

.PHONY: clean
clean: ## Clears the objects folder
	@rm -f $(OUT_DIR)/*.o

.PHONY: fclean
fclean: clean ## Clears the objects folder and the test binary
	@rm tests
