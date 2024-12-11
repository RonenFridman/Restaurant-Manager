#include "Head.h"

KStock* CreateProducts()
{
	FILE* Manot;
	KStock* Head = NULL, * temp, * tail = Head, *checkname;
	char buffer[256] = "", skipper; // buffer[256] = "" for initializing 
	int flag = 0;
	if ((Manot = fopen("Manot.txt", "r")) == NULL)
	{
		printf("Error! Manot.txt file didnt open");
		exit(1);
	}
	while (1)
	{
		flag = 0;
		temp = (KStock*)malloc(sizeof(KStock)); // allocates memory for new block
		if (temp == NULL)
		{
			printf("Memory allocating failed");
			FreeMemoryKitchenStock(Head);
			exit(1);
		}
		if (fscanf(Manot, "%s%d%d", &buffer, &temp->Quantity, &temp->Price) != 3) // checks if returned value is 3
		{
			printf("Error! fscanf returned value error");
			FreeMemoryKitchenStock(Head);
			exit(1);
		}
		if (temp->Price > 0 && temp->Quantity > 0) // checks if price/quantity is positive number
		{
			checkname = Head; // initializing checkname
			while (checkname != NULL) // loop for checking if product name is already exist
			{
				if ((strcmp(buffer, checkname->ProductName)) == 0)
				{
					printf("Error! Dish: %s is already exist in the kitchen\n", buffer);
					flag = 1;
				}
				checkname = checkname->next;
			}
			if (flag == 0)
			{
				temp->ProductName = (char*)malloc(strlen(buffer) * sizeof(char) + 1); // allocates memory 
				if (temp->ProductName == NULL)
				{
					printf("Memory allocating failed");
					FreeMemoryKitchenStock(Head);
					exit(1);
				}
				strcpy(temp->ProductName, buffer);
				if (Head == NULL) // enters only 1 time when head is null
				{
					Head = temp;
					tail = temp;
				}
				else // inserts new node to the end of the list
				{
					tail->next = temp;
					tail = tail->next;
				}
				tail->next = NULL;
				skipper = fgetc(Manot);
				while ((skipper == '\n') || (skipper == ' ')) // loop to skip unwanted spaces and '\n' 
				{
					skipper = fgetc(Manot);
				}
				if (feof(Manot) != 0)
					break;
				fseek(Manot, -1, SEEK_CUR); // if end of file didnt reached by forwarding the skipper ,pointer moves one step back in the text 
			}
		}
		else
			printf("Error! Price or quantity for %s is illegal\n", buffer);
	}
	fclose(Manot);
	printf("The kitchen was created\n");
	return Head;
}

void Add_Items(KStock* KS_Head, char* ProductName, int Quantity)
{
	KStock* temp = KS_Head;
	int flag = 0;
	if (Quantity > 0)
	{
		while (temp != NULL)
		{
			if ((strcmp(ProductName, temp->ProductName)) == 0) // if dish exist in the kitchen
			{
				temp->Quantity += Quantity; // adds dish quantity to kitchen stock
				flag = 1;
				printf("%d %s were added to the kitchen\n", Quantity, ProductName);
				break;
			}
			temp = temp->next;
		}
		if (flag == 0) // if dish is not in kitchen stock
			printf("We don't have %s in the kitchen stock!\n", ProductName);
	}
	else
		printf("Error, Quantity is negetive\n");
}

