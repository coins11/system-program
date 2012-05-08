#include<stddef.h>

#define SWAP(_type, a, b) do{ \
        _type _tmp = a; \
        a = b; \
        b = _tmp; \
    } while(0)

void bubblesort(int *data, size_t count)
{
    size_t i, j;
    for(i = 0; i+1 < count; i++)
        for(j = 0; j+1 < count-i; j++)
            if(data[j] > data[j + 1])
                SWAP(int, data[j], data[j+1]);
}

int main(void){
    int array[100000] = {
#include "array"
    };
    bubblesort(array, 100000);
    return 0;
}
