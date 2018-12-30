#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<sensor_msgs/Joy.h>

//Global variables to store the information of the sensor msgs outside the scope of the callback function.
float thumb[6];
int keys[17];
float vel_mult = 1;
bool newMessage = false;
ros::Publisher turtle_pub;

//Define functions name and input.
void joy_callback(const sensor_msgs::Joy::ConstPtr msg);
void drive(float lin, float ang);
void circle(); //Function makes the bot run in a circle.
void triangle(); //Function makes the bot run in a triangle.
void square(); //Function makes the bot run in a square.
void cross(); //Function makes the bot run in a Cross.

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "teleop_node");
  ros::NodeHandle nh;
  //The subscriber is defined to store the information in the sensor_msgs::Joy format and it looks for the information on the /joy topic.
  ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("/joy", 1, joy_callback); //Queue of 1 and the callback loop runs everytime we get new info.
  //The Publisher is defined to advertise/publish information of the geometry_msgs::Twist format to the /cmd_vel_mux/input/teleop topic. Queue of 10.
  turtle_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 10);
  //We just print init done.
  std::cout << "init done" << std::endl;

  std::cout << "You can now control your turtlebot!" << std::endl;
  std::cout << "Press select to toggle joystick control" << std::endl;
  std::cout << "Or use one of the four geometric buttons to make the turtle draw a shape!" << std::endl;


  //Manual is gonna be a condition we use to switch manual control on and off.
  int manual = 0;

  // The while loop runs at 25HZ
  ros::Rate r(25);
  while (ros::ok()){
  //######################################MANUAL SWITCH##############################
    if(newMessage == true){

      //The 9th index is the select key. We use that to switch manual control on/off.
      if(keys[8] == 1 && manual == 0){
        std::cout << "Manual control on." << std::endl;
        manual = 1;
      }
      else if(keys[8] == 1 && manual == 1){
        std::cout << "Manual control off." << std::endl;
        manual = 0;
      }
  //#################################################################################
      if(!manual)
      {
        // The first index of keys is the "X" button.
        if(keys[0] == 1){
          std::cout << "You pressed X" << std::endl;
            cross();
        }
        // The second index of keys is the circle button.
        if(keys[1] == 1){
          std::cout << "You pressed O" << std::endl;
             circle();
        }
        // The third index of keys is the triangle button.
        if(keys[2] == 1){
          std::cout << "You pressed Triangle" << std::endl;
            triangle();
        }
        // The fourth index of keys is the sqaure button.
        if(keys[3] == 1){
          std::cout << "You pressed Square" << std::endl;
            square();
        }
      }


//############################  SPEED MULTIPLIER ##########################################

      if (keys[13] == 1 && vel_mult < 2) { //Adds 0.2 to the velocity multiplier. 
        vel_mult = vel_mult + 0.2;
        std::cout << "Speed is " << vel_mult << std::endl;
      }

      if (keys[14] == 1 && vel_mult > 0.4) { //Checks that the key is pressed and vel_mult is greater than 0.4
        vel_mult = vel_mult - 0.2; //Subtracts 0.2 to the velocity multiplier.
        std::cout << "Speed is " << vel_mult << std::endl;
      }

//########################################################################################

      newMessage = false;
    }
//######################### END OF NEWMESSAGE BLOCK ###################################

//########################### Manual control on, code #######################################
    if (manual == 1){
      //Calls the drive function with the linear to be up an down on left thumbstick. And left and right for the second input.
      drive(thumb[1]/5, thumb[0]); //Must be placed outside newMessage so that the robot will keep running at whatever speed the thumb is held at.
    }
//#############################################################################################
    ros::spinOnce();
    r.sleep();
  }
  return 0;
}

//#################################### DEFINE FUNCTIONS #########################################
void drive(float lin, float ang){
  geometry_msgs::Twist vel_msg;
  //Our linear speed control looks
  vel_msg.linear.x = lin*vel_mult;
  vel_msg.angular.z = ang*vel_mult;
  //Publishes the geometry to the turtle_pub
  turtle_pub.publish(vel_msg);
}

void joy_callback(const sensor_msgs::Joy::ConstPtr msg){
  // For loop maps the axes globally in the thumb array. The syntax -> is used instead of . beacuse it is a pointer.
  for (int i = 0; i < 6; i++){
    thumb[i] = msg->axes[i];
  }
  // For loop maps the buttons array globally into our own array called keys.
  for (int i = 0; i < 17; i++){
    keys[i] = msg->buttons[i];
  }
  newMessage = true;
}

//Velocity multipler is set to one in all shape functions as if it is changing the forms won't be correctly drawn.

void circle(){
  vel_mult = 1;
  //Makes the robot run 5 times with speed of 0.2 and turn rate of 1. This makes it run in a circle 1 time.
  for (size_t i = 0; i < 15; i++) {
    drive(0.2, 1);
    ros::Duration(0.5).sleep();
  }
}

void triangle(){
  vel_mult = 1;
  for (size_t i = 0; i < 3; i++) {
    //Makes the robot drive in 1/3 of a triangle and for loop runs this 3 times.
    drive(0.2,0);
    ros::Duration(0.5).sleep();
    drive(0.2,0);
    ros::Duration(0.5).sleep();
    drive(0.2,0);
    ros::Duration(0.5).sleep();
    drive(0,1.05);
    ros::Duration(0.5).sleep();
    drive(0,1.05);
    ros::Duration(0.5).sleep();
    drive(0,1.05);
    ros::Duration(0.5).sleep();
    drive(0,1.05);
    ros::Duration(0.5).sleep();
    drive(0,1.05);
    ros::Duration(0.5).sleep();
  }
}

void cross(){
  vel_mult = 1;
  drive(0.2,0);
  ros::Duration(0.5).sleep();
  drive(0.2,0);
  ros::Duration(0.5).sleep();
  drive(-0.2,0);
  ros::Duration(0.5).sleep();
  drive(0,1.57);
  ros::Duration(0.5).sleep();
  drive(0.2,0);
  ros::Duration(0.5).sleep();
  drive(-0.2,0);
  ros::Duration(0.5).sleep();
  drive(-0.2,0);
  ros::Duration(0.5).sleep();
}

void square(){
  vel_mult = 1;
 for (size_t i = 0; i < 4; i++) {
   drive(0.2,0);
   ros::Duration(0.5).sleep();
   drive(0,1.57);
   ros::Duration(0.5).sleep();
 }
}
