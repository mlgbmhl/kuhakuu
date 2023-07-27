#pragma once

#include <QObject>
#include <QJsonObject>
#include <QString>
#include "ProtocolWebCtrl.h"


class WebCtrlAnalyzeCore : public QObject
{
    Q_OBJECT;

public:
    WebCtrlAnalyzeCore(QObject *parent = NULL);
    ~WebCtrlAnalyzeCore();

    void ntHearbeat(const QJsonObject& job, const QString& ip, ushort port) const;
    
    void lgSetLogin(const QJsonObject& job, const QString& ip, ushort port) const;
    void lgSetUserKeyMod(const QJsonObject& job, const QString& ip, ushort port) const;
    void lgSetKeyForg(const QJsonObject& job, const QString& ip, ushort port) const;
    void lgGetLoginStat(const QJsonObject& job, const QString& ip, ushort port) const;
    
    void acSetPlayFile(const QJsonObject& job, const QString& ip, ushort port) const;
    void acSetPlay(const QJsonObject& job, const QString& ip, ushort port) const;
    void acSetMute(const QJsonObject& job, const QString& ip, ushort port) const;
    void acSetVolume(const QJsonObject& job, const QString& ip, ushort port) const;
    void acSetLoop(const QJsonObject& job, const QString& ip, ushort port) const;
    void acSetDelFile(const QJsonObject& job, const QString& ip, ushort port) const;
    void acSetAttack(const QJsonObject& job, const QString& ip, ushort port) const;
    void acGetFileNum(const QJsonObject& job, const QString& ip, ushort port) const;
    void acGetFileList(const QJsonObject& job, const QString& ip, ushort port) const;
    void acGetFilePlaying(const QJsonObject& job, const QString& ip, ushort port) const;
    void acGetDevStat(const QJsonObject& job, const QString& ip, ushort port) const;
    void acGetAttackStat(const QJsonObject& job, const QString& ip, ushort port) const;

    void rtSetStop(const QJsonObject& job, const QString& ip, ushort port) const;
    void rtSetDirecTurn(const QJsonObject& job, const QString& ip, ushort port) const;
    void rtSetVel(const QJsonObject& job, const QString& ip, ushort port) const;
    void rtSetResident(const QJsonObject& job, const QString& ip, ushort port) const;
    void rtSetZero(const QJsonObject& job, const QString& ip, ushort port) const;
    void rtGetStat(const QJsonObject& job, const QString& ip, ushort port) const;

    void lsSetAttack(const QJsonObject& job, const QString& ip, ushort port) const;
    void lsGetStat(const QJsonObject& job, const QString& ip, ushort port) const;

    void coSetWorkModel(const QJsonObject& job, const QString& ip, ushort port) const;
    void coGetWorkModel(const QJsonObject& job, const QString& ip, ushort port) const;

private:
    using Key = PrtcWebCtrlJsonKey;
    using Val = PrtcWebCtrlJsonValue;

    void logLackKey(const QString& ip, ushort port) const;
    void logErrorPrmType(const QString& ip, ushort port) const;
    void logErrorPrmValue(const QString& ip, ushort port) const;
    void log(QString slog, const QString& ip, ushort port) const;
};

