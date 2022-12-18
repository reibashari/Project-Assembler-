#include "Assembler.h"

void update_lists(FILE *file)
{
	int i;
	char *sep = "\t\n, \r";
	char *token;
	char *label_name;
	char *str_tol_end;
	char line[SIZE_OF_LINE];
	int token_length;
	int str_tol_int;
	int current_line_number = 0;
	int label_flage;

	while (fgets(line, SIZE_OF_LINE, file) != NULL)
	{
		if (feof(file) == FALSE)
		{
			*(strchr(line, '\n')) = '\0';
		}
		current_line_number++;
		label_flage = FALSE;
		/* read word by word */
		token = strtok(line, sep);
		/* comment or empty line */
		if (token == NULL || token[0] == ';')
			continue;
		else
		{
			/* label */
			if (((if_label(token, TRUE)) == TRUE))
			{
				/* label already exist */
				if (if_label_exists(token, label_list, FALSE) == TRUE)
				{
					printf("in line %d  label already exist\n", current_line_number);
					syntax_error_flage = TRUE;
					continue;
				}
				else
				{
					/* move to next word */
					label_flage = TRUE;
					label_name = token;
					token = strtok(NULL, sep);
				}
			}
			/* line contin with data */
			if (strcmp(token, ".data") == 0)
			{
				/* get first number */
				token = strtok(NULL, sep);
				/* decode number from string */
				str_tol_int = strtol(token, &str_tol_end, SIZE_OF_WORD);

				if (label_flage == TRUE)
				{
					/* add first number and label */
					add_to_list(&data_list, label_name, str_tol_int, "d", DC);
					/* add to label list address of first number */
					add_to_list(&label_list, label_name, 0, "data", DC);
				}
				else
				{
					/* add first number */
					add_to_list(&data_list, NULL, str_tol_int, "d", DC);
				}
				token = strtok(NULL, sep);
				/* read the rest of numbers */
				while (token != NULL)
				{
					DC++;
					str_tol_int = strtol(token, &str_tol_end, SIZE_OF_WORD);
					add_to_list(&data_list, NULL, str_tol_int, "d", DC);
					token = strtok(NULL, sep);
				}
				DC++;
			}
			/* string */
			else if (strcmp(token, ".string") == 0)
			{

				token = strtok(NULL, sep);
				token_length = strlen(token);
				if ((token[0] == '"') && (token[token_length - 1] == '"'))
				{
					/* add first char and label */
					if (label_flage == TRUE)
					{
						/* add first char and label adress */
						add_to_list(&data_list, label_name, (int)token[1], "d", DC);
						add_to_list(&label_list, label_name, 0, "data", DC);
					}
					else
					{
						/* add first char */
						add_to_list(&data_list, NULL, (int)token[1], "d", DC);
					}
					DC++;

					/* add the rest of the string */
					for (i = 2; i < token_length - 1; ++i)
					{
						add_to_list(&data_list, "", (int)token[i], "d", DC);
						DC++;
					}
					/* put '\0' at the end */
					add_to_list(&data_list, "", 0, "d", DC);
					DC++;
				}
				else
				{
					printf("in line %d missing quetes\n", current_line_number);
					syntax_error_flage = TRUE;
					continue;
				}
			}

			/* struct */
			else if (strcmp(token, ".struct") == 0)
			{
				token = strtok(NULL, sep);
				str_tol_int = strtol(token, &str_tol_end, SIZE_OF_WORD);

				if (label_flage == TRUE)
				{
					add_to_list(&data_list, label_name, str_tol_int, "d", DC);
					add_to_list(&label_list, label_name, 0, "data", DC);
				}
				else
				{
					add_to_list(&data_list, NULL, str_tol_int, "d", DC);
				}

				DC++;
				token = strtok(NULL, sep);
				token_length = strlen(token);

				if ((token[0] == '"') && (token[token_length - 1] == '"'))
				{
					/*add first char and label*/
					add_to_list(&data_list, NULL, (int)token[1], "d", DC);
					DC++;

					/* add the all chars */
					for (i = 2; i < token_length - 1; ++i)
					{
						add_to_list(&data_list, "", (int)token[i], "d", DC);
						DC++;
					}
					add_to_list(&data_list, "", 0, "d", DC);
					DC++;
				}
				else
				{
					printf("in line %d missing quetes\n", current_line_number);
					syntax_error_flage = 1;
					continue;
				}
			}
			/* extern */
			else if (strcmp(token, ".extern") == 0)
			{

				token = strtok(NULL, sep);
				if ((if_label(token, FALSE)) == TRUE)
				{
					if (if_label_exists(token, label_list, TRUE) == TRUE)
					{
						printf("in line %d label already defineds as no external\n", current_line_number);
						syntax_error_flage = TRUE;
						continue;
					}
					else
					{
						add_to_list(&label_list, token, 0, "external", 0);
						extern_flage = TRUE;
					}
				}
				else
				{
					/* error: label not legal */
					printf("in line %d label not legal\n", current_line_number);
					syntax_error_flage = TRUE;
					continue;
				}
			}
			/* the line begin with entry */
			else if (strcmp(token, ".entry") == 0)
			{
				token = strtok(NULL, sep);
				if ((if_label(token, FALSE)) == TRUE)
				{
					add_to_list(&entry_list, token, 0, "entry", 0);
					entry_flage = TRUE;
				}
			}
			else /* the line contin with command */
			{
				if (label_flage == TRUE)
				{
					add_to_list(&label_list, label_name, 0, "code", IC);
					decode_command(token, sep, label_name, current_line_number);
				}
				else
				{
					decode_command(token, sep, NULL, current_line_number);
				}
			}
		}
	}

	ICF = IC;
	DCF = DC;
	/*change the address for data code to the correct address and update the address of the labels in list of labels*/
	update_address(&data_list, ICF, FALSE);
	update_address(&label_list, ICF, TRUE);
}

/*change the address for list of data to the correct address*/
void update_address(List **head, int offset, int label_flage)
{
	List *tempList;

	for (tempList = (*head); tempList; tempList = tempList->next)
	{
		if (label_flage == TRUE)
		{
			if (strcmp(tempList->type, "data") == TRUE)
			{
				/*update the address*/
				tempList->address = tempList->address + offset;
			}
		}
		else if (label_flage == FALSE)
		{
			/*update the address*/
			tempList->address = tempList->address + offset;
			/*mask out negative*/
			tempList->code = tempList->code & NEGATIV_MASK;
		}
	}
}

/*check if the label already exist*/
int if_label_exists(char *label, List *head, int externalFLAGE)
{
	List *tempList;

	for (tempList = (head); tempList; tempList = tempList->next)
	{
		if (strcmp(tempList->name, label) == FALSE)
		{
			if (externalFLAGE == TRUE)
			{
				if (strcmp(tempList->type, "exteral") == TRUE)
					return TRUE;
			}
			else
				return TRUE;
		}
	}

	return FALSE;
}
