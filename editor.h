#ifndef __EDITOR__
#define __EDITOR__

#include "common.h"
#include "window.h"
#include <limits.h>
#include <stdio.h>
#include <ncurses.h>
#include <assert.h>

typedef vec2 text_pointer_t;

typedef enum move_cursor_action_e
{
    MOVE_CURSOR_UP,
    MOVE_CURSOR_DOWN,
    MOVE_CURSOR_LEFT,
    MOVE_CURSOR_RIGHT,
    MOVE_CURSOR_MAX
} move_cursor_action_t;

typedef struct buffer_s
{
    text_pointer_t text_pointer;

} buffer_t;


typedef struct file_handle_s
{
    FILE* fd;
    valid_t valid;

} file_handle_t;

typedef struct editor_s
{
    file_handle_t file_handle;

    buffer_t buffer;

#define EDITOR_WINDOWS_MAX 8
    _Static_assert(EDITOR_WINDOWS_MAX > WINDOW_MAX, "EDITOR must have enough space to store windows");
    window_t windows[EDITOR_WINDOWS_MAX];

    bool valid;

} editor_t;

// void save_file();

editor_t editor_init();
void editor_move_text_cursor(editor_t *const editor, int move_action);

#endif // __EDITOR__
