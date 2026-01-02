#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include "str.h"
#include <assert.h>
#include <limits.h>
#include <ncurses.h>
#include <stdio.h>

#define LINE_BUFFER_LEN 4096
#define FILE_NAME_LEN 4096

#define TEXT_PTR_X(t_ptr) (t_ptr.pos)
#define TEXT_PTR_Y(t_ptr) (t_ptr.p_line->id)
#define TEXT_PTR_IS_BOUND(t_ptr) (TEXT_PTR_X(t_ptr) + 1 >= t_ptr.p_str->len)

#define MIN(a, b) (a < b ? a : b)

extern WINDOW *top_win;
extern WINDOW *text_win;
extern WINDOW *bottom_win;

extern char file_name_arg[];
extern bool running;

typedef struct str_line_s
{
    string_t *p_str;
    struct str_line_s *next;
    struct str_line_s *prev;

} str_line_t;

typedef struct cursor_s
{
    str_line_t *line;
    int x;
    int y;
} cursor_t;

typedef enum editor_colored_entities_e
{
    TITLE_BAR = 1,
    EDITOR_COLORED_ENTITIES_MAX,
} editor_colored_entities_t;
_Static_assert(EDITOR_COLORED_ENTITIES_MAX < 255, "ncurses colors max exceed");

typedef enum editor_actions_e
{
    E_KEY_UP = KEY_UP,
    E_KEY_DOWN = KEY_DOWN,
    E_KEY_LEFT = KEY_LEFT,
    E_KEY_RIGHT = KEY_RIGHT,
    E_PASTE,
    E_COPY,
    E_NEXT_WORD,
    E_PREV_WORD,
    E_SAVE,
    E_MAX
} editor_actions_t;

typedef struct file_handle_s
{
    FILE *fd;
    string_t *path;

} file_handle_t;

#endif // __DEFINITIONS_
