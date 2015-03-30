#include <cstdio>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <map>
#include <queue>

#define MOD 1000000007

using namespace std;


int NID = 0;
int DID = 0;

template <typename T>
struct vect{
    T dt[5];
    int data;

	// init new vector
    vect()
    {
        data = 0;
    }

	// return vector size
    int size()
    {
        return data;
    }

	// add new element to vector
	// if vector full ret 0, if succes return 1
    bool push(T ins)
    {
        if(data == 20)
            return false;
        dt[data++] = ins;
        return true;
    }
};

template <typename T>
struct labelset{
    unsigned long long hash_id;
    T dt[101];
    int data;

	// initialize label without id
	labelset()
    {
        hash_id = 0;
        data = 0;
    }

	// initialize label with id

	// struct comparator with '==' operator
	// if dt equal return true, else return false
    bool operator==(const labelset &o) const
	{
		return hash_id == o.hash_id;
    }

	// struct comparator with '==' operator
    // if |data| < cmp.|data| ret true;
	// if data[i] < cmp.data[i] ret true;
	// else ret false;
	bool operator<(const labelset &o) const
	{
            return hash_id < o.hash_id;
    }

	// insert new element to label
	// check for duplicates and sort the element after insert
	// if exists ret 0, else return 1
    bool push(T ins)
    {
        for(int i=0;i<data;i++)
        {
            if(ins == dt[i])
            {
                return false;
            }
        }
        dt[data++] = ins;
		sort(dt,dt+data);
		return true;
    }

    void CalculateHash()
    {
        hash_id = 0;
        for(int i=0;i<data;i++)
        {
            hash_id=hash_id*17+dt[i];
        }
    }

	// check element in label
	// if exists return 1, else return 0
    bool count(T ins)
    {
        for(int i=0;i<data;i++)
        {
            if(ins == dt[i])
            {
                return true;
            }
        }
        return false;
    }

	// return how much nodes in label
    int size()
    {
        return data;
    }

    void clear()
    {
        labelset();
    }

};

template <typename T>
struct stack
{
	T dt[181];
	int ptr;

	// initialize stack
	stack()
	{
		ptr = -1;
	}

	// 1 if succes, 0 if full
	bool push(T ins)
	{
		if(ptr+1 < 181)
		{
			dt[++ptr] = ins;
			return true;
		}
		return false;
	}

	// 1 if succes, 0 if fail
	bool pop()
	{
		if(ptr >= 0)
		{
			dt[ptr--] = 0;
			return true;
		}
		return false;

	}

	// T if succes, NULL if empty stack
	T top()
	{
		return dt[ptr];
	}

	// stack size
	int size()
	{
		return ptr;
	}

	// check wether stack is empty
	bool empty()
	{
		return ptr == -1;
	}
};

class NFA_VERTEX;
class DFA_VERTEX;
class NFA_TRACER;

labelset<int> Move(labelset<int>, char);
labelset<int> Eps_Move(labelset<int>);
stack<NFA_TRACER*> states;
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
	labelset<int> label;

	DFA_VERTEX(labelset<int> _label)
	{
		id=DID++;
		next[0] = next[1] = NULL;
		for(int i=0;i<_label.data;i++)
		{
			label.push(_label.dt[i]);
		}
		_label.CalculateHash();
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

    void Debug(int id)
	{
		if(label.count(id))
		{
			printf("%d [shape=\"doublecircle\"]\n", this->id);
		}
		for(int i=0 ;i<2;i++)
		{
			if(next[i] != NULL)
				printf("%d -> %d [label=\"%c\"];\n", this->id, next[i]->id, i+'a');
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

labelset<int> Move(labelset<int> node, int character)
{
    labelset<int> retval;
    NFA_VERTEX* tmp;
    for( int i=0; i<node.data; i++)
    {
        tmp = NFA_GRAPH[node.dt[i]];
        for(int i=0;i<tmp->next[character].size();i++)
        {
            retval.push(tmp->next[character].dt[i]->id);
        }
    }
	retval.CalculateHash();
	return retval;
}

labelset<int> EpsMove(labelset<int> node)
{
    stack<int> nodes;
    labelset<int> retval;
	for(int i=0;i<node.data;i++)
	{
		retval.push(node.dt[i]);
	}
    NFA_VERTEX *tmp, *tmp2;
    for( int i=0; i < node.data;i++)
    {
        nodes.push(node.dt[i]);
    }
    while(nodes.empty() == false)
    {
        int t = nodes.top();
        nodes.pop();
        tmp = NFA_GRAPH[t];
        for(int i=0; i<tmp->next[2].size(); i++)
        {
            tmp2 = tmp->next[2].dt[i];
            if(retval.count(tmp2->id) == 0)
            {
                retval.push(tmp2->id);
                nodes.push(tmp2->id);
            }
        }
    }
	retval.CalculateHash();
	return retval;
}

void OpConcat(NFA_TRACER &r, NFA_TRACER &s)
{
    r.finish->next[2].push(s.start);
    states.push(new NFA_TRACER(r.start, s.finish));
}

void OpUnion(NFA_TRACER &r, NFA_TRACER &s)
{
    NFA_VERTEX* first = new NFA_VERTEX();
    NFA_VERTEX* second = new NFA_VERTEX();

    first->next[2].push(r.start);
    first->next[2].push(s.start);
    s.finish->next[2].push(second);
    r.finish->next[2].push(second);
    states.push(new NFA_TRACER(first,second));
}

void OpStar(NFA_TRACER &r)
{
    NFA_VERTEX* st = new NFA_VERTEX();
    NFA_VERTEX* ed = new NFA_VERTEX();

    st->next[2].push(r.start);
    st->next[2].push(ed);
    r.finish->next[2].push(r.start);
    r.finish->next[2].push(ed);
    states.push(new NFA_TRACER(st,ed));
}

DFA_VERTEX* AddToGraph(labelset<int> _label)
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
                x[a][b]=(x[a][b]+((y[a][c])*(z[c][b])))%MOD;
            }
        }
    }

    return x;
}

