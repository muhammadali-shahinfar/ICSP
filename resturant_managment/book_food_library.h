#pragma once
#pragma once
//include lybrary
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst_needed.h"
//define files
#define users_directory "users.txt"
#define hall_directory "hall.txt"
#define food_directory "food.txt"
#define coupon_directory "coupon.txt"
#define food_report_directory "food_report.txt"
//define special characters
#define student_parameter_seperator ' '
#define student_seperator '\n'
//define constants number
#define zero 0
#define initialy_profit_number 0
#define initialy_reserved_number 0
#define initial_wallet_value 0
#define one 1
#define two 2
#define number_of_day 7
// define boolian
#define TRUE 1
#define FALSE 0
//login status
#define nobody 0
#define USERS 1
#define ADMIN 2
typedef struct admin // struct for admin(we havnt nessecity for define this)
{
	char *username;
	char *password;
}admin;
//functions for read in diffrent situation
void read_one_element_from_screen() { //call for delete space between words
	char c;
	scanf("%c", &c);
}
char * read_unlimited_from_file(FILE *file) { 
	char c;
	int i = 1;
	char *name = (char *)malloc(sizeof(char));
	fscanf(file, "%c", &c);
	while (c != '\n'&&c != ' '&&!feof(file)) {
		i++;
		name = (char *)realloc(name, i*sizeof(char*));
		name[i - 2] = c;
		name[i - 1] = NULL;
		fscanf(file, "%c", &c);
	}
	return name;
}
char * read_unlimited_from_screen() {
	char * name = (char *)malloc(sizeof(char));
	char c;
	int i = 1;
	scanf("%c", &c);
	while (c != '\n'&&c != ' ') {
		i++;
		name = (char *)realloc(name, i*sizeof(char*));
		name[i - 2] = c;
		name[i - 1] = NULL;
		scanf("%c", &c);
	}
	return name;
}
char * read_string_from_screen() { 
	char * name = (char *)malloc(sizeof(char));
	char c;
	int i = 1;
	scanf("%c", &c);
	while (c != '\n') {
		i++;
		name = (char *)realloc(name, i*sizeof(char*));
		name[i - 2] = c;
		name[i - 1] = NULL;
		scanf("%c", &c);
	}
	return name;
}

