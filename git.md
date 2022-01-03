# 添加ssh

1.进入C盘->用户p>用户名文件夹。若存在.ssh文件夹，将其删除。

2.打开git bash.依次输入下列命令:

git config --global user.name "用户名"       -- 可随便输

git config --global user.email "邮箱地址"   --	git绑定的邮箱地址

ssh-keygen -t rsa -C "上面设置的邮箱地址"	--	生成密匙

3.进入C盘->用户p>用户名文件夹->.ssh文件夹.	打开id_rsa.pub文件,复制文件中所有内容.

4.进入github本人主页,打开settings页.setting在右下角:![image-20220103182834751](.\image\git_setting.png)

5.选择SSH and GPG keys项.在左下角:

![git_ssh](.\image\git_ssh.png)

6.选择new ssh key.填写title(随便写).将复制的内容填入key下方的框中.