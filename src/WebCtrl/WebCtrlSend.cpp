#include "WebCtrlSend.h"
#include "WebCtrlUserManager.h"
#include "SI/Communic/NetworkManager.h"
#include "SI/Common/XKCommon.h"
#include "PD/LogManag/Log.h"
#include "DM/Manag/ParameterManager.h"
#include "DM/Manag/StatusManager.h"
#include "ProtocolWebCtrl.h"


WebCtrlSend::WebCtrlSend()
    : QObject(NULL)
{
    mPack = new WebCtrlPack();
}

WebCtrlSend::~WebCtrlSend()
{
    delete mPack;
}

WebCtrlSend* WebCtrlSend::GetInstance()
{
    static WebCtrlSend ins;
    return &ins;
}

void WebCtrlSend::response(
    const QString & cmdCode, const QString & ip, ushort port) const
{
    log(QString("Ö¸ÁîÓ¦´ð£º%1¡£").arg(cmdCode), ip, port);
    send(mPack->response(cmdCode), ip, port);
}

void WebCtrlSend::repNtNetHand(

(mPack->repNtNethand(heartbeat), ip, port);
}

void WebCtrlSend::repLgLoginRequst(
    bool succ, const QString & ip, ushort port) const
{
    send(mPack->repLgLoginRequst(succ), ip, port);
}

void WebCtrlSend::repLgUserNamePasswordMod(
    bool succ, const QString & ip, ushort port) const
{
    send(mPack->repLgUserNamePasswordMod(succ), ip, port);
}

void WebCtrlSend::repLgUserNamePasswordForg(
    bool succ, const QString & ip, ushort port) const
{
    SoftUserInformation sui = gParam->getUsers().mUserWebCtrl;
    send(mPack->repLgUserNamePasswordForg(succ, sui.mName, sui.mPassword), ip, port);
}

void WebCtrlSend::repLgLoginStat(
    bool succ, const QString & ip, ushort port) const
{
    send(mPack->repLgLoginStat(
        (succ ? WebCtrlPack::Succ : WebCtrlPack::Failed), 
        gWebCtrlUsers->isAvailableUser(ip, port)), 
        ip, port);
}

void WebCtrlSend::autoRepLgLoginStat() const
{
    XKNetServeInfo web = gParam->getComm().mWebSrvCtrl;

    QString ip;
    ushort port;

    for (int i = 0; i < web.mAddrClt.size(); i++)
    {
        ip = web.mAddrClt.at(i).mIP;
        port = web.mAddrClt.at(i).mPort;

        send(mPack->repLgLoginStat(
            WebCtrlPack::AutoRep,
            gWebCtrlUsers->isAvailableUser(ip, port)),
            ip, port);
    }
}

void WebCtrlSend::repAcDelFile(
    WebCtrlPack::DelFileReslt resualt, const QString & ip, ushort port) const
{
    send(mPack->repAcDelFile(resualt), ip, port);
}

void WebCtrlSend::repAcFileNum(
    bool succ, const QString & ip, ushort port) const
{
    send(mPack->repAcFileNum(true, gParam->mscCount()), ip, port);
}

void WebCtrlSend::repAcFileList(
    bool succ, const QString & ip, ushort port) const
{
    QStringList flList;
    ParamMusicList pm = gParam->getMscList();
    int num = pm.mNames.size();
    for (int i = 0; i < num; i++)
    {
        flList.append(pm.mNames.at(i));
    }

    send(mPack->repAcFileList(true, flList), ip, port);
}

void WebCtrlSend::repAcFileCurrent(
    bool succ, const QString & ip, ushort port) const
{
    send(mPack->repAcFileCurrent(
        succ ? WebCtrlPack::Succ : WebCtrlPack::Failed, 
        gParam->mscCurr()), ip, port);
}

void WebCtrlSend::autoRepAcFileCurrent() const
{
    sendAll(mPack->repAcFileCurrent(WebCtrlPack::AutoRep, gParam->mscCurr()));
}

void WebCtrlSend::repAcStat(
    bool succ, const QString & ip, ushort port) const
{
    send(acStat(succ ? WebCtrlPack::Succ : WebCtrlPack::Failed), ip, port);
}

void WebCtrlSend::autoRepAcStat() const
{
    sendAll(acStat(WebCtrlPack::AutoRep));
}

void WebCtrlSend::repRtStat(
    bool succ, const QString & ip, ushort port) const
{
    send(rtStat(succ ? WebCtrlPack::Succ : WebCtrlPack::Failed), ip, port);
}

