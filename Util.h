/**
 * @author amshaocong@outlook.com
 * @date 2018-06-23
 */
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

void FindComFullPath(const std::string &com, std::string &fullpath);
void PutChar(const char *c);
#endif