#include "OLED_menu.h"
#include "OLED_driver.h"

menu_t menus[2] = {0};

menu_t main_menu = {0};
menu_item_t main_menu_items[1] = {0};

menu_t start_menu = {0};
menu_item_t start_menu_items[4] = {0};

// menu_t highscore_menu = {0};
// menu_item_t highscore_menu_items[] = {0};

void menu_init() {
    main_menu.id = 0;
    main_menu.current_item = 0;
    main_menu.items = main_menu_items;

    main_menu_items[0].name = "Start game";
    // main_menu_items[1].id = 1;
    // main_menu_items[1].name = "Highscores";
    // main_menu_items[2].id = 2;
    // main_menu_items[2].name = "Settings";


    start_menu.id = 1;
    start_menu.current_item = 0;
    start_menu.items = start_menu_items;

    start_menu_items[0].name = "Back";
    start_menu_items[1].function = &game_start;
    start_menu_items[1].name = "Easy";
    start_menu_items[2].name = "Medium";
    start_menu_items[3].name = "Hard";   


    menus[0] = main_menu;
    menus[1] = start_menu;
}

void menu_print(int id)
{
    menu_t menu = menus[id];
    OLED_clear();
    for (int i = 0; i < sizeof(menu.items); i++)
    {
        OLED_pos(i, 0);
        if (i == menu.current_item)
        {
            OLED_print(">");
        }
        else
        {
            OLED_print(" ");
        }
        OLED_print(menu.items[i].name);
    }
}