#include "Util.h"
#include "AST.h"

#include <stdio.h>
#include <unistd.h>

char QUOTE = 0x80;
const int LINE = 4096;
const int MAXTOKS = 256;
char buf[LINE];
char *linep;
char *toks[MAXTOKS];
char **tokp;
char peekc;
int error;

char GetChar()
{
    char c;
    if (peekc)
    {
        c = peekc;
        peekc = 0;
    }
    else{
        read(STDIN_FILENO, &c, 1);
    }
    return c;
}

void Token()
{
    *tokp++ = linep;
    char c;
    char c1;
TOKEN:
    c = GetChar();
    switch (c)
    {
    case '\t': // if blank character
    case ' ':
        goto TOKEN;
    case '"':
    case '\'':
        c1 = c;
        while ((c = GetChar()) != c1)
        {
            if (c == '\n')
            {
                error++;
                peekc = c;
                return;
            }
            *linep++ = c;
        }
        goto NORMAL;
    case '(': // meta character
    case ')':
    case '&':
    case '|':
    case '>':
    case '<':
    case '^':
    case '\n':
        *linep++ = c;
        *linep++ = 0;
        return;
    }
    // Push Back non-meta character
    peekc = c;
NORMAL:
    for (;;)
    {
        c = GetChar();
        if (IsAny(c, " \"\t;&|<>^()'\n"))
        {
            peekc = c;
            if (IsAny(c, "\"'"))
            {
                goto TOKEN;
            }
            *linep++ = 0;
            return;
        }
        *linep++ = c;
    }
}
void print()
{
    int i = 0;
    for (char **p = toks; *p != 0; ++p)
    {
        printf("[%d] %s\n", i++, *p);
    }
}
void Interaction()
{
    char *cp;
    linep = buf;
    tokp = toks;
    do
    {
        cp = linep;
        Token();
    } while (*cp != '\n');

    if(error) {
        printf("syntax error\n");
        return;
    }
    struct ASTree *past = NULL;
    ConstructAbstractSyntaxTree(toks, tokp, &past);
}

int main(int argc, char **argv)
{
    for (;;)
    {
        Interaction();
        print();
    }
    return 0;
}