#include "menu.h"
#include "OLED_driver.h"
#include "ADC_header.h"
#include <stdio.h>
#include <util/delay.h>


menu_t menus[2];
uint8_t id = 0;
void call_menu_print_0();
void call_menu_print_1();
void poll_joystick(adc_data *data, pos_t *pos_data);


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
    if (id != 0) {
        menus[0].current_item = 0;
    }
    id = 0;
    menu_print(0);
}

void call_menu_print_1()
{
    if (id != 1) {
        menus[1].current_item = 0;
    }
    id = 1;
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

void poll_joystick(adc_data *data, pos_t *pos_data) {
    _delay_ms(100);
    ADC_Read(data);
    *pos_data = pos_read(data);
    joystick_dir dir = dir_read(pos_data);
	// printf("%d\t %d\t %d\t %d\t %d\t\r\n", data->ch1, data->ch2, pos_data->x, pos_data->y, dir);
}

void main_menu() {
    menu_print(id);

    volatile adc_data data = {0};
    pos_t pos_data;
    pos_calibrate(&data.x_offs, &data.y_offs);

    while(1) {
        poll_joystick(&data, &pos_data);
        printf("%d\n\r", dir_read(&pos_data));
        if (dir_read(&pos_data) == DOWN) {
            while (dir_read(&pos_data) == DOWN) {
                poll_joystick(&data, &pos_data);
            }
            menus[id].current_item = (menus[id].current_item + 1);

            if (menus[id].current_item > menus[id].item_count - 1) {
                menus[id].current_item = 0;
            }
            menu_print(id);
        } 
        else if (dir_read(&pos_data) == UP) {
            while (dir_read(&pos_data) == UP) {
                poll_joystick(&data, &pos_data);
            }

            menus[id].current_item = (menus[id].current_item - 1);

            printf("asdf");
            if (menus[id].current_item < 0) {
                menus[id].current_item = menus[id].item_count - 1;
            }
            menu_print(id);
        }
        else if (joy_button_read()) {
            while (joy_button_read()) {
                poll_joystick(&data, &pos_data);
            }
            menus[id].items[menus[id].current_item].function();
        }
    }
}
