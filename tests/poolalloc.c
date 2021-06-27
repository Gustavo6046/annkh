#include "munit/munit.h"

#include "poollist.h"


struct p_root all_numbers = POOL_ROOT(int, 4);

void list_length_and_middle_free(void) {
    int idx;

    struct pl_pool_list mylist = pl_make(&all_numbers, 2);

    int someints[] = { 2, 3, 4 };

    // add three numbers
    idx = pl_insert(&mylist, &someints[0]);
    munit_assert_int(idx, ==, 0);

    idx = pl_insert(&mylist, &someints[1]);
    munit_assert_int(idx, ==, 1);

    idx = pl_insert(&mylist, &someints[2]);
    munit_assert_int(idx, ==, 2);

    munit_assert_int(mylist.length, ==, 3);
    munit_assert_int(mylist.head, ==, 3);
    munit_assert_int(mylist.middle_freed, ==, 0);

    munit_assert_true(pl_has(&mylist, 0));
    munit_assert_true(pl_has(&mylist, 1));
    munit_assert_true(pl_has(&mylist, 2));

    // pop two and ensure they are 4 and 3
    int num;

    munit_assert_true(pl_pop(&mylist, &num));
    munit_assert_false(pl_has(&mylist, 2));
    munit_assert_true(pl_has(&mylist, 1));
    munit_assert_int(num, ==, 4);

    munit_assert_true(pl_pop(&mylist, &num));
    munit_assert_false(pl_has(&mylist, 1));
    munit_assert_int(num, ==, 3);

    munit_assert_int(mylist.length, ==, 1);
    munit_assert_int(mylist.head, ==, 1);
    munit_assert_int(mylist.middle_freed, ==, 0);

    munit_assert_false(pl_has(&mylist, 1));
    munit_assert_false(pl_has(&mylist, 2));

    // make it 3 long again

    pl_insert(&mylist, &someints[1]);
    munit_assert_false(pl_has(&mylist, 2));
    munit_assert_true(pl_has(&mylist, 1));

    pl_insert(&mylist, &someints[2]);
    munit_assert_true(pl_has(&mylist, 1));
    munit_assert_true(pl_has(&mylist, 2));

    munit_assert_int(mylist.length, ==, 3);
    munit_assert_int(mylist.head, ==, 3);
    munit_assert_int(mylist.middle_freed, ==, 0);


    // remove the middle and make sure it sets middle_freed
    // and head properly

    munit_assert_true(pl_remove(&mylist, 1, &num));
    munit_assert_int(num, ==, 3);

    munit_assert_int(mylist.length, ==, 2);
    munit_assert_int(mylist.head, ==, 3);
    munit_assert_int(mylist.middle_freed, ==, 1);

    // pop the head and ensure head is decremented
    // all the way back to 1 and middle_freed is
    // reset

    munit_assert_true(pl_pop(&mylist, &num));
    munit_assert_int(num, ==, 4);

    munit_assert_int(mylist.length, ==, 1);
    munit_assert_int(mylist.head, ==, 1);
    munit_assert_int(mylist.middle_freed, ==, 0);

    // make it 3 long again...

    pl_insert(&mylist, &someints[1]);
    pl_insert(&mylist, &someints[2]);

    munit_assert_int(mylist.length, ==, 3);
    munit_assert_int(mylist.head, ==, 3);
    munit_assert_int(mylist.middle_freed, ==, 0);

    // ...remove the middle again...

    munit_assert_true(pl_remove(&mylist, 1, &num));
    munit_assert_int(num, ==, 3);

    munit_assert_int(mylist.length, ==, 2);
    munit_assert_int(mylist.head, ==, 3);
    munit_assert_int(mylist.middle_freed, ==, 1);

    // ...and ensure the next insert fills it back in!

    pl_insert(&mylist, &someints[1]);

    munit_assert_int(mylist.length, ==, 3);
    munit_assert_int(mylist.head, ==, 3);
    munit_assert_int(mylist.middle_freed, ==, 0);

    munit_assert_true(pl_has(&mylist, 1));
}

int main(void) {
    list_length_and_middle_free();
}
