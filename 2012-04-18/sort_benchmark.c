#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef void(*SortFunc)(int [], const int);

void
print_data(const int a[], const int count)
{
#ifdef DEBUG
	for(int i = 0; i < count; i++) printf("%3d ", a[i]);
	printf("\n");
#endif
}

void
swap_array(int a[], const int i, const int j)
{
	const int tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

void
bubble_sort(int data[], const int count)
{
	const int n = count - 1;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n - i; j++) {
			if (data[j] <= data[j + 1]) continue;
			swap_array(data, j, j + 1);
			print_data(data, count);
		}
	}
}

static void
quick_sort_(int data[], const int count, const int head, const int tail)
{
	int l = head;
	int r = tail;
	const int pivot = data[head];
	while(1) {
		while(data[l] < pivot) l++;
		while(data[r] > pivot) r--;
		if(l >= r) break;
		swap_array(data, l, r);
		print_data(data, count);
		l++;
		r--;
	}
	if(head < l - 1) quick_sort_(data, count, head, l - 1);
	if(tail > r + 1) quick_sort_(data, count, r + 1, tail);
}

void
quick_sort(int data[], const int count)
{
	quick_sort_(data, count, 0, count - 1);
}

double
benchmark(const unsigned long seed, const int count, SortFunc sortfunc)
{
	int sample[count];
	srand(seed);
	for(int i = 0; i < count; i++) sample[i] = rand() % 1000;
	print_data(sample, count);
	const clock_t t = clock();
	(*sortfunc)(sample, count);
	return (double)(clock() - t) / CLOCKS_PER_SEC;
}

int
main(int argc, char *argv[])
{
	if(argc != 2) return 1;
	const unsigned long seed = (unsigned long)rand();
	const int count = atoi(argv[1]);
	puts("*** BUBBLE SORT ***");
	const double score_bubble = benchmark(seed, count, bubble_sort);
	puts("*** QUICK  SORT ***");
	const double score_quick  = benchmark(seed, count, quick_sort);
	printf("bubble: %f\n", score_bubble);
	printf("quick : %f\n", score_quick );
	return 0;
}

