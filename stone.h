#ifndef STONE_H
#define STONE_H

#include<QString>
class stone//棋子类
{
public:
    stone();
    ~stone();

    enum TYPE{JIANG,CHE,PAO,MA,BING,SHI,XIANG};

    int row;
    int col;
    int id;
    bool dead;
    bool red;
    TYPE type;

    QString getText(){
        switch(this->type)
        {   case CHE: return "车";
            case MA: return "马";
            case PAO: return "炮";
            case BING: return "兵";
            case XIANG: return "相";
            case SHI: return "士";
            case JIANG: return "将";
         }

     return "false";
    }

    void init(int x){
        struct{
            int row,col;
            stone::TYPE type;
        }pos[16]={
        {0,0,stone::CHE},
        {0,1,stone::MA},
        {0,2,stone::XIANG},
        {0,3,stone::SHI},
        {0,4,stone::JIANG},
        {0,5,stone::SHI},
        {0,6,stone::XIANG},
        {0,7,stone::MA},
        {0,8,stone::CHE},

        {2,1,stone::PAO},
        {2,7,stone::PAO},
        {3,0,stone::BING},
        {3,2,stone::BING},
        {3,4,stone::BING},
        {3,6,stone::BING},
        {3,8,stone::BING},
    };

         id=x;
        dead=false;//所有棋子一开始都活着
       red=id<16;
        //把0-15的红棋记为真，用bool类型方便判断轮到谁下棋
        if(red)
        {
            row=pos[id].row;
            col=pos[id].col;
            type=pos[id].type;
        }
        else
        {
            row=9-pos[id-16].row;
            col=8-pos[id-16].col;
            type=pos[id-16].type;
        }

        }

};
//找不出bug,就写写注释吧
//该类包含有棋子所在的行，列，编号，是否被吃，红方还是黑方，对应的类型
//该类中gettext函数可以在绘制棋子时输出正确的汉字
//init函数用于初始化棋盘类中的棋子数组
//利用pos数组初始化棋子，图像正确，这里应该没有错误

#endif // STONE_H
