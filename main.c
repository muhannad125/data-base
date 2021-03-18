#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STUDENTS_ENROLLMENT "studentsEnrollment.txt"
#define COURSES "courses.txt"
#define INSTRUCTORS "instructors.txt"
#define STUDENTS "students.txt"
#define TEMP "temp.txt"
#define LINE_SIZE 250
#define COLUMN_SIZE 40

typedef struct  {
  char id[20];
  char name[20];
  char surname[20];
  int courseNumber;
  int credit;
} Student;

typedef struct  {
  char id[20];
  char name[20];
  char surname[20];
  char title[20];
} Instructor;

typedef struct  {
  char id[20];
  char name[40];
  char instructorId[20];
  int credit;
  int capacity;
} Course;

typedef struct  {
  int id;
  char name[40];
  char date[20];
  char courseId[20];
  char status[20];
} StudentsEnrollment;

void adminUser();
void instructorUser();
void studentUser();
void courseOperations();
void instructorOperations();
void deleteDataFromFile();
void list();
void studentOperations();
void addData();
void listInstructorCourses();
void listStudentCourses();
void enrollCourse();
void unenrollCourse();
int checkID();
int checkFile();


int main(int argc, char const *argv[]) {
    char option;
    int count = 1;
    do{
      printf( "1. Admin\n2. Instructor\n3. Student\n4. Exit\n"
              "Select user type:");
      scanf(" %c", &option);
      system("cls");
      switch (option) {
        case '1':
          adminUser();
          break;
        case '2':
          instructorUser();
          break;
        case '3':
          studentUser();
          break;
        default:
          count = 0;
      }
    }while (count);
  return 0;
}

void adminUser() {
  char option;
  int count = 1;
  do{
      printf("## ADMIN ##\n" );
      printf( "1. Instructor operations\n"
              "2. Student operations\n"
              "3. Course operations\n"
              "4. Back\n"
              "Select operation:");
      scanf(" %c", &option);
      system("cls");
      switch (option) {
        case '1':
          instructorOperations();
          break;
        case '2':
          studentOperations();
          break;
        case '3':
          courseOperations();
          break;
        default:
          count = 0;
      }
  }while (count);
}

void courseOperations() {
  char option, *id, *data;
  int count = 1;
  Course course;
  id = (char*) malloc(LINE_SIZE * sizeof(char));
  data = (char*) malloc(LINE_SIZE * sizeof(char));

  do{
    printf("## COURSE OPERATIONS ##\n" );
    printf( "1. Add new course\n"
            "2. Delete a course\n"
            "3. Update a course details\n"
            "4. List all courses\n"
            "5. Back\n"
            "Select operation:");
    scanf(" %c", &option);
    system("cls");

    switch (option) {
      case '1':
        printf("Enter the details of the course you want to add\n"
                "Code:" );
        scanf(" %s", course.id);
        if (checkID(course.id, COURSES, 0)) {
          printf("This course is already added\n");
        }else{
          printf("Name:");
          scanf("%s", course.name);
          do{
            printf("Instructor ID:");
            scanf("%s", course.instructorId);
            if (!checkID(course.instructorId, INSTRUCTORS, 0)) {
              printf("Not valid instructor\n" );
            }
          }while(!checkID(course.instructorId, INSTRUCTORS, 0));
          printf("Credit:");
          scanf("%d", &course.credit);
          printf("Capacity:");
          scanf("%d", &course.capacity);
          sprintf(data, "%s,%s,%s,%d,%d", course.id, course.name, course.instructorId, course.credit, course.capacity);
          addData(data, COURSES);
        }
        break;
      case '2':
        printf("Enter the code of the course you want to remove\n"
              "Code:" );
        scanf(" %s", course.id);
        if (checkID(course.id, COURSES, 0)) {
          deleteDataFromFile(course.id, COURSES);
          printf("The course has been deleted successfully.\n" );
        }else{
          printf("Non valid course code.\n");
        }

        break;
      case '3':

        printf("Enter the code of the course you want to update\n"
              "Code:" );
        scanf(" %s", course.id);
        printf("Enter new name:");
        scanf("%s", course.name);
        do{
          printf("Instructor ID:");
          scanf("%s", course.instructorId);
          if (!checkID(course.instructorId, INSTRUCTORS, 0)) {
            printf("Not valid instructor\n" );
          }
        }while(!checkID(course.instructorId, INSTRUCTORS, 0));
        printf("Enter new credit:");
        scanf("%d", &course.credit);
        printf("Enter new capacity:");
        scanf("%d", &course.capacity);
        sprintf(data, "%s,%s,%s,%d,%d", course.id, course.name, course.instructorId, course.credit, course.capacity);
        deleteDataFromFile(course.id, COURSES);
        addData(data, COURSES);
        break;
      case '4':
        printf("CODE - NAME - INSTRUCTOR - CREDIT - CAPACITY \n" );
        list(COURSES);
        break;
      default:
        count = 0;
    }
  }while(count);

  free(id);
  free(data);
}

