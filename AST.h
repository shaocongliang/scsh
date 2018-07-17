/**
 * @author amshaocong@outlook.com
 * @date 2018-06-23
 */
#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

// 节点类型
#define TELF 1
#define TPIPE 2
#define TLIST 4
#define TPAR 8

// 节点属性
#define FPIN 1
#define FPOUT 2
#define FCAT 4
#define FINT 8
#define FAND 16
#define FPRS 32
#define FPAR 64 // composite command

struct ASTree;

struct ASTree {
    int type_;
    int attribute_;
    std::vector<std::string> cmds_;
    ASTree *left_;
    ASTree *right_;

    ASTree() : left_(NULL), right_(NULL), type_(0), attribute_(0) {
    }

    ~ASTree() {
        delete left_;
        delete right_;
    }
};

typedef std::vector<std::string>::iterator Iterator;
typedef std::vector<std::string>::const_iterator ConstIterator;

ASTree *ConstructAbstractSyntaxTree(ConstIterator begin, ConstIterator end);

#endif