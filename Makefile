NAME = libasm.a

SRC_DIR = src
OUT_DIR = objects
TEST_BIN_NAME = test

NASM = nasm -f elf64

SRCS = $(wildcard $(SRC_DIR)/*.s)
OBJS = $(patsubst $(SRC_DIR)/%.s, $(OUT_DIR)/%.o, $(SRCS))
LINK_CRITERION = -lcriterion


.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

# Build target depends on the output directory and object files
.PHONY: build
build: $(OUT_DIR) $(OBJS) $(NAME) ## Builds the .a lib file in the root

$(NAME): $(OBJS)
	@ar -rcs $(NAME) $(OBJS)

# Rule to create the output directory
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Rule to create object files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.s
	$(NASM) $< -o $@

# Run tests
.PHONY: run-tests
run-tests: build ## Runs all tests using criterion
	@gcc $(LINK_CRITERION) main.c $(NAME) -o $(TEST_BIN_NAME)
	@./$(TEST_BIN_NAME) --verbose

.PHONY: clean
clean: ## Deletes the lib file (.a)
	@rm -f $(NAME)
	@rm -f $(TEST_BIN_NAME)

.PHONY: fclean
fclean: clean ## Deletes the lib file, clears the objects folder, and deletes the test binary
	@rm -rf $(OUT_DIR)
