#include<alloca.h>

#define SWAP(_type, a, b) do{ \
        _type _tmp = a; \
        a = b; \
        b = _tmp; \
    } while(0)

void quicksort(int *data, size_t count)
{
    size_t *stack = alloca(sizeof(size_t)*(count*2)), stacktop = 1;
    stack[0] = 0, stack[1] = count;
    while(stacktop--){
        size_t base = stack[stacktop*2], i = 1, j = stack[stacktop*2+1];
        if(j-- < 1) continue;
        while(i <= j){
            while(i != count && data[base+i] < data[base]) i++;
            while(j && data[base] <= data[base+j]) j--;
            if(i < j) SWAP(int, data[base+i], data[base+j]);
        }
        SWAP(int, data[base], data[base+j]);
        stack[stacktop*2] = base+i;
        stack[stacktop*2+1] -= i;
        stack[stacktop*2+2] = base;
        stack[stacktop*2+3] = j;
        stacktop += 2;
    }
}

int main(void){
    int array[100000] = {
#include "array"
    };
    quicksort(array, 100000);
    return 0;
}