int OrderItem(KStock* KS_Head, Table** arr, int TableNumber, char* ProductName, int Quantity)
{
	KStock *temp = KS_Head;
	TP* New_Node, * Temp_Node = arr[TableNumber]->head;
	int flag = 0, Price = 0;
	int i;
	if ((TableNumber > Table_Amount) && (Quantity > temp->Quantity) && (Quantity < 0)) // checks if table number quantity is legal
	{
		printf("Error! Table Number or Quantity is illegal\n");
		return 0;
	}
	while (temp != NULL) // loop for kitchen stock
	{
		if ((strcmp(temp->ProductName, ProductName) == 0)) // checks if kitchen has the dish
		{
			if (arr[TableNumber]->head == NULL) // checks if its the first order for the table
			{
				strcpy(arr[TableNumber]->LastOrder, ProductName); // saves info for last order
				for (i = 0; i < Quantity; i++) 
					Price += temp->Price;
				arr[TableNumber]->LastPrice = Price; // saves info for last order
				arr[TableNumber]->LastQuantity = Quantity; // saves info for last order
				arr[TableNumber]->Bill = 0; // initialize bill
				arr[TableNumber]->Bill += Price;
				New_Node = (TP*)malloc(sizeof(TP)); // allocate memory
				if (New_Node == NULL)
				{
					printf("Memory allocating failed");
					FreeMemoryTables(KS_Head, arr);
					exit(1);
				}
				New_Node->Name = (char*)malloc(strlen(ProductName) * sizeof(char) + 1); // allocate memory
				if (New_Node->Name == NULL)
				{
					printf("Memory allocating failed");
					FreeMemoryTables(KS_Head, arr);
					exit(1);
				}
				strcpy(New_Node->Name, ProductName);
				New_Node->Quantity = Quantity;
				New_Node->PricePerQuantity = Price; 
				temp->Quantity -= Quantity; // reduce dish quantity from kitchen stock
				New_Node->next = NULL;
				flag = 1; // if adds node flag = 1
				arr[TableNumber]->head = New_Node;
				printf("%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber+1);
				break;
			}
			while (Temp_Node != NULL) // checks if table already ordered the dish before
			{
				if (strcmp(ProductName, Temp_Node->Name) == 0) // if dish is already ordered
				{
					strcpy(arr[TableNumber]->LastOrder, ProductName);
					for (i = 0; i < Quantity; i++)
						Price += temp->Price;
					arr[TableNumber]->LastPrice = Price;
					arr[TableNumber]->LastQuantity = Quantity;
					arr[TableNumber]->Bill += Price;
					Temp_Node->PricePerQuantity += Price;
					Temp_Node->Quantity += Quantity;
					flag = 1;
					printf("%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber+1);
				}
				Temp_Node = Temp_Node->next;
			}
			if (flag == 0) // if table already has orders, adds new node
			{
				New_Node = (TP*)malloc(sizeof(TP)); // allocate memory
				if (New_Node == NULL)
				{
					printf("Memory allocating failed");
					FreeMemoryTables(KS_Head, arr);
					exit(1);
				}
				New_Node->Name = (char*)malloc(strlen(ProductName) * sizeof(char) + 1); // allocate memory
				if (New_Node->Name == NULL)
				{
					printf("Memory allocating failed");
					FreeMemoryTables(KS_Head, arr);
					exit(1);
				}
				strcpy(New_Node->Name, ProductName);
				New_Node->Quantity = Quantity;
				New_Node->PricePerQuantity = 0; // initialize Price Per Quantity
				for (i = 0; i < Quantity; i++) // adds price per dish
					New_Node->PricePerQuantity += temp->Price;
				arr[TableNumber]->Bill += New_Node->PricePerQuantity; // adds dish prices to bill
				temp->Quantity -= Quantity; // reduce dish quantity from kitchen stock
				New_Node->next = arr[TableNumber]->head;
				arr[TableNumber]->head = New_Node;
				flag = 1; // if adds node flag = 1
				strcpy(arr[TableNumber]->LastOrder, ProductName);
				arr[TableNumber]->LastPrice = 0;
				for (i = 0; i < Quantity; i++)
					arr[TableNumber]->LastPrice += temp->Price;
				arr[TableNumber]->LastQuantity = Quantity;
				printf("%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber+1);
				break;
			}
		}
		temp = temp->next;
	}
	if (flag == 0) // if kitchen doesnt have the dish
		printf("We don't have %s, sorry!\n", ProductName);
	return 1;
}

void RemoveItem(KStock* KS_Head, Table** arr, int TableNumber)
{
	TP* Table = arr[TableNumber]->head;
	int flag = 0;
	if (arr[TableNumber]->history == 0) // if table has no calceled orders
	{
		while (Table != NULL) // if table have orders
		{
			if ((strcmp(Table->Name, arr[TableNumber]->LastOrder)) == 0)
			{
				arr[TableNumber]->Bill -= arr[TableNumber]->LastPrice; //removes dishes price from bill
				Table->Quantity -= arr[TableNumber]->LastQuantity; // removes quantity from table order
				Table->PricePerQuantity -= arr[TableNumber]->LastPrice; // removes price from table price per quantity
				flag = 1;
				printf("%d %s were returned to the kitchen from table number %d\n", arr[TableNumber]->LastQuantity, arr[TableNumber]->LastOrder, TableNumber+1);
				if (Table->Quantity == 0) // if last order is the head of the list
				{
					arr[TableNumber]->head = arr[TableNumber]->head->next; // moves list head to previous order
					free(Table->Name); // free order node
					free(Table); // free order node
					break;
				}
			}
			Table = Table->next;
		}
		if (flag == 0) // if table doesnt have orders
			printf("Table Number %d has no orders!\n", TableNumber+1);
	}
	else
		printf("Table Number %d already canceled order!\n", TableNumber+1);
}

void RemoveTable(KStock* KS_Head, Table** arr, int TableNumber)
{
	TP* Table;
	int flag = 0;
	while (arr[TableNumber]->head != NULL) // if table has orders, prints table details and clears memory
	{
		printf("%d %s", arr[TableNumber]->head->Quantity, arr[TableNumber]->head->Name);
		Table = arr[TableNumber]->head;
		arr[TableNumber]->head = arr[TableNumber]->head->next;
		free(Table->Name);
		free(Table);
		flag = 1;
		if (arr[TableNumber]->head != NULL)
			printf(", ");
	}
	if (flag == 1) // if table has orders
		printf(". %d nis + %.0lf nis for tips,please\n", arr[TableNumber]->Bill, (arr[TableNumber]->Bill*0.15));
	if (flag == 0) // if table doesnt orders
		printf("The table number %d is not ordered yet\n", TableNumber+1);
}

void FreeMemoryKitchenStock(KStock* KS_Head) // clears kitchen stock
{
	KStock* temp;
	while (KS_Head != NULL)
	{
		temp = KS_Head;
		KS_Head = KS_Head->next;
		free(temp->ProductName);
		free(temp);
	}
}

void FreeMemoryTables(KStock* KS_Head, Table** arr) // clears kitchen stock + table orders
{
	TP* temp;
	int i;
	FreeMemoryKitchenStock(KS_Head);
	for (i = 0; i < Table_Amount; i++)
	{
		while (arr[i]->head != NULL)
		{
			temp = arr[i]->head;
			arr[i]->head = arr[i]->head->next;
			free(temp->Name);
			free(temp);
		}
	}
}
