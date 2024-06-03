# תכנות מערכות 2 - מטלה 3
מימוש של משחק קטאן מנוון.
## הרצה של הקוד
דרך טרמינל שנפתח בתיקייה עם כל הקבצים, ניתן להריץ את הפקודות הבאות:
| Command | Description |
| ---------- | ---------- |
| `make` or \n`make catan` | Creates and runs an executable `main` and runs 1 round of the game (which isn't very interesting). |
| `make main` | Creates an executable called `main` without running it. |
| `make valgrind` | Creates and runs an executable `main` and runs it through Valgrind (to check for memory leaks). |
| `make test` | Creates and runs a doctest executable called `tests` that runs dozens of tests on the program. Prints a summary of the tests. |
| `make clean` | Deletes all .o files and executables. |
