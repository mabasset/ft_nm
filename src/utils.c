#include "ft_nm.h"

int ft_strlen(char *s) {
    int i;

    for (i = 0; s[i]; i++);
    return i;
}

int ft_strcmp(char *s1, char *s2) {
    int i;

    if ()
    for (i = 0; s1[i] && s2[i] ; i++) {
        if (s1[i] > s2[i])
            return -1;
        else if (s1[i] < s2[i])
            return 1;
        else if (s1[i] == s2[i])
            return 0;
    }
    if (!s1[i])
}

int main() {
    printf("%d\n", strcmp("bbbbb", "bbbba"));
    return (0);
}