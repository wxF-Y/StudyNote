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



# 资源文件

## 1.类型

![DuiLib_ReourceType](.\image\DuiLib_ReourceType.png)

## 2.UILIB_FILE

​	继承WindowImplBase时覆写GetSkinFolder()和GetSkinFile()

## 3.UILIB_ZIP(打包时，包中应直接包含资源文件，不要再包含在文件夹中)

​	继承WindowImplBase时覆写GetZIPFileName()，GetResourceType()和GetSkinFile()；此时不要覆写GetSkinfolder()方法(覆写且返回有实际内容，此时会造成资源路径是错误的)或者覆写的方法中返回空字符(返回空字符，与父类WindowImplBase一样的行为。此时路径是对的)。

​	究其原因是因为WindowImplBase的Create方法中,若我们没有在下面的287行GetResourcePath之前有设置过CPaintManager的ResourcePath则会通过292行调用GetSkinFolder生产资源路径。

![WinImplBase_Create](\image\WinImplBase_Create.png)

​	所以我们如果覆写了GetSkinFolder()方法且该方法返回有内容(强烈建议使用UILIB_ZIP资源时，不要覆写该方法.这样我们就不会有多余的设置.)，我们还可以通过在287行之前通过CPaintManager设置资源路径来规避该自动生成资源路径（一般是在win入口函数中就设置）。

## 4.UILIB_RESOURCE

## 修改CMarkup中的LoadFromMem方法，添加268行的调用Release方法。注：从EXE资源加载时需要添加，否则加载错误。

​	这样修改可能不对，因为CMarkup中有预留内存，可能是想通过当前的CMarkup对象存储所有控件。

​	在CMarkup的Load()（从字符串数组中加载）和LoadFromFile()中都先调用了Release方法。

​	在Release方法中又将这一片预留的内存释放掉了。

#### 	所以不太确定这片预留内存的用途?--私以为CMarkup在每次加载不同控件都是重新申请内存，所以还是应该添加调用Release。

CControlUI* CDialogBuilder::Create(STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback,  CPaintManagerUI* pManager, CControlUI* pParent) 是释放上一个控件内存，重新进行新开辟内存进行解析控件

CControlUI* CDialogBuilder::Create(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent) 搜索CMarkup类型的m_xml成员的m_pElements节点空间的m_pElements[1]是否为空，进行创建

​	![CMarkup_LoadFromMem](.\image\CMarkup_LoadFromMem.png)



​	通过资源文件动态创建控件，IDF_XML2,IDF_XML3是资源宏，在resource.h中定义， .rc文件中修改宏（右键.rc文件-》查看代码 进行修改）

![CDialogBuilder_Create_RESOURCE](\image\CDialogBuilder_Create_RESOURCE.png)

​		



# 5.UILIB_ZIPRESOURCE

## 	在添加进资源时必须将资源类型定为ZIPRES，因为在WinImplBase中使用zipResource时查找资源进行了写死

![CDialogBuilder_Create_ZIPRESOURCE](\image\CDialogBuilder_Create_ZIPRESOURCE.png)

​	1.继承自WindowImplBase时覆写GetResourceType()和GetResourceID()

​	2.使用zipresource包中的控件xml动态创建控件方式：

![CDialogBuilder_Create_ZIPRESOURCE](\image\SubClassWinImplBase_Create_ZIPRESOURCE.png)

​		原因：因为第一个参数-文件名不为空，则CDialogBuilder的Create()调用LoadFromFile().因为CPaintManager在继承的子类窗口类的onCreate()中调用CPaintManager的SetResourceZip()。所以在LoadFromFile ()中从zip中去寻找option.xml或tab.xml文件并解析



# CDialogBuilder解析过程内存申请

1.直接从字符串解析即Load(),在Load()中申请转换后字符串存储的内存——m_pstrXML指向该片内存；然后调用CMarkup的_Parse()-0参的方法，在该方法中通过调用__ReserveElement()方法进行预申请存储转换后节点的内存——m_pElements指向该片内存。m_pstrXML，m_pElements都是CMarkup的成员

2.在_Parse()-0参的方法中，调用 _Parse(LPTSTR& pstrText, ULONG iParent),并在该方法中解析xml并将节点存入m_pElements



# CDialogBuilder使用--磁盘文件

## 1.创建不同组件

### 1.1修改DuiLib中CMarkup类的Release方法

![CMarkup_Release](.\image\CMarkup_Release.png)

### 1.2注意CMarkup在每次从磁盘文件(.xml或zip)加载时，都会先调用Release方法.所以我们自己不用在Create方法之前先调用Release。

![CMarkup_LoadFromFile](.\image\CMarkup_LoadFromFile.png)

### 1.3例程

![CDialogBuilder_Create](.\image\CDialogBuilder_Create.png)

## 2创建相同控件

### 2.1例程

![CDialogBuilder_Create_same](.\image\CDialogBuilder_Create_same.png)
