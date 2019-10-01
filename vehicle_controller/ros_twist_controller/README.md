# Uploading Arduino code

The Arduino source code can be found in the directory:
```
$ cd ~/[catkin_workspace]/src/vehicle_drive/firmware/darpa_vehicle_arduino/src
$ cat main.cpp
```
Along with the libraries used:
```
$ cd ~/[catkin_workspace]/src/vehicle_drive/firmware/darpa_vehicle_arduino/lib
```
where it contains:
* **Motor**: Handles all low level function of a dual motor setup.
* **Encoder**: Handles the update of encoder ticks that is required for PID control.

## Upload
You can upload the Arduino source code in the usual way, either with your favourite IDE or Arduino IDE.\
Be sure to include the libraries during compilation.


# Starting

Begin vehicle operation with the following commands:
```
$ cd ~/[catkin_workspace]/
$ catkin_make
$ roslaunch vehicle_drive start.launch
```
ROS should begin and a new terminal for teleoperation should show up.