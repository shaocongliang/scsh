#include "AST.h"
#include "Util.h"
#include <stdlib.h>
#include <assert.h>

void Parse1(char **p1, char **p2);
struct ASTree* Parse2(char **p1, char **p2);
struct ASTree* Parse3(char **p1, char **p2);

struct ASTree *g_AST;
ASTree* findNodeWithRightLeaf(ASTree *node) {
    assert(node != nullptr);
}
int ConstructAbstractSyntaxTree(char **toks, char **toke){
    char **p1 = toks;
    char **p2 = toke;
    while(p1) {
        if(IsAny(**p1, ";&")) {
           p1++; 
        }
        else {
           Parse1(p1, p2);
        }
    }
    return 0;
}

// 从左至右解析整个命令行，左子树下推
// 右子树递归处理
void Parse1(char **p1, char **p2) {
   char **p;
   int l = 0;
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
                if(l == 0) {
                    if(g_AST == nullptr) {
                        g_AST = new ASTree();
                        g_AST->type_ = CMDSEQ;
                        g_AST->left_ = Parse2(p1, p);
                        g_AST->right_ = Parse3(p+1, p2);
                    }
                    else {
                        ASTree *node = findNodeWithRightLeaf(g_AST);
                    }
                }
       }
   } 
}

// 处理管道线类型，左边下推
// 右边递归处理
struct ASTree* Parse2(char **p1, char **p2) {
    char **p;
    char c;
    int l = 0;
    for(p = p1; p != p2; ++p) {
       c = **p;
       switch(c) {
           case '|':
           case '^':
           if(l == 0) {
               struct ASTree *past = malloc(sizeof(struct ASTree));
               past->type = PIPE;
               past->val = NULL;
               past->left = Parse3(p1, p);
               past->right = Parse3(p+1, p2);
           }
       } 
    }
    return Parse3(p1, p2);
}

struct ASTree* Parse3(char **p1, char **p2) {
    char **p;
    char c;
    int i = 0;
    struct ASTree *past = NULL;
    for(p = p1; p != p2; p++) {
        c = **p;
        switch(c) {
            case '':
            default:
                if(past == NULL) {
                    past = malloc(sizeof(struct ASTree));
                    past->pcmd = p;
                    past->type = ELF;
                    past->left = NULL;
                    past->right = NULL;
                }
                else {
                }
        }
    }
}