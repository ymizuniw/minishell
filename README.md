# Minishell

A minimal bash-like shell implementation (42 school project).

## Features

- Command execution with PATH resolution
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Pipes (`|`)
- Redirections (`<`, `>`, `<<`, `>>`)
- Logical operators (`&&`, `||`)
- Environment variable expansion (`$VAR`, `$?`, `$$`)
- Quote handling (single `'`, double `"`)
- Wildcard expansion (`*`)
- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\)
- Command history (up/down arrows)

## Building

```bash
make        # Build minishell
make clean  # Remove object files
make fclean # Remove all build artifacts
make re     # Rebuild from scratch
```

## Usage

```bash
./minishell
```

## Testing

Tests are maintained in a separate repository: [minishell_tester](../minishell_tester)

### Running Tests

```bash
# Run all tests (integration + stress)
make test

# Run integration tests only (102 tests)
make test-integration

# Run stress tests only (81 tests)
make test-stress

# Using the tester directly
cd ../minishell_tester
./run_tests.sh ../minishell/minishell
./run_tests.sh ../minishell/minishell --integration
./run_tests.sh ../minishell/minishell --stress
```

### Test Coverage

- **Integration Tests (102)**: Core functionality and feature coverage
- **Stress Tests (81)**: Edge cases, boundary conditions, error handling
- **Additional Suites**: Advanced, complex expansion, comprehensive, destructive tests

## Project Structure

```
minishell/
├── Makefile
├── README.md
├── includes/           # Header files
│   ├── minishell.h
│   ├── lexer.h
│   ├── parser.h
│   ├── executor.h
│   └── ...
├── src/                # Source files
│   ├── main.c
│   ├── builtin/        # Built-in commands
│   ├── lexer/          # Tokenization & word expansion
│   ├── parser/         # Syntax analysis & AST generation
│   ├── executer/       # Command execution
│   ├── data_management/
│   ├── env_management/
│   ├── signal_management/
│   └── ft_readline/    # Custom readline implementation
├── libft/              # Standalone C utility library
└── documents/          # Technical documentation
```

## Architecture

**Lexer (Tokenization)**
- Tokenizes input while preserving quotes
- Identifies metacharacters (pipes, redirections, operators)
- Groups tokens by quote context

**Parser (Syntax Analysis)**
- Validates syntax
- Builds Abstract Syntax Tree (AST)
- Handles operator precedence

**Executor (Expansion & Execution)**
- Expands variables (`$VAR`, `$?`, `$$`)
- Expands wildcards (`*`)
- Handles quotes (removes and applies quoting rules)
- Executes commands with proper I/O redirection

## Dependencies

- `readline` library (for line editing and history)
- Standard C library
- POSIX system calls

### macOS (Homebrew)

```bash
brew install readline
```

The Makefile is configured for Homebrew's readline location by default.

## Documentation

Technical documentation is available in the `documents/` directory:

- `README.md` - Project requirements and specifications
- `executor.md` - Execution engine details
- `PARSER_TO_FIX.md` - Parser notes
- `POSIX_SHELL_GRAMMAR.md` - Shell grammar reference
- `signal.md` - Signal handling
- `heredoc_management/` - Heredoc implementation notes

## libft

This project includes `libft`, a standalone C utility library providing:
- String manipulation (ft_strlen, ft_strcmp, ft_split, etc.)
- Memory management (xmalloc, xcalloc, xfree)
- Conversion functions (ft_atoi, ft_atoll, ft_itoa)
- List utilities (ft_lst_last, ft_lst_add_back)

See [libft/README.md](libft/README.md) for details.

## License

This project is part of the 42 school curriculum.
