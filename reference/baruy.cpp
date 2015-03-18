#include <cstdio>
#include <ctime>
#include <algorithm>
#include <map>
using namespace std;

#define pii per<int,int>
#define sint set<int>
#define sset set<sint>
#define MOD 1000000007

void printTime(char *a)
{
	static clock_t begin = clock();
	clock_t end = clock();
	float timesec = ((float)end - (float)begin) / CLOCKS_PER_SEC;
    printf("elapsed time = %f %s\n", timesec, a);
    begin = clock();
}

template <typename T>
struct stack
{
    T elem[100];
    int size;

    stack()
    {
        size = 0;
    }

    void push(T data)
    {
        elem[size++] = data;
    }

    void pop()
    {
        size--;
    }

    T top()
    {
        return elem[size-1];
    }

    void clear()
    {
    	size = 0;
    }
};

template <typename T, typename S>
struct per
{
    T a;
    S b;

    per()
    {

    }

    per(T _a, S _b)
    {
        a = _a;
        b = _b;
    }
};

template <typename T>
struct set
{
    T elem[100];
    int size;

    set()
    {
        size = 0;
    }

    void add(T data)
    {
        if (exist(data))
            return;

        elem[size++] = data;
        sort(elem, elem+size);
    }

    bool exist(T data)
    {
        int l=0, r=size, mid;
        while (r>l)
        {
            mid = (l+r)>>1;
            if (elem[mid] == data)
                return true;
            if (elem[mid] < data)
                l = mid+1;
            else
                r = mid;
        }
        return false;
    }

    bool operator<(const set& r) const
    {
        if (size < r.size)
            return true;
        if (size > r.size)
            return false;
        for (int i=0; i<size; i++)
        {
            if (elem[i] < r.elem[i])
                return true;
            if (!(elem[i] == r.elem[i]))
                return false;
        }
        return false;
    }

    bool operator==(const set& r) const
    {
        if (size != r.size)
            return false;
        for (int i=0; i<size; i++)
            if (!(elem[i] == r.elem[i]))
                return false;
        return true;
    }

    void clear()
    {
        size = 0;
    }
};

pii nfa[100][2];
bool nfasize[100];
int nfastart = -1;
int nfaend = -1;
int nfanow = 0;
stack<pii> operan;
stack<char> operat;

int dfa[100][2];
int dfanow = 0;
map<sint, int> dfanodemap;
sint dfanodemap2[100];

long long adjmat[100][100], res[100][100], hasil;

void reset()
{
	for (int i=0; i<100; i++)
	{
		nfasize[i] = 0;
		dfanodemap2[i].clear();
		for (int j=0; j<2; j++)
		{
			nfa[i][j] = pii(0,0);
			dfa[i][j] = 0;
		}
		for (int j=0; j<100; j++)
			adjmat[i][j] = 0;
	}
	nfastart = nfaend = -1;
	nfanow = dfanow = 0;
	operan.clear();
	operat.clear();
	dfanodemap.clear();
}

void join()
{
    pii operan1, operan2;

    switch(operat.top())
    {
    case '(': //concat
        operan2 = operan.top();
        operan.pop();
        operan1 = operan.top();
        operan.pop();

        nfa[operan1.b][0] = nfa[operan2.a][0];
        if (nfasize[operan2.a])
            nfa[operan1.b][1] = nfa[operan2.a][1];
        nfasize[operan1.b] = nfasize[operan2.a];

        operan1.b = operan2.b;
        operan.push(operan1);

        nfastart = operan1.a;
        nfaend = operan1.b;
        break;

    case '|': //or
        operan2 = operan.top();
        operan.pop();
        operan1 = operan.top();
        operan.pop();

        nfa[nfanow][0] = pii(operan1.a, 2);
        nfa[nfanow][1] = pii(operan2.a, 2);
        nfasize[nfanow] = 1;

        nfa[operan1.b][0] = pii(nfanow+1, 2);
        nfa[operan2.b][0] = pii(nfanow+1, 2);
        nfasize[operan1.b] = nfasize[operan2.b] = 0;

        nfastart = operan1.a = nfanow;
        nfaend = operan1.b = nfanow+1;
        operan.push(operan1);

        nfanow += 2;
        operat.pop();
        break;

    case '*': //star
        operan1 = operan.top();
        operan.pop();

        nfa[nfanow][0] = nfa[operan1.b][0] = pii(operan1.a, 2);
        nfa[nfanow][1] = nfa[operan1.b][1] = pii(nfanow+1, 2);
        nfasize[nfanow] = nfasize[operan1.b] = 1;

        nfastart = operan1.a = nfanow;
        nfaend = operan1.b = nfanow+1;
        operan.push(operan1);

        nfanow += 2;
        operat.pop();
        break;
    }
    operat.pop();
}

