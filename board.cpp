#include "board.h"
#include<QPainter>
#include<QMouseEvent>
board::board(QWidget *parent)
    : QWidget(parent)
{
    for(int i=0;i<32;i++)
        s[i].init(i);//对每个棋子进行初始化

    selected=-1;//还没有已被选的棋子
    bredturn=true;//现在是红方先走
}//构造函数

board::~board()
{

}




//画线
void board::paintEvent(QPaintEvent*){
    QPainter painter(this);
    int d=80;
    r=d/2;
    for(int i=0;i<=10;i++)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    //画十条横线
     }
    for(int i=1;i<=9;i++)
    {
        if(i==1||i==9)
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        else
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
         }
    }//画棋盘的竖线，空出楚河汉界
    painter.drawLine(QPoint(4*d,1*d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,1*d),QPoint(4*d,3*d));

    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    //绘制32颗棋子
    for(int i=0;i<32;i++)
    {
        drawstone(painter,i);
   }
  }

QPoint board::center(int row,int col){
    QPoint ret;
    ret.rx()=(col+1)*r*2;
    ret.ry()=(row+1)*r*2;
    return ret;//根据棋盘的位置找到对应像素点的位置，方便画棋子
}
QPoint board::center(int id){
    return center(s[id].row,s[id].col);
    }//根据棋子的编号找到像素点的位置

//该函数有两个特点，被吃的棋子直接返回，被选中的棋子把画刷设置为灰色
void board::drawstone(QPainter& painter,int id)
{
    if(s[id].dead)
        return;//棋子被吃掉了

    QPoint c=center(id);
    QRect rect=QRect(c.x()-r,c.y()-r,2*r,2*r);//画一个矩形

    if(id==selected)
        painter.setBrush(QBrush(Qt::gray));//选中就是灰色
    else
    painter.setBrush(QBrush(Qt::yellow));//背景设为黄色

    painter.setPen(Qt::black);//画圆形使用黑笔

    painter.drawEllipse(center(id),r,r);

    if(s[id].red) painter.setPen(Qt::red);
    painter.setFont(QFont("system",r,700));

    painter.drawText(rect,s[id].getText(),QTextOption(Qt::AlignCenter));//居中
}



bool board::getRowCol(QPoint pt,int& row,int& col)
{
    for(row=0;row<=9;row++)
        for(col=0;col<=8;col++)
        {
            QPoint c=center(row,col);
            int dx=c.x()-pt.x();
            int dy=c.y()-pt.y();
            int dist=dx*dx+dy*dy;
            if(dist<r*r)
             return true;
            }
    return false;
}//判断点击位置是否有棋子


//找到要去的位置的棋子的编号,自己写的注意检查！！！
int board::getstoneid(int row,int col)
{
    int i;
  for(i=0;i<32;i++)
   { if(s[i].row==row&&s[i].col==col&&s[i].dead==false)
       return i;
   }
     return -1;
}


//服务车炮的函数
//move对应的都是已选所在的位置，row,col,是目标位置的行和列
int board::getcountatline(int move,int row,int col)
{
    int ret=0;
    if(s[move].row!=row&&s[move].col!=col)
        return -1;//车炮不可以斜着走
    if(s[move].row==row&&s[move].col==col)
        return -1;//不可以原地打转呀
   if(s[move].row==row)//在同一行，横着走
   {
        int min=s[move].col<col?s[move].col:col;
        int max=s[move].col<col?col:s[move].col;//这里好像不可以用max函数
        for(int k=min+1;k<max;k++)
        {
            if(getstoneid(row,k)!=-1)
                ret++;
        }
   }
   else
   {
       int min=s[move].row<row?s[move].row:row;
       int max=s[move].row<row?row:s[move].row;
       for(int k=min+1;k<max;k++)
       {
           if(getstoneid(k,col)!=-1)
               ret++;
       }
   }
    return ret;
 }


//将军！！！
bool board::canmove1(int move,int row,int col)
{
    int dr=s[move].row-row;
    int dc=s[move].col-col;
    int d=abs(dr)*10+abs(dc);//步长为一
    if(d!=10&&d!=1)
      return false;

    if(s[move].red&&row>2)return false;
    if(s[move].red==false&&row<7)return false;
    if(col<3)return false;
    if(col>5)return false;
    //将只能移动一步
    return true;
 }


//士！！！
bool board::canmove2(int move,int row,int col)
{
    int dr=s[move].row-row;
    int dc=s[move].col-col;
    int d=abs(dr)*10+abs(dc);//相当巧妙的判断方法诶
    if(d!=11)//士走对角线
        return false;


if(s[move].red&&row>2)return false;
if(s[move].red==false&&row<7)return false;

if(col<3)return false;
if(col>5)return false;//九宫格

return true;
 }


