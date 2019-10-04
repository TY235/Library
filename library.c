#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*Structure declaration*/

/*Books*/
struct book {
	char *title;
	char *author;
	char *subject;
};

/*Nodes*/
struct library {
	struct book collection;
	int num_books;
	struct library *next;
};

/*Function prototype*/
void addBook();
void deleteBook();
void searchBook();
void listByAuthor();
void listBySubject();
void bubbleSort(char listArray[][50],int Max);
void copybook(struct book* dest, struct book* source);
int strCaseCompare(char str1[],char str2[]); 	/*Compare string without case sensitive*/

/*Declare file pointer*/
FILE *libraryPtr; 	/*File pointer to read .txt file*/
FILE *outputPtr;	/*File pointer to write .txt file*/

/*Initialization*/
struct library* firstPtr = NULL; 	/*The first node of the linked list*/
int numberOfBooks =  0;		/*Number of books for checking condition*/

int main(){

	int noOfUpdates;
	int usersChoice;
	int counter;


	libraryPtr = fopen("library.txt","r");	/*Open file to read*/
	outputPtr = fopen("output.txt","w");	/*Open file to write*/

	/*Check if there is any error while opening files*/
	if((libraryPtr==NULL)||(outputPtr==NULL)){
		printf("Error !\n");
		if(outputPtr!=NULL){
			fprintf(outputPtr,"Error !\n");
		}
		exit(1);
	}

	else{
	
	fscanf(libraryPtr,"%d",&noOfUpdates);

	/*Loop until the number of updates has reached*/
	for(counter=1;counter<=noOfUpdates;counter++){
		
		/*Scan for the input form library.txt*/
		fscanf(libraryPtr,"%d",&usersChoice);

			switch(usersChoice)
			{
				/*Add book for choice 1*/
				case 1 :
					addBook();
					break;

				/*Delete book for choice 2*/
				case 2 :
					deleteBook();
					break;

				/*Search book for choice 3*/
				case 3 :
					searchBook();
					break;

				/*List book by author for choice 4*/
				case 4 :
					listByAuthor();
					break;

				/*List book by subject for choice 5*/
				case 5 :
					listBySubject();
					break;

				/*If choice is not in the range*/
				default :
					printf("Choice is out of range.\n");
					fprintf(outputPtr,"Choice is out of range.\n");
					break;
			}
       
			/*Update on the number of books*/
			/*If there are books in the library*/
			if(firstPtr!=NULL){
				numberOfBooks = firstPtr->num_books;
			}
			/*If there is no book in the library*/
			else{
				numberOfBooks = 0;
			}
			printf("\n\n");
			fprintf(outputPtr, "\n\n" );
		}

	}
	return 0;
}


void addBook(){

	struct library* newPtr;		/*Pointer to assign new address*/
	struct library* currentPtr;		/*Current pointer to check for the node which we are currently accessing*/
	struct book* bookPtr;

	/*Malloc the variables*/
	bookPtr = (struct book*)malloc(sizeof(struct book));
	bookPtr->title = (char*)malloc(50);
	bookPtr->author = (char*)malloc(50);
	bookPtr->subject = (char*)malloc(50);
	newPtr = (struct library*)malloc(sizeof(struct library));

	/*Initialization*/
	newPtr->next = NULL;
	newPtr->collection = *bookPtr;

	/*If malloc returns NULL */
	if (bookPtr==NULL){
		printf("Out of memory.\n");
		fprintf(outputPtr,"Out of memory.\n ");
	}
	else{
		/*Scan for book title author and subject from the library.txt*/
		fscanf(libraryPtr,"%s %s %s",bookPtr->title,bookPtr->author,bookPtr->subject);

		/*Skip this function if the library has more than 100 books*/
		if(numberOfBooks >= 100){
			printf("The library is full.\n");
			fprintf(outputPtr,"The library is full.\n");
			return;
		}

		/*If there is no book in the library*/
		if(firstPtr==NULL){
			firstPtr = newPtr;		/*Allocate address to the first pointer*/
			firstPtr->num_books = 0;	/*Initialize number of books as 0*/
			currentPtr = firstPtr;			/*Initialize the current pointer*/
			firstPtr->num_books += 1;	/*Add the number of books by 1*/
			printf("The book %s has been added to the library.\n",currentPtr->collection.title);
			fprintf(outputPtr,"The book %s has been added to the library.\n",currentPtr->collection.title);
			return;
		}

		/*If there are books in the library*/
		else{
			currentPtr = firstPtr;		/*Starts from the first node of the linked list*/

			/*Loop while it is not the last book*/
			while(currentPtr->next!=NULL){

				/*If the title of the book pointed by the cuurent pointer matches with the title of the book the user wanted to add*/
				if(strcmp(bookPtr->title,currentPtr->collection.title)==0){
					printf("The book %s has already exist.\n",currentPtr->collection.title);
					fprintf(outputPtr,"The book %s has already exist.\n",currentPtr->collection.title);
					return;
				}
				
				/*If the title of the last book is the same as the title of the book the user wanted to add*/
				if(strcmp(bookPtr->title,currentPtr->next->collection.title)==0){
					printf("The book %s has already exist.\n",currentPtr->next->collection.title);
					fprintf(outputPtr,"The book %s has already exist.\n",currentPtr->next->collection.title);
					return;
				}

				currentPtr=currentPtr->next;	/*Move to the next book*/
			}
		
			/*If the book is not found in the library*/
			currentPtr->next = newPtr;		/*Assign address to the next node of the last node*/
			firstPtr->num_books += 1;		/*Add the number of books by 1*/
			printf("The book %s has been added to the library.\n",currentPtr->next->collection.title);
			fprintf(outputPtr,"The book %s has been added to the library.\n",currentPtr->next->collection.title);
		}
	}
}

