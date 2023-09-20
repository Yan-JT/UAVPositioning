#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <vector>
#include"mat_warper.h"

#include <opencv2/opencv.hpp>
#include <celex5/celex5.h>

namespace py = pybind11;
using namespace std;
using namespace cv;

#ifdef _WIN32
#include <windows.h>
#include<process.h>
#else
#include<unistd.h>
#include <signal.h>
#include<pthread.h>
#endif

#define FPN_PATH    "../Samples/config/FPN_3.txt"

CeleX5 *pCeleX5 = new CeleX5;

#ifdef _WIN32
bool exit_handler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
		//ctrl-close: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
	case CTRL_C_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		delete pCeleX5;
		pCeleX5 = NULL;
		return(true);
	default:
		return false;
	}
}
#else
void exit_handler(int sig_num)
{
	printf("SIGNAL received: num =%d\n", sig_num);
	if (sig_num == 1 || sig_num == 2 || sig_num == 3 || sig_num == 9 || sig_num == 15)
	{
		delete pCeleX5;
		pCeleX5 = NULL;
		exit(0);
	}
}
#endif

pybind11::array_t<unsigned char> cv_mat_uint8_1c_to_numpy(cv::Mat& input) {

    pybind11::array_t<unsigned char> dst = pybind11::array_t<unsigned char>({ input.rows,input.cols }, input.data);
    return dst;
}

pybind11::array_t<unsigned char> cv_mat_uint8_3c_to_numpy(cv::Mat& input) {

    pybind11::array_t<unsigned char> dst = pybind11::array_t<unsigned char>({ input.rows,input.cols,3}, input.data);
    return dst;
}

class CeleXout{
public:
    // 成员变量
	CeleX5 *pCeleX5;
	CeleX5::CeleX5Mode sensorMode;
	uint8_t * pSensorBuffer;

	// 成员函数
	CeleXout(){
		this->pCeleX5 = new CeleX5();
		this->pCeleX5->openSensor(CeleX5::CeleX5_MIPI);
		this->pCeleX5->setFpnFile(FPN_PATH);
		this->pCeleX5->disableEventStreamModule();
                //this->pCeleX5->setEventFrameTime(5000);
		this->sensorMode = CeleX5::Event_Off_Pixel_Timestamp_Mode;
                //this->sensorMode = CeleX5::Event_Intensity_Mode;
		this->pCeleX5->setSensorFixedMode(sensorMode);

		// install signal use sigaction
		struct sigaction sig_action;
		sigemptyset(&sig_action.sa_mask);
		sig_action.sa_flags = 0;
		sig_action.sa_handler = exit_handler;
		sigaction(SIGHUP, &sig_action, NULL);  // 1
		sigaction(SIGINT, &sig_action, NULL);  // 2
		sigaction(SIGQUIT, &sig_action, NULL); // 3
		sigaction(SIGKILL, &sig_action, NULL); // 9
		sigaction(SIGTERM, &sig_action, NULL); // 15

		
		this->pSensorBuffer = new uint8_t[CELEX5_PIXELS_NUMBER];

	}

	void imgShow(){
		while (true)
		{
			// cout << this->sensorMode << endl;
			if (this->sensorMode == CeleX5::Event_Off_Pixel_Timestamp_Mode)
			{
				// cout << "get buffers when sensor works in EventMode" << endl;
				pCeleX5->getEventPicBuffer(this->pSensorBuffer, CeleX5::EventDenoisedBinaryPic); //event binary pic
				cv::Mat matEventPic(800, 1280, CV_8UC1, this->pSensorBuffer);
				cv::imshow("Event-EventDenoisedBinaryPic", matEventPic);
				cvWaitKey(30);
			}
		}
	}

	pybind11::list getData(){
		//while (true){
			cv::Mat matPic(CELEX5_ROW, CELEX5_COL, CV_8UC1);
			matPic = this->pCeleX5->getEventPicMat(CeleX5::EventDenoisedBinaryPic);
                        //matPic = this->pCeleX5->getEventPicMat(CeleX5::EventGrayPic);

			vector<cv::Mat> dst;
			cv::buildPyramid(matPic, dst, 4);

			pybind11::list res;
			
			res.append<pybind11::array_t<unsigned char>>(cv_mat_uint8_1c_to_numpy(dst.at(0)));

			return res;
			//cv::imshow("Event-EventDenoisedBinaryPic", matPic);
			//cvWaitKey(30);\
			retrun matPic
		//}
	}

        void getXY(){
                        cv::Mat eventPicMat(CELEX5_ROW, CELEX5_COL, CV_8UC1);
			eventPicMat = this->pCeleX5->getEventPicMat(CeleX5::EventDenoisedBinaryPic);
                        cv::Mat XA = eventPicMat.colRange(0,159).clone();
			cv::Mat XB = eventPicMat.colRange(160,319).clone();
			cv::Mat XC = eventPicMat.colRange(320,479).clone();
			cv::Mat XD = eventPicMat.colRange(480,639).clone();
			cv::Mat XE = eventPicMat.colRange(640,799).clone();
			cv::Mat XF = eventPicMat.colRange(800,959).clone();
			cv::Mat XG = eventPicMat.colRange(960,1119).clone();
			cv::Mat XH = eventPicMat.colRange(1120,1279).clone();
			cv::Scalar XAA =cv::sum(XA);
			cv::Scalar XBB =cv::sum(XB);
			cv::Scalar XCC =cv::sum(XC);
			cv::Scalar XDD =cv::sum(XD);
			cv::Scalar XEE =cv::sum(XE);
			cv::Scalar XFF =cv::sum(XF);
			cv::Scalar XGG =cv::sum(XG);
			cv::Scalar XHH =cv::sum(XH);
			std::vector<int> X = {XAA[0], XBB[0], XCC[0], XDD[0], XEE[0],XFF[0],XGG[0],XHH[0]};
			int Index_X = std::max_element(X.begin(),X.end()) - X.begin();
                        
                        cv::Mat YA = eventPicMat.rowRange(0,159).clone();
			cv::Mat YB = eventPicMat.rowRange(160,319).clone();
			cv::Mat YC = eventPicMat.rowRange(320,479).clone();
			cv::Mat YD = eventPicMat.rowRange(480,639).clone();
			cv::Mat YE = eventPicMat.rowRange(640,799).clone();
			cv::Scalar YAA =cv::sum(YA);
			cv::Scalar YBB =cv::sum(YB);
			cv::Scalar YCC =cv::sum(YC);
			cv::Scalar YDD =cv::sum(YD);
			cv::Scalar YEE =cv::sum(YE);
			std::vector<int> Y = {YAA[0], YBB[0], YCC[0], YDD[0], YEE[0]};
			int Index_Y = std::max_element(Y.begin(),Y.end()) - Y.begin();
                        
                        printf("X = %d  Y = %d\n",Index_X,Index_Y);
        }
};

PYBIND11_MODULE(example_pb, m){
    m.doc() = "pybind11 example";
	pybind11::class_<CeleXout>(m, "CeleXout" )
		.def( pybind11::init() )
		.def( "imgShow", &CeleXout::imgShow )  // ""内的是暴露给python的函数名
		.def( "getData", &CeleXout::getData )
                .def( "getXY",&CeleXout::getXY );
}
