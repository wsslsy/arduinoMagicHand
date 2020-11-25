// 首先通过声明调用超声波类库
#include "SR04.h"

// 这是一些有助于程序可读性的宏定义
// 可以忽略它们，仔细读程序你就会明白他们的用途
#define LEFT 0
#define RIGHT 1
#define FORWARD 2
#define BACK 3

// 定义超声波传感器的几个引脚
#define TRIG_PIN 4
#define ECHO_PIN 7

// 定义驱动电机的速度，255为最高，0为最低。修改该数值会修改速度。
#define SPEED 70

// 定义小车驱动电机的几个引脚，要注意的是，这里只有3、5、9、10、11等引脚可以输出PWM。
int carLeftA = 6;
int carLeftB = 11;
int carRightA = 5;
int carRightB = 3;

// 定义储存超声波测得距离的变量
long distance;

// 调用超声波初始化函数，特定语句，不需记忆，用的时候查看示例程序即可。
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
void setup()
{
  // 配置小车电机驱动的几个引脚
  // 需要注意的是，虽然使用到了超声波的几个引脚
  // 但是由于超声波库中已经对其进行了初始化，所以不用重复配置了。
  pinMode(carLeftA, OUTPUT);
  pinMode(carLeftB, OUTPUT);
  pinMode(carRightA, OUTPUT);
  pinMode(carRightB, OUTPUT);
}

// 循环程序，不停的执行
void loop()
{
  // 首先使用测得超声波距离。
  distance = sr04.Distance();
  // 之后对距离进行判断。
  if (distance <= 20)
  {
    // 如果距离小于20CM
    // 调用下面的speedControl函数，配置两个电机的运行方向。
    speedControl(RIGHT, BACK);
    speedControl(LEFT, BACK);
  }
  else if (20 < distance && distance < 50)
  {
    // 如果距离在20～50CM，就前进。
    speedControl(RIGHT, FORWARD);
    speedControl(LEFT, FORWARD);
  }
  else if (distance >= 50)
  {
    // 如果距离大于50，就停车。
    analogWrite(carLeftA, 0);
    analogWrite(carLeftB, 0);
    analogWrite(carRightA, 0);
    analogWrite(carRightB, 0);
  }
  // 接着执行下一次循环。
}

void speedControl(int side, int dir)
{
  // 这是一个便于程序读懂的电机控制函数
  // 它有两个参数，第一个是电机的选择，第二个是运行的方向。
  // 当然，也可以把这段程序中的一些语句移动到loop中，实现相同的效果。
  if (LEFT == side)
  {
    // 首先判断要控制左右哪个电机
    if (FORWARD == dir)
    {
      // 再判断要这个电机向哪个方向运行。
      // 这里用到了模拟输出，它的本质就是输出PWM。
      analogWrite(carLeftA, SPEED);
      analogWrite(carLeftB, 0);
    }
    else if (BACK == dir)
    {
      analogWrite(carLeftA, 0);
      analogWrite(carLeftB, SPEED);
    }
  }
  else if (RIGHT == side)
  {
    if (FORWARD == dir)
    {
      analogWrite(carRightA, 0);
      analogWrite(carRightB, SPEED);
    }
    else if (BACK == dir)
    {
      analogWrite(carRightA, SPEED);
      analogWrite(carRightB, 0);
    }
  }
}
