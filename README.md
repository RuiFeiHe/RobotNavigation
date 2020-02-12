# RobotNavigation

### Augmented A* with feedback control.

Course Project, EE 86120080 Robotics, ZJU, 2019 Fall.

浙江大学 控制学院 机器人学 课程作业



### Results

![image](https://github.com/RuiFeiHe/RobotNavigation/blob/master/RobotNavigation.png)



### Methodology

#### Task 1

Requirement: plan the path between the start and the end that avoids all the obstacles.

Method: use A* algorithm to get a rough solution, and then use Theta* (Augmented version of A*) to ensure the solution is the shortest path.

#### Task2

Requirement:  send message to control the robot and make it move from the start to the end without running into any obstacle.

Method: use advised version of feedback algorithm to let the robot move close to the planned path.