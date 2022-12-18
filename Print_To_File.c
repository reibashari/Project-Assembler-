#include "Assembler.h"

char base32[] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};

void print_to_files(char *file)
{

	/*put assembly code to .ob file*/
	strcat(file, ".ob");
	print_to_ob_file(file);

	/*put the external labels in .ext file*/
	file[strlen(file) - 2] = 0;
	strcat(file, "ext");
	if (extern_flage == TRUE)
		print_to_ext(file, code_list, "e");

	/*put the entry labels in .ent file*/
	file[strlen(file) - 3] = 0;
	strcat(file, "ent");
	if (entry_flage == TRUE)
		print_to_ent(file, label_list, entry_list);

	free_list(&code_list);
	free_list(&data_list);
	free_list(&label_list);
	free_list(&extern_list);
	free_list(&entry_list);
}

/*free list*/
void free_list(List **head)
{
	List *pntList;
	while (*head)
	{
		pntList = (*head);
		free(pntList);
		(*head) = pntList->next;
	}
}

void print_to_ob_file(char *file)
{
	FILE *output_file;

	output_file = fopen(file, "w");
	print_list(code_list, output_file);
	fclose(output_file);

	output_file = fopen(file, "a+");
	print_list(data_list, output_file);
	fclose(output_file);
}

/*print in file the value of linked list*/
void print_list(List *head, FILE *output_file)
{
	int code;
	int i;

	while (head != NULL)
	{
		code = head->code & NEGATIV_MASK;
		/*print counter with leading zeros*/
		fprintf(output_file, "     %c%c ", base32[head->address / BASE32], base32[head->address % BASE32]);
		/*print in special base*/
		fprintf(output_file, "     %c%c", base32[code / BASE32], base32[code % BASE32]);
		head = head->next;
		fprintf(output_file, "\n");
	}
}

/*print listext in file*/
void print_to_ext(char *file, List *head, char *type)
{
	int i;
	List *pntList;
	FILE *output_file;

	output_file = fopen(file, "w");
	for (pntList = (head); pntList; pntList = pntList->next)
	{
		if (strcmp(pntList->type, type) == 0)
		{
			fprintf(output_file, "%s", pntList->name);
			for (i = 10 - strlen(pntList->name); i > 0; i--)
				fprintf(output_file, " ");
			fprintf(output_file, "%c%c\t\n", base32[pntList->address / BASE32], base32[pntList->address % BASE32]);
		}
	}
	fclose(output_file);
}

/*print listent in file*/
void print_to_ent(char *file, List *head, List *head_entry)
{
	int i;
	List *temp_list1;
	List *temp_list2;
	char temp[SIZE_OF_NAME];
	FILE *output_file;

	output_file = fopen(file, "w");
	/*temp_list1  is pointing to the address of listcode*/
	for (temp_list1 = (head); temp_list1 != NULL; temp_list1 = temp_list1->next)
	{
		/*temp_list2  is pointing to copy of listent*/
		temp_list2 = head_entry;

		strcpy(temp, temp_list1->name);

		temp[strlen(temp) - 1] = 0;

		while (temp_list2 != NULL)
		{
			if (strcmp(temp_list2->name, temp) == 0)
			{
				temp_list2->address = temp_list1->address;
				fprintf(output_file, "%s", temp_list2->name);
				for (i = 10 - strlen(temp_list2->name); i > 0; i--)
					fprintf(output_file, " ");
				fprintf(output_file, "%c%c\t\n", base32[temp_list1->address / BASE32], base32[temp_list1->address % BASE32]);
			}

			temp_list2 = temp_list2->next;
		}
	}

	fclose(output_file);
}
