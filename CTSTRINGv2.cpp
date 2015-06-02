#include <cstdio>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <map>
#include <stack>
#include <vector>

#define MOD_CONSTANT 1000000007
#define MAX_NODE 200

using namespace std;

template <typename T>
struct dfa_label{
    unsigned long long hash_id;
    T dt[101];
    int data;

	// initialize label without id
	dfa_label()
    {
        hash_id = 0;
        data = 0;
    }

	// initialize label with id

	// struct comparator with '==' operator
	// if dt equal return true, else return false
    bool operator==(const dfa_label &o) const
	{
		return hash_id == o.hash_id;
    }

	// struct comparator with '==' operator
    // if |data| < cmp.|data| ret true;
	// if data[i] < cmp.data[i] ret true;
	// else ret false;
	bool operator<(const dfa_label &o) const
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
            hash_id=hash_id*211+dt[i];
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
		hash_id = 0;
        data = 0;
    }

};

struct pairint
{
    int a;
    int b;
    pairint(){ }
    pairint(int _a, int _b)
    {
        a = _a;
        b = _b;
    }
};

stack<pairint> states;
vector<int> nfa_nodes[MAX_NODE][3];
map<dfa_label<int>, int> dfa_graph;
dfa_label<int> dfa_lbl[MAX_NODE];
int dfa[MAX_NODE][2];
int nid = 0;
int did = 0;
pairint operan1, operan2, state;

void MoveClosure(dfa_label<int> node, int symbol, dfa_label<int> &retval)
{
    retval.clear();
    for( int i=0; i<node.data; i++)
    {
        for(int j=0;j<nfa_nodes[node.dt[i]][symbol].size();j++)
        {
            retval.push(nfa_nodes[node.dt[i]][symbol][j]);
        }
    }
	retval.CalculateHash();
}

void EpsilonClosure(dfa_label<int> &retval)
{
    stack<int> nodes;
    int tmp;
    for( int i=0; i < retval.size();i++)
    {
        nodes.push(retval.dt[i]);
    }
    while(nodes.empty() == false)
    {
        int t = nodes.top();
        nodes.pop();
        for(int i=0; i<nfa_nodes[t][2].size(); i++)
        {
            tmp = nfa_nodes[t][2][i];
            if(retval.count(tmp) == 0)
            {
                retval.push(tmp);
                nodes.push(tmp);
            }
        }
    }
	retval.CalculateHash();
}

int CreateDFAState(dfa_label<int> label)
{
	if(dfa_graph.count(label) == 1)
		return dfa_graph[label];

    for(int i=0;i<label.data;i++)
    {
        dfa_lbl[did].push(label.dt[i]);
    }
    dfa_lbl[did].CalculateHash();
    dfa_graph[dfa_lbl[did]] = did;
    did++;
    return did-1;
}

void MatrixMultiply(long long** mat1, long long** mat2)
{
    long long result[did][did];
    int i,j,k;
    for(i=0;i<did;i++)
    {
        for(j=0;j<did;j++)
        {
            result[i][j]=0LL;
        }
    }
    for(i=0;i<did;i++)
    {
        for(j=0;j<did;j++)
        {
            for(k=0;k<did;k++)
            {
                if(mat1[i][k] == 0 || mat2[k][j] == 0 ) continue;
                result[i][j]=(result[i][j]+((mat1[i][k])*(mat2[k][j])))%MOD_CONSTANT;
            }
        }
    }
    for(i=0;i<did;i++)
    {
        for(j=0;j<did;j++)
        {
            mat1[i][j]=result[i][j];
        }
    }
}

void MatrixPower(long long** adjM, int L, long long** result)
{
    for(int i=0;i<did;i++)
    {
        for(int j=0;j<did;j++)
        {
            result[i][j]=(i==j);
        }
    }
    while(L){
        if (L&1)
        {
            MatrixMultiply(result, adjM);
        }
        MatrixMultiply(adjM, adjM);
        L>>=1;
    }
}

void reset()
{
    nid = 0;
    did = 0;

    memset(dfa, -1, sizeof(dfa));
    memset(dfa_lbl, 0, sizeof(dfa_lbl));

    for(int i=0;i<MAX_NODE;i++)
    {
        for(int j=0;j<3;j++)
        {
            nfa_nodes[i][j].clear();
        }
    }
    while(!states.empty())
        states.pop();
    dfa_graph.clear();
}

