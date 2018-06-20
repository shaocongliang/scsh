#ifndef AST_H
#define AST_H
#include <vector>
#include <string>
typedef enum{
    PIPE = 1,
    CMDSEQ = 2,
    LEFT_REDIRECT = 4,
    RIGHT_REDIRECT = 8,
    ELF = 16,
    ARGUMENT = 32
}NodeType;

struct ASTree;
struct ASTree{
    int type_;
    std::vector<std::string> cmds_;
    ASTree *left_;
    ASTree *right_;
    ASTree():left_(NULL), right_(NULL){
    }
};
typedef std::vector<std::string>::iterator Iterator;
typedef std::vector<std::string>::const_iterator ConstIterator;
ASTree* ConstructAbstractSyntaxTree(ConstIterator begin, ConstIterator end);
#endif