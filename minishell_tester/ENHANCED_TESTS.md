# Enhanced Test Suite

This document describes the 132 additional test cases added to comprehensively test advanced features.

## Test Coverage (132 tests)

### 1. Advanced Heredoc Tests (20 tests)
- Variable expansion in heredoc
- Quoted delimiters (single/double quotes) - prevents expansion
- Multiple heredocs in one command
- Heredoc with various redirections
- Heredoc with pipes
- Empty heredoc content
- Delimiters with spaces and underscores
- Tab handling (<<-)
- Nested heredoc in subshells
- Special characters and quotes inside heredoc
- Exit status variable ($?) in heredoc
- Long delimiter names

### 2. Comprehensive Subshell Tests (31 tests)
- Simple and nested subshells (2-4 levels deep)
- Subshell exit status handling
- Variable scope (set/modified in subshell)
- Export behavior in subshells
- Directory changes (cd) in subshells
- Multiple commands in subshell
- Subshells with pipes (inside, left, right, both sides)
- Subshells with all redirect types (>, >>, <)
- Subshells with logical operators (&&, ||)
- Sequential subshells
- Subshells in AND/OR chains
- Complex nested combinations
- Subshells with heredoc
- Subshells with builtins
- Empty subshells (error case)
- Subshells with wildcards
- Variable preservation across subshell boundary

### 3. Complex Quoting Edge Cases (20 tests)
- Nested quotes (single in double, double in single)
- Adjacent quotes without spaces
- Multiple quote types adjacent
- Empty quote sequences
- Quotes with variables
- Quotes around special characters (|, >, <, &&, ||)
- Escaped quotes in double quotes
- Dollar before quotes
- Multiple adjacent double quotes
- Quotes with newlines
- Mixed quotes with spaces
- Quoting empty variables
- Quotes with wildcards
- Quotes around redirects
- Backslash in double quotes
- Complex quote combinations

### 4. File Descriptor Edge Cases (15 tests)
- Multiple stdout redirects (last wins)
- Multiple input redirects
- Mixed input/output redirect ordering
- Append after overwrite and vice versa
- Redirects to/from /dev/null
- Multiple redirects in pipelines
- Redirect before command name
- Redirect in middle of arguments
- Many consecutive redirects
- Redirects with extra spaces
- Overwriting existing files
- Reading from nonexistent files

### 5. Advanced Variable Expansion (20 tests)
- Exit status ($?) after success/failure
- Exit status in subshells
- Multiple consecutive $? references
- Variables with numbers and underscores
- Variable concatenation
- Undefined variables in various contexts
- Variable followed by literal text
- Brace syntax (${VAR})
- Empty variable expansion
- Variables in heredoc
- Dollar without variable name
- Dollar with space
- Multiple consecutive dollars ($$$$)
- Variables in different quote contexts
- Variables as command names
- Variables in redirects
- Multiple $? in one command

### 6. Advanced Pipe Combinations (11 tests)
- Long pipe chains (5+ commands)
- Pipes with subshells on left/right/both
- Pipes with heredoc
- Pipes with multiple redirects
- Pipes with grep/other filters
- Empty left side of pipe
- Pipes with builtins (export, pwd, env)

### 7. Logical Operator Edge Cases (9 tests)
- AND/OR with subshells
- Three-command AND chains
- Three-command OR chains
- Mixed AND/OR combinations
- Logical operators with redirects
- Logical operators with pipes

### 8. Wildcard Advanced Tests (6 tests)
- Wildcards in subshells
- Wildcards in quotes (no expansion)
- Wildcards with no matches
- Multiple wildcard patterns
- Wildcards in variables
- Question mark wildcard (?)

## Running Enhanced Tests

```bash
# Run only enhanced tests
make test-enhanced

# Run integration + stress + enhanced (recommended)
make test-all

# Or directly:
../minishell_tester/run_tests.sh ./minishell --enhanced
../minishell_tester/run_tests.sh ./minishell --integration --stress --enhanced
```

## Total Test Count

- Integration Tests: 102
- Stress Tests: 81
- **Enhanced Tests: 132**
- **Grand Total: 315 tests**

## Implementation Notes

All enhanced tests check for:
1. No crashes (segfault, bus error, etc.)
2. Proper error handling
3. Correct exit codes
4. Spec-compliant behavior

Tests marked with `expect_error=1` verify the shell handles invalid input gracefully without crashing.
