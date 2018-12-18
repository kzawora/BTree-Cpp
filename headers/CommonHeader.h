#ifndef SBD2_COMMONHEADER_H
#define SBD2_COMMONHEADER_H

// funkcje pomocnicze, np wizualizacja drzewa
#define BTREE_D (2)
#define BTREE_2D (2 * BTREE_D)
#define BTREE_ELEMENTS_PER_PAGE (BTREE_2D + 2) // BTREE_2D keys per page + 1 for final child pointer + 1 for overflows.
#define BTREE_PAGE_SIZE ((BTREE_ELEMENTS_PER_PAGE * 4) + 1) * 4
#define MAX_SIZE ~(1 << 31)
#define RECORDS_PER_PAGE (8)
#define RECORD_SIZE (sizeof(int) + MAX_RECORD_SIZE * sizeof(double))
#define RECORD_PAGE_SIZE (RECORDS_PER_PAGE * RECORD_SIZE)
#define MAX_RECORD_SIZE (15)
#define DIST_LOWER_LIMIT (-10)
#define DIST_UPPER_LIMIT (10)

class bytearray {
public:
    char *arr;
    int size;

    bytearray(int _size) : size(_size) { arr = new char[_size]{}; }

    ~bytearray() { delete[] arr; }
};

#endif