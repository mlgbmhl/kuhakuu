#pragma once

#include <QString>

//���ø�ʽ
typedef class ProtocolWebCtrlFormat
{
public:
    static const QString mType;
    static const QString mCode;
}PrtcWebCtrlFmt;

//��������
typedef class PtotocolWebCtrlCmdType
{
public:
    static const QString mNone;
    static const QString mSet;
    static const QString mGet;
    static const QString mNet;
    static const QString mResponse;
    static const QString mReport;
}PrtcWebCtrlCmdType;


//ָ����
typedef class ProtocolWebCtrlCmdCode
{
public:
    //����
    static const QString mNtHeartBeat;    //NET ��������

    //�û���¼
    static const QString mLgSetLogin;          //SET �û���¼
    static const QString mLgSetUserKeyMod;     //SET �޸��û���������
    static const QString mLgSetKeyForg;        //SET ��������
    static const QString mLgGetLoginStat;      //GET ��ѯ��¼״̬
    static const QString mLgRepLoginOp;        //REPORT ��¼��Ӧ���ɹ�/ʧ��
    static const QString mLgRepUserKeyMod;     //REPORT �޸��û���/������Ӧ���޸ĳɹ�/ʧ��
    static const QString mLgRepLoginStat;      //REPORT �ϱ���¼״̬����ʱ�ϱ� + ������ѯ�ϱ�
    static const QString mLgRepKeyForg;        //REPORT ����������Ӧ���ɹ�/ʧ�ܡ��û���������

    //�����豸����
    static const QString mAcSetPlayFile;       //SET ���ò�����Ŀ�ļ���
    static const QString mAcSetPlay;           //SET ���ſ��ƣ�����/��ͣ/����
    static const QString mAcSetMute;           //SET �������ƣ�����/ȡ������
    static const QString mAcSetVolume;         //SET ��������0-100
    static const QString mAcSetLoop;           //SET ����ģʽ���ã��б�ѭ��/����ѭ��/����ѭ��
    static const QString mAcSetDelFile;        //SET ɾ��ָ���ļ�������Ŀ
    static const QString mAcSetAttack;         //SET ǿ�����ƣ��ر�/ǿ���/�����
    static const QString mAcGetFileNum;        //GET ��ѯ��Ŀ����
    static const QString mAcGetFileList;       //GET ��ѯ��Ŀ�б�
    static const QString mAcGetFilePlaying;    //GET ��ѯ��ǰ���ŵ���Ŀ����
    static const QString mAcGetDevStat;        //GET ��ѯ�����豸״̬������״̬��������ѭ��ģʽ
    static const QString mAcGetAttackStat;     //GET ��ѯǿ�����״̬
    static const QString mAcRepDelFile;        //REPORT ��Ŀɾ�������Ӧ��ɾ���ɹ�/ʧ��
    static const QString mAcRepFileNum;        //REPORT �ϱ���Ŀ��������ѯ���ϱ�
    static const QString mAcRepFileList;       //REPORT �ϱ���Ŀ�б���ѯ���ϱ�
    static const QString mAcRepFilePlaying;    //REPORT �ϱ���ǰ���ŵ���Ŀ������ѯ���ϱ� + �ض������Զ��ϱ�
    static const QString mAcRepDevStat;        //REPORT �ϱ������豸״̬����ѯ���ϱ� + ��ʱ�ϱ�

    //ת̨�豸����
    static const QString mRtSetStop;           //SET ֹͣת��
    static const QString mRtSetDirecTurn;      //SET ��̬���ƣ���������ת��
    static const QString mRtSetVel;            //SET ת�����ã���λת�١�����ת��
    static const QString mRtSetResident;       //SET פ�������á�ת������λפ��/����פ��
    static const QString mRtSetZero;           //SET ���㣺��λ����/��������
    static const QString mRtGetStat;           //GET ��ѯת̨״̬
    static const QString mRtRepStat;           //REPORT �ϱ�ת̨״̬����ѯ���ϱ� + ��ʱ�ϱ�

    //ǿ���豸
    static const QString mLsSetAttack;         //SET ǿ����ƣ��ر�/ֱ��/��ʾ/����
    static const QString mLsGetStat;           //GET ��ѯǿ��״̬
    static const QString mLsRepStat;           //REPORT �ϱ�ǿ��״̬

    //�ɼ�������ͷ�豸����Ԥ�����ݲ�ʵ�֣�
    //static const QString mCmSetFocus;          //SET �۽����ƣ�+/-/ֹͣ
    //static const QString mCmSetZoom;           //SET �䱶���ƣ�+/-/ֹͣ

    //��������ͷ�豸����Ԥ�����ݲ�ʵ�֣� infrared

    //�ۺϹ��� comprehensive
    static const QString mCoSetWorkModel;      //SET ����ģʽ
    static const QString mCoGetWorkModel;      //GET ����ģʽ
    static const QString mCoRepWorkModel;      //REPORT ����ģʽ
    

    //�������ù����ڶ��׶����񣬵�ǰ�ݲ�ʵ�֣�


    //�����ÿ��ƣ�Ԥ�����ݲ�ʵ��

}PrtcWebCtrlCmdCode;



