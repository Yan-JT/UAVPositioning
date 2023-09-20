This is the manual for drone positioning demo

version:  1.0

time : 2023.9.20

author : Juntong Yan

# 项目结构

UAVPositioning

* CeleX5-MIPI-master（CeleX 5 MIPI源码、手册）
* demo（演示）
* DVSProject（DVS C++2python）
* README.md


# 硬件准备

PC（linux）、CeleX 5 MIPI

# DVS C++2python

> 这部分是因为CeleX 5 MIPI的源码是C++完成的，为了系统整体方便控制，需要打包为python库。

参考以下链接：

[https://gumpest.notion.site/CeleX-Tools-349b24cf45e94af2b513406784c43f23](https://gumpest.notion.site/CeleX-Tools-349b24cf45e94af2b513406784c43f23)

核心文件为getEventDataVector.cpp，其中修改相机各类参数和运行模式（灰度、事件等），见CeleX 5 MIPI使用手册

# Demo展示

进入/home/ailab/Desktop/UAVPositioning/demo

`conda activate RobotGoalKeeper`

`sudo /home/ailab/miniconda3/envs/RobotGoalKeeper/bin/python UAVPositioning.py`
