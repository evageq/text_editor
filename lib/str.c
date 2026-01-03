#include "str.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline size_t
near_pow(size_t n)
{
    n -= 1;
    assert(n > 0 && n <= MAX_SIZE / 2);
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;

    return n + 1;
}

string_t *
string_new(const char_t *const s)
{
    string_t *str;
    if (s == NULL || *s == '\0')
    {
        str = string_new_len(s, 0);
    }
    else
    {
        str = string_new_len(s, strlen(s));
    }

    return str;
}

string_t *
string_new_len(const char_t *const s, size_t len)
{
    string_t *str;

    if (s == NULL || *s == '\0' || len == 0)
    {
        str = string_new_sized(2);
        str->buf[0] = '\0';
    }
    else
    {
        str = string_new_sized(len);
        string_append_len(str, s, len);
    }

    return str;
}

string_t *
string_new_sized(size_t len)
{
    string_t *str = malloc(sizeof(*str));
    if (str == NULL)
    {
        abort();
    }
    *str = (string_t){};
    string_expand(str, MAX(len, 64));
    return str;
}

static inline string_t *
string_maybe_expand(string_t *str, size_t len)
{
    if (str->len + len >= str->alloc)
    {
        string_expand(str, len);
    }

    return str;
}

string_t *
string_expand(string_t *str, size_t len)
{
    if (str->len + len >= MAX_SIZE)
    {
        exit(-1);
    }

    size_t alloc = near_pow(str->len + len + 1);
    if (alloc == 0)
    {
        alloc = str->len + len;
    }

    char_t *p_tmp_buf = realloc(str->buf, alloc);
    if (p_tmp_buf == NULL)
    {
        abort();
    }
    str->buf = p_tmp_buf;
    str->alloc = alloc;

    return str;
}

inline string_t *
string_insert_char(string_t *dst, const char_t c, ssize_t pos)
{
    return string_insert_len(dst, &c, 1, pos);
}

inline string_t *
string_insert_cstring(string_t *dst, const char_t *s, ssize_t pos)
{
    return string_insert_len(dst, s, strlen(s), pos);
}

inline string_t *
string_insert_string(string_t *dst, const string_t *s, ssize_t pos)
{
    return string_insert_len(dst, s->buf, s->len, pos);
}

string_t *
string_insert_len(string_t *dst, const char_t *src, ssize_t len, ssize_t pos)
{
    size_t unsigned_pos = pos;
    size_t unsigned_len = len;

    if (len == 0)
    {
        return dst;
    }

    if (len < 0)
    {
        unsigned_len = strlen(src);
    }

    if (pos == -1 || pos >= dst->len)
    {
        unsigned_pos = dst->len;
    }

    string_maybe_expand(dst, unsigned_len);
    if (!(dst->buf <= src && src <= dst->buf + dst->len))
    {
        memmove(dst->buf + unsigned_pos + unsigned_len,
                dst->buf + unsigned_pos, dst->len - unsigned_pos);
        memcpy(dst->buf + unsigned_pos, src, unsigned_len);
    }
    else
    {
        string_t *tmp = string_new_len(src, unsigned_len);
        string_insert_len(dst, tmp->buf, len, pos);
        string_free(tmp);
        return dst;
    }

    dst->len += unsigned_len;
    dst->buf[dst->len] = 0;

    return dst;
}

inline string_t *
string_append_char(string_t *dst, const char_t c)
{
    return string_insert_char(dst, c, -1);
}

inline string_t *
string_append_cstring(string_t *dst, const char_t *s)
{
    return string_insert_cstring(dst, s, -1);
}

inline string_t *
string_append_string(string_t *dst, const string_t *src)
{
    return string_insert_string(dst, src, -1);
}

inline string_t *
string_append_len(string_t *str, const char_t *s, size_t len)
{
    return string_insert_len(str, s, len, -1);
}

inline string_t *
string_prepend_char(string_t *dst, const char_t c)
{
    return string_insert_char(dst, c, 0);
}

inline string_t *
string_prepend_cstring(string_t *dst, const char_t *s)
{
    return string_insert_cstring(dst, s, 0);
}

inline string_t *
string_prepend_string(string_t *dst, const string_t *src)
{
    return string_insert_string(dst, src, 0);
}

inline string_t *
string_prepend_len(string_t *str, const char_t *s, size_t len)
{
    return string_insert_len(str, s, len, 0);
}

static inline bool
is_trim(char_t ch)
{
    return (ch == ' ' || !isprint(ch));
}

