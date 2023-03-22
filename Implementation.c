#include "Header.h"

/*This function creates a linked list that contains all the list from Manot.txt. */
Manot* CreateProducts(FILE* mf)
{
	Manot* Head = NULL, *prev = NULL, *temp, *flag;
	char tempstr[51] = { 0 };
	//Creating a new node.
	temp = (Manot*)malloc(sizeof(Manot));
	if (temp == NULL)
	{
		DeleteList(Head);
		Error_Msg("Error in memory allocation.");
	}
	//Reading the information inside the file to the linked list.
	while (fscanf(mf, "%s %d %d", tempstr, &temp->Quantity, &temp->Price) != EOF)
	{
		if (temp->Price < 1)
		{
			printf("Price must be a positive number.\n");
			continue;
		}
		if (temp->Quantity < 1)
		{
			printf("Quantity must be a positive number.\n");
			continue;
		}
		//Allocating memory for each name depending by the number of letters in each name.
		temp->ProductName = (char*)malloc(strlen(tempstr) + 1);
		if (temp->ProductName == NULL)
		{
			DeleteList(Head);
			Error_Msg("Error in memory allocation.");
		}
		strcpy(temp->ProductName, tempstr);
		flag = MealChecker(Head, tempstr);
		if (flag != NULL)
			printf("This meal already exists.\n");
		else
		{
			//Next two conditions will be creating the linked list while placing the next node in the back of the list.
			if (Head == NULL)
			{
				temp->Next = Head;
				Head = temp;
				prev = temp;
			}
			else
			{
				temp->Next = prev->Next;
				prev->Next = temp;
				prev = temp;
			}
			temp = (Manot*)malloc(sizeof(Manot));
			if (temp == NULL)
			{
				Error_Msg("Error in memory allocation.");
			}
		}
	}
	//Freeing one unnecessary item that was added to the list inside the loop.
	free(temp);
	return Head;
}

/*This function frees memory of a linked list. */
Manot* DeleteList(Manot* head)
{
	Manot* temp = NULL;
	while (head != NULL)
	{
		temp = head;
		head = head->Next;
		free(temp->ProductName);
		free(temp);
	}
	return head;
}

/*This function checks if Meal string appears as a name of a meal in the kitchen linked list, returns
a pointer to the node with the list if the meal is found or null if the meal was not found. */
Manot* MealChecker(Manot* kitchen, char* Meal)
{
	Manot* temp = kitchen;
	while (temp != NULL)
	{
		if (strcmp(temp->ProductName, Meal) == 0)
			return temp;	
		temp = temp->Next;
	}
	return NULL;
}

/*This function checks if the table exists and if it does, it prints all the meals
that were ordered for this table, and the price + tip that the table should pay, 
then frees the memory that was allocated for the table.  */
void RemoveTable(Manot* kitchen, Table* arr, int TableNumber, int table)
{
	int i = 0;
	Manot* temp = NULL;
	if (table > TableNumber)
	{
		printf("Table %d does not exist.\n", table);
		return;
	}
	//Going through all the tables to check if the current table was ordered.
	while (i < TableNumber + 1)
	{
		if (arr[table].TableNumber == table)
			break;
		i++;
		printf("The table number %d is not ordered yet.\n", table);
		return;
	}
	temp = arr[table].Meal;
	//Printing all the meals that were ordered on the table.
	while (temp != NULL)
	{
		printf("%d %s. ", temp->Quantity, temp->ProductName);
		temp = temp->Next;
	}
	printf("%d nis + %.0lf tip, please!\n", arr[table].Bill, arr[table].Bill*0.1);
	//Freeing the memory that was allocated for each linked list (The meals on the table).
	for (i = 0; i < TableNumber + 1; i++)
	{
		if (arr[i].Meal != NULL)
		{
			temp = arr[i].Meal;
			free(temp);
		}
	}
}

