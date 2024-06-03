# תכנות מערכות 2 - מטלה 3
מימוש של משחק קטאן מנוון.

## **אופן המימוש והחלוקה בקוד**
### לוח המשחק
הלוח מומש כגרף: אבן הבניין הבסיסית ביותר בבניית הלוח היא המחלקה `building_slot`, ולמרות שאינה משומשת באופן ישיר בבניית הלוח, ממנה יורשות המחלקות `townSlot` ו-`roadSlot` שהן למעשה הקודקודים והצלעות של הלוח בהתאמה. לכל `townSlot` יש 2-3 שכנים שהם `roadSlot` ולכל `roadSlot` יש 2 שכנים שהם `townSlot`. בנוסף, לכל `building_slot` יש `ID` המייצג מספר זיהוי ומצביע לשחקן `owner` המצביע על השחקן שבנה עיר/דרך בו. לכל שני `townSlot` לעולם לא יהיה את אותו ה-`id` וגם לכל שני `roadSlot` לא יהיה את אותו ה-`id`.
## הרצה של הקוד
דרך טרמינל שנפתח בתיקייה עם כל הקבצים, ניתן להריץ את הפקודות הבאות:
| Command | Description |
| ---------- | ---------- |
| `make` or <br>`make catan` | Creates and runs an executable `main` and runs 1 round of the game (which isn't very interesting). |
| `make main` | Creates an executable called `main` without running it. |
| `make valgrind` | Creates and runs an executable `main` and runs it through Valgrind (to check for memory leaks). |
| `make test` | Creates and runs a doctest executable called `tests` that runs dozens of tests on the program. Prints a summary of the tests. |
| `make clean` | Deletes all .o files and executables. |
