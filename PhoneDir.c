#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char option;
char e[30];
char f[40];
char m[100];
char n[20];
int found = 0;
int finished = 0;
int first=0;
int error = 0;
int done = 0;
int problem = 0;
FILE *fptr;
char location[100]; 
typedef struct details
{
    char fname[30];
    char gname[40];
    char address[100];
    char telephone[20];
} Details, *DPointer;

typedef struct node
{
    Details info;
    struct node *lChild, *rChild;
    bool deleted;
}Node, *NodePointer;


NodePointer newNode(Details y, NodePointer r, NodePointer l)
{
    NodePointer p = (NodePointer)malloc(sizeof(Node));
	if (p == NULL){
		printf("\nOut of memory - cannot create node\n");
		problem = 1;
	}
	else{
		p->info = y;
		p->lChild = r;
        p->rChild = l;
        p->deleted = false;
	}
	return p;
}

NodePointer insert(Details x, NodePointer root){
    /* Binary search tree insertion to insert a new details structure into the tree following the rules*/
    if(root == NULL){
        root = newNode(x, NULL, NULL);
    }
    else if(strcmp(x.fname, (root->info).fname) < 0){
        root->lChild = insert(x, root->lChild);
    }
    else if((strcmp(x.fname, (root->info).fname) > 0)){
        root->rChild = insert(x, root->rChild);
    }
    else if((strcmp(x.fname, (root->info).fname) == 0)){
        if(strcmp(x.gname, (root->info).gname) < 0){
            root->lChild = insert(x, root->lChild);
        }
        else if(strcmp(x.gname, (root->info).gname) > 0){
            root->rChild = insert(x, root->rChild);
        }
        else{
            printf("Error: The full name you have supplied is already present in the directory.\n");
			problem = 1;
        }
    }
    return root;
}

NodePointer delete(NodePointer root, char g[], char h[]){
    if(root!=NULL && found==0){
        delete(root->lChild, g, h);
        if(strcmp(g, (root->info).fname) == 0 && strcmp(h, (root->info).gname) == 0 && root->deleted == false){
            found = 1;
            root->deleted = true;
            printf("Successfully deleted entry.\n");
        }
        delete(root->rChild,g,h);
    }
    return root;
}

void printtree(NodePointer root){
    if(root!=NULL){
        printtree(root->lChild);
        if(root->deleted == false){
            printf("%s", (root->info).gname);
            printf("%s\n", (root->info).fname);
        }
        printtree(root->rChild);
    }
}

void search(NodePointer root, char g[], char h[]){
    if(root!=NULL && found==0){
        search(root->lChild,g,h);
        if(strcmp(g, (root->info).fname) == 0 && strcmp(h, (root->info).gname) == 0 && root->deleted == false){
            printf("Entry found: \n \n");
            found = 1;
            printf("Family Name: %s\n", (root->info).fname);
            printf("Given Name: %s\n", (root->info).gname);
            printf("Address: %s\n", (root->info).address);
            printf("Telephone Number: %s\n", (root->info).telephone);
        }
        search(root->rChild,g,h);
    }
}

NodePointer editdir(NodePointer root, char g[], char h[]){
    if(root!=NULL && found==0){
        editdir(root->lChild,g,h);
        if(strcmp(g, (root->info).fname) == 0 && strcmp(h, (root->info).gname) == 0 && root->deleted == false){
            printf("Entry found. \n \n");
            found = 1;
            printf("Please enter the new address you would like to save for this name.\n");
            fgets((root->info).address,100,stdin);
            printf("Please enter the new telephone number.\n");
            fgets((root->info).telephone,20,stdin);
            printf("Entry succesfully edited.\n");
        }
        editdir(root->rChild,g,h);
    }
    return root;
}
void savedir(NodePointer root, char g[]){
    if(first==0){
        fptr = fopen(g,"w");
		/*fptr = fopen("C:\\Users\\Ahmad\\Documents\\directory.txt","w");*/
        first++;
    }
    if(fptr==NULL){
        printf("Error creating file. Please try a different location\n");
		error = 1;
		
    }
    else{
        if(root!=NULL){
            savedir(root->lChild,g);
            if(root->deleted==false){
                fprintf(fptr,"%s",root->info.gname);
                fprintf(fptr,"%s",root->info.fname);
                fprintf(fptr,"%s",root->info.address);
                fprintf(fptr,"%s",root->info.telephone);
                fprintf(fptr,"\n");
            }
            savedir(root->rChild,g);
        }
    }
}


