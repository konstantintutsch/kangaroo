# kangaroo 🦘

```
Copyright (c) 2023-2024 Konstantin Tutsch.
This project is distributed under the GNU General Public License.
```

A simple application to count how many lines of code a project has.

```
$ kangaroo --directory . --extension .c --extension .h
File extensions: .c .h

- compare.c: 79 lines
- compare.h: 7 lines
- files.h: 9 lines
- files.c: 194 lines
- main.c: 88 lines
(All files @ /home/konstantin/Code/kangaroo: 377 lines)

Total lines of code: 377
```

## Installation

### Prerequisites

- make (*v4.4+*)
- gcc (*v13.2+*)
- [libzarg](https://github.com/konstantintutsch/zarg) (*v0.0.3*)

### Compile

Compile kangaroo with `make`:

```
make build
```

The kangaroo binary is located at `./kangaroo` by default. You can move it to `~/.local/bin` if you'd like.
