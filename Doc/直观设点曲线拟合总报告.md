# 报告

## 2019年袁昕老师短学期课程

### 1.问题和背景

[见需求分析](https://github.com/zhou317/Experiment/blob/master/demand_analysis/%E7%9B%B4%E8%A7%82%E8%AE%BE%E7%82%B9%E6%9B%B2%E7%BA%BF%E6%8B%9F%E5%90%88%E5%BC%80%E9%A2%98%E6%8A%A5%E5%91%8A.md)

本项目基于Qt5.13.0实现GUI界面。

使用Visual studio 2019 以及 c++17特性进行编程。

使用jenkins进行持续集成。

### 3.开发计划和分工

* 第一轮迭代
  * 邵尧负责windows层与view层
  * 周章华负责viewmodel层与model层
  * 周国晨负责common层与model层的算法

### 4.每轮迭代结果

#### 第一轮迭代

第一轮迭代包括：

v1.0 版本实现了对左侧表格中数据进行线性拟合的方法，如图所示：

![1.0版本图](https://github.com/zhou317/Experiment/blob/dev/img_of_readme/first_iter_01.png)

v1.1 版本完善了MVVM模型。运行效果与v1.0版本相同。

#### 第二轮迭代

第二轮迭代包括：

v2.0实现画出拟合函数图像的功能：

![函数作图](https://github.com/zhou317/Experiment/blob/master/img_of_readme/2nd.png)

实现了错误的检测与错误输出，错误输出包括两个部分

* 底部输出框的错误提示
* 画图模块的图像显示

![错误输出](https://github.com/zhou317/Experiment/blob/dev/img_of_readme/error.png)

v2.1添加鼠标移动到点上显示坐标的功能

![错误输出](https://github.com/zhou317/Experiment/blob/dev/img_of_readme/2_1v.png)



### 第三轮迭代

第三轮迭代包括：