void addData(char* data, char* file) {
  FILE *fptr;
  fptr = fopen(file, "a");

  if ( fptr == NULL ){
    printf("%s faild to open", file);
  }else{
    fprintf(fptr, "%s\n", data );
    printf("%s\n", data);
    printf("Data successfully written in file %s\n", file);
  }
  fclose(fptr);
}

void deleteDataFromFile(char* id,char* file){
  FILE *fptr, *tmpf;
  char *line, *firstWord;
  int isTrue = 0;
  long int deletePosition, position;



  fptr = fopen(file, "r");
  if( fptr == NULL ){
    printf("File not found or unable to open!!\n");
    return ;
  }
  tmpf = fopen("temp.txt", "w") ;
  if (tmpf == NULL){
    printf("Unable to open a temporary file to write!!\n");
    return;
  }
  line = (char*) malloc(LINE_SIZE * sizeof(char));

  deletePosition = -1;
  position = ftell(fptr);
  while (fgets(line, LINE_SIZE,fptr)) {
    firstWord = strtok(line, "\n,");
    if(strcmp(firstWord, id) ){
      position = ftell(fptr);
    }else{
      deletePosition = position;
      isTrue = 1;
    }
  }

  fseek(fptr,0,SEEK_SET);
  if(deletePosition != -1){
    while (ftell(fptr) < deletePosition) {
        fgets(line, LINE_SIZE, fptr);
        fprintf(tmpf, "%s", line);
      }
    if(isTrue){fgets(line, LINE_SIZE, fptr);}
    while (fgets(line, LINE_SIZE,fptr)){
        fprintf(tmpf, "%s", line);
    }
    fclose(fptr);
    fclose(tmpf);
    remove(file);
    rename(TEMP, file);
  }else{
    fclose(fptr);
    fclose(tmpf);
    printf("Error: Cannot remove non-existing course!\n");
  }
  free(line);
}

void list(char* file) {
  FILE *fptr;
  char *line;
  fptr = fopen(file, "r");
  if( fptr == NULL ){
    printf("File not found or unable to open courses.txt file!!\n");
    return ;
  }
  line = (char*) malloc(LINE_SIZE * sizeof(char));
  while (fgets(line, LINE_SIZE, fptr)) {
    printf("%s", line);
  }
  fclose(fptr);
  free(line);
}

