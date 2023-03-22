#include "Header.h"

/*The main function opens file to read in two structs and has a user menu 
to choose different options to operate the kitchen. */
int main()
{
	int  parm = 1, option, NumOfTab, table, funcret;
	char tempstr[51] = { 0 };
	FILE* mf, * insf;
	Table* arr;
	Manot* kitchen = NULL;
	//Opening the file in read mode and checking if it was successfully opened.
	if ((mf = fopen("Manot.txt", "rt")) == NULL)
		Error_Msg("input error");
	//Opening the file in read mode and checking if it was successfully opened.
	if ((insf = fopen("Instructions.txt", "rt")) == NULL)
		Error_Msg("input error");
	else
	{
		fscanf(insf, "%d", &NumOfTab);
		//Allocating memory for an array of tables (+1 to work with indexes 1-15).)
		arr = (Table*)calloc(NumOfTab + 1, sizeof(Table));
		if (arr == NULL)
		{
			Error_Msg("Error in memory allocation.\n");
		}
		/*This loop will run until the EOF while reading all the info from the files and sending
		it as parms to different functions in the switch-case method. */
		
		while (fscanf(insf, "%d", &option) != EOF)
		{	
			switch (option)
			{
				case 1:
					kitchen = CreateProducts(mf);
					printf("The kitchen was created.\n");
					break;
				case 2:
					fscanf(insf, "%s %d", tempstr, &parm);
					funcret = AddItems(kitchen, tempstr, parm);
					if (funcret == 1)
						printf("%d %s were added added to the kitchen.\n", parm, tempstr);
					break;
				case 3:
					fscanf(insf, "%d %s %d", &table, tempstr, &parm);
					funcret = OrderItem(kitchen, arr, NumOfTab, table , tempstr, parm);
					if (funcret == 1)
						printf("%d %s were added added to the table number %d.\n", parm, tempstr, table);
					break;
				case 4:
					fscanf(insf, "%d %s %d", &table, tempstr, &parm);
					funcret = RemoveItem(kitchen, arr, NumOfTab, table, tempstr, parm);
					if (funcret == 1)
						printf("%d %s was returned to the kitchen from table number %d.\n", parm, tempstr, table);
					break;
				case 5:
					fscanf(insf, "%d", &table);
					RemoveTable(kitchen, arr, NumOfTab, table);
					break;
				default:
					break;
			}
		}
		/*Freeing the array of table structs. Each meal linked list for the table will be freed in RemoveItem function. */
		free(arr);
		//Freeing the linked list (All the meals).
		DeleteList(kitchen);
		fclose(mf);
		fclose(insf);
	}
	return 0;
}