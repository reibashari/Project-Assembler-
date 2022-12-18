#include "Assembler.h"

void decode_opcode(char *cmd, int *output, int current_line_number)
{
	if (strcmp(cmd, "cmp") == 0)
		*output = (*output | 1 << 6);
	else if (strcmp(cmd, "add") == 0)
		*output = (*output | 2 << 6);
	else if (strcmp(cmd, "sub") == 0)
		*output = (*output | 3 << 6);
	else if (strcmp(cmd, "not") == 0)
		*output = (*output | 4 << 6);
	else if (strcmp(cmd, "clr") == 0)
		*output = (*output | 5 << 6);
	else if (strcmp(cmd, "lea") == 0)
		*output = (*output | 6 << 6);
	else if (strcmp(cmd, "inc") == 0)
		*output = (*output | 7 << 6);
	else if (strcmp(cmd, "dec") == 0)
		*output = (*output | 8 << 6);
	else if (strcmp(cmd, "jmp") == 0)
		*output = (*output | 9 << 6);
	else if (strcmp(cmd, "bne") == 0)
		*output = (*output | 10 << 6);
	else if (strcmp(cmd, "get") == 0)
		*output = (*output | 11 << 6);
	else if (strcmp(cmd, "prn") == 0)
		*output = (*output | 12 << 6);
	else if (strcmp(cmd, "jsr") == 0)
		*output = (*output | 13 << 6);
	else if (strcmp(cmd, "rts") == 0)
		*output = (*output | 14 << 6);
	else if (strcmp(cmd, "hlt") == 0)
		*output = (*output | 15 << 6);
	else if (!(strcmp(cmd, "mov") == 0))
	{
		/* error: command not found */
		printf("in line %d not found command\n", current_line_number);
		syntax_error_flage = TRUE;
	}
}

void decode_immidiate(char *tok, int *output)
{
	char *code = malloc(strlen(tok) - 1);
	int number;

	strcpy(code, tok + 1);
	number = atoi(code);

	*output = (*output | (-((~number) + 1) & NEGATIV_MASK) << 2);
}

void decode_register(char *tok, char *type, int *output, int current_line_number)
{
	/*remove r before the number*/
	char *code = malloc(strlen(tok) - 1);
	int number;

	strcpy(code, tok + 1);
	number = atoi(code);

	if (number >= 0 && number <= 7)
	{
		if (strcmp(type, "source") == 0)
			/* put in bits 6-9*/
			*output = (*output | (number << 6));
		else if (strcmp(type, "destination") == 0)
			/* put in bits 2-5 */
			*output = (*output | (number << 2));
	}
	else
	{
		printf("in line %d illigal registr value\n", current_line_number);
		syntax_error_flage = TRUE;
	}
}

void decode_method(char *tok, char *type, int *output, int current_line_number)
{
	/* direct addressing*/
	if (get_method_number(tok) == Label)
	{
		if (strcmp(type, "source") == 0)
			/* put 01 in place 4-5 */
			*output = (*output | 1 << 4);
		else if (strcmp(type, "destination") == 0)
			/* put 01 in place 2-3  */
			*output = (*output | 1 << 2);
	}
	/* struct addressing*/
	else if (get_method_number(tok) == Struct)
	{
		if (strcmp(type, "source") == 0)
			/* put 10 in place 4-5 */
			*output = (*output | 2 << 4);
		else if (strcmp(type, "destination") == 0)
			/* put 10 in place 2-3 */
			*output = (*output | 2 << 2);
	}
	/* register addressing*/
	else if (get_method_number(tok) == Register)
	{
		if (strcmp(type, "source") == 0)
			/* put 11 in place 4-5 */
			*output = (*output | 3 << 4);
		else if (strcmp(type, "destination") == 0)
			/* put 11 in place 2-3  */
			*output = (*output | 3 << 2);
	}
	else if (get_method_number(tok) != Immidiate)
	{
		/* if not Immidiate, Label, Struct, Register */
		printf("in line %d illigal method %s\n", current_line_number, tok);
		syntax_error_flage = TRUE;
	}
}

void decode_command(char *token, char *sep, char *label_name, int current_line_number)
{

	char *arg1 = NULL;
	char *arg2 = NULL;
	char *command = token;
	token = strtok(NULL, sep);
	if (token != NULL)
	{
		arg1 = token;
	}
	token = strtok(NULL, sep);
	if (token != NULL)
	{
		arg2 = token;
	}
	token = strtok(NULL, sep);
	if (token != NULL)
	{
		/* too mach operands */
		printf("in line %d too mach operands\n", current_line_number);
		syntax_error_flage = TRUE;
	}
	/*if two operands*/
	else if (arg2 != NULL)
	{
		decode_two_Operands(command, token, label_name, arg1, arg2, current_line_number);
	}
	/*if one operand*/
	else if (arg1 != NULL)
	{
		decode_one_operand(command, token, label_name, arg1, current_line_number);
	}
	/*if zero operands */
	else
	{
		decode_no_operands(command, token, current_line_number);
	}
}

void decode_no_operands(char *command, char *token, int current_line_number)
{
	int output1 = 0;
	char *type = "a";
	/*first word - opcode*/
	decode_opcode(command, &output1, current_line_number);
	/*if command is not rts or hlt, there are too few operands*/
	if (if_no_operands(command) == FALSE)
	{
		printf("in line %d need more operands for %s\n", current_line_number, command);
		syntax_error_flage = TRUE;
	}
	else
	{
		add_to_list(&code_list, "", output1, type, IC);
		++IC;
	}
}

