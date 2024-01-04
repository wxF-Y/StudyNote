## 多参数入栈顺序&参数计算顺序

##### 1.入栈顺序：与调用约定有关, windows API 一般使用**` __stdcall`**, 导出一般为**`__cdecl`**

![func_call_convention](image\func_call_convention.png)

#### 2.参数计算顺序：C/C++规范未定义，与编译器有关

```c++
#include <stdio.h>

int main () {
 int a = 2;
 printf("%d, %d, %d", a, (a = (a + 2)), (a = (a + 3)));
 system("pause");
 return 0;
}
//win10 + VS2019 输出: 7, 7, 7
//clang输出结果 2 4 7 
```

vs的计算顺序是从右至左，clang的计算顺序是从左至右，具体的计算流程分析就很简单了。

对于c/c++函数参数的读取顺序，参数入栈时顺序从右向左入栈，但是在入栈前会先把参数列表里的表达式从右向左算一遍得到表达式的结果，最后再把这些运算结果统一入栈。

在参数入栈前，编译器会先把参数的表达式都处理掉，对于一般的操作来说，参数入栈时取值是直接从变量的内存地址里取的，但是对于a++操作，编译器会开辟一个缓冲区来保存当前a的值，然后再对a继续操作，最后参数入栈时的取值是从缓冲区取，而不是直接从a的内存地址里取。
知乎文章地址：https://zhuanlan.zhihu.com/p/381907110



## 编译器内存屏障(memory barrier)

#### 使用原因：编译器指令重排(Compiler Instruction Reordering)， 该优化可能会导致代码执行顺序发生倒转，memory barrier显示告知编译器不进行优化，保证代码执行顺序,另一个作用则是告知编译器抛弃前面寄存器缓存的值，重新load一次

#### 编译器Barrier类型

##### 	1. 显式编译器屏障(Explicit Compiler Barriers)  --显示调用

```c++
#define barrier() __asm__ __volatile__("": : :"memory")
```

##### 	2.隐式编译器屏障(Implied Compiler Barriers) --调用func中存在barrier的调用或调用一个外部func(相对inline)

##### 