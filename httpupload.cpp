#include "httpupload.h"
#include "ui_httpupload.h"

#define APPLICATION_JSON 0
#define PROGRESS 0

HttpUpload::HttpUpload(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HttpUpload)
{
    ui->setupUi(this);

    ui->txtPass->setEchoMode(QLineEdit::Password);

//    yourLineEdit->setEchoMode(yourCheckBox->checkState() == Qt::Checked ? QLineEdit::Normal : QLineEdit::Password );

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
//    if(!reply->error())
//    {
//        file->close();
//        file->deleteLater();
//        file=0;
//        reply->deleteLater();
//    }

    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
}



void HttpUpload::on_pbSubmit_clicked()
{
    QString user = ui->txtUser->text(); QByteArray baUser = user.toUtf8();
    QString pass = ui->txtPass->text(); QByteArray baPass = pass.toUtf8();

    url = QUrl("http://localhost:3000/post/user");
    QNetworkRequest request(url);

#if APPLICATION_JSON
    //menggunakan application/json  atau application/url-encode
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["user"] = user;
    obj["pass"] = pass;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
//     or
//     QByteArray data("{\"key1\":\"value1\",\"key2\":\"value2\"}");


    reply= manager->post(request,data);

#else

    //menggunakan application/form-data
    QHttpMultiPart *multiform = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart loginUser;
    loginUser.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"user\""));
    loginUser.setBody(baUser);

    QHttpPart loginPass;
    loginPass.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"pass\""));
    loginPass.setBody(baPass);
    multiform->append(loginUser);
    multiform->append(loginPass);
    reply= manager->post(request,multiform);
    multiform->setParent(reply);
#endif

        qDebug() << "url : "<< url;

//    qDebug() << "send POST";
    connect(reply, SIGNAL(finished()), this, SLOT(doneUpload()) );
#if PROGRESS
    connect(reply, &QNetworkReply::uploadProgress, this, &HttpUpload::onUploadProgress);
#endif

}

