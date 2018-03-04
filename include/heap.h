/** @file heap.h
 *
 * @brief Simple definition of a binary heap containing pointers to values.
 *
 * @author Jean-Raphaël GAGLIONE
 *
 * This heap does not handle NULL values.
 *
 * Inserting a value and extracting the minimum value are guaranteed
 * to have a worst-case complexity in $O(\log n)$ where $n$ is the
 * size of the heap.
 *
 * The API of the binary heap is defined as follows:
 *
 * - a function to create an empty heap
 * - a function to check if the heap is empty
 * - a function to insert a new value in the heap
 * - a function to get the minimum value in the heap
 * - a function to deallocate the binary heap
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>

/** @brief An alias to the structure representing a binary heap. */
typedef struct heap heap_t;

/** @brief An alias to comparator functions.
 *
 * Comparator functions are functions that takes two heap elements and
 * compare them by returning:
 * - a negative value if `v1<v2`
 * - `0` if `v1==v2`
 * - a positive value if `v1>v2`
 */
typedef int (*compare_func_t)(void *v1, void *v2);

/** @brief The structure representing the binary heap. */
struct heap;



/**
 * @brief Create a nil binary heap.
 *
 * @param comparator  the comparator function used by the elements
 *
 * @return  a new empty binary heap
 *
 * @pre  `comparator` is not `NULL`
 */
heap_t *heap_new (compare_func_t comparator);

/**
 * @brief Is the binary heap empty?
 *
 * @param p_heap  a pointer to the heap you want to
 *               check the emptiness
 *
 * @return  `true` is `p_heap` is empty, `false` else
 *
 * @pre  `p_heap` is not `NULL`
 */
bool heap_is_empty (heap_t const *p_heap);

/**
 * @brief Insert a new value in the binary heap.
 *
 * The worst-case execution time of this function
 * is guaranteed to be in \f$\log n\f$ where
 * \f$n\f$ is the size of the binary heap.
 *
 * @param p_heap  a pointer to the heap in which the value is
 *                to be inserted
 * @param value   the value to be inserted
 *
 * @pre  `p_heap` is not `NULL`
 * @pre  `value` is not `NULL`
 *
 * @post  After the call, `p_heap` will point to a binary heap
 *        in which the value `value` has been inserted according
 *        to the algorithm presented in the lab session.
 */
void heap_insert (heap_t *p_heap, void *value);

/** @brief Extract the minimum value in the binary heap.
 *
 * The worst-case execution time of this function
 * is guaranteed to be in \f$\log n\f$ where \f$n\f$ is
 * the size of the binary heap.
 *
 * @param p_heap  a pointer to the heap from which the value is
 *                to be extracted
 *
 * @return  the minimum value in the binary heap, or `NULL` if the heap is empty
 *
 * @pre  `p_heap` is not `NULL`
 */
void *heap_extract_min (heap_t *p_heap);

/** @brief Deallocate the binary heap and free the pointer.
 *
 * @param p_heap  a pointer to the binary heap to be deallocated
 *
 * @pre  `p_heap` is not `NULL`
 *
 * @post  After the call, all memory regions used for the nodes
 *        are deallocated. Values are also deallocated.
 */
void heap_deallocate (heap_t *p_heap);

#endif
