## 结构化绑定

```c++
绑定数组

int a[2] = {1, 2};
 
auto [x, y] = a;   // 创建 e[2]，复制 a 到 e，然后 x 指代 e[0]，y 指代 e[1]
auto& [xr, yr] = a; // xr 指代 a[0]，yr 指代 a[1]


绑定数据成员

struct BitFields {
  int b : 4 {1}, d : 4 {2}, p : 4 {3}, q : 4 {4};
  // C++20 ：位域的默认成员初始化器
};

const auto [b, d, p, q] = BitFields{};

结构化绑定最常用的场景就是用于 std::map 和 std::unordered_map 容器的遍历。
for(auto& [key, val] : map) {
...
}
```

