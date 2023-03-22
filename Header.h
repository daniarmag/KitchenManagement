#ifndef _Header
#define _Header
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Manot
{
	char* ProductName;
	int Price, Quantity;
	struct Manot* Next; //A pointer to the next struct to create a linked list.
}Manot; //A struct of type Manot.

typedef struct Table
{
	int TableNumber, Bill;
    struct Manot* Meal; //A pointer to the next meal to create a linked list.
}Table; //A struct of type Table.

//Function declarations.
void Error_Msg(char* str);

void RemoveTable(Manot* kitchen, Table* arr, int TableNumber, int table);

int AddItems(Manot* kitchen, char* Meal, int Quan);

int OrderItem(Manot* kitchen, Table* arr, int TableNumber, int table, char* Meal, int Quan);

int RemoveItem(Manot* kitchen, Table* arr, int TableNumber, int table, char* Meal, int Quan);

Manot* MealChecker(Manot* kitchen, char* Meal);

Manot* CreateProducts(FILE* in);

Manot* DeleteList(Manot* head);

#endif