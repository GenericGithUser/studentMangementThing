#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAXMEM 500


char currentFileLoaded[MAXMEM] = "";

struct student{
	int id;
	char name[MAXMEM];
	float gpa;
	struct student *next;
};

void menu(struct student *head, FILE *fp);

struct student *resetList(struct student *head){
	struct student *temp;
	while(head !=NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
	
	return NULL;
}

struct student *stuAdder(struct student *head){
	
	struct student *current = malloc(sizeof(struct student)), *tail = NULL;
	
	printf("\nEnter New Student ID: ");
	scanf("%d", &current->id);
	getchar();
	printf("\nEnter New Student Name: ");
	fgets(current->name, MAXMEM, stdin);
	current->name[strcspn(current->name,"\n")] = 0;
	printf("\nEnter GPA of New Student: ");
	scanf("%f", &current->gpa);
	current->next = NULL;
	
	if(head == NULL){
		head = current;
	}
	else{
   tail = head;
		while(tail->next !=NULL){
			tail = tail->next;
		}
		tail->next = current;
	}
	
	return head;
}

void displayStu(struct student *head, FILE *fp){
	struct student *current = head;
	
	while(current != NULL){
		printf("ID: %d | Name: %s | GPA: %.2f |\n", current->id, current->name, current->gpa);
		current = current->next;
	}
	
	menu(head, fp);
}

void saveFile(struct student *head, FILE *fp){
	char fileName[MAXMEM]; 
	char buffer[MAXMEM];
	int choice;
	
	printf("\nEnter File Name to Save to: ");
	fgets(fileName, MAXMEM, stdin);
	fileName[strcspn(fileName, "\n")] = 0;
	
	
	fp = fopen(fileName, "r");

 	if(fp == NULL){
		fp = fopen(fileName, "w");
	}
	else if (strcmp(fileName, currentFileLoaded) == 0){
		while(choice < 1 || choice > 2){
			printf("File is the Same as Loaded File\nAppend or Overwrite?(1]Overwrite/2]Append): ");
			scanf("%d", &choice);
		}
		if(choice == 1){
			fp = fopen(fileName, "w+");			
		}else{
			fclose(fp);
			fp = fopen(fileName, "a");
			fprintf(fp, "%c", '\n');
		}
	}else{
		fclose(fp);
		fp = fopen(fileName, "a");
		fprintf(fp, "%c", '\n');
	}
	
	struct student *current = head;
	
	while(current != NULL){
		fprintf(fp, "%d ", current->id);
		fprintf(fp, "%s ", current->name);
		fprintf(fp, "%.2f ", current->gpa);
		fprintf(fp, "%c", '\n');
		current = current->next;
	}
	
	fclose(fp);
	menu(head, fp);
}

struct student *loadFile(struct student *head, FILE *fp){
	int choice = 0;
	char fileName[MAXMEM]; 
	char buffer[MAXMEM];
	struct student *current = NULL, *tail = NULL;
	
	if(head != NULL){
		printf("\033[31m");
		printf("WARNING: You have things in your current database\nMake Sure to Save as this will OVERRIDE your database\n");
		printf("\033[0m");
		do{
			printf("\nDo you wish to continue? 1) Yes 2) No: ");
			scanf("%d", &choice);
			getchar();
		}while(choice != 1 && choice != 2);
		
		if(choice == 2){
			menu(head, fp);
			return head;
		}
		
	}
	
	printf("\nEnter file name to be loaded: ");
	fgets(fileName, MAXMEM, stdin);
	fileName[strcspn(fileName, "\n")] = 0;
	strcpy(currentFileLoaded, fileName);
	
	fp = fopen(fileName, "r");
	
	while(fp == NULL){
		printf("\nFile is Empty, Enter File Name again: ");
		fgets(fileName, MAXMEM, stdin);
		fileName[strcspn(fileName, "\n")] = 0;
		fp = fopen(fileName, "r");
	}
	
	head = resetList(head);
	
	while(fgets(buffer, MAXMEM, fp) != NULL){
		current = malloc(sizeof(struct student));
		sscanf(buffer, "%d %s %f", &current->id, current->name, &current->gpa);
		current->next = NULL;
		
		if(head == NULL){
			head = tail = current;
		}
		else{
			tail->next = current;
			tail = current;
		}
	}
	
	fclose(fp);
	return head;
	
}


struct student *delRecord(struct student *head, FILE *fp){
	struct student *current = head, *prev = NULL;
	
	int idToDel, didOperation = 0;
	
	while(current != NULL){
		printf("ID: %d | Name: %s | GPA: %.2f |\n", current->id, current->name, current->gpa);
		current = current->next;
	}
	
	printf("\nPlease Input the ID number of the Record you want to delete\nID Number: ");
	scanf("%d", &idToDel);
	getchar();
	
	current = head;
	
	while(current != NULL){
		if(current->id == idToDel){
			if(prev == NULL){
				head = current->next;
				free(current);
				current = head;
				didOperation = 1;
			}
			else {
				prev->next = current->next;
				free(current);
				current = prev->next;
				didOperation = 1;
			}
		
		} else{
			prev = current;
			current = current->next;
		}
	}
	if(didOperation == 0){
		printf("\nID not found\nPlease Re-Enter");	
	}else{
		printf("\nRecord Deleted...");
	}
	
	return head;
}

void menu(struct student *head, FILE *fp){
	int choice = 0;
	
	while(choice < 1 || choice > 6){
		printf("What do you want to do:\n ");
		printf("1) Add a Student\n2) Display All Students\n3) Save Records to File\n4) Load Records from a File\n5) Delete A Record?\n6) Exit the Program\nYour Choice: ");
		scanf("%d", &choice);
		getchar();
	}
	switch(choice){
			case 1: 
			head = stuAdder(head);
			menu(head, fp);
			break;
		case 2:
			displayStu(head, fp);
			break;
		case 3: 
			saveFile(head, fp);
			break;
		case 4:
			head = loadFile(head, fp);
			menu(head, fp);
			break;
		case 5:
			head = delRecord(head, fp);
			menu(head, fp);
			break;
		case 6: 
			resetList(head);
			exit(0);
		default:
			printf("\nUnhandled Process, Exiting....\n");	
			exit(1);
			break;
	}
	
}


int main(){
	struct student *head = NULL;
	FILE *fp;
	menu(head, fp);
}