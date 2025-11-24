# Minishell Tester

Comprehensive test suite for minishell implementations.

## Requirements

- bash
- timeout command (coreutils)
- diff

## Usage

```bash
# Build the tester (optional - just copies scripts)
make

# Quick test - auto-detects minishell binary
./quick_test.sh
./quick_test.sh --integration
./quick_test.sh --stress

# Run all tests with explicit binary path
./run_tests.sh ../minishell/minishell

# Run specific test suites
./run_tests.sh ../minishell/minishell --integration
./run_tests.sh ../minishell/minishell --stress
./run_tests.sh ../minishell/minishell --all

# Using make targets
make test MS_BIN=../minishell/minishell
make test-integration MS_BIN=../minishell/minishell
make test-stress MS_BIN=../minishell/minishell
```

## Test Suites

### Integration Tests (102 tests)
Comprehensive coverage of shell features:
- Echo (with/without -n)
- Environment variables ($VAR, $?, $$, etc.)
- Quotes (single, double, mixed)
- Redirections (<, >, <<, >>)
- Pipes (|)
- Logical operators (&&, ||)
- Command execution
- Built-ins (cd, pwd, export, unset, env, exit)
- Wildcards (*)
- Edge cases and error handling

### Stress Tests (81 tests)
Edge cases and destructive testing:
- Quote edge cases
- Complex pipes and redirections
- Environment variable expansion
- Wildcard expansion
- Long inputs
- Special characters
- Error handling

### Advanced Stress Tests
Complex combinations and corner cases.

### Complex Expansion Tests
Advanced word expansion scenarios.

### Comprehensive Tests
Full feature coverage.

### Destructive Tests
Boundary conditions and error paths.

## Exit Codes

- 0: All tests passed
- 1: Some tests failed
- 2: Usage error or missing binary

## Environment Variables

You can customize test behavior:

```bash
# Use custom timeout (default: 8 seconds)
TIMEOUT=10 ./run_tests.sh ./minishell

# Custom environment for tests
ENV_VARS="PATH=/usr/bin:/bin HOME=/tmp" ./run_tests.sh ./minishell
```

## Structure

```
minishell_tester/
├── README.md
├── Makefile
├── run_tests.sh              # Main test runner
├── integration_tests.sh      # 102 integration tests
├── stress_test.sh           # 81 stress tests
├── advanced_stress_test.sh  # Advanced edge cases
├── complex_expansion_test.sh # Expansion tests
├── comprehensive_test.sh    # Comprehensive suite
└── destructive_test.sh      # Destructive testing
```

## License

This tester is designed for the 42 school minishell project.
