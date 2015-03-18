#include <cstdio>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <map>
using namespace std;

#define pii per<int,int>
#define MOD 1000000007ll

float timea, timeb, timec;
float counttime()
{
    static clock_t begin = clock();
	clock_t end = clock();
	float timesec = ((float)end - (float)begin) / CLOCKS_PER_SEC;
    begin = clock();
    return timesec;
}

void printtime(char *a)
{
    printf("elapsed time = %f %s\n", counttime(), a);
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

struct sint
{
    int elem[100];
    int size;

    sint()
    {
        size = 0;
    }

    void add(int data)
    {
        if (exist(data))
            return;

        elem[size++] = data;
        sort(elem, elem+size);
    }

    void add(sint data)
    {
        int index1, index2, counter;
        index1 = index2 = counter = 0;

        while (index2 < data.size)
        {
            while (index1 < size && elem[index1] < data.elem[index2])
                index1++;
            if (index1 == size || elem[index1] > data.elem[index2])
                elem[size+counter++] = data.elem[index2];
            index2++;
        }
        if (counter)
        {
            size += counter;
            sort(elem, elem+size);
        }
    }

    bool exist(int data)
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

    bool operator<(const sint& r) const
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

    bool operator==(const sint& r) const
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
bool isbranch[100];
int nfastart = -1;
int nfaend = -1;
int nfasize = 0;
stack<pii> operan;
stack<char> operat;

int dfa[100][2];
int dfasize = 0;
map<sint, int> dfanodemap;
sint dfanodemap2[100];
sint eps[100];
bool dfsmark[100];
bool markeps[100];

long long adjmat[100][100], res[100][100], hasil;

void reset()
{
    memset(isbranch, 0, 100);
    memset(markeps, 0, 100);
    memset(dfsmark, 0, 100);
    memset(nfa, 0, 1600);
    memset(dfa, 0, 800);
    memset(adjmat, 0, 80000);

	for (int i=0; i<100; i++)\
		dfanodemap2[i].clear();

	nfastart = nfaend = -1;
	nfasize = dfasize = 0;
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
        if (isbranch[operan2.a])
            nfa[operan1.b][1] = nfa[operan2.a][1];
        isbranch[operan1.b] = isbranch[operan2.a];

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

        nfa[nfasize][0] = pii(operan1.a, 2);
        nfa[nfasize][1] = pii(operan2.a, 2);
        isbranch[nfasize] = 1;

        nfa[operan1.b][0] = pii(nfasize+1, 2);
        nfa[operan2.b][0] = pii(nfasize+1, 2);
        isbranch[operan1.b] = isbranch[operan2.b] = 0;

        nfastart = operan1.a = nfasize;
        nfaend = operan1.b = nfasize+1;
        operan.push(operan1);

        nfasize += 2;
        operat.pop();
        break;

    case '*': //star
        operan1 = operan.top();
        operan.pop();

        nfa[nfasize][0] = nfa[operan1.b][0] = pii(operan1.a, 2);
        nfa[nfasize][1] = nfa[operan1.b][1] = pii(nfasize+1, 2);
        isbranch[nfasize] = isbranch[operan1.b] = 1;

        nfastart = operan1.a = nfasize;
        nfaend = operan1.b = nfasize+1;
        operan.push(operan1);

        nfasize += 2;
        operat.pop();
        break;
    }
    operat.pop();
}

void buildeps(int ori)
{
    stack<int> stek;
    int now;

    eps[ori].clear();
    eps[ori].add(ori);
    stek.push(ori);

    while (stek.size)
    {
        now = stek.top();
        stek.pop();
        if (now == nfaend)
            continue;

        if (nfa[now][0].b == 2 && !(eps[ori].exist(nfa[now][0].a)))
        {
            if (markeps[nfa[now][0].a])
                eps[ori].add(eps[nfa[now][0].a]);
            else
            {
                eps[ori].add(nfa[now][0].a);
                stek.push(nfa[now][0].a);
            }
        }
        if (isbranch[now] && nfa[now][1].b == 2 && !(eps[ori].exist(nfa[now][1].a)))
        {
            if (markeps[nfa[now][1].a])
                eps[ori].add(eps[nfa[now][1].a]);
            else
            {
                eps[ori].add(nfa[now][1].a);
                stek.push(nfa[now][1].a);
            }
        }
    }
    markeps[ori] = 1;
}

void dfsbuild(int now)
{
    if (dfsmark[now])
        return;
    dfsmark[now] = 1;

    if (now == nfaend)
    {
        eps[now].add(now);
        markeps[now] = 1;
        return;
    }

    dfsbuild(nfa[now][0].a);
    if (isbranch[now])
        dfsbuild(nfa[now][1].a);
    buildeps(now);
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
        else if(isbranch[nodes.elem[i]] && nfa[nodes.elem[i]][1].b == input)
            retval.add(nfa[nodes.elem[i]][1].a);
    }
}

