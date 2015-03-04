#include <cstdio>
#include <algorithm>
#include <cstring>
#include <stack>
#include <queue>
#include <vector>
#include <map>
#include <set>

using namespace std;

class NFA_VERTEX;
class DFA_VERTEX;
class NFA_TRACER;

set<int> Move(set<int>, char);
set<int> Eps_Move(set<int>);

int NID = 0;
int DID = 0;

map<int, NFA_VERTEX*> NFA_GRAPH;
map<set<int>, DFA_VERTEX*> DFA_GRAPH;

class NFA_VERTEX
{
public:
    int id;
	vector<NFA_VERTEX*> next[3];

	NFA_VERTEX()
	{
		id = NID;
		NFA_GRAPH[NID++] = this;
	}

	~NFA_VERTEX()
	{
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<next[i].size();j++)
			{
				delete next[i][j];
			}
		}
	}
};

class DFA_VERTEX
{
public:
    int id;
	DFA_VERTEX* next[2];
	set<int> label;

	DFA_VERTEX(set<int> _label)
	{
		id=DID++;
		next[0] = next[1] = NULL;
		label = _label;
		DFA_GRAPH[_label] = this;
	}

	~DFA_VERTEX()
	{
		if(next[1] != NULL)
		{
            delete next[1];
		}
		if(next[2] != NULL)
        {
            delete next[2];
        }
	}
};

class NFA_TRACER
{
	public:
	NFA_VERTEX* start;
	NFA_VERTEX* finish;

	NFA_TRACER(NFA_VERTEX* _start, NFA_VERTEX* _finish)
	{
		start = _start;
		finish = _finish;
	}
};

set<int> Move(set<int> node, int character)
{
    set<int>::iterator it;
    set<int> result;
    for( it=node.begin(); it!=node.end(); it++)
    {
        NFA_VERTEX* tmp = NFA_GRAPH[*it];
        for(int i=0;i<tmp->next[character].size();i++)
        {
            result.insert(tmp->next[character][i]->id);
        }
    }
    return result;
}

set<int> EpsMove(set<int> node)
{
    stack<int> nodes;
    set<int> result = node;
    set<int>::iterator it;
    for( it=node.begin(); it!=node.end();it++)
    {
        nodes.push(*it);
    }
    while(nodes.empty() == false)
    {
        int t = nodes.top();
        nodes.pop();
        NFA_VERTEX* tmp = NFA_GRAPH[t];
        for(int i=0; i<tmp->next[2].size(); i++)
        {
            NFA_VERTEX* cur = tmp->next[2][i];
            if(result.count(cur->id) == 0)
            {
                result.insert(cur->id);
                nodes.push(cur->id);
            }
        }
    }
    return result;
}

NFA_TRACER* OpConcat(NFA_TRACER* r, NFA_TRACER* s)
{
    r->finish->next[2].push_back(s->start);
    return new NFA_TRACER(r->start, s->finish);
}

NFA_TRACER* OpUnion(NFA_TRACER* r, NFA_TRACER* s)
{
    NFA_VERTEX* first = new NFA_VERTEX();
    NFA_VERTEX* second = new NFA_VERTEX();

    first->next[2].push_back(r->start);
    first->next[2].push_back(s->start);
    s->finish->next[2].push_back(second);
    r->finish->next[2].push_back(second);
    return new NFA_TRACER(first,second);
}

NFA_TRACER* OpStar(NFA_TRACER* r)
{
    NFA_VERTEX* ed = new NFA_VERTEX();
    ed->next[2].push_back(r->start);
    r->finish->next[2].push_back(ed);
    return new NFA_TRACER(ed,ed);
}

DFA_VERTEX* AddToGraph(set<int> _label)
{
	if(DFA_GRAPH.count(_label) == 1)
		return DFA_GRAPH[_label];

    DFA_VERTEX* tmp = new DFA_VERTEX(_label);
    return tmp;
}

long long** MatrixMul(long long** y, long long** z)
{
    long long** x;
    x = (long long**)calloc(DID, sizeof(long long*));
    for(int i=0;i<DID;i++)
    {
        x[i] = (long long*)calloc(DID, sizeof(long long));
    }
    for(int a=0;a<DID;a++)
    {
        for(int b=0;b<DID;b++)
        {
            for(int c=0;c<DID;c++)
            {
                x[a][b]=(x[a][b]+((y[a][c])*(z[c][b])))%1000000007;
            }
        }
    }

    return x;
}

long long** ExpMatrix(long long** awal, int exp)
{
    int a, b;
    long long** x;
    x = (long long**)calloc(DID, sizeof(long long*));
    for(int i=0;i<DID;i++)
    {
        x[i] = (long long*)calloc(DID, sizeof(long long));
    }
    for(int i=0;i<DID;i++)
    {
        for(int j=0;j<DID;j++)
        {
            x[i][j]=(i==j);
        }
    }
    while(exp){
        if (exp&1)
        {
            x=MatrixMul(x, awal);
        }
        awal=MatrixMul(awal, awal);
        exp>>=1;
    }
    return x;
}


