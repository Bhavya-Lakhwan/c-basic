/*
 * ============================================================
 *  STUDENT RECORD MANAGEMENT SYSTEM
 *  Level   : Basic
 *  Modules : Structures, Arrays, Functions
 *
 *  Year-1  : Works perfectly as-is
 *  
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Flush leftover '\n' from input buffer ──────────────────── */
void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ── Constants ─────────────────────────────────────────────── */
#define MAX_STUDENTS 100
#define NAME_LEN     50
#define FILE_NAME    "students.dat"

/* ── Data Structure ─────────────────────────────────────────── */
typedef struct {
    int    id;
    char   name[NAME_LEN];
    int    age;
    float  marks[5];
    float  percentage;
    char   grade;
} Student;

/* ── Global Storage ─────────────────────────────────────────── */
Student students[MAX_STUDENTS];
int     count = 0;

/* ── Helper: Compute percentage & grade ────────────────────── */
void compute_result(Student *s) {
    float total = 0;
    for (int i = 0; i < 5; i++) total += s->marks[i];
    s->percentage = total / 5.0f;

    if      (s->percentage >= 90) s->grade = 'A';
    else if (s->percentage >= 75) s->grade = 'B';
    else if (s->percentage >= 60) s->grade = 'C';
    else if (s->percentage >= 50) s->grade = 'D';
    else                          s->grade = 'F';
}

/* ── 1. Add Student ─────────────────────────────────────────── */
void add_student() {
    if (count >= MAX_STUDENTS) {
        printf("Storage full!\n"); return;
    }
    Student s;
    printf("\n--- Add New Student ---\n");
    printf("Enter ID     : ");
    scanf("%d", &s.id); flush_input();

    printf("Enter Name   : ");
    fgets(s.name, NAME_LEN, stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter Age    : ");
    scanf("%d", &s.age); flush_input();

    printf("Enter marks for 5 subjects:\n");
    for (int i = 0; i < 5; i++) {
        printf("  Subject %d : ", i + 1);
        scanf("%f", &s.marks[i]);
    }
    flush_input();
    compute_result(&s);
    students[count++] = s;
    printf("Student added successfully!\n");
}

/* ── 2. Display All Students ────────────────────────────────── */
void display_all() {
    if (count == 0) { printf("No records found.\n"); return; }
    printf("\n%-5s %-20s %-5s %-10s %-6s\n",
           "ID", "Name", "Age", "Percent", "Grade");
    printf("------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-5d %-10.2f %-6c\n",
               students[i].id, students[i].name,
               students[i].age, students[i].percentage,
               students[i].grade);
    }
}

/* ── 3. Search by ID ────────────────────────────────────────── */
void search_student() {
    int id;
    printf("Enter Student ID to search: ");
    scanf("%d", &id); flush_input();
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            Student *s = &students[i];
            printf("\n--- Student Found ---\n");
            printf("ID         : %d\n",   s->id);
            printf("Name       : %s\n",   s->name);
            printf("Age        : %d\n",   s->age);
            printf("Percentage : %.2f\n", s->percentage);
            printf("Grade      : %c\n",   s->grade);
            return;
        }
    }
    printf("Student with ID %d not found.\n", id);
}

/* ── 4. Update Student ──────────────────────────────────────── */
void update_student() {
    int id;
    printf("Enter Student ID to update: ");
    scanf("%d", &id); flush_input();
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            Student *s = &students[i];
            printf("Enter new Name   : ");
            fgets(s->name, NAME_LEN, stdin);
            s->name[strcspn(s->name, "\n")] = 0;

            printf("Enter new Age    : ");
            scanf("%d", &s->age); flush_input();

            printf("Enter new marks for 5 subjects:\n");
            for (int j = 0; j < 5; j++) {
                printf("  Subject %d : ", j + 1);
                scanf("%f", &s->marks[j]);
            }
            flush_input();
            compute_result(s);
            printf("Record updated successfully!\n");
            return;
        }
    }
    printf("Student with ID %d not found.\n", id);
}

/* ── 5. Delete Student ──────────────────────────────────────── */
void delete_student() {
    int id;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id); flush_input();
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < count - 1; j++)
                students[j] = students[j + 1];
            count--;
            printf("Student deleted successfully!\n");
            return;
        }
    }
    printf("Student with ID %d not found.\n", id);
}

/* ── 6. Save to File ────────────────────────────────────────── */
void save_to_file() {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (!fp) { perror("Cannot open file"); return; }
    fwrite(&count,   sizeof(int),     1,     fp);
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);
    printf("Data saved to %s\n", FILE_NAME);
}

/* ── 7. Load from File ──────────────────────────────────────── */
void load_from_file() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("No saved data found. Starting fresh.\n"); return; }
    fread(&count,   sizeof(int),     1,     fp);
    fread(students, sizeof(Student), count, fp);
    fclose(fp);
    printf("Loaded %d record(s) from %s\n", count, FILE_NAME);
}

/* ── Main Menu ──────────────────────────────────────────────── */
int main() {
    load_from_file();
    int choice;
    do {
        printf("\n========= Student Record Management =========\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Save & Exit\n");
        printf("=============================================\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; }
        flush_input();

        switch (choice) {
            case 1: add_student();    break;
            case 2: display_all();    break;
            case 3: search_student(); break;
            case 4: update_student(); break;
            case 5: delete_student(); break;
            case 6: save_to_file();   printf("Goodbye!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);
    return 0;
}