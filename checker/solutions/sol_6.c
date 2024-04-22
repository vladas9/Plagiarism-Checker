#include<stdio.h>

int main(){

    int broj_primjera;
    scanf("%d", &broj_primjera);
    int i, n;

    for(i = 1; i <= broj_primjera; i++){
        scanf("%d", &n);
        if(n < 7 || n == 9){
            printf("NO \n");
        }
        else{
            printf("YES \n");
            if(n % 3 != 0){
                printf("1 2 %d \n", n - 3);
            }
            else{
                printf("1 4 %d \n", n - 5);
            }
        }
    }

    return 0;
}