void instructorOperations() {
  char option, *data;
  Instructor instructor;
  int count = 0;
  data = (char*) malloc(LINE_SIZE * sizeof(char));
  do{
    printf("## INSTRUCTOR OPERATIONS ##\n" );
    printf(   "1. Register new instructor\n"
              "2. Delete an instructor\n"
              "3. Update an instructor informations\n"
              "4. List all instructors\n"
              "5. Back\n"
              "Select operation:");
    scanf(" %c", &option);
    system("cls");

    switch (option) {
      case '1':
      printf("Enter the following informations to add an instructor\n");
      printf("Instructor ID: ");
      scanf("%s", instructor.id);
      if (checkID(instructor.id, INSTRUCTORS, 0)) {
        printf("This instructor is already registered\n");
      }else{
        printf("Name: ");
        scanf("%s", instructor.name);
        printf("Surname: ");
        scanf("%s", instructor.surname);
        printf("Title: ");
        scanf("%s", instructor.title);
        sprintf(data, "%s,%s,%s,%s", instructor.id, instructor.name, instructor.surname, instructor.title);
        addData(data, INSTRUCTORS);
      }
      break;
      case '2':

      printf("Enter the id of the instructor you want to remove\n"
      "id:" );
      scanf(" %s", instructor.id);
      if (checkID(instructor.id, INSTRUCTORS, 0)) {
        deleteDataFromFile(instructor.id, INSTRUCTORS);
        printf("The instructor has been deleted successfully.\n");
      }else{
        printf("Non valid instructor id.\n" );
      }

      break;
      case '3':
      printf("Enter the following informations to add an instructor\n");
      printf("Instructor ID: ");
      scanf("%s", instructor.id);
      printf("Name: ");
      scanf("%s", instructor.name);
      printf("Surname: ");
      scanf("%s", instructor.surname);
      printf("Title: ");
      scanf("%s", instructor.title);
      sprintf(data, "%s,%s,%s,%s", instructor.id, instructor.name, instructor.surname, instructor.title);
      deleteDataFromFile(instructor.id, INSTRUCTORS);
      addData(data, INSTRUCTORS);
      break;
      case '4':
      printf("ID - NAME - SURENAME - TITLE \n" );
      list(INSTRUCTORS);
      break;
      default:
      count = 0;
    }
  }while(count);
  free(data);
}

void studentOperations() {
  char option, *data;
  int count = 1;
  Student student;
  data = (char*) malloc(LINE_SIZE * sizeof(char));
  do{
    printf("## STUDENT OPERATIONS ##\n" );
    printf(   "1. Register new student\n"
            "2. Delete a student\n"
            "3. Update a student informations\n"
            "4. List all students\n"
            "5. Back\n"
            "Select operation:");
    scanf(" %c", &option);
    system("cls");

    switch (option) {
      case '1':
        printf("Enter the following informations to add an student\n");
        printf("Student ID: ");
        scanf("%s", student.id);
        if (checkID(student.id, STUDENTS, 0)) {
          printf("This student is already registered\n");
        }else{
          printf("Name: ");
          scanf("%s", student.name);
          printf("Surname: ");
          scanf("%s", student.surname);
          student.courseNumber = 0;
          student.credit = 0;
          sprintf(data, "%s,%s,%s,%d,%d", student.id, student.name, student.surname, student.courseNumber, student.credit);
          addData(data, STUDENTS);
        }
        break;
      case '2':
        printf("Enter the id of the student you want to remove\n"
               "id:" );
        scanf(" %s", student.id);
        deleteDataFromFile(student.id, STUDENTS);
        break;
      case '3':
          printf("Enter the following informations to add an instructor\n");
          printf("Student ID: ");
          scanf("%s", student.id);
          printf("Name: ");
          scanf("%s", student.name);
          printf("Surname: ");
          scanf("%s", student.surname);
          sprintf(data, "%s,%s,%s", student.id, student.name, student.surname);
          deleteDataFromFile(student.id, STUDENTS);
          addData(data, STUDENTS);
        break;
      case '4':
        printf("ID - NAME - SURENAME\n" );
        list(STUDENTS);
        break;
      default:
        count = 0;
    }
  }while(count);
  free(data);
}