void epsmove(sint& nodes, sint& retval)
{
    retval.clear();

//    stack<int> stek;
//    int now;

    retval = nodes;
    for (int i=0; i<nodes.size; i++)
        retval.add(eps[nodes.elem[i]]);
//    retval = nodes;

//    while (stek.size)
//    {
//        now = stek.top();
//        stek.pop();
//        if (now == nfaend)
//            continue;
//
//        if (nfa[now][0].b == 2 && !retval.exist(nfa[now][0].a))
//        {
//            retval.add(nfa[now][0].a);
//            stek.push(nfa[now][0].a);
//        }
//        if (isbranch[now] && nfa[now][1].b == 2 && !retval.exist(nfa[now][1].a))
//        {
//            retval.add(nfa[now][1].a);
//            stek.push(nfa[now][1].a);
//        }
//    }
}

void matmul(long long x[][100], long long y[][100])
{
	long long z[100][100];

	for(int a=0; a<dfasize; a++)
        for(int b=0; b<dfasize; b++)
		{
			z[a][b] = 0;
            for(int c=0; c<dfasize; c++)
                z[a][b] = (z[a][b]+((x[a][c])*(y[c][b]))) % MOD;
		}

	for(int a=0; a<dfasize; a++)
        for(int b=0; b<dfasize; b++)
			x[a][b] = z[a][b];
}

void matexp(long long awal[][100], int exp, long long x[][100])
{
    for(int i=0; i<dfasize; i++)
        for(int j=0; j<dfasize; j++)
            x[i][j] = (i==j);

    while(exp)
    {
        if (exp & 1)
            matmul(x, awal);

        matmul(awal, awal);
        exp >>= 1;
    }
}

bool nfamark[100];
void printnfa(int now)
{
    if (nfamark[now] || now == nfaend)
        return;
    nfamark[now] = 1;

    printf("%d -> %d: %d\n", now, nfa[now][0].a, nfa[now][0].b);
    printnfa(nfa[now][0].a);
    if (isbranch[now])
    {
        printf("%d -> %d: %d\n", now, nfa[now][1].a, nfa[now][1].b);
        printnfa(nfa[now][1].a);
    }
}

bool dfamark[100];
void printdfa(int now)
{
    if (dfamark[now])
        return;
    dfamark[now] = 1;

    for (int i=0; i<2; i++)
        if (dfa[now][i] != -1)
        {
            printf("%d -> %d: %c\n", now, dfa[now][i], 'a'+i);
            printdfa(dfa[now][i]);
        }
}

void printset(sint& a)
{
    for(int i=0; i<a.size; i++)
        printf("%d ", a.elem[i]);
    printf("\n");
}

int main()
{
    //counttime();

    int t, tt, len;
    char c[105];

    scanf("%d", &t);

    while(t--)
    {
    	reset();
    	len = -1;

    	scanf("%s%d", c, &tt);

        if (strlen(c) == 1)
        {
            printf("%d\n", tt == 1);
            continue;
        }

        while(c[++len])
        {
            switch(c[len])
            {
            case 'a':
            case 'b':
                nfa[nfasize][0] = pii(nfasize+1, c[len]-'a');
                isbranch[nfasize] = 0;
                operan.push(pii(nfasize, nfasize+1));
                nfasize += 2;
                break;

            case '(':
            case '|':
            case '*':
                operat.push(c[len]);
                break;

            case ')':
                join();
                break;
            }
        }

        dfsbuild(nfastart);

        stack<int> stek;
        sint dfainit, dfamove, dfaeps;
        int nowtemp;
        map<sint, int>::iterator it;

        dfainit.add(nfastart);
        epsmove(dfainit, dfaeps);
        dfanodemap[dfaeps] = dfasize;
        dfanodemap2[dfasize] = dfaeps;
        stek.push(dfasize++);

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
                        dfanodemap[dfaeps] = dfasize;
                        dfanodemap2[dfasize] = dfaeps;
                        stek.push(dfasize);
                        dfa[nowtemp][i] = dfasize;
                        adjmat[nowtemp][dfasize++] += 1;
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

        for (int i=0; i<dfasize; i++)
			if (dfanodemap2[i].exist(nfaend))
				hasil += res[0][i];

		printf("%lld\n", hasil % MOD);
    }
	printf("time a: %f\ntime b: %f\ntime c: %f\n", timea, timeb, timec);
	printtime("end");
}