/*This function decreases the quantity of a meal on a table, returns 1
if the meal was found on the table with the correct quantity, and 0 if
one the conditions were not met. */
int RemoveItem(Manot* kitchen, Table* arr, int TableNumber, int table, char* Meal, int Quan)
{
	Manot* temp = NULL, * secondtemp = NULL;
	int i = 1;
	if (table > TableNumber)
	{
		printf("Table %d does not exist.\n", table);
		return 0;
	}
	//Going through all the tables to check if the current table was ordered.
	while (i < TableNumber+1)
	{
		if (arr[table].TableNumber == table)
			break;
		i++;	
		printf("The table number %d is not ordered yet.\n", table);
		return 0;
	}
	//Using the MealChecker function to check if the meal exists in the kitchen.
	secondtemp = MealChecker(kitchen, Meal);
	if (secondtemp == NULL)
	{
		printf("We don't have %s, sorry!\n", Meal);
		return 0;
	}
	//Using the MealChecker function to check if the meal exists on the table.
	temp = MealChecker(arr[table].Meal, Meal);
	if (temp == NULL)
	{
		printf("Table number %d did not order %s!\n", table, Meal);
		return 0;
	}
	if (Quan < 1)
	{
		printf("Quantiaty must be positive.\n");
		return 0;
	}
	else if (Quan > temp->Quantity)
	{
		printf("The quantiaty on the table must be higher than the desired amount to cancle.\n");
		return 0;
	}
	//Subtracting the desired amount from the quantity if all the conditions were met.
	temp->Quantity -= Quan;
	//Updating the bill.
	arr[table].Bill = arr[table].Bill - (Quan*temp->Price);
	return 1;
}

/*This function builds a linked list that consists of the meals that were ordered on a specific
table, updates the bill tab for the table, returns 0 if one of the conditions was not met 
and 1 if all the conditions were met. */
int OrderItem(Manot* kitchen, Table* arr, int TableNumber, int table, char* Meal, int Quan)
{
	Manot* temp = NULL, *OnTable = NULL, *newtemp;
	if (table > TableNumber)
	{
		printf("Table %d does not exist.\n", table);
		return 0;
	}
	//Using the MealChecker function to check if the meal exists in the kitchen.
	temp = MealChecker(kitchen, Meal);
	if (temp == NULL)
	{
		printf("We don't have %s, sorry!\n", Meal);
		return 0;
	}
	if (Quan < 1)
	{
		printf("Quantiaty must be positive.\n");
		return 0;
	}
	else if (Quan > temp->Quantity)
	{
		printf("Not enough quantity in the kitchen.\n");
		return 0;
	}
	//Subtracting  the quantity that was ordered from the kitchen stock.
	temp->Quantity -= Quan;
	newtemp = (Manot*)malloc(sizeof(Manot));
	if (newtemp == NULL)
	{
		Error_Msg("Error in memory allocation.");
	}
	//Creating a new node.
	newtemp->Price = temp->Price;
	newtemp->ProductName = temp->ProductName;
	newtemp->Quantity = Quan;
	//This condition checks if the table was already ordered before, and if wasn't, it creates the head of the linked list (meals) on the table.
	if (arr[table].TableNumber != table)
	{
		arr[table].TableNumber = table;
		newtemp->Next = arr[table].Meal;
		arr[table].Meal = newtemp;
		//Updating the bill.
		arr[table].Bill = arr[table].Bill + (newtemp->Price * newtemp->Quantity);
	}
	else
	{
		//Using the MealChecker function to check if the meal exists on the table.
		OnTable = MealChecker(arr[table].Meal, Meal);
		//Checking if the current meal was already ordered to only update the quantity and avoid creating a new node.
		if (OnTable != NULL)
		{
			OnTable->Quantity += Quan;
			//Updating the bill.
			arr[table].Bill = arr[table].Bill + (arr[table].Meal->Price * Quan);
			return 1;
		}
		//Adding the next elements to the linked list.
		newtemp->Next = arr[table].Meal->Next;
		arr[table].Meal->Next = newtemp;
		//Updating the bill.
		arr[table].Bill = arr[table].Bill + (newtemp->Price * newtemp->Quantity);
	}
	return 1;
}

/*This function adds to the quantity of a meal that is already on a table.
returns 0 if one of the conditions was not met and 1 if all worked successfully. */
int AddItems(Manot* kitchen, char* Meal, int Quan)
{
	Manot* temp = NULL;
	if (Quan < 1)
		printf("Quantiaty must be positive.\n");
	//Using the MealChecker function to check if the meal exists in the kitchen.
	temp = MealChecker(kitchen, Meal);
	if (temp == NULL)
		printf("This meal does not exist.\n");
	else
	{
		//Updating the stock of the meal in the kitchen.
		temp->Quantity += Quan;
		return 1;
	}
	return 0;	
}

/*This function will be called to print a message if a file was not open successfully or
the memory was not allocated successfully. */
void Error_Msg(char* str)
{
	printf("\n%s", str);
	exit(1);
}