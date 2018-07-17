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
                    if(c == '&'){
                        node->left_->attribute_ |= FINT | FAND | FPRS;
                    }
                }
                return node;
        }
    }
    return node;
}

/**
 *
 * @param iter1 begin
 * @param iter2 end-of-past
 * @return syntax tree node
 */
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
                    node->right_->attribute_ |= FPIN;
                }
                return node;
        }
    }
    return Parse3(iter1, iter2);
}

/**
 *
 * @param iter1 begin
 * @param iter2 end-of-past
 * @return syntax node
 */

ASTree *Parse3(ConstIterator iter1, ConstIterator iter2) {
    ConstIterator iter;
    ConstIterator left = iter2, right = iter2;
    char c;
    int layer = 0;
    int error = 0;
    ASTree *input = nullptr;
    ASTree *output = nullptr;
    ASTree *node = new ASTree();
    int attr = 0;
    int ntok = 0;
    if((*iter2)[0] == ')') {
       attr |= FPAR;
    }
    for (iter = iter1; iter != iter2; iter++) {
        c = (*iter)[0];
        switch (c) {
            case '(':
                layer++;
                if(left != iter2) error++;
                left = iter+1;
                continue;
            case ')':
                layer--;
                if(right != iter2) error++;
                right = iter;
                continue;
            case '>':
                iter++;
                if ((*iter)[0] == '>')
                    attr |= FCAT;
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
                    }
                }
                else {
                    if (output) {
                        printf("syntax error\n");
                        return nullptr;
                    } else {
                        output = new ASTree();
                        output->cmds_.push_back(*iter);
                    }
                }
                continue;
            default:
                node->type_ = TELF;
                node->cmds_.push_back(*iter);
        }
    }
    if(left != iter2){
        if(ntok != 0)
            error++;
        node->type_ |= TPAR;
        goto OUT;
    }
    OUT:
        node->left_ = input;
        node->right_ = output;
        node->attribute_ = attr;
    return node;
}