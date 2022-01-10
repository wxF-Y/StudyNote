# 控件

## Label

默认拉伸占满整个容器

## 占位符

一个空的Control控件

### 使用方法：

​	<Control />

### 注意事项：

Control会根据父容器的宽度进行无限拉伸。如果父容器中其他子容器设置了固定宽度，则Control占满父容器剩下的宽度。



# 属性

##  padding="x,x,x,x" 

组件的外间距，位置分别为左、上、右、下

## childpadding

在HorizontalLayout中为子组件的水平间距，在VerticalLayout中为子组件的垂直间距。只在子组件中间空出间距.

## inset

组件的内间距，位置分别为左、上、右、下



# 消息循环

CWindowWnd(__WndProc) 

​	-> 子窗口类(HandleMessage) 

​		 -> 子窗口类(HandleCustomMessage)

​		 ->子窗口成员CPaintManager(MessageHandler) 

​			->查找到的控件的Event(&event),若存在继承关系，则调用孙子的Event()添加事件，然后调用其事件处理函数DoEvent.再决定是否需要调用父类控件的				DoEvent.