string_t *
string_ltrim(string_t *str)
{
    size_t i = 0;
    while (i < str->len && is_trim(str->buf[i]))
    {
        ++i;
    }
    memmove(str->buf, str->buf + i, str->len - i);
    str->len -= i;
    str->buf[str->len] = 0;

    return str;
}

string_t *
string_rtrim(string_t *str)
{
    if (str->len == 0)
    {
        return str;
    }

    size_t i = str->len - 1;
    size_t cnt = 0;
    while (is_trim(str->buf[i]))
    {
        ++cnt;
        if (i == 0)
        {
            break;
        }
        --i;
    }
    str->len = str->len - cnt;
    str->buf[str->len] = 0;

    return str;
}

string_t *
string_trim(string_t *str)
{
    string_ltrim(str);
    string_rtrim(str);

    return str;
}

string_t *
string_toupper(string_t *str)
{
    for (size_t i = 0; i < str->len; ++i)
    {
        str->buf[i] = toupper(str->buf[i]);
    }

    return str;
}

string_t *
string_tolower(string_t *str)
{
    for (size_t i = 0; i < str->len; ++i)
    {
        str->buf[i] = tolower(str->buf[i]);
    }

    return str;
}

/* a < b */
bool
string_compare(const string_t *str1, const string_t *str2)
{
    size_t n = MIN(str1->len, str2->len);

    bool flag = true;
    for (size_t i = 0; i < n; ++i)
    {
        if (str1->buf[i] != str2->buf[i])
        {
            return str1->buf[i] < str2->buf[i];
        }
    }

    return false;
}

int
string_compare_lexic(const string_t *str1, const string_t *str2)
{
    if (str1->len == str2->len && memcmp(str1->buf, str2->buf, str1->len) == 0)
    {
        return STRING_COMPARE_EQUAL;
    }
    else
    {
        if (string_compare(str1, str2) == true)
        {
            return STRING_COMPARE_TRUE;
        }
        else
        {
            return STRING_COMPARE_FALSE;
        }
    }
}

string_t *
string_erase(string_t *str, size_t start, ssize_t len)
{
    size_t end;

    if (len <= 0 || start + len >= str->len)
    {
        end = str->len;
    }
    else
    {
        end = start + len;
    }

    if (start >= str->len || end - start <= 0)
    {
        return str;
    }

    assert(end <= str->len);

    string_t *tmp = string_new(str->buf + end);
    str->len = str->len - (end - start);
    memcpy(str->buf + start, tmp->buf, tmp->len);
    str->buf[str->len] = '\0';
    string_free(tmp);

    return str;
}

string_t **
string_split(string_t *str, char_t sep)
{
#define STRCHR_LOOP(p_ch, p_str, sep)                                 \
    for (p_ch = p_str->buf;                                           \
         (p_ch = memchr(p_ch, sep, p_str->len - (p_ch - p_str->buf))) \
         != NULL;                                                     \
         ++p_ch)

    size_t cnt = 1; // store NULL
    char_t *p_ch;

    // count last divided unchecked string part
    if (memchr(str->buf, sep, str->len) != NULL)
    {
        ++cnt;
    }

    STRCHR_LOOP(p_ch, str, sep)
    {
        ++cnt;
    }

    string_t **str_split_ar = malloc(sizeof(*str_split_ar) * cnt);
    if (str_split_ar == NULL)
    {
        abort();
    }
    str_split_ar[cnt - 1] = NULL;

    size_t pos = 0;
    ssize_t prev_pos = -1;
    size_t id = 0;
    STRCHR_LOOP(p_ch, str, sep)
    {
        size_t pos = p_ch - str->buf;
        size_t chunk_len = pos - prev_pos - 1LL;
        str_split_ar[id++]
            = string_new_len(str->buf + prev_pos + 1, chunk_len);
        prev_pos = pos;
        assert(id <= cnt - 1);
    }
    str_split_ar[id]
        = string_new_len(str->buf + prev_pos + 1, str->len - prev_pos - 1LL);

    return str_split_ar;
#undef STRCHR_LOOP
}

void
string_split_free(string_t **ar)
{
    string_t *tmp;
    size_t i = 0;
    while ((tmp = ar[i++]) != NULL)
    {
        string_free(tmp);
    }
    free(ar);
}

void
string_free(string_t *str)
{
    free(str->buf);
    free(str);
}

char_t *
string_free_and_steal(string_t *str)
{
    char_t *s = str->buf;

    free(str);

    return s;
}
