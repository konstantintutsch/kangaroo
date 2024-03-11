# kangaroo 🦘

```
Copyright (c) 2023-2024 Konstantin Tutsch.
This project is distributed under the GNU General Public License.
```

A simple application to count how many lines of a code project has.

```
$ kangaroo -d. -e.c -e.h
File extensions: .c .h

- parse.h: 8 lines
- compare.h: 7 lines
- files.h: 8 lines
- main.c: 111 lines
- compare.c: 81 lines
- parse.c: 91 lines
- files.c: 162 lines
(All files @ /home/konstantin/Code/kangaroo: 468 lines)

Total lines of code: 468
```

## Usage

| Argument | Description                                                                                         | Type  |
| -------- | --------------------------------------------------------------------------------------------------- | ----- |
| `-d`     | Which *directory/directories* to scan for files. Can be given multiple times.                       | Value |
| `-e`     | What a filename should end with to be included. Can be given multiple times.                        | Value |
| `-r`     | Scan not only the directories from `-d` arguments, but also sub-, subsub- and so forth- directories | Flag  |
