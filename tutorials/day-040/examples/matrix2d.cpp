#include <iomanip>
#include <iostream>

// Day 40：二维动态数组——创建、乘法、正确释放（Day 14 二维数组的堆版）
int** make_matrix(int rows, int cols) {
    int** m = new int*[rows];              // 第一层：行指针数组
    for (int i = 0; i < rows; ++i) {
        m[i] = new int[cols]{};            // 第二层：每行一个 int 数组（{} 零初始化）
    }
    return m;
}

void free_matrix(int** m, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] m[i];                     // 先还每一行（逆序创建的依赖）
    }
    delete[] m;                            // 再还行指针数组
}

void fill_identity(int** m, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void print_matrix(int** m, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        std::cout << "  [";
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(4) << m[i][j];
        }
        std::cout << " ]" << std::endl;
    }
}

// 矩阵乘法：m×n · n×p = m×p（Day 14 的三重循环，数据源换成堆版）
int** multiply(int** a, int** b, int m, int n, int p) {
    int** c = make_matrix(m, p);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
    return c;
}

int main() {
    std::cout << "== 创建与填充 ==" << std::endl;
    int** a = make_matrix(2, 3);
    int v = 1;
    for (int i = 0; i < 2; ++i) {          // a = [1 2 3; 4 5 6]
        for (int j = 0; j < 3; ++j) { a[i][j] = v++; }
    }
    std::cout << "a (2×3) =" << std::endl; print_matrix(a, 2, 3);

    int** e = make_matrix(3, 2);
    e[0][0] = 7;  e[0][1] = 8;
    e[1][0] = 9;  e[1][1] = 10;
    e[2][0] = 11; e[2][1] = 12;
    std::cout << "e (3×2) =" << std::endl; print_matrix(e, 3, 2);

    std::cout << "\n== 乘法 a·e（2×3 · 3×2 = 2×2） ==" << std::endl;
    int** c = multiply(a, e, 2, 3, 2);
    std::cout << "c = a·e =" << std::endl; print_matrix(c, 2, 2);

    std::cout << "\n== 单位矩阵验证 ==" << std::endl;
    int** i3 = make_matrix(3, 3);
    fill_identity(i3, 3);
    int** ai = multiply(a, i3, 2, 3, 3);   // a · I = a
    std::cout << "a · I(3) =" << std::endl; print_matrix(ai, 2, 3);
    std::cout << "（与 a 相同——右乘单位矩阵不变）" << std::endl;

    std::cout << "\n== 正确释放（逆依赖序） ==" << std::endl;
    free_matrix(a, 2);   std::cout << "  a 已释放（1 个指针数组 + 2 行）" << std::endl;
    free_matrix(e, 3);   std::cout << "  e 已释放（1 + 3）" << std::endl;
    free_matrix(c, 2);   std::cout << "  c 已释放（1 + 2）" << std::endl;
    free_matrix(i3, 3);  std::cout << "  i3 已释放（1 + 3）" << std::endl;
    free_matrix(ai, 2);  std::cout << "  ai 已释放（1 + 2）——共 5 次 free_matrix" << std::endl;
    return 0;
}
