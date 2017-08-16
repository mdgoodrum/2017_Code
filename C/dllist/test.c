#include "dllist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Here we are going to write some functions to support a dllist that stores
 * person data (name, age)
 */
struct person {
	char *name;
	int age;
};

/* Example functions given to you. If you want to truly grasp these concepts,
 * try writing your own structs and functions as well!
 */

// Create a new person
struct person *create_person(const char *name, int age) {
	struct person *p = (struct person*) malloc(sizeof(struct person));
	p->name = malloc(strlen(name) + 1);
	strcpy(p->name, name);
	p->age = age;
	return p;
}

static int print_count;
static char *last_printed;

// Print a person
void print_person(void *data) {
	struct person *p = (struct person*) data;
	++print_count;
	last_printed = p->name;
	printf("%s, %d\n", p->name, p->age);
}

// Copy a person
void *copy_person(const void *data) {
	struct person *p = (struct person*) data;
	return create_person(p->name, p->age);
}

// Free a person
void free_person(void *data) {
	// This is safe because we should only be passing in person struct pointers
	struct person *p = (struct person*) data;
	// free any malloc'd pointers contained in the person struct (just name)
	free(p->name);
	// Now free the struct itself; this takes care of non-malloc'd data, like age.
	free(p);
}

// Compare people by age
int person_age_comp(const void *data1, const void *data2) {
	struct person *p1 = (struct person*) data1;
	struct person *p2 = (struct person*) data2;
	return p1->age - p2->age;
}

// Compare people by name
int person_name_comp(const void *data1, const void *data2) {
	struct person *p1 = (struct person*) data1;
	struct person *p2 = (struct person*) data2;
	return strcmp(p1->name, p2->name);
}

// Tell if a person is age 50
int is_age_50(const void *data) {
	struct person *p = (struct person*) data;
	return p->age == 50;
}

// Tell if a person is 23 or older
int is_age_23_or_greater(const void *data) {
	struct person *p = (struct person*) data;
	return p->age >= 23;
}

/* This main function does a little testing. Like all good CS Majors, you should
 * test your code here. There is no substitute for testing and you should be
 * sure to test for all edge cases, like calling empty_dllist on an empty dllist.
 */
