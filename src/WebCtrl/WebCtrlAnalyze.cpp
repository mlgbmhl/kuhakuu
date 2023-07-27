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
    //���ÿ��Ƿְ���ƴ�������
    //ÿһ��Websocket�����յ�һ��������Text
    preAnalyze(data, ipSrc, portSrc);
}

void WebCtrlAnalyze::preAnalyze(
    const QByteArray & data, const QString & ipSrc, const ushort & portSrc)
{
    //����Json����������ʧ�ܣ�˵�����ݲ�����Json��ʽ������ʧ��
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
    {
        gLog->write(QString("��Web������ʧ�ܣ����ݸ�ʽ���󣨼���Json����ʧ�ܣ���"), true);
        return;
    }

    //�ж�json�����Ƿ�Ϸ������Ϸ���˵�����ݸ�ʽ���󣬽���ʧ��
    if (!doc.isObject())
    {
        gLog->write(QString("��Web������ʧ�ܣ����ݸ�ʽ����Json�����ʽ���󣩣�"), true);
        return;
    }

    //ȡ��Json����
    QJsonObject obRecv = doc.object();

    using Fmt = PrtcWebCtrlFmt;
    if (!obRecv.contains(Fmt::mType) || !obRecv.contains(Fmt::mCode))
    {
        gLog->write(QString("��Web������ʧ�ܣ��ؼ�������ȱʧ��cmdType��cmdCodeȱʧ����"), true);
        return;
    }

    using Ty = PrtcWebCtrlCmdType;
    using Cd = PrtcWebCtrlCmdCode;
    
    QString cmdType, cmdCode;
    cmdType = obRecv.value(Fmt::mType).toString();
    cmdCode = obRecv.value(Fmt::mCode).toString();

    //ֻҪ�����������������¼ָ�������־
    if (cmdType != Ty::mNet)
    {
        gLog->write(QString("��Web���յ�����%1��%2��ָ����� = %3�� ָ���� = %4��")
            .arg(ipSrc).arg(portSrc)
            .arg(cmdType).arg(cmdCode), true);
    }

    //�ж�ָ�����Ƿ���Ч����Ч�򷵻�
    if (!mMapCodeToCore.contains(cmdCode))
    {
        gLog->write(QString("��Web��ָ�������%1��").arg(cmdCode), true);
        return;
    }

    //����ָ���ֱ������ͨ�ţ������Ƿ��ѵ�¼����������Ҫ�û���¼�����ִ��
    bool isValidMsg = false;
    if (mVcCodesPermitAny.contains(cmdCode))
    {
        isValidMsg = true;
    }
    else
    {
        //��������������û��ѵ�¼��������Чͨ��
        isValidMsg = gWebCtrlUsers->isAvailableUser(ipSrc, portSrc);
    }

    //��Ϊ������ͨ��״̬������ʾ����ֱ�ӷ���
    if (!isValidMsg)
    {
        gLog->write(QString("��Web��ָ��ִ��ʧ�ܣ���ǰ�û�%1��%2δ��¼��"
            "���߱������������������¼�����ͨ��Ȩ�ޣ�").arg(ipSrc).arg(portSrc), true);
        return;
    }

    //����ΪSET��GET�ı��ģ�������ִһ��Ӧ����   
    if (cmdType == Ty::mSet || cmdType == Ty::mGet)
    {
        gWebCtrlSend->response(cmdCode, ipSrc, portSrc);
    }

    //����cmdCode���ַ��������ִ�к���
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

