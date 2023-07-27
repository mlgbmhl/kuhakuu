#include "ProtocolWebCtrl.h"


const QString PrtcWebCtrlFmt::mType = QString("cmdType");
const QString PrtcWebCtrlFmt::mCode = QString("cmdCode");

//报文类型
const QString PrtcWebCtrlCmdType::mNone     = QString("NONE");
const QString PrtcWebCtrlCmdType::mSet      = QString("SET");
const QString PrtcWebCtrlCmdType::mGet      = QString("GET");
const QString PrtcWebCtrlCmdType::mNet      = QString("NET");
const QString PrtcWebCtrlCmdType::mResponse = QString("RESPONSE");
const QString PrtcWebCtrlCmdType::mReport   = QString("REPORT");


//指令码
//网络
const QString PrtcWebCtrlCmdCode::mNtHeartBeat     = QString("0x0001");
//用户登录                                           
const QString PrtcWebCtrlCmdCode::mLgSetLogin       = QString("0x0101");
const QString PrtcWebCtrlCmdCode::mLgSetUserKeyMod  = QString("0x0102");
const QString PrtcWebCtrlCmdCode::mLgSetKeyForg     = QString("0x0103");
const QString PrtcWebCtrlCmdCode::mLgGetLoginStat   = QString("0x0104");
const QString PrtcWebCtrlCmdCode::mLgRepLoginOp     = QString("0x0181");
const QString PrtcWebCtrlCmdCode::mLgRepUserKeyMod  = QString("0x0182");
const QString PrtcWebCtrlCmdCode::mLgRepLoginStat   = QString("0x0183");
const QString PrtcWebCtrlCmdCode::mLgRepKeyForg     = QString("0x0184");
//声能设备管理
const QString PrtcWebCtrlCmdCode::mAcSetPlayFile    = QString("0x0201");
const QString PrtcWebCtrlCmdCode::mAcSetPlay        = QString("0x0202");
const QString PrtcWebCtrlCmdCode::mAcSetMute        = QString("0x0203");
const QString PrtcWebCtrlCmdCode::mAcSetVolume      = QString("0x0204");
const QString PrtcWebCtrlCmdCode::mAcSetLoop        = QString("0x0205");
const QString PrtcWebCtrlCmdCode::mAcSetDelFile     = QString("0x0206");
const QString PrtcWebCtrlCmdCode::mAcSetAttack      = QString("0x0207");
const QString PrtcWebCtrlCmdCode::mAcGetFileNum     = QString("0x0208");
const QString PrtcWebCtrlCmdCode::mAcGetFileList    = QString("0x0209");
const QString PrtcWebCtrlCmdCode::mAcGetFilePlaying = QString("0x020A");
const QString PrtcWebCtrlCmdCode::mAcGetDevStat     = QString("0x020B");
const QString PrtcWebCtrlCmdCode::mAcGetAttackStat  = QString("0x020C");
const QString PrtcWebCtrlCmdCode::mAcRepDelFile     = QString("0x0281");
const QString PrtcWebCtrlCmdCode::mAcRepFileNum     = QString("0x0282");
const QString PrtcWebCtrlCmdCode::mAcRepFileList    = QString("0x0283");
const QString PrtcWebCtrlCmdCode::mAcRepFilePlaying = QString("0x0284");
const QString PrtcWebCtrlCmdCode::mAcRepDevStat     = QString("0x0285");
//转台设备管理
const QString PrtcWebCtrlCmdCode::mRtSetStop        = QString("0x0301");
const QString PrtcWebCtrlCmdCode::mRtSetDirecTurn   = QString("0x0302");
const QString PrtcWebCtrlCmdCode::mRtSetVel         = QString("0x0303");
const QString PrtcWebCtrlCmdCode::mRtSetResident    = QString("0x0304");
const QString PrtcWebCtrlCmdCode::mRtSetZero        = QString("0x0305");
const QString PrtcWebCtrlCmdCode::mRtGetStat        = QString("0x0306");
const QString PrtcWebCtrlCmdCode::mRtRepStat        = QString("0x0381");
//强光设备管理
const QString PrtcWebCtrlCmdCode::mLsSetAttack      = QString("0x0401");
const QString PrtcWebCtrlCmdCode::mLsGetStat        = QString("0x0402");
const QString PrtcWebCtrlCmdCode::mLsRepStat        = QString("0x0481");
//综合管理
const QString PrtcWebCtrlCmdCode::mCoSetWorkModel   = QString("0xA001");
const QString PrtcWebCtrlCmdCode::mCoGetWorkModel   = QString("0xA041");
const QString PrtcWebCtrlCmdCode::mCoRepWorkModel   = QString("0xA081");
                                               


