#include "WebCtrlAnalyzeCore.h"
#include "WebCtrlSend.h"
#include "WebCtrlUserManager.h"
#include "PD/LogManag/Log.h"
#include "PD/DevManag/MusicPlayerManager.h"
#include "PD/DevManag/RotManager.h"
#include "PD/DevManag/LaserManager.h"
#include "PD/DataProcess/SynWorkAuto.h"
#include "DM/Manag/ParameterManager.h"
#include "DM/Manag/StatusManager.h"
#include "DM/File/ConfigManager.h"
#include "DM/File/MusicFileManager.h"
#include "SI/Common/XKCommon.h"

WebCtrlAnalyzeCore::WebCtrlAnalyzeCore(QObject *parent)
    : QObject(parent)
{

}

WebCtrlAnalyzeCore::~WebCtrlAnalyzeCore()
{

}

void WebCtrlAnalyzeCore::ntHearbeat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    if (job.contains(Key::mNtHeartBeat))
    {
        gWebCtrlSend->repNtNetHand(
            (ushort)job.value(Key::mNtHeartBeat).toInt(), ip, port);
    }
}

void WebCtrlAnalyzeCore::lgSetLogin(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //检查条目是否齐备
    if (!job.contains(Key::mLgLoginRequst)
        || !job.contains(Key::mLgUserName)
        || !job.contains(Key::mLgPassword))
    {
        logLackKey(ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    int errVal = -1;
    int lgRequst = job.value(Key::mLgLoginRequst).toInt(errVal);
    QString userName = job.value(Key::mLgUserName).toString();
    QString password = job.value(Key::mLgPassword).toString();

    //类型检查
    if (lgRequst == errVal)
    {
        logErrorPrmType(ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    //值检查
    if (lgRequst != Val::mLgLonoutRequst
        && lgRequst != Val::mLgLoginRequst)
    {
        logErrorPrmValue(ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    SoftUserInformation userInfo = gParam->getUsers().mUserWebCtrl;
    SoftUserInformation userInfoReq(userName, password);

    //该地址是否已登录
    bool isLogined = gWebCtrlUsers->isExistedUser(ip, port);

    //退出登录请求
    if (lgRequst == Val::mLgLonoutRequst)
    {
        //若未登录，则不需执行退出登录操作
        if (!isLogined)
        {
            log(QString("用户退出登录请求：该地址未登录，默认执行成功！"), ip, port);
            gWebCtrlSend->repLgLoginRequst(true, ip, port);
            return;
        }

        //已登录，则移除
        gWebCtrlUsers->removeUser(ip, port);
        log(QString("用户退出登录请求：执行成功！"), ip, port);
        gWebCtrlSend->repLgLoginRequst(true, ip, port);
        return;
    }


    //若用户原本已登录，则直接返回成功
    if (isLogined)
    {
        log(QString("用户登录请求：该地址已登录，默认执行成功！"), ip, port);
        gWebCtrlSend->repLgLoginRequst(true, ip, port);
        return;
    }

    //用户未登录，则判断用户名和密码是否正确
    //若用户名和密码不匹配，则登录失败
    if (userInfoReq != userInfo)
    {
        log(QString("用户登录请求：执行失败，用户名(%1)或密码(%2)错误！")
            .arg(userName).arg(password), ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    //用户名和密码正确，若登录数已超过最大允许个数，则返回失败
    if (!gWebCtrlUsers->appendUser(ip, port))
    {
        log(QString("用户登录请求：执行失败，用户名和密码正确，"
            "但当前用户数已达到最大允许数量(%1)！").arg(gWebCtrlUsers->getNumMax()), ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    //登录成功
    log(QString("用户登录请求：执行成功！"), ip, port);
    gWebCtrlSend->repLgLoginRequst(true, ip, port);

}

void WebCtrlAnalyzeCore::lgSetUserKeyMod(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //检查条目是否齐备
    if (!job.contains(Key::mLgUserNameOld)
        || !job.contains(Key::mLgPasswordOld)
        || !job.contains(Key::mLgUserNameNew)
        || !job.contains(Key::mLgPasswordNew))
    {
        logLackKey(ip, port);
        return;
    }

    QString nameOld = job.value(Key::mLgUserNameOld).toString();
    QString passwordOld = job.value(Key::mLgPasswordOld).toString();
    QString nameNew = job.value(Key::mLgUserNameNew).toString();
    QString passwordNew = job.value(Key::mLgPasswordNew).toString();

    ParameterUserInfo pu = gParam->getUsers();

    //原用户名和密码不正确，则修改失败
    if (SoftUserInformation(nameOld, passwordOld) != pu.mUserWebCtrl)
    {
        log(QString("修改用户名和密码：执行失败，原用户名(%1)或原密码(%2)不正确！")
            .arg(nameOld).arg(passwordOld), ip, port);
        gWebCtrlSend->repLgUserNamePasswordMod(false, ip, port);
        return;
    }

    //更新用户名和密码，并更新配置文件
    pu.mUserWebCtrl = SoftUserInformation(nameNew, passwordNew);
    gParam->setUsers(pu);
    gConfig->saveUserInfo();

    log(QString("修改用户名和密码：执行成功，新用户名(%1)，新密码(%2)。")
        .arg(nameNew).arg(passwordNew), ip, port);
    gWebCtrlSend->repLgUserNamePasswordMod(true, ip, port);
}

void WebCtrlAnalyzeCore::lgSetKeyForg(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行；当前直接明码返回用户名和密码

    log(QString("忘记用户名和密码：执行成功，当前直接返回用户名和密码。"), ip, port);
    gWebCtrlSend->repLgUserNamePasswordForg(true, ip, port);
}

void WebCtrlAnalyzeCore::lgGetLoginStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行；当前直接明码返回用户名和密码

    log(QString("查询登录状态：执行成功，当前地址%1登录。")
        .arg(gWebCtrlUsers->isAvailableUser(ip, port) ? 
            QString("已") : QString("未")), ip, port);

    gWebCtrlSend->repLgLoginStat(true, ip, port);
}




void WebCtrlAnalyzeCore::acSetPlayFile(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //检查条目是否齐备
    if (!job.contains(Key::mAcFileName))
    {
        logLackKey(ip, port);
        return;
    }

    QString fileName = job.value(Key::mAcFileName).toString();

    //若文件不存在，则失败
    if (!gParam->isMscExisted(fileName))
    {
        log(QString("设置播放曲目：执行失败，曲目《%1》不存在！").arg(fileName), ip, port);
        return;
    }

    gParam->setMscCurr(fileName);
    log(QString("设置播放曲目：执行成功，曲目《%1》。").arg(fileName), ip, port);
    gMscPlayerManag->setFile();
}

void WebCtrlAnalyzeCore::acSetPlay(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //检查条目是否齐备
    if (!job.contains(Key::mAcPlayCode))
    {
        logLackKey(ip, port);
        return;
    }

    //类型检查
    int errVal = -1;
    int playCode = job.value(Key::mAcPlayCode).toInt(errVal);
    if (playCode == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (playCode != Val::mAcPlayCodePlay
        && playCode != Val::mAcPlayCodePause
        && playCode != Val::mAcPlayCodeStop)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    QString splay;
    ParamDevAcst acst = gParam->getDevAcst();
    if (playCode == Val::mAcPlayCodePlay)
    {
        splay = QString("开始");      
        acst.mPlay = acst.Playing;       
    }
    else if (playCode == Val::mAcPlayCodePause)
    {
        splay = QString("暂停");
        acst.mPlay = acst.PlayPause;
    }
    else
    {
        splay = QString("结束");
        acst.mPlay = acst.PlayStop;
    }

    log(QString("音频播放控制：%1播放。").arg(splay), ip, port);
    gParam->setDevAcst(acst);
    gMscPlayerManag->setPlay();
}

void WebCtrlAnalyzeCore::acSetMute(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //检查条目是否齐备
    if (!job.contains(Key::mAcMuteCode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int muteCode = job.value(Key::mAcMuteCode).toInt(errVal);

    //类型检查
    if (muteCode == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (muteCode != Val::mAcMuteOn
        && muteCode != Val::mAcMuteOff)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    log(QString("设置声能设备静音：静音%1。")
        .arg(muteCode == Val::mAcMuteOn ? QString("开") : QString("关")),
        ip, port);

    ParamDevAcst acst = gParam->getDevAcst();
    acst.mMute = muteCode == Val::mAcMuteOn ? acst.VolumnOff : acst.VolumnOn;
    gParam->setDevAcst(acst);

    //播放器静音控制
    gMscPlayerManag->setMute();
}

void WebCtrlAnalyzeCore::acSetVolume(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mAcVolPlayer))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int vol = job.value(Key::mAcVolPlayer).toInt(errVal);

    //类型检查
    if (vol == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (vol < 0 || vol > 100)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    log(QString("设置音量：%1。").arg(vol), ip, port);

    ParamDevAcst acst = gParam->getDevAcst();
    acst.mVolume = vol;
    gParam->setDevAcst(acst);
    gMscPlayerManag->setVol();
}

void WebCtrlAnalyzeCore::acSetLoop(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mAcLoopMode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int loop = job.value(Key::mAcLoopMode).toInt(errVal);

    //类型检查
    if (loop == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (loop != Val::mAcLoopList 
        && loop != Val::mAcLoopOne
        && loop != Val::mAcLoopNone)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    QString sloop;
    ParamDevAcst acst = gParam->getDevAcst();
    if (loop == Val::mAcLoopList)
    {
        acst.mLoop = acst.LoopList;
        sloop = QString("列表循环");
    }
    else if (loop == Val::mAcLoopOne)
    {
        acst.mLoop = acst.LoopOne;
        sloop = QString("单曲循环");
    }
    else
    {
        acst.mLoop = acst.LoopNone;
        sloop = QString("单次播放");
    }

    log(QString("设置播放模式：%1。").arg(sloop), ip, port);
    gParam->setDevAcst(acst);
}

void WebCtrlAnalyzeCore::acSetDelFile(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mAcFileName))
    {
        logLackKey(ip, port);
        return;
    }

    QString fileName = job.value(Key::mAcFileName).toString();

    //文件名错误
    if (!gParam->isMscExisted(fileName))
    {
        log(QString("删除曲目：执行失败，曲目名《%1》错误！")
            .arg(fileName), ip, port);
        gWebCtrlSend->repAcDelFile(WebCtrlPack::AcDelErrFileName, ip, port);
        return;
    }

    //如果文件正在播放，则不应该删除 
    //Qt删除文件存在问题：正在使用的文件也能删除，但是删除后
    //被删文件会驻留在文件系统一段时间，导致更新文件列表的时候，被删文件依然存在
    bool succ = false;
    if (gParam->getDevAcst().mPlay == ParamDevAcst::Playing
        && gParam->mscCurr() == fileName)
    {
        succ = false;
    }
    else
    {
        //删除曲目
        QString path = gParam->mscPath(fileName);
        succ = XKCommon::removeFile(path);
    }

    
    QString slog;
    WebCtrlPack::DelFileReslt res;

    if (succ)
    {
        slog = QString("删除曲目：执行成功，曲目《%1》。").arg(fileName);
        res = WebCtrlPack::AcDelSucc;
    }
    else
    {
        slog = QString("删除曲目：执行失败，曲目《%1》正在占用中。").arg(fileName);
        res = WebCtrlPack::AcDelErrFileUsing;
    }

    log(slog, ip, port);

    //更新本地曲目列表
    if (succ)
    {
        gMscFlManag->updateList();
    }
    
    gWebCtrlSend->repAcDelFile(res, ip, port);
}

void WebCtrlAnalyzeCore::acSetAttack(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mAcAttackCode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int attack = job.value(Key::mAcAttackCode).toInt(errVal);

    //类型检查
    if (attack == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (attack != Val::mAcAttackOff
        && attack != Val::mAcAttackStrong
        && attack != Val::mAcAttackWeak)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    QString slog = QString("强声打击：");
    if (attack == Val::mAcAttackOff)
    {
        log(slog + QString("关。"), ip, port);
        gMscPlayerManag->attackStop();
    }
    else if (attack == Val::mAcAttackStrong)
    {
        log(slog + QString("强打击。"), ip, port);
        gMscPlayerManag->attackStrong();
    }
    else
    {
        log(slog + QString("弱打击。"), ip, port);
        gMscPlayerManag->attackWeak();
    }

}

void WebCtrlAnalyzeCore::acGetFileNum(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("查询曲目总数：执行成功，当前曲目总数为%1。")
        .arg(gParam->mscCount()), ip, port);

    gWebCtrlSend->repAcFileNum(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetFileList(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("查询曲目列表：执行成功，当前曲目总数为%1，文件列表略。")
        .arg(gParam->mscCount()), ip, port);

    gWebCtrlSend->repAcFileList(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetFilePlaying(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("查询当前播放的曲目名：执行成功，当前曲目为《%1》。")
        .arg(gParam->mscCurr()), ip, port);

    gWebCtrlSend->repAcFileCurrent(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetDevStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("查询声能设备状态：执行成功。"), ip, port);

    gWebCtrlSend->repAcStat(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetAttackStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("查询强声打击状态：执行失败，当前协议暂不支持。"), ip, port);
}

void WebCtrlAnalyzeCore::rtSetStop(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("转台停止转动：执行成功。"), ip, port);

    gRot->stop();
}

void WebCtrlAnalyzeCore::rtSetDirecTurn(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备查询
    if (!job.contains(Key::mRtDirecTurn))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int turn = job.value(Key::mRtDirecTurn).toInt(errVal);

    //类型检查
    if (turn == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (turn != Val::mRtUp
        && turn != Val::mRtDown
        && turn != Val::mRtLeft
        && turn != Val::mRtRight)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    ParamDevRot rot = gParam->getDevRot();
    QString sturn = QString("Error!");

    switch (turn)
    {
    case Val::mRtUp:
        rot.mIdtDirec = rot.TurnUp;
        sturn = QString("上转");
        break;
    case Val::mRtDown:
        rot.mIdtDirec = rot.TurnDown;
        sturn = QString("下转");
        break;
    case Val::mRtLeft:
        rot.mIdtDirec = rot.TurnLeft;
        sturn = QString("左转");
        break;
    case Val::mRtRight:
        rot.mIdtDirec = rot.TurnRight;
        sturn = QString("右转");
        break;
    default:
        return;
    }
    
    gParam->setDevRot(rot);

    log(QString("转台姿态控制：执行成功，开始%1。").arg(sturn), ip, port);

    //转台姿态控制
    gRot->turnDirec();
}

void WebCtrlAnalyzeCore::rtSetVel(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mRtAPSign)
        && !job.contains(Key::mRtVelAzi)
        && !job.contains(Key::mRtVelPit))
    {
        logLackKey(ip, port);
        return;
    }

    int errSign = -1;
    double errVel = -10.0;
    int sign = job.value(Key::mRtAPSign).toInt(errSign);
    double velAzi = job.value(Key::mRtVelAzi).toDouble(errVel);
    double velPit = job.value(Key::mRtVelPit).toDouble(errVel);

    //类型检查
    if (sign == errSign
        || abs(velAzi - errVel) < 0.5
        || abs(velPit - errVel) < 0.5)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (sign != Val::mRtSignAzi
        && sign != Val::mRtSignPit
        && sign != Val::mRtSignAziPit
        || velAzi < 0.0
        || velPit < 0.0)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    QString slog = QString("设置转台转速：执行成功，");
    ParamDevRot rot = gParam->getDevRot();
    rot.mVelAzi = qBound(0.0, velAzi, (double)rot.VelHMax);
    rot.mVelPit = qBound(0.0, velPit, (double)rot.VelVMax);

    switch (sign)
    {
    case Val::mRtSignAzi:
        rot.mIdtVel = rot.AziOnly;
        slog.append(QString("方位转速%1°/s。").arg(rot.mVelAzi, 0, 'f', 2));
        break;
    case Val::mRtSignPit:
        rot.mIdtVel = rot.PitOnly;
        slog.append(QString("俯仰转速%1°/s。").arg(rot.mVelPit, 0, 'f', 2));
        break;
    case Val::mRtSignAziPit:
        rot.mIdtVel = rot.AziAndPit;
        slog.append(QString("方位转速%1°/s，俯仰转速%2°/s。")
            .arg(rot.mVelAzi, 0, 'f', 2).arg(rot.mVelPit, 0, 'f', 2));
        break;
    default:
        return;
    }

    gParam->setDevRot(rot);

    log(slog, ip, port);
}

void WebCtrlAnalyzeCore::rtSetResident(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mRtAPSign)
        && !job.contains(Key::mRtResAzi)
        && !job.contains(Key::mRtResPit))
    {
        logLackKey(ip, port);
        return;
    }

    int errSign = -1;
    double errAngle = -1000.0;
    int sign = job.value(Key::mRtAPSign).toInt(errSign);
    double azi = job.value(Key::mRtResAzi).toDouble(errAngle);
    double pit = job.value(Key::mRtResPit).toDouble(errAngle);

    //类型检查
    if (sign == errSign
        || abs(azi - errAngle) < 0.5
        || abs(pit - errAngle) < 0.5)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (sign != Val::mRtSignAzi
        && sign != Val::mRtSignPit
        && sign != Val::mRtSignAziPit
        || azi < 0.0
        || azi >= 360.0
        || pit < -90.0
        || pit > 90.0)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    QString slog = QString("控制转台驻留角：执行成功，");
    ParamDevRot rot = gParam->getDevRot();
    rot.mAzi = XKCommon::angleToPos(azi);
    rot.mPit = XKCommon::angleToNeg(pit);

    switch (sign)
    {
    case Val::mRtSignAzi:
        rot.mIdtDeg = rot.AziOnly;
        slog.append(QString("方位%1°。").arg(rot.mAzi, 0, 'f', 2));
        break;
    case Val::mRtSignPit:
        rot.mIdtDeg = rot.PitOnly;
        slog.append(QString("俯仰%1°。").arg(rot.mPit, 0, 'f', 2));
        break;
    case Val::mRtSignAziPit:
        rot.mIdtDeg = rot.AziAndPit;
        slog.append(QString("方位%1°，俯仰%2°。")
            .arg(rot.mAzi, 0, 'f', 2).arg(rot.mPit, 0, 'f', 2));
        break;
    default:
        return;
    }

    gParam->setDevRot(rot);

    log(slog, ip, port);

    //设置转台驻留角
    gRot->resident();
}

void WebCtrlAnalyzeCore::rtSetZero(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mRtZero))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int reset = job.value(Key::mRtZero).toInt(errVal);

    //类型检查
    if (reset == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (reset != Val::mRtSignAzi
        && reset != Val::mRtSignPit
        && reset != Val::mRtSignAziPit)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    ParamDevRot rot = gParam->getDevRot();
    QString slog = QString("控制转台归零：执行成功，");

    switch (reset)
    {
    case Val::mRtSignAzi:
        rot.mIdtZero = rot.AziOnly;
        slog.append(QString("方位归零。"));
        break;
    case Val::mRtSignPit:
        rot.mIdtZero = rot.PitOnly;
        slog.append(QString("俯仰归零。"));
        break;
    case Val::mRtSignAziPit:
        rot.mIdtZero = rot.AziAndPit;
        slog.append(QString("方位和俯仰归零。"));
        break;
    default:
        return;
    }

    gParam->setDevRot(rot);
    log(slog, ip, port);

    //控制归零
    gRot->zero();
}

void WebCtrlAnalyzeCore::rtGetStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("查询转台设备状态：执行成功。"), ip, port);

    gWebCtrlSend->repRtStat(true, ip, port);
}

void WebCtrlAnalyzeCore::lsSetAttack(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mLsAttackCode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int attack = job.value(Key::mLsAttackCode).toInt(errVal);

    //类型检查
    if (attack == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (attack != Val::mLsOff
        && attack != Val::mLsBeat
        && attack != Val::mLsWarning
        && attack != Val::mLsFlash)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    switch (attack)
    {
    case Val::mLsOff:
        log(QString("强光控制：关闭。"), ip, port);
        gLaser->turnOffBeingOn();
        break;
    case Val::mLsBeat:
        log(QString("强光控制：直射。"), ip, port);
        gLaser->beat(gParam->getSimple().mPrmLaser.mColorBeat);
        break;
    case Val::mLsWarning:
        log(QString("强光控制：警示。"), ip, port);
        gLaser->warning(gParam->getSimple().mPrmLaser.mColorWarning);
        break;
    case Val::mLsFlash:
        log(QString("强光控制：爆闪。"), ip, port);
        gLaser->burstFlash(gParam->getSimple().mPrmLaser.mColorBurstFlash);
        break;
    default:
        return;
    }
}

void WebCtrlAnalyzeCore::lsGetStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接查询

    log(QString("查询强光设备状态：执行成功。"), ip, port);

    gWebCtrlSend->repLsStat(true, ip, port);
}

void WebCtrlAnalyzeCore::coSetWorkModel(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //条目齐备检查
    if (!job.contains(Key::mCoWorkModel))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int model = job.value(Key::mCoWorkModel).toInt(errVal);

    //类型检查
    if (model == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //值检查
    if (model != Val::mCoWorkAuto
        && model != Val::mCoWorkManual)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    ParamIntegrated itg = gParam->getInteg();
    QString slog = QString("综合管理：设置工作模式为");

    switch (model)
    {
    case Val::mCoWorkAuto:
        itg.mWorkModel = ParamIntegrated::WorkAuto;
        slog = slog + QString("自动模式");
        break;
    case Val::mCoWorkManual:
        itg.mWorkModel = ParamIntegrated::WorkManual;
        slog = slog + QString("手动模式");
        break;
    default:
        return;
    }

    gParam->setInteg(itg);
    gConfig->saveToRestore();

    log(slog, ip, port);

    //模式切换
    gSynAuto->procWorkModel();
}

void WebCtrlAnalyzeCore::coGetWorkModel(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //无参数，直接执行

    log(QString("查询综合管理-工作模式：执行成功。"), ip, port);

    gWebCtrlSend->repCoWorkModel(true, ip, port);
}

void WebCtrlAnalyzeCore::logLackKey(const QString & ip, ushort port) const
{
    gLog->write(QString("【Web %1:%2】错误：参数数据项缺失！").arg(ip).arg(port), true);
}

void WebCtrlAnalyzeCore::logErrorPrmType(const QString & ip, ushort port) const
{
    gLog->write(QString("【Web %1:%2】错误：参数类型错误！").arg(ip).arg(port), true);
}

void WebCtrlAnalyzeCore::logErrorPrmValue(const QString & ip, ushort port) const
{
    gLog->write(QString("【Web %1:%2】错误：参数值错误！").arg(ip).arg(port), true);
}

void WebCtrlAnalyzeCore::log(QString slog, const QString & ip, ushort port) const
{
    slog = QString("【Web %1:%2 执行】").arg(ip).arg(port) + slog;
    gLog->write(slog, true);
}

