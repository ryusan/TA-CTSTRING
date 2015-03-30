#include <cstdio>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <map>
using namespace std;

#define pii per<int,int>
#define MAXNODE 300

template <typename T>
struct stack
{
    T elem[MAXNODE];
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
    int elem[MAXNODE];
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

pii nfa[MAXNODE][2];
bool isbranch[MAXNODE];
int nfastart = -1;
int nfaend = -1;
int nfasize = 0;
stack<pii> operan;
stack<char> operat;

int dfa[MAXNODE][2];
int dfasize = 0;
map<sint, int> dfanodemap;
sint dfanodemap2[MAXNODE];
sint eps[MAXNODE];
bool dfsmark[MAXNODE];
bool markeps[MAXNODE];

void reset()
{
    memset(dfanodemap2, 0, sizeof(dfanodemap2));
    memset(eps, 0, sizeof(eps));
    memset(isbranch, 0, sizeof(isbranch));
    memset(markeps, 0, sizeof(markeps));
    memset(dfsmark, 0, sizeof(dfsmark));
    memset(nfa, 0, sizeof(nfa));
    memset(dfa, 0, sizeof(dfa));

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
    case '(':
        operan1 = operan.top();

        nfastart = operan1.a;
        nfaend = operan1.b;
        break;

    case '.': //concat
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

        operat.pop();
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
    retval = nodes;
    for (int i=0; i<nodes.size; i++)
        retval.add(eps[nodes.elem[i]]);
}

int main()
{
    int t, len;
    char re[200], str[200];

    while(scanf("%s", re) != EOF)
    {
    	reset();
    	len = -1;

    	scanf("%d", &t);
    	getchar();

        if (strlen(re) == 1)
        {
            while(t--)
            {
                str[0] = 0;
                scanf("%[^\n]", str);
                getchar();
                if (strlen(str) == 1 && str[0] == re[0])
                    printf("Y\n");
                else printf("N\n");
            }

            continue;
        }

        while(re[++len])
        {
            switch(re[len])
            {
            case 'a':
            case 'b':
                nfa[nfasize][0] = pii(nfasize+1, re[len]-'a');
                isbranch[nfasize] = 0;
                operan.push(pii(nfasize, nfasize+1));
                nfasize += 2;
                break;

            case '(':
            case '|':
            case '*':
            case '.':
                operat.push(re[len]);
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
                        dfa[nowtemp][i] = dfasize++;
                    }
                    else
                        dfa[nowtemp][i] = it->second;
                }
                else
                    dfa[nowtemp][i] = -1;
            }
        }

        while(t--)
        {
            str[0] = 0;
            scanf("%[^\n]", str);
//            printf("keks %s\n", str);
            getchar();
            len = -1;
            int now = 0;
            while(str[++len])
            {
                now = dfa[now][str[len]-'a'];
                if (now == -1)
                    break;
            }
            if (len != strlen(str) || !dfanodemap2[now].exist(nfaend))
                printf("N\n");
            else
                printf("Y\n");
        }
        printf("\n");
    }
}

