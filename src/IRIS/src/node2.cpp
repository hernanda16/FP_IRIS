#include "ros/ros.h"
#include <bits/stdc++.h>
#include "IRIS/BSTX.h"
#include "IRIS/BSRX.h"
#include "std_msgs/Int64.h"
#include <opencv2/highgui/highgui.hpp>
#include <termios.h>
#include <sys/ioctl.h>
#include <cv_bridge/cv_bridge.h>
#define PI 3.141592654
#define RAD 0.0174532925199 //PI/180
#define DEG 57.295779513082 //180/PI
using namespace std;

int basestatus;
int resstats,prevstats,stats,xtu,ytu,temp,prevtuy,prevtux,prevth=0;
int bx,by,vth=10,jarak=100,spn=2;
double posth,posx,posy,vx=10,vy=10,thtemp=-180,thspn;
char prevkeyb,keyb;

ros::Timer init;
ros::Timer proses;
// ros::Timer validate;
ros::Subscriber base;
ros::Subscriber pc;
ros::Publisher upload;
void reset(const ros::TimerEvent &msg);
void loop(const ros::TimerEvent &msg);
// void cek(const ros::TimerEvent &msg);
void rx(const IRIS::BSRX::ConstPtr& msg);
void tx(const IRIS::BSTX::ConstPtr& msg);
void inkey();
uint8_t kbhit();
void status1();
void status2();
void status3();
void status4();

int main(int argc, char** argv){
    posx=0;
    posy=0;
    posth=0;
    ros::init(argc, argv,"node2");
    ros::NodeHandle nh;
    ros::MultiThreadedSpinner ms(0);
    //timer
    init=nh.createTimer(ros::Duration(0.02),reset);
    //subs
    base=nh.subscribe("bs2pc_telemetry",10,rx);
    pc=nh.subscribe("nd",10,tx);
    //timer
    proses=nh.createTimer(ros::Duration(0.02),loop);
    // validate=nh.createTimer(ros::Duration(0.02),cek);
    //pubs
    upload=nh.advertise<IRIS::BSTX>("pc2bs_telemetry",10);
    ms.spin();
}

void reset(const ros::TimerEvent &msg){
        if(resstats!=prevstats){
        IRIS::BSTX reset;
        posx=0;
        posy=0;
        posth=0;
        vx=10;
        vy=10;
        spn=2;
        thtemp=-180;
        reset.pos_x=posx;
        reset.pos_y=posy;
        reset.pos_theta=posth;
        upload.publish(reset);
        // cout<<"sini"<<endl;
        resstats=prevstats;
    }
}

void loop(const ros::TimerEvent &msg){
    switch (basestatus)
        {
        case 1111:
            // inkey();
            status1();
            break;
        case 2222:
            status2();
            break;
        case 3333:
            status3();
            break;
        case 4444:
            status4();
            break;
        default:
            break;
        }

    if(posx>600){
        posx=600;
    }else if(posy>900){
        posy=900;
    }else if(posx<0){
        posx=0;
    }else if(posy<0){
        posy=0;
    }

    IRIS::BSTX isipesan;
    isipesan.pos_x=posx;
    isipesan.pos_y=posy;
    isipesan.pos_theta=posth;
    isipesan.v_x=vx;
    isipesan.v_y=vy;
    isipesan.v_theta=vth;
    // cout<<bx<<by<<endl;
    isipesan.bola_x=bx;
    isipesan.bola_y=by;
    upload.publish(isipesan);
}

// void cek(const ros::TimerEvent &msg){
//     if(basestatus==)
// }

void rx(const IRIS::BSRX::ConstPtr& msg){
    if(prevstats!=msg->status){
        // cout<<msg->status<<endl;
        switch (msg->status)
        {
        case 1:
            cout<<"\nStatus 1\n";
            basestatus=1111;
            break;
        case 2:
            cout<<"\nStatus 2\n";
            basestatus=2222;
            break;
        case 3:
            cout<<"\nStatus 3\n";
            basestatus=3333;
            break;
        case 4:
            cout<<"\nStatus 4\n";
            basestatus=4444;
            break;
        default:
            basestatus=0;
            break;
        }
    }
    stats=msg->status;
    prevstats=msg->status;
    xtu=msg->x_tujuan;
    ytu=msg->y_tujuan;
}

