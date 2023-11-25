#include <ros/ros.h>                      //类似 C 语言的 stdio.h
#include <serial/serial.h>                //ROS已经内置了的串口包
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <signal.h>
#include <sensor_msgs/Imu.h>
#include <ros/duration.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include "ros_bms_msg.h"
#include "ros_bms_msg/bms.h"      //要用到 msg 中定义的数据类型

serial::Serial ser; //声明串口对象

u8 task1_data[13] ={0xA5,0x40,0x90,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D};
u8 task2_data[13] ={0xA5,0x40,0x93,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80};
u8 task3_data[13] ={0xA5,0x40,0x96,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83};
u8 task4_data[13] ={0xA5,0x40,0x98,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x85};
/*************************************************************************/
//当关闭包时调用，关闭
void mySigIntHandler(int sig)
{
   ROS_INFO("close the com serial!\n");
   ser.close();
   ros::shutdown();
}
/*************************************************************************/

void task1_func()
{
    ser.write(task1_data,13);
    // ROS_INFO("task1_write");
}

void task2_func()
{
    ser.write(task2_data,13);
    // ROS_INFO("task2_write");
}

void task3_func()
{
    ser.write(task3_data,13);
    // ROS_INFO("task3_write");
}

void task4_func()
{
    ser.write(task4_data,13);
    // ROS_INFO("task4_write");
}

void timercallback(const ros::TimerEvent&)
{
switch (state)
{
case 1:
    task1_func();
    // usleep(1000);
    break;
case 2 :
    task2_func();
    // usleep(1000);
    break;
case 3:
    task3_func();
    // usleep(1000);
    break;
case 4:
    task4_func();
    // usleep(1000);
    break;
default:
    state = 0;
    break;
}
state++;
// if(state == 4)
// state = 0;
}

int main(int argc,char **argv)
{
   u16 len = 0;
   u8  data[200];
   std::string bms_port;

   ros::init(argc,argv,"ros_bms_talker",ros::init_options::NoSigintHandler);            //解析参数 
   ros_bms_msg::bms bms_msg;
   ros::NodeHandle nh;
   ros::Publisher pub = nh.advertise<ros_bms_msg::bms>("kuma_bms_info",1);            //创建一个publisher对象，发布名为bms_info，队列长度为1，消息类型为ros_bms_msg::bms。
   nh.param<std::string>("bms_port",bms_port,"/dev/ttyUSB1");
   signal(SIGINT, mySigIntHandler);  											//把原来ctrl+c中断函数覆盖掉，把信号槽连接到mySigIntHandler保证关闭节点

   ros::Timer timer1 = nh.createTimer(ros::Duration (1), timercallback,false,true);    //设置定时器每隔1s发送数据

   try
    {
       //设置串口属性，并打开串口
      ser.setPort(bms_port);
      ser.setBaudrate(9600);
      serial::Timeout to = serial::Timeout::simpleTimeout(1000);
      ser.setTimeout(to);
      ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    //检测串口是否已经打开，并给出提示信息
    if(ser.isOpen())
    {
        ser.flushInput();      //清空输入缓存,把多余的无用数据删除
        ROS_INFO_STREAM("Serial Port initialized");
    }
    else
    {
        return -1;
    }
    ros::Rate loop_rate(10);   
    // timer1.start;
    while(ros::ok)
    {
        memset(return_robot_data.rx_buffer,0,sizeof(return_robot_data.rx_buffer));
        ser.read(return_robot_data.rx_buffer,sizeof(return_robot_data.rx_buffer));
        
        u16 TempCheck = 0;
        u8 check = 0;
        char num1 =0 ,num2 =0;
         for(u8 i=0 ; i<sizeof(return_robot_data.rx_buffer)-1; i++)
        {
            TempCheck += return_robot_data.rx_buffer[i];
        }
        check =  TempCheck & 0xff;
        // ROS_INFO("check:%X,tempcheck:%X",check,TempCheck);
        if(return_robot_data.rx_buffer[0]==0xA5 && return_robot_data.rx_buffer[2]==0x90 &&  return_robot_data.rx_buffer[12] == check )
        {
            // ROS_INFO("1");
            voltage = (return_robot_data.rx_buffer[4]<<8) | return_robot_data.rx_buffer[5];
            s16 current_check  = (return_robot_data.rx_buffer[8]<<8) | return_robot_data.rx_buffer[9];
            current = current_check - 30000;
            ROS_INFO("1check:%X,1tempcheck:%X",check,TempCheck);
        }

        if(return_robot_data.rx_buffer[0] == 0xA5 && return_robot_data.rx_buffer[2] == 0x93 &&  return_robot_data.rx_buffer[12] == check )
        {
            // ROS_INFO("2");
            ROS_INFO("2check:%X,2tempcheck:%X",check,TempCheck);
            status = return_robot_data.rx_buffer[4];
            remaining_capacity  = (return_robot_data.rx_buffer[8]<<24) | (return_robot_data.rx_buffer[9]<<16) | (return_robot_data.rx_buffer[10]<<8) | return_robot_data.rx_buffer[11];
        }

        if(return_robot_data.rx_buffer[0] == 0xA5 && return_robot_data.rx_buffer[2] == 0x96 &&  return_robot_data.rx_buffer[12] == check )
        {
            // ROS_INFO("3");
            
            u8 temp_check = return_robot_data.rx_buffer[5]; 
            ROS_INFO("3check:%X,3tempcheck:%X",check,TempCheck);
            temp =  temp_check -40;
        }
          if(return_robot_data.rx_buffer[0] == 0xA5 && return_robot_data.rx_buffer[2] == 0x98 &&  return_robot_data.rx_buffer[12] == check )
        {
            // ROS_INFO("4");
            ROS_INFO("4check:%X,4tempcheck:%X",check,TempCheck);
            if(return_robot_data.rx_buffer[4] != 0)
            {
                error = 1;
            }
           else if(return_robot_data.rx_buffer[5] != 0)
            {
                error = 2;
            }
            else if(return_robot_data.rx_buffer[6] != 0)
            {
                error = 3;
            }
            else if(return_robot_data.rx_buffer[7] != 0)
            {
                error = 4;
            }
            else if(return_robot_data.rx_buffer[8] != 0)
            {
                error = 5;
            }
            else if(return_robot_data.rx_buffer[9] != 0)
            {
                error = 6;
            }
            else if(return_robot_data.rx_buffer[10] != 0)
            {
                error = 7;
            }
            else if(return_robot_data.rx_buffer[11] != 0)
            {
                error = 8;
            }
            else
            {
                error = 0;
            }
        }
        bms_msg.voltage = voltage;
        bms_msg.temp = temp;
        bms_msg.current = current;
        bms_msg.status = status;
        bms_msg.remaining_capacity = remaining_capacity;
        bms_msg.error = error;
        pub.publish(bms_msg);
        ros::spinOnce();      //集中处理本节点回调函数
        loop_rate.sleep();   
    }
    return 0;
}
