#include "Assembler.h"

void update_labels(char *file)
{
	/*update label address in code list*/
	update_label_address(&code_list, label_list);

	/*update ext address in code list*/
	update_extern_address(&code_list, extern_list);
}

void update_label_address(List **head, List *head_label)
{
	List *temp_list1;
	List *temp_list2;
	char temp[SIZE_OF_NAME];

	for (temp_list1 = (*head); temp_list1; temp_list1 = temp_list1->next)
	{
		if ((if_label((temp_list1->name), FALSE)) == TRUE)
		{
			for (temp_list2 = (head_label); temp_list2; temp_list2 = temp_list2->next)
			{
				strcpy(temp, temp_list2->name);
				temp[strlen(temp) - 1] = 0;
				if (strcmp(temp_list1->name, temp) == 0 || strcmp(temp_list1->name, temp_list2->name) == 0)
				{
					if (!(strcmp(temp_list2->type, "external") == 0))
					{
						temp_list1->code = temp_list2->address << 2;
						temp_list1->code = temp_list1->code | 2;
					}

					/*if external*/
					else
					{
						temp_list1->code = temp_list1->code | 1;
						add_to_list(&extern_list, temp_list1->name, 0, "e", temp_list1->address);
					}
				}
			}
		}
	}
}

/*updata external symbol address in listcode*/
void update_extern_address(List **head, List *headex)
{
	List *temp_list1;
	List *temp_list2;
	/*temp_list1  is pointing to the address of listcode*/
	for (temp_list1 = (*head); temp_list1 != NULL; temp_list1 = temp_list1->next)
	{
		/*temp_list2  is pointing to copy of listext*/

		if ((if_label((temp_list1->name), FALSE)) == TRUE)
		{
			for (temp_list2 = headex; temp_list2 != NULL; temp_list2 = temp_list2->next)
			{
				if (strcmp(temp_list2->name, temp_list1->name) == 0)
				{
					strcpy(temp_list1->type, "e");
					break;
				}
			}
		}
	}
}
