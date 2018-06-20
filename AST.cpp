#include <AST.h>
#include <Util.h>
#include <stdlib.h>
#include <assert.h>

ASTree* Parse1(char **p1, char **p2);
ASTree* Parse2(char **p1, char **p2);
ASTree* Parse3(char **p1, char **p2);

struct ASTree *g_AST;
ASTree* findNodeWithRightLeaf(ASTree *node) {
    assert(node != nullptr);
    if(node->right_ == nullptr) {
        return node;
    }
    findNodeWithRightLeaf(node->right_);
}

ASTree* ConstructAbstractSyntaxTree(char **toks, char **toke){
    char **p1 = toks;
    char **p2 = toke;
    while(p1 != p2) {
        if(IsAny(**p1, ";&")) {
           p1++; 
        }
        else {
           return Parse1(p1, p2);
        }
    }
    return nullptr;
}

// 从左至右解析整个命令行，左子树下推
// 右子树递归处理
ASTree* Parse1(char **p1, char **p2) {
   char **p;
   int l = 0;
   struct ASTree *node = new ASTree(); 
   for(p = p1; p != p2; p++) {
       char c = **p;
       switch (c) {
            case '(':
                l++;
                continue;
            case ')':
                l--;
                continue;
            case ';':
            case '&':
            case '\n':
                if(l == 0) {
                    if(g_AST == NULL) {
                        g_AST = node;
                    }
                    node->type_ = CMDSEQ;
                    node->left_ = Parse2(p1, p);
                    node->right_ = ConstructAbstractSyntaxTree(p+1, p2);
                }
                return node;
       }
   } 
   return node;
}

// 处理管道线类型，左边下推
// 右边递归处理
struct ASTree* Parse2(char **p1, char **p2) {
    ASTree *node = new ASTree();
    char **p;
    char c;
    int l = 0;
    for(p = p1; p != p2; ++p) {
       c = **p;
       switch(c) {
           case '(':
            l++;
            continue;
           case ')':
            l--;
            continue;
           case '|':
           case '^':
           if(l == 0) {
               node->type_ = PIPE;
               node->left_ = Parse3(p1, p);
               node->right_ = Parse2(p+1, p2);
           }
       } 
    }
    return Parse3(p1, p2);
}

struct ASTree* Parse3(char **p1, char **p2) {
    char **p;
    char c;
    int i = 0;
    ASTree *node = new ASTree();
    for(p = p1; p != p2; p++) {
        c = **p;
        switch(c) {
            case '>':
            case '<':
                continue;
            default:
                node->type_ = ELF;
                node->cmds_.push_back(std::string(*p));
        }
    }
    return node;
}