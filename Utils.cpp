// funkcje pomocnicze, np wizualizacja drzewa
#pragma once
#define BTREE_D (2)
#define BTREE_2D (2 * BTREE_D)
#define BTREE_ELEMENTS_PER_PAGE (BTREE_2D + 2) // BTREE_2D keys per page + 1 for final child pointer + 1 for overflows.
#define BTREE_PAGE_SIZE ((BTREE_ELEMENTS_PER_PAGE * 4) + 1) * 4
#define MAX_SIZE ~(1 << 31)

class bytearray {
  public:
    char* arr;
    int size;
    bytearray(int _size) : size(_size) { arr = new char[_size]{}; }
    ~bytearray() { delete[] arr; }
};
