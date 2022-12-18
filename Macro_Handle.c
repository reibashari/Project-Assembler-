#include "Assembler.h"
#include "Macro_Handle.h"
/**
 *@param char* "filename.as" with/without macro
 *create file filename.as without macro
 *@return char* "filename.am"
 */

char *macros_run(char *as_file_name)
{
	FILE *as_file, *temp_file, *am_file;
	char macro[] = "macro";
	char endmacro[] = "endmacro";
	char *am_file_name = (char *)calloc(SIZE_OF_NAME, sizeof(char));
	char line[SIZE_OF_LINE];
	char temp_str[SIZE_OF_LINE];
	char temp_str_macro[SIZE_OF_LINE];
	char corrent_macro_name[SIZE_OF_NAME];
	int macro_found_flage = FALSE;
	int macro_name_size;
	macro_line *head = NULL;
	macro_line *corrent;
	macro_line *p;

	strncpy(am_file_name, as_file_name, strlen(as_file_name) - 3);
	strcat(am_file_name, ".am");
	as_file = fopen(as_file_name, "r");
	if (as_file == NULL)
	{
		/* error: could not open the file */
		printf("macros_run: could not open file %s\n", as_file_name);
		return NULL;
	}
	temp_file = fopen("temp", "w+");
	am_file = fopen(am_file_name, "w+");
	if (temp_file == NULL || am_file == NULL)
	{
		/* error: could not create the file */
		printf("macros_run: could not create file %s\n", am_file_name);
		fclose(as_file);
		remove("temp");
		remove(am_file_name);
		return NULL;
	}

	while (fgets(line, SIZE_OF_LINE, as_file) != NULL)
	{
		sscanf(line, "%s", temp_str);
		macro_name_size = sscanf(line, "%*s %s", temp_str_macro);

		if (strcmp(temp_str, macro) == 0 && if_name_free(head, temp_str_macro) == TRUE)
		{
			if (macro_name_size == EOF)
			{
				/* error: could not create the file */
				printf("macros_run: macro have no name %s\n", am_file_name);
				fclose(as_file);
				remove("temp");
				remove(am_file_name);
				return NULL;
			}
			macro_found_flage = TRUE;
			strcpy(corrent_macro_name, temp_str_macro);
			continue;
		}
		if (macro_found_flage == TRUE)
		{
			if (strcmp(temp_str, endmacro) == 0)
			{
				macro_found_flage = FALSE;
				continue;
			}
			else
			{
				corrent = (macro_line *)malloc(sizeof(macro_line));
				strcpy(corrent->name, corrent_macro_name);
				strcpy(corrent->text, line);
				if (head == NULL)
				{
					head = corrent;
				}
				else
					add_to_macro_list(head, corrent);
			}
		}
		else
		{
			if (strcmp(temp_str, macro) == 0)
			{
				/* error: could not create the file */
				printf("macros_run: macro cannot have macro %s\n", am_file_name);
				fclose(as_file);
				remove("temp");
				remove(am_file_name);
				return NULL;
			}
			fprintf(temp_file, "%s", line);
		}
	}

	fclose(as_file);
	fseek(temp_file, 0, SEEK_SET);
	while (fgets(line, SIZE_OF_LINE, temp_file) != NULL)
	{
		sscanf(line, "%s", temp_str);
		if (if_macro(head, temp_str) == TRUE)
		{
			for (p = head; p != NULL; p = p->next)
			{
				if (strcmp(p->name, temp_str) == 0)
				{
					fprintf(am_file, "%s", p->text);
				}
			}
		}
		else
		{
			fprintf(am_file, "%s", line);
		}
	}
	remove("temp");
	fclose(am_file);
	return am_file_name;
}

int if_macro(macro_line *head, char *macro_name)
{
	macro_line *p;
	for (p = head; p != NULL; p = p->next)
	{
		if (strcmp(p->name, macro_name) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int if_name_free(macro_line *head, char *corrent_macro_name)
{
	macro_line *p;
	if (head == NULL)
	{
		return TRUE;
	}
	for (p = head; p != NULL; p = p->next)
	{
		if (strcmp(p->name, corrent_macro_name) == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void add_to_macro_list(macro_line *head, macro_line *new)
{
	macro_line *p;
	for (p = head; p != NULL; p = p->next)
	{
		if (p->next == NULL)
		{
			p->next = new;
			return;
		}
	}
}
