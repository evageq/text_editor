#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include "str.h"
#include <assert.h>
#include <limits.h>
#include <ncurses.h>
#include <stdio.h>

#define LINE_BUFFER_LEN 4096
#define FILE_NAME_LEN 4096

#define MIN(a, b) (a < b ? a : b)

#define DECR_N(x, n) \
    do               \
    {                \
        if (x >= n)  \
        {            \
            x -= n;  \
        }            \
    } while (0)
#define DECR(x) DECR_N(x, 1)

#define INCR_N(x, n, upper_bound) \
    do                            \
    {                             \
        if (x + n < upper_bound)  \
        {                         \
            x += n;               \
        }                         \
    } while (0)
#define INCR(x, upper_bound) INCR_N(x, 1, upper_bound)

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

#define CTRL(c) ((c) & 037)

typedef enum editor_actions_e
{
    E_KEY_UP = KEY_UP,
    E_KEY_DOWN = KEY_DOWN,
    E_KEY_LEFT = KEY_LEFT,
    E_KEY_RIGHT = KEY_RIGHT,
    E_VIM_KEY_UP = CTRL('k'),
    E_VIM_KEY_DOWN = CTRL('j'),
    E_VIM_KEY_LEFT = CTRL('h'),
    E_VIM_KEY_RIGHT = CTRL('l'),
    E_DEL_CH = KEY_BACKSPACE,
    E_PASTE,
    E_COPY,
    E_NEXT_WORD,
    E_PREV_WORD,
    E_SAVE = CTRL('s'),
    E_EXIT = CTRL('x'),
    E_MAX
} editor_actions_t;

typedef enum editor_save_choice_e
{
    SAVE_Y_CHOICE,
    SAVE_N_CHOICE,
    SAVE_CANCEL_CHOICE,
} editor_save_choise_t;

typedef struct file_handle_s
{
    FILE *fd;
    string_t *path;

} file_handle_t;

#endif // __DEFINITIONS_