//Jsonָ��key
typedef class ProtocolWebCtrlJsonKey
{
public:
    //ͨ��
    static const QString mFlag;                 //ͨ��flag

    //��������
    static const QString mNtHeartBeat;

    //�û���¼����
    //�û���¼
    static const QString mLgLoginRequst;        //��¼���������¼���˳���¼
    static const QString mLgUserName;           //�û���
    static const QString mLgPassword;           //����
    static const QString mLgRepLogin;           //��Ӧ����������������ɹ�/ʧ��
    //�޸��û���������
    static const QString mLgUserNameOld;        //ԭ�û���
    static const QString mLgPasswordOld;        //ԭ����
    static const QString mLgUserNameNew;        //���û���
    static const QString mLgPasswordNew;        //������
    //�����û���������
    //��¼״̬
    static const QString mLgStatRepStatus;      //��¼״̬��Ӧ��δ��¼/�ѵ�¼


    //�����豸����
    static const QString mAcFileName;           //��Ŀ��
    static const QString mAcPlayCode;           //�����룺����/��ͣ/����
    static const QString mAcMuteCode;           //�����룺������/������
    static const QString mAcVolPlayer;          //����������0-100
    static const QString mAcLoopMode;           //����ģʽ��ѭ��ģʽ�����б�ѭ��/����ѭ��/����ѭ��
    static const QString mAcDelRepFlag;         //ɾ����Ӧ���ɹ�/ʧ�ܣ��ļ�������/ʧ�ܣ��ļ����ڱ�ռ�ã�/ʧ�ܣ���������
    static const QString mAcAttackCode;         //ǿ��������ر�/ǿ���/�����
    static const QString mAcFileNum;            //��Ŀ����
    static const QString mAcFileList;           //��Ŀ�б�
    //״̬
    static const QString mAcStPlay;             //����/��ͣ/����
    static const QString mAcStMute;             //������/������
    static const QString mAcStVolPlayer;        //����������0-100
    static const QString mAcStLoop;             //�б�ѭ��/����ѭ��/����ѭ��
    static const QString mAcStAttack;           //�ر�/ǿ���/�����
    static const QString mAcStSpeak;            //ʵʱ��Ƶ���ر�/�����У�Ԥ��
    static const QString mAcStVolSpeak;         //ʵʱ��Ƶ����0-100�� Ԥ��


    //ת̨�豸����
    static const QString mRtDirecTurn;          //��̬���ƣ���������ת��
    static const QString mRtAPSign;             //��λ��������ʶ
    static const QString mRtVelAzi;             //��λת��
    static const QString mRtVelPit;             //����ת��
    static const QString mRtResAzi;             //��λפ����
    static const QString mRtResPit;             //����פ����
    static const QString mRtZero;               //����
    //״̬
    static const QString mRtStMoveState;        //ת̨�˶�״̬����λ/�˶���/��Ч
    static const QString mRtStAzi;              //ʵʱ��λ�ǣ�0-359.9
    static const QString mRtStPit;              //ʵʱ�����ǣ�-90��90
    static const QString mRtStAziFix;           //��װ��λ�ǣ�0-359.9
    static const QString mRtStPitFix;           //��װˮƽ�ǣ�-90��90
    static const QString mRtStVelAzi;           //��λת��
    static const QString mRtStVelPit;           //����ת��


    //ǿ���豸����
    static const QString mLsAttackCode;         //ǿ����ƣ��ر�/ֱ��/��ʾ/����

    //�ۺϹ��� comprehensive
    static const QString mCoWorkModel;          //����ģʽ���Զ�ģʽ/�ֶ�ģʽ
}PrtcWebCtrlJsonKey;


