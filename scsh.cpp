#include <Util.h>
#include <AST.h>
#include <Exec.h>

#include <string>

#include <stdio.h>
#include <unistd.h>

char QUOTE = 0x80;
const int LINE = 4096;
const int MAXTOKS = 256;
char buf[LINE];
char *linep;
char *tokp[MAXTOKS];
char peekc;
extern int error;

std::vector<std::string> toks;

char GetChar() {
    char c;
    if (peekc) {
        c = peekc;
        peekc = 0;
    } else {
        read(STDIN_FILENO, &c, 1);
    }
    return c;
}

void Token() {
    *tokp = linep;
    char c;
    char c1;
    TOKEN:
    c = GetChar();
    switch (c) {
        case '\t': // if blank character
        case ' ':
            goto TOKEN;
        case '"':
        case '\'':
            c1 = c;
            while ((c = GetChar()) != c1) {
                if (c == '\n') {
                    error++;
                    peekc = c;
                    return;
                }
                *linep++ = c;
            }
            goto NORMAL;
        case ';':
        case '(':
        case ')':
        case '&':
        case '|':
        case '>':
        case '<':
        case '^':
        case '\n':
            *linep++ = c;
            *linep++ = 0;
            toks.push_back(*tokp);
            return;
    }
    // Push Back non-meta character
    peekc = c;
    NORMAL:
    for (;;) {
        c = GetChar();
        if (IsAny(c, " \"\t;&|<>^()'\n")) {
            peekc = c;
            if (IsAny(c, "\"'")) {
                goto TOKEN;
            }
            *linep++ = 0;
            toks.push_back(*tokp);
            return;
        }
        *linep++ = c;
    }
}

void Interaction() {
    char *cp;
    linep = buf;
    do {
        cp = linep;
        Token();
    } while (*cp != '\n');

    if (error) {
        printf("syntax error\n");
        return;
    }
    ASTree *ast = ConstructAbstractSyntaxTree(toks.cbegin(), toks.cend());
    Exec(ast, nullptr, nullptr);
    delete ast;
    toks.clear();
}

int main(int argc, char **argv) {
    std::string prompt = "$ ";
    uid_t uid = getuid();
    if(uid == 0) prompt[0] = '#';
    for (;;) {
        PutChar(prompt.c_str());
        Interaction();
    }
    return 0;
}