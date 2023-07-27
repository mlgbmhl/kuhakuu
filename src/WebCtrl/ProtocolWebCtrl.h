#pragma once

#include <QString>

//公用格式
typedef class ProtocolWebCtrlFormat
{
public:
    static const QString mType;
    static const QString mCode;
}PrtcWebCtrlFmt;

//报文类型
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


//指令码
typedef class ProtocolWebCtrlCmdCode
{
public:
    //网络
    static const QString mNtHeartBeat;    //NET 网络心跳

    //用户登录
    static const QString mLgSetLogin;          //SET 用户登录
    static const QString mLgSetUserKeyMod;     //SET 修改用户名和密码
    static const QString mLgSetKeyForg;        //SET 忘记密码
    static const QString mLgGetLoginStat;      //GET 查询登录状态
    static const QString mLgRepLoginOp;        //REPORT 登录响应：成功/失败
    static const QString mLgRepUserKeyMod;     //REPORT 修改用户名/密码响应：修改成功/失败
    static const QString mLgRepLoginStat;      //REPORT 上报登录状态：定时上报 + 主动查询上报
    static const QString mLgRepKeyForg;        //REPORT 忘记密码响应：成功/失败、用户名、密码

    //声能设备管理
    static const QString mAcSetPlayFile;       //SET 设置播放曲目文件名
    static const QString mAcSetPlay;           //SET 播放控制：播放/暂停/结束
    static const QString mAcSetMute;           //SET 静音控制：静音/取消静音
    static const QString mAcSetVolume;         //SET 音量设置0-100
    static const QString mAcSetLoop;           //SET 播放模式设置：列表循环/单曲循环/单次循环
    static const QString mAcSetDelFile;        //SET 删除指定文件名的曲目
    static const QString mAcSetAttack;         //SET 强声控制：关闭/强打击/弱打击
    static const QString mAcGetFileNum;        //GET 查询曲目总数
    static const QString mAcGetFileList;       //GET 查询曲目列表
    static const QString mAcGetFilePlaying;    //GET 查询当前播放的曲目名称
    static const QString mAcGetDevStat;        //GET 查询声能设备状态：播放状态、音量、循环模式
    static const QString mAcGetAttackStat;     //GET 查询强声打击状态
    static const QString mAcRepDelFile;        //REPORT 曲目删除结果响应：删除成功/失败
    static const QString mAcRepFileNum;        //REPORT 上报曲目总数，查询后上报
    static const QString mAcRepFileList;       //REPORT 上报曲目列表，查询后上报
    static const QString mAcRepFilePlaying;    //REPORT 上报当前播放的曲目名：查询后上报 + 特定条件自动上报
    static const QString mAcRepDevStat;        //REPORT 上报声能设备状态，查询后上报 + 定时上报

    //转台设备管理
    static const QString mRtSetStop;           //SET 停止转动
    static const QString mRtSetDirecTurn;      //SET 姿态控制：上下左右转动
    static const QString mRtSetVel;            //SET 转速设置：方位转速、俯仰转速
    static const QString mRtSetResident;       //SET 驻留角设置、转动：方位驻留/俯仰驻留
    static const QString mRtSetZero;           //SET 归零：方位归零/俯仰归零
    static const QString mRtGetStat;           //GET 查询转台状态
    static const QString mRtRepStat;           //REPORT 上报转台状态：查询后上报 + 定时上报

    //强光设备
    static const QString mLsSetAttack;         //SET 强光控制：关闭/直射/警示/爆闪
    static const QString mLsGetStat;           //GET 查询强光状态
    static const QString mLsRepStat;           //REPORT 上报强光状态

    //可见光摄像头设备管理（预留，暂不实现）
    //static const QString mCmSetFocus;          //SET 聚焦控制：+/-/停止
    //static const QString mCmSetZoom;           //SET 变倍控制：+/-/停止

    //红外摄像头设备管理（预留，暂不实现） infrared

    //综合管理 comprehensive
    static const QString mCoSetWorkModel;      //SET 工作模式
    static const QString mCoGetWorkModel;      //GET 工作模式
    static const QString mCoRepWorkModel;      //REPORT 工作模式
    

    //参数配置管理（第二阶段任务，当前暂不实现）


    //不常用控制：预留，暂不实现

}PrtcWebCtrlCmdCode;



