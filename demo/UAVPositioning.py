import numpy as np
import os
import cv2
import example_pb
import keyboard

def rec(image):#根据亮度横纵直方图，找到四个角点，绘制方形
    # # 将图像转换为灰度图像
    # gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    gray = image
    # print(image.shape)

    #横向直方图list
    hist_x = []
    #纵向直方图list
    hist_y = []

    # 计算横向直方图
    for i in range(gray.shape[0]):
        hist_x.append(sum(gray[i]))

    # 计算纵向直方图
    for i in range(gray.shape[1]):
        hist_y.append(sum(gray[:, i]))

    # print(hist_x)
    # print(hist_y)

    # 找到横向直方图大于阈值的区间两端点
    x = []
    for i in range(len(hist_x)):
        if hist_x[i] > max(max(hist_x)*0.08,800):#0.2锁灯
            x.append(i)
    # print(max(x, default=0),min(x, default=0))
    # 找到纵向直方图大于阈值的区间两端点
    y = []
    for i in range(len(hist_y)):
        if hist_y[i] > max(max(hist_y)*0.08,800):
            y.append(i)
    # print(max(y, default=0),min(y, default=0))

    # print('len(x) is %d len(y) is %d' %(len(x), len(y)))
    #端点作为四个角点，绘制方形
    if len(x)>25 and len(y)>25:
        cv2.rectangle(image, (min(y), min(x)), (max(y), max(x)), 200, 3)

        #在方形中间绘制十字准心
        cv2.line(image, (int((max(y)+min(y))/2), int((max(x)+min(x))/2)-10), (int((max(y)+min(y))/2), int((max(x)+min(x))/2)+10), 255, 4)
        cv2.line(image, (int((max(y)+min(y))/2)-10, int((max(x)+min(x))/2)), (int((max(y)+min(y))/2)+10, int((max(x)+min(x))/2)), 255, 4)

    cv2.imshow('image', image)
    cv2.waitKey(1)

obj = example_pb.CeleXout()

while True:

    img = np.array(obj.getData()).reshape(800,1280)
    frame = cv2.medianBlur(img, 3)
    rec(frame)
    
    if keyboard.is_pressed('esc'):
        # break  # 退出循环
        quit()  # 退出程序