int main(void){
    Details temp;
    NodePointer directory = NULL;
    char blah[10];
    printf("Welcome to the MAK phone directory!\n");
    printf("Would you like to import an existing directory from a text file? (Y/N)\n");
    scanf("%c", &option);
    if(option == 'Y' || option == 'y'){
        printf("Please enter the location of the directory file.\n");
        printf("Please enter the file name in the following format: C:\\\\Users...\\\\filename.txt. Enter \\\\ between folders!\n");
        scanf("%s", &location);
        fptr = fopen(location,"r");

        if(fptr==NULL){
            printf("Error opening file. Please try again!\n");
            error = 1;
        }
        else{
            error = 0;
        }
        while(error==1){
            printf("Please enter the location of the directory file.\n");
            printf("Please enter the file name in the following format: C:\\\\Users...\\\\filename.txt. Enter \\\\ between folders!\n");
            scanf("%s", &location);
            fptr = fopen(location,"r");

            if(fptr==NULL){
                printf("Error opening file. Please try again!\n");
                error = 1;
            }
            else{
                error = 0;
            }    
        }
        while(done==0){
            fgets(temp.gname,30,fptr);
            fgets(temp.fname, 40, fptr);
            fgets(temp.address,100,fptr);
            fgets(temp.telephone, 20, fptr);
			fgets(blah,10,fptr);
            if(feof(fptr)){
                done = 1;
				printf("Successfully imported directory.\n");
            }    
            else{
                directory = insert(temp, directory);
            }
        }
    }
    while(finished == 0){
        printf("Press I to insert a new entry, D to delete an entry, S to search for a name, E to edit an entry, \n");
        printf("P to print all names in the directory or Q to quit the program and save the directory.\n");
        scanf(" %c", &option);
        if(option == 'I'){
			printf("\n");
            printf("Family Name:\n");
            getchar();
            fgets(temp.fname, 30, stdin);
            printf("Given Name:\n");
            fgets(temp.gname, 40, stdin);
            printf("Address:\n");
            fgets(temp.address, 100, stdin);
            printf("Telephone Number: \n");
            fgets(temp.telephone, 20, stdin);
            directory = insert(temp, directory);
			if(problem==0){
            	printf("Successfully added entry to directory.\n");
			}
			else{
				problem = 0;
			}
        }
        else if(option == 'D'){
			printf("\n");
            printf("Please enter the family name of the entry you would like to delete.\n");
            getchar();
            fgets(e, 30, stdin);
            printf("Please enter the given name.\n");
            fgets(f, 40, stdin);
            delete(directory, e, f);
            if(found == 0){
                printf("Error. No entry found with the provided name.\n");
            }
            else{
                found = 0;
            }
        }
        else if(option == 'S'){
			printf("\n");
            printf("Please enter the family name of the entry you would like to search the directory for.\n");
            getchar();
            fgets(e, 30, stdin);
            printf("Please enter the given name.\n");
            fgets(f, 40, stdin);
            search(directory,e,f);
            if(found == 0){
                printf("Error. No entry found with the provided name.\n");
            }
            else{
                found = 0;
            }
        }
		else if(option == 'E'){
			printf("\n");
			printf("Please enter the family name of the entry you would like to edit.\n");
            getchar();
            fgets(e,30,stdin);
            printf("Please enter the given name.\n");
            fgets(f,40,stdin);
            directory = editdir(directory,e,f);
            if(found == 0){
                printf("Error. No entry found with the provided name.\n");
            }  
            else{
                found = 0;
            }
		}
        else if(option == 'P'){
			printf("\n");
            printtree(directory);
			printf("\n");
        }
        else if(option == 'Q'){
            first = 0;
			printf("\n");
            printf("Please enter the file location to which you would like to save the file.\n");
            printf("Please enter the file name in the following format: C:\\\\Users...\\\\filename.txt. Enter \\\\ between folders!\n");
            /*getchar();
            fgets(location,100,stdin);*/
			scanf("%s",&location);
            savedir(directory,location);  
            fclose(fptr);
			if(error==0){
            	finished = 1;
				printf("Successfully saved directory to %s\n", location);
			}
			else{
				error = 0;
			}
			first = 0;
        }
		else {
			printf("Please enter a valid letter: I, D, S, E, P or Q\n");
		}
    }
    
	return 0;
}