void tx(const IRIS::BSTX::ConstPtr& msg){
    if(basestatus!=1){
        bx=msg->bola_x;
        by=msg->bola_y;
    }
}

void status1(){
    // bx=0;
    // by=0;
    // cout<<prevstats<<" "<<stats<<endl;
    // cout<<"atau ini";
    // if(kbhit()){
    //     // cout<<"ini juga";
    //     keyb=cin.get();
    //     switch (keyb)
    //     {
    //     case 'w':
    //         // cout<<"ini";
    //         posx-=vx;
    //         break;
    //     case 's':
    //         posx+=vx;
    //         break;
    //     case 'd':
    //         posy+=vy;
    //         break;
    //     case 'a':
    //         posy-=vy;
    //         break;
    //     case 'q':
    //         if(posth<180&&posth>=0){
    //                 posth+=vth;
    //             }else if(posth<0&&posth>-180){
    //                 posth+=vth;
    //             }else{
    //                 posth=-180;
    //                 posth+=vth;
    //             }
    //             break;
    //     case 'e':
    //         if(posth<180&&posth>=0){
    //                 posth-=vth;
    //             }else if(posth<0&&posth>-180){
    //                 posth-=vth;
    //             }else{
    //                 posth=180;
    //                 posth-=vth;
    //             }
    //             break;
    //     default:
    //         break;
    //     }
    // }
    
    inkey();
    switch (keyb)
        {
        case 'w':
            // cout<<"ini";
            posx-=vx;
            break;
        case 's':
            posx+=vx;
            break;
        case 'd':
            posy+=vy;
            break;
        case 'a':
            posy-=vy;
            break;
        case 'q':
            if(posth<180&&posth>=0){
                    posth+=vth;
                }else if(posth<0&&posth>-180){
                    posth+=vth;
                }else{
                    posth=-180;
                    posth+=vth;
                }
                break;
        case 'e':
            if(posth<180&&posth>=0){
                    posth-=vth;
                }else if(posth<0&&posth>-180){
                    posth-=vth;
                }else{
                    posth=180;
                    posth-=vth;
                }
                break;
        default:
            break;
        }
}

void status2(){
    static float v_x2=vx*0.5;
    static float v_y2=vy*0.5;
    double sudut=atan2((double)by,(double)bx);
    posth=sudut*DEG;// jadi degree
    if(abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))>=5){
        vx=v_x2*cos(sudut);
        vy=v_y2*sin(sudut);
        posx+=vx;
        posy+=vy;
        cout<<sudut<<" "<<posth<<" "<<posx<<" "<<posy<<endl;
        vx*=2;
        vy*=2;
    }
}

void status3(){
    static float v_x3=vx*0.5;
    static float v_y3=vy*0.5;

    // if(ytu-posy>5){
    //     posy+=vy; 
    // }else if(ytu-posy>5){
    //     posy=ytu;
    // }else if(ytu-posy<-5){
    //     posy-=vy;
    // }else if(ytu-posy<-5){
    //     posy=ytu;
    // }
    
    // if(xtu-posx>5){
    //     posx+=vx; 
    // }else if(xtu-posx>5){
    //     posx=xtu;
    // }else if(xtu-posx<-5){
    //     posx-=vx;
    // }else if(xtu-posx<-5){
    //     posx=xtu;
    // }

    // cout<<prevtux<<" "<<xtu<<" "<<prevtuy<<" "<<ytu<<endl;
    double sudut=atan2((double)ytu-(double)posy,(double)xtu-(double)posx);
    if(prevtux!=xtu||prevtuy!=ytu){
        cout<<sudut*DEG<<endl;
        prevtux=xtu;
        prevtuy=ytu;
        posth=sudut*DEG;
    }
    if(abs(xtu-posx)>0&&abs(ytu-posy)>0){
        if(abs(sqrt(pow(xtu,2)+pow(ytu,2))-sqrt(pow(posx,2)+pow(posy,2)))>=3){
            vx=v_x3*cos(posth*RAD);
            vy=v_y3*sin(posth*RAD);
            posx+=vx;
            posy+=vy;
            vx*=2;
            vy*=2;
        }else{
            posx=xtu;
            posy=ytu;
        }
    }
}

