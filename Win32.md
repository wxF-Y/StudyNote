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