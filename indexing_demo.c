#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure for the main data record */
typedef struct {
    int id;
    char name[50];
    float gpa;
} Student;

/* Structure for the index record */
typedef struct {
    int id;         /* Key: Student ID */
    long offset;    /* Value: Byte offset in the data file */
} Index;

const char *DATA_FILE = "students.dat";
const char *INDEX_FILE = "students.idx";

/* Helper to clear input buffer */
void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* 
 * add_student:
 * 1. Appends a new Student record to the data file.
 * 2. Records the starting position (offset) of that record.
 * 3. Appends a new Index entry (ID, offset) to the index file.
 */
void add_student() {
    FILE *f_data = fopen(DATA_FILE, "ab"); /* Append binary */
    FILE *f_index = fopen(INDEX_FILE, "ab");
    
    if (!f_data || !f_index) {
        perror("Error opening files");
        if (f_data) fclose(f_data);
        if (f_index) fclose(f_index);
        return;
    }

    Student s;
    Index idx;

    printf("\nEnter Student ID: ");
    scanf("%d", &s.id);
    clear_input();
    
    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0; /* Remove newline */
    
    printf("Enter GPA: ");
    scanf("%f", &s.gpa);
    clear_input();

    /* Get current offset in data file before writing */
    long current_offset = ftell(f_data);

    /* Write data record */
    fwrite(&s, sizeof(Student), 1, f_data);

    /* Prepare and write index record */
    idx.id = s.id;
    idx.offset = current_offset;
    fwrite(&idx, sizeof(Index), 1, f_index);

    printf("Student added successfully.\n");

    fclose(f_data);
    fclose(f_index);
}

/*
 * search_student:
 * 1. Searches the Index file for the matching ID.
 * 2. If found, retrieves the offset.
 * 3. Opens the Data file, seeks to that offset, and reads the record.
 * 
 * Note: This avoids reading the full data file.
 */
void search_student() {
    int search_id;
    printf("\nEnter Student ID to search: ");
    scanf("%d", &search_id);
    clear_input();

    FILE *f_index = fopen(INDEX_FILE, "rb");
    if (!f_index) {
        printf("No index file found (no records added yet).\n");
        return;
    }

    Index idx;
    int found = 0;
    long offset = -1;

    /* Linear search on index file (for simplicity) */
    /* In a real DB, this would be a binary search or B-Tree */
    while (fread(&idx, sizeof(Index), 1, f_index)) {
        if (idx.id == search_id) {
            offset = idx.offset;
            found = 1;
            break;
        }
    }
    fclose(f_index);

    if (!found) {
        printf("Student with ID %d not found.\n", search_id);
        return;
    }

    /* Found in index, now fetch from data file */
    FILE *f_data = fopen(DATA_FILE, "rb");
    if (!f_data) {
        perror("Error opening data file");
        return;
    }

    /* Jump directly to the record */
    if (fseek(f_data, offset, SEEK_SET) != 0) {
        perror("fseek failed");
        fclose(f_data);
        return;
    }

    Student s;
    if (fread(&s, sizeof(Student), 1, f_data) == 1) {
        printf("\n--- Record Found ---\n");
        printf("ID: %d\n", s.id);
        printf("Name: %s\n", s.name);
        printf("GPA: %.2f\n", s.gpa);
        printf("--------------------\n");
    } else {
        printf("Error reading record from data file.\n");
    }

    fclose(f_data);
}

/*
 * display_all:
 * Reads and prints all records from the data file sequentially.
 */
void display_all() {
    FILE *f_data = fopen(DATA_FILE, "rb");
    if (!f_data) {
        printf("No data file found.\n");
        return;
    }

    Student s;
    printf("\n--- All Students ---\n");
    while (fread(&s, sizeof(Student), 1, f_data)) {
        printf("ID: %d | Name: %-20s | GPA: %.2f\n", s.id, s.name, s.gpa);
    }
    printf("--------------------\n");

    fclose(f_data);
}

int main() {
    int choice;
    while (1) {
        printf("\n=== File Indexing Demo ===\n");
        printf("1. Add Student\n");
        printf("2. Search Student (using Index)\n");
        printf("3. Display All Students\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input();
            continue;
        }
        clear_input();

        switch (choice) {
            case 1: add_student(); break;
            case 2: search_student(); break;
            case 3: display_all(); break;
            case 4: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
