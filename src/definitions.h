#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include <limits.h>
#include <stdio.h>
#include <ncurses.h>
#include <assert.h>
#include "str.h"

#define BUFFER_LINES_MAX 4096
#define FILE_NAME_LEN_MAX 4096

extern WINDOW *top_win;
extern WINDOW *text_win;
extern WINDOW *bottom_win;

extern char file_name_arg[];
extern bool running; 

typedef struct text_pointer_s
{
    size_t row;
    size_t col;
} text_pointer_t;

typedef enum editor_colored_entities_e
{
    TITLE_BAR = 1,
    EDITOR_COLORED_ENTITIES_MAX,
} editor_colored_entities_t;
_Static_assert(EDITOR_COLORED_ENTITIES_MAX < 255, "ncurses colors max exceed");

typedef enum editor_actions_e
{
    E_KEY_UP,
    E_KEY_DOWN,
    E_KEY_LEFT,
    E_KEY_RIGHT,
    E_PASTE,
    E_COPY,
    E_NEXT_WORD,
    E_PREV_WORD,
    E_SAVE,
    E_MAX
} editor_actions_t;

typedef struct buffer_s
{
    text_pointer_t text_pointer;
    string_t *lines[BUFFER_LINES_MAX];
} buffer_t;


typedef struct file_handle_s
{
    FILE* fd;
    string_t *path;

} file_handle_t;

#endif // __DEFINITIONS__
