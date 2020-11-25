# Arduino 超声波魔术手

## 2020.11.25 更新

最近发现这几个小 Demo 还有人在下载，因此重新看了之前传的代码，发现各种注释和格式的不规范，不禁汗颜……
想到下载这个项目的应该都是些初学者，为了不引人『误入歧途』，决定抽空把代码都规范一下。
需要注意的是，这几个项目的教学目标受众是只有微弱 C 基础的人。实际项目中，**注释不应是代码的翻译**。
希望大家学得开心！

## 概要

超声波魔术手程序，主要应用了第三方SR04库，对超声波测距传感器进行操作。实现功能如下：

- 测距结果为50cm以上时，小车停止不动；
- 测距结果为20cm~50cm时，小车前进；
- 测距结果为20cm以下时，小车倒退。

## 接线表

|电机左A|电机左B|电机右A|电机右B| ECHO | TRIG |
|:----:|:----:|:----:|:----:|:----:|:----:|
| D6   |   D11|   D5 |   D3 |   D7 |   D4 |

## 程序讲解

通过调用`SR04.h`文件中配置好的函数，读取超声波传感器测距结果。
首先初始化超声波模块引脚：

```cpp
// 通过宏定义设定超声波模块的绑定引脚
#define TRIG_PIN 6
#define ECHO_PIN 7
// 初始化函数，特定格式，无需特殊记忆
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
// 声明一个长整形变量储存测距结果
long distance;
```

进入循环体后不停获取距离并判断：

```cpp
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
```

另附`speedControl()`函数源代码：

```cpp
#define LEFT 0
#define RIGHT 1
#define FORWARD 2
#define BACK 3
#define SPEED 70

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
```

## 其他

`magic_hand.ino`文件已经上传到项目中，可以打包下载，其中有更详细的注释。
