#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include"stone.h"
class board : public QWidget
{
    Q_OBJECT
public:
    explicit board(QWidget *parent = nullptr);
    ~board();

     int r;
     stone s[35];//棋盘类里面有32个棋子类
     int selected=-1;
     bool bredturn=true;//轮到谁走了

    //绘制棋盘
    QPoint center(int row,int col);//返回棋盘行列对应的像素位置
    QPoint center(int id);//函数重载
     void drawstone(QPainter&painter,int id);//如果不加引用，传来是painter的拷贝
    void paintEvent(QPaintEvent*);


    //点击鼠标，确定要移动的棋子，和即将移动去的位置
    bool getRowCol(QPoint pt,int& row,int& col);//该点是否对应一个棋子，并且算出
    //对应的行列位置，确定了row和col的值
    void mouseReleaseEvent(QMouseEvent*);


    //是否可以移动的判断,棋盘下方无法走动呐？
     bool canmove(int move,int row,int col,int kill);
     bool canmove1(int move,int row,int col);
     bool canmove2(int move,int row,int col);
     bool canmove3(int move,int row,int col);
     bool canmove4(int move,int row,int col);
     bool canmove5(int move,int row,int col);
     bool canmove6(int move,int row,int col,int kill);
     bool canmove7(int move,int row,int col);

     //在判断是否可以移动时的辅助函数
  int getstoneid(int row,int col);
  //通过棋盘坐标找到对应的棋子去的id,疑惑，棋子动了啊？
  int getcountatline(int move,int row,int col);
  //算算两个隔了几个棋子，用于车炮的前进


signals:

};

#endif // BOARD_H
