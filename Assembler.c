#include "Assembler.h"

int main(int num_of_args, char *args[])
{

	FILE *am_file;
	int i;
	char as_file_name[SIZE_OF_NAME];
	char *am_file_name;

	/* one arge for command and at least one file name */
	if (num_of_args < 2)
	{
		printf("no file name given\n");
		return 1;
	}
	else
	{
		for (i = 1; i < num_of_args; i++)
		{
			label_list = NULL;
			code_list = NULL;
			DC = 0;
			IC = 100;
			entry_flage = FALSE;
			extern_flage = FALSE;
			syntax_error_flage = FALSE;

			strcpy(as_file_name, args[i]);
			strcat(as_file_name, ".as");
			am_file_name = macros_run(as_file_name);

			am_file = fopen(am_file_name, "r");
			if (am_file == NULL)
			{
				printf("file %s not open\n", am_file_name);
			}
			else
			{
				update_lists(am_file);
				if (syntax_error_flage == 0)
				{
					am_file_name[strlen(am_file_name) - 3] = 0;
					update_labels(am_file_name);
					print_to_files(am_file_name);
					printf("file %s done!\n", am_file_name);
				}
				fclose(am_file);
			}
		}

		return 0;
	}
}
