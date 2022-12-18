Assembler:Assembler.o Macro_Handle.o Update_Label.o Update_Lists.o Print_To_File.o Decode.o Methods.o 
	gcc -g -fcommon -ansi -Wall Macro_Handle.o Print_To_File.o Update_Lists.o Decode.o Methods.o Update_Label.o Assembler.o -o  Assembler -lm
Assembler.o: Assembler.c Assembler.h
	gcc -c -fcommon -ansi -Wall -pedantic Assembler.c -o Assembler.o
Macro_Handle.o: Macro_Handle.c Assembler.h Macro_Handle.h
	gcc -c -fcommon -ansi -Wall -pedantic Macro_Handle.c -o Macro_Handle.o 
Update_Lists.o: Update_Lists.c Assembler.h 
	gcc -c -fcommon -ansi -Wall -pedantic Update_Lists.c -o Update_Lists.o 
Update_Label.o: Update_Label.c Assembler.h 
	gcc -c -fcommon -ansi -Wall -pedantic Update_Label.c -o Update_Label.o 
Methods.o: Methods.c Assembler.h 
	gcc -c -fcommon -ansi -Wall -pedantic Methods.c -o Methods.o 
Decode.o: Decode.c Assembler.h 
	gcc -c -fcommon -ansi -Wall -pedantic Decode.c -o Decode.o 
Print_To_File.o: Print_To_File.c Assembler.h
	gcc -c -fcommon -ansi -Wall -pedantic Print_To_File.c -o Print_To_File.o 
