# kangaroo 🦘

```
Copyright (c) 2023-2024 Konstantin Tutsch.
This project is distributed under the GNU General Public License.
```

A simple application to count how many lines of code a project has.

```
$ kangaroo --directory . --extension .c --extension .h
File extensions: .c .h

- files.h: 8 lines
- compare.h: 7 lines
- files.c: 165 lines
- compare.c: 79 lines
- main.c: 88 lines
(All files @ /home/konstantin/Code/kangaroo: 347 lines)

Total lines of code: 347
```

## Usage

| Argument                | Description                                                                                         | Type   |
| ----------------------- | --------------------------------------------------------------------------------------------------- | ------ |
| `--directory`, `-d`     | Which *directory/directories* to scan for files. Can be given multiple times.                       | String |
| `--extension`, `-e`     | What a filename should end with to be included. Can be given multiple times.                        | String |
| `--recursive`, `-r`     | Scan not only the directories from `-d` arguments, but also sub-, subsub- and so forth- directories | Flag   |
