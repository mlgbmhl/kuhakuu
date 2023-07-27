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

    //cmdCode --> 执行函数
    using CoreFun = void (WebCtrlAnalyzeCore::*)(const QJsonObject&, const QString&, ushort) const;
    QMap<QString, CoreFun> mMapCodeToCore;

    //不需要登录即可允许通信的指令码
    QVector<QString> mVcCodesPermitAny;
};