void move(sint& nodes, int input, sint& retval)
{
    retval.clear();
    for (int i=0; i<nodes.size; i++)
    {
        if (nodes.elem[i] == nfaend)
            continue;

        if (nfa[nodes.elem[i]][0].b == input)
            retval.add(nfa[nodes.elem[i]][0].a);
        else if(nfasize[nodes.elem[i]] && nfa[nodes.elem[i]][1].b == input)
            retval.add(nfa[nodes.elem[i]][1].a);
    }
}

void epsmove(sint& nodes, sint& retval)
{
    retval.clear();

    stack<int> stek;
    int now;

    for (int i=0; i<nodes.size; i++)
        stek.push(nodes.elem[i]);
    retval = nodes;

    while (stek.size)
    {
        now = stek.top();
        stek.pop();
        if (now == nfaend)
            continue;

        if (nfa[now][0].b == 2 && !retval.exist(nfa[now][0].a))
        {
            retval.add(nfa[now][0].a);
            stek.push(nfa[now][0].a);
        }
        if (nfasize[now] && nfa[now][1].b == 2 && !retval.exist(nfa[now][1].a))
        {
            retval.add(nfa[now][1].a);
            stek.push(nfa[now][1].a);
        }
    }
}

void matmul(long long x[][100], long long y[][100])
{
	long long z[100][100];

	for(int a=0; a<dfanow; a++)
        for(int b=0; b<dfanow; b++)
		{
			z[a][b] = 0;
            for(int c=0; c<dfanow; c++)
                z[a][b] = (z[a][b]+((x[a][c])*(y[c][b]))) % MOD;
		}

	for(int a=0; a<dfanow; a++)
        for(int b=0; b<dfanow; b++)
			x[a][b] = z[a][b];
}

void matexp(long long awal[][100], int exp, long long x[][100])
{
    for(int i=0; i<dfanow; i++)
        for(int j=0; j<dfanow; j++)
            x[i][j] = (i==j);

    while(exp)
    {
        if (exp & 1)
            matmul(x, awal);

        matmul(awal, awal);
        exp >>= 1;
    }
}

int main()
{
	printTime("begin");

    int t, tt;
    char c;

    scanf("%d%c", &t, &c);

    while(t--)
    {
    	reset();
        while(scanf("%c", &c), (c!=' ' && c!='\n'))
        {
            switch(c)
            {
            case 'a':
            case 'b':
                nfa[nfanow][0] = pii(nfanow+1, c-'a');
                nfasize[nfanow] = 0;
                operan.push(pii(nfanow, nfanow+1));
                nfanow += 2;
                break;

            case '(':
            case '|':
            case '*':
                operat.push(c);
                break;

            case ')':
                join();
                break;
            }
        }
        scanf("%d%c", &tt, &c);

        stack<int> stek;
        sint dfainit, dfamove, dfaeps;
        int nowtemp;
        map<sint, int>::iterator it;

        dfainit.add(nfastart);
        epsmove(dfainit, dfaeps);
        dfanodemap[dfaeps] = dfanow;
        dfanodemap2[dfanow] = dfaeps;
        stek.push(dfanow++);

        while(stek.size)
        {
            nowtemp = stek.top();
            stek.pop();

            for (int i=0; i<2; i++)
            {
                move(dfanodemap2[nowtemp], i, dfamove);
                epsmove(dfamove, dfaeps);
                if (dfaeps.size)
                {
                    it = dfanodemap.find(dfaeps);
                    if (it == dfanodemap.end())
                    {
                        dfanodemap[dfaeps] = dfanow;
                        dfanodemap2[dfanow] = dfaeps;
                        stek.push(dfanow);
                        dfa[nowtemp][i] = dfanow;
                        adjmat[nowtemp][dfanow++] += 1;
                    }
                    else
					{
                        dfa[nowtemp][i] = it->second;
                        adjmat[nowtemp][it->second] += 1;
					}
                }
                else
                    dfa[nowtemp][i] = -1;
            }
        }


        matexp(adjmat, tt, res);
        hasil = 0;

        for (int i=0; i<dfanow; i++)
			if (dfanodemap2[i].exist(nfaend))
				hasil += res[0][i];

		printf("%I64d\n", hasil % MOD);
    }
	printTime("end");
}
