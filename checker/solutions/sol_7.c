#include <stdio.h>

int main () {
    int l;
    scanf("%d", &l);
    for (int i = 0; i < l; i++) {
        int n;
        scanf("%d", &n);
        if (n < 7) {
            puts("NO");
            continue;
        }
        switch (n % 3) {
            case 1:
            case 2:
                printf("YES\n1 2 %d\n", n-3);
                break;

            case 0:
                if (n==9) puts("NO");
                else printf("YES\n1 4 %d\n", n-5);
        }
    }
    return 0;
}