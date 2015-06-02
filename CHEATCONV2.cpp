#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
#include <cstring>
#include <set>

using namespace std;

struct dfa_vertex;
struct nfa_vertex;
struct nfa_states;

int dfa_id;
int nfa_id;

nfa_vertex* nfamap[212];
map<set<int>, dfa_vertex*> dfamap;

stack<char> ops;
stack<nfa_states> states;

struct nfa_states
{
    int id_first;
    int id_last;

    nfa_states()
    {

    }

    nfa_states(int _begin, int _end)
    {
        id_first = _begin;
        id_last = _end;
    }
};

struct nfa_vertex
{
    int id;
    vector<nfa_vertex*> next[3];

    nfa_vertex()
    {
        id = nfa_id++;
        nfamap[id] = this;
    }

    ~nfa_vertex()
    {
        for( int i=0; i<3; ++i )
            next[i].clear();
    }
    void Debug(int start, int finish)
	{
		for(int i=0 ;i<3;i++)
		{
			for(unsigned j=0;j<next[i].size();j++)
			{
				printf("%d -> %d [label=\"%c\"];\n", this->id, next[i][j]->id, (i!=2 ? i+'a':'e'));
			}
		}
		if(this->id == finish)
		{
			printf("%d [shape=\"doublecircle\"];\n", id);
		}
		if(this->id == start)
        {
            printf("%d [label=\"start>%d\"];\n", id, id);
        }
	}
};

struct dfa_vertex
{
    int id;
    dfa_vertex* next[2];
    set<int> label;

    dfa_vertex(set<int> _label)
    {
        id = dfa_id++;
        label = _label;
        dfamap[label] = this;
        for( int i=0; i<2; ++i )
        {
            next[i] = NULL;
        }
    }

    ~dfa_vertex()
    {
        for( int i=0; i<2; ++i )
        {
            next[i] = NULL;
        }
    }

    void Debug(int id)
	{
		if(label.count(id))
		{
			printf("%d [shape=\"doublecircle\"]\n", this->id);
		}
        printf("%d [label=\"", this->id);
        set<int>::iterator t;
        for(t=label.begin();t!=label.end();t++)
        {
            if(t==label.begin())
                printf("%d", *t);
            else
                printf(",%d", *t);
        }
        printf("\"];\n");
		for(int i=0 ;i<2;i++)
		{
			if(next[i] != NULL)
				printf("%d -> %d [label=\"%c\"];\n", this->id, next[i]->id, i+'a');
		}
	}
};

void create_dfa(dfa_vertex* &dv, set<int> label)
{
    if(dfamap.count(label))
    {
        dv = dfamap[label];
    }
    else
    {
        dv = new dfa_vertex(label);
    }
}

void move_closure(set<int> &retval, set<int> ins, int character)
{
    retval.clear();
    set<int>::iterator it;
    for( it=ins.begin(); it!=ins.end(); ++it )
    {
        for(unsigned i=0;i<nfamap[*it]->next[character].size(); ++i)
        {
            retval.insert(nfamap[*it]->next[character][i]->id);
        }
    }
}

void eps_closure(set<int> &retval)
{
    stack<int> st;
    set<int>::iterator it;
    int tmp;
    for( it=retval.begin(); it!=retval.end(); ++it )
    {
        st.push(*it);
    }
    while(!st.empty())
    {
        int t = st.top(); st.pop();
        for(unsigned i=0; i<nfamap[t]->next[2].size(); ++i )
        {
            tmp = nfamap[t]->next[2][i]->id;
            if(!retval.count(tmp))
            {
                st.push(tmp);
                retval.insert(tmp);
            }
        }
    }
}

void reset()
{
    for( int i=0; i<nfa_id; ++i )
    {
        delete nfamap[i];
    }
    map<set<int>, dfa_vertex*>::iterator it;

    for( it=dfamap.begin(); it!=dfamap.end(); ++it )
    {
        delete it->second;
    }

    dfa_id = 0;
    nfa_id = 0;
    while(!ops.empty())
        ops.pop();
    while(!states.empty())
        states.pop();

    memset(nfamap,0,sizeof(nfamap));
    dfamap.clear();
}

