/** @file dummy.h
 *
 * @brief Definition of a dummy structure for tests.
 *
 * @author Jean-Raphaël GAGLIONE
 */

#ifndef DUMMY_H
#define DUMMY_H

/** @brief An alias to the structure used for tests. */
typedef struct dummy dummy_t;

/** @brief The structure representing the binary heap. */
struct dummy {
    double key;
    char  *value;
};

/** @brief Compare two dummy structures.
 * @param dummy1  the first dummy to compare
 * @param dummy2  the second dummy to compare
 * @return  -1,0,1 if the first dummy is respectively lower, equal, greater
 *          than the second dummy
 */
int compare_dummies(void *dummy1, void *dummy2) {
    dummy_t *d1 = dummy1;
    dummy_t *d2 = dummy2;
    return (d1->key < d2->key) ? -1 :
           (d1->key > d2->key) ?  1 :
                                  0 ;
}

#endif
