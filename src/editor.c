#include "debug.h"
#include "definitions.h"
#include "prototypes.h"
#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WINDOW *g_top_win;
WINDOW *g_text_win;
WINDOW *g_bottom_win;

int g_top_win_lines;
int g_text_win_lines;
int g_bottom_win_lines;

str_line_t *g_line_head;

cursor_t g_curs;

bool is_modified = false;

char g_line_buffer[LINE_BUFFER_LEN];
char g_file_name_arg[FILE_NAME_LEN];

bool g_running = FALSE;

char *
read_line(FILE *f)
{

    char *p = fgets(g_line_buffer, sizeof(g_line_buffer), f);

    if (p != NULL)
    {
        p[strlen(p) - 1] = '\0';
    }

    return p;
}

str_line_t *
get_first_print_line()
{
    str_line_t *tmp = g_curs.line;
    for (int i = g_curs.y; i > 0; --i)
    {
        assert(tmp->prev != NULL);
        tmp = tmp->prev;
    }

    return tmp;
}

str_line_t *
get_last_print_line()
{
    str_line_t *start_line = get_first_print_line();
    str_line_t *tmp_line = start_line;

    for (size_t i = 0; i < g_text_win_lines; ++i)
    {
        if (tmp_line->next != NULL)
        {
            tmp_line = tmp_line->next;
        }
        else
        {
            break;
        }
    }

    assert(tmp_line != NULL);
    return tmp_line;
}

size_t
get_in_file_len()
{
    size_t i = 0;
    for (str_line_t *tmp_line = g_line_head; tmp_line != NULL;
         tmp_line = tmp_line->next)
    {
        ++i;
    }
    return i;
}

size_t
get_out_file_len()
{
    size_t i = 0;
    FILE *f = fopen(g_file_name_arg, "r");

    while (read_line(f))
    {
        ++i;
    }

    fclose(f);

    return i;
}

bool
cmp_changes()
{

    if (get_in_file_len() != get_out_file_len())
    {
        return true;
    }

    FILE *f = fopen(g_file_name_arg, "r");
    str_line_t *tmp_in_line = g_line_head;
    char *tmp_out_line = read_line(f);
    size_t len = get_in_file_len();
    for (size_t i = 0; i < len; ++i)
    {
        if (memcmp(tmp_in_line->p_str->buf, tmp_out_line,
                   tmp_in_line->p_str->len)
            != 0)
        {
            fclose(f);
            return true;
        }
        tmp_in_line = tmp_in_line->next;
        tmp_out_line = read_line(f);
    }

    fclose(f);

    return false;
}

void
load_file(const char *file)
{

    FILE *f = fopen(file, "r");

    str_line_t *tmp_line;
    str_line_t *prev_line = NULL;
    while (read_line(f) != NULL)
    {
        tmp_line = malloc(sizeof(*tmp_line));
        *tmp_line = (str_line_t){ .next = NULL,
                                  .prev = prev_line,
                                  .p_str = string_new(g_line_buffer) };
        if (prev_line != NULL)
        {
            prev_line->next = tmp_line;
        }

        prev_line = tmp_line;

        if (g_line_head == NULL)
        {
            g_line_head = tmp_line;
        }
    }

    fclose(f);
}

void
draw_text_curs()
{
    wmove(g_text_win, g_curs.y, g_curs.x);
}

void
draw()
{
    draw_top_win();
    draw_text_win();
    draw_bottom_win();
    draw_text_curs();
    doupdate();
}

void
draw_top_win()
{
    WINDOW *win = g_top_win;
    bool is_modified = cmp_changes();
    const char editor_banner[] = "GNU nano clone";
    werase(win);
    wbkgd(win, COLOR_PAIR(TITLE_BAR));
    wprintw(win, "%s %s", editor_banner,
            (is_modified == true ? "Modified" : ""));
    wnoutrefresh(win);
}

void
draw_text_line(str_line_t *line)
{
    wprintw(g_text_win, "%s\n", line->p_str->buf);
}

void
draw_text_win()
{
    WINDOW *win = g_text_win;
    werase(win);

    str_line_t *line = get_first_print_line();
    str_line_t *end_line = get_last_print_line();
    while (line != NULL && line != end_line)
    {
        draw_text_line(line);
        line = line->next;
    }
    draw_text_line(end_line);

    wnoutrefresh(win);
}

void
draw_bottom_win()
{
    WINDOW *win = g_bottom_win;
    box(win, 0, 0);
    wnoutrefresh(win);
}

void
setup()
{
    debug_init();

    initscr();
    start_color();
    cbreak();
    noecho();
    nl();
    raw();
    if (has_colors() == FALSE)
    {
        debug("Terminal doesn't support colors");
    }
    else
    {
        use_default_colors();
    }
    init_pair(TITLE_BAR, COLOR_BLACK, COLOR_WHITE);
    window_init();
    keypad(g_text_win, TRUE);

    load_file(g_file_name_arg);
    g_curs = (cursor_t){ .x = 0, .y = 0, .line = g_line_head };

    g_running = TRUE;
}

void
window_init()
{

    if (LINES >= 3)
    {
        g_top_win_lines = 1;
        g_bottom_win_lines = 3;
        assert(g_top_win_lines >= 0);
        assert(g_bottom_win_lines >= 0);

        g_text_win_lines = LINES - g_top_win_lines - g_bottom_win_lines;
        assert(g_text_win_lines >= 0);

        g_top_win = newwin(g_top_win_lines, COLS, 0, 0);
        g_text_win = newwin(g_text_win_lines, COLS, 0 + g_top_win_lines, 0);
        g_bottom_win = newwin(g_bottom_win_lines, COLS, g_text_win_lines, 0);
    }
    else
    {
        g_top_win = newwin(0, 0, 0, 0);
        g_text_win = newwin(LINES, COLS, 0, 0);
        g_bottom_win = newwin(0, 0, 0, 0);
    }
}

