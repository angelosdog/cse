//
//  allocator.h
//  COMP1927 Assignment 1 - Suballocator
//
//  Created by Liam O'Connor on 18/07/12.
//  Copyright (c) 2012 UNSW. All rights reserved.
//

#ifndef allocator_allocator_h
#define allocator_allocator_h

#include <stdlib.h>

// Input: size - number of bytes to make available to the suballocator
// Output: none              
// Precondition: Size is a power of two.
// Postcondition: `size` bytes are now available to the suballocator
// 
// (If the suballocator is already initialised, this function does nothing, even 
// if it was initialisd with different size)
void allocator_init(u_int32_t size);

// Input: n - number of bytes requested
// Output: p - a pointer, or NULL
// Precondition: n is < size of memory available to the suballocator
// Postcondition: If a region of size n or greater cannot be found, p = NULL 
//                Else, p points to a location immediately after a header block
//                      for a newly-allocated region of some size >= 
//                      n + header size. 
void *allocator_malloc(u_int32_t n);

// Input: object, a pointer.
// Output: none
// Precondition: object points to a location immediately after a header block
//               within the suballocator's memory.
// Postcondition: The region pointed to by object can be re-allocated by 
//                allocator_malloc
void allocator_free(void *object);

// Stop the allocator, so that it can be init'ed again:
void allocator_end(void);


#endif
