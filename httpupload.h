#ifndef HTTPUPLOAD_H
#define HTTPUPLOAD_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class HttpUpload; }
QT_END_NAMESPACE

class HttpUpload : public QMainWindow
{
    Q_OBJECT

public:
    HttpUpload(QWidget *parent = nullptr);
    ~HttpUpload();

    void startUpload(QUrl url);

private slots:
    void on_btn_browse_clicked();

    void on_btn_upload_clicked();
    void onUploadProgress(qint64 bytesent, qint64 bytetotal);
//    void onUploadFinished(QNetworkReply *reply);
    void doneUpload();

private:
    Ui::HttpUpload *ui;
    QString fileName;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFileInfo infofile;


    QUrl url;
    QFile *file;

};
#endif // HTTPUPLOAD_H
