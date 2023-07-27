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

    //查询应答 + 自动上报 标识
    typedef enum ReportFlag : int
    {
        Succ    = Value::mSucc,
        Failed  = Value::mFailed,
        AutoRep = Value::mRepAuto,
    }RepFlag;

    //曲目删除结果码
    typedef enum AcstDelFileResault : int
    {
        AcDelSucc    = Value::mAcDelSucc,
        AcDelErrFileName = Value::mAcDelFailFileNameErr,
        AcDelErrFileUsing = Value::mAcDelFailFileUsing,
        AcDelErrUndef = Value::mAcDelFailOtherErr,
    }DelFileReslt;

    //综合管理自动工作模式
    typedef enum CompWorkModelCode : int
    {
        CompWorkAuto = Value::mCoWorkAuto,
        CompWorkManual = Value::mCoWorkManual,       
    }CompWork;

    //强光打击码
    typedef enum LaserAttackCode
    {
        LsOff = Value::mLsOff,
        LsBeat = Value::mLsBeat,
        LsWarning = Value::mLsWarning,
        LsFlash = Value::mLsFlash,
    }LsAttCode;

    //声能设备状态
    typedef class DevAcstStat
    {
    public:
        int    mPlayCode;    //播放状态：0播放，1暂停，2结束
        int    mMute;        //播放器静音：0-静音开，1静音关
        int    mVolPlayer;   //播放器音量：0-100
        int    mLoop;        //播放模式：0列表循环，1单曲循环，2单次播放
        int    mAttackCode;  //强声打击：0关闭，1强打击，2弱打击
        int    mSpeakCode;   //预留，实时音频：0关闭，1讲话中
        int    mVolSpeak;    //预留，实时音频音量：0-100

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

    //转台设备状态
    typedef class DevRotaryStat
    {
    public:
        int    mMoveStat;       //运动状态
        double mAzi;            //实时方位角， 0-359.99
        double mPit;            //实时俯仰角， -90 ~ +90
        double mAziFixed;       //安装方位角， 0-359.99
        double mPitFixed;       //安装俯仰角， -90 ~ +90
        double mVelAzi;         //方位转速
        double mVelPit;         //俯仰转速

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


    //网络
    //应答：网络心跳
    QByteArray repNtNethand(const ushort& heartbeat) const;



    //登录管理
    //应答：用户登录
    QByteArray repLgLoginRequst(bool succ) const;

    //应答：修改用户名和密码
    QByteArray repLgUserNamePasswordMod(bool succ) const;

    //应答：忘记用户名和密码
    QByteArray repLgUserNamePasswordForg(bool succ, 
        const QString& name, const QString& password) const;

    //应答 + 自动上报：登录状态 
    QByteArray repLgLoginStat(RepFlag flag, bool logined) const;



    //声能设备管理
    //应答：删除指令文件的曲目：当succ为false时，必须正确填写错误码
    //1-文件名错误；2-文件正在被占用； 3-其他错误
    QByteArray repAcDelFile(DelFileReslt resualt) const;

    //应答：曲目总数
    QByteArray repAcFileNum(bool succ, int num) const;

    //应答：曲目列表
    QByteArray repAcFileList(bool succ, const QStringList& list) const;

    //应答 + 特定条件自动上报：当前曲目名
    QByteArray repAcFileCurrent(RepFlag flag, const QString& fileName) const;

    //应答 + 特定条件自动上报：声能设备状态
    QByteArray repAcStat(RepFlag flag, const AcStat& stat) const;


    //转台设备管理
    //应答 + 特定条件自动上报 转台状态
    QByteArray repRtStat(RepFlag flag, const RtStat& stat) const;


    //强光设备管理
    //应答 + 特定条件自动上报 强光设备状态
    QByteArray repLsStat(RepFlag flag, LsAttCode attCode) const;


    //综合管理
    //应答 工作模式
    QByteArray repCoWorkModel(bool succ, CompWork workModel) const;
};