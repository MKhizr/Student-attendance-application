//Muhammad Khizr Shahid - 2413235
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>

//structure for Student with basic student properties
typedef struct Student {

    char student_ID[4];
    char Name[40];
    char department_code[4];

}STUDENT;

//structure for Table along with table related properties
typedef struct Table {

    STUDENT *STUDENTS;
    int Table_size;
    int number_of_students;

}TABLE;


//creates a new table according to the size input and initializes it
TABLE *createEmptyTable(int size) {

    int i;
    TABLE* Hash_table =  (TABLE *) malloc(sizeof(TABLE));

    if(Hash_table==NULL) {
        printf("Exiting; out of memory");
        exit(1);
    }

    Hash_table->Table_size=size;
    Hash_table->number_of_students=0;
    Hash_table->STUDENTS=(struct STUDENT*) calloc(Hash_table->Table_size, sizeof(STUDENT));



    for(i=0; i<Hash_table->Table_size;i++) {

        strcpy(Hash_table->STUDENTS[i].department_code," ");
        strcpy(Hash_table->STUDENTS[i].Name," ");
        strcpy(Hash_table->STUDENTS[i].student_ID," ");

    }

    return Hash_table;
}

//a function to calculate the prime number after doubling the size of table
int prime_tablesize(int size) {

    int newsize,temp_size,m,n,sum;

    temp_size=2*size;

    for(m=temp_size+1;m>0;m++) {
        sum=0;
        for(n=1;n<=m;n++) {
            if((m%n) == 0) {
                sum=sum+1;
            }
        }
        if(sum == 2) {
            newsize = m;
            break;
        }
    }

    return newsize;
}

//function for rehahsing
TABLE *Rehashing(TABLE *Hash_table, int size, int method){

    int i,value,newsize,key;

    //calculates the next prime number after doubling the value of size of current table
    newsize = prime_tablesize(size);

    //rehashing is performed so a new table with bigger size is required
    //hence a new table with bigger new prime size is created
    TABLE *new_table= createEmptyTable(newsize);

    //all the previous data of previous table is being copied to the new table after performing the quadratic probing or double hashing on the values again
    //as new table size will allot new indexes to the previous values

    //for double hashing
    if(method ==1) {
        for(i=0; i<size; i++) {
            if(strcmp(Hash_table->STUDENTS[i].Name," ") != 0) {

                char *temparr = (char *) malloc(sizeof(char)*4);
                strcpy(temparr,Hash_table->STUDENTS[i].student_ID);
                value= Index_Calculator(temparr,new_table);
                key = key_calculator(temparr);
                double_hashing(new_table,Hash_table->STUDENTS[i],value,key);
                new_table->number_of_students++;

            }
        }
    }

    //for quadratic probing
    if(method ==2){
        for(i=0; i<size; i++){
            if(strcmp(Hash_table->STUDENTS[i].Name," ") != 0) {

                char *temparr = (char *) malloc(sizeof(char)*4);
                strcpy(temparr,Hash_table->STUDENTS[i].student_ID);
                value= Index_Calculator(temparr,new_table);
                quadratic_collision(new_table,Hash_table->STUDENTS[i],value);
                new_table->number_of_students++;

            }
        }
    }

    return new_table;

}

//function for quadratic probing
void quadratic_collision(TABLE *Hash_Table,STUDENT temp,int index){

    int i,newindex;

    //formula for quadratic probing is applied
    //and as soon as we find an empty index in the table which isn't assigned to any other student we place the data in it
    for (i=0; i< Hash_Table->Table_size; i++) {

        newindex= (index + (i*i)) % Hash_Table->Table_size;

        if(strcmp(Hash_Table->STUDENTS[newindex].Name," ") == 0) {
            Hash_Table->STUDENTS[newindex] = temp;
            Hash_Table->number_of_students++;
            break;
        }
    }

}

//function for double hahsing
void double_hashing(TABLE *Hash_Table,STUDENT temp,int index,int key) {

    int i,newindex;

    //formula for double hashing is applied
    //and as soon as we find an empty index in the table which isn't assigned to any other student we place the data in it
    for (i=0; i< Hash_Table->Table_size; i++) {

        newindex= (index + (i* (7 - (key % 7)))) % Hash_Table->Table_size;

        if(strcmp(Hash_Table->STUDENTS[newindex].Name," ") == 0) {
            Hash_Table->STUDENTS[newindex] = temp;
            Hash_Table->number_of_students++;
            break;
        }
    }

}

//function to calculate the simple basic h1(key) which is used for double hashing and quadratic probing both
int Index_Calculator(char *temparr,TABLE* Hash_Table) {

    int key = key_calculator(temparr);
    int index = (2 * key) % Hash_Table->Table_size;
    return index;

}


