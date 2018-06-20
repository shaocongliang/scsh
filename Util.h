#ifndef UTIL_H
#define UTIL_H
#include <AST.h>

#include <string>
#include <vector>
// help function for debug
void PrintTokens(const std::vector<std::string> &toks);
// ditto
void InorderPrintAST(const ASTree *root);

bool IsAny(const char c, const char *s);
#endif