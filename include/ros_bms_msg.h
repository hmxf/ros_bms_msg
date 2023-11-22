#ifndef ROS_UAV_MSG_H
#define ROS_UAV_MSG_H

#include <ros/ros.h>

#define HEADER    0xEA5		//数据头

typedef                char s8;
typedef unsigned       char u8;
typedef unsigned short int u16;
typedef          short int s16;
typedef unsigned       int u32;
typedef                int s32;

#define RX_BUFFER_SIZE    (13)    /* 串口收数据缓存大小 */

#define SERIAL_PACK_HEAD    (0xA5)

#define SERIAL_NAME "/dev/ttyUSB0"	//usb

struct return_robot_data_s
{
        u8 rx_buffer[RX_BUFFER_SIZE];
};
typedef struct return_robot_data_s return_robot_data_t;

return_robot_data_t return_robot_data;

int state = 1;

u16 voltage ;
s16 current  ;
u8  status ;
u8  temp;
u32 remaining_capacity  ;
u8  error;


void task1_func();
void task2_func();
void task3_func();
void task4_func();
#endif // ROS_UAV_MSG_H
