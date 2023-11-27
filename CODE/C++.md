# C++
面向对象编程
## 类
### 类定义
```c++
class Box
{
    public:
        double length;         // 长度
        double breadth;        // 宽度
        double height;         // 高度
        double getVolume(void);// 返回体积
    private:
};

double Box::getVolume(void)
{
    return length * breadth * height;
}
```
成员函数可以定义在类定义内部，或者单独使用范围解析运算符 :: 来定义。

### 定义类对象

- `Box *pbox1 = new Box(); //使用指针接收` 
- `Box box2;`
- 此种创建方式，使用完后不需要手动释放，该类析构函数会自动执行。而new申请的对象，则只有调用到delete时再会执行析构函数，如果程序退出而没有执行delete则会造成内存泄漏。

三种方法创建类对象
```c++
#include <iostream>  
using namespace std;  
class  Test {   
  private:  
  public:  
      add()
      {
         int x,y,sum;
         x=5;
         y=5;
         sum=x+y;
         cout<<sum<<endl;
     }
 };  
 void main()  
 {  
    Test test1;              //栈中分配，由操作系统进行内存的分配和管理
    Test test2 = Test();     //栈中分配，由操作系统进行内存的分配和管理
    Test *test3=new Test();  //堆中分配，由管理者进行内存的分配和管理，用完必须delete()，否则可能造成内存泄漏
    test1.add();
    test2.add();             //"." 是结构体成员引用
    test3->add();            //"->"是指针引用
    delete(test3);
    system("pause"); 
}
```
- C++在new时的初始化的规律可能为：
对于有构造函数的类，不论有没有括号，都用构造函数进行初始化；如果没有构造函数，则不加括号的new只分配内存空间，不进行内存的初始化，而加了括号的new会在分配内存的同时初始化为0。

### 调用对象的成员函数
```c++
Box myBox;          // 创建一个对象
myBox.getVolume();  // 调用该对象的成员函数
```

### 类访问修饰符
成员和类的默认访问修饰符是 private
```c++
class Base {
 
   public:
  // 公有成员
   protected:
  // 受保护成员
   private:
  // 私有成员
};
```
- public    公有成员在程序中类的外部是可访问的。
- private   私有成员变量或函数在类的外部是不可访问的，甚至是不可查看的。只有类和友元函数可以访问私有成员。
- protected protected（受保护）成员在派生类（即子类）中是可访问的。

### 类的构造函数和析构函数
构造函数的名称与类的名称是完全相同的，并且不会返回任何类型，也不会返回 void。构造函数可用于为某些成员变量设置初始值。
类的析构函数是类的一种特殊的成员函数，它会在每次删除所创建的对象时执行。
析构函数的名称与类的名称是完全相同的，只是在前面加了个波浪号（~）作为前缀，它不会返回任何值，也不能带有任何参数。析构函数有助于在跳出程序（比如关闭文件、释放内存等）前释放资源。


### 友元函数
友元可以是一个函数，该函数被称为友元函数；友元也可以是一个类，该类被称为友元类，在这种情况下，整个类及其所有成员都是友元。
如果要声明函数为一个类的友元，需要在类定义中该函数原型前使用关键字 friend
```c++
class Box
{
   double width;
public:
   friend void printWidth( Box box );
   void setWidth( double wid );
};
 
// 成员函数定义
void Box::setWidth( double wid )
{
    width = wid;
}
 
// 请注意：printWidth() 不是任何类的成员函数
void printWidth( Box box )
{
   /* 因为 printWidth() 是 Box 的友元，它可以直接访问该类的任何成员 */
   cout << "Width of box : " << box.width <<endl;
}
```

### this指针
- this 指针是一个特殊的指针，它指向当前对象的实例。
- 每一个**对象**都能通过 this 指针来访问自己的地址。
- this是一个隐藏的指针，可以在类的成员函数中使用，它可以用来指向调用对象。
- 友元函数没有 this 指针，因为友元不是类的成员，只有成员函数才有 this 指针。

### 类的静态成员
- 静态成员在类的所有对象中是共享的。
- 如果不存在其他的初始化语句，在创建第一个对象时，所有的静态数据都会被初始化为零。
- 不能把静态成员的初始化放置在类的定义中，但是可以在类的外部通过使用范围解析运算符 :: 来重新声明静态变量从而对它进行初始化
```c++
class Box
{
   public:
      static int objectCount;
      // 构造函数定义
      Box(double l=2.0, double b=2.0, double h=2.0)
      {
         cout <<"Constructor called." << endl;
         length = l;
         breadth = b;
         height = h;
         // 每次创建对象时增加 1
         objectCount++;
      }
      double Volume()
      {
         return length * breadth * height;
      }
   private:
};
// 初始化类 Box 的静态成员
int Box::objectCount = 0;
```
#### 静态成员函数
- 如果把函数成员声明为静态的，就可以把函数与类的任何特定对象独立开来。
- 静态成员函数即使在类对象不存在的情况下也能被调用，静态函数只要使用类名加范围解析运算符 :: 就可以访问。
- 静态成员函数只能访问静态成员数据、其他静态成员函数和类外部的其他函数。

##### 静态成员函数与普通成员函数的区别：

- 静态成员函数没有 this 指针，只能访问静态成员（包括静态成员变量和静态成员函数）。
- 普通成员函数有 this 指针，可以访问类中的任意成员；而静态成员函数没有 this 指针。

```c++
class Box
{
   public:
      static int objectCount;
      // 构造函数定义
      Box(double l=2.0, double b=2.0, double h=2.0)
      {
         cout <<"Constructor called." << endl;
         length = l;
         breadth = b;
         height = h;
         // 每次创建对象时增加 1
         objectCount++;
      }
      double Volume()
      {
         return length * breadth * height;
      }
      static int getCount()
      {
         return objectCount;
      }
   private:
      double length;     // 长度
      double breadth;    // 宽度
      double height;     // 高度
};
 
// 初始化类 Box 的静态成员
int Box::objectCount = 0;
 
int main(void)
{
  
   // 在创建对象之前输出对象的总数
   cout << "Inital Stage Count: " << Box::getCount() << endl;
 
   Box Box1(3.3, 1.2, 1.5);    // 声明 box1
 
   // 在创建对象之后输出对象的总数
   cout << "Final Stage Count: " << Box::getCount() << endl;
 
   return 0;
}
```

## 继承
- 当创建一个类时，您不需要重新编写新的数据成员和成员函数，只需指定新建的类继承了一个已有的类的成员即可。
- 这个已有的类称为基类，新建的类称为派生类。

```c++
// 基类
class Animal {
    // eat() 函数
    // sleep() 函数
};


//派生类
class Dog : public Animal {
    // bark() 函数
};
```

### 虚函数
- 定义一个函数为虚函数，不代表函数为不被实现的函数。
定义他为虚函数是为了允许用基类的指针来调用子类的这个函数。

- 当函数被 virtual关键字修饰时，该函数被称为虚函数，虚函数是指一个类中你希望重载的成员函数，当你用一个基类指针或引用指向一个继承类对象的时候，你调用一个虚函数，实际调用的是继承类的版本。

```c++
class A
{
public:
    virtual void foo()
    {
        cout<<"A::foo() is called"<<endl;
    }
};
class B:public A
{
public:
    void foo()
    {
        cout<<"B::foo() is called"<<endl;
    }
};
int main(void)
{
    A *a = new B();
    a->foo();   // 在这里，a虽然是指向A的指针，但是被调用的函数(foo)却是B的!
    return 0;
}
```