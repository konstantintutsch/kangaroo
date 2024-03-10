```
Copyright (c) 2023-2024 Konstantin Tutsch.
This project is distributed under the GNU General Public License.
```
A simple application to count how many **l**ines **o**f a **c**ode project has.

```
$ loc -d. -e.c -e.h
File extensions: .c .h

Directory /home/konstantin/Code/loc
- files.c: 82
- files.h: 7
- parse.h: 8
- compare.h: 7
- compare.c: 80
- parse.c: 90
- main.c: 142

Total lines of code: 416
```

**loc** also supports:
- multiple directories
- multiple extensions
