#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_COLUMNS 100

void print_separator(int *col_widths, int col_count) {
    printf("+");
    for (int i = 0; i < col_count; i++) {
        for (int j = 0; j < col_widths[i] + 2; j++) printf("-");
        printf("+");
    }
    printf("\n");
}

void print_table(const char *filename, char delimiter) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int col_widths[MAX_COLUMNS] = {0};
    int col_count = 0;
    char *rows[1000][MAX_COLUMNS];
    int row_count = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token;
        char *rest = line;
        col_count = 0;

        while ((token = strsep(&rest, &delimiter)) != NULL && col_count < MAX_COLUMNS) {
            token[strcspn(token, "\n")] = '\0';
            int len = strlen(token);
            if (len > col_widths[col_count]) {
                col_widths[col_count] = len;
            }
            rows[row_count][col_count] = strdup(token);
            col_count++;
        }
        row_count++;
    }
    fclose(file);

    print_separator(col_widths, col_count);

    for (int i = 0; i < row_count; i++) {
        printf("|");
        for (int j = 0; j < col_count; j++) {
            printf(" %-*s |", col_widths[j], rows[i][j]);
            free(rows[i][j]);
        }
        printf("\n");
        print_separator(col_widths, col_count);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <delimiter>\n", argv[0]);
        return 1;
    }

    char delimiter = argv[2][0];
    print_table(argv[1], delimiter);
    return 0;
}