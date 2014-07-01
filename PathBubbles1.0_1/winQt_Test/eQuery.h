#ifndef EQUERY_H  
#define EQUERY_H  
  
#include <QObject>  
#include <QNetworkAccessManager>  
#include <QNetworkReply>  
  
class eQuery : public QObject  
{  
    Q_OBJECT  
public:  
    eQuery();  //QObject *parent = 0
    ~eQuery();  
  
    void query(const QString &ip);  
    void query(quint32 ip);  
  
signals:  
    void finished(bool bOK, QString ip, QString area);  
  
protected slots:  
    void onReplyFinished(QNetworkReply *reply);  
  
private:  
    QNetworkAccessManager m_nam;  
    QString m_emptyString;  
};  
#endif  