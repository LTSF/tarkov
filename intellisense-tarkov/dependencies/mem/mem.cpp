#include "mem.hpp"
#include <stdio.h>

extern "C"
{
#undef memset
    void* memset(void* dest, int c, size_t count)
    {
        char* bytes = (char*)dest;
        while (count--)
        {
            *bytes++ = (char)c;
        }
        return dest;
    }

#undef memcpy
    void* memcpy(void* dest, const void* src, size_t count)
    {
        char* dest8 = (char*)dest;
        const char* src8 = (const char*)src;
        while (count--)
        {
            *dest8++ = *src8++;
        }
        return dest;
    }

#undef memmove
    void* memmove(void* dst, const void* src, size_t len)
    {
        if (dst > src)
        {
            while (len > 0)
            {
                ((char*)dst)[len - 1] = ((char*)src)[len - 1];
                len--;
            }
        }
        else
            memcpy(dst, src, len);
        return (dst);
    }
}

int mem::cstrcmp(const char* a, const char* b)
{
	while (*a && *a == *b) 
    { 
        ++a; ++b; 
    }
	return static_cast<int>(static_cast<unsigned char>(*a)) - static_cast<int>(static_cast<unsigned char>(*b));
}

int mem::cstrlen(const char* str)
{
    int index = 0;
    while (str[index] != '\0') {
        index += 1;
    }
    return index;
}

char* mem::cstrcat(char* dest, const char* source)
{
    int index_of_src = 0;
    int index_of_dest = cstrlen(dest);

    while (source[index_of_src] != '\0') {
        dest[index_of_dest] = source[index_of_src];
        index_of_src += 1;
        index_of_dest += 1;
    }
    dest[index_of_dest] = '\0'; 

    return dest;
}


#ifndef _DEBUG
int atexit(void __attribute__((unused)) (*function)(void)) {
    return -1;
}
#endif
