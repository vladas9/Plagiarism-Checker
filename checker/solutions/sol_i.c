#include <stdio.h>
int main()
{
    int m,n,x=1,y=2,z=4;
    scanf("%d",&m);

    for(int i=1; i<=m;i++)
    {
        scanf("%d",&n);
        if(n<7 || n==9)
            printf("NO\n");
        else if((n-3)%3!=0)
        {
            printf("YES\n");
            printf("%d %d %d\n",x,y,(n-3));
        }
        else if ((n-5)%3!=0)
        {
            printf("YES\n");
            printf("%d %d %d\n",x,z,(n-5));
        }
    }
    return 0;
}