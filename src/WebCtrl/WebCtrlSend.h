#pragma once

#include <QObject>
#include "WebCtrlPack.h"

#define gWebCtrlSend    WebCtrlSend::GetInstance()

class WebCtrlSend : public QObject
{
    Q_OBJECT;

private:
    WebCtrlSend();

public:
    ~WebCtrlSend();
    static WebCtrlSend* GetInstance();

    //通用应答
    void response(const QString& cmdCode, const QString& ip, ushort port) const;

    //心跳应答
    void repNtNetHand(ushort heartbeat, const QString& ip, ushort port) const;

    //登录管理
    //应答：用户登录
    void repLgLoginRequst(bool succ, const QString& ip, ushort port) const;

    //应答：修改用户名和密码
    void repLgUserNamePasswordMod(bool succ, const QString& ip, ushort port) const;

    //应答：忘记用户名和密码
    void repLgUserNamePasswordForg(bool succ, const QString& ip, ushort port) const;

    //应答：登录状态 
    void repLgLoginStat(bool succ, const QString& ip, ushort port) const;
    //自动上报：登录状态 
    void autoRepLgLoginStat() const;



    //声能设备管理
    //应答：删除指令文件的曲目：当succ为false时，必须正确填写错误码
    //1-文件名错误；2-文件正在被占用； 3-其他错误
    void repAcDelFile(WebCtrlPack::DelFileReslt resualt, const QString& ip, ushort port) const;

    //应答：曲目总数
    void repAcFileNum(bool succ, const QString& ip, ushort port) const;

    //应答：曲目列表
    void repAcFileList(bool succ, const QString& ip, ushort port) const;

    //应答：当前曲目名
    void repAcFileCurrent(bool succ,  const QString& ip, ushort port) const;
    //特定条件自动上报：当前曲目名
    void autoRepAcFileCurrent() const;

    //应答：声能设备状态
    void repAcStat(bool succ, const QString& ip, ushort port) const;
    //特定条件自动上报：声能设备状态
    void autoRepAcStat() const;


    //转台设备管理
    //应答：转台状态
    void repRtStat(bool succ, const QString& ip, ushort port) const;
    //特定条件自动上报：转台状态
    void autoRepRtStat() const;


    //强光设备管理
    //应答： 强光设备状态
    void repLsStat(bool succ, const QString& ip, ushort port) const;
    //特定条件自动上报： 强光设备状态
    void autoRepLsStat() const;


    //综合管理
    //应答：工作模式
    void repCoWorkModel(bool succ, const QString& ip, ushort port) const;

private:
    bool send(const QByteArray& data, const QString& ip, ushort port) const;
    bool sendAll(const QByteArray& data) const;
    void log(QString slog, const QString& ip, ushort port) const;


    QByteArray acStat(int flag) const;
    QByteArray rtStat(int flag) const;
    QByteArray lsStat(int flag) const;

private:
    WebCtrlPack    *mPack;
};