void instructorUser() {
    char option, *data;
    int count = 1;
    Instructor instructor;
    Course course;
    data = (char*) malloc(LINE_SIZE * sizeof(char));
    printf("Enter your ID:\n"
            "ID: ");
    scanf(" %s", instructor.id);
    do{
      if (checkID(instructor.id, INSTRUCTORS, 0)) {
        printf("Welcome \n");
        printf(   "1. Update a course\n"
                  "2. List my courses\n"
                  "3. Back\n"
                  "Select operation: ");
        scanf(" %c", &option);
        system("cls");
      }else{
        option = '3';
        printf("Your ID is not correct if you are an instructor ask the admin to register you\n");
      }
        switch (option) {
          case '1':
            printf("Enter the code of the course you want to update\n"
                  "Code:" );
            scanf(" %s", course.id);
            if (checkFile(course.id, instructor.id, COURSES, 2)) {
              printf("Enter new name:");
              scanf("%s", course.name);
              strcpy(course.instructorId, instructor.id);
              printf("Enter new credit:");
              scanf("%d", &course.credit);
              printf("Enter new capacity:");
              scanf("%d", &course.capacity);
              sprintf(data, "%s,%s,%s,%d,%d", course.id, course.name, course.instructorId, course.credit, course.capacity);
              deleteDataFromFile(course.id, COURSES);
              addData(data, COURSES);
            }else{
              printf("you can't update a course you don't give!\n" );
            }
            break;
          case '2':
            printf("COURSE ID, NAME, INSTRUCTOR ID, CREDIT, CAPACITY\n");
            listInstructorCourses(instructor.id,COURSES, 2);
            printf("\n");
            break;
          default:
            count = 0;
        }
    }while(count);
    free(data);
}

int checkFile(char* courseId, char* id, char* file, int controlColumn, int chekEnrollment){
  FILE *fptr;
  char *column, *line;
  int i = 0, isTrue = 0;
  fptr = fopen(file, "r");
  if( fptr == NULL ){
    printf("File not found or unable to open %s file!!\n", file);
    return 0;
  }
  line = (char*) malloc(LINE_SIZE * sizeof(char));

  while (fgets(line, LINE_SIZE,fptr)) {
    i = 0;
    column = strtok(line, "\n,");
    if(!strcmp(column, courseId)){
      while (column) {
        i++;
        column = strtok(NULL, "\n,");
        if (i == controlColumn) {
          if(!strcmp(column, id)){
            isTrue = 1;
          }
        }
        if (chekEnrollment == 1 && isTrue == 1 && i == 2) {
          if (!strcmp(column, "Enrolled")) {
            isTrue = 1;
          }else{
            isTrue = 0;
          }
        }
      }
      if (isTrue == 1) {
        fclose(fptr);
        free(line);
        return 1;
      }
    }
  }
  fclose(fptr);
  free(line);
  return 0;
}

int checkID(char* id, char* file, int controlColumn){
  FILE *fptr;
  char *column, *line;
  int i = 0;
  fptr = fopen(file, "r");
  if( fptr == NULL ){
    printf("File not found or unable to open courses.txt file!!\n");
    return 0;
  }
  line = (char*) malloc(LINE_SIZE * sizeof(char));

  while (fgets(line, LINE_SIZE,fptr)) {
    i = 0;
    column = strtok(line, "\n,");
    if(controlColumn == 0 && !strcmp(column, id)){
      fclose(fptr);
      free(line);
      return 1;
    }
    while (column) {
      i++;
      column = strtok(NULL, "\n,");
      if (i == controlColumn) {
        if(!strcmp(column, id)){
          fclose(fptr);
          free(line);
          return 1;
        }
      }
    }
  }
  fclose(fptr);
  free(line);
  return 0;
}

void listInstructorCourses(char *id, char *file, int controlColumn){
  FILE *fptr;
  char *line, *column, *completeLine;
  int i = 0;
  fptr = fopen(file, "r");
  if( fptr == NULL ){
    printf("File not found or unable to open courses.txt file!!\n");
    return ;
  }
  line = (char*) malloc(LINE_SIZE * sizeof(char));
  completeLine =(char*) malloc(LINE_SIZE * sizeof(char));
  while (fgets(line, LINE_SIZE, fptr)) {
    i = 0;
    strcpy( completeLine, line);
    column = strtok(line, "\n,");
    if(controlColumn == 0 && !strcmp(column, id)){
      printf("%s",completeLine );
    }
    while (column) {
      i++;
      column = strtok(NULL, "\n,");
      if (i == controlColumn) {
        if(!strcmp(column, id)){
          printf("%s",completeLine );
        }
      }
    }
  }

  fclose(fptr);
  free(line);
  free(completeLine);
}

