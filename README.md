# Kuma BMS ROS Comm Package Instructions

This package is part of AgRobot 2 Project.

This package is only used for Kuma Battery Management System's communication of low-level functions, advanced scheduling are completed by other ROS nodes.

The data corresponding to the ROS Topic `kuma_bms_info` are `Voltage`, `Current`, `Charge State`, `Temperature`, `Remaining Capacity` and `Error State`.

- The unit of `Voltage` is `0.1V`.
- The unit of `Current` is `0.1A`.
- The unit of `Temperature` is `°C`.
- The unit of `Remaining Capacity` is `mAH`.
- The `Charge State` has the following situations:
    - `0`: Normal
    - `1`: Charging
    - `2`: Discharging