void deleteBook(){

	struct library* currentPtr;		/*Current pointer to check for the node which we are currently accessing*/
	struct library* previousPtr;	/*Previous pointer to check for the second last node*/
	struct library* thisPointer;	/*Pointer that points to the book that the user wanted to delete*/
	int needDelete = 0;		/*A flag to check if there is book to be deleted*/
	char title[50];		/*Title of the book the user wanted to delete*/
	fscanf(libraryPtr,"%s",title);

	/*If there is no book in the library skip this function*/
	if(firstPtr==NULL){
		printf("The book %s is unable to be deleted.Library is empty.\n",title);
		fprintf(outputPtr,"The book %s is unable to be deleted.Library is empty.\n",title);
		return;
	}


	/*Start from the first node of the linked list*/
	currentPtr = firstPtr;

	/*If the book to be deleted is the first and only book in the library*/
	if((strcmp(firstPtr->collection.title,title)==0)&&(currentPtr->next==NULL)){
		printf("The book %s has been removed from the library.\n",currentPtr->collection.title);
		fprintf(outputPtr,"The book %s has been removed from the library.\n",currentPtr->collection.title);
		firstPtr->num_books -=1; 	/*Minus the number of books by 1*/
		firstPtr = NULL;	/*Initialize the unused first pointer to NULL to avoid junk value*/
		return;
	}

	/*If first book is the book to be deleted*/
	if(strcmp(firstPtr->collection.title,title)==0){
		printf("The book %s has been removed from the library.\n",currentPtr->collection.title);
		fprintf(outputPtr,"The book %s has been removed from the library.\n",currentPtr->collection.title);
		firstPtr->num_books -=1; 	/*Minus the number of books by 1*/
		while(currentPtr->next!=NULL){
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}
		copybook(&firstPtr->collection,&currentPtr->collection);	/*Call the copy book function and copy the last book to the node of the book to be deleted*/
		previousPtr->next= NULL;	/*Make the second last node to become the last node*/
		free(currentPtr);	/*Free the pointer*/
		return;	/*Return to the main function*/
	}

	/*Loop while it is not the last book*/
	while(currentPtr->next!=NULL){

		/*If the book currently pointed by the current pointer matches with the book the user wanted to delete*/
		if(strcmp(currentPtr->collection.title,title)==0){
			previousPtr = currentPtr;	/*Previous pointer follows behind the current pointer*/
			thisPointer = currentPtr;	/*Assign the current pointer address which points to the book that needs to be deleted to this pointer */
			needDelete = 1; 	/*Flag the need delete to true*/
			currentPtr = currentPtr->next;	/*Move to the next pointer*/
		}

		/*If the book to be deleted is not found yet*/
		else{
			/*Move to the next pointer*/
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}
	}

	/*If the book to be deleted is the last book*/
	if((currentPtr->next==NULL)&&(strcmp(currentPtr->collection.title,title)==0)){
		thisPointer = currentPtr; 	/*Assign the current pointer address which points to the book that needs to be deleted to this pointer*/
		needDelete = 1;	/*Flag need delete to true*/
	}

	/*If the book to be deleted is found*/
	if(needDelete==1){
		printf("The book %s has been removed from the library.\n",thisPointer->collection.title);
		fprintf(outputPtr,"The book %s has been removed from the library.\n",thisPointer->collection.title);
		copybook(&thisPointer->collection,&currentPtr->collection);	/*Call the copy book function and copy the last book to the node of the book to be deleted*/
		previousPtr->next = NULL;	/*Assign NULL to the second last node of the linked list and make it the last node*/
		firstPtr->num_books -= 1;	/*Minus the total number of books by 1*/
		free(currentPtr);	/*Free the pointer*/
		return; /*Return to the main function*/
	}

	/*If the book to be deleted is not found in the library*/
	else{
		printf("The book %s is unable to be deleted.It's not found in the library.\n",title);
		fprintf(outputPtr,"The book %s is unable to be deleted.It's not found in the library. \n",title);
	}
}

