#ifndef MENU_H_
#define MENU_H_

#define MAX_ITEMS 4

typedef struct{
    char *name;
    void (*function)(void);
} menu_item_t;

typedef struct{
    int id;
    int current_item;
    int item_count;
	menu_item_t items[MAX_ITEMS];
} menu_t;

void menu_init();
void menu_print(int id);
void main_menu();

#endif