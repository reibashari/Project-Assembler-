#include "Assembler.h"

int if_no_operands(char *command)
{
	if ((strcmp(command, "rts") == 0) || (strcmp(command, "hlt") == 0))
		return TRUE;
	return FALSE;
}

int if_two_operands(char *command)
{
	if ((strcmp(command, "mov") == 0) ||
		 (strcmp(command, "add") == 0) ||
		 (strcmp(command, "sub") == 0) ||
		 (strcmp(command, "lea") == 0) ||
		 (strcmp(command, "cmp") == 0))
		return TRUE;
	return FALSE;
}

int if_struct(char *tok)
{
	char *token;
	char *tokCopy = malloc(sizeof(tok));
	char *str_tol_end;
	int str_tol_int;

	/* copy to save original tok */
	strcpy(tokCopy, tok);
	token = strtok(tokCopy, ".");

	/* if is a legal label, check the number*/
	if (if_label(token, FALSE))
	{
		token = strtok(NULL, " \t\n");
		if (token != NULL)
		{
			str_tol_int = strtol(token, &str_tol_end, SIZE_OF_WORD);
			if (str_tol_int == 1 || str_tol_int == 2)
				return TRUE;
		}
	}
	return FALSE;
}

int if_immidiate(char *tok)
{
	char *number;
	number = malloc(strlen(tok) - 1);
	strcpy(number, tok + 1);
	if (tok[0] == '#' && if_number(number) == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

int if_number(char *tok)
{
	int length;
	int i;
	length = strlen(tok);
	if (tok[0] != '-' && isdigit(tok[0]) == FALSE)
		return FALSE;
	for (i = 1; i < length; i++)
	{
		if (isdigit(tok[i]) == FALSE)
			return FALSE;
	}
	return TRUE;
}

int if_register(char *tok)
{
	char *number;
	int regNumber;
	/* if starts with r*/
	if (tok[0] == 'r')
	{
		number = malloc(strlen(tok) - 1);
		strcpy(number, tok + 1);
		regNumber = atoi(number);
		/* number is 0-7 */
		if (regNumber >= 0 && regNumber <= 7)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int if_label(char *tok, int decleration)
{
	int length = strlen(tok);
	int i;

	/* label must start with alphabet char*/
	if ((isalpha(tok[0])))
	{
		if (decleration)
		{
			length = length - 1;
		}
		for (i = 0; i < length; i++)
		{
			/* label must not contain a non alphanumeric character*/
			if ((isalpha(tok[i])) == FALSE && (isdigit(tok[i])) == FALSE)
				return FALSE;
		}
		/*if decleration is 1 but no colon, then it is extern and not label*/
		if ((decleration == TRUE && tok[length] == ':') || decleration == FALSE)
		{
			return TRUE;
		}
	}

	return FALSE;
}

int get_structs_number(char *tok)
{
	char *token;
	char *tokCopy = malloc(sizeof(tok));
	char *str_tol_end;
	int str_tol_int;

	/* copy to save original value */
	strcpy(tokCopy, tok);
	/* move to the number after the . */
	token = strtok(tokCopy, ".");
	token = strtok(NULL, " \t\n");
	str_tol_int = strtol(token, &str_tol_end, SIZE_OF_WORD);
	return str_tol_int;
}

char *get_structs_label(char *tok)
{
	char *token;
	char *tokCopy = malloc(sizeof(tok));
	/* copy to save original tok */
	strcpy(tokCopy, tok);
	token = strtok(tokCopy, ".");
	return token;
}

int get_method_number(char *tok)
{
	if (if_immidiate(tok) == TRUE)
	{
		return Immidiate;
	}
	else if (if_struct(tok) == TRUE)
	{
		return Struct;
	}
	else if (if_register(tok) == TRUE)
	{
		return Register;
	}
	else if (if_label(tok, FALSE))
	{
		return Label;
	}

	return -1;
}

/*add to list a new node */
void add_to_list(List **head, char *label, int number, char *type, int address)
{
	List *tempNode = (List *)malloc(sizeof(List));
	List *temp_list1;
	List *temp_list2;

	if (!tempNode)
	{
		/* error: can not allocate memory */
		printf("malloc error");
		exit(0);
	}
	/*if it external/label/code*/
	if (type != NULL)
	{
		strcpy(tempNode->type, type);
	}
	/*if has a label*/
	if (label != NULL)
	{
		strcpy(tempNode->name, label);
	}
	tempNode->code = number;
	if ((type != NULL) && (strcmp(type, "e") == FALSE))
	{
		/*if it external the address is 0*/
		tempNode->address = 0;
	}
	else
	{
		tempNode->address = address;
	}

	/* get last in list */
	for (temp_list1 = (*head); temp_list1; temp_list1 = temp_list1->next)
	{
		/* save one before last*/
		temp_list2 = temp_list1;
	}
	/*if the node is the first node in the list*/
	if (temp_list1 == (*head))
	{
		(*head) = tempNode;
		tempNode->next = temp_list1;
	}
	else
	{
		/*add to end of list*/
		temp_list2->next = tempNode;
		tempNode->next = temp_list1;
	}
}
