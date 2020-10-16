#include <QUdpSocket>
#include <QApplication>
#include <QThread>
#include <QDebug>
#include <iostream>
#include <fstream>

void trans_fix_content();
void trans_txt();


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    trans_txt();

    return a.exec();
}

void trans_fix_content()
{
    //用户变量定义
    //QHostAddress ip = static_cast<QHostAddress>("127.0.0.1");
    QHostAddress ip = static_cast<QHostAddress>("192.168.1.100");
    quint16 port = 3000;

    //socket
    QUdpSocket socket;
    int send_count = 0; //发送次数计数

    while (1) {

        //报文内容
        QByteArray send_data = "111111111111111111111111111111111111111111111111111";
        send_data.append(std::to_string(send_count).c_str());

        //发送报文
        socket.writeDatagram(send_data, ip, port);

        //调试信息
        qDebug() << "send a message " << send_count++ << Qt::endl;

        QThread::msleep(1000);
    }
}

void trans_txt()
{
    std::string txt_name = "/home/ganyd/Projects/udp_origin_content.txt";
    std::ifstream txt;
    txt.open(txt_name, std::ios::in);

    //用户变量定义
    QHostAddress ip = static_cast<QHostAddress>("192.168.0.102");
    quint16 port = 3000;

    //socket
    QUdpSocket socket;
    int send_count = 0; //发送次数计数

    char buff[2014];
    int count_amessage_len = 0;
    while(1)
    {
        if (txt.eof()) {
            txt.clear();
            txt.seekg(0, std::ios::beg);
            memset(buff, 0, sizeof(buff));
            count_amessage_len = 0;
        }

        txt >> buff[count_amessage_len];
        count_amessage_len++;
        if (buff[count_amessage_len] == '\0'){

            if (count_amessage_len > 72) {
                std::cout << "wh" << std::endl;

                //报文内容
                //QByteArray send_data = "111111111111111111111111111111111111111111111111111";
                QByteArray send_data;
                send_data = QByteArray(buff, 72);//因为buf[]中有`\0`，必须要写上数据长度；否则，数据会直接截断，丢失数据

                //发送报文
                socket.writeDatagram(send_data, ip, port);

                //调试信息
                qDebug() << "send a message " << send_count++ << Qt::endl;

                QThread::msleep(1000);
            }
        }
        else {
            count_amessage_len = 0;
        }
    }
}
