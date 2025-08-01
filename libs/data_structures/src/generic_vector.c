/**
 * @file generic_vector.c
 * @brief Generic dynamic array implementation using void pointers
 * @author dunamismax <dunamismax@tutamail.com>
 * @version 2.0.0
 * @date 2024
 *
 * Production-ready generic vector implementation in C
 * Supports any data type through void pointers and element size specification
 */

#include "generic_vector.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Default initial capacity
#define DEFAULT_CAPACITY 4
#define GROWTH_FACTOR 2
#define SHRINK_THRESHOLD 4

/**
 * Safe memory allocation with overflow checking
 */
static void *safe_malloc(size_t count, size_t size) {
  if (count == 0 || size == 0) {
    return NULL;
  }

  // Check for multiplication overflow
  if (count > SIZE_MAX / size) {
    return NULL;
  }

  return malloc(count * size);
}

/**
 * Safe memory reallocation with overflow checking
 */
static void *safe_realloc(void *ptr, size_t count, size_t size) {
  if (count == 0 || size == 0) {
    free(ptr);
    return NULL;
  }

  // Check for multiplication overflow
  if (count > SIZE_MAX / size) {
    return NULL;
  }

  return realloc(ptr, count * size);
}

/**
 * Grow vector capacity when needed
 */
static int gvector_grow(GenericVector *vec) {
  if (!vec) {
    return -1;
  }

  size_t new_capacity =
      vec->capacity == 0 ? DEFAULT_CAPACITY : vec->capacity * GROWTH_FACTOR;
  void *new_data = safe_realloc(vec->data, new_capacity, vec->element_size);

  if (!new_data) {
    return -1;
  }

  vec->data = new_data;
  vec->capacity = new_capacity;
  return 0;
}

/**
 * Shrink vector capacity if size is much smaller than capacity
 */
static int gvector_maybe_shrink(GenericVector *vec) {
  if (!vec || vec->capacity <= DEFAULT_CAPACITY) {
    return 0;
  }

  if (vec->size * SHRINK_THRESHOLD <= vec->capacity) {
    size_t new_capacity = vec->capacity / GROWTH_FACTOR;
    if (new_capacity < DEFAULT_CAPACITY) {
      new_capacity = DEFAULT_CAPACITY;
    }

    void *new_data = safe_realloc(vec->data, new_capacity, vec->element_size);
    if (new_data) {
      vec->data = new_data;
      vec->capacity = new_capacity;
    }
  }

  return 0;
}

GenericVector *gvector_create(size_t initial_capacity, size_t element_size) {
  if (element_size == 0) {
    return NULL;
  }

  GenericVector *vec = malloc(sizeof(GenericVector));
  if (!vec) {
    return NULL;
  }

  vec->element_size = element_size;
  vec->size = 0;
  vec->capacity = initial_capacity > 0 ? initial_capacity : DEFAULT_CAPACITY;

  vec->data = safe_malloc(vec->capacity, element_size);
  if (!vec->data) {
    free(vec);
    return NULL;
  }

  return vec;
}

void gvector_destroy(GenericVector *vec) {
  if (vec) {
    free(vec->data);
    free(vec);
  }
}

int gvector_push(GenericVector *vec, const void *element) {
  if (!vec || !element) {
    return -1;
  }

  if (vec->size >= vec->capacity) {
    if (gvector_grow(vec) != 0) {
      return -1;
    }
  }

  char *dest = (char *)vec->data + (vec->size * vec->element_size);
  memcpy(dest, element, vec->element_size);
  vec->size++;

  return 0;
}

int gvector_pop(GenericVector *vec, void *element) {
  if (!vec || vec->size == 0) {
    return -1;
  }

  vec->size--;

  if (element) {
    const char *src = (const char *)vec->data + (vec->size * vec->element_size);
    memcpy(element, src, vec->element_size);
  }

  gvector_maybe_shrink(vec);
  return 0;
}

int gvector_get(const GenericVector *vec, size_t index, void *element) {
  if (!vec || !element || index >= vec->size) {
    return -1;
  }

  const char *src = (const char *)vec->data + (index * vec->element_size);
  memcpy(element, src, vec->element_size);

  return 0;
}

int gvector_set(GenericVector *vec, size_t index, const void *element) {
  if (!vec || !element || index >= vec->size) {
    return -1;
  }

  char *dest = (char *)vec->data + (index * vec->element_size);
  memcpy(dest, element, vec->element_size);

  return 0;
}

