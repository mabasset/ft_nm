#include "src/ft_nm.h"

int *create_num() {
    int x = 3;

    return &x;
}

int main() {
    int *num = create_num();
    printf("%d\n", *num);
}