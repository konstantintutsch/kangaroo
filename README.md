```
Copyright (c) 2023-2024 Konstantin Tutsch.
This project is distributed under the GNU General Public License.
```
A simple application to count how many **l**ines **o**f a **c**ode project has.

```
$ loc -d. -e.c -e.md
File extensions: .c .md

Directory .
- README.md: 20
- main.c: 266

Total lines of code: 286
```

**loc** also supports:
- multiple directories
- multiple extensions
