#include <cstdio>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

using namespace std;


int NID = 0;
int DID = 0;

class NFA_VERTEX;
class DFA_VERTEX;
class NFA_TRACER;

set<int> Move(set<int>, char);
set<int> Eps_Move(set<int>);
stack<NFA_TRACER*> states;
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
		for(unsigned i=0;i<3;i++)
		{
			for(unsigned j=0;j<next[i].size();j++)
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
	bool finish;

	DFA_VERTEX(set<int> _label)
	{
		id=DID++;
		finish = false;
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
    set<int> retval;
    NFA_VERTEX* tmp;
    set<int>::iterator it;
    for(it = node.begin(); it!=node.end(); it++)
    {
        tmp = NFA_GRAPH[*it];
        for(unsigned i=0;i<tmp->next[character].size();i++)
        {
            retval.insert(tmp->next[character][i]->id);
        }
    }
	return retval;
}

set<int> EpsMove(set<int> node)
{
    stack<int> nodes;
    set<int> retval;
	set<int>::iterator it;
	retval = node;
    NFA_VERTEX *tmp, *tmp2;
    for(it = node.begin(); it != node.end();it++)
    {
        nodes.push(*it);
    }
    while(nodes.empty() == false)
    {
        int t = nodes.top();
        nodes.pop();
        tmp = NFA_GRAPH[t];
        for(unsigned i=0; i<tmp->next[2].size(); i++)
        {
            tmp2 = tmp->next[2][i];
            if(retval.count(tmp2->id) == 0)
            {
                retval.insert(tmp2->id);
                nodes.push(tmp2->id);
            }
        }
    }
	return retval;
}

void OpConcat(NFA_TRACER &r, NFA_TRACER &s)
{
    r.finish->next[2].push_back(s.start);
    states.push(new NFA_TRACER(r.start, s.finish));
}

void OpUnion(NFA_TRACER &r, NFA_TRACER &s)
{
    NFA_VERTEX* first = new NFA_VERTEX();
    NFA_VERTEX* second = new NFA_VERTEX();

    first->next[2].push_back(r.start);
    first->next[2].push_back(s.start);
    s.finish->next[2].push_back(second);
    r.finish->next[2].push_back(second);
    states.push(new NFA_TRACER(first,second));
}

void OpStar(NFA_TRACER &r)
{
    NFA_VERTEX* st = new NFA_VERTEX();
    NFA_VERTEX* ed = new NFA_VERTEX();

    st->next[2].push_back(r.start);
    st->next[2].push_back(ed);
    r.finish->next[2].push_back(r.start);
    r.finish->next[2].push_back(ed);
    states.push(new NFA_TRACER(st,ed));
}

DFA_VERTEX* AddToGraph(set<int> _label)
{
	if(DFA_GRAPH.count(_label) == 1)
		return DFA_GRAPH[_label];

    DFA_VERTEX* tmp = new DFA_VERTEX(_label);
    return tmp;
}


int main()
{
    // Untuk Input
    char in[181], regexp[101];
    int len, N;

    stack<char> OP;
    while(scanf("%s", in)!=EOF)
    {

    	NID = 0;
    	DID = 0;
    	NFA_GRAPH.clear();
    	DFA_GRAPH.clear();

    	// Begin input.
        len = strlen(in);
    	NFA_TRACER* state;

        // Begin parsing input into NFA
        for(int i=0;i<len;i++)
        {
            if(in[i]=='(') continue;
            else if (in[i]=='a' || in[i]=='b')
            {
            	NFA_VERTEX* first = new NFA_VERTEX();
          		NFA_VERTEX* second = new NFA_VERTEX();
          		first->next[in[i]-'a'].push_back(second);
          		state = new NFA_TRACER(first, second);
          		states.push(state);
            }
            else if (in[i]==')')
            {

                char op = '-';
                if(!OP.empty())
                    op = OP.top(), OP.pop();
                if(op=='|')
                {
                    NFA_TRACER* s = states.top();
                    states.pop();
                    NFA_TRACER* r = states.top();
                    states.pop();

                    OpUnion(*r, *s);
                }
                else if(op=='.')
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
                OP.push(in[i]);
            }
        }
        state = states.top();
        states.pop();

        NFA_VERTEX* start = state->start;
        NFA_VERTEX* finish = state->finish;

		set<int> initNode;
		set<int> visited;

		map<set<int>, DFA_VERTEX*>::iterator it_dfa;
		map<int, NFA_VERTEX*>::iterator it_nfa;

        initNode.insert(start->id);
        initNode = EpsMove(initNode);
        // Create DFA from NFA
        DFA_VERTEX* initDFA = AddToGraph(initNode);
        visited.insert(initDFA->id);
        queue<DFA_VERTEX*> qu;
        qu.push(initDFA);
        DFA_VERTEX* tmp;
        set<int> ret;
        while(qu.empty() == false)
        {
            DFA_VERTEX* dv = qu.front();
            qu.pop();

            if(dv->label.count(finish->id))
                dv->finish = true;

            for(int i=0; i<2; i++)
            {
                ret = EpsMove(Move((dv->label),i));
                if(ret.empty()) continue;
                tmp = AddToGraph(ret);

                dv->next[i] = tmp;
                if(visited.count(tmp->id) == 0)
                {
                    visited.insert(tmp->id);
                    qu.push(tmp);
                }
            }
        }

        for(scanf("%d", &N), getchar();N--;)
        {
            regexp[0]=0;
            scanf("%[^\n]", regexp);
            getchar();
//            puts(regexp);
            len = strlen(regexp);
            tmp = initDFA;
            int i;
            for(i=0;i<len;)
            {
                tmp = tmp->next[regexp[i]-'a'];
                if(tmp==NULL)
                    break;
                ++i;
            }
            if(i!=len || !tmp->finish) printf("N\n");
            else printf("Y\n");
        }
        printf("\n");
    }
    return 0;
}

