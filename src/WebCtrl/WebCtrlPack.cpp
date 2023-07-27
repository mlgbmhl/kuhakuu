#include "WebCtrlPack.h"
#include <QJsonDocument> 
#include <QJsonObject>
#include <QJsonArray>


WebCtrlPack::WebCtrlPack()
{

}

WebCtrlPack::~WebCtrlPack()
{

}


static QJsonObject CreateJsonOb(const QString& type, const QString& code)
{
    QJsonObject ob;
    ob.insert(ProtocolWebCtrlFormat::mType, type);
    ob.insert(ProtocolWebCtrlFormat::mCode, code);
    return ob;
}

static QByteArray JsonObToByteArray(const QJsonObject& ob)
{
    QJsonDocument doc(ob);
    return doc.toJson();
}

QByteArray WebCtrlPack::response(const QString & cmdCode) const
{
    return JsonObToByteArray(CreateJsonOb(Type::mResponse, cmdCode));
}

QByteArray WebCtrlPack::repNtNethand(const ushort & heartbeat) const
{
    QJsonObject ob = CreateJsonOb(Type::mNet, Code::mNtHeartBeat);
    ob.insert(Key::mNtHeartBeat, heartbeat);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repLgLoginRequst(bool succ) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mLgRepLoginOp);

    int v = succ ? Value::mSucc : Value::mFailed;
    ob.insert(Key::mLgRepLogin, v);


    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repLgUserNamePasswordMod(bool succ) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mLgRepUserKeyMod);

    int v = succ ? Value::mSucc : Value::mFailed;
    ob.insert(Key::mFlag, v);


    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repLgUserNamePasswordForg(
    bool succ, const QString & name, const QString & password) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mLgRepKeyForg);

    int v = succ ? Value::mSucc : Value::mFailed;
    ob.insert(Key::mFlag, v);

    //第一阶段：直接明码传输
    ob.insert(Key::mLgUserName, name);
    ob.insert(Key::mLgPassword, password);


    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repLgLoginStat(RepFlag flag, bool logined) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mLgRepLoginStat);

    int v = (int)flag;
    ob.insert(Key::mFlag, v);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repAcDelFile(DelFileReslt resualt) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mAcRepDelFile);

    int v = int(resualt);
    ob.insert(Key::mFlag, v);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repAcFileNum(bool succ, int num) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mAcRepFileNum);

    int v = succ ? Value::mSucc : Value::mFailed;
    ob.insert(Key::mFlag, v);
    ob.insert(Key::mAcFileNum, num);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repAcFileList(bool succ, const QStringList & list) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mAcRepFileList);

    int v = succ ? Value::mSucc : Value::mFailed;
    ob.insert(Key::mFlag, v);
    ob.insert(Key::mAcFileNum, list.size());

    QJsonArray ja;
    for (int i=0; i < list.size(); i++)
    {
        ja.append(list.at(i));
    }

    ob.insert(Key::mAcFileList, ja);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repAcFileCurrent(RepFlag flag, const QString & fileName) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mAcRepFilePlaying);

    int v = int(flag);
    ob.insert(Key::mFlag, v);
    ob.insert(Key::mAcFileName, fileName);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repAcStat(RepFlag flag, const AcStat & stat) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mAcRepDevStat);

    int v = int(flag);
    ob.insert(Key::mFlag, v);
    
    ob.insert(Key::mAcStPlay, stat.mPlayCode);
    ob.insert(Key::mAcStMute, stat.mMute);
    ob.insert(Key::mAcStVolPlayer, stat.mVolPlayer);
    ob.insert(Key::mAcStLoop, stat.mLoop);
    ob.insert(Key::mAcStAttack, stat.mAttackCode);
    ob.insert(Key::mAcStSpeak, stat.mSpeakCode);
    ob.insert(Key::mAcStVolSpeak, stat.mVolSpeak);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repRtStat(RepFlag flag, const RtStat & stat) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mRtRepStat);

    int v = int(flag);
    ob.insert(Key::mFlag, v);

    ob.insert(Key::mRtStMoveState, stat.mMoveStat);
    ob.insert(Key::mRtStAzi, stat.mAzi);
    ob.insert(Key::mRtStPit, stat.mPit);
    ob.insert(Key::mRtStAziFix, stat.mAziFixed);
    ob.insert(Key::mRtStPitFix, stat.mPitFixed);
    ob.insert(Key::mRtStVelAzi, stat.mVelAzi);
    ob.insert(Key::mRtStVelPit, stat.mVelPit);

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repLsStat(RepFlag flag, LsAttCode attCode) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mLsRepStat);

    int v = int(flag);
    ob.insert(Key::mFlag, v);

    ob.insert(Key::mLsAttackCode, int(attCode));

    return JsonObToByteArray(ob);
}

QByteArray WebCtrlPack::repCoWorkModel(bool succ, CompWork workModel) const
{
    QJsonObject ob = CreateJsonOb(Type::mReport, Code::mCoRepWorkModel);

    int v = succ ? Value::mSucc : Value::mFailed;
    ob.insert(Key::mFlag, v);

    ob.insert(Key::mCoWorkModel, int(workModel));

    return JsonObToByteArray(ob);
}






