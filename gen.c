#include<stdio.h>
#include<time.h>

int main()
{
    char car[] = {'a', 'b'};
    char op[] = {'+', '|', '*'};
    char cur;
    int tmp, i;
    srand(time(NULL));
    for(i=0;i<101;i++)
    {
        if(!(i&1))
        {
            tmp = rand()%2;
            cur = car[tmp];
        }
        else
        {
            tmp = rand()%3;
            cur = op[tmp];
            if(cur=='*') --i;
        }
        printf("%c", cur);
    }
    puts("");

    return 0;
}
