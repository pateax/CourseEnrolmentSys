// Axita Patel 

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 30
#define fieldLength 200


#define diskFile "diskFile.dat"
#define courseFile "course.txt"

struct db_type
{
   char name[fieldLength];
   int age;
   char course1[fieldLength];
   char course2[fieldLength];
   char status[fieldLength];
 };

struct courseInfo
{ 
  char code [20]; // e.g., EECS2030
  char title [fieldLength];
  char  date [20];
  char time_start [20];
  char time_end [20];
  char  location [20]; 
};
 
 
struct courseInfo courseArr[SIZE]; // global variable  array of struc
     

char prompt_menu(void);
void init_list(struct db_type * pArr[]); 
void clearDB(struct db_type * pArr[]);
void init_courseArr(void);

void writeDisk(struct db_type * pArr[]); 
void emptyDisk(void); 
void loadDisk(struct db_type * pArr[]); 

void displayCourses(void);
void enterNew(struct db_type * pArr[SIZE]);
int findCourse(char course[]);
int findCourseIndex(char course[]);
int checkConflict(int indexA, int indexB);
void displayDB(struct db_type * pArr[]);
void removeRecord (struct db_type * pArr[]);
void swap(struct db_type * pArr[]);
void sort(struct db_type * pArr[]);


int main(int argc, char *argv[])
{
    
    struct db_type * db_pArr[SIZE];  // main db storage

    init_list(db_pArr);  // set to NULL
    
    init_courseArr();  // load course from diskfile
    
    char choice;
    for(; ;){
      choice = prompt_menu();
      switch (choice)
      {
         case 'n': enterNew(db_pArr); break;
         case 'd': displayDB(db_pArr); break;
         case 'w': writeDisk(db_pArr);  break;    
         case 'l': loadDisk(db_pArr); break;
         case 's': sort(db_pArr); break;
	   
         case 'c': clearDB(db_pArr); break;  
         case 'e': emptyDisk();break;

         case 'v': displayCourses();break;
         case 'p': swap(db_pArr); break;
         case 'r': removeRecord(db_pArr);break;
         
         case 'q': exit(1); // terminate the whole program
       }
	
     }
     return 0;
}

void init_list(struct db_type * pArr[]){
  int t;
  for (t=0; t<SIZE; t++)
  { 
     pArr[t]= NULL;
  }
}

void clearDB(struct db_type * pArr[]){
   char c3[3];
   printf("are you sure to clear db? (y) or (n)? ");
 
   fgets(c3,3,stdin);
   
   if(! strcmp(c3, "y\n"))  
      init_list(pArr);
}


char prompt_menu(void){
  char s[80];
  while(1){
    printf("\n-----------------------------------------------------------------\n");
    printf("|    %-20s","(N)ew record");
    printf("%-20s","(R)emove record");
    printf("Swa(p) records\t|\n");
    printf("|    %-20s","(S)ort database");
    printf("%-20s","(C)lear database");
    printf("(D)isplay db\t|\n");
  
    printf("|    %-20s","(L)oad disk");
    printf("%-20s","(W)rite disk");
    printf("(E)mpty disk\t|\n");
  
    printf("|    %-20s", "(V)iew courses");
    printf("%-20s","(Q)uit");
    printf("*Case Insensitive*\t|\n");
    printf("-----------------------------------------------------------------\n");
    printf("choose one: ");
  
    fgets(s,50, stdin); 
   
    if (strlen(s) == 2 && strchr("edlsuqrcwnvpr", tolower(*s))) 
       return tolower(*s); // s[0], return the first character of s  
    //else
    printf("not a valid input!\n");
	 
 }
}

