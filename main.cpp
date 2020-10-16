#include <QUdpSocket>
#include <QApplication>
#include <QThread>
#include <QDebug>
#include <iostream>
#include <fstream>

void trans_fix_content();
void trans_txt(std::string txt_path, QHostAddress ip_ = static_cast<QHostAddress>("192.168.1.100"));

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //只需要在这里修改数据文件地址和ip就行
    trans_txt("/home/ganyd/Projects/udp_origin_content.txt",
              static_cast<QHostAddress>("192.168.1.103"));

    return a.exec();
}

void trans_txt(std::string txt_path, QHostAddress ip_) //默认端口3000
{
    std::string txt_name = txt_path;
    std::ifstream txt;
    txt.open(txt_name, std::ios::in);

    //用户变量定义
    QHostAddress ip = ip_;
    quint16 port = 3000;

    //socket
    QUdpSocket socket;
    int send_count = 0; //发送次数计数

    unsigned char buff[2014];
    int count_amessage_len = 0;
    while(1)
    {
        if (txt.eof()) { //如果文件结束，重新读，无限循环该文件
            txt.clear();
            txt.seekg(0, std::ios::beg);
            memset(buff, 0, sizeof(buff));
            count_amessage_len = 0;
        }

        txt >> buff[count_amessage_len]; //读一个字节
        count_amessage_len++;
        if (buff[0] == 0xE7 && buff[count_amessage_len] == '\0' && count_amessage_len > 72){
            //报文内容
            QByteArray send_data;
            send_data = QByteArray(buff, 72);

            //发送报文
            socket.writeDatagram(send_data, ip, port);

            //调试信息
            send_count++;
            //qDebug() << "send a message " << send_count << Qt::endl;

            //发送频率
            QThread::msleep(100);
        }
        else {
            count_amessage_len = 0;
        }
    }
}

void trans_fix_content()
{
    //用户变量定义
    QHostAddress ip = static_cast<QHostAddress>("192.168.1.100");
    quint16 port = 3000;

    //socket
    QUdpSocket socket;
    int send_count = 0; //发送次数计数

    while (1) {

        //报文内容
        QByteArray send_data = "tmp-";
        send_data.append(std::to_string(send_count).c_str());

        //发送报文
        socket.writeDatagram(send_data, ip, port);

        //调试信息
        qDebug() << "send a message " << send_count++ << Qt::endl;

        QThread::msleep(1000);
    }
}