void searchBook(){

	struct library* currentPtr;	/*Current pointer to check for the node which we are currently accessing*/
	char title[50];
	fscanf(libraryPtr,"%s",title);

	/*If there is no book in the library*/
	if(firstPtr==NULL){
		printf("The book %s is NOT currently in the library.Library is empty.\n",title);
		fprintf(outputPtr,"The book %s is NOT currently in the library.Library is empty.\n",title);
		return;		/*Return to the main function*/
	}

	/*Start from the first node of the linked list*/
	currentPtr = firstPtr;

	/*Loop until the last book*/
	while(currentPtr!=NULL){

		/*If the book to be searched is the same as title of the book ponited by the current pointer*/
		if(strcmp(title,currentPtr->collection.title)==0){
			printf("The book %s is currently in the library.\n",currentPtr->collection.title);
			fprintf(outputPtr,"The book %s is currently in the library.\n",currentPtr->collection.title);
			return;		/*Return to main function*/
		}

		currentPtr=currentPtr->next;	/*Move to the next pointer*/
	}
	
	/*Print this line when the program has moved through the linked list and the title of the book is not yet found.*/
	printf("The book %s is NOT currently in the library.\n",title);
	fprintf(outputPtr,"The book %s is NOT currently in the library.\n",title);
}

void listByAuthor(){

	struct library* currentPtr;	/*Current pointer to check for the node which we are currently accessing*/
	char author[50];	/*A string array to store the author input by the user*/
	char listArray [101][50];	/*2-D array to store the author of the books*/
	int noOfBooks;	/*Number of books by the author*/
	int counter;

	noOfBooks = 0;	/*Initialize the number of books by the author to 0*/


	fscanf(libraryPtr,"%s",author);

	/*Start from the first node of the linked list*/
	currentPtr = firstPtr;

	/*If there is no book in the library*/
	if(firstPtr==NULL){
		printf("List all the books by %s\n",author);
		fprintf(outputPtr,"List all the books by %s\n",author);
		printf("No book from author %s\n",author);
		fprintf(outputPtr,"No book from author %s\n",author);
		return;	/*Return to the main function*/
	}
	printf("List all the books by %s\n",author);
	fprintf(outputPtr,"List all the books by %s\n",author);

	/*Loop until the last book*/
	while(currentPtr!=NULL){

		/*If the author entered by user matches with the author pointed by the current pointer*/
		if(strCaseCompare(author,currentPtr->collection.author)==0){
			noOfBooks ++; 	/*Add the number of books by the author by 1*/
			strcpy(listArray[noOfBooks],currentPtr->collection.title);	/*Copy the title of the book into the 2-D array*/
		}
		currentPtr=currentPtr->next;	/*Move to the next node*/
	}

	bubbleSort(listArray,noOfBooks);	/*Call the bubble sort function*/

	/*If there is no book by the author*/
	if(noOfBooks==0){
			printf("No book from author %s\n",author);
			fprintf(outputPtr,"No book from author %s\n",author);
			return;		/*Return to main function*/
		}
	
	/*If there is book by the author*/
	else{
		/*Loop through the array and print the list in ascending order*/
		for(counter = 1; counter <= noOfBooks; counter++){
			printf("%s\n",listArray[counter]);
			fprintf(outputPtr,"%s\n",listArray[counter]);
		}
	}
}

