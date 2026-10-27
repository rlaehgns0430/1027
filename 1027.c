#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_COUNT 10000
#define TRIALS 100

typedef struct {
    int key1;
    int key2;
    int original_index;
} Data;

void generate_random_data(Data arr[], int n);
void copy_data(Data dest[], const Data src[], int n);
int compare_keys(const void* a, const void* b);

void selection_sort(Data arr[], int n, long long* comp, long long* move);
void bubble_sort(Data arr[], int n, long long* comp, long long* move);
void bubble_sort_early(Data arr[], int n, long long* comp, long long* move);
void insertion_sort(Data arr[], int n, int use_ge,
    long long* comp, long long* move);

int is_sorted(const Data arr[], int n);
int check_stability(const Data arr[], int n);

void make_sorted_data(Data dest[], const Data src[], int n);
void make_reverse_sorted_data(Data dest[], const Data src[], int n);
void make_partially_sorted_data(Data dest[], const Data src[], int n);

void run_all_sorts(const Data input[], Data work[], int n);

int main(void) {
    srand((unsigned int)time(NULL));

    Data* original_random = malloc(sizeof(Data) * DATA_COUNT);
    Data* work_array = malloc(sizeof(Data) * DATA_COUNT);
    Data* sorted_data = malloc(sizeof(Data) * DATA_COUNT);
    Data* rev_data = malloc(sizeof(Data) * DATA_COUNT);
    Data* part_data = malloc(sizeof(Data) * DATA_COUNT);

    if (!original_random || !work_array || !sorted_data ||
        !rev_data || !part_data) {
        printf("Memory allocation failed!\n");
        free(original_random);
        free(work_array);
        free(sorted_data);
        free(rev_data);
        free(part_data);
        return 1;
    }

    printf("Data Count : %d\n", DATA_COUNT);
    printf("Number of Trials : %d\n\n", TRIALS);
    printf("Random Data Experiments Running...\n");

    long long sel_total_comp = 0, sel_total_move = 0;
    long long bub_total_comp = 0, bub_total_move = 0;
    long long ebub_total_comp = 0, ebub_total_move = 0;
    long long ins_total_comp = 0, ins_total_move = 0;

    for (int t = 0; t < TRIALS; t++) {
        generate_random_data(original_random, DATA_COUNT);

        long long comp, move;

        copy_data(work_array, original_random, DATA_COUNT);
        selection_sort(work_array, DATA_COUNT, &comp, &move);
        sel_total_comp += comp;
        sel_total_move += move;

        if (!is_sorted(work_array, DATA_COUNT))
            printf("Trial %d: Selection Sort Error!\n", t + 1);

        copy_data(work_array, original_random, DATA_COUNT);
        bubble_sort(work_array, DATA_COUNT, &comp, &move);
        bub_total_comp += comp;
        bub_total_move += move;

        if (!is_sorted(work_array, DATA_COUNT))
            printf("Trial %d: Bubble Sort Error!\n", t + 1);

        copy_data(work_array, original_random, DATA_COUNT);
        bubble_sort_early(work_array, DATA_COUNT, &comp, &move);
        ebub_total_comp += comp;
        ebub_total_move += move;

        if (!is_sorted(work_array, DATA_COUNT))
            printf("Trial %d: Early Bubble Sort Error!\n", t + 1);

        copy_data(work_array, original_random, DATA_COUNT);
        insertion_sort(work_array, DATA_COUNT, 0, &comp, &move);
        ins_total_comp += comp;
        ins_total_move += move;

        if (!is_sorted(work_array, DATA_COUNT))
            printf("Trial %d: Insertion Sort Error!\n", t + 1);
    }

    printf("\n========== Random Data Results ==========\n\n");

    printf("Selection Sort\n");
    printf("Total Comparisons   : %lld\n", sel_total_comp);
    printf("Average Comparisons : %.2f\n",
        (double)sel_total_comp / TRIALS);
    printf("Average Movements   : %.2f\n\n",
        (double)sel_total_move / TRIALS);

    printf("Bubble Sort\n");
    printf("Total Comparisons   : %lld\n", bub_total_comp);
    printf("Average Comparisons : %.2f\n",
        (double)bub_total_comp / TRIALS);
    printf("Average Movements   : %.2f\n\n",
        (double)bub_total_move / TRIALS);

    printf("Bubble Sort (Early Termination)\n");
    printf("Total Comparisons   : %lld\n", ebub_total_comp);
    printf("Average Comparisons : %.2f\n",
        (double)ebub_total_comp / TRIALS);
    printf("Average Movements   : %.2f\n\n",
        (double)ebub_total_move / TRIALS);

    printf("Insertion Sort\n");
    printf("Total Comparisons   : %lld\n", ins_total_comp);
    printf("Average Comparisons : %.2f\n",
        (double)ins_total_comp / TRIALS);
    printf("Average Movements   : %.2f\n\n",
        (double)ins_total_move / TRIALS);

    generate_random_data(original_random, DATA_COUNT);
    make_sorted_data(sorted_data, original_random, DATA_COUNT);
    make_reverse_sorted_data(rev_data, sorted_data, DATA_COUNT);
    make_partially_sorted_data(part_data, sorted_data, DATA_COUNT);

    printf("\n========== Sorted Data ==========\n");
    run_all_sorts(sorted_data, work_array, DATA_COUNT);

    printf("\n========== Reverse-Sorted Data ==========\n");
    run_all_sorts(rev_data, work_array, DATA_COUNT);

    printf("\n========== Partially Sorted Data ==========\n");
    run_all_sorts(part_data, work_array, DATA_COUNT);

    Data stab_test[4] = {
        {527, 100, 0},
        {527, 200, 1},
        {125, 300, 2},
        {892, 400, 3}
    };

    Data stab_work[4];
    long long comp, move;

    printf("\n========== Stability Verification ==========\n\n");

    copy_data(stab_work, stab_test, 4);
    selection_sort(stab_work, 4, &comp, &move);
    printf("Selection Sort       : %s\n",
        check_stability(stab_work, 4) ? "Stable" : "Unstable");

    copy_data(stab_work, stab_test, 4);
    bubble_sort(stab_work, 4, &comp, &move);
    printf("Bubble Sort          : %s\n",
        check_stability(stab_work, 4) ? "Stable" : "Unstable");

    copy_data(stab_work, stab_test, 4);
    bubble_sort_early(stab_work, 4, &comp, &move);
    printf("Early Bubble Sort    : %s\n",
        check_stability(stab_work, 4) ? "Stable" : "Unstable");

    copy_data(stab_work, stab_test, 4);
    insertion_sort(stab_work, 4, 0, &comp, &move);
    printf("Insertion Sort (>)   : %s\n",
        check_stability(stab_work, 4) ? "Stable" : "Unstable");

    copy_data(stab_work, stab_test, 4);
    insertion_sort(stab_work, 4, 1, &comp, &move);
    printf("Insertion Sort (>=)  : %s\n",
        check_stability(stab_work, 4) ? "Stable" : "Unstable");

    free(original_random);
    free(work_array);
    free(sorted_data);
    free(rev_data);
    free(part_data);

    printf("\nAll experiments completed.\n");

    return 0;
}

