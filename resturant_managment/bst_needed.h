#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book_food_library.h"
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
typedef struct  reserve_food  reserve_food;
typedef struct hall hall;
typedef struct food food;
typedef struct coupon coupon;
 struct  reserve_food  { // struct to store reserved food for each students
	int food_id;
	int price;
	char *hall_name;
	char *name;
	int day;
	reserve_food *next;
	//store in linkedlist
};
typedef struct student { // dtruct for students
	char * username;
	int student_id;
	char *password;
	int wallet;
	reserve_food *reserved_food;
	struct student * left_child;
	struct student * right_child;
	//store in bst and sort by name
}student;
typedef struct hall {// struct for halls
	int capacity;
	int hall_id;
	char *name;
	int *profit;
	int *reserved;
	hall *left_child;
	hall *right_child;
	//store in bst and sort by hall id
}hall;
typedef struct food { // struct for foods
	int price;
	int capacity;
	int reserved;
	int profit;
	int hall_id;
	char *hall_name;
	int day;
	char*name;
	int food_id;
	food *left_child;
	food *right_child;
	//store in bst and sort by food id
}food;
typedef struct coupon { // struct for coupon
	int capacity;
	int coupon_id;
	int percentage;
	coupon *left_child;
	coupon *right_child;
	//store in bst and sort by coupon id
}coupon;
// functions for check bst`s empty situation
int isEmpty_student_bst(student *root) {
	if (root == NULL) return 1;
	return 0;
}
int isEmpty_hall_bst(hall* root) {
	if (root==NULL) return 1;
	return 0;
}
int isEmpty_food_bst(food* root) {
	if (root == NULL) return 1;
	return 0;
}
int isEmpty_coupon_bst(coupon* root) {
	if (root == NULL) return 1;
	return 0;
}
// functions for create structes tree
struct student* create_student_binary_tree() {
	struct student * new_node;
	new_node = NULL;
	return new_node;
}
struct hall* create_hall_binary_tree() {
	struct hall * new_node;
	new_node = NULL;
	return new_node;
}
struct food* create_food_binary_tree() {
	struct food * new_node;
	new_node = NULL;
	return new_node;
}
struct coupon* create_coupon_binary_tree() {
	struct coupon * new_node;
	new_node = NULL;
	return new_node;
}
//functions for add element to trees
void add_to_tree(struct student** root_add, struct student* new_std) {
	if (isEmpty_student_bst(*(root_add))) {
		*(root_add) = new_std;
		return;
	}
	struct student * ptr = *(root_add);
	while (TRUE) {
		if (strcmp(ptr->username, new_std->username) > 0 && ptr->left_child == NULL) {
			ptr->left_child = new_std;
			return;
		}
		if (strcmp(ptr->username, new_std->username) < 0 && ptr->right_child == NULL) {
			ptr->right_child = new_std;
			return;
		}
		if (strcmp(ptr->username, new_std->username) > 0 && ptr->left_child) {
			ptr = ptr->left_child;
			continue;
		}
		if (strcmp(ptr->username, new_std->username) < 0 && ptr->right_child) {
			ptr = ptr->right_child;
			continue;
		}
	}
}
void add_to_hall_tree(struct hall** root_add, struct hall* new_hall) {
	if (isEmpty_hall_bst(*(root_add))) {
		*(root_add) = new_hall;
		return;
	}
	struct hall * ptr = *(root_add);
	while (TRUE) {
		if (ptr->hall_id < new_hall->hall_id && ptr->right_child == NULL) {
			ptr->right_child = new_hall;
			return;
		}
		if (ptr->hall_id > new_hall->hall_id && ptr->left_child == NULL) {
			ptr->left_child = new_hall;
			return;
		}
		if (ptr->hall_id < new_hall->hall_id && ptr->right_child) {
			ptr = ptr->right_child;
			continue;
		}
		if (ptr->hall_id > new_hall->hall_id && ptr->left_child) {
			ptr = ptr->left_child;
			continue;
		}
	}
}
void add_to_food_tree(struct food** root_add, struct food* new_food) {
	if (isEmpty_food_bst(*(root_add))) {
		*(root_add) = new_food;
		return;
	}
	struct food * ptr = *(root_add);
	while (TRUE) {
		if (ptr->food_id < new_food->food_id && ptr->right_child == NULL) {
			ptr->right_child = new_food;
			return;
		}
		if (ptr->food_id > new_food->food_id && ptr->left_child == NULL) {
			ptr->left_child = new_food;
			return;
		}
		if (ptr->food_id < new_food->food_id && ptr->right_child) {
			ptr = ptr->right_child;
			continue;
		}
		if (ptr->food_id > new_food->food_id && ptr->left_child) {
			ptr = ptr->left_child;
			return;
		}
	}
}
void add_to_coupon_tree(struct coupon** root_add, struct coupon* new_coupon) {
	if (isEmpty_coupon_bst(*(root_add))) {
		*(root_add) = new_coupon;
		return;
	}
	struct coupon * ptr = *(root_add);
	while (TRUE) {
		if (ptr->coupon_id < new_coupon->coupon_id && ptr->right_child == NULL) {
			ptr->right_child = new_coupon;
			return;
		}
		if (ptr->coupon_id > new_coupon->coupon_id && ptr->left_child == NULL) {
			ptr->left_child = new_coupon;
			return;
		}
		if (ptr->coupon_id < new_coupon->coupon_id && ptr->right_child) {
			ptr = ptr->right_child;
			continue;
		}
		if (ptr->coupon_id > new_coupon->coupon_id && ptr->left_child) {
			ptr = ptr->left_child;
			continue;
		}
	}
}
// print sorted students by name
void print_sorted_bst(struct student* root) {
	if (root) {
		print_sorted_bst(root->left_child);
		printf("%s\n", root->username);
		print_sorted_bst(root->right_child);
	}
}
//functions for check existence of element in trees
int find_student(student* root,char *name,char *password) {
	if (!root) return 0;
	while (TRUE) {
		if (!strcmp(name, root->username) && !strcmp(password, root->password)) {
			return 1;
		}
		if (!strcmp(name, root->username)) return 0;
		if (strcmp(name, root->username) > 0) {
			if (root->right_child == NULL) return 0;
			root = root->right_child;
		}
		else {
			if (root->left_child == NULL)return 0;
			root = root->left_child;
		}
	}
}
int find_hall(hall* root,int hall_id) {
	if (!root) return 0;
	while (TRUE) {
		if (root->hall_id==hall_id) {
			return 1;
		}
		if (hall_id>root->hall_id) {
			if (root->right_child == NULL) return 0;
			root = root->right_child;
		}
		else {
			if (root->left_child == NULL)return 0;
			root = root->left_child;
		}
	}
}
int find_food(food* root, int food_id) {
	if (!root) return 0;
	while (TRUE) {
		if (root->food_id == food_id) {
			return 1;
		}
		if (food_id>root->food_id) {
			if (root->right_child == NULL) return 0;
			root = root->right_child;
		}
		else {
			if (root->left_child == NULL)return 0;
			root = root->left_child;
		}
	}
}
int find_coupon(coupon* root, int coupon_id) {
	if (!root) return 0;
	while (TRUE) {
		if (root->coupon_id == coupon_id) {
			return 1;
		}
		if (coupon_id>root->coupon_id) {
			if (root->right_child == NULL) return 0;
			root = root->right_child;
		}
		else {
			if (root->left_child == NULL)return 0;
			root = root->left_child;
		}
	}
}
// functions for search and return element in trees
food *return_food_by_food_id(food* root, int food_id) {
	if (!root) return NULL;
	while (TRUE) {
		if (root->food_id == food_id) {
			return root;
		}
		if (food_id>root->food_id) {
			if (root->right_child == NULL) return NULL;
			root = root->right_child;
		}
		else {
			if (root->left_child == NULL)return NULL;
			root = root->left_child;
		}
	}
}
hall *return_hall_by_hall_id(hall* root, int hall_id) {
	if (!root) return NULL;
	while (TRUE) {
		if (root->hall_id == hall_id) {
			return root;
		}
		if (hall_id>root->hall_id) {
			if (root->right_child == NULL) return NULL;
			root = root->right_child;
		}
		else {
			if (root->left_child == NULL)return NULL;
			root = root->left_child;
		}
	}
}
coupon *return_coupon_by_coupon_id(coupon* root, int coupon_id) {
	if (!root) return NULL;
	while (TRUE) {
		if (root->coupon_id == coupon_id) {
			return root;
		}
		if (coupon_id>root->coupon_id) {
			if (root->right_child == NULL) return NULL;
			root = root->right_child;
		}
		else {
			if (root->left_child == NULL)return NULL;
			root = root->left_child;
		}
	}
}
//print foods that reserved in spesific day
void find_food_in_day(food *foods,int day) {
	if (!foods) return;
	find_food_in_day(foods->left_child, day);
	if (foods->day == day) {
		printf("%s %d %d %s %d\n", foods->name, foods->price, foods->food_id, foods->hall_name, foods->capacity - foods->reserved);
	}
	find_food_in_day(foods->right_child, day);
}
void find_reserved_food_in_day(reserve_food *reserved_food, int day) {
	reserved_food = reserved_food->next;
	while (reserved_food) {
		if (reserved_food->day == day) {
			printf("%s %d %d %s\n", reserved_food->name, reserved_food->price, reserved_food->food_id, reserved_food->hall_name);
		}
		reserved_food = reserved_food->next;
	}
}
// return hall name with hall id
char *name_of_hall_id(hall *halls, int hall_id) {
	if (!halls) return 0;
	while (TRUE) {
		if (halls->hall_id == hall_id) {
			return halls->name;
		}
		if (hall_id>halls->hall_id) {
			if (halls->right_child == NULL) return 0;
			halls = halls->right_child;
		}
		else {
			if (halls->left_child == NULL)return 0;
			halls = halls->left_child;
		}
	}
}
//functions for free trees
void free_food_tree(food* foods) {
	if (foods) {
		free_food_tree(foods->left_child);
		free_food_tree(foods->right_child);
		free(foods->name);
		free(foods);
	}
}
void free_hall_tree(hall* halls) {
	if (halls) {
		free_hall_tree(halls->left_child);
		free_hall_tree(halls->right_child);
		free(halls->name);
		free(halls);
	}
}
void free_coupon_tree(coupon* coupons) {
	if (coupons) {
		free_coupon_tree(coupons->left_child);
		free_coupon_tree(coupons->right_child);
		free(coupons);
	}
}
void free_reserved_food(reserve_food *reserved_foods) {
	if (reserved_foods) {
		free_reserved_food(reserved_foods->next);
		free(reserved_foods);
	}
}
void free_std_tree(student* students) {
	if (students) {
		free_std_tree(students->left_child);
		free_std_tree(students->right_child);
		free(students->password);
		free_reserved_food(students->reserved_food);
		free(students->username);
		free(students);
	}
}