//in the name of allah
//include lybrary
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book_food_library.h"
#include "bst_needed.h"
//define files
#define users_directory "users.txt"
#define hall_directory "hall.txt"
#define food_directory "food.txt"
#define coupon_directory "coupon.txt"
#define food_report_directory "food_report.txt"
#define hall_report_directory "hall_report.txt"
//define special characters
#define student_parameter_seperator ' '
#define student_seperator '\n'
//define constants number
#define zero 0
#define one 1
#define two 2
#define max_word_character_initiali 50
#define max_num_of_command_char 20
// define boolian
#define TRUE 1
#define FALSE 0
//login status
#define nobody 0
#define USERS 1
#define ADMIN 2
int main() {
	//open files you need during program
	FILE *users = fopen(users_directory, "a+");
	FILE *fhall = fopen(hall_directory, "a+");
	FILE *ffood = fopen(food_directory, "a+");
	FILE *fcoupon = fopen(coupon_directory, "a+");
	FILE *ffood_report = fopen(food_report_directory, "a+");
	FILE *fhall_report = fopen(hall_report_directory, "a+");
	//create &open files
	/*FILE *users = fopen(users_directory, "w+");
	FILE *fhall = fopen(hall_directory, "w+");
	FILE *ffood = fopen(food_directory, "w+");
	FILE *fcoupon = fopen(coupon_directory, "w+");
	FILE *ffood_report = fopen(food_report_directory, "w+");
	FILE *fhall_report = fopen(hall_report_directory, "w+");*/
	//define and ibitialize your variable
	int student_number = number_of_things(users), hall_number = number_of_things(fhall), food_number = number_of_things(ffood), coupon_number = number_of_things(fcoupon);
	char *login = "login", *signup = "signup", *logout = "logout", *add_hall = "add_hall", *add_food = "add_food", *_exit = "exit", *add_coupon = "add_coupon", *food_report = "food_report", *hall_report = "hall_report", *get_money = "get_money", *show_money = "show_money",*menu = "menu",*reserve = "reserve",*reserve_discount = "reserve_discount",*show_reserved = "show_reserved"; // commands
	int user_login_status = nobody; // user login status : 0 for nobody  1 for student and 2 for admin
	char * command = (char *)malloc(max_num_of_command_char*sizeof(char));
	student *current_student;
	//initialize  structurs arrays that you need
	student *students = create_student_binary_tree();
	hall *halls = create_hall_binary_tree();
	food *foods = create_food_binary_tree();
	coupon *coupons = create_coupon_binary_tree();
	//initialize structures array
	input_students(&students, student_number, users);
	input_halls(&halls,hall_number,fhall);
	input_food(&foods,food_number,ffood,halls);
	input_coupon(&coupons, coupon_number, fcoupon);
	while (TRUE) {
		command = read_unlimited_from_screen(); //input command
		if (!strcmp(command, login) && user_login_status == nobody) { // login command
			user_login_status = login_status(students, student_number,&current_student);
		}
		else if (!strcmp(command, login) && user_login_status != nobody) { //login command when user is already loged in
			printf("not possible\n");
			char * tmp = (char*)malloc(sizeof(char));
			tmp = read_string_from_screen();
			free(tmp);
		}
		else if (!strcmp(command, signup) && user_login_status == nobody) { // signup command
			signup_func(&students, &student_number, users);
		}
		else if (!strcmp(command, signup)) {// signup command when user id already loged in
			printf("not possible\n");
		}
		else if (!strcmp(command, logout) && user_login_status) { // logout
			user_login_status = nobody;
			printf("done\n");
		}
		else if (!strcmp(command, logout) && !user_login_status) { //logout when user isn`t login
			printf("access denied\n");
		}
		else if (!strcmp(command, add_hall) && user_login_status == ADMIN) { // addhall for admin
			add_hall_func(&halls, &hall_number, fhall);
		}
		else if (!strcmp(command, add_hall) && user_login_status == USERS) { // add hall for students
			printf("permission denied\n");
			clear_screen_for_invalid_command();
		}
		else if (!strcmp(command, add_food) && user_login_status == ADMIN) { //add food for admins
			add_food_func(&foods, halls, &food_number, hall_number, ffood);
		}
		else if (!strcmp(command, add_food) && user_login_status == USERS) { // add food for students
			printf("permission denied\n");
			clear_screen_for_invalid_command();
		}
		else if (!strcmp(command, add_coupon) && user_login_status == ADMIN) { // add coupon for admin
			add_coupons_func(&coupons, &coupon_number, fcoupon);
		}
		else if (!strcmp(command, add_coupon) && user_login_status == USERS) { //add coupon for student
			printf("permission denied\n");
			clear_screen_for_invalid_command();
		}
		else if (!strcmp(command, food_report) && user_login_status == ADMIN) { // write food report on file
			if (foods == NULL) fprintf(ffood_report, "there's no food availabl\n");
			food_report_func(foods, ffood_report);
			printf("done\n");
		}
		else if (!strcmp(command, food_report) && user_login_status == USERS) { //food report for students
			printf("permission denied\n");
		}
		else if (!strcmp(command, hall_report) && user_login_status == ADMIN) { // write hall report on file
			if (halls == NULL) fprintf(fhall_report, "there's no hall availabl\n");
			hall_report_func(halls, fhall_report);
			printf("done\n");
		}
		else if (!strcmp(command, hall_report) && user_login_status == USERS) { // hall report for students
			printf("permission denied\n");
		}
		else if (!strcmp(command, get_money) && user_login_status == USERS) { // add money to student wallet
			get_money_func(current_student);
		}
		else if (!strcmp(command, get_money) && user_login_status == USERS) { // get money for admin
			printf("Access denied\n");
		}
		else if (!strcmp(command, show_money) && user_login_status == USERS) { // show wallet of student
			show_money_func(current_student);
		}
		else if (!strcmp(command, show_money) && user_login_status != USERS) { // show money for other
			printf("access denied\n");
		}
		else if (!strcmp(command, menu) && user_login_status == USERS) { //show menu for user
			show_menu_func(foods);
		}
		else if (!strcmp(command, menu) && user_login_status != USERS) { // show menu for other
			printf("access denied\n");
		}
		else if (!strcmp(command, reserve) && user_login_status == USERS) { // reserve food for student
			reserve_func(current_student, foods, halls);
		}
		else if (!strcmp(command, reserve) && user_login_status != USERS) { // reserve food for other
			printf("access denied\n");
		}
		else if (!strcmp(command, reserve_discount) && user_login_status == USERS) { // reserve food with discount for syudents
			reserve_discount_func(current_student, foods, halls, coupons);
		}
		else if (!strcmp(command, reserve_discount) && user_login_status != USERS) { // reserve food with discount for other
			printf("access denied\n");
		}
		else if (!strcmp(command, show_reserved) && user_login_status == USERS) { // show student`s reserved food
			show_reserved_func(current_student);
		}
		else if (!strcmp(command, show_reserved) && user_login_status != USERS) { // show reserved for other
			printf("premission denied\n");
		}
		else if (!strcmp(command, _exit)) { // for exit program, we must free allocated memories
			//free allocated memory
			free_food_tree(foods);
			free_coupon_tree(coupons);
			free_hall_tree(halls);
			free_std_tree(students);
			printf("goodby!\n");
			//close opened file
			fclose(users);
			fclose(fhall);
			fclose(ffood);
			fclose(fcoupon);
			fclose(ffood_report);
			fclose(fhall_report);
			break;
		}
		else if (user_login_status == nobody) {
			printf("access denied\n");
			char * tmp = (char*)malloc(sizeof(char));
			tmp = read_string_from_screen();
			free(tmp);
		}
		else {
			printf("command dosnt exist\n");
			clear_screen_for_invalid_command();
		}
	}
}
