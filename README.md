# cat_program
reimplementation_of_standart_Linux_cat_command

all standard functions of cat command will be reimplemented with
an additional feature of highlighting lines with colour.

User can choose wich lines should be hightlighted, e.g.
every 2nd line, every 3d and so on. 

Flag can look like -c[lines]: 

-c - default option, stays for every 2nd line to be colored ( the same as -c2 )

-c3 - every 3d line to be colored
	
-A - display all
-n - number all output lines
-N - display file name before outtputting it's contents
-b - number non-blanl lines
-e - equivalent to -vE
-E - display $ at end of each line
-v - show non-printing characters
-s - suppress repeated empty output lines
-t - equivalent to -vT
-T - display TAB characters as ^I
-c - colout every second line, counting from the 1st line
	

