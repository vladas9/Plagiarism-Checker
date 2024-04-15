#include <stdio.h>
int main(void) {
    int t;
    scanf("%d",&t);
    while(t--){
        long n;
        scanf("%ld",&n);
        if(n>=1&&n<=6||n==9){
            printf("NO\n");
        }
        else if(n%3==0){
            printf("YES\n1 4 %d\n",n-5);
        }
        else{
            printf("YES\n1 2 %d\n",n-3);
        }
    }
    return 0;
}