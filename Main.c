#include "Head.h"

int main()
{
	FILE* In;
	KStock* KS_Head = NULL;
	Table Table_arr[Table_Amount], *Parr[Table_Amount];
	char Pname[256];
	int Quantity, option, Tnumber, i, flag = 0;
	for (i = 0; i < Table_Amount; i++) // initializing lists to NULL
	{
		Table_arr[i].history = 0; // if table didnt delete order, n = 0. initialize all tables to 0
		Table_arr[i].head = NULL;
		Parr[i] = &Table_arr[i];
	}
	if ((In = fopen("Instructions.txt", "r")) == NULL)
	{
		printf("ERROR, Instructions file didn't open");
		exit(1);
	}
	while (1)
	{
		if (feof(In) != 0)
			break;
		if ((fscanf(In, " %d", &option)) != 1)
		{
			printf("fscanf returned value false");
			exit(1);
		}
		while(option != 1 && flag == 0) // if first option is not '1', ends program
		{
			printf("User Must Enter option '1' to initilize kitchen stock: ");
			if (scanf("%d", &option)!= 1)
			{
				printf("fscanf returned value false");
				exit(1);
			}
			if (option == 1)
				flag = 1;
		}
		flag = 1;
		switch (option)
		{
		case 1:
		{
			KS_Head = CreateProducts();
			break;
		}
		case 2:
		{
			if (fscanf(In, "%s%d", Pname, &Quantity) != 2)
				FreeMemoryKitchenStock(KS_Head);
			Add_Items(KS_Head, Pname, Quantity);
			break;
		}
		case 3:
		{
			if (fscanf(In, "%d%s%d", &Tnumber, Pname, &Quantity) != 3)
				FreeMemoryKitchenStock(KS_Head);
			OrderItem(KS_Head, Parr, Tnumber-1, Pname, Quantity);
			break;
		}
		case 4:
		{
			if (fscanf(In, "%d", &Tnumber) != 1)
				FreeMemoryTables(KS_Head, Parr);
			RemoveItem(KS_Head, Parr, Tnumber-1);
			break;
		}
		case 5:
		{
			if (fscanf(In, "%d", &Tnumber) != 1)
				FreeMemoryTables(KS_Head, Parr);
			RemoveTable(KS_Head, Parr, Tnumber-1);
		}
		}
	}
	FreeMemoryTables(KS_Head, Parr); // clears kitchen stock + table orders
	fclose(In);
	return 0;
}
