#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "checker.hpp"
#include<stdio.h>
#include<vector>
#include "Tools.h"
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include "BooleanNet.hpp"
class interface :private Tools
{
public:
    interface(string);
    string tracefile;
    int length_explored;
    int trace_num;
    int CheckTrace(vector<string> varName, vector<vector<int> > trace);
private:
    int varNum;
    int traceLength;
    string propFile;
    vector<string> state_vars;
    vector<pair<string, double> >state;
    void read_property(char *);
    void init_signals(vector<string>);
    valType advance(vector<double>);
    Checker *c;
};
#endif
