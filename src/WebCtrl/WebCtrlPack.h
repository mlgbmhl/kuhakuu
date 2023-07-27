#pragma once

#include <QStringList>
#include <QByteArray>
#include "ProtocolWebCtrl.h"

class WebCtrlPack
{
public:
    using Fmt  = ProtocolWebCtrlFormat;
    using Type = PrtcWebCtrlCmdType;
    using Code = PrtcWebCtrlCmdCode;
    using Key = PrtcWebCtrlJsonKey;
    using Value = PrtcWebCtrlJsonValue;

    //��ѯӦ�� + �Զ��ϱ� ��ʶ
    typedef enum ReportFlag : int
    {
        Succ    = Value::mSucc,
        Failed  = Value::mFailed,
        AutoRep = Value::mRepAuto,
    }RepFlag;

    //��Ŀɾ�������
    typedef enum AcstDelFileResault : int
    {
        AcDelSucc    = Value::mAcDelSucc,
        AcDelErrFileName = Value::mAcDelFailFileNameErr,
        AcDelErrFileUsing = Value::mAcDelFailFileUsing,
        AcDelErrUndef = Value::mAcDelFailOtherErr,
    }DelFileReslt;

    //�ۺϹ����Զ�����ģʽ
    typedef enum CompWorkModelCode : int
    {
        CompWorkAuto = Value::mCoWorkAuto,
        CompWorkManual = Value::mCoWorkManual,       
    }CompWork;

    //ǿ������
    typedef enum LaserAttackCode
    {
        LsOff = Value::mLsOff,
        LsBeat = Value::mLsBeat,
        LsWarning = Value::mLsWarning,
        LsFlash = Value::mLsFlash,
    }LsAttCode;

    //�����豸״̬
    typedef class DevAcstStat
    {
    public:
        int    mPlayCode;    //����״̬��0���ţ�1��ͣ��2����
        int    mMute;        //������������0-��������1������
        int    mVolPlayer;   //������������0-100
        int    mLoop;        //����ģʽ��0�б�ѭ����1����ѭ����2���β���
        int    mAttackCode;  //ǿ�������0�رգ�1ǿ�����2�����
        int    mSpeakCode;   //Ԥ����ʵʱ��Ƶ��0�رգ�1������
        int    mVolSpeak;    //Ԥ����ʵʱ��Ƶ������0-100

        DevAcstStat()
        {
            memset(this, 0, sizeof(*this));
        }

        DevAcstStat(int play, int mute, int volPlayer, int loop, int attack)
        {
            mPlayCode = play;
            mMute = mute;
            mVolPlayer = volPlayer;
            mLoop = loop;
            mAttackCode = attack;
            mSpeakCode = 0;
            mVolSpeak = 0;
        }
    }AcStat;

    //ת̨�豸״̬
    typedef class DevRotaryStat
    {
    public:
        int    mMoveStat;       //�˶�״̬
        double mAzi;            //ʵʱ��λ�ǣ� 0-359.99
        double mPit;            //ʵʱ�����ǣ� -90 ~ +90
        double mAziFixed;       //��װ��λ�ǣ� 0-359.99
        double mPitFixed;       //��װ�����ǣ� -90 ~ +90
        double mVelAzi;         //��λת��
        double mVelPit;         //����ת��

        DevRotaryStat()
        {
            memset(this, 0, sizeof(*this));
        }
    }RtStat;

public:
    WebCtrlPack();
    ~WebCtrlPack();

    //RESPONSE
    QByteArray response(const QString& cmdCode) const;


    //����
    //Ӧ����������
    QByteArray repNtNethand(const ushort& heartbeat) const;



    //��¼����
    //Ӧ���û���¼
    QByteArray repLgLoginRequst(bool succ) const;

    //Ӧ���޸��û���������
    QByteArray repLgUserNamePasswordMod(bool succ) const;

    //Ӧ�������û���������
    QByteArray repLgUserNamePasswordForg(bool succ, 
        const QString& name, const QString& password) const;

    //Ӧ�� + �Զ��ϱ�����¼״̬ 
    QByteArray repLgLoginStat(RepFlag flag, bool logined) const;



    //�����豸����
    //Ӧ��ɾ��ָ���ļ�����Ŀ����succΪfalseʱ��������ȷ��д������
    //1-�ļ�������2-�ļ����ڱ�ռ�ã� 3-��������
    QByteArray repAcDelFile(DelFileReslt resualt) const;

    //Ӧ����Ŀ����
    QByteArray repAcFileNum(bool succ, int num) const;

    //Ӧ����Ŀ�б�
    QByteArray repAcFileList(bool succ, const QStringList& list) const;

    //Ӧ�� + �ض������Զ��ϱ�����ǰ��Ŀ��
    QByteArray repAcFileCurrent(RepFlag flag, const QString& fileName) const;

    //Ӧ�� + �ض������Զ��ϱ��������豸״̬
    QByteArray repAcStat(RepFlag flag, const AcStat& stat) const;


    //ת̨�豸����
    //Ӧ�� + �ض������Զ��ϱ� ת̨״̬
    QByteArray repRtStat(RepFlag flag, const RtStat& stat) const;


    //ǿ���豸����
    //Ӧ�� + �ض������Զ��ϱ� ǿ���豸״̬
    QByteArray repLsStat(RepFlag flag, LsAttCode attCode) const;


    //�ۺϹ���
    //Ӧ�� ����ģʽ
    QByteArray repCoWorkModel(bool succ, CompWork workModel) const;
};