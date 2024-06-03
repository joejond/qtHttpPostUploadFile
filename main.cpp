#include "httpupload.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HttpUpload w;
    w.show();
    return a.exec();
}
