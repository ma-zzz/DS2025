#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <iomanip> // 用于设置输出精度

// 定义复数类
class Complex {
public:
    double real; // 实部
    double imag; // 虚部

    // 构造函数
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // 计算复数的模
    double modulus() const {
        return std::sqrt(real * real + imag * imag);
    }

    // 重载等号运算符，用于比较两个复数是否相等
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    // 重载输出运算符，方便打印复数
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }
};

// 随机生成一个无序的复数向量（有重复项）
std::vector<Complex> generateRandomComplexVector(int size) {
    std::vector<Complex> vec;
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // 设置随机种子
    for (int i = 0; i < size; ++i) {
        double real = std::rand() % 10; // 实部随机生成0到9
        double imag = std::rand() % 10; // 虚部随机生成0到9
        vec.push_back(Complex(real, imag));
    }
    return vec;
}

// 置乱向量
void shuffleVector(std::vector<Complex>& vec) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // 设置随机种子
    std::random_shuffle(vec.begin(), vec.end()); // 随机置乱
}

// 查找复数，实部和虚部均相同才视为相等
bool findElement(const std::vector<Complex>& vec, const Complex& target) {
    for (const auto& elem : vec) {
        if (elem == target) {
            return true;
        }
    }
    return false;
}

// 向向量中插入一个复数
void insertElement(std::vector<Complex>& vec, const Complex& elem) {
    vec.push_back(elem);
}

// 从向量中删除一个复数
void deleteElement(std::vector<Complex>& vec, const Complex& elem) {
    vec.erase(std::remove(vec.begin(), vec.end(), elem), vec.end());
}

// 去除向量中的重复元素
void uniqueVector(std::vector<Complex>& vec) {
    // 先按复数的模排序，模相同的情况下按实部排序
    std::sort(vec.begin(), vec.end(), [](const Complex& a, const Complex& b) {
        return a.modulus() < b.modulus() || (a.modulus() == b.modulus() && a.real < b.real);
    });
    // 使用unique函数去除重复元素
    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
}

// 冒泡排序
void bubbleSort(std::vector<Complex>& vec) {
    bool swapped;
    do {
        swapped = false;
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1].modulus() > vec[i].modulus() || 
                (vec[i - 1].modulus() == vec[i].modulus() && vec[i - 1].real > vec[i].real)) {
                std::swap(vec[i - 1], vec[i]);
                swapped = true;
            }
        }
    } while (swapped);
}

// 归并排序
void mergeSort(std::vector<Complex>& vec) {
    // 使用标准库的sort函数进行归并排序
    std::sort(vec.begin(), vec.end(), [](const Complex& a, const Complex& b) {
        return a.modulus() < b.modulus() || (a.modulus() == b.modulus() && a.real < b.real);
    });
}

// 区间查找算法
std::vector<Complex> rangeSearch(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& elem : vec) {
        if (elem.modulus() >= m1 && elem.modulus() < m2) {
            result.push_back(elem);
        }
    }
    return result;
}

int main() {
    // 随机生成一个包含20个复数的无序向量
    std::vector<Complex> vec = generateRandomComplexVector(20);

    // 在随机生成的向量后面追加一个相同的向量
    std::vector<Complex> vecCopy = vec;
    vec.insert(vec.end(), vecCopy.begin(), vecCopy.end());

    std::cout << "随机生成的向量: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 测试置乱
    shuffleVector(vec);
    std::cout << "置乱后的向量: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 测试查找
    Complex target(3, 4);
    if (findElement(vec, target)) {
        std::cout << "找到元素: " << target << std::endl;
    } else {
        std::cout << "未找到元素。" << std::endl;
    }

    // 测试插入
    Complex newElem(5, 5);
    insertElement(vec, newElem);
    std::cout << "插入元素后的向量: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 测试删除
    deleteElement(vec, target);
    std::cout << "删除元素后的向量: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 测试唯一化
    uniqueVector(vec);
    std::cout << "去除重复元素后的向量: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 测试排序效率
    vecCopy = vec;

    clock_t start = clock(); // 记录开始时间
    bubbleSort(vecCopy); // 冒泡排序
    clock_t end = clock(); // 记录结束时间
    std::cout << "冒泡排序时间: " << (end - start) / CLOCKS_PER_SEC << " 秒" << std::endl;

    vecCopy = vec;
    start = clock(); // 记录开始时间
    mergeSort(vecCopy); // 归并排序
    end = clock(); // 记录结束时间
    std::cout << "归并排序时间: " << (end - start) / CLOCKS_PER_SEC << " 秒" << std::endl;

    // 测试区间查找
    double m1 = 2.0, m2 = 5.0;
    std::vector<Complex> result = rangeSearch(vecCopy, m1, m2);
    std::cout << "模在区间 [" << m1 << ", " << m2 << ") 内的元素: ";
    for (const auto& elem : result) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
