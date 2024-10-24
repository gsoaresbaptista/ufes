#include <stdlib.h>
#include <stdio.h>

void selection_sort(unsigned int *vector, unsigned int length) {
    for (unsigned int i = 0; i < length - 1; i++) {
        unsigned int min_index = i;

        // search for the next minimum value 
        for (unsigned int j = i; j < length; j++) {
            if (vector[j] < vector[min_index]) {
                min_index = j;
            }
        }

        // exchange the minimum value for the current position
        unsigned int tmp = vector[i];
        vector[i] = vector[min_index];
        vector[min_index] = tmp;
    }
}

void bubble_sort(unsigned int *vector, unsigned int length) {
    for (int i = 0; i < length - 1; i++) {
        // Float the following numbers
        for (int j = i + 1; j < length; j++) {
            if (vector[j] < vector[i]) {
                int tmp = vector[j];
                vector[j] = vector[i];
                vector[i] = tmp;
            }
        }
    }
}

void insertion_sort(unsigned int *vector, unsigned int length) {
    int j;

    for (int i = 1; i < length; i++) {
        int min_index = i;
        int key = vector[i];

        // Insert a number
        for (j = i - 1; j >= 0; j--) {
            if (key < vector[j]) {
                vector[j + 1] = vector[j];
            } else {
                break;
            }
        }

        vector[j + 1] = key;
    }
}

static void _merge_sort_merge(unsigned int *vector, unsigned int start, unsigned int half, unsigned int end) {
    unsigned int top_left = 0, top_right = 0;
    unsigned int stack_left[half-start], stack_right[end-half];

    // Copy vectors
    for (int i = 0; i < half-start; i++) {
        stack_left[i] = vector[start + i];
    }

    for (int i = 0; i < end-half; i++) {
        stack_right[i] = vector[half + i];
    }

    // Merge vectors
    for (int i = start; i < end; i++) {
        if (top_left >= half - start) {
            vector[i] = stack_right[top_right];
            top_right++;
        } else if (top_right >= end - half) {
            vector[i] = stack_left[top_left];
            top_left++;
        } else if (stack_left[top_left] < stack_right[top_right]) {
            vector[i] = stack_left[top_left];
            top_left++;
        } else {
            vector[i] = stack_right[top_right];
            top_right++;
        }
    }
}

static void _merge_sort_step(unsigned int *vector, unsigned int start, unsigned int end) {
    if (end - start > 1) {
        unsigned int half = (end + start) / 2;
        _merge_sort_step(vector, start, half);
        _merge_sort_step(vector, half, end);
        _merge_sort_merge(vector, start, half, end);
    }
}

void merge_sort(unsigned int *vector, unsigned int length) {
    _merge_sort_step(vector, 0, length);
}

static void _quick_sort_step(unsigned int *vector, unsigned int start, unsigned int end) {
    if (start == end + 1) {
        return;
    }

    unsigned int p_smaller = start, p_bigger = start, tmp;

    while (p_bigger < end) {
        if (vector[p_bigger] < vector[end]) {
            tmp = vector[p_bigger];
            vector[p_bigger] = vector[p_smaller];
            vector[p_smaller] = tmp;
            p_smaller++;
        }
        p_bigger++;
    }

    tmp = vector[p_smaller];
    vector[p_smaller] = vector[end];
    vector[end] = tmp;

    _quick_sort_step(vector, start, p_smaller - 1);
    _quick_sort_step(vector, p_smaller + 1, end);
}

void quick_sort(unsigned int *vector, unsigned int length) {
    _quick_sort_step(vector, 0, length - 1);
}

