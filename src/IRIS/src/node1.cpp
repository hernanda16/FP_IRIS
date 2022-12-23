#include "ros/ros.h"
#include <bits/stdc++.h>
#include "IRIS/BSTX.h"
#include "IRIS/BSRX.h"
#include "std_msgs/Int64.h"
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#define PI 3.141592654

class data{
    public:
    cv::Point2f center;
    float bolax=0,bolay=0,posx=0,posy=0,thet=0,vx=5,vy=5,vt=10;
    int stat,tux,tuy,temp=-180,count=0;
};

data isi;
ros::Publisher inipublisher;

int main(int argc,char** argv){
    // std::cout<<"halo"<<std::endl;
    cv::Mat img=cv::imread("/home/her/Documents/cobafp/src/IRIS/img/Bola2.jpeg");
    cv::Mat resized;
    cv::Mat color;
    cv::Mat bola;
    cv::Mat ebola;
    cv::Mat dbola;

    cv::resize(img,resized,cv::Size(900,600),cv::INTER_LINEAR);
    cv::cvtColor(resized,color,cv::COLOR_BGR2HSV);
    cv::inRange(color,cv::Scalar(0,100,12),cv::Scalar(15,255,255),bola);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10,6), cv::Point(-1,-1));//erode dilate dengan pendekatan bentuk elips
    cv::erode(bola,ebola,element,cv::Point(-1,-1),1);
    cv::dilate(ebola,dbola,element,cv::Point(-1,-1),1);
    
    std::vector<std::vector<cv::Point>> contoursb;
    std::vector<cv::Vec4i> hierarchyb;

    float rad;

    cv::findContours(dbola, contoursb, hierarchyb, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::minEnclosingCircle(contoursb[0],isi.center,rad);

    ros::init(argc, argv, "node1");//node dengan nama node1
    ros::NodeHandle nh;

    isi.bolax=isi.center.x;
    isi.bolay=isi.center.y;

    inipublisher = nh.advertise<IRIS::BSTX>("nd", 1000);
    IRIS::BSTX isipesan;
    ros::Rate loop_rate(10);
    while(ros::ok()){
        isipesan.bola_x = isi.bolay;
        isipesan.bola_y = isi.bolax;
        inipublisher.publish(isipesan);
        // std::cout<<isi.bolax<<isi.bolay<<std::endl;
    }
    ros::spinOnce();
    loop_rate.sleep();
}   