void status4(){
    static float v_x=vx*0.5;
    static float v_y=vy*0.5;

    double sudut=atan2((double)by-(double)posy,(double)bx-(double)posx);
    if(posx==0&&posy==0){
        cout<<sudut*DEG<<" "<<thtemp<<endl;
        posth=sudut*DEG;
        thspn=posth;
    }
    if(abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))>jarak){
        vx=v_x*cos(posth*RAD);
        vy=v_y*sin(posth*RAD);
        posx+=vx;
        posy+=vy;
        vx*=2;
        vy*=2;
    }else if(abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))<jarak+1&&spn>0){
        cout<<posth+thtemp<<endl;
        posx=bx+jarak*cos((thspn+thtemp)*RAD);
        posy=by+jarak*sin((thspn+thtemp)*RAD);
        vx=v_x*cos((thspn+thtemp)*RAD);
        vy=v_y*sin((thspn+thtemp)*RAD);       
        if(thtemp>180){
            thtemp=-179;
            spn--;
        }else{
            thtemp++;
        }
        vth=posth-prevth;
        prevth=posth;
        posth=sudut*DEG;
        vx*=2;
        vy*=2;
    }else if(abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))<jarak+1&&abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))>=2&&spn==0){
        vx=v_x*cos(posth*RAD);
        vy=v_y*sin(posth*RAD);
        posx+=vx;
        posy+=vy;
        vx*=2;
        vy*=2;
    }else if(abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))<3&&spn==0){
        posx=bx;
        posy=by;
    }

    // static int count=0;
    // int cacah=count;
    
    // // double sudut=atan2((double)by,(double)bx);
    // if(count==0){
    //     posth=sudut*DEG;
    //     // cout<<posth<<" "<<"kok gak masuk ";
    // }
    // static float tmp=posth;
    // static int sdt=posth;
    // cout<<tmp<<"   "<<thtemp<<"\n";
    // if(abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))>100&&count==0){
    //     posx+=5*cos(sudut);
    //     posy+=5*sin(sudut);
    // }
    // if(abs(sqrt(pow(bx,2)+pow(by,2))-sqrt(pow(posx,2)+pow(posy,2)))<=101&&count<390){
    //     std::cout<<count<<"  ";
    //     if(count<360){
    //         // vx=vx*cos((thtemp+tmp)*RAD);
    //         // vy=vy*sin((thtemp+tmp)*RAD);
    //         posx=bx+100*cos((thtemp+tmp)*RAD);
    //         posy=by+100*sin((thtemp+tmp)*RAD);
    //         if(thtemp>180){
    //             thtemp=-179;
    //         }else{
    //             thtemp++;
    //         }
    //         if(tmp>180){
    //             tmp=-179;
    //         }else{
    //             tmp++;
    //         }
    //         if(posth>180){
    //             posth=-179;
    //         }else{
    //             posth+=2;
    //         }
                                
    //     }
    //     count++;
    //     // std::cout<<cacah<<"----";
    //     if(abs(sqrt(pow(xtu,2)+pow(ytu,2))-sqrt(pow(posx,2)+pow(posy,2)))>=2&&cacah>=359){
    //         posx+=5*cos((posth)*RAD);
    //         posy+=5*sin((posth)*RAD);
    //     }
    // } 
}

//input keyboard googling
void inkey(){
    static struct termios oldt;
    static struct termios newt;
    tcgetattr( STDIN_FILENO, &oldt);           // save old settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON);                 // disable buffering      
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings
    keyb = getchar();  // read character (non-blocking)
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
}

//input keyboard IRIS
uint8_t kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized)
    {
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}