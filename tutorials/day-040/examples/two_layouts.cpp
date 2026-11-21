#include <iostream>

// 两种"二维"的内存布局对照：锯齿阵 vs 一维折叠（Day 65 Matrix 类的 r*cols+c）
int main() {
    std::cout << "== 布局一：锯齿阵（今天的 int** 版） ==" << std::endl;
    int** jag = new int*[2];
    jag[0] = new int[3]{1, 2, 3};
    jag[1] = new int[3]{4, 5, 6};
    std::cout << "  jag        = " << jag << "（行指针数组，堆）" << std::endl;
    std::cout << "  jag[0]     = " << jag[0] << std::endl;
    std::cout << "  jag[1]     = " << jag[1] << "（两行地址不同、不必相邻！）" << std::endl;
    std::cout << "  &jag[0][1] = " << &jag[0][1] << "，&jag[0][2] = " << &jag[0][2] << "（行内连续）" << std::endl;

    std::cout << "\n== 布局二：一维折叠（r*cols + c） ==" << std::endl;
    int* flat = new int[6]{1, 2, 3, 4, 5, 6};
    auto at = [&](int r, int c) -> int& { return flat[r * 3 + c]; };
    std::cout << "  flat       = " << flat << "（一整块，天然连续）" << std::endl;
    std::cout << "  at(1,2) 即 flat[1*3+2] = flat[5] = " << at(1, 2) << std::endl;
    std::cout << "  &at(0,2) 与 &at(1,0) 相邻（差 4 字节）——行与行无缝" << std::endl;
    std::cout << "  " << &at(0,2) << " vs " << &at(1,0) << std::endl;

    std::cout << "\n== 对照结论 ==" << std::endl;
    std::cout << "  锯齿：2 次分配、行可不齐（每行长度可不同）——释放要逐行" << std::endl;
    std::cout << "  折叠：1 次分配、强制矩形——释放一次搞定" << std::endl;
    std::cout << "  （Day 65 的 Matrix 类选折叠——泄漏面更小）" << std::endl;

    // 两种释放
    delete[] jag[0]; delete[] jag[1]; delete[] jag;   // 锯齿：3 次
    delete[] flat;                                     // 折叠：1 次
    std::cout << "\n两种布局都已正确释放" << std::endl;
    return 0;
}