/* display all or specified course */
void displayCourses(void){   
  
      char in[20];
      printf("\ncourse code (or 'a')?: ");
      fgets(in, 20, stdin);

      //print all courses
      if(strcmp(in, "a\n") == 0) {
        int i = 0;
        while(strcmp(courseArr[i].code, "EECS0000") != 0) {
         printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[i].code, courseArr[i].title,
         courseArr[i].date, courseArr[i].time_start, courseArr[i].time_end, courseArr[i].location);
         i++;
        }
      }
    
     else {
       in[strlen(in) - 1] = '\0';

       int a = findCourse(in);
       if(a == 1) {
         int i = findCourseIndex(in);
         printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[i].code, courseArr[i].title,
         courseArr[i].date, courseArr[i].time_start, courseArr[i].time_end, courseArr[i].location);
       }
       else {
         printf("error! course does not exist\n");
       }
     }


}


/* input items into the list */
void enterNew(struct db_type * pArr[SIZE]){  
	
  int num = 0;
  while (pArr[num] != NULL) {
    num++;
  }
  struct db_type * newS = malloc(sizeof(struct db_type));

  char nm[fieldLength];
  char age[3];
  char crA[fieldLength];
  char crB[fieldLength];
  printf("name: ");
  fgets(nm, fieldLength, stdin);
  nm[strlen(nm)-1] = '\0';
  printf("age: ");
  fgets(age, 3, stdin);
  int ag = atoi(age);

  int d = 0;
  printf("course-1: ");
  fgets(crB, fieldLength, stdin);
  crB[strlen(crB) - 1] = '\0';
  d = findCourse(crB);
  
  while(d == 0) {
    
    fgets(crB, fieldLength, stdin);
    crB[strlen(crB) - 1] = '\0';
    d = findCourse(crB);
    if(d==0) {
    printf("course does not exist, enter again: ");
    
    }
    else {
      break;
    }
  }
  int indexB = findCourseIndex(crB);
  
  
  int c = 1;
  printf("course-2: ");
  fgets(crA, fieldLength, stdin);
  crA[strlen(crA) - 1] = '\0';
  c = findCourse(crA);
 
    while(c == 0) {
     printf("course does not exist, enter again: ");
     fgets(crA, fieldLength, stdin);
     crA[strlen(crA) - 1] = '\0';
     c = findCourse(crA);
   }
 
  int indexA = findCourseIndex(crA);
  

  int conf = checkConflict(indexB, indexA);

  
  if(conf == 0) {
    printf("SUCCESSFUL! no time conflict\n");
    
  }
  else {
    printf("ATTENTION! time conflict\n");
  }
  
    //add student record to pArr
    sprintf(newS->name, "%s", nm);
    newS->age = ag;
    sprintf(newS -> course1, "%s\t%-40s%-5s %s-%s   %s\n", courseArr[indexB].code, courseArr[indexB].title,
         courseArr[indexB].date, courseArr[indexB].time_start, courseArr[indexB].time_end, courseArr[indexB].location);
    sprintf(newS -> course2, "%s\t%-40s%-5s %s-%s   %s\n", courseArr[indexA].code, courseArr[indexA].title,
         courseArr[indexA].date, courseArr[indexA].time_start, courseArr[indexA].time_end, courseArr[indexA].location);
    if(conf == 0) {
      sprintf(newS->status, "SUCCESSFUL! no time conflict\n");
    }
    else {
      sprintf(newS->status, "ATTENTION! time conflict\n");
    }
    pArr[num] = newS;
    num++;
   

}

/* helper function to check if user entered a valid course */
int findCourse(char course[]) {
  int i = 0;
  char cr[20];

  //if user enterted course without "EECS", then add it to the string 
  if(course[0] != 'E') {
    strcpy(cr, "EECS");
    strcat(cr, course);
  }
  else {
    strcpy(cr, course);
  }
  while(strcmp(courseArr[i].code, "EECS0000") != 0) {
    if(strcmp(courseArr[i].code, cr) == 0) {
      return 1;
    }
    else {
      i++;
    }
  }
  return 0;
}

/* Finds index of the specified course in courseArr */
int findCourseIndex(char course[]) {
  int i = 0;
  char cr[20];

  if(course[0] != 'E') {
    strcpy(cr, "EECS");
    strcat(cr, course);
  }
  else {
    strcpy(cr, course);
  }
  while(strcmp(courseArr[i].code, "EECS0000") != 0) {
    if(strcmp(courseArr[i].code, cr) == 0) {
      return i;
    }
    else {
      i++;
    }
  }
  return -1;
}

