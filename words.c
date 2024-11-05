
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

#define BUFFER_SIZE 4096
#define TABLE_SIZE 1000
#define MAX_WORD_LENGTH 100
#define MAX_PATH 4096


// Creation of struct
typedef struct {
    char words[MAX_WORD_LENGTH];
    int counter;

}WORD;

WORD word_table[TABLE_SIZE];
int count = 0;

int word_finder(const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_table[i].words, word) == 0) {
            return i;
        }
    }
    return -1;
}

// Adds words to table
void word_add(char *word) {
    int index = word_finder(word);
    if (index != -1) {
        word_table[index].counter++;
    }
    else {
        if (count < TABLE_SIZE ) {
            strcpy(word_table[count].words, word);
            word_table[count].counter = 1;
            count++;
        }
        else {
            fprintf("Word Table is full");
        }
    }
}

// Will compare and sort words to be printed
int compare_words(const void *a, const void *b ) {
    WORD *entry_a = (WORD *)a;
    WORD *entry_b = (WORD *)b;

    if(entry_b->counter != entry_a->counter) {
        return entry_b->counter - entry_a->counter;
    }

    return strcmp(entry_a->counter, entry_b->counter);
}

void print_words() {
    qsort(word_table, count, sizeof(WORD), compare_words);

    for (int i = 0; i < count; i++) {
        printf("%s &d\n", word_table[i].words, word_table[i].counter );
    }
}

// Will read throught a file ending in .txt
void process_file(const char *filename) {
    int file = open(filename, O_RDONLY);
    if (file < 0) {
        perror("fopen");
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes;
    char temp[MAX_WORD_LENGTH];
    int length = 0;

        while ((bytes = read(file, buffer, sizeof(buffer)-1)) > 0 ) {
            buffer[bytes] = '\0';
            char *ptr = buffer;

        while (*ptr) {
            if (isalpha(*ptr) || *ptr == '\'' || (*ptr == '-' && length > 0 && isalpha(*(ptr +1)))) {
                if (length < MAX_WORD_LENGTH - 1) {
                    temp[length++] = *ptr;
                }
            } 
            else {
                if (length > 0) {
                    temp[length] = '\0';
                    add_word(temp);
                    length = 0;
                }
            }
            ptr++;
        }
    }

    if (length > 0) {
        temp[length] = '\0';
        add_word(temp);
    }

    close(file);
}

// Will traverse a directory looking at each file and ignoring hidden ones
void process_directory(const char *dirname) {
    DIR *directory = opendir(dirname);
    if (!directory) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char path[MAX_PATH];
        snprintf(path, sizeof(path), "&s/%s", dirname, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == -1 ) {
            perror("stat");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)) {
            process_directory(path); 
        }
        else if (S_ISDIR(statbuf.st_mode) && strstr(entry->d_name, ".txt")) {
            process_file(path);
        }
    }

    closedir(directory);
}

// Will read to see if a directory is a file or subdirectory
int main (int argc, char *argv[]) { 

    if(argc < 2) {
        fprint(stderr, "Usage: %s <file_or_directory>...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++ ) {
        struct stat statbuf;

        if (stat(argv[i], &statbuf) != 0) {
            perror("Stat");
            continue;  
        }

        if (S_ISDIR(statbuf.st_mode)) {
            process_directory(argv[i]);
        } 
        else if (S_ISDIR(statbuf.st_mode)) {
            process_file(argv[i]);
        }
    }

    print_words();
    return 0;
}