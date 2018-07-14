/**
 * @author amshaocong@outlook.com
 * @date 2018-06-23
 */
#include <AST.h>
#include <Util.h>
#include <stdlib.h>
#include <assert.h>


ASTree *Parse1(ConstIterator iter1, ConstIterator iter2);

ASTree *Parse2(ConstIterator iter1, ConstIterator iter2);

ASTree *Parse3(ConstIterator iter1, ConstIterator iter2);

ASTree *ConstructAbstractSyntaxTree(ConstIterator begin, ConstIterator end) {
    ConstIterator iter1 = begin;
    ConstIterator iter2 = end;
    while (iter1 != iter2) {
        if (IsAny((*iter1)[0], ";&\n")) {
            iter1++;
        } else {
            return Parse1(iter1, iter2);
        }
    }
    return nullptr;
}

// 从左至右解析整个命令行，左子树下推
// 右子树递归处理
ASTree *Parse1(ConstIterator iter1, ConstIterator iter2) {
    ConstIterator iter;
    int l = 0;
    struct ASTree *node = new ASTree();
    for (iter = iter1; iter != iter2; iter++) {
        char c = (*iter)[0];
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
                if (l == 0) {
                    node->type_ = TLIST;
                    node->left_ = Parse2(iter1, iter);
                    node->right_ = ConstructAbstractSyntaxTree(iter + 1, iter2);
                }
                return node;
        }
    }
    return node;
}

// 处理管道线类型，左边下推
// 右边递归处理
ASTree *Parse2(ConstIterator iter1, ConstIterator iter2) {
    ASTree *node = new ASTree();
    char c;
    int l = 0;
    ConstIterator iter;
    for (iter = iter1; iter != iter2; ++iter) {
        c = (*iter)[0];
        switch (c) {
            case '(':
                l++;
                continue;
            case ')':
                l--;
                continue;
            case '|':
            case '^':
                if (l == 0) {
                    node->type_ = TPIPE;
                    node->cmds_.push_back(*iter);
                    node->left_ = Parse3(iter1, iter);
                    node->right_ = Parse2(iter + 1, iter2);
                }
                return node;
        }
    }
    return Parse3(iter1, iter2);
}

ASTree *Parse3(ConstIterator iter1, ConstIterator iter2) {
    ConstIterator iter;
    char c;
    ASTree *input = nullptr;
    ASTree *output = nullptr;
    ASTree *node = new ASTree();
    for (iter = iter1; iter != iter2; iter++) {
        c = (*iter)[0];
        switch (c) {
            case '>':
                iter++;
                if ((*iter)[0] == '>')
                    node->attribute_ |= FCAT;
                else
                    iter--;
            case '<':
                iter++;
                if(c == '<'){
                    if(input){
                        printf("syntax error\n");
                        return nullptr;
                    }
                    else {
                        input = new ASTree();
                        input->cmds_.push_back(*iter);
                        node->left_ = input;
                    }
                }
                else {
                    if (output) {
                        printf("syntax error\n");
                        return nullptr;
                    } else {
                        output = new ASTree();
                        output->cmds_.push_back(*iter);
                        node->right_ = output;
                    }
                }
                continue;
            default:
                node->type_ = TELF;
                node->cmds_.push_back(*iter);
        }
    }
    return node;
}