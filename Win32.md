# LPCTSTR

L 	long指针

P 	指针

C	常量

T	win32环境，_T宏

STR	该变量是字符串



# HRSRC

资源句柄

# DWORD

一个word（字）为2字节，D为double。

double word -- 4字节

# HGLOBAL

全局内存块句柄

# 窗口样式和窗口扩展样式

https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles

https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles

# 窗口消息

https://docs.microsoft.com/en-us/windows/win32/winmsg/window-notifications

#判断point是否在rect里
BOOL PtInRect(
  [in] const RECT *lprc,
  [in] POINT      pt
);

# SendMessage\PostMessage

同线程：

https://blog.51cto.com/dreamylights/1325841

SendMessage不走消息循环，windows内部直接调用窗口处理函数

PostMessage走消息循环调窗口处理函数

跨线程:

SendMessage将消息放入消息队列并等待对应线程响应消息直到响应返回

PostMessage走消息循环调窗口处理函数





### 顶级窗口和顶层窗口

顶层窗口--不具有WS_CHILD 属性