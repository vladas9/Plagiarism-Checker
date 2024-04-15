#include<stdio.h>
int main()
{
    int t=0,i=0,n=0;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        if(n<=6||n==9)
        {
            printf("NO\n");
        }
        else
        {
            printf("YES\n%d %d %d\n",1,2+2*(n%3==0),n-3-2*(n%3==0));
        }
    }
    return 0;
}