void
run()
{

    while (g_running == TRUE)
    {
        draw();
        int ch;
        ch = wgetch(g_text_win);
        process_key(ch);
    }
}

void
del_str_line_node(str_line_t *node)
{
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
        if (node->next != NULL)
        {
            node->next->prev = node->prev;
        }
    }
    string_free(node->p_str);
    free(node);
}

void
del_char()
{
    if (g_curs.x > 0)
    {
        string_erase(g_curs.line->p_str, g_curs.x - 1, 1);
        move_curs_left();
    }
    else if (g_curs.line->prev != NULL)
    {
        move_curs_left();
        string_append(g_curs.line->p_str, g_curs.line->next->p_str);
        del_str_line_node(g_curs.line->next);
    }
}

str_line_t *
create_str_line_node(str_line_t *node)
{
    str_line_t *tmp = malloc(sizeof(*tmp));

    tmp->next = node->next;
    tmp->prev = node;

    if (node->next != NULL)
    {
        node->next->prev = tmp;
    }
    node->next = tmp;

    return tmp;
}

void
paste_new_line()
{
    str_line_t *line = create_str_line_node(g_curs.line);
    line->p_str = string_new_len(g_curs.line->p_str->buf + g_curs.x,
                                 g_curs.line->p_str->len - g_curs.x);
    string_erase(line->prev->p_str, g_curs.x, -1);
    move_curs_down();
    move_curs_to_left();
}

void
paste_print_char(char ch)
{
    string_insert(g_curs.line->p_str, ch, g_curs.x);
    move_curs_right();
}

void
save_changes()
{
    str_line_t *tmp = g_line_head;
    FILE *f = fopen(g_file_name_arg, "w");

    while (tmp != NULL)
    {
        fputs(tmp->p_str->buf, f);
        fputs("\n", f);
        tmp = tmp->next;
    }

    fclose(f);
}

void
move_curs_up()
{
    if (g_curs.line->prev != NULL)
    {
        g_curs.line = g_curs.line->prev;
        DECR(g_curs.y);
        g_curs.x = MIN(g_curs.x, g_curs.line->p_str->len);
    }
    else
    {
        assert(g_curs.y == 0);
    }
}

void
move_curs_down()
{
    if (g_curs.line->next != NULL)
    {
        g_curs.line = g_curs.line->next;
        INCR(g_curs.y, g_text_win_lines);
        g_curs.x = MIN(g_curs.x, g_curs.line->p_str->len);
    }
}

void
move_curs_left()
{
    if (g_curs.x <= 0)
    {
        if (g_curs.line->prev != NULL)
        {
            g_curs.line = g_curs.line->prev;
            g_curs.x = g_curs.line->p_str->len;
            DECR(g_curs.y);
        }
    }
    else
    {
        DECR(g_curs.x);
    }
}

void
move_curs_right()
{
    if (g_curs.x + 1 > g_curs.line->p_str->len)
    {
        if (g_curs.line->next != NULL)
        {
            g_curs.line = g_curs.line->next;
            g_curs.x = 0;
            INCR(g_curs.y, g_text_win_lines);
        }
    }
    else
    {
        INCR(g_curs.x, g_curs.line->p_str->len + 1);
    }
}

void
move_curs_n_up(int n)
{
    for (int i = 0; i < n; ++i)
    {
        move_curs_up();
    }
}

void
move_curs_n_down(int n)
{
    for (int i = 0; i < n; ++i)
    {
        move_curs_down();
    }
}

void
move_curs_to_left()
{
    while (g_curs.x > 0)
    {
        move_curs_left();
    }
}

void
move_cursor_to_right()
{
    while (g_curs.y != g_curs.line->p_str->len)
    {
        move_curs_right();
    }
}

void
move_curs_n_left(int n)
{
    for (int i = 0; i < n; ++i)
    {
        move_curs_left();
    }
}

void
move_curs_n_right(int n)
{
    for (int i = 0; i < n; ++i)
    {
        move_curs_right();
    }
}

void
die()
{
    cleanup();
    exit(0);
}

void
process_key(int key)
{
    switch (key)
    {
        case E_KEY_UP:
        {
            move_curs_up();
            break;
        }
        case E_KEY_DOWN:
        {
            move_curs_down();
            break;
        }
        case E_KEY_LEFT:
        {
            move_curs_left();
            break;
        }
        case E_KEY_RIGHT:
        {
            move_curs_right();
            break;
        }
        case E_DEL_CH:
        {
            del_char();
            break;
        }
        case E_PASTE:
        {
            break;
        }
        case E_COPY:
        {
            break;
        }
        case E_NEXT_WORD:
        {
            break;
        }
        case E_PREV_WORD:
        {
            break;
        }
        case E_SAVE:
        {
            save_changes();
            break;
        }
        case E_EXIT:
        {
            die();
            break;
        }
        default: // this branch mean we're probably typing
        {
            debug("key %d", key);
            if (isprint(key))
            {
                paste_print_char(key);
            }
            if (key == '\n')
            {
                paste_new_line();
            }
        }
    }
}

void
cleanup()
{
    debug_close();
    endwin();
}

int
main(int argc, char *argv[])
{

    const char help_str[] = "Usage:\
                         ./a.out <file_name>";

    if (argc < 1)
    {
        error("Failed to run\n%s", help_str);
        exit(-1);
    }
    else
    {
        strncpy(g_file_name_arg, argv[1], sizeof(g_file_name_arg));
    }

    setup();
    run();
    cleanup();

    return 0;
}
