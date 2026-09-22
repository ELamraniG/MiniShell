# minishell

A small Unix shell written in C, built as part of the **42 curriculum**. It reads
command lines with a prompt, parses them into a command tree, and executes them the
way `bash` does — with pipes, redirections, quoting, environment-variable expansion,
wildcards, signals, and a set of built-in commands. A **bonus** version adds logical
operators (`&&`, `||`) and parentheses for grouping.

## Features

### Command line
- Interactive prompt with line editing and command **history** (via `readline`)
- Runs executables by **absolute path**, **relative path**, or by searching `$PATH`
- Proper exit statuses, including `$?`

### Quoting & expansion
- Single quotes `'…'` — everything literal
- Double quotes `"…"` — literal except variable expansion
- Environment variable expansion: `$VAR`, `$?` (last exit status)
- Wildcard expansion: `*` in the current directory

### Redirections & pipes
- Input `<`, output `>`, append `>>`
- Heredoc `<<` (with expansion of the delimiter body)
- Pipelines `cmd1 | cmd2 | cmd3`

### Signals
- `Ctrl-C` — new prompt on a fresh line
- `Ctrl-D` — exits the shell (EOF)
- `Ctrl-\` — ignored, like in bash

### Built-in commands
| Builtin | Description |
|---------|-------------|
| `echo` | print arguments, supports `-n` |
| `cd` | change directory |
| `pwd` | print working directory |
| `export` | set / list environment variables |
| `unset` | remove environment variables |
| `env` | print the environment |
| `exit` | quit the shell |

### Bonus
- Logical operators `&&` and `||`
- Parentheses `( … )` for grouping and precedence
- Wildcards

## Build

Requires `cc`, `make`, and the **readline** library.

```bash
make          # builds ./minishell (mandatory)
make bonus    # builds ./minishell_bonus (&&, ||, parentheses)
make clean    # remove object files
make fclean   # remove objects and binaries
make re       # full rebuild
```

> On macOS, readline is expected under a Homebrew prefix; adjust the readline
> `-L`/`-I` paths in the `Makefile` to match your install if needed.

## Usage

```bash
./minishell
```

```console
minishell$ echo "hello $USER" | tr a-z A-Z
HELLO MOEL-AMR
minishell$ ls -la > out.txt && cat out.txt
minishell$ cat << EOF
> line one
> EOF
line one
minishell$ export NAME=42 && echo $NAME
42
minishell$ exit
```

## Project structure

```
minishell/
├── Makefile
├── includes/            # headers (mandatory)
├── includes_bonus/      # headers (bonus)
├── main_m/              # entry point & main loop
├── parsing/             # lexer, quotes, syntax checks, command tree
├── execution/           # exec tree, pipes, redirections, heredoc, signals
├── exec_continue/       # variable / argument expansion, wildcards
├── built_ins/           # echo, cd, pwd, export, unset, env, exit
├── str_utils/           # small string / libft-style helpers
└── *_bonus/             # bonus counterparts of the above
```

## How it works

1. **Read** — `readline` prints the prompt and returns the line; non-empty lines are
   added to history.
2. **Lex & parse** — the line is tokenized (words, quotes, operators, redirections)
   and syntax-checked, then built into a tree of commands.
3. **Expand** — environment variables, `$?`, and wildcards are expanded on the
   arguments; quoting rules decide what gets expanded.
4. **Execute** — the tree is walked: pipes are wired with `pipe()`/`dup2()`,
   redirections are opened, heredocs are collected, built-ins run in-process, and
   external commands are `fork`/`execve`'d. The final exit status becomes `$?`.

## Authors

- **ELamraniG** — [github.com/ELamraniG](https://github.com/ELamraniG)
- **Jihad Hamdaoui (Bladesjj)** — [github.com/Bladesjj](https://github.com/Bladesjj)

Built for the 42 *minishell* project.
