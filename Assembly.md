### 寄存器

​	%ax~%bp		16位寄存器

​	%eax~%ebp	32位寄存器

​	%rax~%rbp	64位寄存器

​	%rsp	栈指针

![register](E:\StudyNote\image\register.png)



### 寄存器标志位

​	ZF: 零标志位，flag的第六位记录相关指令执行后，结果为0，则ZF=1;若结果不为0，ZF=0.

​	PF,奇偶标志位，flag的第2位记录相关指令执行后，其结果所有bit位中1的个数是否为偶数，若为偶数，则PF=1;若为奇数，PF=0.

​	SF,符号标志位，flag的第7位，记录相关指令执行后，其结果是否为负，若为负，则SF=1;若为非负，SF=0.

​	CF,进位标志位，flag的第0位，在进行**无符号数**运算的时候，CF记录了运算结果的最高有效位向更高有效位向更高位的进位值/借位值

​	OF,溢出标志位，flag的第11位，超出机器所能表示的范围称为溢出若发生了溢出OF=1，若没有则OF=0

​	

### 汇编指令

XOR : 同为0，异为1.（常用于对一个寄存器置0）

INC\DEC : 加1\减一

NEG : 取相反数     =      按位取反 + 1

move dst src

![image-20241209110821151](E:\StudyNote\image\dsm_instruction.png)

mov         ecx,dword ptr fs:[2Ch]//TEB便宜0x2Ch的地方是__tls_array__指针

