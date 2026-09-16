*This project has been created as part of the 42 curriculum by bahbibe, ybel-hac.*

# minishell

## Description

`minishell` is a simplified re-implementation of a Unix shell (Bash-like), written in C
from scratch as a 42/1337 School project. It reads a command line, parses it, and
executes it much like a real shell: builtins run in the current process, external
programs are looked up in `PATH` and executed via `fork`/`execve`, and the usual
redirection, piping, quoting and environment-variable-expansion rules apply.

The goal of the project is to understand processes, file descriptors, and how a shell
turns a line of text into running programs.

## Features

- Interactive prompt with command history (readline).
- Executable lookup via `PATH`, or a relative/absolute path.
- Single (`'`) and double (`"`) quote handling, with `$` expansion still active inside
  double quotes.
- Environment variable expansion (`$VAR`) and exit-status expansion (`$?`).
- Redirections: `<`, `>`, `>>`, and heredoc (`<<`).
- Pipes (`|`), chaining any number of commands.
- `ctrl-C`, `ctrl-D` and `ctrl-\` behave like in Bash (new prompt, exit shell, ignored).
- Builtins: `echo` (with `-n`), `cd` (relative/absolute path, `cd -`), `pwd`, `export`,
  `unset`, `env`, `exit`.

## Instructions

### Build

```sh
make
```

The Makefile detects the platform (`uname -s`) and, on macOS, adds Homebrew's
`readline` include/lib paths automatically (`brew install readline` if you don't have
it). On Linux, install the readline development headers first:

```sh
sudo apt install libreadline-dev   # Debian/Ubuntu
```

Other Makefile rules: `make clean`, `make fclean`, `make re`.

### Run

```sh
./minishell
```

Type commands like you would in Bash, `exit` (or `ctrl-D`) to quit.

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline Library manual](https://tiswww.case.edu/php/chet/readline/rltop.html)
- `man` pages for `fork`, `execve`, `pipe`, `dup2`, `wait`, `signal`.

### AI usage

The mandatory implementation (parsing, execution, builtins) was written and defended
without AI assistance. AI (Claude) was later used, after the project had already been
evaluated, for a scoped code-review/portability pass:

- Diagnosing and fixing why the project only built on macOS and not on Linux (a macro
  name clashing with `readline`'s own headers, a missing `<stdint.h>` include, a
  hardcoded Homebrew library path in the Makefile, and modern GCC's `-fno-common`
  default breaking the project's single global variable across translation units).
- Finding and fixing an unsequenced-side-effect bug in the tokenizer that only
  corrupted parsing on Linux (compiler-dependent evaluation order).
- Finding and fixing a few file-descriptor/memory leaks and an invalid `close(-1)`
  call, using `valgrind --track-fds=yes`.
- Fixing a buffering bug where `echo`'s output could be flushed to the wrong file
  descriptor after a redirection ended, when the shell's own stdout wasn't a terminal.

All changes were reviewed and tested manually (real commands, pipes, redirections,
heredocs, `valgrind`) before being kept.
