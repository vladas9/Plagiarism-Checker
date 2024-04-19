#include<stdio.h>
int main(){
    int t;
    scanf("%d",&t);
    for(int i=0;i<t;i++){
        int n;
        scanf("%d",&n);
        if(n%3==0&&n>=12) printf("YES\n1 4 %d\n",n-5);
        else if(n%3==1&&n>=7) printf("YES\n1 2 %d\n",n-3);
        else if(n%3==2&&n>=8) printf("YES\n1 2 %d\n",n-3);
        else printf("NO\n");
    }
    return 0;
    return 0;
}