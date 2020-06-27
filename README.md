# design
some work, designs, or thinks.

# git shell use:

首先，要安装软件工具环境：windows/linux下安装git。

1. 初次使用，需要先在github网站上新建一个repository仓库

2. 下拉该仓库到本地

$ git clone https://github.com/embarassed01/XXXXXX.git

3. 在本地更新该目录下内容，添加/删减目录，或添加/更新文件

4. windows下右击该目录的根目录，选择“Git Bash here”，打开Linux虚拟shell环境；

    or , linux下进到该目录的根目录下：

5. 更新本地缓存并推送到网站仓库，必须“commit”才会生效：

5.(1) 当第一次提交时，使用如下命令：

// 推送到本地缓存：

$ git init   // 初始化版本库

$ git add .   // 添加文件到版本库（只是添加到缓存区），.代表添加文件夹下所有文件

$ git commit -m "first commit"   // 把添加的文件提交到版本库，并填写提交备注

// 推送到网站仓库：

$ git remote add origin https://github.com/embarassed01/XXXXXX.git  // 把本地库与远程库关联

$ git push -u origin master    // 第一次推送时 。会提示输入用户名，密码

$ git push origin master      // 第一次推送后，直接使用该命令即可推送修改。会提示输入用户名，密码

5.(2) 当以后提交时，使用如下命令：

$ git add .

$ git commit -m "some 提交注释"

$ git push origin master     //  会提示输入用户名，密码

6.以后使用时，请使用git pull用于更新；使用git push推送上传同步。

