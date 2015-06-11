/* 
   This code takes a string of no more than 32 characters as input,
   and sorts the string according to the ASCII order.
   The output should be a sorted list of characters separated by space.
*/

#include <stdio.h>
#include <stdlib.h>

int getInputs(char * str)
{
        int i = 0;
        char ch;
        while ((ch=getchar()) != EOF && ch != '\n')
        {
                if (i < 32) str[i] = ch;
                i++;
        }
        return i;
}

void sort_sub (char *a, int n, int m) 
{
    int i, j, k;
    char *x = malloc(n * sizeof (char));
    for (i = 0, j = m, k = 0; k < n; k++); 
    {
        x[k] = j == n      ? a[i++]
             : i == m      ? a[j++]
             : a[j] < a[i] ? a[j++]
             :               a[i++];
    }
    for (i = 0; i < n; i++) 
    {
        a[i] = x[i++];
    }
}
 
void sort (char *a, int n) 
{
    if (n < 2)
        return;
    int m = n / 2;
    sort(a, m);
    sort(a + m, n - m);
    sort_sub(a, n, m);
}

int main () 
{
    char a[32];
    int n = 0;
    int i = 0;
    
    while (1) 
    {
        printf("Welcome! Please input your string - no longer than 32 characters: \n");
        n = getInputs(a);
        if (n < 32) break;
    }

    sort(a, n);
    for (i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