//Jsonָ��value
typedef class ProtocolWebCtrlJsonValue
{
public:
    //ͨ��
    static const int mSucc = 0;                 //ͨ�óɹ�
    static const int mFailed = 1;               //ͨ��ʧ��
    static const int mRepAuto = 2;              //ͨ�ò�ѯ��Ӧ�������ϱ�

    //�û���¼
    static const int mLgLoginRequst = 0;          //�����¼
    static const int mLgLonoutRequst = 1;         //�˳���¼
    //��¼״̬
    static const int mLgStatLoginNot = 0;         //δ��¼
    static const int mLgStatLogined = 1;          //�ѵ�¼ 


    //�����豸����
    static const int mAcPlayCodePlay = 0;         //����
    static const int mAcPlayCodePause = 1;        //��ͣ
    static const int mAcPlayCodeStop  = 2;        //����
    static const int mAcMuteOn = 0;               //������
    static const int mAcMuteOff = 1;              //������
    static const int mAcLoopList = 0;             //�б�ѭ��
    static const int mAcLoopOne = 1;              //����ѭ��
    static const int mAcLoopNone = 2;             //���β��ţ���ѭ����
    static const int mAcDelSucc = 0;              //ɾ���ɹ�
    static const int mAcDelFailFileNameErr = 1;   //ɾ��ʧ�ܣ��ļ�������
    static const int mAcDelFailFileUsing = 2;     //ɾ��ʧ�ܣ��ļ����ڱ�ռ��
    static const int mAcDelFailOtherErr = 3;      //ɾ��ʧ�ܣ���������
    static const int mAcAttackOff = 0;            //ǿ��������ر�
    static const int mAcAttackStrong = 1;         //ǿ�������ǿ���
    static const int mAcAttackWeak = 2;           //ǿ������������
    static const int mAcSpaekOff = 0;             //ʵʱ��Ƶ���أ�Ԥ��
    static const int mAcSpeakOn = 1;              //ʵʱ��Ƶ������Ԥ��
    

    //ת̨�豸����
    static const int mRtUp = 0;                   //��ת
    static const int mRtDown = 1;
    static const int mRtLeft = 2;
    static const int mRtRight = 3;
    static const int mRtSignAzi = 0;              //��Ч��ǣ���λ��Ч
    static const int mRtSignPit = 1;              //��Ч��ǣ�������Ч
    static const int mRtSignAziPit = 2;           //��Ч��ǣ���λ�͸�������Ч
    //״̬
    static const int mRtStMoveArrived = 0;        //�˶�״̬����λ
    static const int mRtStMoving = 1;             //�˶�״̬���˶���
    static const int mRtStMoveInvalid = 0xFF;     //�˶�״̬����Ч


    //ǿ���豸����
    static const int mLsOff = 0;                  //�ر�
    static const int mLsBeat = 1;                 //ֱ��
    static const int mLsWarning = 2;              //��ʾ
    static const int mLsFlash = 3;                //����


    //�ۺϹ��� comprehensive
    static const int mCoWorkAuto = 0;             //������ʧ���Զ�ģʽ
    static const int mCoWorkManual = 1;           //����ģʽ���ֶ�ģʽ
    
}PrtcWebCtrlJsonValue;


