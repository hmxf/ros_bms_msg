# Version Log

源项目地址：https://github.com/dslab-agrobot/AgRobot2

本项目地址：https://github.com/hmxf/ros_bms_msg

工作内容：

- 添加了 Kuma ROS 通讯包，可以通过 `/dev/USBtty` 接口直接通过该项目获取电池包的详细信息。概要信息在 [ros_ht_msg](https://github.com/hmxf/ros_ht_msg) 项目中就可以读出，但由于概要信息的数据来源是 `ros_ht_msg` 读取系统电压得出的，因此并不具有如本项目一样的详细程度。