//Json指令key
typedef class ProtocolWebCtrlJsonKey
{
public:
    //通用
    static const QString mFlag;                 //通用flag

    //网络心跳
    static const QString mNtHeartBeat;

    //用户登录管理
    //用户登录
    static const QString mLgLoginRequst;        //登录请求：请求登录、退出登录
    static const QString mLgUserName;           //用户名
    static const QString mLgPassword;           //密码
    static const QString mLgRepLogin;           //响应参数：操作结果，成功/失败
    //修改用户名和密码
    static const QString mLgUserNameOld;        //原用户名
    static const QString mLgPasswordOld;        //原密码
    static const QString mLgUserNameNew;        //新用户名
    static const QString mLgPasswordNew;        //新密码
    //忘记用户名和密码
    //登录状态
    static const QString mLgStatRepStatus;      //登录状态响应：未登录/已登录


    //声能设备管理
    static const QString mAcFileName;           //曲目名
    static const QString mAcPlayCode;           //播放码：播放/暂停/结束
    static const QString mAcMuteCode;           //静音码：静音开/静音关
    static const QString mAcVolPlayer;          //播放器音量0-100
    static const QString mAcLoopMode;           //播放模式（循环模式）：列表循环/单曲循环/单次循环
    static const QString mAcDelRepFlag;         //删除响应：成功/失败（文件名错误）/失败（文件正在被占用）/失败（其他错误）
    static const QString mAcAttackCode;         //强声打击：关闭/强打击/弱打击
    static const QString mAcFileNum;            //曲目总数
    static const QString mAcFileList;           //曲目列表
    //状态
    static const QString mAcStPlay;             //播放/暂停/结束
    static const QString mAcStMute;             //静音开/静音关
    static const QString mAcStVolPlayer;        //播放器音量0-100
    static const QString mAcStLoop;             //列表循环/单曲循环/单次循环
    static const QString mAcStAttack;           //关闭/强打击/弱打击
    static const QString mAcStSpeak;            //实时音频：关闭/讲话中，预留
    static const QString mAcStVolSpeak;         //实时音频音量0-100， 预留


    //转台设备管理
    static const QString mRtDirecTurn;          //姿态控制：上下左右转动
    static const QString mRtAPSign;             //方位、俯仰标识
    static const QString mRtVelAzi;             //方位转速
    static const QString mRtVelPit;             //俯仰转速
    static const QString mRtResAzi;             //方位驻留角
    static const QString mRtResPit;             //俯仰驻留角
    static const QString mRtZero;               //归零
    //状态
    static const QString mRtStMoveState;        //转台运动状态：达位/运动中/无效
    static const QString mRtStAzi;              //实时方位角：0-359.9
    static const QString mRtStPit;              //实时俯仰角：-90至90
    static const QString mRtStAziFix;           //安装方位角：0-359.9
    static const QString mRtStPitFix;           //安装水平角：-90至90
    static const QString mRtStVelAzi;           //方位转速
    static const QString mRtStVelPit;           //俯仰转速


    //强光设备管理
    static const QString mLsAttackCode;         //强光控制：关闭/直射/警示/爆闪

    //综合管理 comprehensive
    static const QString mCoWorkModel;          //工作模式：自动模式/手动模式
}PrtcWebCtrlJsonKey;


//Json指令value
typedef class ProtocolWebCtrlJsonValue
{
public:
    //通用
    static const int mSucc = 0;                 //通用成功
    static const int mFailed = 1;               //通用失败
    static const int mRepAuto = 2;              //通用查询响应：主动上报

    //用户登录
    static const int mLgLoginRequst = 0;          //请求登录
    static const int mLgLonoutRequst = 1;         //退出登录
    //登录状态
    static const int mLgStatLoginNot = 0;         //未登录
    static const int mLgStatLogined = 1;          //已登录 


    //声能设备管理
    static const int mAcPlayCodePlay = 0;         //播放
    static const int mAcPlayCodePause = 1;        //暂停
    static const int mAcPlayCodeStop  = 2;        //结束
    static const int mAcMuteOn = 0;               //静音开
    static const int mAcMuteOff = 1;              //静音关
    static const int mAcLoopList = 0;             //列表循环
    static const int mAcLoopOne = 1;              //单曲循环
    static const int mAcLoopNone = 2;             //单次播放（无循环）
    static const int mAcDelSucc = 0;              //删除成功
    static const int mAcDelFailFileNameErr = 1;   //删除失败：文件名错误
    static const int mAcDelFailFileUsing = 2;     //删除失败：文件正在被占用
    static const int mAcDelFailOtherErr = 3;      //删除失败：其他错误
    static const int mAcAttackOff = 0;            //强声打击：关闭
    static const int mAcAttackStrong = 1;         //强声打击：强打击
    static const int mAcAttackWeak = 2;           //强声打击：弱打击
    static const int mAcSpaekOff = 0;             //实时音频：关，预留
    static const int mAcSpeakOn = 1;              //实时音频：开，预留
    

    //转台设备管理
    static const int mRtUp = 0;                   //上转
    static const int mRtDown = 1;
    static const int mRtLeft = 2;
    static const int mRtRight = 3;
    static const int mRtSignAzi = 0;              //有效标记：方位有效
    static const int mRtSignPit = 1;              //有效标记：俯仰有效
    static const int mRtSignAziPit = 2;           //有效标记：方位和俯仰均有效
    //状态
    static const int mRtStMoveArrived = 0;        //运动状态：达位
    static const int mRtStMoving = 1;             //运动状态：运动中
    static const int mRtStMoveInvalid = 0xFF;     //运动状态：无效


    //强光设备管理
    static const int mLsOff = 0;                  //关闭
    static const int mLsBeat = 1;                 //直射
    static const int mLsWarning = 2;              //警示
    static const int mLsFlash = 3;                //爆闪


    //综合管理 comprehensive
    static const int mCoWorkAuto = 0;             //工作迷失：自动模式
    static const int mCoWorkManual = 1;           //工作模式：手动模式
    
}PrtcWebCtrlJsonValue;