int gvector_insert(GenericVector *vec, size_t index, const void *element) {
  if (!vec || !element || index > vec->size) {
    return -1;
  }

  if (vec->size >= vec->capacity) {
    if (gvector_grow(vec) != 0) {
      return -1;
    }
  }

  // Move elements after index one position to the right
  if (index < vec->size) {
    char *data = (char *)vec->data;
    char *src = data + (index * vec->element_size);
    char *dest = data + ((index + 1) * vec->element_size);
    size_t bytes_to_move = (vec->size - index) * vec->element_size;
    memmove(dest, src, bytes_to_move);
  }

  // Insert new element
  char *dest = (char *)vec->data + (index * vec->element_size);
  memcpy(dest, element, vec->element_size);
  vec->size++;

  return 0;
}

int gvector_remove(GenericVector *vec, size_t index, void *element) {
  if (!vec || index >= vec->size) {
    return -1;
  }

  // Copy element if requested
  if (element) {
    const char *src = (const char *)vec->data + (index * vec->element_size);
    memcpy(element, src, vec->element_size);
  }

  // Move elements after index one position to the left
  if (index < vec->size - 1) {
    char *data = (char *)vec->data;
    char *dest = data + (index * vec->element_size);
    char *src = data + ((index + 1) * vec->element_size);
    size_t bytes_to_move = (vec->size - index - 1) * vec->element_size;
    memmove(dest, src, bytes_to_move);
  }

  vec->size--;
  gvector_maybe_shrink(vec);

  return 0;
}

size_t gvector_size(const GenericVector *vec) { return vec ? vec->size : 0; }

size_t gvector_capacity(const GenericVector *vec) {
  return vec ? vec->capacity : 0;
}

int gvector_empty(const GenericVector *vec) {
  if (!vec) {
    return -1;
  }
  return vec->size == 0 ? 1 : 0;
}

void gvector_clear(GenericVector *vec) {
  if (vec) {
    vec->size = 0;
    gvector_maybe_shrink(vec);
  }
}

int gvector_reserve(GenericVector *vec, size_t new_capacity) {
  if (!vec || new_capacity <= vec->capacity) {
    return 0;
  }

  void *new_data = safe_realloc(vec->data, new_capacity, vec->element_size);
  if (!new_data) {
    return -1;
  }

  vec->data = new_data;
  vec->capacity = new_capacity;

  return 0;
}

int gvector_shrink_to_fit(GenericVector *vec) {
  if (!vec || vec->size == vec->capacity) {
    return 0;
  }

  size_t new_capacity = vec->size > 0 ? vec->size : DEFAULT_CAPACITY;
  void *new_data = safe_realloc(vec->data, new_capacity, vec->element_size);

  if (!new_data) {
    return -1;
  }

  vec->data = new_data;
  vec->capacity = new_capacity;

  return 0;
}

size_t gvector_find(const GenericVector *vec, const void *element,
                    CompareFunc compare) {
  if (!vec || !element || !compare) {
    return SIZE_MAX;
  }

  for (size_t i = 0; i < vec->size; i++) {
    const char *current = (const char *)vec->data + (i * vec->element_size);
    if (compare(current, element) == 0) {
      return i;
    }
  }

  return SIZE_MAX;
}

int gvector_sort(GenericVector *vec, CompareFunc compare) {
  if (!vec || !compare || vec->size <= 1) {
    return 0;
  }

  qsort(vec->data, vec->size, vec->element_size, compare);
  return 0;
}

void gvector_foreach(GenericVector *vec, ProcessFunc process, void *user_data) {
  if (!vec || !process) {
    return;
  }

  for (size_t i = 0; i < vec->size; i++) {
    char *element = (char *)vec->data + (i * vec->element_size);
    process(element, user_data);
  }
}

GenericVector *gvector_copy(const GenericVector *vec) {
  if (!vec) {
    return NULL;
  }

  GenericVector *new_vec = gvector_create(vec->capacity, vec->element_size);
  if (!new_vec) {
    return NULL;
  }

  if (vec->size > 0) {
    memcpy(new_vec->data, vec->data, vec->size * vec->element_size);
    new_vec->size = vec->size;
  }

  return new_vec;
}

void *gvector_at(const GenericVector *vec, size_t index) {
  if (!vec || index >= vec->size) {
    return NULL;
  }

  return (char *)vec->data + (index * vec->element_size);
}

void *gvector_front(const GenericVector *vec) {
  if (!vec || vec->size == 0) {
    return NULL;
  }

  return vec->data;
}

void *gvector_back(const GenericVector *vec) {
  if (!vec || vec->size == 0) {
    return NULL;
  }

  return (char *)vec->data + ((vec->size - 1) * vec->element_size);
}

void *gvector_data(const GenericVector *vec) { return vec ? vec->data : NULL; }