void run_all_sorts(const Data input[], Data work[], int n) {
    long long comp, move;

    copy_data(work, input, n);
    selection_sort(work, n, &comp, &move);
    printf("\nSelection Sort\nComparisons : %lld\nMovements   : %lld\n",
        comp, move);
    if (!is_sorted(work, n))
        printf("Sorting Error!\n");

    copy_data(work, input, n);
    bubble_sort(work, n, &comp, &move);
    printf("\nBubble Sort\nComparisons : %lld\nMovements   : %lld\n",
        comp, move);
    if (!is_sorted(work, n))
        printf("Sorting Error!\n");

    copy_data(work, input, n);
    bubble_sort_early(work, n, &comp, &move);
    printf("\nBubble Sort (Early Termination)\nComparisons : %lld\nMovements   : %lld\n",
        comp, move);
    if (!is_sorted(work, n))
        printf("Sorting Error!\n");

    copy_data(work, input, n);
    insertion_sort(work, n, 0, &comp, &move);
    printf("\nInsertion Sort\nComparisons : %lld\nMovements   : %lld\n",
        comp, move);
    if (!is_sorted(work, n))
        printf("Sorting Error!\n");

    printf("------------------------------------------\n");
}

void generate_random_data(Data arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i].key1 = rand() % 1000001;
        arr[i].key2 = rand() % 1000001;
        arr[i].original_index = i;
    }
}

