#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

char hasil[10001];
int len;

void expand(int index)
{
	char temp[10001];
	temp[0] = 0;

	while(hasil[index] != 'a' && hasil[index] != 'b')
		index = (index+1) % len;

	int operat = rand() % 1;//rand
	bool order = rand() % 2;//rand

	switch(operat)
	{
	case 0: //concat
		if (order)
			strcat(temp, "(ab)");
		else
			strcat(temp, "(ba)");
		strcat(temp, hasil+index+1);
		hasil[index] = 0;
		strcat(hasil, temp);

		break;
	case 1: //or
		if (order)
			strcat(temp, "(a|b)");
		else
			strcat(temp, "(b|a)");
		strcat(temp, hasil+index+1);
		hasil[index] = 0;
		strcat(hasil, temp);

		break;
	case 2: //star
		if (order)
			strcat(temp, "(a*)");
		else
			strcat(temp, "(b*)");
		strcat(temp, hasil+index+1);
		hasil[index] = 0;
		strcat(hasil, temp);
		break;
	}
}

int main(int argc, char *args[])
{
//    printf("%s\n", args[0]);
	srand(time(NULL));
	int i=1;
	int re[] = {10,20,30,40,50,60,70,80,90,100};
	int L[] = {10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
    int s = 9;
    sscanf(args[1], "%d", &s);
    int t = 1;
	printf("%d\n", 1);
	while(i--)
	{
		hasil[0] = 0;
		strcat(hasil, "a");

		while((len = strlen(hasil)) < re[s])
			expand(rand() % len);
//        for(int j=0;j<t;j++)
            printf("%s %d\n", hasil, L[8]);
	}
}
