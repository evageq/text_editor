#include "editor.h"
#include "window.h"

editor_t
editor_init()
{
    editor_t editor;

    window_editor_init(editor.windows);
    editor.buffer.text_pointer = (text_pointer_t){0,0};

    editor.valid = TRUE;

    return editor;
}

static int
gen_window_newx(editor_t editor, text_pointer_t new, text_pointer_t old)
{
    return new.x;
}

static int
gen_window_newy(editor_t editor, text_pointer_t new, text_pointer_t old)
{
    return new.y;
}

void
editor_move_text_cursor(editor_t *const p_editor, int move_action)
{
    int window_newx, window_newy;
    text_pointer_t old_pointer = p_editor->buffer.text_pointer;
    text_pointer_t new_pointer = old_pointer;

    switch (move_action)
    {
        case MOVE_CURSOR_UP:
            {
                --new_pointer.y;
                break;
            }
        case MOVE_CURSOR_DOWN:
            {
                ++new_pointer.y;
                break;
            }
        case MOVE_CURSOR_LEFT:
            {
                --new_pointer.x;
                break;
            }
        case MOVE_CURSOR_RIGHT:
            {
                ++new_pointer.x;
                break;
            }
        default:
            {

            }
    }
    window_newx = gen_window_newx(*p_editor, new_pointer, old_pointer);
    window_newy = gen_window_newy(*p_editor, new_pointer, old_pointer);

    window_set_cursor(&p_editor->windows[WINDOW_TEXT_AREA], window_newx, window_newy);
    p_editor->buffer.text_pointer = new_pointer;
}