void Preprocess(char in[], char input[])
{
    int len = strlen(in);
    int iter=0;
    for(int i=0;i<len; i++)
    {
        if(i!=0 && ( in[i]=='(' || in[i]=='a' || in[i] == 'b') && (in[i-1]==')' || in[i-1]=='a' || in[i-1]=='b'))
        {
            input[iter++]='.';
        }
        input[iter++]=in[i];
    }
    input[iter]=0;
}

void ConvertREtoNFA(char input[])
{
    int len = strlen(input);
    stack<char> ops;
    char op;
    for(int i=0;i<len;i++)
        {
            switch(input[i])
            {
            case '(':
                   continue;
                break;
            case 'a':
            case 'b':
                {
                    nfa_nodes[nid][input[i]-'a'].push_back(nid+1);
                    states.push(pairint(nid, nid+1));
                    nid+=2;
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

                        nfa_nodes[nid][2].push_back(operan1.a);
                        nfa_nodes[nid][2].push_back(operan2.a);
                        nfa_nodes[operan1.b][2].push_back(nid+1);
                        nfa_nodes[operan2.b][2].push_back(nid+1);
                        states.push(pairint(nid, nid+1));
                        nid+=2;
                    }
                    break;
                case '.':
                    {
                        operan2 = states.top(); states.pop();
                        operan1 = states.top(); states.pop();
                        nfa_nodes[operan1.b][2].push_back(operan2.a);
                        states.push(pairint(operan1.a, operan2.b));
                    }
                    break;
                case '*':
                    {
                        operan2 = states.top(); states.pop();
                        nfa_nodes[nid][2].push_back(operan2.a);
                        nfa_nodes[operan2.b][2].push_back(nid);
                        states.push(pairint(nid, nid));
                        nid++;
                    }
                    break;
                }
                break;

            default:
                ops.push(input[i]);
            }
        }
}

void ConvertNFAtoDFA(pairint state)
{
    int dv, initDFA, NFAstart = state.a, tmp;
    stack<int> st;

    dfa_label<int> visited;
    dfa_label<int> retval;

    retval.push(NFAstart);
    EpsilonClosure(retval);

    initDFA = CreateDFAState(retval);
    visited.push(initDFA);
    st.push(initDFA);
    while(st.empty() == false)
    {
        dv = st.top();
        st.pop();
        for(int i=0; i<2; i++)
        {
            MoveClosure((dfa_lbl[dv]),i, retval);
            EpsilonClosure(retval);
            if(retval.data == 0) continue;
            tmp = CreateDFAState(retval);
            dfa[dv][i] = tmp;
            if(visited.count(tmp) == 0)
            {
                visited.push(tmp);
                st.push(tmp);
            }
        }
    }
}
int main()
{
    char regexp[121], input[222], op;
    int dv, dv2, len, L, nfaFinish, test;
    for(scanf("%d", &test);test--;)
    {
        // initialize Variables
        reset();
        // Input
        scanf("%s %d", regexp, &L);
        // Preprocess Input
        Preprocess(regexp, input);
        // Begin parsing RE into NFA
        ConvertREtoNFA(input);
        // Get NFA automaton
        state = states.top();
        states.pop();
        nfaFinish = state.b;
        // Convert NFA to DFA
        ConvertNFAtoDFA(state);
        // Create adjacency matrix from DFA Graph.
        long long** adjMatrix, **result;
        map<dfa_label<int>, int>::iterator it_dfa;

        adjMatrix = (long long**)calloc(did, sizeof(long long*));
        result = (long long**)calloc(did, sizeof(long long*));
        for(int i=0;i<did;i++)
        {
            adjMatrix[i] = (long long*)calloc(did, sizeof(long long));
            result[i] = (long long*)calloc(did, sizeof(long long));
        }
		for(it_dfa = dfa_graph.begin(); it_dfa!=dfa_graph.end(); it_dfa++)
        {
            dv = it_dfa->second;
            for(int i=0;i<2;i++)
            {
                dv2 = dfa[dv][i];
                if( dv2 !=-1 )
                {
                    adjMatrix[dv][dv2]++;
                }
            }
        }

        // Perform matrix multiplication
        MatrixPower(adjMatrix, L, result);
        // sum cell of the matrix which have finish state of NFA
        long long res = 0;
        for(it_dfa=dfa_graph.begin();it_dfa != dfa_graph.end(); it_dfa++)
        {
            if(dfa_lbl[it_dfa->second].count(nfaFinish))
            {
                res += result[0][it_dfa->second];
            }

        }
        printf("%I64d\n", res%MOD_CONSTANT);
    }
    return 0;
}