long long** ExpMatrix(long long** awal, int exp)
{
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
    clock_t all_start = clock();
    clock_t c_end = clock();
    clock_t c_start = clock();

    double nfa_time, dfa_time, matmul_time, all_time;
    // Untuk Input
    char in[121], regexp[222];
    int test, len, N;

    //
    stack<char> OP;

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
            if(i!=0 && ( in[i]=='(' || in[i]=='a' || in[i] == 'b') && (in[i-1]==')' || in[i-1]=='a' || in[i-1]=='b'))
            {
                regexp[iter++]='+';
            }
            regexp[iter++]=in[i];
        }
        regexp[iter]=0;
//        printf("%s\n", regexp);
        len = strlen(regexp);

    	c_start = clock();
    	NFA_TRACER* state;

        // Begin parsing input into NFA
        for(int i=0;i<len;i++)
        {
            if(regexp[i]=='(') continue;
            else if (regexp[i]=='a' || regexp[i]=='b')
            {
            	NFA_VERTEX* first = new NFA_VERTEX();
          		NFA_VERTEX* second = new NFA_VERTEX();
          		first->next[regexp[i]-'a'].push(second);
          		state = new NFA_TRACER(first, second);
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

                    OpUnion(*r, *s);
                }
                else if(op=='+')
                {
                    NFA_TRACER* s = states.top();
                    states.pop();
                    NFA_TRACER* r = states.top();
                    states.pop();

                    OpConcat(*r, *s);
                }
                else if(op=='*')
                {
                    NFA_TRACER* r = states.top();
                    states.pop();

                    OpStar(*r);
                }
            }
            else
            {
                OP.push(regexp[i]);
            }
        }
        state = states.top();
        states.pop();

        NFA_VERTEX* start = state->start;
        NFA_VERTEX* finish = state->finish;

        c_end = clock();
        nfa_time = (c_end - c_start) / static_cast<double>( CLOCKS_PER_SEC );

		labelset<int> initNode;
		labelset<int> visited;

		map<labelset<int>, DFA_VERTEX*>::iterator it_dfa;
		map<int, NFA_VERTEX*>::iterator it_nfa;

        initNode.push(start->id);
        initNode = EpsMove(initNode);
        // Create DFA from NFA
        DFA_VERTEX* initDFA = AddToGraph(initNode);
		visited.push(initDFA->id);
        queue<DFA_VERTEX*> qu;
        qu.push(initDFA);
        DFA_VERTEX* tmp;
        labelset<int> ret;
        while(qu.empty() == false)
        {
            DFA_VERTEX* dv = qu.front();
            qu.pop();
            for(int i=0; i<2; i++)
            {
                ret = EpsMove(Move((dv->label),i));
                if(ret.data == 0) continue;
                tmp = AddToGraph(ret);
                dv->next[i] = tmp;
                if(visited.count(tmp->id) == 0)
                {
                    visited.push(tmp->id);
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


		DFA_VERTEX *dv, *dv2;
		for(it_dfa = DFA_GRAPH.begin(); it_dfa!=DFA_GRAPH.end(); it_dfa++)
        {
            dv = it_dfa->second;
            for(int i=0;i<2;i++)
            {
                dv2 = dv->next[i];
                if(dv2!=NULL)
                {
                    adjMatrix[dv->id][dv2->id]+=1;
                }
            }
        }
//////////////////////////////////////////////////////////////////////////////////////
////Debugging
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
//		puts("\nprinting adj matrix");
//		for(int i=0;i<DID;i++)
//        {
//            for(int j=0;j<DID;j++)
//            {
//                printf("%I64d ", adjMatrix[i][j]);
//            }
//            puts("");
//        }
////////////////////////////////////////////////////////////////////////////////////////

        c_end = clock();
        dfa_time = (c_end - c_start) / static_cast<double>( CLOCKS_PER_SEC );

        long long** res = ExpMatrix(adjMatrix, N);
        long long hasil = 0;
        for(it_dfa=DFA_GRAPH.begin();it_dfa != DFA_GRAPH.end(); it_dfa++)
        {
            if(it_dfa->second->label.count(finish->id))
            {
                hasil += res[0][it_dfa->second->id];
            }

        }
        printf("%I64d\n", hasil%MOD);

        c_end = clock();
        matmul_time = (c_end - c_start) / static_cast<double>( CLOCKS_PER_SEC );
//        printf("nfa=%f\ndfa=%f\nmatmul=%f\n\n", nfa_time, dfa_time, matmul_time);
    }
    c_end = clock();
    all_time = (c_end - all_start) / static_cast<double>( CLOCKS_PER_SEC );
//printf("Total elapsed time = %f\n", all_time);

    return 0;
}


//(b|((a*)b)) 30
