#ifndef MENU_H_
#define MENU_H_

typedef struct{
    char *name;
    void (*function)(void);
} menu_item_t;

typedef struct{
    int id;
    int current_item;
	menu_item_t *items[];
} menu_t;

void menu_init();
void menu_print(int id);

#endif