int main()
{
    // Untuk Input
    char in[101], regexp[202];
    int test, len, N;

    //
    stack<char> OP;
    stack<NFA_TRACER*> states;

    for(scanf("%d", &test);test--;)
    {
    	// Initialize variables.
    	NID = 0;
    	DID = 0;
    	NFA_GRAPH.clear();
    	DFA_GRAPH.clear();

    	// Begin input.
        scanf("%s %d", in, &N);
        len = strlen(in);

        int iter=0;
        for(int i=0;i<len; i++)
        {
            if(i!=0 && ( in[i]=='(' || in[i]=='a' || in[i] == 'b')
                     && (in[i-1]==')' || in[i-1]=='a' || in[i-1]=='b'))
            {
                regexp[iter++]='+';
            }
            regexp[iter++]=in[i];
        }
        regexp[iter]=0;
        len = strlen(regexp);
//        printf("%s\nlength = %d\n", regexp, len);

        // Begin parsing input into NFA
        for(int i=0;i<len;i++)
        {
            if(regexp[i]=='(') continue;
            else if (regexp[i]=='a' || regexp[i]=='b')
            {
            	NFA_VERTEX* first = new NFA_VERTEX();
          		NFA_VERTEX* second = new NFA_VERTEX();
          		first->next[regexp[i]-'a'].push_back(second);
          		NFA_TRACER* state = new NFA_TRACER(first, second);
          		states.push(state);
            }
            else if (regexp[i]==')')
            {
                char op = OP.top();
                OP.pop();
                if(op=='|')
                {
                    NFA_TRACER* s = states.top();
                    states.pop();
                    NFA_TRACER* r = states.top();
                    states.pop();

                    NFA_TRACER* state = OpUnion(r, s);
                    states.push(state);
                }
                else if(op=='+')
                {
                    NFA_TRACER* s = states.top();
                    states.pop();
                    NFA_TRACER* r = states.top();
                    states.pop();

                    NFA_TRACER* state = OpConcat(r, s);
                    states.push(state);
                }
                else if(op=='*')
                {
                    NFA_TRACER* r = states.top();
                    states.pop();

                    NFA_TRACER* state = OpStar(r);
                    states.push(state);
                }
            }
            else
            {
                OP.push(regexp[i]);
            }
        }
        NFA_TRACER* finalNFA = states.top();
        states.pop();

        NFA_VERTEX* start = finalNFA->start;
        NFA_VERTEX* finish = finalNFA->finish;
        set<int> initNode;
        set<int> visited;

        initNode.insert(start->id);
        initNode = EpsMove(initNode);


        // Create DFA from NFA
        DFA_VERTEX* initDFA = AddToGraph(initNode);

        queue<DFA_VERTEX*> qu;
        qu.push(initDFA);
        while(qu.empty() == false)
        {
            DFA_VERTEX* dv = qu.front();
            qu.pop();
            for(int i=0; i<2; i++)
            {
                set<int> ret = EpsMove(Move(dv->label,i));
                if(ret.empty() == true) continue;
                DFA_VERTEX* tmp = AddToGraph(ret);
                dv->next[i] = tmp;
                if(visited.count(tmp->id) == 0)
                {
                    visited.insert(tmp->id);
                    qu.push(tmp);
                }
            }
        }

        // Create adjacency matrix from DFA Graph.
        long long** adjMatrix;
        adjMatrix = (long long**)calloc(DID, sizeof(long long*));
        for(int i=0;i<DID;i++)
        {
            adjMatrix[i] = (long long*)calloc(DID, sizeof(long long));
        }
        map<set<int>, DFA_VERTEX*>::iterator it;
        for(it = DFA_GRAPH.begin(); it!=DFA_GRAPH.end(); it++)
        {
            DFA_VERTEX* dv = it->second;
            for(int i=0;i<2;i++)
            {
                DFA_VERTEX* dv2 = dv->next[i];
                if(dv2!=NULL)
                {
                    adjMatrix[dv->id][dv2->id]+=1;
                }
            }
        }

        long long** res = ExpMatrix(adjMatrix, N);
        long long hasil=0;
        for(it=DFA_GRAPH.begin();it != DFA_GRAPH.end(); it++)
        {
            if(it->second->label.count(finish->id))
            {
                hasil += res[0][it->second->id];
            }
        }
        printf("%lld\n", hasil);
    }
    return 0;
}


//for(int i=0;i<DID;i++)
//{
//    for(int j=0;j<DID;j++)
//    {
//        printf("%lld ", adjMatrix[i][j]);
//    }
//    printf("\n");
//}
//printf("\n");
