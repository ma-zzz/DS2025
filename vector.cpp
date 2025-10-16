#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

typedef int Rank;               // 秩
#define DEFAULT_CAPACITY 3      // 默认初始容量

template <typename T>
class Vector {
private:
    Rank _size;                 // 当前元素个数
    int _capacity;              // 当前容量
    T* _elem;                   // 数据区首地址

    /* 内部工具函数 */
    void copyFrom(T const* A, Rank lo, Rank hi) {   // 复制数组区间 A[lo, hi)
        _size = 0;
        _capacity = 2 * (hi - lo);
        _elem = new T[_capacity];
        while (lo < hi) _elem[_size++] = A[lo++];
    }
    void expand() {             // 扩容
        if (_size < _capacity) return;
        _capacity = std::max(_capacity, DEFAULT_CAPACITY) << 1;
        T* old = _elem;
        _elem = new T[_capacity];
        for (Rank i = 0; i < _size; ++i) _elem[i] = old[i];
        delete [] old;
    }
    void shrink() {             // 装填因子过小时缩容
        if (_capacity < DEFAULT_CAPACITY << 1) return;
        if (_size << 2 > _capacity) return;         // 25% 以上不缩
        _capacity >>= 1;
        T* old = _elem;
        _elem = new T[_capacity];
        for (Rank i = 0; i < _size; ++i) _elem[i] = old[i];
        delete [] old;
    }

    /* 排序相关内部实现 */
    bool bubble(Rank lo, Rank hi) {                 // 一趟起泡扫描
        bool sorted = true;
        while (++lo < hi)
            if (_elem[lo - 1] > _elem[lo]) {
                std::swap(_elem[lo - 1], _elem[lo]);
                sorted = false;
            }
        return sorted;
    }
    void bubbleSort(Rank lo, Rank hi) {             // 起泡排序
        while (!bubble(lo, hi--));
    }
    Rank max(Rank lo, Rank hi) {                    // 选取最大元素
        Rank mx = lo;
        while (++lo < hi)
            if (_elem[lo] > _elem[mx]) mx = lo;
        return mx;
    }
    void selectionSort(Rank lo, Rank hi) {          // 选择排序
        while (lo < --hi) {
            Rank mx = max(lo, hi + 1);
            std::swap(_elem[mx], _elem[hi]);
        }
    }
    void merge(Rank lo, Rank mi, Rank hi) {         // 归并
        Rank lb = mi - lo, lc = hi - mi;
        T* B = new T[lb];
        for (Rank i = 0; i < lb; B[i] = _elem[lo + i], ++i);
        Rank i = 0, j = 0, k = lo;
        while (j < lc) {
            if (i < lb && B[i] <= _elem[mi + j]) _elem[k++] = B[i++];
            else                                  _elem[k++] = _elem[mi + j++];
        }
        while (i < lb) _elem[k++] = B[i++];
        delete [] B;
    }
    void mergeSort(Rank lo, Rank hi) {              // 归并排序
        if (hi - lo < 2) return;
        Rank mi = (lo + hi) >> 1;
        mergeSort(lo, mi);
        mergeSort(mi, hi);
        merge(lo, mi, hi);
    }
    Rank partition(Rank lo, Rank hi) {              // 快速排序轴点构造
        T pivot = _elem[lo];
        while (lo < hi) {
            while (lo < hi && pivot <= _elem[hi]) --hi;
            _elem[lo] = _elem[hi];
            while (lo < hi && _elem[lo] <= pivot) ++lo;
            _elem[hi] = _elem[lo];
        }
        _elem[lo] = pivot;
        return lo;
    }
    void quickSort(Rank lo, Rank hi) {              // 快速排序
        if (hi - lo < 2) return;
        Rank mi = partition(lo, hi - 1);
        quickSort(lo, mi);
        quickSort(mi + 1, hi);
    }
    void heapSort(Rank lo, Rank hi) {               // 堆排序（用 STL 简化）
        std::make_heap(_elem + lo, _elem + hi);
        std::sort_heap(_elem + lo, _elem + hi);
    }

public:
    /* 构造与析构 */
    Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = T())
        : _capacity(c), _size(0) {
        _elem = new T[_capacity];
        while (_size < s) _elem[_size++] = v;
    }
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
    ~Vector() { delete [] _elem; }

    /* 只读接口 */
    Rank size() const { return _size; }
    bool empty() const { return !_size; }
    int disordered() const {                        // 返回逆序对数，0 表示有序
        int cnt = 0;
        for (Rank i = 1; i < _size; ++i)
            if (_elem[i - 1] > _elem[i]) ++cnt;
        return cnt;
    }
    Rank find(T const& e) const { return find(e, 0, _size); }
    Rank find(T const& e, Rank lo, Rank hi) const { // 无序顺序查找
        while (lo < hi-- && e != _elem[hi]);
        return hi;
    }
    Rank search(T const& e) const { return search(e, 0, _size); }
    Rank search(T const& e, Rank lo, Rank hi) const {// 有序二分查找
        while (lo < hi) {
            Rank mi = (lo + hi) >> 1;
            (e < _elem[mi]) ? hi = mi : lo = mi + 1;
        }
        return --lo;
    }

    /* 可写接口 */
    T& operator[](Rank r) const { return _elem[r]; }
    Vector<T>& operator=(Vector<T> const& V) {
        if (this != &V) {
            delete [] _elem;
            copyFrom(V._elem, 0, V._size);
        }
        return *this;
    }
    T remove(Rank r) {                              // 删除秩为 r 的元素
        T e = _elem[r];
        remove(r, r + 1);
        return e;
    }
    int remove(Rank lo, Rank hi) {                  // 删除区间 [lo, hi)
        if (lo == hi) return 0;
        while (hi < _size) _elem[lo++] = _elem[hi++];
        _size = lo;
        shrink();
        return hi - lo;
    }
    Rank insert(Rank r, T const& e) {               // 插入元素
        expand();
        for (Rank i = _size; i > r; --i) _elem[i] = _elem[i - 1];
        _elem[r] = e; ++_size;
        return r;
    }
    Rank insert(T const& e) { return insert(_size, e); }

    void sort(Rank lo, Rank hi) { mergeSort(lo, hi); }  // 可随意切换排序算法
    void sort() { sort(0, _size); }
    void unsort(Rank lo, Rank hi) {                 // 置乱
        T* A = _elem + lo;
        for (Rank i = hi - lo; i > 0; --i)
            std::swap(A[i - 1], A[std::rand() % i]);
    }
    void unsort() { unsort(0, _size); }

    int deduplicate() {                             // 无序去重
        int old = _size;
        Rank i = 1;
        while (i < _size)
            (find(_elem[i], 0, i) < 0) ? ++i : remove(i);
        return old - _size;
    }
    int uniquify() {                                // 有序去重
        Rank i = 0, j = 0;
        while (++j < _size)
            if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
        _size = i + 1;
        shrink();
        return j - i;
    }

    /* 遍历 */
    void traverse(void (*visit)(T&)) {
        for (Rank i = 0; i < _size; ++i) visit(_elem[i]);
    }

    /* 调试用打印 */
    void print() const {
        for (Rank i = 0; i < _size; ++i) std::cout << _elem[i] << " ";
        std::cout << "\n";
    }
};
