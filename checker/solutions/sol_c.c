#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a,b;
    scanf("%d",&a);
    while(a--) {
        scanf("%d",&b);
        if(b<7||b==9) puts("NO");
        else {puts("YES");
            if(b%3==0) printf("1 4 %d\n",b-5);
            else printf("1 2 %d\n",b-3);}
    }
    return 0;
}