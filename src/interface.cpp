/**
 * the interface to the external software (C code) which wants to use the online model
 * checker
 */
#include "interface.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
extern int yyparse(void *YYPARSE_PARAM);
#else
extern int yyparse();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
extern int yyparse(void);
#else
extern int yyparse();
#endif
#endif /* ! YYPARSE_PARAM */
extern FILE *yyin;
interface::interface(string File)
{
    trace_num = 0;
    propFile = File;
    yyin = fopen(propFile.c_str(), "r");
    if (!yyin)
    {
        fprintf(stderr, "ERROR: Property file does not exist\n");
        exit(EXIT_FAILURE);
    }
    yyparse();
    fclose(yyin);
}
/**
 * assuming the external software is a C code

extern "C" {
  void read_property(char *);
  void init_signals(vector<string>);
  valType advance(vector<double>);
}*/

/**
 * reads the BLTL property specified in the file whose path is specified in the
 * parameter `file'
 *
 * ideally, this has to be the first thing to do
 */
void interface::read_property(char *file)
{

    yyin = fopen(file, "r");
    if (!yyin)
    {
        fprintf(stderr, "ERROR: Property file does not exist\n");
        exit(EXIT_FAILURE);
    }
    yyparse();
    fclose(yyin);
    c = NULL;
}

/**
 * register all the signals whose values appear in the trace
 *
 * parameter `list' contains the list of character strings of the
 * alphanumeric and case-sensitive names of the signals, in the exact order as
 * they appear in the trace
 *
 * parameter `len' is the number of signals in the parameter `list'
 *
 * a call to this has to precede a call to `advance' to check the trace
 */
void interface::init_signals(vector<string> list)
{
    state_vars.clear();
    for (int i = 0; i < list.size(); i++)
    {
        state_vars.push_back(list[i]);
#ifdef VERBOSE
        cout << list[i] << " ";
#endif
    }
#ifdef VERBOSE
    cout << endl;
    length_explored = 0;
#endif
    /* create a new checker */
    delete c;
    c = new Checker();
}
/**
 * advances model checking the trace by one step
 *
 * parameter `new_state' is the sequence of double values corresponding to the
 * signals
 *
 * parameter `len' is the length of the sequence `new_state'
 *
 * returns
 *   SAT/UNSAT if the property satisfiability can be decided at this point on
 *             the trace
 *   DONT_KNOW if the trace has to explored further
 */
valType interface::advance(vector<double> new_state)
{
#ifdef VERBOSE
    length_explored++;
    cout << "step " << length_explored << endl;
#endif
    double ts = new_state[0];
    valType result;
#ifdef VERBOSE
    cout << "timestamp : " << ts << endl;
    cout << ts << " ";
#endif
    for (int i = 0; i < new_state.size() - 1; i++)
    {
        state.push_back(make_pair(state_vars.at(i), new_state[i + 1]));
#ifdef VERBOSE
        cout << state_vars[i] << " ";
#endif
    }
#ifdef VERBOSE
    cout << endl;
#endif
    result = c->advance(state, ts);
    state.clear();
    return result;
}
int interface::CheckTrace(vector<string> varName, vector<vector<int> > trace)
{
    //cout<<endl;
    state.clear();
    state_vars.clear();
    varNum = varName.size();
    traceLength = trace.size();
    valType t;
    double ts;
    c = new Checker();
    //cout<<"check"<<endl;
    ts = 0;
    
    for(int k=0;k<traceLength;k++)
    {
        if(k==0)
        {
            for (int i = 0; i < varName.size(); i++)
            {
                state_vars.push_back(varName[i]);
                //cout<<varName[i]<<" ";
            }
        }
        for(int j=0;j<varNum;j++)
        {
            state.push_back(make_pair(state_vars[j], double(trace[k][j])));
        }
        if ((t = (*c).advance(state, ts)) == SAT)
        {
            return 1;
        }
        else if (t == UNSAT)
        {
            return 0;
        }
        state.clear();
        ts += 1;
        /**/
    }
    return -1;
}