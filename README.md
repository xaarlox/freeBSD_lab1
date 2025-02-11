# freeBSD_lab1
## Завдання
Створіть функцію, яка приймає вхідний файл у форматі CSV та виводить його вміст у вигляді відформатованої таблиці. Додайте підтримку різних роздільників, таких як кома, крапка з комою або табуляція.
## Початок роботи
Спочатку я вирішила встановити середовище робочого столу, щоб забезпечити комфортну роботу з системою. Для цього я скористалася [відео](https://www.youtube.com/watch?v=mWUL7hhg-vs&ab_channel=FreeBSDFoundation), автор якого надав покрокову інструкцію з налаштування.
Мій робочий стіл має вигляд:
![image](https://github.com/user-attachments/assets/d3e55c91-ec13-429b-94f1-848aee110e3a)
## Реалізація завдання
### Функція для друку роздільної лінії
```
void print_separator(int *col_widths, int col_count) {
    printf("+");
    for (int i = 0; i < col_count; i++) {
        for (int j = 0; j < col_widths[i] + 2; j++) printf("-");
        printf("+");
    }
    printf("\n");
}
```
Ця функція приймає масив `col_widths`, який містить ширину кожного стовпця. Використовує `+----+` для розділення стовпців та додає два додаткові пробіли між текстом, що забезпечує гарне форматування.
### Основна функція `print_table()`
Приймає два аргументи:
- `filename` - шлях до CSV-файлу;
- `delimiter` - роздільник, який використовується у файлі.
#### Відкриття файлу
```
FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
```
* `fopen(filename, "r")` - відкриває файл у режимі читання;
* Якщо файл не вдалося відкрити, то виводиться повідомлення про помилку.
#### Оголошення змінних
```
char line[MAX_LINE_LENGTH];
    int col_widths[MAX_COLUMNS] = {0};
    int col_count = 0;
    char *rows[1000][MAX_COLUMNS];
    int row_count = 0;
```
+ `line[MAX_LINE_LENGTH]` - буфер для читання рядків з файлу;
+ `col_widths[MAX_COLUMNS]` - масив для збереження максимальної ширини кожного стовпця;
+ `rows[1000][MAX_COLUMNS]` - двовимірний масив для збереження всіх значень CSV-файлу;
+ `row_count` - кількість зчитаних рядків.
#### Перший прохід: визначення ширини кожного стовпця
```
while (fgets(line, sizeof(line), file)) {
        char *token;
        char *rest = line;
        col_count = 0;

        while ((token = strsep(&rest, &delimiter)) != NULL && col_count < MAX_COLUMNS) {
            token[strcspn(token, "\n")] = '\0'; // Видаляємо новий рядок
            int len = strlen(token);
            if (len > col_widths[col_count]) {
                col_widths[col_count] = len;
            }
            rows[row_count][col_count] = strdup(token);
            col_count++;
        }
        row_count++;
    }
```
Спочатку за допомогою функції `fgets()` читається файл рядок за рядком. `strsep(&rest, &delimiter)` розбиває рядок на частини за вказаним роздільником. `strcspn(token, "\n")` шукає символ нового рядка та видаляє його. `strlen(token)` визначає довжину тексту в осередку, щоб знайти найширше значення для стовпця. `strdup(token)` створює копію рядка, щоб зберегти дані. Після проходження всіх рядків файл закривається.
#### Другий прохід: виведення таблиці
```
print_separator(col_widths, col_count);
    for (int i = 0; i < row_count; i++) {
        printf("|");
        for (int j = 0; j < col_count; j++) {
            printf(" %-*s |", col_widths[j], rows[i][j]);
            free(rows[i][j]); // Звільняємо пам’ять
        }
        printf("\n");
        print_separator(col_widths, col_count);
    }
}
```
Спочатку друкується горизонтальна роздільна лінія `(print_separator)`. Далі друкується кожен рядок у форматі `| data |`. `free(rows[i][j])` звільняє виділену пам'ять для уникнення витоків.
### Головна функція `main()`
```
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <delimiter>\n", argv[0]);
        return 1;
    }
    char delimiter = argv[2][0];
    print_table(argv[1], delimiter);
    return 0;
}
```
Спочатку перевіряється, чи користувач передав 2 аргументи (ім'я файлу та роздільник). Якщо аргументів недостатньо, виводиться повідомлення про правильний формат команди. `argv[2][0]` - отримує перший символ другого аргументу (роздільник). Викликається функція для друку таблиці.
## Компіляція та виконання програми
![image](https://github.com/user-attachments/assets/5e5cf952-48cb-43b9-9ea4-b9c45b1d4402)
## Висновок
Отже, було написано програму, яка обробляє CSV-файли з будь-яким роздільником. Ця програма використовує двопрохідний алгоритм: спочатку визначає ширину стовпців, а потім друкує відформатовану таблицю. Всі динамічно виділені рядки очищуються за допомгою `free()`.