void decode_one_operand(char *command, char *token, char *label_name, char *arg1, int current_line_number)
{
	int output1 = 0;
	int output2 = 0;
	int output3 = 0;
	char *type = "a";
	/*first word - opcode*/
	decode_opcode(command, &output1, current_line_number);
	if (if_no_operands(command) == TRUE)
	{
		/* error: too many operands, needs to be zero operands */
		printf("in line %d too many operands\n", current_line_number);
		syntax_error_flage = TRUE;
	}

	else if (if_two_operands(command) == TRUE)
	{
		/* error: too few operands, needs to be two operands */
		printf("in line %d need more operands for\n", current_line_number);
		syntax_error_flage = TRUE;
	}
	else if (!(strcmp(command, "prn") == 0) && get_method_number(arg1) == Immidiate)
	{
		/* error: incorrect method, only prn can be with method 0*/
		printf("in line %d incorrect operand %s for %s\n", current_line_number, arg1, command);
		syntax_error_flage = TRUE;
	}
	else
	{
		decode_method(arg1, "destination", &output1, current_line_number);
		add_to_list(&code_list, "", output1, type, IC);
		IC++;

		if (get_method_number(arg1) == Register)
		{
			decode_register(arg1, "destination", &output2, current_line_number);
			add_to_list(&code_list, "", output2, type, IC);
			IC++;
		}
		else if (get_method_number(arg1) == Immidiate)
		{
			decode_immidiate(arg1, &output2);
			add_to_list(&code_list, "", output2, type, IC);
			IC++;
		}
		else if (get_method_number(arg1) == Label)
		{
			label_name = malloc(sizeof(arg1));
			strcpy(label_name, arg1);
			add_to_list(&code_list, label_name, output2, type, IC);
			IC++;
		}
		else if (get_method_number(arg1) == Struct)
		{
			label_name = malloc(sizeof(arg1));
			strcpy(label_name, get_structs_label(arg1));
			add_to_list(&code_list, label_name, output2, type, IC);
			IC++;
			add_to_list(&code_list, "", output3 | get_structs_number(arg1) << 2, type, IC);
			IC++;
		}
	}
}

void decode_two_Operands(char *command, char *token, char *label_name, char *arg1, char *arg2, int current_line_number)
{
	int output1 = 0;
	int output2 = 0;
	int output3 = 0;
	int output4 = 0;
	int output5 = 0;
	char *type = "a";

	decode_opcode(command, &output1, current_line_number);

	if (if_two_operands(command) == FALSE)
	{
		printf("in line %d too many operands\n", current_line_number);
		syntax_error_flage = TRUE;
	}
	else
	{
		if ((strcmp(command, "lea") == 0) && (get_method_number(arg1) == Immidiate || get_method_number(arg1) == Register))
		{
			printf("in line %d incorrect operand %s not legal with %s\n", current_line_number, command, arg1);
			syntax_error_flage = TRUE;
		}
		else if (!(strcmp(command, "cmp") == 0) && get_method_number(arg2) == Immidiate)
		{
			/* dest methon 0 is legal only in cmp command*/
			printf("in line %d incorrect operand %s not legal with %s\n", current_line_number, command, arg2);
			syntax_error_flage = TRUE;
		}
		else
		{
			decode_method(arg1, "source", &output1, current_line_number);
			decode_method(arg2, "destination", &output1, current_line_number);
			add_to_list(&code_list, "", output1, type, IC);
			IC++;
			/*add extra words for arg1*/
			if (get_method_number(arg1) == Register)
			{
				decode_register(arg1, "source", &output2, current_line_number);
				if (get_method_number(arg2) == Register)
					decode_register(arg2, "destination", &output2, current_line_number);
				add_to_list(&code_list, "", output2, type, IC);
				IC++;
			}
			else if (get_method_number(arg1) == Immidiate)
			{
				decode_immidiate(arg1, &output2);
				add_to_list(&code_list, "", output2, type, IC);
				IC++;
			}
			else if (get_method_number(arg1) == Label)
			{
				label_name = malloc(sizeof(arg1));
				strcpy(label_name, arg1);
				add_to_list(&code_list, label_name, output2, type, IC);
				IC++;
			}
			else if (get_method_number(arg1) == Struct)
			{
				label_name = malloc(sizeof(arg1));
				strcpy(label_name, get_structs_label(arg1));
				add_to_list(&code_list, label_name, output2, type, IC);
				IC++;
				add_to_list(&code_list, "", output3 | get_structs_number(arg1) << 2, type, IC);
				IC++;
			}

			if (get_method_number(arg2) == Register)
			{
				if (get_method_number(arg1) != Register)
				{
					decode_register(arg2, "destination", &output4, current_line_number);
					add_to_list(&code_list, "", output4, type, IC);
					IC++;
				}
			}
			else if (get_method_number(arg2) == Immidiate)
			{
				decode_immidiate(arg2, &output4);
				add_to_list(&code_list, "", output4, type, IC);
				IC++;
			}
			else if (get_method_number(arg2) == Label)
			{
				label_name = malloc(sizeof(arg2));
				strcpy(label_name, arg2);
				add_to_list(&code_list, label_name, output4, type, IC);
				IC++;
			}
			else if (get_method_number(arg2) == Struct)
			{
				label_name = malloc(sizeof(arg2));
				strcpy(label_name, get_structs_label(arg2));
				add_to_list(&code_list, label_name, output4, type, IC);
				IC++;
				add_to_list(&code_list, "", output5 | get_structs_number(arg2) << 2, type, IC);
				IC++;
			}
		}
	}
}