void clear_screen_for_invalid_command() { // clear screen when invalid user write a command
	char * tmp = (char*)malloc(sizeof(char));
	tmp = read_string_from_screen();
	free(tmp);
}
//functions for input structes from files
void input_students(student **students, int student_number, FILE *users) { //input students(users)from file into an array with data type student
	fseek(users, zero, SEEK_SET);
	for (int i = zero; i<student_number; i++) {
		student * new_std = (student *)malloc(sizeof(student));
		new_std->username = (char *)malloc(sizeof(char));
		new_std->password = (char*)malloc(sizeof(char));
		new_std->left_child = NULL;
		new_std->right_child = NULL;
		new_std->wallet = initial_wallet_value;
		new_std->reserved_food = (reserve_food*)malloc(sizeof(reserve_food));
		new_std->reserved_food->next = NULL;
		if (new_std->username == NULL || new_std->password == NULL) {
			printf("cant alloc students[%i].usernameor passwords in input_students function", i);
			exit(zero); // porside shavad che konim
		}
		new_std->username = read_unlimited_from_file(users);
		fscanf(users, "%d", &(new_std->student_id));
		fseek(users, 1, SEEK_CUR);
		new_std->password = read_unlimited_from_file(users);
		add_to_tree(students, new_std);
	}
	fseek(users, zero, SEEK_SET);
}
void input_halls(hall **halls, int hall_number, FILE *fhall) { //input halls from file into an array with data type hall
	fseek(fhall, zero, SEEK_SET);
	for (int i = zero; i<hall_number; i++) {
		hall *new_hall = (hall*)malloc(sizeof(hall));
		new_hall->name = (char *)malloc(sizeof(char));
		new_hall->left_child = NULL;
		new_hall->right_child = NULL;
		new_hall->profit = (int *)calloc(number_of_day, sizeof(int));
		new_hall->reserved = (int *)calloc(number_of_day, sizeof(int));
		for (int i = zero; i < number_of_day; i++) {
			new_hall->profit[i] = initialy_profit_number;
			new_hall->reserved[i] = initialy_reserved_number;
		}
		if (new_hall->name == NULL) {
			printf("cant alloc halls[i].name or hall_id in input_halls function", i);
			exit(zero); // porside shavad che konim
		}
		new_hall->name = read_unlimited_from_file(fhall);
		fscanf(fhall, "%d %d", &(new_hall->hall_id), &(new_hall->capacity));
		fseek(fhall, 2, SEEK_CUR);
		add_to_hall_tree(halls, new_hall);
	}
}
void input_food(food **foods, int food_number, FILE*ffood, hall *halls) { //input foods from file into an array with data type food
	fseek(ffood, zero, SEEK_SET);
	for (int i = zero; i<food_number; i++) {
		food * new_food = (food*)malloc(sizeof(food));
		if (new_food == NULL) {
			printf("cant alloc food(%d) in  input_foods function", i);
			exit(zero); // porside shavad che konim
		}
		new_food->name = (char*)malloc(sizeof(char));
		if (new_food->name == NULL) {
			printf("cant alloc food(%d)->name in  input_foods function", i);
			exit(zero); // porside shavad che konim
		}
		new_food->left_child = NULL;
		new_food->right_child = NULL;
		new_food->profit = zero;
		new_food->reserved = zero;
		new_food->name = read_unlimited_from_file(ffood);
		fscanf(ffood, "%d %d %d %d %d", &(new_food->food_id), &(new_food->price), &(new_food->capacity), &(new_food->hall_id), &(new_food->day));
		new_food->hall_name = name_of_hall_id(halls, new_food->hall_id);
		fseek(ffood, 2, SEEK_CUR);//check
		add_to_food_tree(foods, new_food);
	}
}
void input_coupon(coupon **coupons, int coupon_number, FILE *fcoupon) { //input coupon from file into an array with data type coupon
	fseek(fcoupon, zero, SEEK_SET);
	for (int i = zero; i<coupon_number; i++) {
		coupon * new_coupon = (coupon*)malloc(sizeof(coupon));
		int coupon_id, percentage, capacity;
		fscanf(fcoupon, "%d %d %d", &coupon_id, &percentage, &capacity);
		if (new_coupon == NULL) {
			printf("cant alloc coupon in  input_coupon function");
			exit(zero); // porside shavad che konim
		}
		new_coupon->capacity = capacity;
		new_coupon->percentage = percentage;
		new_coupon->coupon_id = coupon_id;
		new_coupon->left_child = NULL;
		new_coupon->right_child = NULL;
		add_to_coupon_tree(coupons, new_coupon);
	}
}
//
int number_of_things(FILE *file) { // counts the number of elements in file(students in user.txt,hall in hall.txt or...)
	fseek(file, zero, SEEK_SET);
	int thing_counter = zero;
	char c = fgetc(file);
	while (!feof(file)) {
		if (c == '\n') { //pay attantion! it works corroctly when the last character of text was '\n',if data's input was with program, it works correctly
			thing_counter++;
		}
		c = fgetc(file);
	}
	fseek(file, zero, SEEK_SET);
	return thing_counter;
}
//
int login_status(student *students, int student_number,student **current_student) { // determine who one is using program,1 for students,2 for admin and 0 for nobody 
	char*username = (char*)malloc(sizeof(char));
	char *password = (char*)malloc(sizeof(char));
	username = read_unlimited_from_screen();
	password = read_unlimited_from_screen();
	admin Admin = { "admin","admin" };
	if (!strcmp(username, Admin.username) && !strcmp(password, Admin.password)) {
		printf("welcome %s\n", Admin.username);
		return ADMIN;
	}
	while (students) {
		if (!strcmp(username, students->username) && !strcmp(password, students->password)) {
			printf("welcome %s\n", students->username);
			*current_student = students;
			return USERS;
		}
		if (!strcmp(username, students->username)) break;
		if (strcmp(username, students->username) > 0) {
			if (students->right_child == NULL) return nobody;
			students = students->right_child;
		}
		else {
			if (students->left_child == NULL)break;
			students = students->left_child;
		}
	}
	printf("wrong username or pass\n");
	return nobody;
}
//functions for add element
void signup_func(student **students, int *student_number, FILE *users) { // add a student to last line of users.txt
	char *username = (char *)malloc(sizeof(char));
	char *password = (char *)malloc(sizeof(char));
	int student_id;
	username = read_unlimited_from_screen();
	scanf("%d", &student_id);
	char*c;
	scanf("%c", &c); // delet space
	password = read_unlimited_from_screen();
	if (find_student(*(students), username, password)) {
		printf("user already existed\n");
		return;
	}
	student *new_std = (student *)malloc(sizeof(student));
	new_std->left_child = NULL;
	new_std->password = password;
	new_std->right_child = NULL;
	new_std->student_id = student_id;
	new_std->wallet = initial_wallet_value;
	new_std->reserved_food = (reserve_food*)malloc(sizeof(reserve_food));
	new_std->reserved_food->next = NULL;
	new_std->username = username;
	add_to_tree(students, new_std);
	fseek(users, zero, SEEK_END);
	fprintf(users, "%s %d %s\n", username, student_id, password);
	*(student_number) = *(student_number)+one;
	printf("done\n");
}
void add_hall_func(hall **halls, int *hall_number, FILE *fhall) { // add a hall to last line of hall.txt
	char *name = (char *)malloc(sizeof(char));
	long long capacity;
	int hall_id;
	name = read_unlimited_from_screen();
	scanf("%i %d", &hall_id, &capacity);
	read_one_element_from_screen();
	if (find_hall(*(halls),hall_id)) {
			printf("hall id is not unique\n");
			return;
		}
	fseek(fhall, zero, SEEK_END);
	fprintf(fhall, "%s %i %d\n", name, hall_id, capacity);
	*(hall_number) = *(hall_number)+one;
	hall *new_hall = (hall*)malloc(sizeof(hall));
	new_hall->capacity = capacity;
	new_hall->hall_id = hall_id;
	new_hall->left_child = NULL;
	new_hall->name = name;
	new_hall->right_child = NULL;
	new_hall->left_child = NULL;
	new_hall->profit = (int *)calloc(number_of_day, sizeof(int));
	new_hall->reserved = (int *)calloc(number_of_day, sizeof(int));	
	for (int i = zero; i < number_of_day; i++) {
		new_hall->profit[i] = initialy_profit_number;
		new_hall->reserved[i] = initialy_reserved_number;
	}
	add_to_hall_tree(halls, new_hall);
	printf("done\n");
}
void add_food_func(food **foods, hall *halls, int *food_number, int hall_number, FILE *ffood) { // add a food to last line of food.txt
	char *name = (char *)malloc(sizeof(char));
	long long capacity, price;
	int day, hall_id, food_id;
	name = read_unlimited_from_screen();
	scanf(" %d %d %d %i %d", &food_id, &price, &capacity, &hall_id, &day);
	read_one_element_from_screen();
	if (find_food(*(foods), food_id)) {
		printf("food id is not unique\n");
		return;
	}
	if (!find_hall(halls, hall_id)) {
		printf("wrong hall id\n");
		return;
	}
	fseek(ffood, zero, SEEK_END);
	food *new_food = (food*)malloc(sizeof(food));
	if (new_food == NULL) {
		printf("cant alloc food in  input_foods function");
		exit(zero); // porside shavad che konim
	}
	new_food->left_child = NULL;
	new_food->right_child = NULL;
	new_food->capacity = capacity;
	new_food->day = day;
	new_food->food_id = food_id;
	new_food->name = name;
	new_food->price = price;
	new_food->hall_id = hall_id;
	new_food->profit = zero;
	new_food->reserved = zero;
	new_food->hall_name = name_of_hall_id(halls, hall_id);
	fprintf(ffood, "%s %i %d %d %i %d\n", new_food->name, new_food->food_id, new_food->price, new_food->capacity, new_food->hall_id, new_food->day);
	*(food_number) = *(food_number)+one;
	add_to_food_tree(foods, new_food);
	printf("done\n");
}

