#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>

// 栈的实现
class Stack {
private:
    std::vector<int> items; // 使用vector来存储栈中的元素

public:
    // 判断栈是否为空
    bool is_empty() const {
        return items.empty();
    }

    // 向栈中压入一个元素
    void push(int item) {
        items.push_back(item);
    }

    // 从栈中弹出一个元素
    int pop() {
        if (is_empty()) {
            throw std::out_of_range("从空栈中弹出元素");
        }
        int item = items.back(); // 获取栈顶元素
        items.pop_back(); // 移除栈顶元素
        return item;
    }

    // 获取栈顶元素
    int top() const {
        if (is_empty()) {
            throw std::out_of_range("从空栈中获取栈顶元素");
        }
        return items.back();
    }
};
// 定义运算符的优先级
int precedence(char op) {
    switch (op) {
    case '+':
    case '-':
        return 1; // 加法和减法的优先级为1
    case '*':
    case '/':
        return 2; // 乘法和除法的优先级为2
    default:
        return 0; // 其他字符的优先级为0
    }
}

// 判断是否是运算符
bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// 判断是否是数字
bool is_number(char c) {
    return std::isdigit(c);
}

// 应用运算符进行计算
int apply_operator(int a, int b, char op) {
    switch (op) {
    case '+': return a + b; // 加法
    case '-': return a - b; // 减法
    case '*': return a * b; // 乘法
    case '/':
        if (b == 0) throw std::invalid_argument("除数不能为零");
        return a / b; // 除法
    default: throw std::invalid_argument("无效的运算符");
    }
}

// 评估表达式
int evaluate(const std::string& expression) {
    Stack values; // 用于存储操作数的栈
    Stack operators; // 用于存储运算符的栈

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (c == ' ') continue; // 跳过空格

        if (is_number(c)) {
            // 如果是数字，读取完整的数字
            int val = 0;
            while (i < expression.length() && is_number(expression[i])) {
                val = val * 10 + (expression[i] - '0');
                ++i;
            }
            --i;
            values.push(val); // 将数字压入操作数栈
        }
        else if (c == '(') {
            operators.push(c); // 左括号直接压入运算符栈
        }
        else if (c == ')') {
            // 右括号，处理直到遇到左括号
            while (!operators.is_empty() && operators.top() != '(') {
                int val2 = values.pop();
                int val1 = values.pop();
                char op = operators.pop();
                values.push(apply_operator(val1, val2, op));
            }
            if (!operators.is_empty()) {
                operators.pop(); // 弹出左括号
            }
        }
        else if (is_operator(c)) {
            // 如果是运算符，处理优先级
            while (!operators.is_empty() && precedence(operators.top()) >= precedence(c)) {
                int val2 = values.pop();
                int val1 = values.pop();
                char op = operators.pop();
                values.push(apply_operator(val1, val2, op));
            }
            operators.push(c); // 将当前运算符压入栈
        }
        else {
            throw std::invalid_argument("无效的字符");
        }
    }

    // 处理剩余的运算符
    while (!operators.is_empty()) {
        int val2 = values.pop();
        int val1 = values.pop();
        char op = operators.pop();
        values.push(apply_operator(val1, val2, op));
    }

    if (!values.is_empty()) {
        return values.pop(); // 返回最终结果
    }

    throw std::invalid_argument("无效的表达式");
}

int main() {
    std::string expression;
    std::cout << "请输入一个表达式: ";
    std::getline(std::cin, expression);

    try {
        int result = evaluate(expression);
        std::cout << "结果: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }

    return 0;
}
