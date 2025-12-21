#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <functional> // 包含 std::function 的头文件

// 边界框结构
struct BoundingBox {
    int x1, y1, x2, y2;
    float confidence;
};

// 一、排序算法实现

// 快速排序
void quickSort(std::vector<float>& arr, int low, int high) {
    if (low < high) {
        float pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 归并排序
void merge(std::vector<float>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<float> L(n1);
    std::vector<float> R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(std::vector<float>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// 冒泡排序
void bubbleSort(std::vector<float>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 插入排序
void insertionSort(std::vector<float>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        float key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 二、非极大值抑制（NMS）算法实现

float computeIOU(const BoundingBox& boxA, const BoundingBox& boxB) {
    float interX1 = std::max(boxA.x1, boxB.x1);
    float interY1 = std::max(boxA.y1, boxB.y1);
    float interX2 = std::min(boxA.x2, boxB.x2);
    float interY2 = std::min(boxA.y2, boxB.y2);
    float interArea = std::max(0.0f, interX2 - interX1) * std::max(0.0f, interY2 - interY1);
    float boxAArea = (boxA.x2 - boxA.x1) * (boxA.y2 - boxA.y1);
    float boxBArea = (boxB.x2 - boxB.x1) * (boxB.y2 - boxB.y1);
    return interArea / (boxAArea + boxBArea - interArea);
}

std::vector<BoundingBox> nms(std::vector<BoundingBox>& bboxes, float threshold) {
    std::vector<BoundingBox> pick;
    std::sort(bboxes.begin(), bboxes.end(), [](const BoundingBox& a, const BoundingBox& b) {
        return a.confidence > b.confidence;
        });
    while (!bboxes.empty()) {
        BoundingBox last = bboxes.back();
        pick.push_back(last);
        bboxes.pop_back();
        std::vector<BoundingBox> temp;
        for (size_t i = 0; i < bboxes.size(); i++) {
            if (computeIOU(last, bboxes[i]) <= threshold) {
                temp.push_back(bboxes[i]);
            }
        }
        bboxes = temp;
    }
    return pick;
}

// 三、数据生成

std::vector<BoundingBox> generateRandomBboxes(int numBboxes) {
    std::vector<BoundingBox> bboxes;
    for (int i = 0; i < numBboxes; i++) {
        BoundingBox bbox;
        bbox.x1 = rand() % 1000;
        bbox.y1 = rand() % 1000;
        bbox.x2 = bbox.x1 + (rand() % 151 + 50);
        bbox.y2 = bbox.y1 + (rand() % 151 + 50);
        bbox.confidence = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.5 + 0.5;
        bboxes.push_back(bbox);
    }
    return bboxes;
}

std::vector<BoundingBox> generateClusteredBboxes(int numBboxes) {
    std::vector<BoundingBox> bboxes;
    int centerX = rand() % 600 + 200;
    int centerY = rand() % 600 + 200;
    for (int i = 0; i < numBboxes; i++) {
        BoundingBox bbox;
        bbox.x1 = centerX + (rand() % 201 - 100);
        bbox.y1 = centerY + (rand() % 201 - 100);
        bbox.x2 = bbox.x1 + (rand() % 151 + 50);
        bbox.y2 = bbox.y1 + (rand() % 151 + 50);
        bbox.confidence = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.5 + 0.5;
        bboxes.push_back(bbox);
    }
    return bboxes;
}

// 四、性能测试

void testSortingAlgorithms() {
    std::vector<std::string> sortingAlgorithms = { "Quick Sort", "Merge Sort", "Bubble Sort", "Insertion Sort" };
    std::vector<int> dataScales = { 100, 1000, 5000, 10000 };
    std::vector<std::string> distributions = { "Random", "Clustered" };
    std::vector<std::function<std::vector<BoundingBox>(int)>> genFuncs = { generateRandomBboxes, generateClusteredBboxes };

    for (const auto& algo : sortingAlgorithms) {
        for (size_t distIdx = 0; distIdx < distributions.size(); distIdx++) {
            for (int scale : dataScales) {
                auto bboxes = genFuncs[distIdx](scale);
                std::vector<float> confidences;
                for (const auto& bbox : bboxes) {
                    confidences.push_back(bbox.confidence);
                }

                auto start = std::chrono::high_resolution_clock::now();
                if (algo == "Quick Sort") {
                    quickSort(confidences, 0, confidences.size() - 1);
                }
                else if (algo == "Merge Sort") {
                    mergeSort(confidences, 0, confidences.size() - 1);
                }
                else if (algo == "Bubble Sort") {
                    bubbleSort(confidences);
                }
                else if (algo == "Insertion Sort") {
                    insertionSort(confidences);
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;

                std::cout << algo << " on " << distributions[distIdx] << " data with " << scale << " bboxes: " << duration.count() << " seconds" << std::endl;
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    testSortingAlgorithms();
    return 0;
}