void copy_data(Data dest[], const Data src[], int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

int compare_keys(const void* a, const void* b) {
    const Data* d1 = (const Data*)a;
    const Data* d2 = (const Data*)b;

    if (d1->key1 < d2->key1) return -1;
    if (d1->key1 > d2->key1) return 1;
    return 0;
}

int is_sorted(const Data arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i].key1 > arr[i + 1].key1)
            return 0;
    }
    return 1;
}

int check_stability(const Data arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i].key1 == arr[j].key1 &&
                arr[i].original_index > arr[j].original_index)
                return 0;
        }
    }
    return 1;
}

void make_sorted_data(Data dest[], const Data src[], int n) {
    copy_data(dest, src, n);
    qsort(dest, n, sizeof(Data), compare_keys);
}

void make_reverse_sorted_data(Data dest[], const Data src[], int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[n - 1 - i];
}

void make_partially_sorted_data(Data dest[], const Data src[], int n) {
    copy_data(dest, src, n);

    int swap_count = n / 20;

    for (int i = 0; i < swap_count; i++) {
        int a = rand() % n;
        int b = rand() % n;

        Data temp = dest[a];
        dest[a] = dest[b];
        dest[b] = temp;
    }
}

void selection_sort(Data arr[], int n,
    long long* comp, long long* move) {
    *comp = 0;
    *move = 0;

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < n; j++) {
            (*comp)++;
            if (arr[j].key1 < arr[min_idx].key1)
                min_idx = j;
        }

        if (min_idx != i) {
            Data temp = arr[i];
            (*move)++;
            arr[i] = arr[min_idx];
            (*move)++;
            arr[min_idx] = temp;
            (*move)++;
        }
    }
}

void bubble_sort(Data arr[], int n,
    long long* comp, long long* move) {
    *comp = 0;
    *move = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comp)++;

            if (arr[j].key1 > arr[j + 1].key1) {
                Data temp = arr[j];
                (*move)++;
                arr[j] = arr[j + 1];
                (*move)++;
                arr[j + 1] = temp;
                (*move)++;
            }
        }
    }
}

void bubble_sort_early(Data arr[], int n,
    long long* comp, long long* move) {
    *comp = 0;
    *move = 0;

    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;

        for (int j = 0; j < n - 1 - i; j++) {
            (*comp)++;

            if (arr[j].key1 > arr[j + 1].key1) {
                Data temp = arr[j];
                (*move)++;
                arr[j] = arr[j + 1];
                (*move)++;
                arr[j + 1] = temp;
                (*move)++;
                swapped = 1;
            }
        }

        if (!swapped)
            break;
    }
}

void insertion_sort(Data arr[], int n, int use_ge,
    long long* comp, long long* move) {
    *comp = 0;
    *move = 0;

    for (int i = 1; i < n; i++) {
        Data key_data = arr[i];
        (*move)++;

        int j = i - 1;

        while (j >= 0) {
            (*comp)++;

            int cond = use_ge
                ? (arr[j].key1 >= key_data.key1)
                : (arr[j].key1 > key_data.key1);

            if (cond) {
                arr[j + 1] = arr[j];
                (*move)++;
                j--;
            }
            else {
                break;
            }
        }

        arr[j + 1] = key_data;
        (*move)++;
    }
}