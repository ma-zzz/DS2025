#include <iostream>
#include <vector>
#include <stack>
#include <algorithm> // 包含 algorithm 头文件
#include <sstream>   // 包含 stringstream 头文件

using namespace std;

// 计算矩形最大面积的函数
int largestRectangleArea(vector<int>& heights) {
    stack<int> s; // 单调递增栈，存储柱子的索引
    int max_area = 0;
    heights.push_back(0); // 添加一个高度为0的柱子，方便处理栈中剩余的柱子

    for (int i = 0; i < heights.size(); ++i) {
        while (!s.empty() && heights[s.top()] > heights[i]) {
            int height = heights[s.top()];
            s.pop();
            int width = s.empty() ? i : i - s.top() - 1;
            max_area = max(max_area, height * width);
        }
        s.push(i);
    }

    return max_area;
}

int main() {
    string input;
    cout << "请输入柱子的高度: ";
    getline(cin, input);

    // 去掉输入字符串的首尾空格和括号
    input.erase(0, input.find_first_not_of("[] \t\n"));
    input.erase(input.find_last_not_of("[] \t\n") + 1);

    vector<int> heights;
    stringstream ss(input);
    string item;

    while (getline(ss, item, ',')) {
        heights.push_back(stoi(item));
    }

    cout << "最大矩形面积: " << largestRectangleArea(heights) << endl;

    return 0;
}