void WebCtrlSend::autoRepRtStat() const
{
    sendAll(rtStat(WebCtrlPack::AutoRep));
}

void WebCtrlSend::repLsStat(
    bool succ, const QString & ip, ushort port) const
{
    send(lsStat(succ ? WebCtrlPack::Succ : WebCtrlPack::Failed), ip, port);
}

void WebCtrlSend::autoRepLsStat() const
{
    sendAll(lsStat(WebCtrlPack::AutoRep));
}

void WebCtrlSend::repCoWorkModel(bool succ, const QString & ip, ushort port) const
{
    WebCtrlPack::CompWork cw =
        (gStatus->getInteg().mWorkModel == StatIntegrated::WorkAuto) ?
        WebCtrlPack::CompWorkAuto : WebCtrlPack::CompWorkManual;
    send(mPack->repCoWorkModel(succ ? WebCtrlPack::Succ : WebCtrlPack::Failed, cw), ip, port);
}

bool WebCtrlSend::send(
    const QByteArray & data, const QString & ip, ushort port) const
{
    return gWebSrvCtrl->send(data, ip, port, false);
}

bool WebCtrlSend::sendAll(const QByteArray & data) const
{
    return gWebSrvCtrl->send(data, false);
}

void WebCtrlSend::log(QString slog, const QString& ip, ushort port) const
{
    slog = QString("¡¾Web %1:%2¡¿").arg(ip).arg(port) + slog;
    gLog->write(slog, true);
}

QByteArray WebCtrlSend::acStat(int flag) const
{
    StatDevAcst sda = gStatus->getAcst();
    WebCtrlPack::AcStat was;

    using V = PrtcWebCtrlJsonValue;

    was.mPlayCode = (sda.mPlay == sda.PlayCurr) ? V::mAcPlayCodePlay
        : ((sda.mPlay == sda.PlayPause) ? V::mAcPlayCodePause : V::mAcPlayCodeStop);

    was.mMute = (sda.mMute == sda.VolumnOff) ? V::mAcMuteOn : V::mAcMuteOff;
    was.mVolPlayer = qBound(0, sda.mVolume, 100);

    was.mLoop = (sda.mLoop == sda.LoopList) ? V::mAcLoopList
        : ((sda.mLoop == sda.LoopOne) ? V::mAcLoopOne : V::mAcLoopNone);

    was.mAttackCode = (sda.mAttack == sda.AttackOff) ? V::mAcAttackOff
        : ((sda.mAttack == sda.AttackStrong) ? V::mAcAttackStrong : V::mAcAttackWeak);

    was.mSpeakCode = 0;
    was.mVolSpeak = was.mVolPlayer;

    return mPack->repAcStat(WebCtrlPack::RepFlag(flag), was);
}

QByteArray WebCtrlSend::rtStat(int flag) const
{
    StatDevRot sdt = gStatus->getRot();
    WebCtrlPack::RtStat rst;

    using V = PrtcWebCtrlJsonValue;

    rst.mMoveStat = (sdt.mRunStat == sdt.Arrived) ? V::mRtStMoveArrived : V::mRtStMoving;
    rst.mAzi = XKCommon::angleToPos(sdt.mAzi);
    rst.mPit = XKCommon::angleToNeg(sdt.mPit);
    rst.mAziFixed = XKCommon::angleToPos(sdt.mAziDelt);
    rst.mPitFixed = XKCommon::angleToNeg(sdt.mPitDelt);
    rst.mVelAzi = sdt.mVelAzi;
    rst.mVelPit = sdt.mVelPit;

    return mPack->repRtStat(WebCtrlPack::RepFlag(flag), rst);
}

QByteArray WebCtrlSend::lsStat(int flag) const
{
    StatDevLaser sdl = gStatus->getLaser();
    WebCtrlPack::LsAttCode attack;

    switch (sdl.mAttack)
    {
    case StatDevLaser::AttackOff:
        attack = WebCtrlPack::LsOff;
        break;
    case StatDevLaser::AttackBeat:
        attack = WebCtrlPack::LsBeat;
        break;
    case StatDevLaser::AttackWarning:
        attack = WebCtrlPack::LsWarning;
        break;
    case StatDevLaser::AttackFlash:
        attack = WebCtrlPack::LsFlash;
        break;
    default:
        attack = WebCtrlPack::LsOff;
        break;
    }

    return mPack->repLsStat(WebCtrlPack::RepFlag(flag), attack);
}

