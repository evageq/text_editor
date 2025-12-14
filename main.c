#include <stdio.h>
#include <ncurses.h>
#include "common.h"
#include "editor.h"
#include "window.h"
#include "render.h"

int main(int argc, char *argv[]) {

    const char help_str[] = "Usage:\
                         ./a.out <file_name>";

    debug_init();
    debug("Start text_editor");

    FILE *file;
    if (argc > 1)
    {
        file = fopen((const char*)argv[1], "a+");
    }
    else
    {
        debug("Failed to run\n%s", help_str);
        return -1;
    }


    ncurse
    {
        // cbreak();
        noecho();
        assert(has_colors() == TRUE);
        use_default_colors();
        // keypad();
        // nodelay() for text area

        editor_t editor = editor_init();
        if (editor.valid == FALSE)
        {
            debug("Failed to init text_editor");
        }

        render_t renderer = render_init(&editor);
        if (renderer.valid == FALSE)
        {
            debug("Failed to init renderer");
        }

        render_editor(renderer);

        while (1)
        {

            int ch;
            ch = wgetch(editor.windows[WINDOW_TEXT_AREA].win);
            int move_action;
            switch (ch)
            {
                case KEY_UP:
                    {
                        move_action = MOVE_CURSOR_UP;
                        break;
                    }
                case KEY_DOWN:
                    {
                        move_action = MOVE_CURSOR_DOWN;
                        break;
                    }
                case KEY_LEFT:
                    {
                        move_action = MOVE_CURSOR_LEFT;
                        break;
                    }
                case KEY_RIGHT:
                    {
                        move_action = MOVE_CURSOR_RIGHT;
                        break;
                    }
                default:
                    {
                        debug("Get unknown ch %d at window TEXT_AREA", ch);
                    }
            }
            
            editor_move_text_cursor(&editor, move_action);
            render_editor(renderer);
        }
        // wgetch(editor.windows[WINDOW_TEXT_AREA].win);
    }

    debug_close();

    return 0;
}
