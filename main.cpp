#include<QApplication>
#include"board.h"
int main(int argc,char*argv[]){
    QApplication app( argc,argv);
    board board;//定义了对象
    board.show();
    return app.exec();


}