//function to search for a student in the hash table
void Search(char *identifier,TABLE *Hash_Table,int method) {

    int i=0,flag=0,index,newindex,newflag=0;
    int innercount=0,outercount=0;

    //calculates the h1(key) of student which we want to search
    newindex=Index_Calculator(identifier,Hash_Table);


    //quadratic probing or double hashing formula is applied to find the exact index
    //which will be used next to make an attempt to search the value

    int calloriginalindexquadratic,calloriginalindexDH;

    //for double hashing
    if(method ==1) {
        int key = key_calculator(identifier);
        index= (newindex + (i* (7 - (key % 7)))) % Hash_Table->Table_size;
        calloriginalindexDH = index;
    }

    //for quadratic probing
    if(method == 2) {
        index = (newindex + (i*i)) % Hash_Table->Table_size;
        calloriginalindexquadratic = index;
    }


    //if hash table size isn't zero we search for the student
    if(Hash_Table->Table_size != 0) {

        while( (i < Hash_Table->Table_size ) || (flag == 1) || (newflag==1)) {

            //we check if the slot has any student or not
            if(strcmp(Hash_Table->STUDENTS[index].Name," ") != 0){

                    //if the slot has a student and the match is successful we print the required details and leave the loop
                    if(strcmp(Hash_Table->STUDENTS[index].student_ID,identifier) == 0) {
                        printf("Name:  %s\n",Hash_Table->STUDENTS[index].Name);
                        printf("Department:  %s\n",Hash_Table->STUDENTS[index].department_code);
                        printf("\n");
                        flag=1;
                        break;
                    }

                    else{

                        //if the slot has a student but the match is unsuccessful we increment the value of i to search at the new slot
                        //as per the hash function by following it.

                        //for double hashing
                        if(method ==1) {

                            int key = key_calculator(identifier);
                            index= (calloriginalindexDH + (i* (7 - (key % 7)))) % Hash_Table->Table_size;
                            i++;

                        }

                        //for quadratic probing
                        if(method == 2) {

                            index = (calloriginalindexquadratic + (i*i)) % Hash_Table->Table_size;
                            i++;

                        }
                    }
            }

            //after applying quadratic probing or double hashing again and again (and keep on getting unsuccessful match at every slot), as soon as we find an EMPTY slot after these unsucessfull attempts we break from the loop completely
            if (strcmp(Hash_Table->STUDENTS[index].Name," ") == 0 ) {
                newflag=1;
                break;
            }

        }
    }

    if(flag == 0) {
        printf("\nStudent is not found! \n\n");
    }


}

//function to calculate key
int key_calculator(char *temparr) {

    int key1=0;
    key1 = key1 + temparr[0];
    int temp1 = ((int) temparr[1]-48);
    int temp2 = ((int) temparr[2]-48);
    int key = (key1 - 65) + temp1 + temp2;

    return key;
}

TABLE* AddStudent(TABLE* Hash_Table,int method) {

    //initializations and declarations
    STUDENT temp;
    int index,i;
    double loadFactor;

    char *temparr = (char *) malloc(sizeof(char)*3);

    printf("Enter unique identifier: \n");
    scanf(" %[^\n]s",temp.student_ID);
    fflush(stdin);
    strcpy(temparr,temp.student_ID);

    int flag_check;

    //check if ID exists in the system or not
    for(i=0; i<Hash_Table->Table_size; i++) {
        if(strcmp(Hash_Table->STUDENTS[i].student_ID ,temparr) == 0) {
            flag_check=1;
            break;
        }

        else{
            flag_check=0;
        }
    }


    //if Id does not exist meaning flag ==0 then accept the new input
    if(flag_check==0) {

    printf("Enter name: \n");
    fflush(stdin);
    scanf(" %[^\n]s",temp.Name);
    printf("Enter department: \n");
    scanf(" %s",temp.department_code);

    //calculate the h1(key)
    index= Index_Calculator(temparr,Hash_Table);

    //for double hashing to add the student by applying the appropriate formulas as per the method input
    if(method==1) {
        int key = key_calculator(temparr);
        double_hashing(Hash_Table,temp,index,key);
    }

    //for quadratic probing to add the student by applying the appropriate formulas as per the method input
    if(method==2) {
        quadratic_collision(Hash_Table,temp,index);
    }

    printf("\n%s has been registered.\n\n",temp.Name);

    //load factor calculation
    loadFactor = (double) (Hash_Table->number_of_students) / (double) (Hash_Table->Table_size);

    //rehashing check
    if(loadFactor > 0.5) {
        Hash_Table= Rehashing(Hash_Table, Hash_Table->Table_size,method);
    }

    }

    //if Id already exist meaning flag == 1 then give an error message
    else {
        printf("ID should be unique!\n");
    }


return Hash_Table;


}

void PrintTable(TABLE *Hash_table){

    int i=0,j;

    printf("Index \t");
    printf("ID \t");
    printf("Name \t\t");
    printf("Department");
    printf("\n");

    for (i=0; i< Hash_table->Table_size; i++) {

        printf("%d \t",i);
        printf("%s \t", Hash_table->STUDENTS[i].student_ID);
        printf("%s \t", Hash_table->STUDENTS[i].Name);
        printf("%s \t", Hash_table->STUDENTS[i].department_code);
        printf("\n");
    }

    printf("\n");

}


int main () {


    int load_factor,method,option;


    //asks for user choice of hashing method
    printf("Enter open addressing technique: \n");
    printf("1) Double hashing\n");
    printf("2) Quadratic probing\n");
    scanf("%d",&method);
    printf("\n\n");

    //creates a table with size 11
    TABLE *Hash_table= createEmptyTable(11);

    do {

        printf("---------STUDENT ATTENDANCE APPLICATION---------\n");
        printf("1. Add a student\n");
        printf("2. Search for a student using ID\n");
        printf("3. Print Table\n");
        printf("4. Exit\n");
        printf("Enter option: \n");
        scanf("%d",&option);
        printf("\n");

        if(option== 1 || option==2 || option==3) {

        //adds a student
        if (option == 1) {

            Hash_table= AddStudent(Hash_table,method);

        }

        //asks for the ID of the person to be searched and goes to search function
        if(option == 2) {

            char uniqueID[4];
            printf("Enter unique identifier: ");
            scanf(" %s",uniqueID);
            Search(uniqueID,Hash_table,method);

        }


        //prints the table
        if(option == 3) {

            PrintTable(Hash_table);

        }

        }

    }while(option != 4);


return 0;
}


