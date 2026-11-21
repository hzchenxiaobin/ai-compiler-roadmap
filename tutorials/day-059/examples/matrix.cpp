#include <cassert>
#include <iomanip>
#include <iostream>

// ===== Day 59：Matrix 矩阵类——本周运算符全家桶的总演练场 =====
// m[i][j] 链式下标 + 算术（+ × 标量*）+ == + <<
class Matrix {
private:
    int rows_, cols_;
    int* data_;

    int& at(int r, int c) { return data_[r * cols_ + c]; }
    const int& at(int r, int c) const { return data_[r * cols_ + c]; }

public:
    Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(new int[rows * cols]{}) {}

    ~Matrix() { delete[] data_; }

    Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_),
                                  data_(new int[other.rows_ * other.cols_]) {
        for (int i = 0; i < rows_ * cols_; ++i) { data_[i] = other.data_[i]; }
    }

    Matrix& operator=(const Matrix& other) {
        if (this == &other) { return *this; }
        delete[] data_;
        rows_ = other.rows_; cols_ = other.cols_;
        data_ = new int[rows_ * cols_];
        for (int i = 0; i < rows_ * cols_; ++i) { data_[i] = other.data_[i]; }
        return *this;
    }

    // 链式下标：Row 代理类（两次 [] 分两步走）
    struct Row {
        Matrix* m_;
        int r_;
        int& operator[](int c) { return m_->at(r_, c); }        // 第二步：定列
    };
    struct ConstRow {
        const Matrix* m_;
        int r_;
        const int& operator[](int c) const { return m_->at(r_, c); }
    };

    Row operator[](int r) { return Row{this, r}; }               // 第一步：定行
    ConstRow operator[](int r) const { return ConstRow{this, r}; }

    // 矩阵加法：同型矩阵逐元素
    Matrix operator+(const Matrix& rhs) const {
        assert(rows_ == rhs.rows_ && cols_ == rhs.cols_);        // 教学版断言（真实库抛异常）
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_ * cols_; ++i) { result.data_[i] = data_[i] + rhs.data_[i]; }
        return result;
    }

    // 矩阵乘法：m×n · n×p = m×p（Day 14 老朋友的类版）
    Matrix operator*(const Matrix& rhs) const {
        assert(cols_ == rhs.rows_);
        Matrix result(rows_, rhs.cols_);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < rhs.cols_; ++j) {
                int sum = 0;
                for (int k = 0; k < cols_; ++k) {
                    sum += at(i, k) * rhs.at(k, j);
                }
                result.at(i, j) = sum;
            }
        }
        return result;
    }

    // 标量乘：矩阵 × 数字
    Matrix operator*(int k) const {
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_ * cols_; ++i) { result.data_[i] = data_[i] * k; }
        return result;
    }

    bool operator==(const Matrix& rhs) const {
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_) { return false; }
        for (int i = 0; i < rows_ * cols_; ++i) {
            if (data_[i] != rhs.data_[i]) { return false; }
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (int i = 0; i < m.rows_; ++i) {
            os << "  [";
            for (int j = 0; j < m.cols_; ++j) {
                os << std::setw(4) << m.at(i, j);
            }
            os << " ]" << std::endl;
        }
        return os;
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }
};

int main() {
    std::cout << "== 链式下标 m[i][j] 读写 ==" << std::endl;
    Matrix a(2, 3);
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;      // 写：两次 [] 各定一步
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    std::cout << "a =\n" << a;
    std::cout << "a[1][2] = " << a[1][2] << std::endl;   // 读

    std::cout << "\n== 拷贝构造 + ==" << std::endl;
    Matrix b = a;                                 // 三件套的拷贝构造
    Matrix c = a + b;
    std::cout << "c = a + b =\n" << c;

    std::cout << "\n== 标量乘 ==" << std::endl;
    Matrix d = a * 10;
    std::cout << "d = a * 10 =\n" << d;

    std::cout << "\n== 矩阵乘法 ==" << std::endl;
    Matrix e(3, 2);                               // 2×3 · 3×2 = 2×2
    e[0][0] = 7; e[0][1] = 8;
    e[1][0] = 9; e[1][1] = 10;
    e[2][0] = 11; e[2][1] = 12;
    Matrix f = a * e;
    std::cout << "f = a * e =\n" << f;

    std::cout << "\n== 相等比较 ==" << std::endl;
    std::cout << "a == b ? " << (a == b ? "相等" : "不等") << std::endl;
    std::cout << "a == c ? " << (a == c ? "相等" : "不等") << std::endl;
    return 0;
}
