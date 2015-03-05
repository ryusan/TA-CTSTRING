#include <cstdio>
#include <algorithm>
#include <cstring>
#include <stack>
#include <queue>
#include <map>
#include <ctime>

using namespace std;

int NID = 0;
int DID = 0;
int LID = 0;

template <typename T>
struct vect{
    T dt[200];
    int data;

    vect()
    {
        data = 0;
        memset((void*)dt,0,sizeof(dt));
    }

    int size()
    {
        return data;
    }

    int push(T ins)
    {
        if(data == 200)
            return 0;
        dt[data++] = ins;
        return 1;
    }
};

template <typename T>
struct labelset{
    int id;
    T dt[200];
    int data;

	labelset()
    {
        data = 0;
        memset((void*)dt,-1,sizeof(dt));
    }

    labelset(int _id)
    {
        id = _id;
        data = 0;
        memset((void*)dt,-1,sizeof(dt));
    }

    bool operator==(const labelset &o) const
	{
		if(o.data==data)
		{
			for(int i=0;i<data;i++)
			{
				if(o.dt[i]!=dt[i])
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		return true;
    }

    bool operator<(const labelset &o) const
	{
		int iter = min(o.data, data);
        if(data < o.data)
			return true;
		if(data == o.data)
			for(int i=0;i<iter;i++)
			{
				if(dt[i]<o.dt[i]) return true;
			}
		return false;
    }

    int push(T ins)
    {
        bool exists=false;
        for(int i=0;i<data;i++)
        {
            if(ins == dt[i])
            {
                exists = true;
            }
        }

        if (exists)
            return 0;
        dt[data++] = ins;
		sort(dt,dt+data);
		return 1;
    }

    int count(T ins)
    {
        bool exists=false;
        for(int i=0;i<data;i++)
        {
            if(ins == dt[i])
            {
                exists = true;
            }
        }
        if (exists)
            return 1;
        return 0;
    }

    int size()
    {
        return data;
    }

    void clear()
    {
        labelset();
    }

};

class NFA_VERTEX;
class DFA_VERTEX;
class NFA_TRACER;

labelset<int>* Move(labelset<int>*, char);
labelset<int>* Eps_Move(labelset<int>*);

map<int, NFA_VERTEX*> NFA_GRAPH;
map<labelset<int>, DFA_VERTEX*> DFA_GRAPH;


class NFA_VERTEX
{
public:
    int id;
	vect<NFA_VERTEX*> next[3];

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
				delete next[i].dt[j];
			}
		}
	}

	void Debug(int id)
	{
		for(int i=0 ;i<3;i++)
		{
			for(int j=0;j<next[i].size();j++)
			{
				printf("%d -> %d [label=\"%c\"];\n", this->id, next[i].dt[j]->id, (i!=2 ? i+'a':'e'));
			}
		}
		if(this->id == id)
		{
			printf("%d [shape=\"doublecircle\"];\n", id);
		}
	}
};

class DFA_VERTEX
{
public:
    int id;
	DFA_VERTEX* next[2];
	labelset<int>* label;

