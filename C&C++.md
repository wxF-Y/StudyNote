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



# 单例实例--https://blog.shipengx.com/archives/f61b5eee.html

Q:动态内存分配时 `new` 底层操作的是**非原子性**导致的，执行 `pinstance_ = new LazySingleton;` 语句时，底层其实对应了三个步骤：

1. 向系统申请分配内存，大小为 `sizeof(LazySingleton)`
2. 调用 `LazySingleton` 的默认构造函数在申请的内存上构造出实例
3. 返回申请内存的指针给 `pinstance_`

A:使用memory barrier，禁止乱序优化

### Meyers单例

```c++
class MeyersSingleton
{
private:
    MeyersSingleton() {};
    MeyersSingleton(const MeyersSingleton &) = delete;
    MeyersSingleton &operator=(const MeyersSingleton &) = delete;

public:
    ~MeyersSingleton();

public:
    static MeyersSingleton &GetInstance()
    {
        static MeyersSingleton instance;
        return instance;
    }
};
```

多线程安全：C++11 及以后的标准中 - If multiple threads attempt to initialize the same static local variable concurrently, the initialization occurs exactly once (similar behavior can be obtained for arbitrary functions with std::call_once).



### 百度 Apollo 中的泛化懒汉式单例

```c++
#ifndef CYBER_COMMON_MACROS_H_
#define CYBER_COMMON_MACROS_H_

#include <iostream>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include "cyber/base/macros.h"

DEFINE_TYPE_TRAIT(HasShutdown, Shutdown)

template <typename T>
typename std::enable_if<HasShutdown<T>::value>::type CallShutdown(T *instance) {
  instance->Shutdown();
}

template <typename T>
typename std::enable_if<!HasShutdown<T>::value>::type CallShutdown(
    T *instance) {
  (void)instance;
}

// There must be many copy-paste versions of these macros which are same
// things, undefine them to avoid conflict.
#undef UNUSED
#undef DISALLOW_COPY_AND_ASSIGN

#define UNUSED(param) (void)param

#define DISALLOW_COPY_AND_ASSIGN(classname) \
  classname(const classname &) = delete;    \
  classname &operator=(const classname &) = delete;

#define DECLARE_SINGLETON(classname)                                      \
 public:                                                                  \
  static classname *Instance(bool create_if_needed = true) {              \
    static classname *instance = nullptr;                                 \
    if (!instance && create_if_needed) {                                  \
      static std::once_flag flag;                                         \
      std::call_once(flag,                                                \
                     [&] { instance = new (std::nothrow) classname(); }); \
    }                                                                     \
    return instance;                                                      \
  }                                                                       \
                                                                          \
  static void CleanUp() {                                                 \
    auto instance = Instance(false);                                      \
    if (instance != nullptr) {                                            \
      CallShutdown(instance);                                             \
    }                                                                     \
  }                                                                       \
                                                                          \
 private:                                                                 \
  classname();                                                            \
  DISALLOW_COPY_AND_ASSIGN(classname)

#endif  // CYBER_COMMON_MACROS_H_
```

实现：使用static的onec_flag变量和std::call_once保证多线安全



# 模板

## 模板元编程核心：静态计算和类型萃取

## 模板参数自动推导

1. 根据函数参数自动推导（模板函数）

2. 根据构造参数自动推导（模板类）

3. 自定义构造推导（模板类）

## 模板全特化及偏特化

## 偏特化模板的匹配优先级：全特化>偏特化>通用模板(若一个模板实例化时能同时满足多个匹配(匹配参数互补的情况)则编译时提示不明确)

## SFINAE:实例化模板时会尝试该模板的所有匹配形式，因此匹配失败不是错误（Substitution Failure Is Not An Error）