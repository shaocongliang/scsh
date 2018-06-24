/**
 * @author amshaocong@outlook.com
 * @date 2018-06-23
 */
#ifndef AST_H
#define AST_H
#include <vector>
#include <string>
typedef enum{
    PIPE = 1,
    COMSEQ = 2,
    LEFT_REDIRECT = 4,
    RIGHT_REDIRECT = 8,
    ELF = 16,
    ARGUMENT = 32
}NodeType;

typedef enum
{
    REDIRECT = 1,
    REDIRECT_APPEND = 2
}NodeArrtibute;

struct ASTree;
struct ASTree{
    int type_;
    int attribute_;
    std::vector<std::string> cmds_;
    ASTree *left_;
    ASTree *right_;
    ASTree():left_(NULL), right_(NULL), type_(0), attribute_(0){
    }
};

typedef std::vector<std::string>::iterator Iterator;
typedef std::vector<std::string>::const_iterator ConstIterator;
ASTree* ConstructAbstractSyntaxTree(ConstIterator begin, ConstIterator end);
#endif