	DFA_VERTEX(labelset<int>* _label)
	{
		id=DID++;
		next[0] = next[1] = NULL;
		label = new labelset<int>(DID);
		for(int i=0;i<_label->data;i++)
		{
			label->push(_label->dt[i]);
		}
		DFA_GRAPH[*_label] = this;
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

    void Debug(int id)
	{
		if(label->count(id))
		{
			printf("%d [shape=\"doublecircle\"]\n", this->id);
		}
		for(int i=0 ;i<2;i++)
		{
			if(next[i] != NULL)
				printf("%d -> %d [label=\"%c\"];\n", this->id, next[i]->id, i+'a');
			//printf("%d node label\n", id);
			//for(it = label.begin(); it!=label.end() && printf(",");it++)
			//{
			//	printf("%d", *it);
			//}
			//printf("\n");
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

labelset<int>* Move(labelset<int>* node, int character)
{
    labelset<int>* result = new labelset<int>();
    NFA_VERTEX* tmp;
    for( int i=0; i<node->data; i++)
    {
        tmp = NFA_GRAPH[node->dt[i]];
        for(int i=0;i<tmp->next[character].size();i++)
        {
            result->push(tmp->next[character].dt[i]->id);
        }
    }
    return result;
}

labelset<int>* EpsMove(labelset<int>* node)
{
    stack<int> nodes;
    labelset<int>* result = new labelset<int>();
	for(int i=0;i<node->data;i++)
	{
		result->push(node->dt[i]);
	}
    NFA_VERTEX* tmp;
    NFA_VERTEX* tmp2;
    for( int i=0; i < node->data;i++)
    {
        nodes.push(node->dt[i]);
    }
    while(nodes.empty() == false)
    {
        int t = nodes.top();
        nodes.pop();
        tmp = NFA_GRAPH[t];
        for(int i=0; i<tmp->next[2].size(); i++)
        {
            tmp2 = tmp->next[2].dt[i];
            if(result->count(tmp2->id) == 0)
            {
                result->push(tmp2->id);
                nodes.push(tmp2->id);
            }
        }
    }
    return result;
}

NFA_TRACER* OpConcat(NFA_TRACER* r, NFA_TRACER* s)
{
    r->finish->next[2].push(s->start);
    return new NFA_TRACER(r->start, s->finish);
}

NFA_TRACER* OpUnion(NFA_TRACER* r, NFA_TRACER* s)
{
    NFA_VERTEX* first = new NFA_VERTEX();
    NFA_VERTEX* second = new NFA_VERTEX();

    first->next[2].push(r->start);
    first->next[2].push(s->start);
    s->finish->next[2].push(second);
    r->finish->next[2].push(second);
    return new NFA_TRACER(first,second);
}

NFA_TRACER* OpStar(NFA_TRACER* r)
{
    NFA_VERTEX* ed = new NFA_VERTEX();
    ed->next[2].push(r->start);
    r->finish->next[2].push(ed);
    return new NFA_TRACER(ed,ed);
}

DFA_VERTEX* AddToGraph(labelset<int>* _label)
{
	if(DFA_GRAPH.count(*_label) == 1)
		return DFA_GRAPH[*_label];

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
    clock_t begin = clock();
    // Untuk Input
    char in[121], regexp[222];
    int test, len, N;

    //
    stack<char> OP;
    stack<NFA_TRACER*> states;

    for(scanf("%d", &test);test--;)
    {
    	// Initialize variables.
    	NID = 0;
    	DID = 0;
    	LID = 0;
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
          		first->next[regexp[i]-'a'].push(second);
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
		labelset<int>* initNode = new labelset<int>();
		labelset<int>* visited = new labelset<int>();

        initNode->push(start->id);
        initNode = EpsMove(initNode);

        // Create DFA from NFA
        DFA_VERTEX* initDFA = AddToGraph(initNode);
		visited->push(initDFA->id);
        queue<DFA_VERTEX*> qu;
        qu.push(initDFA);
        DFA_VERTEX* tmp;
        labelset<int>* ret;
        while(qu.empty() == false)
        {
            DFA_VERTEX* dv = qu.front();
            qu.pop();
            for(int i=0; i<2; i++)
            {
                ret = EpsMove(Move((dv->label),i));
                if(ret->data == 0) continue;
                tmp = AddToGraph(ret);
                dv->next[i] = tmp;
                if(visited->count(tmp->id) == 0)
                {
                    visited->push(tmp->id);
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

		map<labelset<int>, DFA_VERTEX*>::iterator it_dfa;
		map<int, NFA_VERTEX*>::iterator it_nfa;

////////////////////////////////////////////////////////////////////////////////////////
//Debugging
//		puts("\nnfa");
//		for(it_nfa = NFA_GRAPH.begin(); it_nfa!=NFA_GRAPH.end();it_nfa++)
//		{
//			it_nfa->second->Debug(finish->id);
//		}
//		puts("\ndfa");
//		for(it_dfa = DFA_GRAPH.begin(); it_dfa!=DFA_GRAPH.end();it_dfa++)
//		{
//			it_dfa->second->Debug(finish->id);
//		}
//////////////////////////////////////////////////////////////////////////////////////
		for(it_dfa = DFA_GRAPH.begin(); it_dfa!=DFA_GRAPH.end(); it_dfa++)
        {
            DFA_VERTEX* dv = it_dfa->second;
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
        long long hasil = 0;
        for(it_dfa=DFA_GRAPH.begin();it_dfa != DFA_GRAPH.end(); it_dfa++)
        {
            if(it_dfa->second->label->count(finish->id))
            {
                hasil += res[0][it_dfa->second->id];
            }
        }
        printf("%lld\n", hasil%1000000007);
    }
    clock_t end = clock();
    double timeSec = (end - begin) / static_cast<double>( CLOCKS_PER_SEC );

    printf("elapsed time = %lf\n", timeSec);
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
