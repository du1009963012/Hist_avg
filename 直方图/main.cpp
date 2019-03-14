#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <mutex>
#include <vector>
using namespace std;
using namespace cv;

HANDLE hMutex = NULL;

DWORD WINAPI Fun(LPVOID lpParamter) {
	for(int i = 0; ; i++) {
		WaitForSingleObject(hMutex,INFINITE);
		cout << "A Thread Fun Display"<<i<<endl;
		Sleep(20);
		ReleaseMutex(hMutex);
	}
	return 0;
}

std::mutex mutex1;
class MyTHread {
public:
	void addThread(vector<Vec3f> lines);
	void mythread(std::vector<Vec3f> lines);

};
void MyTHread::addThread(vector<Vec3f> lines) {
	std::thread t1(&MyTHread::mythread,this,lines);

}
void MyTHread::mythread(vector<Vec3f> lines) {
	
	ofstream outfile;
	outfile.open("1.txt",ios::in|ios::out);
	for(size_t i = 0;i<lines.size();i++)
	{
		Vec3f lin = lines[i];
		stringstream s;
		s << i;
		outfile <<"line:"<< "(x1,y1)=" << "(" <<lin[0] << "," << lin[1] << ")"<<"半径 =("<<lin[2];
	}
	outfile.close();
}
class MyHist {//直方图计算
private:
	Mat src;

public:
	MyHist() {
		
	};
	MyHist(Mat &img) {
		img.copyTo(src);
	};
	~MyHist() {
		delete &src;
	}
	void set(Mat &img) {
		img.copyTo(src);
	}
	Mat get() {
		return src;
	}
	static Mat Hist_avg(Mat &img);
	static Mat ji_suan_hist(Mat &img);
	static vector<Vec4f> HoughLinesP_my(Mat &img);
	static vector<Vec3f> HoughCirces_my(Mat &img);
};
vector<Vec4f> MyHist::HoughLinesP_my(Mat &mat) {
	Mat temp,w;
	vector<Vec4f> lines;
	cvtColor(mat,temp,CV_RGB2GRAY);
	Canny(temp, w,100,250);
	HoughLinesP(w, lines,1,CV_PI/180,10,10,5);
	return lines;
}
vector<Vec3f> MyHist::HoughCirces_my(Mat &mat) {
	Mat temp;
	vector<Vec3f> Circles;
	mat.copyTo(temp);
	medianBlur(temp, temp,5);
	GaussianBlur(temp,temp,Size(5,5),1);
	cvtColor(temp,temp,CV_RGB2GRAY);
	Canny(temp,temp,50,200);
	HoughCircles(temp,Circles,CV_HOUGH_GRADIENT,1,20,100,30,10,20);
	return Circles;
}
Mat MyHist::ji_suan_hist(Mat &mat) {
	Mat temp;
	mat.copyTo(temp);
	return temp;
}
Mat MyHist::Hist_avg(Mat &mat) {
	Mat temp1,temp2,temp3;
	mat.copyTo(temp1);
	cvtColor(temp1, temp2,CV_RGB2GRAY);
	equalizeHist(temp2, temp3);
	return temp3;
}
int main() {
	VideoCapture cap("E:\\MyProject\\Reimu.mp4");
	Mat src,temp;
	MyHist my;
	vector<Vec4i> line_my;
	vector<Vec3f> cire_my;
	MyTHread a;
	HANDLE hThread = CreateThread(NULL,0,Fun,NULL,0,NULL);
	hMutex = CreateMutexA(NULL,FALSE, "screen");
	CloseHandle(hThread);
	cap >> src;
	if (!src.data) {
		cout << "文件为空！";
		return -1;
	}
	int x1 = 1;
	int x2 = 2;
	int x3 = 10;
	int x4 =100;
	int x5 = 100;
	int x6 = 20;
	int x7 = 50;
	namedWindow("src",WINDOW_AUTOSIZE);
	cvCreateTrackbar("x1","src",&x1,1000);
	cvCreateTrackbar("x2", "src", &x2, 1000);
	cvCreateTrackbar("x3", "src", &x3, 1000);
	cvCreateTrackbar("x4", "src", &x4, 1000);
	cvCreateTrackbar("x5", "src", &x5, 1000);
	cvCreateTrackbar("x6", "src", &x6, 1000);
	cvCreateTrackbar("x7", "src", &x7, 1000); 
	int i = 0;
	while (waitKey(10) != 27) {
		i++;
		cap >> src;
	
		Mat dst,can;
		resize(src, temp,src.size()/4);
		cvtColor(temp, dst, CV_RGB2GRAY);
		//equalizeHist(dst,dst);
		Canny(temp, can,x1,x2);
		//HoughCircles(can, cire_my,CV_HOUGH_GRADIENT,1.0,x3*1.0,x4*1.0,x5*1.0,x6,x7);这里有问题注意
	//	a.addThread(cire_my);	
		WaitForSingleObject(hMutex,INFINITE);
		cout << "Main Thread Fun Display" <<(int)cap.get(CV_CAP_PROP_FPS) << "\t\t";
		cout << ((int)cap.get(CV_CAP_PROP_FRAME_COUNT)-i)<<"\t\t";
		cout << cire_my.size()<<endl;
		ReleaseMutex(hMutex);
		imshow("Hist_avg", dst);
		
		imshow("Canny", can);
		
		imshow("yunatu", temp);
	
	}
	
	return 0;
}