//Json key
//通用
const QString PrtcWebCtrlJsonKey::mFlag             = QString("flag");

//网络心跳
const QString PrtcWebCtrlJsonKey::mNtHeartBeat      = QString("heartbeat");

//用户登录
const QString PrtcWebCtrlJsonKey::mLgLoginRequst    = QString("loginStatus");
const QString PrtcWebCtrlJsonKey::mLgUserName       = QString("userName");
const QString PrtcWebCtrlJsonKey::mLgPassword       = QString("password");
const QString PrtcWebCtrlJsonKey::mLgRepLogin       = QString("loginStatus");
//修改用户名和密码
const QString PrtcWebCtrlJsonKey::mLgUserNameOld    = QString("userNameOld");
const QString PrtcWebCtrlJsonKey::mLgPasswordOld    = QString("passwordOld");
const QString PrtcWebCtrlJsonKey::mLgUserNameNew    = QString("userNameNew");
const QString PrtcWebCtrlJsonKey::mLgPasswordNew    = QString("passwordNew");
//登录状态查询
const QString PrtcWebCtrlJsonKey::mLgStatRepStatus  = QString("loginStatus");



//声能设备管理
const QString PrtcWebCtrlJsonKey::mAcFileName       = QString("fileName");
const QString PrtcWebCtrlJsonKey::mAcPlayCode       = QString("playStatus");
const QString PrtcWebCtrlJsonKey::mAcMuteCode       = QString("muteStatus");
const QString PrtcWebCtrlJsonKey::mAcVolPlayer      = QString("playerVolume");
const QString PrtcWebCtrlJsonKey::mAcLoopMode       = QString("playMode");
const QString PrtcWebCtrlJsonKey::mAcDelRepFlag     = QString("flag");
const QString PrtcWebCtrlJsonKey::mAcAttackCode     = QString("voiceAttack");
const QString PrtcWebCtrlJsonKey::mAcFileNum        = QString("num");
const QString PrtcWebCtrlJsonKey::mAcFileList       = QString("list");
//状态
const QString PrtcWebCtrlJsonKey::mAcStPlay         = QString("playState");
const QString PrtcWebCtrlJsonKey::mAcStMute         = QString("muteState");
const QString PrtcWebCtrlJsonKey::mAcStVolPlayer    = QString("playVolume");
const QString PrtcWebCtrlJsonKey::mAcStLoop         = QString("playMode");
const QString PrtcWebCtrlJsonKey::mAcStAttack       = QString("voiceAttack");
const QString PrtcWebCtrlJsonKey::mAcStSpeak        = QString("speakState");
const QString PrtcWebCtrlJsonKey::mAcStVolSpeak     = QString("speakVolume");



//转台设备管理
const QString PrtcWebCtrlJsonKey::mRtDirecTurn      = QString("azimuth");   //???
const QString PrtcWebCtrlJsonKey::mRtAPSign         = QString("sign");
const QString PrtcWebCtrlJsonKey::mRtVelAzi         = QString("azimuthSp");
const QString PrtcWebCtrlJsonKey::mRtVelPit         = QString("pitchSp");
const QString PrtcWebCtrlJsonKey::mRtResAzi         = QString("azimuthAngle");
const QString PrtcWebCtrlJsonKey::mRtResPit         = QString("pitchAngle");
const QString PrtcWebCtrlJsonKey::mRtZero           = QString("reset");
//状态
const QString PrtcWebCtrlJsonKey::mRtStMoveState    = QString("turnState");
const QString PrtcWebCtrlJsonKey::mRtStAzi          = QString("azimuthAngle");
const QString PrtcWebCtrlJsonKey::mRtStPit          = QString("pitchAngle");
const QString PrtcWebCtrlJsonKey::mRtStAziFix       = QString("azimuthDeflection");
const QString PrtcWebCtrlJsonKey::mRtStPitFix       = QString("pitchDeflection");
const QString PrtcWebCtrlJsonKey::mRtStVelAzi       = QString("azimuthSpeech");
const QString PrtcWebCtrlJsonKey::mRtStVelPit       = QString("pitchSpeech");


//强光设备管理
const QString PrtcWebCtrlJsonKey::mLsAttackCode     = QString("glare");


//综合管理
const QString PrtcWebCtrlJsonKey::mCoWorkModel      = QString("workModel");
