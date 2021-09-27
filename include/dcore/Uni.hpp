#pragma once

// Macros for pointer clarification.

#define DCORE_OWN /* The object owns the pointer. (it allocates/deallocates it) */
#define DCORE_REF /* A reference to an object through a pointer (e.g. a polymorphic class) */
