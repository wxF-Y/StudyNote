#### lm--列出已加载的模块

## 寄存器

eax	函数返回

ebp	栈底基地址指针

ecx 	存放this指针

esp	栈顶指针

eip	下一指令在内存中的位置



## 切换Bit

```
.load wow64exts
!sw
```

## IDA地址与windbg地址对应

文件偏移地址（File Offset）

装载地址 （Image Base）

虚拟内存地址（VA）

相对虚拟地址（RVA）

其中相关概念字面理解即可，需要了解一个关系是：
 *VA = Image Base + RVA*

windbg中为运行时虚拟地址，IDA加载时可通过Manual load设置模块的加载基地址（即Image Base）