```
Copyright (c) 2023-2024 Konstantin Tutsch.
This project is distributed under the GNU General Public License.
```
A simple application to count how many **l**ines **o**f a **c**ode project has.

```
$ loc -d. -e.c -e.h
File extensions: .c .h

Directory .
- files.h: 7
- files.c: 82
- parse.h: 9
- parse.c: 110
- main.c: 136

Total lines of code: 344
```

**loc** also supports:
- multiple directories
- multiple extensions
