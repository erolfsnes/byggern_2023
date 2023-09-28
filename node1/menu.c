#include "menu.h"
#include "OLED_driver.h"
#include "ADC_header.h"
#include <stdio.h>
#include <util/delay.h>

menu_t menus[2];
void call_menu_print_0();
void call_menu_print_1();
void poll_joystick(adc_data data);


void menu_init() {
    menus[0].id = 0;
    menus[0].current_item = 0;
    menus[0].item_count = 1;
    menus[0].items[0].name = "Start game";
    menus[0].items[0].function = call_menu_print_1;

    menus[1].id = 1;
    menus[1].current_item = 0;
    menus[1].item_count = 4;
    menus[1].items[0].name = "Back";
    menus[1].items[0].function = call_menu_print_0;
    menus[1].items[1].name = "Easy";
    // menus[1].items[1].function = NULL;
    menus[1].items[2].name = "Medium";
    // menus[1].items[2].function = NULL;
    menus[1].items[3].name = "Hard";
    // menus[1].items[3].function = NULL;
}

void call_menu_print_0()
{
    menus[0].current_item = 0;
    menu_print(0);
}

void call_menu_print_1()
{
    menus[1].current_item = 0;
    menu_print(1);
}


void menu_print(int id) {
    menu_t menu = menus[id];
    OLED_reset();
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

void poll_joystick(adc_data data) {
    _delay_ms(50);
    ADC_Read(&data);
    pos_data = pos_read(&data);
}

void main_menu() {
    menu_print(0);

    adc_data data;
    ADC_Read(&data);
    pos_t pos_data = pos_read(&data);

    while(1) {
        poll_joystick();
        if (joy_button_read()) {
            while (joy_button_read()) {
                poll_joystick();
            }
            menu.items[menu.current_item].function();
            break;
        } else if (dir_read(&pos_data) == DOWN) {
            while (dir_read(&pos_data) == DOWN) {
                poll_joystick();
            }
            
            menus[id].current_item = (menu.current_item + 1) % menu.item_count;
            menu_print(id);
            break;
        } else if (dir_read(&pos_data) == UP) {
            while (dir_read(&pos_data) == UP) {
                poll_joystick();
            }

            menus[id].current_item = (menu.current_item - 1) % menu.item_count;
            menu_print(id);
            break;
        }
    }
}