int main(void) {

	printf("\nTEST CASE 1\nAn empty dllist should have size 0:\n");
	dllist *d1 = create_dllist();
	printf("Size: %d\n", size(d1));
	assert(!size(d1));
	assert(!d1->head);
	assert(!d1->tail);
	assert(is_empty(d1));

	printf("\nTEST CASE 2\nAfter adding one element, size should be 1:\n");
	push_front(d1, create_person("Andrew", 26));
	printf("Size: %d\n", size(d1));
	assert(size(d1) == 1);
	assert(!is_empty(d1));

	printf("\nTEST CASE 3\nTraversal should print the one element:\n");
	print_count = 0;
	last_printed = NULL;
	traverse(d1, print_person);
	assert(!strcmp(last_printed, "Andrew") && print_count == 1);

	printf("\nTEST CASE 4\nThe one element should be the front:\n");
	print_count = 0;
	last_printed = NULL;
	print_person(front(d1));
	assert(!strcmp(last_printed, "Andrew") && print_count == 1);

	printf("\nTEST CASE 5\nThe one element should be the back:\n");
	print_count = 0;
	last_printed = NULL;
	print_person(back(d1));
	assert(!strcmp(last_printed, "Andrew") && print_count == 1);

	printf("\nTEST CASE 6\nAndrew should be contained in the dllist:\n");
	struct person *andrew = create_person("Andrew", 50);
	printf("Andrew %s in dllist\n", contains(d1, andrew, person_name_comp) ?
			"is" : "isn\'t");
	struct person *ptemp = (struct person*) contains(d1, andrew, person_name_comp);
	assert(ptemp->age == 26);

	printf("\nTEST CASE 7\nCollin should not be contained in the dllist:\n");
	struct person *collin = create_person("Collin", 23);
	printf("Collin %s in dllist\n", contains(d1, collin, person_name_comp) ?
			"is" : "isn\'t");
	assert(!contains(d1, collin, person_name_comp));

	printf("\nTEST CASE 8\nPop Andrew from the front...\n");
	ptemp = (struct person*) pop_front(d1);
	assert(!size(d1));
	assert(!person_name_comp(andrew, ptemp));
	printf("Success!\n");
	free_person(ptemp);

	printf("\nTEST CASE 9\nAdd Collin, pop from the back...\n");
	push_back(d1, collin);
	assert(size(d1) == 1);
	ptemp = pop_back(d1);
	assert(ptemp == collin);
	assert(!size(d1));
	printf("Success!\n");

	// Free these people because we are done using them
	free_person(andrew);
	free_person(collin);

	printf("\nTEST CASE 10\nThere should be 7 elements in this dllist:\n");
	// (L, 66) -> (A, 25) -> (M, 22) -> (N, 23) -> (H, 21) -> (C, 24) -> (B, 28)
	dllist *d2 = create_dllist();
	push_front(d2, create_person("Nick", 23));
	push_back(d2, create_person("Henry", 21));
	push_front(d2, create_person("Marie", 22));
	push_back(d2, create_person("Clayton", 24));
	push_front(d2, create_person("Andrew", 25));
	push_back(d2, create_person("Brandon", 28));
	push_front(d2, create_person("Leahy", 66));
	printf("Size: %d\n", size(d2));
	assert(size(d2) == 7);

	printf("\nTEST CASE 11\nFront should be last element pushed to front (Leahy):\n");
	ptemp = (struct person*) front(d2);
	print_person(ptemp);
	struct person *leahy = create_person("Leahy", 67);
	assert(!person_name_comp(leahy, ptemp));
	assert(ptemp->age == 66);

	printf("\nTEST CASE 12\nBack should be last element pushed to back (Brandon):\n");
	ptemp = (struct person*) back(d2);
	print_person(ptemp);
	struct person *brandon = create_person("Brandon", 29);
	assert(!person_name_comp(brandon, ptemp));
	assert(ptemp->age == 28);

	printf("\nTEST CASE 13\nThe person at index 2 should be Marie:\n");
	ptemp = (struct person*) get(d2, 2);
	print_person(ptemp);
	struct person *marie = create_person("Marie", 23);
	assert(!person_name_comp(marie, ptemp));
	assert(ptemp->age == 22);

	printf("\nTEST CASE 14\nThere should be someone age 24 in the dllist:\n");
	struct person *person24 = create_person("", 24);
	ptemp = (struct person*) contains(d2, person24, person_age_comp);
	print_person(ptemp);
	assert(!strcmp(ptemp->name, "Clayton"));

	printf("\nTEST CASE 15\nThere should be nobody age 30 in the dllist:\n");
	struct person *person30 = create_person("", 30);
	ptemp = (struct person*) contains(d2, person30, person_age_comp);
	if(ptemp) {
		printf("Found: ");
		print_person(ptemp);
	} else {
		printf("Success!\n");
	}
	assert(!ptemp);

	// Clean up temporary people
	free_person(leahy);
	free_person(brandon);
	free_person(marie);
	free_person(person24);
	free_person(person30);

	printf("\nTEST CASE 16\nPopping front and back leaves a size 5 dllist:\n");
	// (A, 25) -> (M, 22) -> (N, 23) -> (H, 21) -> (C, 24)
	assert(ptemp = (struct person*) pop_front(d2));
	free_person(ptemp);
	assert(ptemp = (struct person*) pop_back(d2));
	free_person(ptemp);
	printf("Size: %d\n", size(d2));
	assert(size(d2) == 5);

	printf("\nTEST CASE 17\nPrint the 5 elements from the dllist:\n");
	print_count = 0;
	last_printed = NULL;
	traverse(d2, print_person);
	assert(print_count == 5);
	assert(!strcmp(last_printed, "Clayton"));


	// Empty the dllist now that we're done with it
	empty_dllist(d2, free_person);
	assert(!size(d2));

	printf("\nTEST CASE 18\nLet's try making a copy of a dllist of 4 people:\n");
	dllist *d3 = create_dllist();
	push_front(d3, create_person("Baijun", 20));
	push_front(d3, create_person("Brandi", 21));
	push_front(d3, create_person("Patrick", 19));
	push_front(d3, create_person("Shayan", 22));
	printf("Original:\n");
	traverse(d3, print_person);
	printf("Copy:\n");
	dllist *d4 = copy_dllist(d3, copy_person);
	print_count = 0;
	last_printed = NULL;
	traverse(d4, print_person);
	assert(print_count == 4);
	assert(!strcmp(last_printed, "Baijun"));

	printf("\nTEST CASE 19\nAfter emptying first dllist, second should persist:\n");
	empty_dllist(d3, free_person);
	print_count = 0;
	last_printed = NULL;
	traverse(d4, print_person);
	assert(print_count == 4);
	assert(!strcmp(last_printed, "Baijun"));
	empty_dllist(d4, free_person);
	assert(!size(d4));

	printf("\nTEST CASE 20\nPassing in invalid indexes for add should return false\n");
	struct person* michael = create_person("Michael", 20);
	assert(!add(d4, michael, -1));
	assert(!add(d4, michael, 1));
	free_person(michael);
	printf("Success!\n");

	printf("\nTEST CASE 21\nPassing in size for add should be the same as push_back\n");
	assert(add(d4, create_person("Michael", 20), 0));
	assert(add(d4, create_person("Sanjay", 20), 1));

	ptemp = (struct person*) get(d4, 1);
	struct person *sanjay = create_person("Sanjay", 20);
	assert(!person_name_comp(sanjay, ptemp));

	empty_dllist(d4, free_person);
	free_person(sanjay);
	printf("Success!\n");

	printf("\nTEST CASE 22\nInserting into the middle of a list\n");
	assert(add(d4, create_person("Michael", 20), 0));
	assert(add(d4, create_person("Sanjay", 20), 1));
	assert(add(d4, create_person("Henry", 20), 2));
	assert(add(d4, create_person("Leahy", 20), 3));

	ptemp = (struct person*) get(d4, 2);
	struct person *henry = create_person("Henry", 20);
	assert(!person_name_comp(henry, ptemp));
	empty_dllist(d4, free_person);
	free_person(henry);
	printf("Success!\n");

	printf("\nTEST CASE 23\nReversing a linked list\n");
	assert(is_empty(d4));

	push_back(d4, create_person("Michael", 20));
	push_back(d4, create_person("Henry", 20));
	push_back(d4, create_person("Sanjay", 20));
	push_back(d4, create_person("Leahy", 20));

	reverse(d4);

	ptemp = (struct person*) pop_front(d4);
	leahy = create_person("Leahy", 20);
	assert(!person_name_comp(leahy, ptemp));
	free_person(ptemp);

	ptemp = (struct person*) pop_front(d4);
	sanjay = create_person("Sanjay", 20);
	assert(!person_name_comp(sanjay, ptemp));
	free_person(ptemp);

	ptemp = (struct person*) pop_front(d4);
	henry = create_person("Henry", 20);
	assert(!person_name_comp(henry, ptemp));
	free_person(ptemp);

	ptemp = (struct person*) pop_front(d4);
	michael = create_person("Michael", 20);
	assert(!person_name_comp(michael, ptemp));
	free_person(ptemp);

	free_person(leahy);
	free_person(sanjay);
	free_person(henry);
	free_person(michael);

	assert(is_empty(d4));

	printf("Success!\n");

	printf("\nTEST CASE 24\nPassing in null dllists to functions should return 0...\n");
	assert(!copy_dllist(NULL, NULL));
	assert(!front(NULL));
	assert(!back(NULL));
	assert(!is_empty(NULL));
	assert(!size(NULL));
	assert(!contains(NULL, NULL, NULL));
	assert(!pop_front(NULL));
	assert(!pop_back(NULL));
	assert(!add(NULL, NULL, 0));
	printf("Success!\n");



	// TODO
	printf("\nMake sure to write more test cases as well in test.c! "
			"The ones given aren't comprehensive.\n"
			"Also test using valgrind. Half credit will be given to functions "
			"with memory leaks or memory errors.\n");



	// Clean up any malloc'd structures leftover to prevent memory leaks
	free(d1);
	free(d2);
	free(d3);
	free(d4);

	return 0;
}