int main()
{
    char input[202], query[77], op;
    int t, len;
    nfa_states operan1, operan2;
    while(scanf("%s", input)!=EOF)
    {
        reset();
        scanf("%d", &t);
        getchar();

        len = strlen(input);
        for( int i=0; i<len; ++i )
        {
            switch(input[i])
            {
            case '(':
                   continue;
                break;
            case 'a':
            case 'b':
                {
                    nfa_vertex* st = new nfa_vertex();
                    nfa_vertex* ed = new nfa_vertex();
                    st->next[input[i]-'a'].push_back(ed);
                    states.push(nfa_states(st->id, ed->id));
                }
                break;
            case ')':
                op = '-';
                if(ops.empty() == false)
                {
                    op = ops.top();
                    ops.pop();
                }
                switch(op)
                {
                case '|':
                    {
                        operan2 = states.top(); states.pop();
                        operan1 = states.top(); states.pop();
                        nfa_vertex* st = new nfa_vertex();
                        nfa_vertex* ed = new nfa_vertex();
                        st->next[2].push_back(nfamap[operan1.id_first]);
                        st->next[2].push_back(nfamap[operan2.id_first]);
                        nfamap[operan1.id_last]->next[2].push_back(ed);
                        nfamap[operan2.id_last]->next[2].push_back(ed);
                        states.push(nfa_states(st->id, ed->id));
                    }
                    break;
                case '.':
                    {
                        operan2 = states.top(); states.pop();
                        operan1 = states.top(); states.pop();
                        nfamap[operan1.id_last]->next[2].push_back(nfamap[operan2.id_first]);
                        states.push(nfa_states(operan1.id_first, operan2.id_last));
                    }
                    break;
                case '*':
                    {
                        operan2 = states.top(); states.pop();
                        nfa_vertex* st = new nfa_vertex;
                        nfa_vertex* ed = new nfa_vertex;

                        st->next[2].push_back(nfamap[operan2.id_first]);
                        nfamap[operan2.id_last]->next[2].push_back(st);
                        states.push(nfa_states(st->id, st->id));
                    }
                    break;
                }
                break;

            default:
                ops.push(input[i]);
            }
        }
        operan1 = states.top(); states.pop();
        int start = operan1.id_first;
        int finish = operan1.id_last;

        set<int> visited;
        set<int> retval;

        retval.insert(start);
        eps_closure(retval);
        dfa_vertex *tmp, *tmp1;
        dfa_vertex *initial_dfa;

        create_dfa(tmp, retval);
        visited.insert(tmp->id);
        initial_dfa = tmp;
        stack<dfa_vertex*> st;
        st.push(tmp);
        while(!st.empty())
        {
            tmp = st.top(); st.pop();
            for( int i=0; i<2; ++i )
            {
                move_closure(retval,tmp->label, i);
                eps_closure(retval);
                if(retval.size()==0)
                    continue;
                create_dfa(tmp1,retval);
                tmp->next[i] = tmp1;
                if(visited.count(tmp1->id)==0)
                {
                    visited.insert(tmp1->id);
                    st.push(tmp1);
                }
            }
        }
////      Debug
        for( int i=0; i<nfa_id ; ++i )
        {
            nfamap[i]->Debug(start,finish);
        }
        map<set<int>, dfa_vertex*>::iterator it_dfa;
        puts("\ndfa");
        for(it_dfa = dfamap.begin(); it_dfa!=dfamap.end(); ++it_dfa)
        {
            it_dfa->second->Debug(finish);
        }

        while(t--)
        {
            query[0]=0;
            scanf("%[^\n]", query);
            getchar();
            len = strlen(query);
            tmp = initial_dfa;
            int i;
            for(i=0; i<len; ++i )
            {
                tmp = tmp->next[query[i]-'a'];
                if( tmp==NULL )
                    break;
            }
            if(i!=len || !tmp->label.count(finish))
                printf("N\n");
            else
                printf("Y\n");
        }
        puts("");
    }
    return 0;
}