/* Helper function to check if the courses at indexA and indexB have
a time conflict. Return 0 if no time conflict, otherwise return 1. */
int checkConflict(int indexA, int indexB) {
  
  int i, j; 
  int conflt = 0;
  for(i = 0; i < strlen(courseArr[indexA].date); i++) {
    for(j = 0; j < strlen(courseArr[indexB].date); j++) {
      if(courseArr[indexA].date[i] == courseArr[indexB].date[j]) {
        conflt = 1;
      }
    }
  }

  if(conflt == 0) {
    return 0;
  }
  else {
  //for ex. startA is "17:30"
  char startA[20];
  strcpy(startA, courseArr[indexA].time_start);
  //and endA is "19:00"
  char endA[20];
  strcpy(endA, courseArr[indexA].time_end);
  
  //convert to int 1730 and 1900
  
  //START time
  char A[3];
  strncpy(A, startA, 2);
  A[2] = '\0';
  //17
  int a = atoi(A);
  
  char B[3];
  //B[0] -> "3"
  B[0] = startA[3];
  //B[1] -> "0"
  B[1] = startA[4];
  B[2] = '\0';
  //30
  int b = atoi(B);
  //1730 = (17*100) + 30 
  int stA = (100 * a) + b;

  //END time
  char E[3];
  //E -> "19"
  strncpy(E, endA, 2);
  E[2] = '\0';
  //19
  int e = atoi(E);
  
  char F[3];
  //F[0] -> "0"
  F[0] = endA[3];
  //F[1] -> "0"
  F[1] = endA[4];
  F[2] = '\0';
  //0
  int f = atoi(F);
  //1900 = (19*100) + 0 
  int edA = (100 * e) + f;


  char startB[20];
  strcpy(startB, courseArr[indexB].time_start);
  char endB[20];
  strcpy(endB, courseArr[indexB].time_end);
  
  //START time
  char C[3];
  strncpy(C, startB, 2);
  C[2] = '\0';
  int c = atoi(C);
  
  char D[3];
  D[0] = startB[3];
  D[1] = startB[4];
  D[2] = '\0';
  int d = atoi(D);
  int stB = (100 * c) + d;

   //END time
  char G[3];
  strncpy(G, endB, 2);
  G[2] = '\0';
  int g = atoi(G);
  
  char H[3];
  H[0] = endB[3];
  H[1] = endB[4];
  H[2] = '\0';
  int h = atoi(H);
  int edB = (100 * g) + h;

   //return 0 if no conflict          
  if((stA < stB && edA <= stB) || (stA > stB && stA >= edB)) {
    return 0;
  }
  
  else {
    return 1;
  }
  }

}

/* display records */
void displayDB(struct db_type * pArr[]){
  
  printf("===============================\n\n");
  int i = 0;
  while(pArr[i] != NULL) {
    printf("name:    %s\n", pArr[i]->name);
    printf("age:     %d\n", pArr[i]->age);
    printf("course1: %s", pArr[i]->course1);
    printf("course2: %s", pArr[i]->course2);
    printf("remarks: %s\n", pArr[i]->status);
    i++;
  }
  printf("========== %d records ==========\n", i);
 
}


/* remove an existing item */
void removeRecord (struct db_type * pArr[])	
{
  char nm[fieldLength];
  printf("enter full name to remove: ");
  fgets(nm, fieldLength, stdin);
  nm[strlen(nm) - 1] = '\0';

  int i, j = 0;
  while(pArr[i] != NULL) {
    if(strcmp(nm, pArr[i]->name) == 0) {
      j = 1;
      break;
    }
    i++;
  }

  int n = i;
  if(j == 1) {
    while(n + 1 < SIZE && pArr[n+1] != NULL) {
      pArr[n] = pArr[n + 1];
      n++;
    }
    if (n < SIZE) {
      pArr[n] = NULL;
    }
    
    printf("record [%s] removed successfully.\n", nm);
  }
  else {
    printf("record not found\n");
  }

}