void add_coupons_func(coupon **coupons, int *coupon_number, FILE *fcoupon) { // add a coupon to last line of coupon.txt
	int capacity, percentage, coupon_id;
	scanf("%d %d %d", &coupon_id, &percentage, &capacity);
	fseek(fcoupon, zero, SEEK_SET);
	if (find_coupon(*(coupons), coupon_id)) {
		printf("coupon id is not unique\n");
		read_one_element_from_screen();
		return;
	}
	coupon * new_coupon = (coupon*)malloc(sizeof(coupon));
	if (new_coupon == NULL) {
		printf("cant alloc coupon in  input_coupon function");
		exit(zero); // porside shavad che konim
	}
	new_coupon->capacity = capacity;
	new_coupon->percentage = percentage;
	new_coupon->coupon_id = coupon_id;
	new_coupon->left_child = NULL;
	new_coupon->right_child = NULL;
	add_to_coupon_tree(coupons, new_coupon);
	fseek(fcoupon, zero, SEEK_END);
	fprintf(fcoupon, "%d %d %d\n", coupon_id, percentage, capacity);
	*(coupon_number) = *(coupon_number)+one;
	printf("done\n");
	read_one_element_from_screen();
}
// functions for report
void food_report_func(food * foods,FILE*ffood_report) {
	if (foods) {
		food_report_func(foods->left_child,ffood_report);
		fprintf(ffood_report,"%i %i %i %i\n", foods->food_id,foods->capacity,foods->reserved,foods->profit); //foods->reserved bug ajib
		food_report_func(foods->right_child,ffood_report);
	}
}
void hall_report_func(hall *halls, FILE *fhall_report) {
	if (halls) {
		hall_report_func(halls->left_child, fhall_report);
		fprintf(fhall_report, "%d %s %d\n", halls->hall_id, halls->name, halls->capacity);
		for (int i = 0; i < number_of_day; i++) {
			fprintf(fhall_report, "%d %d %d\n", i+1, halls->reserved[i], halls->profit[i]);
		}
		hall_report_func(halls->right_child, fhall_report);
	}
}
//functions for wallet
void get_money_func(student* current_student) {
	int amount_to_add;
	scanf("%d", &amount_to_add);
	current_student->wallet += amount_to_add;
	printf("done\n");
	read_one_element_from_screen();
}
void show_money_func(student *current_student) {
	printf("%d\n", current_student->wallet);
}
//menu function
void show_menu_func(food *foods) {
	for (int i = 0; i < number_of_day; i++) {
		printf("%d:\n", i+1);
		find_food_in_day(foods, i+1);
	}
}
//reserve functions
void reserve_func(student *current_student, food *foods, hall *halls) {
	int food_id;
	scanf("%d", &food_id);
	read_one_element_from_screen();
	food *requsted_food = return_food_by_food_id(foods,food_id);
	hall *requsted_hall = return_hall_by_hall_id(halls, requsted_food->hall_id);
	if (!requsted_food || !(requsted_food->capacity - requsted_food->reserved) || !(requsted_hall->capacity - requsted_hall->reserved[requsted_food->day]) || current_student->wallet < requsted_food->price) {
		printf("not possible\n");
		return;
	}
	reserve_food *ptr = current_student->reserved_food;
	while (ptr->next) {
		ptr = ptr->next;
	}
	requsted_food->reserved = requsted_food->reserved + one;
	requsted_food->profit += requsted_food->price;
	current_student->wallet -= requsted_food->price;
	requsted_hall->reserved[(requsted_food->day)-one] += one; //kar nemikone
	requsted_hall->profit[(requsted_food->day) - one] += requsted_food->price;
	reserve_food * new_reserved_food = (reserve_food*)malloc(sizeof(reserve_food));
	new_reserved_food->food_id = requsted_food->food_id;
	new_reserved_food->hall_name = requsted_food->hall_name;
	new_reserved_food->name = requsted_food->name;
	new_reserved_food->next = NULL;
	new_reserved_food->day = requsted_food->day;
	new_reserved_food->price = requsted_food->price;
	ptr->next = new_reserved_food;
	printf("done\n");
}
void reserve_discount_func(student *current_student, food *foods, hall *halls, coupon *coupons) {
	int food_id, coupon_id;
	scanf("%d %d",&food_id, &coupon_id);
	read_one_element_from_screen();
	food *requsted_food = return_food_by_food_id(foods, food_id);
	hall *requsted_hall = return_hall_by_hall_id(halls, requsted_food->hall_id);
	coupon *requsted_coupon = return_coupon_by_coupon_id(coupons, coupon_id);
	if (!requsted_food || !(requsted_food->capacity - requsted_food->reserved) || !(requsted_hall->capacity - requsted_hall->reserved[requsted_food->day]) || current_student->wallet < requsted_food->price || !requsted_coupon||requsted_coupon->capacity==zero) {
		printf("not possible\n");
		return;
	}
	reserve_food *ptr = current_student->reserved_food;
	while (ptr->next) {
		ptr = ptr->next;
	}
	int fee_after_discount = requsted_food->price *requsted_coupon->percentage;
	fee_after_discount /= 100;
	fee_after_discount = requsted_food->price - fee_after_discount;
	requsted_coupon->capacity -= one;
	requsted_food->reserved = requsted_food->reserved + one;
	requsted_food->profit += fee_after_discount;
	current_student->wallet -= fee_after_discount;
	requsted_hall->reserved[requsted_food->day - one] += one; //kar nemikone
	requsted_hall->profit[requsted_food->day - one] += fee_after_discount;
	reserve_food * new_reserved_food = (reserve_food*)malloc(sizeof(reserve_food));
	new_reserved_food->food_id = requsted_food->food_id;
	new_reserved_food->hall_name = requsted_food->hall_name;
	new_reserved_food->name = requsted_food->name;
	new_reserved_food->next = NULL;
	new_reserved_food->day = requsted_food->day;
	new_reserved_food->price = fee_after_discount;
	ptr->next = new_reserved_food;
	printf("done\n");
}
void show_reserved_func(student *current_student) {
	for (int i = 0; i < number_of_day; i++) {
		printf("%d:\n", i + 1);
		find_reserved_food_in_day(current_student->reserved_food, i + 1);
	}
}