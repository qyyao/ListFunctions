/**
 * Sample test code, tests that functions work as they are supposed to
 */

#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)

// For checking the "free" function called
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void* pItem) 
{
    CHECK(pItem != NULL);
    complexTestFreeCounter++;
}

// For searching
static bool itemEquals(void* pItem, void* pArg) 
{
    return (pItem == pArg);
}

static void testComplex()
{
    //creating a new list and checking initial stats
    List * list = List_create();
    CHECK(List_count(list) == 0);
    CHECK(List_first(list) == NULL);
    CHECK(List_last(list) == NULL);
    CHECK(List_next(list) == NULL);
    CHECK(List_prev(list) == NULL);

    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    int six = 6;
    int seven = 7;
    int eight = 8;
    int nine = 9;
    int ten = 10;

    int eleven = 11;


    //ADD to an empty list
    List_add(list, &one);

    CHECK(List_curr(list) == &one);
    CHECK(List_next(list) == NULL);
    CHECK(List_prev(list) == &one);
    CHECK(List_prev(list) == NULL);

    //TRIM when there is one item
    CHECK(List_trim(list) == &one);

    CHECK(List_count(list) == 0);
    CHECK(List_first(list) == NULL);
    CHECK(List_last(list) == NULL);
    CHECK(List_next(list) == NULL);
    CHECK(List_prev(list) == NULL);

    //inserting
    CHECK(List_insert(list, &five) == 0);
    CHECK(List_insert(list, &four) == 0);
    CHECK(List_insert(list, &three) == 0);
    
    //appending
    CHECK(List_append(list, &six) == 0);
    CHECK(List_append(list, &seven) == 0);

    //prepending
    CHECK(List_prepend(list, &two) == 0);
    CHECK(List_prepend(list, &one) == 0);

    //adding
    CHECK(List_last(list) == &seven);
    CHECK(List_add(list, &eight) == 0);
    CHECK(List_add(list, &nine) == 0);
    CHECK(List_add(list, &ten) == 0);

    //go through list using next and prev
    CHECK(List_first(list) == &one);
    CHECK(List_prev(list) == NULL);

    CHECK(List_next(list) == &one);
    CHECK(List_next(list) == &two);
    CHECK(List_next(list) == &three);
    CHECK(List_next(list) == &four);
    CHECK(List_next(list) == &five);
    CHECK(List_next(list) == &six);
    CHECK(List_next(list) == &seven);
    CHECK(List_next(list) == &eight);
    CHECK(List_next(list) == &nine);
    CHECK(List_next(list) == &ten);
    CHECK(List_next(list) == NULL);

    CHECK(List_prev(list) == &ten);
    CHECK(List_prev(list) == &nine);
    CHECK(List_prev(list) == &eight);
    CHECK(List_prev(list) == &seven);
    CHECK(List_prev(list) == &six);
    CHECK(List_prev(list) == &five);
    CHECK(List_prev(list) == &four);
    CHECK(List_prev(list) == &three);
    CHECK(List_prev(list) == &two);
    CHECK(List_prev(list) == &one);
    CHECK(List_prev(list) == NULL);

    //adding when all nodes used (given there are 10 available nodes)
   // CHECK(List_add(list, &eleven) == -1);
    CHECK(List_insert(list, &eleven) == -1);
    CHECK(List_prepend(list, &eleven) == -1);
    CHECK(List_append(list, &eleven) == -1);

    //create second list
    List * list2 = List_create();

    //try creating new list when all lists are used (provided only 2 heads).
    List * list3 = List_create();
    CHECK(list3 == NULL);

    CHECK(List_add(list2, &eleven) == -1);
    CHECK(List_insert(list2, &eleven) == -1);
    CHECK(List_prepend(list2, &eleven) == -1);
    CHECK(List_append(list2, &eleven) == -1);    

    //remove from list one using trim and remove
    CHECK(List_trim(list) == &ten);
    CHECK(List_trim(list) == &nine);
    CHECK(List_trim(list) == &eight);

    CHECK(List_prev(list) == &six);
    CHECK(List_remove(list) == &six);
    CHECK(List_remove(list) == &seven);

    //add them to list 2
    CHECK(List_add(list2, &six) == 0);
    CHECK(List_append(list2, &seven) == 0);  
    CHECK(List_append(list2, &eight) == 0);
    CHECK(List_append(list2, &nine) == 0);
    CHECK(List_append(list2, &ten) == 0);  

    //JOIN the two lists
    List_concat(list, list2);

    CHECK(List_prev(list) == &ten);
    CHECK(List_prev(list) == &nine);
    CHECK(List_prev(list) == &eight);
    CHECK(List_prev(list) == &seven);
    CHECK(List_prev(list) == &six);
    CHECK(List_prev(list) == &five);
    CHECK(List_prev(list) == &four);
    CHECK(List_prev(list) == &three);
    CHECK(List_prev(list) == &two);
    CHECK(List_prev(list) == &one);
    CHECK(List_prev(list) == NULL);

    CHECK(List_next(list) == &one);
    CHECK(List_next(list) == &two);
    CHECK(List_next(list) == &three);
    CHECK(List_next(list) == &four);
    CHECK(List_next(list) == &five);
    CHECK(List_next(list) == &six);
    CHECK(List_next(list) == &seven);
    CHECK(List_next(list) == &eight);
    CHECK(List_next(list) == &nine);
    CHECK(List_next(list) == &ten);
    CHECK(List_next(list) == NULL);

    //we can now make another list 
    List * list4 = List_create();
    CHECK(list4 != NULL);

    //but we should NOT be able to add new nodes to it
    CHECK(List_add(list4, &eleven) == -1);
    CHECK(List_insert(list4, &eleven) == -1);
    CHECK(List_prepend(list4, &eleven) == -1);
    CHECK(List_append(list4, &eleven) == -1);  

    //try freeing the main list
    complexTestFreeCounter = 0;
    List_free(list, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 10);

    //now we can reuse the nodes for list 4!

    CHECK(List_prepend(list4, &ten) == 0);
    CHECK(List_prepend(list4, &nine) == 0);    
    CHECK(List_prepend(list4, &eight) == 0);
    CHECK(List_prepend(list4, &seven) == 0);
    CHECK(List_prepend(list4, &six) == 0);
    CHECK(List_prepend(list4, &five) == 0);
    CHECK(List_prepend(list4, &four) == 0);
    CHECK(List_prepend(list4, &three) == 0);
    CHECK(List_prepend(list4, &two) == 0);
    CHECK(List_prepend(list4, &one) == 0);

    CHECK(List_prev(list4) == NULL); //check all the right values are there

    CHECK(List_next(list4) == &one);
    CHECK(List_next(list4) == &two);
    CHECK(List_next(list4) == &three);
    CHECK(List_next(list4) == &four);
    CHECK(List_next(list4) == &five);
    CHECK(List_next(list4) == &six);
    CHECK(List_next(list4) == &seven);
    CHECK(List_next(list4) == &eight);
    CHECK(List_next(list4) == &nine);
    CHECK(List_next(list4) == &ten);
    CHECK(List_next(list4) == NULL);

    CHECK(List_prev(list4) == &ten);
    CHECK(List_prev(list4) == &nine);
    CHECK(List_prev(list4) == &eight);
    CHECK(List_prev(list4) == &seven);
    CHECK(List_prev(list4) == &six);
    CHECK(List_prev(list4) == &five);
    CHECK(List_prev(list4) == &four);
    CHECK(List_prev(list4) == &three);
    CHECK(List_prev(list4) == &two);
    CHECK(List_prev(list4) == &one);
    CHECK(List_prev(list4) == NULL);

    //SEARCH
    CHECK(List_search(list4, itemEquals, &five) == &five);
    CHECK(List_search(list4, itemEquals, &four) == NULL);
    List_first(list4);
    CHECK(List_search(list4, itemEquals, &six) == &six);
    CHECK(List_search(list4, itemEquals, &five) == NULL);
    List_first(list4);
    CHECK(List_search(list4, itemEquals, &eleven) == NULL);
}

int main(int argCount, char *args[]) 
{
    testComplex();

    // We got here?!? PASSED!
    printf("********************************\n");
    printf("           PASSED\n");
    printf("********************************\n");
    return 0;
}