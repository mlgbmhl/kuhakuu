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
    //�����Ŀ�Ƿ��뱸
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

    //���ͼ��
    if (lgRequst == errVal)
    {
        logErrorPrmType(ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    //ֵ���
    if (lgRequst != Val::mLgLonoutRequst
        && lgRequst != Val::mLgLoginRequst)
    {
        logErrorPrmValue(ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    SoftUserInformation userInfo = gParam->getUsers().mUserWebCtrl;
    SoftUserInformation userInfoReq(userName, password);

    //�õ�ַ�Ƿ��ѵ�¼
    bool isLogined = gWebCtrlUsers->isExistedUser(ip, port);

    //�˳���¼����
    if (lgRequst == Val::mLgLonoutRequst)
    {
        //��δ��¼������ִ���˳���¼����
        if (!isLogined)
        {
            log(QString("�û��˳���¼���󣺸õ�ַδ��¼��Ĭ��ִ�гɹ���"), ip, port);
            gWebCtrlSend->repLgLoginRequst(true, ip, port);
            return;
        }

        //�ѵ�¼�����Ƴ�
        gWebCtrlUsers->removeUser(ip, port);
        log(QString("�û��˳���¼����ִ�гɹ���"), ip, port);
        gWebCtrlSend->repLgLoginRequst(true, ip, port);
        return;
    }


    //���û�ԭ���ѵ�¼����ֱ�ӷ��سɹ�
    if (isLogined)
    {
        log(QString("�û���¼���󣺸õ�ַ�ѵ�¼��Ĭ��ִ�гɹ���"), ip, port);
        gWebCtrlSend->repLgLoginRequst(true, ip, port);
        return;
    }

    //�û�δ��¼�����ж��û����������Ƿ���ȷ
    //���û��������벻ƥ�䣬���¼ʧ��
    if (userInfoReq != userInfo)
    {
        log(QString("�û���¼����ִ��ʧ�ܣ��û���(%1)������(%2)����")
            .arg(userName).arg(password), ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    //�û�����������ȷ������¼���ѳ����������������򷵻�ʧ��
    if (!gWebCtrlUsers->appendUser(ip, port))
    {
        log(QString("�û���¼����ִ��ʧ�ܣ��û�����������ȷ��"
            "����ǰ�û����Ѵﵽ�����������(%1)��").arg(gWebCtrlUsers->getNumMax()), ip, port);
        gWebCtrlSend->repLgLoginRequst(false, ip, port);
        return;
    }

    //��¼�ɹ�
    log(QString("�û���¼����ִ�гɹ���"), ip, port);
    gWebCtrlSend->repLgLoginRequst(true, ip, port);

}

void WebCtrlAnalyzeCore::lgSetUserKeyMod(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�����Ŀ�Ƿ��뱸
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

    //ԭ�û��������벻��ȷ�����޸�ʧ��
    if (SoftUserInformation(nameOld, passwordOld) != pu.mUserWebCtrl)
    {
        log(QString("�޸��û��������룺ִ��ʧ�ܣ�ԭ�û���(%1)��ԭ����(%2)����ȷ��")
            .arg(nameOld).arg(passwordOld), ip, port);
        gWebCtrlSend->repLgUserNamePasswordMod(false, ip, port);
        return;
    }

    //�����û��������룬�����������ļ�
    pu.mUserWebCtrl = SoftUserInformation(nameNew, passwordNew);
    gParam->setUsers(pu);
    gConfig->saveUserInfo();

    log(QString("�޸��û��������룺ִ�гɹ������û���(%1)��������(%2)��")
        .arg(nameNew).arg(passwordNew), ip, port);
    gWebCtrlSend->repLgUserNamePasswordMod(true, ip, port);
}

void WebCtrlAnalyzeCore::lgSetKeyForg(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ�У���ǰֱ�����뷵���û���������

    log(QString("�����û��������룺ִ�гɹ�����ǰֱ�ӷ����û��������롣"), ip, port);
    gWebCtrlSend->repLgUserNamePasswordForg(true, ip, port);
}

void WebCtrlAnalyzeCore::lgGetLoginStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ�У���ǰֱ�����뷵���û���������

    log(QString("��ѯ��¼״̬��ִ�гɹ�����ǰ��ַ%1��¼��")
        .arg(gWebCtrlUsers->isAvailableUser(ip, port) ? 
            QString("��") : QString("δ")), ip, port);

    gWebCtrlSend->repLgLoginStat(true, ip, port);
}




void WebCtrlAnalyzeCore::acSetPlayFile(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�����Ŀ�Ƿ��뱸
    if (!job.contains(Key::mAcFileName))
    {
        logLackKey(ip, port);
        return;
    }

    QString fileName = job.value(Key::mAcFileName).toString();

    //���ļ������ڣ���ʧ��
    if (!gParam->isMscExisted(fileName))
    {
        log(QString("���ò�����Ŀ��ִ��ʧ�ܣ���Ŀ��%1�������ڣ�").arg(fileName), ip, port);
        return;
    }

    gParam->setMscCurr(fileName);
    log(QString("���ò�����Ŀ��ִ�гɹ�����Ŀ��%1����").arg(fileName), ip, port);
    gMscPlayerManag->setFile();
}

void WebCtrlAnalyzeCore::acSetPlay(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�����Ŀ�Ƿ��뱸
    if (!job.contains(Key::mAcPlayCode))
    {
        logLackKey(ip, port);
        return;
    }

    //���ͼ��
    int errVal = -1;
    int playCode = job.value(Key::mAcPlayCode).toInt(errVal);
    if (playCode == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
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
        splay = QString("��ʼ");      
        acst.mPlay = acst.Playing;       
    }
    else if (playCode == Val::mAcPlayCodePause)
    {
        splay = QString("��ͣ");
        acst.mPlay = acst.PlayPause;
    }
    else
    {
        splay = QString("����");
        acst.mPlay = acst.PlayStop;
    }

    log(QString("��Ƶ���ſ��ƣ�%1���š�").arg(splay), ip, port);
    gParam->setDevAcst(acst);
    gMscPlayerManag->setPlay();
}

void WebCtrlAnalyzeCore::acSetMute(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�����Ŀ�Ƿ��뱸
    if (!job.contains(Key::mAcMuteCode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int muteCode = job.value(Key::mAcMuteCode).toInt(errVal);

    //���ͼ��
    if (muteCode == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
    if (muteCode != Val::mAcMuteOn
        && muteCode != Val::mAcMuteOff)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    log(QString("���������豸����������%1��")
        .arg(muteCode == Val::mAcMuteOn ? QString("��") : QString("��")),
        ip, port);

    ParamDevAcst acst = gParam->getDevAcst();
    acst.mMute = muteCode == Val::mAcMuteOn ? acst.VolumnOff : acst.VolumnOn;
    gParam->setDevAcst(acst);

    //��������������
    gMscPlayerManag->setMute();
}

void WebCtrlAnalyzeCore::acSetVolume(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
    if (!job.contains(Key::mAcVolPlayer))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int vol = job.value(Key::mAcVolPlayer).toInt(errVal);

    //���ͼ��
    if (vol == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
    if (vol < 0 || vol > 100)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    log(QString("����������%1��").arg(vol), ip, port);

    ParamDevAcst acst = gParam->getDevAcst();
    acst.mVolume = vol;
    gParam->setDevAcst(acst);
    gMscPlayerManag->setVol();
}

void WebCtrlAnalyzeCore::acSetLoop(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
    if (!job.contains(Key::mAcLoopMode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int loop = job.value(Key::mAcLoopMode).toInt(errVal);

    //���ͼ��
    if (loop == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
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
        sloop = QString("�б�ѭ��");
    }
    else if (loop == Val::mAcLoopOne)
    {
        acst.mLoop = acst.LoopOne;
        sloop = QString("����ѭ��");
    }
    else
    {
        acst.mLoop = acst.LoopNone;
        sloop = QString("���β���");
    }

    log(QString("���ò���ģʽ��%1��").arg(sloop), ip, port);
    gParam->setDevAcst(acst);
}

void WebCtrlAnalyzeCore::acSetDelFile(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
    if (!job.contains(Key::mAcFileName))
    {
        logLackKey(ip, port);
        return;
    }

    QString fileName = job.value(Key::mAcFileName).toString();

    //�ļ�������
    if (!gParam->isMscExisted(fileName))
    {
        log(QString("ɾ����Ŀ��ִ��ʧ�ܣ���Ŀ����%1������")
            .arg(fileName), ip, port);
        gWebCtrlSend->repAcDelFile(WebCtrlPack::AcDelErrFileName, ip, port);
        return;
    }

    //����ļ����ڲ��ţ���Ӧ��ɾ�� 
    //Qtɾ���ļ��������⣺����ʹ�õ��ļ�Ҳ��ɾ��������ɾ����
    //��ɾ�ļ���פ�����ļ�ϵͳһ��ʱ�䣬���¸����ļ��б��ʱ�򣬱�ɾ�ļ���Ȼ����
    bool succ = false;
    if (gParam->getDevAcst().mPlay == ParamDevAcst::Playing
        && gParam->mscCurr() == fileName)
    {
        succ = false;
    }
    else
    {
        //ɾ����Ŀ
        QString path = gParam->mscPath(fileName);
        succ = XKCommon::removeFile(path);
    }

    
    QString slog;
    WebCtrlPack::DelFileReslt res;

    if (succ)
    {
        slog = QString("ɾ����Ŀ��ִ�гɹ�����Ŀ��%1����").arg(fileName);
        res = WebCtrlPack::AcDelSucc;
    }
    else
    {
        slog = QString("ɾ����Ŀ��ִ��ʧ�ܣ���Ŀ��%1������ռ���С�").arg(fileName);
        res = WebCtrlPack::AcDelErrFileUsing;
    }

    log(slog, ip, port);

    //���±�����Ŀ�б�
    if (succ)
    {
        gMscFlManag->updateList();
    }
    
    gWebCtrlSend->repAcDelFile(res, ip, port);
}

void WebCtrlAnalyzeCore::acSetAttack(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
    if (!job.contains(Key::mAcAttackCode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int attack = job.value(Key::mAcAttackCode).toInt(errVal);

    //���ͼ��
    if (attack == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
    if (attack != Val::mAcAttackOff
        && attack != Val::mAcAttackStrong
        && attack != Val::mAcAttackWeak)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    QString slog = QString("ǿ�������");
    if (attack == Val::mAcAttackOff)
    {
        log(slog + QString("�ء�"), ip, port);
        gMscPlayerManag->attackStop();
    }
    else if (attack == Val::mAcAttackStrong)
    {
        log(slog + QString("ǿ�����"), ip, port);
        gMscPlayerManag->attackStrong();
    }
    else
    {
        log(slog + QString("�������"), ip, port);
        gMscPlayerManag->attackWeak();
    }

}

void WebCtrlAnalyzeCore::acGetFileNum(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("��ѯ��Ŀ������ִ�гɹ�����ǰ��Ŀ����Ϊ%1��")
        .arg(gParam->mscCount()), ip, port);

    gWebCtrlSend->repAcFileNum(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetFileList(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("��ѯ��Ŀ�б�ִ�гɹ�����ǰ��Ŀ����Ϊ%1���ļ��б��ԡ�")
        .arg(gParam->mscCount()), ip, port);

    gWebCtrlSend->repAcFileList(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetFilePlaying(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("��ѯ��ǰ���ŵ���Ŀ����ִ�гɹ�����ǰ��ĿΪ��%1����")
        .arg(gParam->mscCurr()), ip, port);

    gWebCtrlSend->repAcFileCurrent(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetDevStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("��ѯ�����豸״̬��ִ�гɹ���"), ip, port);

    gWebCtrlSend->repAcStat(true, ip, port);
}

void WebCtrlAnalyzeCore::acGetAttackStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("��ѯǿ�����״̬��ִ��ʧ�ܣ���ǰЭ���ݲ�֧�֡�"), ip, port);
}

void WebCtrlAnalyzeCore::rtSetStop(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("תֹ̨ͣת����ִ�гɹ���"), ip, port);

    gRot->stop();
}

void WebCtrlAnalyzeCore::rtSetDirecTurn(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸��ѯ
    if (!job.contains(Key::mRtDirecTurn))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int turn = job.value(Key::mRtDirecTurn).toInt(errVal);

    //���ͼ��
    if (turn == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
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
        sturn = QString("��ת");
        break;
    case Val::mRtDown:
        rot.mIdtDirec = rot.TurnDown;
        sturn = QString("��ת");
        break;
    case Val::mRtLeft:
        rot.mIdtDirec = rot.TurnLeft;
        sturn = QString("��ת");
        break;
    case Val::mRtRight:
        rot.mIdtDirec = rot.TurnRight;
        sturn = QString("��ת");
        break;
    default:
        return;
    }
    
    gParam->setDevRot(rot);

    log(QString("ת̨��̬���ƣ�ִ�гɹ�����ʼ%1��").arg(sturn), ip, port);

    //ת̨��̬����
    gRot->turnDirec();
}

void WebCtrlAnalyzeCore::rtSetVel(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
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

    //���ͼ��
    if (sign == errSign
        || abs(velAzi - errVel) < 0.5
        || abs(velPit - errVel) < 0.5)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
    if (sign != Val::mRtSignAzi
        && sign != Val::mRtSignPit
        && sign != Val::mRtSignAziPit
        || velAzi < 0.0
        || velPit < 0.0)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    QString slog = QString("����ת̨ת�٣�ִ�гɹ���");
    ParamDevRot rot = gParam->getDevRot();
    rot.mVelAzi = qBound(0.0, velAzi, (double)rot.VelHMax);
    rot.mVelPit = qBound(0.0, velPit, (double)rot.VelVMax);

    switch (sign)
    {
    case Val::mRtSignAzi:
        rot.mIdtVel = rot.AziOnly;
        slog.append(QString("��λת��%1��/s��").arg(rot.mVelAzi, 0, 'f', 2));
        break;
    case Val::mRtSignPit:
        rot.mIdtVel = rot.PitOnly;
        slog.append(QString("����ת��%1��/s��").arg(rot.mVelPit, 0, 'f', 2));
        break;
    case Val::mRtSignAziPit:
        rot.mIdtVel = rot.AziAndPit;
        slog.append(QString("��λת��%1��/s������ת��%2��/s��")
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
    //��Ŀ�뱸���
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

    //���ͼ��
    if (sign == errSign
        || abs(azi - errAngle) < 0.5
        || abs(pit - errAngle) < 0.5)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
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

    QString slog = QString("����ת̨פ���ǣ�ִ�гɹ���");
    ParamDevRot rot = gParam->getDevRot();
    rot.mAzi = XKCommon::angleToPos(azi);
    rot.mPit = XKCommon::angleToNeg(pit);

    switch (sign)
    {
    case Val::mRtSignAzi:
        rot.mIdtDeg = rot.AziOnly;
        slog.append(QString("��λ%1�㡣").arg(rot.mAzi, 0, 'f', 2));
        break;
    case Val::mRtSignPit:
        rot.mIdtDeg = rot.PitOnly;
        slog.append(QString("����%1�㡣").arg(rot.mPit, 0, 'f', 2));
        break;
    case Val::mRtSignAziPit:
        rot.mIdtDeg = rot.AziAndPit;
        slog.append(QString("��λ%1�㣬����%2�㡣")
            .arg(rot.mAzi, 0, 'f', 2).arg(rot.mPit, 0, 'f', 2));
        break;
    default:
        return;
    }

    gParam->setDevRot(rot);

    log(slog, ip, port);

    //����ת̨פ����
    gRot->resident();
}

void WebCtrlAnalyzeCore::rtSetZero(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
    if (!job.contains(Key::mRtZero))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int reset = job.value(Key::mRtZero).toInt(errVal);

    //���ͼ��
    if (reset == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
    if (reset != Val::mRtSignAzi
        && reset != Val::mRtSignPit
        && reset != Val::mRtSignAziPit)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    ParamDevRot rot = gParam->getDevRot();
    QString slog = QString("����ת̨���㣺ִ�гɹ���");

    switch (reset)
    {
    case Val::mRtSignAzi:
        rot.mIdtZero = rot.AziOnly;
        slog.append(QString("��λ���㡣"));
        break;
    case Val::mRtSignPit:
        rot.mIdtZero = rot.PitOnly;
        slog.append(QString("�������㡣"));
        break;
    case Val::mRtSignAziPit:
        rot.mIdtZero = rot.AziAndPit;
        slog.append(QString("��λ�͸������㡣"));
        break;
    default:
        return;
    }

    gParam->setDevRot(rot);
    log(slog, ip, port);

    //���ƹ���
    gRot->zero();
}

void WebCtrlAnalyzeCore::rtGetStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("��ѯת̨�豸״̬��ִ�гɹ���"), ip, port);

    gWebCtrlSend->repRtStat(true, ip, port);
}

void WebCtrlAnalyzeCore::lsSetAttack(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
    if (!job.contains(Key::mLsAttackCode))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int attack = job.value(Key::mLsAttackCode).toInt(errVal);

    //���ͼ��
    if (attack == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
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
        log(QString("ǿ����ƣ��رա�"), ip, port);
        gLaser->turnOffBeingOn();
        break;
    case Val::mLsBeat:
        log(QString("ǿ����ƣ�ֱ�䡣"), ip, port);
        gLaser->beat(gParam->getSimple().mPrmLaser.mColorBeat);
        break;
    case Val::mLsWarning:
        log(QString("ǿ����ƣ���ʾ��"), ip, port);
        gLaser->warning(gParam->getSimple().mPrmLaser.mColorWarning);
        break;
    case Val::mLsFlash:
        log(QString("ǿ����ƣ�������"), ip, port);
        gLaser->burstFlash(gParam->getSimple().mPrmLaser.mColorBurstFlash);
        break;
    default:
        return;
    }
}

void WebCtrlAnalyzeCore::lsGetStat(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ�Ӳ�ѯ

    log(QString("��ѯǿ���豸״̬��ִ�гɹ���"), ip, port);

    gWebCtrlSend->repLsStat(true, ip, port);
}

void WebCtrlAnalyzeCore::coSetWorkModel(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //��Ŀ�뱸���
    if (!job.contains(Key::mCoWorkModel))
    {
        logLackKey(ip, port);
        return;
    }

    int errVal = -1;
    int model = job.value(Key::mCoWorkModel).toInt(errVal);

    //���ͼ��
    if (model == errVal)
    {
        logErrorPrmType(ip, port);
        return;
    }

    //ֵ���
    if (model != Val::mCoWorkAuto
        && model != Val::mCoWorkManual)
    {
        logErrorPrmValue(ip, port);
        return;
    }

    ParamIntegrated itg = gParam->getInteg();
    QString slog = QString("�ۺϹ������ù���ģʽΪ");

    switch (model)
    {
    case Val::mCoWorkAuto:
        itg.mWorkModel = ParamIntegrated::WorkAuto;
        slog = slog + QString("�Զ�ģʽ");
        break;
    case Val::mCoWorkManual:
        itg.mWorkModel = ParamIntegrated::WorkManual;
        slog = slog + QString("�ֶ�ģʽ");
        break;
    default:
        return;
    }

    gParam->setInteg(itg);
    gConfig->saveToRestore();

    log(slog, ip, port);

    //ģʽ�л�
    gSynAuto->procWorkModel();
}

void WebCtrlAnalyzeCore::coGetWorkModel(
    const QJsonObject & job, const QString & ip, ushort port) const
{
    //�޲�����ֱ��ִ��

    log(QString("��ѯ�ۺϹ���-����ģʽ��ִ�гɹ���"), ip, port);

    gWebCtrlSend->repCoWorkModel(true, ip, port);
}

void WebCtrlAnalyzeCore::logLackKey(const QString & ip, ushort port) const
{
    gLog->write(QString("��Web %1:%2�����󣺲���������ȱʧ��").arg(ip).arg(port), true);
}

void WebCtrlAnalyzeCore::logErrorPrmType(const QString & ip, ushort port) const
{
    gLog->write(QString("��Web %1:%2�����󣺲������ʹ���").arg(ip).arg(port), true);
}

void WebCtrlAnalyzeCore::logErrorPrmValue(const QString & ip, ushort port) const
{
    gLog->write(QString("��Web %1:%2�����󣺲���ֵ����").arg(ip).arg(port), true);
}

void WebCtrlAnalyzeCore::log(QString slog, const QString & ip, ushort port) const
{
    slog = QString("��Web %1:%2 ִ�С�").arg(ip).arg(port) + slog;
    gLog->write(slog, true);
}

