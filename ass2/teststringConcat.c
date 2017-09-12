#include <stdio.h>
#include <string.h>

int main(){
	char str[256]= "\0" ;
	str[0] = 'A';
	str[1] = 'B';
	char str2[256] = "\0";
	str2[0] = 'C';
	strcat(str, str2);
	printf("%d\n",sizeof(str));
	printf("%s\n",str);

	if(1){
		char str[256] = "\0";
		str[0]='Z';
		printf("%s\n",str);
		printf("%d\n",sizeof(str));
	}
}


