#include "urlpixmap.h"
#include <QPixmap>
#include <QIcon>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QDir>


UrlPixmap::UrlPixmap(QNetworkAccessManager *pNetAM, QObject *parent) : QObject(parent)
{
    m_pPixmap = new QPixmap();
    m_pSmallPixmap = new QPixmap();
    m_pNetAM = pNetAM;
}

UrlPixmap::~UrlPixmap()
{
    delete m_pPixmap;
    if (m_pSmallPixmap) delete m_pSmallPixmap;
}

void UrlPixmap::loadFromUrl(QString imageUrl)
{
    connect(
      m_pNetAM, SIGNAL (finished(QNetworkReply*)),
      this, SLOT (fileDownloaded(QNetworkReply*))
      );

    QNetworkRequest request(imageUrl);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    m_pNetAM->get(request);
}

void UrlPixmap::setPixmapData(QByteArray ba)
{
    m_pPixmap->loadFromData(ba);
}

void UrlPixmap::fileDownloaded(QNetworkReply *pReply)
{
    QByteArray baImg = pReply->readAll();
    qDebug() << this;
    m_pPixmap->loadFromData(baImg);
    qDebug() << m_pPixmap->size();
    qDebug() << pReply->url();
    pReply->deleteLater();
    emit downloaded();
}

QPixmap* UrlPixmap::getPixmap()
{
    return m_pPixmap;
}

QPixmap* UrlPixmap::getSmallPixmap(int size)
{
    if (!m_pPixmap->isNull() && m_pSmallPixmap->isNull())
    {
        QPixmap smPM = m_pPixmap->scaledToWidth(size);
         m_pSmallPixmap->swap(smPM);
    }


    return m_pSmallPixmap;
}