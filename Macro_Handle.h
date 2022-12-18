
typedef struct macro_line
{
	char name[SIZE_OF_NAME];
	char text[SIZE_OF_LINE];
	struct macro_line *next;

} macro_line;

char *handel_macro(char *as_file_name);
int if_macro(macro_line *head, char *macro_name);
int if_name_free(macro_line *head, char *corrent_macro_name);
void add_to_macro_list(macro_line *head, macro_line *new);