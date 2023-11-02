#include "menu.h"
#include "OLED_driver.h"
#include "ADC_header.h"
#include <stdio.h>
#include <util/delay.h>
#include "can.h"

extern uint32_t uw_tick;
extern uint16_t score;
extern int can_score_recieved;

menu_t menus[2];
uint8_t id = 0;
void call_menu_print_0();
void call_menu_print_1();
void call_start_game_easy();
void call_start_game_medium();
void call_start_game_hard();

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
    menus[1].items[1].function = call_start_game_easy;
    menus[1].items[2].name = "Medium";
    menus[1].items[2].function = call_start_game_medium;
    menus[1].items[3].name = "Hard";
    menus[1].items[3].function = call_start_game_hard;
}

void call_menu_print_0()
{
    if (id != 0) {
        menus[0].current_item = 0;
    }
    id = 0;
    menu_print(0);
    main_menu();
}

void call_menu_print_1()
{
    if (id != 1) {
        menus[1].current_item = 0;
    }
    id = 1;
    menu_print(1);
    main_menu();
}

void print_game_over()
{
    OLED_reset();
    OLED_pos(0,0);
    OLED_print("GAME OVER!");
    OLED_pos(1,0);
    OLED_print("(Press button)");
}

void print_score(uint16_t score, uint8_t lives) 
{
    OLED_reset();
    OLED_pos(0,0);
    char buf[12];
    sprintf(buf, "Score: %d", score);
    OLED_print(buf);

    OLED_pos(1,0);
    sprintf(buf, "Lives: %d", lives);
    OLED_print(buf);
}


void start_game(uint8_t lives) {
    can_msg_t msg;
    msg.id = 11;
    msg.len = 1;
    msg.data[0] = ++lives;
    can_score_recieved = 0;  
    can_transmit(msg);
    uint32_t send_time = uw_tick;
    while (1) {
        if (can_score_recieved) {
            score = 0;
            printf("CAN score recieved");
            break;
        }
        if (uw_tick - send_time > 100) {
            printf("AAAAAA\r\n");
            can_transmit(msg);
            send_time = uw_tick;
        }
        printf("waiting...\r\n");
        
    }

}
void call_start_game_easy() {
    start_game(3);
    
}

void call_start_game_medium() {
    start_game(2);
    
}

void call_start_game_hard() {
    start_game(1);
    
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

    adc_data data = {0};
    pos_t pos_data;
    pos_calibrate(&data.x_offs, &data.y_offs);

    while(1) {
        poll_joystick(&data, &pos_data);
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
            break;
        }
    }
}

