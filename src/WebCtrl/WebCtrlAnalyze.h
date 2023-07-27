#pragma once

#include <QByteArray>
#include <QString>
#include <QMap>
#include <QVector>
#include "WebCtrlAnalyzeCore.h"

#define gAnalyzeWebCtrl    WebCtrlAnalyze::GetInstance()


class WebCtrlAnalyze
{
private:
    WebCtrlAnalyze();

public:
    ~WebCtrlAnalyze();
    static WebCtrlAnalyze* GetInstance();

    void append(const QByteArray& data,
        const QString& ipSrc, const ushort& portSrc);

private:
    void preAnalyze(const QByteArray& data, 
        const QString& ipSrc, const ushort& portSrc);

    void init();

private:
    WebCtrlAnalyzeCore *mCore;

    //cmdCode --> ִ�к���
    using CoreFun = void (WebCtrlAnalyzeCore::*)(const QJsonObject&, const QString&, ushort) const;
    QMap<QString, CoreFun> mMapCodeToCore;

    //����Ҫ��¼��������ͨ�ŵ�ָ����
    QVector<QString> mVcCodesPermitAny;
};

