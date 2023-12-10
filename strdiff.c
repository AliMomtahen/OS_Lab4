#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
   if(argc != 3){
    	printf(2, "Not enough words\n");
        exit();
    }
   if(strlen(argv[1]) > 15|| strlen(argv[2]) > 15) {
        printf(2, "The words are allowed up to 15 characters\n");
        exit();
    }

	int len1 = strlen(argv[1]);
	int len2 = strlen(argv[2]);
	int lenDiff = (len1>len2)?len1-len2:len2-len1;
	int minLen = (len1<len2)?len1:len2;
	
    if (unlink("strdiff_result.txt") > 0 )
    {
        printf(1,"error to unlink the file\n");
        exit();
    }

    int resultFile;
    resultFile = open("strdiff_result.txt",O_CREATE | O_WRONLY);

    for(int i = 0; i<minLen; i++){
    	if (argv[1][i] > 96)
    		argv[1][i] -= 32;
    	if (argv[2][i] > 96)
    		argv[2][i] -= 32;
    	

    	if (argv[1][i] < argv[2][i])
            write(resultFile,"1",1);
    	else
    		write(resultFile,"0",1);
    }
    
    char *lenCompareFlag = (len1<len2)? "1":"0";
    for(int i = 0; i<lenDiff; i++){
        write(resultFile,lenCompareFlag,1);
    }
    write(resultFile,"\n",1);
    close(resultFile);
    exit();
    
}