//象！！！
bool board::canmove3(int move,int row,int col )
{
    int dr=s[move].row-row;
    int dc=s[move].col-col;
    int d=abs(dr)*10+abs(dc);
    if(d!=22) return false;//相走田

    //相眼
    int reye=(s[move].row+row)/2;
    int ceye=(s[move].col+col)/2;
    if(getstoneid(reye,ceye)!=-1)return false;

    //象不能过河
    if(s[move].red&&row>4) return false;
    if(s[move].red==false&&row<5)return false;

   return true;
}


//车！！！
bool board::canmove4(int move,int row,int col )
{
    int ret=getcountatline(move,row,col);
    if(ret==0)
        return true;

    return false;
}

//马！！！
bool board::canmove5(int move,int row,int col)
{
    int dr=s[move].row-row;
    int dc=s[move].col-col;
    int d=abs(dr)*10+abs(dc);
    //马走日
    if(d!=12&&d!=21) return false;
    //蹩马脚
    if(d==12)
    {
        if(getstoneid(s[move].row,(s[move].col+col)/2)!=-1)
            return false;
    }
    if(d==21)
    {
        if(getstoneid((s[move].row+row)/2,s[move].col)!=-1)
            return false;
    }
     return true;
}

//炮！！！
bool board::canmove6(int move,int row,int col,int kill )
{
    int ret=getcountatline(move,row,col);
    if(kill!=-1)//去检查一下后面的kill!
    {
        if(ret==1)return true;
    }
    else
    {
        if(ret==0) return true;
    }
    return false;
}

//兵！！！
bool board::canmove7(int move,int row,int col )
{
    int dr=s[move].row-row;
    int dc=s[move].col-col;
    int d=abs(dr)*10+abs(dc);

    if(d!=1&&d!=10)
        return false;
    if(s[move].red)
    {
        if(row<s[move].row)return false;
        if(s[move].row<=4&&s[move].row==row)return false;
        //红方棋子，不可以后退，未过河不可以左右移动
    }
    if(s[move].red==false)
    {
        if(row>s[move].row)return false;
        if(s[move].row>=5&&s[move].row==row)return false;
     }
   return true;
}


//历时一天改对的bug,容易嘛？注意数组越界！！！
bool board::canmove(int move,int row,int col,int kill)
{
    if(kill==-1)
    { switch(s[move].type)
        {
        case stone::JIANG:
        return canmove1(move,row,col);

        case stone::SHI:
          return canmove2(move,row,col);

        case stone::XIANG:
          return canmove3(move,row,col);

        case stone::CHE:
       return  canmove4(move,row,col);

        case stone::MA:
        return  canmove5(move,row,col);

        case stone::PAO:
           return canmove6(move,row,col,kill);

        case stone::BING:
        return canmove7(move,row,col);
         }
    }
    else if(s[move].red==s[kill].red)
       { selected=kill;
        update();
      return false;
    }
    else
    {
        switch(s[move].type)
        {
          case stone::JIANG:
          return canmove1(move,row,col);

          case stone::SHI:
          return canmove2(move,row,col);

          case stone::XIANG:
          return canmove3(move,row,col);

          case stone::CHE:
          return  canmove4(move,row,col);

           case stone::MA:
           return  canmove5(move,row,col);

           case stone::PAO:
          return canmove6(move,row,col,kill);

           case stone::BING:
           return canmove7(move,row,col);
       }
    }
    return true;
}

void board::mouseReleaseEvent(QMouseEvent *ev)
    {
        QPoint pt=ev->pos();
        int row,col;
        bool bRet=getRowCol(pt,row,col);//可以传回，定义的是引用
        if(bRet==0) return;
        int i;
        int clicked=-1;
    for(i=0;i<32;i++)
        {
            if(s[i].row==row&&s[i].col==col&&s[i].dead==false)
                break;
        }//找出点击位置对应的棋子，并且得到行标及列标

        if(i<32)
        {
           clicked=i;
        }//选中的地方没有子为-1.有子则为子的序号

    if(selected==-1)//之前没有选中的棋子
    {
        if(clicked!=-1)
        {
          if(bredturn==s[clicked].red)//轮到我方走棋
            selected=clicked;
         }
    }
    else//之前有选中的棋子，要行棋或者吃子,就不用再考虑该回合能不能行棋了
    {
         //注意在selected！=-1的情况下调用canmove函数
        //row,col对应的都是clicked对应的整点的坐标
        if(canmove(selected,row,col,clicked))
        {
         s[selected].row=row;
         s[selected].col=col;//行棋
         if(clicked!=-1)//吃子
          s[clicked].dead=true;

         selected=-1;//一定要恢复！！！
       bredturn=!bredturn;
        }
    }
    update();
}






