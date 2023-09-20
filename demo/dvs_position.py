import cv2
import os
import numpy as np
# 指定视频文件路径为苹果电脑桌面上的一个视频文件
video_path = 'MipiData_20230906_092544936_E_100M.mkv'

# 创建一个VideoCapture对象
cap = cv2.VideoCapture(video_path)

# 检查视频是否正确打开
if not cap.isOpened():
    print("Error: Couldn't open the video file.")
    exit()


def rec(image,num):#根据亮度横纵直方图，找到四个角点，绘制方形
    # # 将图像转换为灰度图像
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

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
    # print(max(x),min(x))
    # 找到纵向直方图大于阈值的区间两端点
    y = []
    for i in range(len(hist_y)):
        if hist_y[i] > max(max(hist_y)*0.08,800):
            y.append(i)
    # print(max(y),min(y))

    #端点作为四个角点，绘制方形
    if len(x)>25 and len(y)>25:
        cv2.rectangle(image, (min(y), min(x)), (max(y), max(x)), (0, 255, 0), 2)

        #在方形中间绘制红色十字准心
        cv2.line(image, (int((max(y)+min(y))/2), int((max(x)+min(x))/2)-10), (int((max(y)+min(y))/2), int((max(x)+min(x))/2)+10), (0, 0, 255), 2)
        cv2.line(image, (int((max(y)+min(y))/2)-10, int((max(x)+min(x))/2)), (int((max(y)+min(y))/2)+10, int((max(x)+min(x))/2)), (0, 0, 255), 2)

    cv2.imshow('image', image)
    cv2.imwrite('rec/frame%d.jpg'%num,frame)




num=0
while True:
    # 读取视频的下一帧
    ret, frame = cap.read()
    # 如果读取帧失败，可能到达视频末尾，就退出循环
    if not ret:
        break

    # 中值滤波
    frame = cv2.medianBlur(frame, 3)
    rec(frame,num)


    num=num+1
    print(num)
    # 显示帧
    # cv2.imshow('Video Playback', frame)

    # 按 'q' 键停止播放
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

# 释放资源并关闭显示窗口
cap.release()
cv2.destroyAllWindows()
print('video_over')


#图片合成视频
def picvideo(path,size):
    filelist = os.listdir(path) #获取该目录下的所有文件名
    filelist.sort(key=lambda x:int(x[5:-4])) #(x[5:-4]意思是从第5个字符开始取，取到倒数第4个字符结束，即取出了文件名中的数字)
    fps = 30    #视频每秒24帧
    #size = (591, 591) #需要转为视频的图片的尺寸
    file_path = path+'.avi'#导出路径
    # fourcc = cv2.VideoWriter_fourcc('I', '4', '2', '0')#不同视频编码对应不同视频格式（例：'I','4','2','0' 对应avi格式）
    video = cv2.VideoWriter(file_path, cv2.VideoWriter_fourcc(*'MJPG'), fps, size,isColor=True)#视频保存在当前目录下
    for item in filelist:
        item = path + '/' + item
        # print(item)
        img = cv2.imread(item)
        #展示一下图片
        # cv2.imshow("Image",img)
        # cv2.waitKey(1)

        video.write(img) 
    
    video.release()
    print('over')


picvideo('rec',(1280,800))
# picvideo('initial',(1280,800))
# picvideo('blur',(1280,800))

