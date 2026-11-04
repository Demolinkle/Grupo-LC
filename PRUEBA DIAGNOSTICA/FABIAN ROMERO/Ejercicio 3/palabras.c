#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to associate C keywords with their translation
typedef struct {
    const char *english;
    const char *spanish;
    int counter;
} Keyword;

// Table containing the main C reserved keywords
static Keyword table[] = {
    {"int", "entero", 0},
    {"char", "caracter", 0},
    {"float", "flotante", 0},
    {"double", "doble", 0},
    {"if", "si", 0},
    {"else", "sino", 0},
    {"for", "para", 0},
    {"while", "mientras", 0},
    {"return", "retornar", 0},
    {"void", "vacio", 0},
    {"struct", "estructura", 0},
    {"break", "romper", 0},
    {"switch", "segun", 0},
    {"case", "caso", 0},
    {"default", "por_defecto", 0}
};
#define NUM_KEYWORDS (sizeof(table) / sizeof(table[0]))

// 1. Dynamic file loading into memory using malloc and realloc
char* load_file(const char *path, size_t *size_out) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    size_t capacity = 512, size = 0;
    char *buffer = malloc(capacity);
    if (!buffer) { fclose(f); return NULL; }

    int c;
    while ((c = fgetc(f)) != EOF) {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char *temp = realloc(buffer, capacity);
            if (!temp) { free(buffer); fclose(f); return NULL; }
            buffer = temp;
        }
        buffer[size++] = (char)c;
    }
    buffer[size] = '\0';
    fclose(f);
    *size_out = size;
    return buffer;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s source_file.c\n", argv[0]);
        return 1;
    }

    size_t size;
    char *content = load_file(argv[1], &size);
    if (!content) {
        printf("Error: Could not read the file.\n");
        return 1;
    }

    FILE *f_trad = fopen("translated_code.txt", "w");
    FILE *f_rep = fopen("keywords_report.txt", "w");

    fprintf(f_rep, "=== KEYWORDS REPORT ===\n");
    fprintf(f_rep, "Analyzed file: %s (%zu bytes loaded into dynamic memory)\n\n", argv[1], size);

    // 2. Parse the dynamic buffer character by character
    size_t i = 0;
    while (i < size) {
        if (isalpha(content[i]) || content[i] == '_') {
            size_t start = i;
            while (i < size && (isalnum(content[i]) || content[i] == '_')) {
                i++;
            }
            size_t len = i - start;
            char *word = malloc(len + 1);
            memcpy(word, &content[start], len);
            word[len] = '\0';

            // Check if the word is a reserved keyword
            int found = 0;
            for (size_t j = 0; j < NUM_KEYWORDS; j++) {
                if (strcmp(word, table[j].english) == 0) {
                    table[j].counter++;
                    fprintf(f_trad, "%s", table[j].spanish);
                    fprintf(f_rep, "-> '%s' translated to '%s'\n", table[j].english, table[j].spanish);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                fprintf(f_trad, "%s", word);
            }
            free(word);
        } else {
            fputc(content[i], f_trad);
            i++;
        }
    }

    // 3. Write summary to the report
    fprintf(f_rep, "\n=== OCCURRENCE SUMMARY ===\n");
    int total = 0;
    for (size_t j = 0; j < NUM_KEYWORDS; j++) {
        if (table[j].counter > 0) {
            fprintf(f_rep, "%s -> %s : %d times\n", table[j].english, table[j].spanish, table[j].counter);
            total += table[j].counter;
        }
    }
    fprintf(f_rep, "\nTotal reserved keywords found: %d\n", total);

    // Close files and free the main dynamic memory buffer
    fclose(f_trad);
    fclose(f_rep);
    free(content);

    printf("Process successful!\nFiles generated: 'translated_code.txt' and 'keywords_report.txt'\n");
    return 0;
}