#ifndef _Head   
#define _Head
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define Table_Amount 50

typedef struct Table
{
    char LastOrder[256]; // saves last order for each table
    int LastQuantity; // saves last order for each table
    int LastPrice; // saves last order for each table
    int history; // saving 1 if order already deleted
    int Bill;
    struct Table_Products* head;
}Table;

typedef struct Table_Products
{
    char* Name;
    int Quantity;
    int PricePerQuantity;
    struct Table_Products *next;
}TP;

typedef struct KitchenStock
{
    char* ProductName;
    int Price;
    int Quantity;
    struct KitchenStock* next;
}KStock;

KStock* CreateProducts();
void Add_Items(KStock* Head, char* ProdName, int num);
int OrderItem(KStock* KS_Head, Table** arr, int TableNumber, char* ProductName, int Quantity);
void RemoveItem(KStock* KS_Head, Table** Table_arr, int Tnumber);
void RemoveTable(KStock* KS_Head, Table** Table_arr, int Tnumber);
void FreeMemoryKitchenStock(KStock* Head);
void FreeMemoryTables(KStock* KS_Head, Table** arr);

#endif