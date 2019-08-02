#include<stdio.h>
#include<string.h>
#include<malloc.h>
#define HASHSIZE 99999997

typedef struct l_node
{
	char pan[10];
	int freq;
	int found;
	struct node *left;
	struct node *right;
} node;
FILE *out, *paid;
int k;

/******************************************************
function name: 	getHashIndex
description: 	Calculates the hash index for every PAN
arguments: 	pan number whose hash index is required
******************************************************/
//changed
unsigned long int getHashIndex(char panNo[10]){
	unsigned long int index=0;
	int i;
	for(i=0;i<10;i++)
		index = index*31 + panNo[i];
	return (index%HASHSIZE);
}

/******************************************************
function name: 	insertPan
description: 	Inserts a PAN into the BST at the 
	     	specific index of Hash
arguments: 	root of the BST in which the PAN is to be inserted
	   	pan number which is to be inserted
******************************************************/
void insertPan(node **root, char panNo[10])
{
	int cmp;
	if(*root == NULL)
	{
		*root = (node*)malloc(sizeof(node));
		strcpy((*root)->pan,panNo);
		(*root)->freq=1;
		(*root)->found=0;
		(*root)->left = NULL;
		(*root)->right = NULL;
		return;
	}
	else
	{
        cmp=strncmp(panNo,(*root)->pan,10);
		if(cmp==0){
			((*root)->freq)++;
			return;
		}
		if(cmp<0)
		insertPan(&((*root)->left),panNo);
		else
		insertPan(&((*root)->right),panNo);
	}
}
/******************************************************
function name: 	printAllPans
description: 	Traverse through the BST with root
		and print all the nodes 
arguments: 	root of the BST whose nodes
		are to be printed
******************************************************/
void printAllPans(node *root)
{
	if(root!=NULL)
	{
		printAllPans(root->left);
        if(((root->found) == 1) && ((root->freq) >=k)){
			fprintf(out,"%s,%d\n",root->pan,root->freq);
            printf("Found pan %s",root->pan);
        }
		printAllPans(root->right);
	}
	else
	return;
	
}

/******************************************************
function name: 	checkPans
description: 	Iteratively takes PANs from paid file
		and searches it in spend
arguments: 	spendRoot- the entire hash where the 
		PAN is to be searched
******************************************************/
void checkPans(node *spendRoot[HASHSIZE])
{
	char paidPan[10];
	int freq;
	unsigned long int index;
	
        while(fgets(paidPan,11,paid)!=NULL){
			index = getHashIndex(paidPan);
			search(spendRoot[index],paidPan);
		}
}	

/******************************************************
function name: 	search
description: 	Search the panNo in a particular BST
arguments: 	root of the BST where the PAN
		is to be searched
		panNo: PAN which is to be searched
******************************************************/
void search(node *root, char panNo[10]){
	node *q;
	int found;
	q=root;
	found=0;
	while((q!= NULL) && (found == 0)){
        if((strncmp(panNo,(q->pan),10)==0) && ((q->freq) >=k)){
			q->found = 1;
			found=1;
		}
        else if(strncmp(panNo,(q->pan),10) < 0)
			q=q->left;
		else
			q=q->right;
	}
}
/******************************************************
function name: 	main
description: 	Opens the respective files and
		calls the respective functions
arguments: 	argc- total number of command line
		arguments
		argsv- holds the command line 
		arguments
******************************************************/
int main(int argc, char *argsv[]){
	static node *spendRoot[HASHSIZE];
	FILE *spend;
	char panNo[10], spendFile[100], paidFile[1000]; 
	unsigned long int i,index;
	
	//Get the command line parameters
	k=atoi(argsv[1]);
	strcpy(paidFile,argsv[2]);
	strcpy(spendFile,argsv[3]);
		
    out = fopen("C:/Users/Getene/Documents/test/output.txt","w");
	if(out==NULL){
		printf("\nOutput file could not be opened");	
		exit(1);
	}
	spend=fopen(spendFile,"r");
	if(spend==NULL){
		printf("\nSpend file not found");	
	}
	
	//Initialize the hash table
	for(i=0;i<HASHSIZE;i++)
		spendRoot[i]=NULL;
	i=0;

	//Insert Spend PANs in BST at every index of hash	
    while(fgets(panNo,11,spend)!=NULL){
		index = getHashIndex(panNo);
		insertPan(&(spendRoot[index]),panNo);	
	}
	paid = fopen(paidFile,"r");
	if(paid == NULL)
		printf("\nPaid file not found");
	else
		// Check PANs in paid file which are not in Spend
		checkPans(spendRoot);
	for(i=0;i<HASHSIZE;i++)
		printAllPans(spendRoot[i]);
	fclose(spend);
	fclose(paid);
	fclose(out);	
    return 0;
}

