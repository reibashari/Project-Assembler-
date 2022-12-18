/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/*defines*/
#define TRUE 1
#define FALSE 0
#define SIZE_OF_WORD 10
#define SIZE_OF_LINE 81
#define SIZE_OF_NAME 30
#define BASE32 32
#define NEGATIV_MASK 1023
#define IMMI_ORIENTATION 0
#define LABEL_ORIENTATION 1
#define STRUCT_ORIENTATION 2
#define REGISTER_ORIENTATION 3
#define DATA 1
#define STRING 2
#define STRUCT 3
#define LABEL_EXIST_EXCEPTION 1
#define LABEL_SYNTAX_EXCEPTION 2
#define Immidiate 0
#define Label 1
#define Struct 2
#define Register 3

/*the list structer*/
typedef struct List
{
	char name[SIZE_OF_NAME];
	char type[SIZE_OF_NAME];
	unsigned int address;
	int code;
	struct List *next;
} List;

/*global parameters*/
int DC;
int IC;
int ICF;
int DCF;
List *code_list;
List *data_list;
List *label_list;
List *entry_list;
List *extern_list;
int current_line_number;
int syntax_error_flage;
int entry_flage;
int extern_flage;

/* assembler */
int main(int argc, char *argv[]);

/*macrosRun*/
char *macros_run(char *as_file_name);

/* update_lists */
void update_lists(FILE *file);
void update_address(List **head, int offset, int label_list);
int if_label_exists(char *label, List *head_label, int external);

/* update_labels */
void update_labels(char *file);
void update_label_address(List **head, List *head_label);
void update_extern_address(List **head, List *head_external);

/* print_lists */
void print_to_files(char *file);
void print_to_ob_file(char *file);
void print_to_ext(char *file, List *head, char *type);
void print_to_ent(char *file, List *head, List *head_entry);
void print_list(List *head, FILE *output_file);

/* decode */
void decode_opcode(char *cmd, int *output, int line_number);
void decode_register(char *val, char type[], int *output, int line_number);
void decode_method(char *val, char *type, int *output, int line_number);
void decode_immidiate(char *val, int *output);
void decode_command(char *current_token, char *sep, char *label_name, int line_number);
void decode_no_operands(char *command, char *token, int line_number);
void decode_one_operand(char *command, char *token, char *label_name, char *arg1, int line_number);
void decode_two_Operands(char *command, char *token, char *label_name, char *arg1, char *arg2, int line_number);

/* methods */
int if_no_operands(char *command);
int if_two_operands(char *command);
int if_label(char val[], int if_with_colon);
int if_register(char *val);
int if_immidiate(char *val);
int if_number(char *val);
int if_struct(char *val);
int get_method_number(char *val);
int get_structs_number(char *val);
char *get_structs_label(char *val);
void add_to_list(List **head, char *label, int number, char *val, int adrress);
void free_list(List **head);
