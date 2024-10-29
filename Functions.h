#ifndef ALL_SORTS_H
#define ALL_SORTS_H

#include <vector>
#include <algorithm> 
#include <iostream>
#include <cstdlib> 

using namespace std;

// Bubble Sort
int bubble(std::vector<int>& vec) {
    int n = vec.size();
    int comparisons = 0;
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            comparisons++; // Count comparison
            if (vec[j] > vec[j+1]) {
                std::swap(vec[j], vec[j+1]);
            }
        }
    }
    return comparisons;
}

// Merge Sort helper function with comparison count
int merge(std::vector<int>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int comparisons = 0;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        comparisons++; // Count comparison
        if (L[i] <= R[j]) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
    return comparisons;
}

// Merge Sort
int merge_sort(std::vector<int>& vec, int left, int right) {
    int comparisons = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;

        comparisons += merge_sort(vec, left, mid);
        comparisons += merge_sort(vec, mid + 1, right);

        comparisons += merge(vec, left, mid, right);
    }
    return comparisons;
}

// Quick Sort helper function
int partition(std::vector<int>& arr, int low, int high, int& comparisons) {
    int pivot = arr[low];
    int left = low + 1;
    int right = high;

    while (true) {
        while (left <= right && arr[left] <= pivot) {
            left++;
            comparisons++; // Count comparison
        }
        while (left <= right && arr[right] >= pivot) {
            right--;
            comparisons++; // Count comparison
        }
        if (left > right) break;
        std::swap(arr[left], arr[right]);
    }
    std::swap(arr[low], arr[right]);
    return right;
}

// QuickSort function with tail call optimization and comparison count
int quick(std::vector<int>& arr, int low, int high, int pivotType) {
    int comparisons = 0;
    while (low < high) {
        int pi;
        if (pivotType == 0) {
            pi = partition(arr, low, high, comparisons);
        } else if (pivotType == 1) {
            int mid = low + (high - low) / 2;
            if (arr[mid] < arr[low]) std::swap(arr[mid], arr[low]);
            if (arr[high] < arr[low]) std::swap(arr[high], arr[low]);
            if (arr[mid] > arr[high]) std::swap(arr[mid], arr[high]);
            std::swap(arr[mid], arr[low]); // Place median as pivot
            pi = partition(arr, low, high, comparisons);
        } else {
            int pivotIndex = low + rand() % (high - low + 1);
            std::swap(arr[low], arr[pivotIndex]);
            pi = partition(arr, low, high, comparisons);
        }

        // Tail call optimization: sort the smaller partition iteratively
        if (pi - low < high - pi) {
            comparisons += quick(arr, low, pi - 1, pivotType);
            low = pi + 1;
        } else {
            comparisons += quick(arr, pi + 1, high, pivotType);
            high = pi - 1;
        }
    }
    return comparisons;
}

// Heap Sort helper function
int heapify(std::vector<int>& vec, int n, int i, int& comparisons) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        comparisons++; // Count comparison
        if (vec[left] > vec[largest])
            largest = left;
    }

    if (right < n) {
        comparisons++; // Count comparison
        if (vec[right] > vec[largest])
            largest = right;
    }

    if (largest != i) {
        std::swap(vec[i], vec[largest]);
        heapify(vec, n, largest, comparisons);
    }
    return comparisons;
}

// Heap Sort
int heap(std::vector<int>& vec) {
    int n = vec.size();
    int comparisons = 0;

    for (int i = n / 2 - 1; i >= 0; i--)
        comparisons += heapify(vec, n, i, comparisons);

    for (int i = n - 1; i >= 0; i--) {
        std::swap(vec[0], vec[i]);
        comparisons += heapify(vec, i, 0, comparisons);
    }
    return comparisons;
}

// Insertion Sort
int insertion(std::vector<int>& vec) {
    int n = vec.size();
    int comparisons = 0;
    for (int i = 1; i < n; ++i) {
        int key = vec[i];
        int j = i - 1;

        while (j >= 0 && vec[j] > key) {
            vec[j + 1] = vec[j];
            j = j - 1;
            comparisons++; // Count comparison
        }
        comparisons++; // Count final comparison when the loop exits
        vec[j + 1] = key;
    }
    return comparisons;
}

// Radix Sort helper function for counting sort
void countingSort(std::vector<int>& arr, int exp, int& comparisons) {
    int n = arr.size();
    std::vector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) {
        comparisons++; // Count comparison during output copying
        arr[i] = output[i];
    }
}

// Radix Sort
int radix(std::vector<int>& arr) {
    int maxNum = *max_element(arr.begin(), arr.end());
    int comparisons = 0;

    for (int exp = 1; maxNum / exp > 0; exp *= 10)
        countingSort(arr, exp, comparisons);

    return comparisons;
}

#endif
