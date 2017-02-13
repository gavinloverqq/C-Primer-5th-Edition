#include <iostream>
#include <vector>

using namespace std;
//使用 ! 表示重要程度,! 越多越重要
//便于查看输出结果的分割显示函数
void pIndexofTest(int i){
    cout << endl
    << endl
    << "Test Rounds "
    << i
    << endl
    << "--------------------------------------------"
    << endl;
}


class TestA{

    friend istream& read(istream& is,TestA& a);

    //    B类可以访问A类私有成员
    friend class TestB;

public:
    TestA() = default;
    TestA(int i,int j):iVal1(i),iVal2(j){}
    TestA(istream& is){
        read(is,*this);
    }

private:
    int iVal1,iVal2;
};

istream& read(istream& is,TestA& a){
    is >> a.iVal1 >> a.iVal2;
    return is;
}

class TestB;
class TestD{
public:
    TestD() = default;
    TestD(int i,int j):iVal1(i),iVal2(j){}
    void DprintB(TestB b);//不可再此处写函数实现

private:
    int iVal1,iVal2;
};
class TestB{
    friend class TestC;//友元关系的传递

    friend void TestD::DprintB(TestB);//只让成员函数作为友元!!! (仔细看这个地方的实现,看看上面定义的TestD,以及这个函数声明,和实现的位置,尝试不这样做会发生什么)

public:
    TestB() = default;
    TestB(double i, double j):dVal1(i),dVal2(j){}

    void getAval(TestA a){
        cout << a.iVal1 << " " << a.iVal2 << endl;
    }
    void printB(){
        cout << dVal1 << " " << dVal2 << endl;
    }
private:
    double dVal1,dVal2;
};

class TestC{

public:
//    void getAval(TestA a){
//        cout << a.iVal1 << " " << a.iVal2 << endl;//B是A的友元类,C是B的友元类,但是A不是C的友元类,友元关系不可传递
//    }

};


void TestD::DprintB(TestB b){
    cout << b.dVal1 << " " << b.dVal2 << endl;
}


//  !!! 友元函数的声明的作用仅仅是影响访问权限,它本身并非普通意义上的声明。
class C1{
    friend void f(){
        cout << "c1" << endl;
    }

public:
    C1() = default;
    C1(int i):iVal1(i){}

    void G();
    void H();
private:
    int iVal1;
};
//下面的代码会发生错误,error: use of undeclared identifier 'f',f为声明!!!!
//void C1::G(){
//    f();
//}
void f();
void C1::H() {
    f();
}

//  ! 类的作用域(注意下面一种情况,返回类型是类内定义的)
class C2{
public:
    using typeT = std::string::size_type ;
    typeT func();
};
C2::typeT C2::func(){
    cout << " xxx " << endl;
}
//第二个C2省略也可以,书上说不行????????????????????

//! 名字查找与类的作用域:编译成员的声明，直到类全部可见后才编译函数体
typedef double Money;
string bal;
class Account{
public:
    Money balance(){//编译器到此处会向前找money的声明，直到找到函数外面的typedef
        return bal;//函数体在整个类可见后才被处理，此处的bal是类的成员，而不是string
    }

private:
    Money bal;
};


typedef double Money;
class Account2{
public:
    Money balance(){//编译器到此处会向前找money的声明，直到找到函数外面的typedef
        return bal;//函数体在整个类可见后才被处理，此处的bal是类的成员，而不是string
    }

private:
//    typedef double Money;//错误，在类中，如果成员使用了外层作用域中的某个名字，而该名字代表一种类型，则类中不能重新定义改名字。！！！ 但是我编译并没出错
    Money bal;
};
//类型名的定义通常出现在类的开始处，这样就能确保所有使用该类型的成员都出现在类名定义后

//!! 普通快的作用域的名字查找
int i = 1000;
class C3{
public:
    void dummyFunc(int i = 10){
        cout << i << endl;
    }
    void dummyFunc2(int i = 10){
        cout << ::i << endl;
    }
    void dummyFunc3(int i = 10);//此时func不可见

private:
    int i = 3;
};
void func(){
    cout << __func__ << endl;
}
void C3::dummyFunc3(int i){//此时func可见
    func();//
}


class C4{
public:
//!!! 注意下面的构造函数的区别，第一个构造函数采用赋值的方式会报错，第二个初始化列表的函数可以！！！
//    C4(int ii){
//        iValCnt = ii;
//        iVal = ii;
//        iRef = i;
//    }
    C4(int ii):iVal(ii),iValCnt(ii),iRef(i){}

private:
    int iVal;
    const int iValCnt;
    int& iRef;
};

//!!! 初始化顺序（与定义的顺序相同）
class C5{
    int i;
    int j;
public:
    C5(int iVal):j(iVal++),i(j){}//先初始化i，因为j还未被初始化，采用的默认初始化，因此i = j = 0，接着用ival初始化j
    void print(){//注意看此处后面例子的输出
        cout << i << " " << j << endl;
    }
};

//!!! cin接受istream&的参数的构造函数
class C6{
    friend std::istream& operator >> (std::istream& is,C6& c){//注意此处的参数，第二个必须传入引用，否则无法修改对象的值
        is >> c.i >> c.j;
        return is;
    }
    friend std::ostream& operator << (std::ostream& os,const C6& c){//此处引用的使用，也值的考虑，不使用引用则会拷贝一个类的副本，使用的话没有拷贝
        os << c.i << " " << c.j;
        return os;
    }

private:
    int i,j;
public:
    C6(std::istream& is = std::cin){
        is >> *this;
    }
};

int main(){


//    ! 友元类
    pIndexofTest(1);
    {
        TestA a(cin);
        TestB b(1.2,3.333);
        b.getAval(a);
    }

//    !!! 成员函数作为友元(注意此处的实现!!!!!)
    pIndexofTest(2);
    {
        TestB b(1.2222,2.2222);
        TestD d;
        d.DprintB(b);
    }

//    !!! 普通快的作用域的名字查找,先在成员函数内查找，只有在函数使用之前出现的声明才被考虑，如果在函数内没有找到，则在类内继续查找，这时类的所有成员都可以被考虑。如果类内也没找到，在成员函数定义之前的作用域内据徐查找
    pIndexofTest(3);
    {
        C3 c3;
        c3.dummyFunc();//10
        c3.dummyFunc2();//1000
        c3.dummyFunc3();//
    }

//    !!! 必须提供构造函数! 需要通过初始值列表来提供初值。先初始化，后赋值，因为有些数据成员必须被初始化
    pIndexofTest(4);
    {
        C4 c4(1);
    }

//    !! 初始化顺序与定义的顺序有关，与出生计划列表无关
    pIndexofTest(5);
    {
        C5 c5(100);
        c5.print();// 0 100,
    }

    pIndexofTest(6);
    {
        C6 c6(cin);
        cout << c6 << endl;
    }
    return 0;
}
