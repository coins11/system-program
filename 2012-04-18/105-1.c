#include<alloca.h>
#include<stdio.h>
#include<string.h>

#define SWAP(_type, a, b) do{ \
        _type _tmp = a; \
        a = b; \
        b = _tmp; \
    } while(0)

int sorted(int *data, size_t count)
{
    size_t i = 0;
    for(i = 0; i+1 < count; i++) if(data[i+1] < data[i]) return 0;
    return 1;
}

void parray(int *data, int count)
{
    size_t i;
    for(i = 0; i != count; i++) printf("%d ", data[i]);
    printf("\n");
}

int bubblesort(int *data, size_t count)
{
    size_t i, j;
    int complexity = 0;
    for(i = 0; i+1 < count; i++){
        for(j = 0; j+1 < count-i; j++){
            complexity++;
            if(data[j] > data[j + 1]){
                parray(data, count);
                SWAP(int, data[j], data[j+1]);
            }
        }
    }
    return complexity;
}

int quicksort(int *data, size_t count)
{
    size_t *stack = alloca(sizeof(size_t)*(count*2)), stacktop = 1;
    int cmpl = 0;
    stack[0] = 0, stack[1] = count;
    while(stacktop--){
        size_t base = stack[stacktop*2], i = 1, j = stack[stacktop*2+1];
        if(j-- < 1) continue;
        while(i <= j){
            parray(data, count);
            while(i != count && (cmpl++, data[base+i] < data[base])) i++;
            while(j && (cmpl++, data[base] <= data[base+j])) j--;
            if(i < j) SWAP(int, data[base+i], data[base+j]);
        }
        SWAP(int, data[base], data[base+j]);
        stack[stacktop*2] = base+i;
        stack[stacktop*2+1] -= i;
        stack[stacktop*2+2] = base;
        stack[stacktop*2+3] = j;
        stacktop += 2;
    }
    return cmpl;
}

int main(void){
    int array[20] = {10, 5, 11, 7, 0, 13, 18, 2, 15, 8,
        16, 1, 17, 9, 12, 19, 3, 4, 6, 14}, buf[20], r;
    printf("bubblesort()\n");
    memcpy(buf, array, sizeof(int)*20);
    r = bubblesort(buf, 20);
    parray(buf, 20);
    printf("%d\n", r);
    printf("quicksort()\n");
    memcpy(buf, array, sizeof(int)*20);
    r = quicksort(buf, 20);
    parray(buf, 20);
    printf("%d\n", r);
    return 0;
}
