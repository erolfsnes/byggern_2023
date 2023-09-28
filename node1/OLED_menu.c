#include "OLED_menu.h"
#include "OLED_driver.h"

menu_t menus[2];

void menu_init() {
    menus[0].id = 0;
    menus[0].current_item = 0;
    menus[0].item_count = 1;
    menus[0].items[0].name = "Start game";
    // menus[0].items[0].function = NULL;

    menus[1].id = 1;
    menus[1].current_item = 0;
    menus[1].item_count = 4;
    menus[1].items[0].name = "Back";
    // menus[1].items[0].function = NULL;
    menus[1].items[1].name = "Easy";
    // menus[1].items[1].function = NULL;
    menus[1].items[2].name = "Medium";
    // menus[1].items[2].function = NULL;
    menus[1].items[3].name = "Hard";
    // menus[1].items[3].function = NULL;
}


void menu_print(int id) {
    menu_t menu = menus[id];
    OLED_clear();
    for (int i = 0; i < menu.item_count; i++) {
        OLED_pos(i, 0);
        if (i == menu.current_item) {
            OLED_print(">");
        } else {
            OLED_print(" ");
        }
        OLED_print(menu.items[i].name);
    }
}