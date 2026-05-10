/**
 * @file stack_overflow.cpp
 * @author cao zhang
 * @brief 深拷贝示例
 * 
 * 本程序演示栈相关的内容
 * 1、查看当前进程栈限制
 * 2、故意制造栈溢出 (Segment fault)
 * 3、代码里用 setrlimit 动态修改栈大小
 * 
 * @version 0.1
 * @date 2026-05-10
 */

#include <iostream>
#include <sys/resource.h>
#include <cstring>


// 打印当前栈资源限制
void print_stack_limit()
{
    struct rlimit rl;
    getrlimit(RLIMIT_STACK, &rl);

    std::cout << "===== 当前栈空间限制 =====" << std::endl;
    std::cout << "软限制(soft): " << rl.rlim_cur / 1024 << " KB" << std::endl;
    std::cout << "硬限制(hard): " << rl.rlim_max / 1024 << " KB" << std::endl;
    std::cout << "-------------------------" << std::endl;

    /**
     * 软限制（Soft Limit）：当前生效的上限，程序现在就受这个约束，超了就报错 / 段错误。
     * 硬限制（Hard Limit）：天花板上限，是软限制能调到的最大值，不能突破它。
     * 
     * 普通用户：
     * 可以随便把软限制往小调
     * 可以把软限制往大调，但最大不能超过硬限制
     * 无权修改硬限制
     * 
     * root 管理员：
     * 可以随意改软限制、硬限制，可以把硬限制改得更大
     */
}

// 递归爆栈：局部大数组吃栈
void stack_overflow_test()
{
    // 局部数组，分配在【栈】上
    char big_arr[1024 * 1024 * 2]; // 单次 2MB 栈内存
    memset(big_arr, 0, sizeof(big_arr));

    // 递归无限调用，持续吃栈
    stack_overflow_test();
}

// 代码内动态修改栈大小
void set_stack_limit(size_t mb)
{
    struct rlimit rl;
    // 先获取原有硬限制
    getrlimit(RLIMIT_STACK, &rl);

    size_t kb = mb * 1024;
    rl.rlim_cur = kb;       // 软限制设为指定 kb

    if (setrlimit(RLIMIT_STACK, &rl) == -1)
    {
        perror("setrlimit failed");
    }
    else
    {
        std::cout << "已将栈软限制设置为: " << mb << " MB" << std::endl;
    }
}

int main()
{
    // 1. 打印初始默认栈大小
    print_stack_limit();

    // 2. 动态把栈改成 16MB
    set_stack_limit(16);

    // 3. 改完再看
    print_stack_limit();

    // 4. 开启栈溢出测试，程序会直接段错误崩溃
    std::cout << "开始递归爆栈，马上 Segmentation fault..." << std::endl;
    stack_overflow_test();

    return 0;
}