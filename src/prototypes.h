#include "definitions.h"

void setup();
void run();
void cleanup();

void draw();
void draw_top_win();
void draw_text_win();
void draw_bottom_win();

void load_file(const char *file);
void process_key(int ch);

void move_curs_up();
void move_curs_down();
void move_curs_left();
void move_curs_right();

void move_curs_n_up(int n);
void move_curs_n_down(int n);
void move_curs_n_left(int n);
void move_curs_n_right(int n);
void move_curs_to_left();

void window_init();
