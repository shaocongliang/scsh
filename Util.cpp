#include <Util.h>
#include <stdbool.h>

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