/* swap records */
void swap(struct db_type * pArr[]){
   
	int i = 0;
  int j = 1;
  while(pArr[j] != NULL) {
    struct db_type * temp = pArr[i];
    pArr[i] = pArr[j];
    pArr[j] = temp;
    i += 2;
    j += 2;
  }
} 

/* load from course.txt, store into (global) array of courses  */
void init_courseArr(void){ 
 
   FILE *fr; //file pointer
   fr = fopen (courseFile, "r");
   char arr[50];  
   int count = 0;
   while ((fgets(arr,100,fr)) != NULL)
   {  
     strncpy(courseArr[count].code,  arr, strlen(arr)-1); // remove \n  
     courseArr[count].code[strlen(arr)-1] = '\0';  //to be safe
	 
     fgets(arr,100,fr);  
     strncpy(courseArr[count].title ,  arr, strlen(arr)-1);
     courseArr[count].title[strlen(arr)-1] = '\0'; 
     
          
     fgets(arr,100,fr); 
     strncpy(courseArr[count].date,  arr, strlen(arr)-1);
     courseArr[count].date[strlen(arr)-1] = '\0';  
	   
     
     fgets(arr,100,fr);
     arr[strlen(arr)-1] = '\0';
     strcpy(courseArr[count].time_start,  arr);
      
     fgets(arr,100,fr);
     arr[strlen(arr)-1] = '\0';
     strcpy(courseArr[count].time_end,  arr);
      
     fgets(arr,100,fr);
     arr[strlen(arr)-1] = '\0';
     strcpy(courseArr[count].location,  arr); 
	  
     count++;
     fgets(arr,100,fr); // read the empty line
   }

   strcpy(courseArr[count].code, "EECS0000"); // a terminator token structure

   fclose(fr);
	 
 }
    
/************************ DISK IO *****************************************************************/

void writeDisk(struct db_type * pArr[]){
  FILE *fp;
  int i;

  if ( (fp=fopen(diskFile,"ab")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  
  for (i=0; i< SIZE ; i++)
  {
     if ( pArr[i] != NULL)
     {  
       if (fwrite( pArr[i], sizeof(struct db_type), 1, fp) != 1)
       { 
          printf("file write error\n");
       }
     } 
   }
   fclose(fp);

}

void emptyDisk(void){

  FILE *fp;
  
  char c3[3];
  printf("are you sure to empty disk? (y) or (n)? ");
  fgets(c3,3,stdin);
  if(strcmp(c3, "y\n"))  
     return;
 
  //else
  if ( (fp = fopen(diskFile,"w")) == NULL)
  {
     printf("no file to open\n");
     return;
  }
  fclose(fp); // open close, will empty the file
}


void loadDisk(struct db_type * pArr[]){
  FILE *fp;
  int i;
  char c3[3];
  printf("will overwrite current records. are you sure to load disk? (y) or (n)? ");
  fgets(c3,3,stdin);
  if(strcmp(c3, "y\n"))  
     return;

  struct db_type * tmp;  
  
  if ( (fp = fopen(diskFile,"r")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  init_list(pArr);
  for (i=0; i < SIZE ; i++)
  {    
     
     tmp =  (struct db_type *) malloc (sizeof(struct db_type));   
	   
     if (fread( tmp, sizeof(struct db_type), 1, fp) != 1)
     {
       if (feof(fp))
       { fclose(fp); 
         return;
       }
       printf("file read error\n");
     }
     else pArr[i] = tmp;
  }
}



/* sort the records by ages */
void sort(struct db_type * pArr[])
{
  int n = 0;
  while(pArr[n] != NULL) {
    n++;
  }
  int i, j, smallest; 
    for(i = 0; i < n-1; i++) {
        smallest = i;
        for(j = i + 1; j < n; j++) {
            if(pArr[j]->age < pArr[smallest]->age) {
                smallest = j;
            }
            
        }
        struct db_type * temp = pArr[i];
        pArr[i] = pArr[smallest];
        pArr[smallest] = temp;

    }

}  

 
 
