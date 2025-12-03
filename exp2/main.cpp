#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

// 定义哈夫曼树的节点
struct HuffNode {
    char ch;
    int freq;
    HuffNode* left;
    HuffNode* right;

    HuffNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}

    // 用于优先队列的比较
    bool operator>(const HuffNode& other) const {
        return freq > other.freq;
    }
};

// 哈夫曼树类
class HuffTree {
public:
    HuffNode* root;

    HuffTree(std::unordered_map<char, int>& freq) {
        // 创建优先队列
        std::priority_queue<HuffNode*, std::vector<HuffNode*>, std::greater<HuffNode*>> pq;

        // 将所有字符及其频率加入优先队列
        for (auto& pair : freq) {
            pq.push(new HuffNode(pair.first, pair.second));
        }

        // 构造哈夫曼树
        while (pq.size() > 1) {
            HuffNode* left = pq.top(); pq.pop();
            HuffNode* right = pq.top(); pq.pop();

            HuffNode* sum = new HuffNode('\0', left->freq + right->freq);
            sum->left = left;
            sum->right = right;

            pq.push(sum);
        }

        root = pq.top();
    }

    // 生成哈夫曼编码
    void generateCodes(HuffNode* node, const std::string& code, std::unordered_map<char, std::string>& huffCode) {
        if (!node) return;
        if (node->left == nullptr && node->right == nullptr) {
            huffCode[node->ch] = code;
        }
        generateCodes(node->left, code + "0", huffCode);
        generateCodes(node->right, code + "1", huffCode);
    }
};

// 哈夫曼编码类
class HuffmanCoding {
public:
    std::unordered_map<char, int> frequencies;
    std::unordered_map<char, std::string> huffCode;

    // 构造函数，根据文本频率统计
    HuffmanCoding(const std::string& filename) {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();  // 读取文件内容

        std::string text = buffer.str();
        for (char ch : text) {
            if (isalpha(ch)) frequencies[tolower(ch)]++; // 统计字符频率
        }
    }

    // 执行哈夫曼编码
    void encode() {
        HuffTree huffTree(frequencies);  // 根据字符频率构造哈夫曼树
        huffTree.generateCodes(huffTree.root, "", huffCode);  // 生成编码
    }

    // 获取单词的哈夫曼编码
    std::string getCode(const std::string& word) {
        std::string encoded = "";
        for (char ch : word) {
            encoded += huffCode[tolower(ch)] + " ";  // 每个编码之间加空格
        }
        return encoded;
    }

    // 输出26个字母的哈夫曼编码
    void printLetterCodes() {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            if (huffCode.find(ch) != huffCode.end()) {
                std::cout << ch << ": ";
                for (char bit : huffCode[ch]) {
                    std::cout << bit << " ";  // 输出编码时在每个 bit 之间添加空格
                }
                std::cout << std::endl;
            }
        }
    }

    // 输出所有的哈夫曼编码
    void printCodes() {
        for (auto& pair : huffCode) {
            std::cout << pair.first << ": ";
            for (char bit : pair.second) {
                std::cout << bit << " ";  // 输出编码时在每个 bit 之间添加空格
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    std::string speech;       // 存储从文件读取的文本
    // 直接写入你的绝对路径（注意：Windows系统中路径分隔符用双反斜杠 \\，或单斜杠 /）
    std::string filePath = "D:\\code\\数据结构实验\\实验2\\exp2\\exp2\\x64\\Debug\\i have a dream.txt";
    std::ifstream file;       // 文件输入流

    // 打开文件并检查是否成功
    file.open(filePath, std::ios::in); // ios::in 表示只读模式
    if (!file.is_open(

        // 读取文件全部内容到 speech 中（逐行读取，保留完整文本）
        std::string line;
    while (getline(file, line)) {
        speech += line + " "; // 每行末尾加空格，避免单词拼接
    }
    file.close(); // 读取完成后关闭文件，释放资源)) {
    cerr << "错误：无法打开文件！请检查路径是否正确。" << endl;
    system("pause");
    return 1; // 异常退出
}

    HuffmanCoding huffman(filename);  // 根据文件创建HuffmanCoding对象
    huffman.encode();  // 执行哈夫曼编码

    // 输出所有字母的编码
    std::cout << "Huffman Codes for 26 Letters: " << std::endl;
    huffman.printLetterCodes();

    // 输出单词 "dream" 的哈夫曼编码
    std::string word = "dream";
    std::cout << "Huffman encoding for word '" << word << "': " << huffman.getCode(word) << std::endl;

    return 0;
}
