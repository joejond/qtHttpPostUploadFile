#include "httpupload.h"
#include "ui_httpupload.h"

HttpUpload::HttpUpload(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HttpUpload)
{
    ui->setupUi(this);

    ui->progressBar->setValue(0);
    manager = new QNetworkAccessManager(this);

}

HttpUpload::~HttpUpload()
{
    delete ui;
}


void HttpUpload::on_btn_browse_clicked()
{
    qDebug()<< __FUNCTION__;
    fileName = QFileDialog::getOpenFileName(this,tr("Open Document"),
                                            QDir::currentPath(),
                                            "All File (*.*);; JPG (*.jpg *.jpeg)");
    if(!fileName.isNull())
    {
        ui->line_file->setText(fileName);
        infofile = QFileInfo(fileName);

    }

    qDebug()<< infofile.absoluteDir();

    qDebug()<< infofile.absoluteFilePath();

    qDebug()<< infofile.fileName();

    qDebug()<< infofile.size();

}


void HttpUpload::on_btn_upload_clicked()
{
    qDebug() << __FUNCTION__;
    url = QUrl("http://localhost:8080/upload");
    startUpload(url);


}

void HttpUpload::startUpload(QUrl url)
{
    qDebug() << __FUNCTION__;
    file = new QFile(fileName);

    qDebug() << "open File";
    if(!file->open(QIODevice::ReadOnly))
    {
        delete file;
        file = 0;
        return;
    }

    QHttpMultiPart *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;

    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(tr("form-data; name=\"file\"; filename=\"%1\" ").arg(fileName)));
    filePart.setBodyDevice(file);
    file->setParent(multipart);

    multipart->append(filePart);
    qDebug() << "After append multipart";

    QNetworkRequest request(url);
        qDebug() << "url : "<< url;
    reply= manager->post(request,multipart);
    multipart->setParent(reply);
//    qDebug() << "send POST";
    connect(reply, SIGNAL(finished()), this, SLOT(doneUpload()) );
    connect(reply, &QNetworkReply::uploadProgress, this, &HttpUpload::onUploadProgress);
//    qDebug()<< " after connect";
//    qDebug()<<"end of function";



}

void HttpUpload::onUploadProgress(qint64 bytesent, qint64 bytetotal)
{
    qDebug() << __FUNCTION__;
    qreal n = (qreal)bytesent/(qreal)bytetotal;
//       ui->progressBar->setValue(bytesent/bytetotal * 100);
    ui->progressBar->setValue(n*100);

    qDebug() << "Uploaded " << bytesent << " of " << bytetotal << n*100<<"%";

}

//void HttpUpload::onUploadFinished(QNetworkReply *reply)
//{
//    qDebug() << __FUNCTION__;
//    if(!reply->error())
//    {
//        file->close();
//        file->deleteLater();
//        file=0;
//        reply->deleteLater();
//    }

//    QByteArray buffer = reply->readAll();
//    qDebug() << buffer;
//}

void HttpUpload::doneUpload()
{
    qDebug() << __FUNCTION__;
    if(!reply->error())
    {
        file->close();
        file->deleteLater();
        file=0;
        reply->deleteLater();
    }

    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
}