void listBySubject(){

	struct library* currentPtr;	/*Current pointer to check for the node which we are currently accessing*/
	char listArray1 [101][50];	/*2-D array to store the subject of the books*/
	char subject[50];	/*The subject to be listed*/
	int noOfBooks1;	/*Number of books on the subject*/
	int counter;

	noOfBooks1 = 0; /*Initialize the number of books to 0*/

	fscanf(libraryPtr,"%s",subject);

	/*Start from the first node of the linked list*/
	currentPtr = firstPtr;
   
    /*If there is no book in the library*/
	if(firstPtr==NULL){
		printf("List of all books on %s\n",subject);
		fprintf(outputPtr,"List of all books on %s\n",subject);
		printf("No related book for %s is found.\n",subject);
		fprintf(outputPtr,"No related book for %s is found.\n",subject);
		return;	/*Return to the main function*/
	}
	printf("List of all books on %s\n",subject);
	fprintf(outputPtr,"List of all books on %s\n",subject);

	/*Loop until the last book*/
	while(currentPtr!=NULL){
		
		/*If the subject entered by user matches with the subject pointed by the current pointer*/
		if(strCaseCompare(subject,currentPtr->collection.subject)==0){
			noOfBooks1 ++;	/*Add the number of books on the subject by 1*/
			strcpy(listArray1[noOfBooks1],currentPtr->collection.title);	/*Copy the title of the book into the 2-D array*/
			}
			currentPtr=currentPtr->next;	/*Move to the next pointer*/
		}

		bubbleSort(listArray1,noOfBooks1);	/*Call bubble sort function*/

		/*If there is no related book for the subject found*/
		if(noOfBooks1==0){
				printf("No related book for %s is found\n",subject);
				fprintf(outputPtr,"No related book for %s is found\n",subject);
				return; /*Return to the main function*/
			}

		/*If there are books found on the subject*/
		else{

			/*Loop through the array and print the title of the books in descending order*/
			for(counter = noOfBooks1; counter >= 1; counter--){
				printf("%s\n",listArray1[counter]);
				fprintf(outputPtr,"%s\n",listArray1[counter]);
			}
		}
}

void bubbleSort(char listArray[][50],int Max){

	char temp[50];	/*Temporary array to store the title of the book*/
	int counter;
	int noMoreSwap;	/*A flag to check if swapping is still required*/

	do{
		noMoreSwap = 1;	/*Initialize the flag as true*/

		/*Loop through the related books*/
		for(counter=1 ; counter < Max ; counter++){

			/*If the title of the book of the current position has a bigger ASCII value than the next position*/
			if(strCaseCompare(listArray[counter],listArray[counter+1])>0){
				strcpy(temp,listArray[counter]); 	/*Copy the title of the book on the current position to the temporary string array*/
				strcpy(listArray[counter],listArray[counter+1]);	/*Copy the title of the book on the next position to the current position*/
				strcpy(listArray[counter+1],temp);	/*Copy the title of the books from the temporary string array to the next position*/
				noMoreSwap = 0;		/*Flag no more swap to false if the swapping process is carried out*/
			}
		}
		Max--;	/*Minus the pass by one*/

	}while (noMoreSwap == 0); /*Loop while swap is still required*/
}

void copybook(struct book* dest, struct book* source){

	/*Copy title, author and subject of the book from the last node to the node of the book that needs to be deleted*/
	strcpy(dest->title,source->title);
	strcpy(dest->author,source->author);
	strcpy(dest->subject,source->subject);

}

int strCaseCompare(char str1[], char str2[]){

	int i ;		/*Counter*/
	char string1[50];
	char string2[50];
    strcpy(string1,str1);	/*Copy the first original string to a new string array to avoid changes to the original string*/
    strcpy(string2,str2);	/*Copy the second original string to a new string array to avoid changes to the original string*/

	/*Convert the first string to uppercase*/
	for(i = 0; str1[i] != '\0'; i++){
		string1[i] = toupper(string1[i]);
	}

	/*Convert the second string to uppercase*/
	for(i = 0; str2[i] != '\0'; i++){
		string2[i] = toupper(string2[i]);
	}

	/*Return the compared value*/
    return strcmp(string1,string2);
}
