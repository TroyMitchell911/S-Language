# S-Language

## 作者信息

Author: Troy

E-mail: andrew998@126.com

Github: [https://github.com/JiangLin998](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FJiangLin998)

## 开发环境

Windows11

Clion 2023.3.4

## 更新记录

### Mar 11,2024

- 完成分词功能
- 完成报错和警告功能
- 完成对于小数的识别
- 完成对于行注释和多行注释的跳过以及多行注释对不匹配的检测

### Mar 20,2024

- 修复debug函数无法打印target的bug
- 添加open_list库
- 消除symbols_func.c文件中的警告
- utils中的报告支持格式化输出
- scanner分词器添加获取下一个token前所必须的初始化函数
- 修改flood中对于未知字符的处理为报错
- 修复symbols_func.c文件中对于多行注释可能存在的bug
- 符号支持文件symbols.inc与symbols_func.c中添加对逗号和分号的支持

### Mar 21,2024

#### 5:07pm

- 补提交lib仓库openlist忘传的问题
- 添加二叉搜索字符串库
- utils中添加初始化函数和清空中间文件函数
- 完成导出二元式

#### 6:47pm

- 完成建立标识符表和常量表
- 添加摧毁二叉搜索树函数
- 在删除scanner时会回收二叉树内存

### Mar 22,2024

#### 6:15pm

- 修复二叉树摧毁函数的bug
- 修复debug宏无法关闭的问题
- 修改二叉树根节点为每个分词器单独使用
- 修改symbols_func.c文件中单符号通用宏
- 将符号解析机械函数合并为宏

## TODO

- [ ] 增加函数内注释
- [ ] 找寻可能存在的Bug
- [ ] 完善关于路径的部分