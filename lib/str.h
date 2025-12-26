#ifndef __STR__
#define __STR__

#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>

#define MAX(a, b) (a < b ? b : a)
#define MAX_SIZE ULONG_MAX

typedef char char_t;

typedef struct string_s
{
    size_t len;
    size_t alloc;
    char_t *buf;

} string_t;

string_t *string_new(const char_t *s);
string_t *string_new_len(const char_t *s, size_t len);
string_t *string_new_sized(size_t len);

string_t *string_expand(string_t *s, size_t len);

string_t *string_toupper(string_t *str);
string_t *string_tolower(string_t *str);

string_t *string_rrim(string_t *str);
string_t *string_lrim(string_t *str);
string_t *string_trim(string_t *str);

string_t *string_insert_char(string_t *dst, char_t src, ssize_t pos);
string_t *string_insert_cstring(string_t *dst, const char_t *src, ssize_t pos);
string_t *string_insert_string(string_t *dst, const string_t *src,
                               ssize_t pos);
#define string_insert(dst, src, pos)                     \
    _Generic((src),                                      \
        int: string_insert_char,                         \
        char_t: string_insert_char,                      \
        char_t *: string_insert_cstring,                 \
        string_t *: string_insert_string)(dst, src, pos)
string_t *string_insert_len(string_t *dst, const char_t *src, ssize_t pos,
                            ssize_t len);

string_t *string_erase(string_t *str, size_t start, size_t len);

bool string_compare(const string_t *str1, const string_t *str2);

string_t *string_append_char(string_t *dst, char_t src);
string_t *string_append_cstring(string_t *dst, const char_t *src);
string_t *string_append_string(string_t *dst, const string_t *src);

#define string_append(dst, src)                     \
    _Generic((src),                                 \
        int: string_append_char,                    \
        char_t: string_append_char,                 \
        char_t *: string_append_cstring,            \
        string_t *: string_append_string)(dst, src)
string_t *string_append_len(string_t *str, const char_t *s, size_t len);

string_t *string_prepend_char(string_t *dst, char_t src);
string_t *string_prepend_cstring(string_t *dst, const char_t *src);
string_t *string_prepend_string(string_t *dst, const string_t *src);
#define string_prepend(dst, src)                     \
    _Generic((src),                                  \
        int: string_prepend_char,                    \
        char_t: string_prepend_char,                 \
        char_t *: string_prepend_cstring,            \
        string_t *: string_prepend_string)(dst, src)
string_t *string_prepend_len(string_t *str, const char_t *s, size_t len);

string_t** string_split(string_t *str, char_t sep);
void string_split_free(string_t **ar);

void string_free(string_t *str);
char_t *string_free_and_steal(string_t *str);

#endif // __STR__
