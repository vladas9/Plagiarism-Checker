#include<stdio.h>
int main()
{
    long long int i,t,n;
    scanf("%lld",&t);
    for(i=0;i<t;i++)
    {
        scanf("%lld",&n);
        if(n%3!=0 && n-3>=4)
        {printf("YES\n1 2 %d\n",n-3);}
        else if((n-3)%3==0 && n>=10)
        {printf("YES\n1 4 %d\n",n-5);}
        else
        {printf("NO\n");}
    }
}