# Minishell Tester Makefile
# This tester is a standalone program for testing minishell implementations

.PHONY: all clean install test help

INSTALL_DIR = .
SCRIPTS = run_tests.sh \
          integration_tests.sh \
          stress_test.sh \
          advanced_stress_test.sh \
          complex_expansion_test.sh \
          comprehensive_test.sh \
          destructive_test.sh

all: install

install:
	@echo "Installing minishell tester..."
	@chmod +x $(SCRIPTS)
	@echo "Done! Use './run_tests.sh <path_to_minishell>' to run tests."

test:
	@if [ -z "$(MS_BIN)" ]; then \
		echo "Error: Please specify MS_BIN=<path_to_minishell>"; \
		echo "Example: make test MS_BIN=../minishell/minishell"; \
		exit 1; \
	fi
	@./run_tests.sh $(MS_BIN)

test-integration:
	@if [ -z "$(MS_BIN)" ]; then \
		echo "Error: Please specify MS_BIN=<path_to_minishell>"; \
		exit 1; \
	fi
	@./run_tests.sh $(MS_BIN) --integration

test-stress:
	@if [ -z "$(MS_BIN)" ]; then \
		echo "Error: Please specify MS_BIN=<path_to_minishell>"; \
		exit 1; \
	fi
	@./run_tests.sh $(MS_BIN) --stress

clean:
	@echo "Cleaning up test artifacts..."
	@rm -f ./*.tmp
	@echo "Done."

help:
	@echo "Minishell Tester"
	@echo ""
	@echo "Usage:"
	@echo "  make              Install and set permissions"
	@echo "  make test MS_BIN=<path>  Run all tests"
	@echo "  make test-integration MS_BIN=<path>  Run integration tests only"
	@echo "  make test-stress MS_BIN=<path>  Run stress tests only"
	@echo "  make clean        Remove temporary files"
	@echo ""
	@echo "Examples:"
	@echo "  make"
	@echo "  make test MS_BIN=../minishell/minishell"
	@echo "  ./run_tests.sh ../minishell/minishell"
	@echo "  ./run_tests.sh ../minishell/minishell --integration"
