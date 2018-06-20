#include <Util.h>
#include <stdbool.h>
#include <iostream>

bool IsAny(const char c, const char *s)
{
    while (*s)
    {
        if (c == *s++)
        {
            return true;
        }
    }
    return false;
}
using namespace std;

void PrintTokens(const std::vector<std::string> &tokens){
    for(auto it : tokens) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}

void InorderPrintAST(const ASTree *node)
{
    if(node == nullptr) {
        return;
    }
    InorderPrintAST(node->left_);
    for(std::size_t i = 0; i < node->cmds_.size(); ++i) {
        cout << node->cmds_[i] << " ";
    }
    cout << endl;
    InorderPrintAST(node->right_);
}