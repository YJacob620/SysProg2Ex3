# תכנות מערכות 2 - מטלה 3
4 ספרות אחרונות של ת"ז: 1536

מייל: yairjacob620@gmail.com

מימוש של משחק קטאן מנוון בהתאם להנחיות המטלה.

## **אופן המימוש והחלוקה בקוד**
### לוח המשחק
הלוח מומש כגרף. אבן הבניין הבסיסית ביותר בבניית הלוח היא המחלקה `building_slot`, ולמרות שאינה משומשת באופן ישיר בבניית הלוח, ממנה יורשות המחלקות `townSlot` ו-`roadSlot` שהן למעשה הקודקודים והצלעות של הלוח בהתאמה. לכל `townSlot` יש 2-3 שכנים שהם `roadSlot` ולכל `roadSlot` יש 2 שכנים שהם `townSlot`. 

לכל `building_slot` יש `ID` המייצג מספר זיהוי ומצביע לשחקן המצביע על השחקן שבנה עיר/דרך בו. לכל שני `townSlot` לעולם לא יהיה את אותו ה-`id` וגם לכל שני `roadSlot` לא יהיה את אותו ה-`id`. באופן אינטואיטיבי, הקודקודים והצלעות ממוספרים בסדר עולה, בספירלה עם כיוון השעון, מהמדבר (נקרא לו מעגל 0) עד למעטפת המשושים החיצונית ביותר של הלוח (נקרא לה מעגל 2). הצלעות שמחברות בין מעגלים שונים מוספרו אחרונות. ראו [catan board.jpg_](https://github.com/YJacob620/SysProg2Ex3/blob/main/_catan%20board.jpg) להבהרה (התמונה גם מודבקת בתחתית ה-`README`). במימוש צלעות אלו נקראות "Special Road Slots".

### איך משחקים
המשחק מתנהל דרך ה-command line לאחר הרצת `main`. בכל תור ובכל פעולה יוצגו פלטים מתאימים עבור מצב המשחק, מה להקליד והאם פעולות שבוצעו הוצלחו או לא.
## הרצה של התכנית
דרך טרמינל שנפתח בתיקייה עם כל הקבצים, ניתן להריץ את הפקודות הבאות:
| Command | Description |
| ---------- | ---------- |
| `make` or <br>`make catan` | Creates and runs an executable `main` and runs 1 round of the game (which isn't very interesting). |
| `make main` | Creates an executable called `main` without running it. |
| `make valgrind` | Creates and runs an executable `main` and runs it through Valgrind (to check for memory leaks). |
| `make test` | Creates and runs a doctest executable called `tests` that runs dozens of tests on the program. Prints a summary of the tests. |
| `make clean` | Deletes all .o files and executables. |

![alt text](https://github.com/YJacob620/SysProg2Ex3/blob/main/_catan%20board.jpg)
