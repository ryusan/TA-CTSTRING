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

	int operat = rand() % 3;//rand
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

int main()
{
	srand(time(NULL));
	int i=1;
	int re[] = {40,105,170};
	int L[] = {999999993,99999989,9999991};
    int s = 1;
    int t = 0;
	printf("%d\n", i);
	while(i--)
	{
		hasil[0] = 0;
		strcat(hasil, "a");

		while((len = strlen(hasil)) < re[s])
			expand(rand() % len);

		printf("%s %d\n", hasil, L[t]);
	}
}