void studentUser() {
    char option;
    Student student;
    Course course;
    int count = 1;
    printf("Enter your ID:\n"
            "ID: ");
    scanf(" %s", student.id);
  do{
    if (checkID(student.id, STUDENTS, 0)){
    printf(   "1. enroll a course\n"
              "2. unenroll a course\n"
              "3. List my courses\n"
              "4. Back\n"
              "Select operation: " );
    scanf(" %c", &option);
    system("cls");
  }else{
    option = '4';
    printf("Your ID is not corect. if you are a student ask the admin to register you\n");
    return;
  }
    switch (option) {
      case '1':
        printf("Enter the code of the course you want to enroll\n"
                "Code:");
        scanf(" %s",course.id );
        enrollCourse(course.id, student.id);
        break;
      case '2':
        printf("Enter the code of the course you want to unenroll\n"
               "Code:");
        scanf(" %s",course.id );
        unenrollCourse(course.id, student.id);
        break;
      case '3':
        printf("COURSE CODE, STUDENT ID, STATUS, DATE\n" );
        listStudentCourses(student.id);
        printf("\n" );
        break;
      default:
        count = 0;
        break;
    }
  }while (count);
}

void enrollCourse(char* courseId, char* studentId){
  FILE *fptr, *fcourses;
  time_t now;
  StudentsEnrollment studentsEnrollment;
  char *line, *status, *column, *tmpcolumn;
  long int size;
  int count,isTrue, i;
  fptr = fopen(STUDENTS_ENROLLMENT, "a+");
  if( fptr == NULL ){
    printf("File not found or unable to open %s file!!\n", STUDENTS_ENROLLMENT);
    return ;
  }
  fcourses = fopen(COURSES, "r");
  if( fcourses == NULL ){
    printf("File not found or unable to open %s file!!\n", COURSES);
    return ;
  }
  line = (char*) malloc(LINE_SIZE * sizeof(char));
  status = (char*) malloc(COLUMN_SIZE * sizeof(char));
  tmpcolumn = (char*) malloc(COLUMN_SIZE * sizeof(char));
  fseek(fptr, 0, SEEK_END);
  size = ftell(fptr);
  if (size == 0){
    count = 0;
  }else{
    fseek(fptr,0, SEEK_SET);
    while (fgets(line, LINE_SIZE, fptr)) {
      i = 0;
      column = strtok(line, "\n,");
      while (column) {
        i++;
        column = strtok(NULL, "\n,");
        if (i == 3) {
          tmpcolumn = column;
        }
      }
    }
    int tmpcount = atoi(tmpcolumn);
    count = tmpcount + 1;
  }
  isTrue = 0;

  while (fgets(line, LINE_SIZE, fcourses)) {
    column = strtok(line, "\n,");
    if (!strcmp(column,courseId)) {
      isTrue = 1;
    }
  }
  if (!isTrue) {
    printf("This course is not existing\n" );
    fclose(fcourses);
    fclose(fptr);
    free(line);
    free(status);
    free(tmpcolumn);
    return ;
  }
  time(&now);
  struct tm *local = localtime(&now);
  sprintf(studentsEnrollment.date, "%02d/%02d/%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
  status = "Enrolled";
  if (checkFile(courseId, studentId, STUDENTS_ENROLLMENT, 1, 0) == 1 && checkFile(courseId, studentId, STUDENTS_ENROLLMENT, 1, 1) == 0) {
    sprintf(line, "%s,%s,%s,%d,%s", courseId, studentId, status, count, studentsEnrollment.date);
    fclose(fptr);
    deleteDataFromFile(courseId, STUDENTS_ENROLLMENT);
    addData(line, STUDENTS_ENROLLMENT);
  }else if (checkFile(courseId, studentId, STUDENTS_ENROLLMENT, 1, 1)) {
    printf("You are already enrolled in this course!\n" );
    fclose(fptr);
  }else{
    printf("Enrolled successfully.\n" );
    fprintf(fptr, "%s,%s,%s,%d,%s\n", courseId, studentId, status, count, studentsEnrollment.date);
    fclose(fptr);
  }
  fclose(fcourses);
  free(line);
  free(status);   // FIXME freed even if not allocated
  free(tmpcolumn);
}

void unenrollCourse(char* courseId, char* studentId) {
  FILE *fptr, *fcourses;
  long int size;
  time_t now;
  StudentsEnrollment studentsEnrollment;
  char *line, *column, *tmpcolumn, *status;
  int i, count;
  fptr = fopen(STUDENTS_ENROLLMENT, "a+");
  if( fptr == NULL ){
    printf("File not found or unable to open %s file!!\n", STUDENTS_ENROLLMENT);
    return ;
  }
  fcourses = fopen(COURSES, "r");
  if( fcourses == NULL ){
    printf("File not found or unable to open %s file!!\n", COURSES);
    return ;
  }
  line = (char*) malloc(LINE_SIZE * sizeof(char));
  tmpcolumn = (char*) malloc(COLUMN_SIZE * sizeof(char));
  status = (char*) malloc(COLUMN_SIZE * sizeof(char));
  fseek(fptr, 0, SEEK_END);
  size = ftell(fptr);
  if (size == 0){
    printf("You are not enrolled in a course with %s id\n",courseId );
    fclose(fptr);
    return;
  }else{
    fseek(fptr,0, SEEK_SET);
    while (fgets(line, LINE_SIZE, fptr)) {
      i = 0;
      column = strtok(line, "\n,");
      while (column) {
        i++;
        column = strtok(NULL, "\n,");
        if (i == 3) {
          tmpcolumn = column;
        }
      }
    }
    int tmpcount = atoi(tmpcolumn);
    count = tmpcount ;

  }

  time(&now);
  struct tm *local = localtime(&now);
  sprintf(studentsEnrollment.date, "%02d/%02d/%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);


  status = "Unenrolled";
  if (checkFile(courseId, studentId, STUDENTS_ENROLLMENT, 1, 1)) {
    sprintf(line, "%s,%s,%s,%d,%s", courseId, studentId, status, count, studentsEnrollment.date);
    fclose(fptr);
    deleteDataFromFile(courseId, STUDENTS_ENROLLMENT);
    addData(line, STUDENTS_ENROLLMENT);
  }else{
    printf("You are not enrolled in a course with %s id\n",courseId );
    fclose(fptr);
  }
  free(line);
  fclose(fcourses);
}

void listStudentCourses(char *studentId){
FILE *fptr;
char *line, *completeLine, *column;
int isTrue, i;

fptr = fopen(STUDENTS_ENROLLMENT, "r");
if( fptr == NULL ){
  printf("File not found or unable to open %s file!!\n", STUDENTS_ENROLLMENT);
  return ;
}

line = (char*) malloc(LINE_SIZE * sizeof(char));
completeLine = (char*) malloc(LINE_SIZE * sizeof(char));

while (fgets(line, LINE_SIZE,fptr)) {
  i = 0;
  strcpy(completeLine, line);
  column = strtok(line, "\n,");
  while (column) {
    i++;
    column = strtok(NULL, "\n,");
    if (i == 1) {
      if(!strcmp(column, studentId)){
        isTrue = 1;
      }
    }
    if (isTrue == 1 && i == 2) {
      if (!strcmp(column, "Enrolled")) {
        isTrue = 1;
      }else{
        isTrue = 0;
      }
    }
  }
  if (isTrue == 1) {
    printf("%s",completeLine );
  }
}
fclose(fptr);
free(completeLine);
free(line);
}
