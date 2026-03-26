#ifndef SV_H
#define SV_H

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *data;
    size_t len;
} String_View;

#define SV_FMT "%.*s"
#define SV_ARG(s) (int)(s).len, (s).data
#define sv_to_cstr(s)                                                                              \
    ({                                                                                             \
        String_View _s = (s);                                                                      \
        char *_buf = alloca(_s.len + 1);                                                           \
        memcpy(_buf, _s.data, _s.len);                                                             \
        _buf[_s.len] = '\0';                                                                       \
        _buf;                                                                                      \
    })

String_View sv_from(const char *cstr);
String_View sv_from_parts(const char *data, size_t len);
size_t sv_find(String_View sv, char c);
void sv_chop_left(String_View *sv, size_t n);
void sv_chop_right(String_View *sv, size_t n);
void sv_trim_left(String_View *sv);
void sv_trim_right(String_View *sv);
void sv_trim(String_View *sv);
String_View sv_chop_by_delim(String_View *sv, char delim);
String_View sv_substring_by_delim(String_View *sv, char delim);
bool sv_eq(String_View a, String_View b);
bool sv_starts_with(String_View sv, String_View prefix);
void sv_println(String_View sv);

#ifdef SV_IMPLEMENTATION

String_View sv_from(const char *cstr) { return (String_View){.data = cstr, .len = strlen(cstr)}; }

String_View sv_from_parts(const char *data, size_t len) {
    return (String_View){.data = data, .len = len};
}

size_t sv_find(String_View sv, char c) {
    char *p = memchr(sv.data, c, sv.len);
    return p ? (size_t)(p - sv.data) : sv.len;
}

void sv_chop_left(String_View *sv, size_t n) {
    if (n > sv->len)
        n = sv->len;
    sv->data += n;
    sv->len -= n;
}

void sv_chop_right(String_View *sv, size_t n) {
    if (n > sv->len)
        n = sv->len;
    sv->len -= n;
}

void sv_trim_left(String_View *sv) {
    while (sv->len > 0 && isspace((unsigned char)sv->data[0])) {
        sv_chop_left(sv, 1);
    }
}

void sv_trim_right(String_View *sv) {
    while (sv->len > 0 && isspace((unsigned char)sv->data[sv->len - 1])) {
        sv_chop_right(sv, 1);
    }
}

void sv_trim(String_View *sv) {
    sv_trim_left(sv);
    sv_trim_right(sv);
}

String_View sv_chop_by_delim(String_View *sv, char delim) {
    size_t i = sv_find(*sv, delim);
    String_View result = sv_from_parts(sv->data, i);
    sv_chop_left(sv, i < sv->len ? i + 1 : sv->len);
    return result;
}

String_View sv_substring_by_delim(String_View *sv, char delim) {
    size_t i = sv_find(*sv, delim);
    return sv_from_parts(sv->data, i);
}

bool sv_eq(String_View a, String_View b) {
    if (a.len != b.len)
        return false;
    return memcmp(a.data, b.data, a.len) == 0;
}

bool sv_starts_with(String_View sv, String_View prefix) {
    if (prefix.len > sv.len)
        return false;
    return sv_eq(sv_from_parts(sv.data, prefix.len), prefix);
}

void sv_println(String_View sv) { printf("%.*s\n", (int)sv.len, sv.data); }

#endif // SV_IMPLEMENTATION
#endif // SV_H
