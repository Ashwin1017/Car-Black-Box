/* 
 * File:   main.h
 * Author: ashwi
 *
 * Created on 25 September, 2024, 8:00 AM
 */

#ifndef MAIN
#define MAIN

#define DASHBOARD               0
#define PASSWORD                1
#define MENU                    2
#define MENU_ENTER              3

#define VIEWLOG                 0
#define DOWNLOADLOG             1
#define CLEARLOG                2
#define SETTIME                 3
#define CHANGEPASS              4

void dashboard();               
void store_event();
void password(unsigned char key);
void menu(char key);
void view_log(char key);
void download_log();
void clear_log(char key);
void set_time(char key);
void change_password(char key);

void init_config();
void init_config1();
int my_strcmp(char *str1, char *str2);
void get_time(void);

#endif