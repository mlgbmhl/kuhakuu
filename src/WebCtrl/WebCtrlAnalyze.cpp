#include "WebCtrlAnalyze.h"
#include "PD/LogManag/Log.h"
#include "ProtocolWebCtrl.h"
#include "WebCtrlSend.h"
#include "WebCtrlUserManager.h"

#include <QJsonDocument> 
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

WebCtrlAnalyze::WebCtrlAnalyze()
{
    init();
}

WebCtrlAnalyze::~WebCtrlAnalyze()
{
    delete mCore;
}

WebCtrlAnalyze* WebCtrlAnalyze::GetInstance()
{
    static WebCtrlAnalyze ins;
    return &ins;
}

void WebCtrlAnalyze::append(
    const QByteArray & data, const QString & ipSrc, const ushort & portSrc)
{
    //不用考虑分包、拼包的情况
    //每一次Websocket都是收到一个完整的Text
    preAnalyze(data, ipSrc, portSrc);
}

void WebCtrlAnalyze::preAnalyze(
    const QByteArray & data, const QString & ipSrc, const ushort & portSrc)
{
    //加载Json对象，若加载失败，说明数据不满足Json格式，解析失败
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
    {
        gLog->write(QString("【Web】解析失败：数据格式错误（加载Json对象失败）！"), true);
        return;
    }

    //判断json对象是否合法，不合法则说明数据格式错误，解析失败
    if (!doc.isObject())
    {
        gLog->write(QString("【Web】解析失败：数据格式错误（Json对象格式有误）！"), true);
        return;
    }

    //取出Json对象
    QJsonObject obRecv = doc.object();

    using Fmt = PrtcWebCtrlFmt;
    if (!obRecv.contains(Fmt::mType) || !obRecv.contains(Fmt::mCode))
    {
        gLog->write(QString("【Web】解析失败：关键数据项缺失（cmdType或cmdCode缺失）！"), true);
        return;
    }

    using Ty = PrtcWebCtrlCmdType;
    using Cd = PrtcWebCtrlCmdCode;
    
    QString cmdType, cmdCode;
    cmdType = obRecv.value(Fmt::mType).toString();
    cmdCode = obRecv.value(Fmt::mCode).toString();

    //只要不是网络心跳，则记录指令接收日志
    if (cmdType != Ty::mNet)
    {
        gLog->write(QString("【Web】收到来自%1：%2的指令：类型 = %3， 指令码 = %4。")
            .arg(ipSrc).arg(portSrc)
            .arg(cmdType).arg(cmdCode), true);
    }

    //判断指令码是否有效，无效则返回
    if (!mMapCodeToCore.contains(cmdCode))
    {
        gLog->write(QString("【Web】指令码错误：%1！").arg(cmdCode), true);
        return;
    }

    //部分指令可直接允许通信（无论是否已登录），其余需要用户登录后才能执行
    bool isValidMsg = false;
    if (mVcCodesPermitAny.contains(cmdCode))
    {
        isValidMsg = true;
    }
    else
    {
        //其余情况，必须用户已登录，才能有效通信
        isValidMsg = gWebCtrlUsers->isAvailableUser(ipSrc, portSrc);
    }

    //若为不允许通信状态，则提示错误并直接返回
    if (!isValidMsg)
    {
        gLog->write(QString("【Web】指令执行失败：当前用户%1：%2未登录，"
            "不具备除网络心跳、请求登录以外的通信权限！").arg(ipSrc).arg(portSrc), true);
        return;
    }

    //类型为SET、GET的报文，立即回执一条应答报文   
    if (cmdType == Ty::mSet || cmdType == Ty::mGet)
    {
        gWebCtrlSend->response(cmdCode, ipSrc, portSrc);
    }

    //根据cmdCode，分发具体解析执行函数
    (mCore->*mMapCodeToCore.value(cmdCode))(obRecv, ipSrc, portSrc);
}

void WebCtrlAnalyze::init()
{
    using Cr = WebCtrlAnalyzeCore;
    using Cd = PrtcWebCtrlCmdCode;

    mCore = new Cr();

    mMapCodeToCore.insert(Cd::mNtHeartBeat, &Cr::ntHearbeat);

    mMapCodeToCore.insert(Cd::mLgSetLogin, &Cr::lgSetLogin);
    mMapCodeToCore.insert(Cd::mLgSetUserKeyMod, &Cr::lgSetUserKeyMod);
    mMapCodeToCore.insert(Cd::mLgSetKeyForg, &Cr::lgSetKeyForg);
    mMapCodeToCore.insert(Cd::mLgGetLoginStat, &Cr::lgGetLoginStat);

    mMapCodeToCore.insert(Cd::mAcSetPlayFile, &Cr::acSetPlayFile);
    mMapCodeToCore.insert(Cd::mAcSetPlay, &Cr::acSetPlay);
    mMapCodeToCore.insert(Cd::mAcSetMute, &Cr::acSetMute);
    mMapCodeToCore.insert(Cd::mAcSetVolume, &Cr::acSetVolume);
    mMapCodeToCore.insert(Cd::mAcSetLoop, &Cr::acSetLoop);
    mMapCodeToCore.insert(Cd::mAcSetDelFile, &Cr::acSetDelFile);
    mMapCodeToCore.insert(Cd::mAcSetAttack, &Cr::acSetAttack);
    mMapCodeToCore.insert(Cd::mAcGetFileNum, &Cr::acGetFileNum);
    mMapCodeToCore.insert(Cd::mAcGetFileList, &Cr::acGetFileList);
    mMapCodeToCore.insert(Cd::mAcGetFilePlaying, &Cr::acGetFilePlaying);
    mMapCodeToCore.insert(Cd::mAcGetDevStat, &Cr::acGetDevStat);
    mMapCodeToCore.insert(Cd::mAcGetAttackStat, &Cr::acGetAttackStat);

    mMapCodeToCore.insert(Cd::mRtSetStop, &Cr::rtSetStop);
    mMapCodeToCore.insert(Cd::mRtSetDirecTurn, &Cr::rtSetDirecTurn);
    mMapCodeToCore.insert(Cd::mRtSetVel, &Cr::rtSetVel);
    mMapCodeToCore.insert(Cd::mRtSetResident, &Cr::rtSetResident);
    mMapCodeToCore.insert(Cd::mRtSetZero, &Cr::rtSetZero);
    mMapCodeToCore.insert(Cd::mRtGetStat, &Cr::rtGetStat);

    mMapCodeToCore.insert(Cd::mLsSetAttack, &Cr::lsSetAttack);
    mMapCodeToCore.insert(Cd::mLsGetStat, &Cr::lsGetStat);

    mMapCodeToCore.insert(Cd::mCoSetWorkModel, &Cr::coSetWorkModel);
    mMapCodeToCore.insert(Cd::mCoGetWorkModel, &Cr::coGetWorkModel);

    mVcCodesPermitAny.append(Cd::mNtHeartBeat);
    mVcCodesPermitAny.append(Cd::mLgSetLogin);
    mVcCodesPermitAny.append(Cd::mLgSetUserKeyMod);
    mVcCodesPermitAny.append(Cd::mLgSetKeyForg);
    mVcCodesPermitAny.append(Cd::mLgGetLoginStat);

}

