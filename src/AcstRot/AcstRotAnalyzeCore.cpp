#include "AcstRotAnalyzeCore.h"
#include "ProtocolAcstRot.h"
#include "DM/Manag/StatusManager.h"
#include "DM/Manag/ParameterManager.h"
#include "DM/File/ConfigManager.h"
#include "PD/LogManag/Log.h"
#include "ARCmdCodeDescrible.h"
#include "SI/Communic/NetworkManager.h"
#include "AcstRotPack.h"
#include "SI/Common/XKCommon.h"
#include "SI/LocalSys/SysVolumeHandler.h"
#include "PD/DevManag/AccessManager.h"
#include "DM/File/MusicFileManager.h"
#include "PD/DevManag/MusicPlayerManager.h"
#include "PD/DevManag/RotManager.h"
#include "PD/DataProcess/SynWorkAuto.h"
#include "SI/LocalSys/SysCtrlHandler.h"
#include <QCoreApplication>


AcstRotAnalyzeCore::AcstRotAnalyzeCore()
{
	mIpSrc = QString();
	mPortSrc = 0;
}

AcstRotAnalyzeCore::~AcstRotAnalyzeCore()
{

}

void AcstRotAnalyzeCore::netHand(const QByteArray& data)
{
	PrtARNetHand ob;
	memcpy(&ob, data.constData(), sizeof(ob));
	
	PrtARNetHand cmd;
	cmd.mPrep.mType = ProtcARFmt::MsgNet;
	cmd.mPrep.mAddrSrc = ob.mPrep.mAddrDst;
	cmd.mPrep.mAddrDst = ob.mPrep.mAddrSrc;
	cmd.mData.mNo = ob.mData.mNo;

	send(PrtARPack::pack(&cmd, sizeof(cmd)));
}


//Access
void AcstRotAnalyzeCore::accApply(const QByteArray& data)
{
	PrtAccApply ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mCmd > ob.FreeReject)
	{
		writeLog("申请控制权指令码错误！");
		return;
	}

	QStringList lst = QStringList()
		<< "请求控制权" << "取消请求控制权"
		<< "同意释放控制权" << "拒绝释放控制权";
	writeLog(QString("%1。").arg(lst.at(ob.mData.mCmd)));

	//提取用户信息
	ParamAccess::UserInfo user(mIpSrc, mPortSrc, ob.mPrep.mAddrSrc);

	//权限管理：处理申请
	gAccess->apply(user, ob.mData.mCmd);
}

void AcstRotAnalyzeCore::accQuery(const QByteArray& data)
{
	PrtAccQuery ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	//提取用户信息
	ParamAccess::UserInfo user(mIpSrc, mPortSrc, ob.mPrep.mAddrSrc);

	//权限查询回应
	gAccess->query(user);
}

//Acoustic
void AcstRotAnalyzeCore::acstSetFlName(const QByteArray& data)
{
	PrtAcstSetFlName ob;
	ob.fromBytes(data.constData(), data.size());

	if (!gParam->isMscExisted(ob.mData.mName))
	{
		writeLog(QString("设置当前曲目出错，文件《%1》不存在！").arg(ob.mData.mName));
		return;
	}

	//更新曲目管理
	gParam->setMscCurr(ob.mData.mName);
	writeLog(QString("设置当前曲目为《%1》。").arg(ob.mData.mName));

	//更新播放器播放曲目
	gMscPlayerManag->setFile();
}

void AcstRotAnalyzeCore::acstPlayCtrl(const QByteArray& data)
{
	PrtAcstPlayCtrl ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mCmd > ob.PlayStop)
	{
		writeLog(QString("控制播放器指令码（%1）错误！").arg(ob.mData.mCmd));
		return;
	}

	ParamDevAcst acst = gParam->getDevAcst();
	acst.mPlay = ob.mData.mCmd;
	gParam->setDevAcst(acst);

	QStringList lst = QStringList() << "播放" << "暂停" << "结束";
	writeLog(QString("播放器%1。").arg(lst.at(acst.mPlay)));

	//播放器控制
	gMscPlayerManag->setPlay();
}

void AcstRotAnalyzeCore::acstMuteCtrl(const QByteArray& data)
{
	PrtAcstMuteCtrl ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mMute > ob.MuteOff)
	{
		writeLog(QString("静音控制指令码（%1）错误！").arg(ob.mData.mMute));
		return;
	}

	ParamDevAcst acst = gParam->getDevAcst();
	acst.mMute = ob.mData.mMute;
	gParam->setDevAcst(acst);

	QStringList lst = QStringList() << "开启静音" << "关闭静音" ;
	writeLog(QString("%1。").arg(lst.at(acst.mMute)));

	//播放器静音控制
	gMscPlayerManag->setMute();
}

void AcstRotAnalyzeCore::acstVolumeCtrl(const QByteArray& data)
{
	PrtAcstVolumeCtrl ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.VolBoth)
	{
		writeLog(QString("音量控制有效标记位（%1）错误！").arg(ob.mData.mIdt));
		return;
	}

	QString slog;

	ParamDevAcst acst = gParam->getDevAcst();
	bool isSetPlayer = false;
	bool isSetDev = false;
	switch (ob.mData.mIdt)
	{
	case ob.VolPlayer:
		acst.mVolume = qBound(0, (int)ob.mData.mVolPlayer, 100);
		slog.append(QString("设置播放器音量为%1。").arg(acst.mVolume));
		isSetPlayer = true;
		break;
	case ob.VolDevice:
		acst.mVolumeSys = qBound(0, (int)ob.mData.mVolDevice, 100);
		slog.append(QString("设置工控机音量为%1。").arg(acst.mVolumeSys));
		isSetDev = true;
		break;
	case ob.VolBoth:
		acst.mVolume = qBound(0, (int)ob.mData.mVolPlayer, 100);
		acst.mVolumeSys = qBound(0, (int)ob.mData.mVolDevice, 100);
		slog.append(QString("设置播放器音量为%1，工控机音量为%2。")
			.arg(acst.mVolume).arg(acst.mVolumeSys));
		isSetPlayer = true;
		isSetDev = true;
		break;
	default:
		break;
	}
	gParam->setDevAcst(acst);

	writeLog(slog);

	//播放器音量控制
	if (isSetPlayer)
	{
		gMscPlayerManag->setVol();
	}
	
	//系统音量控制
	if (isSetDev)
	{
		gSysVolHdlr->setVol(acst.mVolumeSys);
	}
}

void AcstRotAnalyzeCore::acstLoopModel(const QByteArray& data)
{
	PrtAcstLoopModel ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mLoop > ob.LoopOne)
	{
		writeLog(QString("播放模式指令码（%1）错误！").arg(ob.mData.mLoop));
		return;
	}

	ParamDevAcst acst = gParam->getDevAcst();
	acst.mLoop = ob.mData.mLoop;
	gParam->setDevAcst(acst);

	QStringList lst = QStringList() << "列表循环" << "单曲循环";
	writeLog(QString("设置播放模式为%1。").arg(lst.at(acst.mLoop)));
}

void AcstRotAnalyzeCore::acstDeleteFile(const QByteArray& data)
{
	PrtAcstDeleteFile ob;
	ob.fromBytes(data.constData(), data.size());

	QString name = ob.mData.mName;
	if (!gParam->isMscExisted(name))
	{
		writeLog(QString("删除曲目出错：文件《%1》不存在！").arg(name));
		send(gPackAR->acstRepDelete(PrtAcstRepDelete::ErrorName, ob.mPrep.mAddrSrc));
		return;
	}

	writeLog(QString("删除曲目《%1》。").arg(name));

	//删除曲目
	QString path = gParam->mscPath(name);
	bool succ = XKCommon::removeFile(path);

	QString slog = QString("删除曲目《%1》%2。")
		.arg(name).arg(succ ? "成功" : "失败，文件被占用。");
	int idt = succ ? PrtAcstRepDelete::Succeed : PrtAcstRepDelete::ErrorUsing;
	
	send(gPackAR->acstRepDelete(idt, ob.mPrep.mAddrSrc));
	writeLog(slog);

	//更新本地曲目列表
	gMscFlManag->updateList();
}

void AcstRotAnalyzeCore::acstQueryCount(const QByteArray& data)
{
	PrtAcstQueryCount ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	send(gPackAR->acstRepCount(ob.mPrep.mAddrSrc));
}

void AcstRotAnalyzeCore::acstQueryNames(const QByteArray& data)
{
	PrtAcstQueryNames ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	send(gPackAR->acstRepNames(ob.mPrep.mAddrSrc));
}

void AcstRotAnalyzeCore::acstQueryCurr(const QByteArray& data)
{
	PrtAcstQueryCurr ob;
	memcpy(&ob, data.constData(), sizeof(ob));
	send(gPackAR->acstRepCurr(ob.mPrep.mAddrSrc));
}

void AcstRotAnalyzeCore::acstQueryState(const QByteArray& data)
{
	PrtAcstQueryState ob;
	memcpy(&ob, data.constData(), sizeof(ob));
	send(gPackAR->acstRepState(ob.mPrep.mAddrSrc));
}

//Rotary
void AcstRotAnalyzeCore::rotStop(const QByteArray& data)
{
	writeLog("转台停止。");

	//下发转台停止指令
	gRot->stop();
}

void AcstRotAnalyzeCore::rotTurnDirec(const QByteArray& data)
{
	PrtRotTurnDirec ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.TurnRight)
	{
		writeLog(QString("转台姿态控制指令码（%1）错误！").arg(ob.mData.mIdt));
		return;
	}

	QStringList lst = QStringList() << "上转" << "下转" << "左转" << "右转";
	writeLog(QString("控制转台开始%1。").arg(lst.at(ob.mData.mIdt)));

	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtDirec = ob.mData.mIdt;
	gParam->setDevRot(rot);

	//转台姿态控制
	gRot->turnDirec();
}

void AcstRotAnalyzeCore::rotSetVel(const QByteArray& data)
{
	PrtRotSetVel ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.AziAndPit)
	{
		writeLog(QString("转台转速设置标记位（%1）错误！").arg(ob.mData.mIdt));
		return;
	}

	QString slog = QString("设置转台");
	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtVel = ob.mData.mIdt;
	switch (ob.mData.mIdt)
	{
	case ob.AziOnly:
		rot.mVelAzi = ob.mData.mVelAzi * 0.01;
		slog.append(QString("方位转速%1°/s。").arg(rot.mVelAzi, 0, 'f', 2));
		break;
	case ob.PitOnly:
		rot.mVelPit = ob.mData.mVelPit * 0.01;
		slog.append(QString("俯仰转速%1°/s。").arg(rot.mVelPit, 0, 'f', 2));
		break;
	case ob.AziAndPit:
		rot.mVelAzi = ob.mData.mVelAzi * 0.01;
		rot.mVelPit = ob.mData.mVelPit * 0.01;
		slog.append(QString("方位转速%1°/s，俯仰转速%2°/s。")
			.arg(rot.mVelAzi, 0, 'f', 2).arg(rot.mVelPit, 0, 'f', 2));
		break;
	default:
		break;
	}
	gParam->setDevRot(rot);

	writeLog(slog);

	//设置转台转速
	//当前云台无转速设置指令，放入数据中心，姿态控制时采用该转速
}

void AcstRotAnalyzeCore::rotTurnTo(const QByteArray& data)
{
	PrtRotTurnTo ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.AziAndPit)
	{
		writeLog(QString("转台驻留角设置标记位（%1）错误！").arg(ob.mData.mIdt));
		return;
	}

	QString slog = QString("控制转台转到");
	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtDeg = ob.mData.mIdt;
	switch (ob.mData.mIdt)
	{
	case ob.AziOnly:
		rot.mAzi = XKCommon::angleToPos(ob.mData.mAzi * 0.01);
		slog.append(QString("方位%1°。").arg(rot.mAzi, 0, 'f', 2));
		break;
	case ob.PitOnly:
		rot.mPit = XKCommon::angleToNeg(ob.mData.mPit * 0.01);
		slog.append(QString("俯仰%1°。").arg(rot.mPit, 0, 'f', 2));
		break;
	case ob.AziAndPit:
		rot.mAzi = XKCommon::angleToPos(ob.mData.mAzi * 0.01);
		rot.mPit = XKCommon::angleToNeg(ob.mData.mPit * 0.01);
		slog.append(QString("方位%1°，俯仰%2°。")
			.arg(rot.mAzi, 0, 'f', 2).arg(rot.mPit, 0, 'f', 2));
		break;
	default:
		break;
	}
	gParam->setDevRot(rot);

	writeLog(slog);

	//设置转台驻留角
	gRot->resident();
}

void AcstRotAnalyzeCore::rotZero(const QByteArray& data)
{
	PrtRotZero ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.AziAndPit)
	{
		writeLog(QString("转台归零标记位（%1）错误！").arg(ob.mData.mIdt));
		return;
	}

	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtZero = ob.mData.mIdt;
	gParam->setDevRot(rot);

	QStringList list = QStringList() << "方位" << "俯仰" << "方位、俯仰";
	QString slog = QString("控制转台%1归零。").arg(list[ob.mData.mIdt]);
	
	writeLog(slog);

	//控制归零
	gRot->zero();
}

void AcstRotAnalyzeCore::rotQueryState(const QByteArray& data)
{
	PrtRotQueryState ob;
	memcpy(&ob, data.constData(), sizeof(ob));
	send(gPackAR->rotRepState(ob.mPrep.mAddrSrc));
}

//Synthetical-Management
void AcstRotAnalyzeCore::synWorkModel(const QByteArray& data)
{
	PrtSynWorkModel ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.WorkManul)
	{
		writeLog(QString("综合管理-工作模式（%1）错误！").arg(ob.mData.mIdt));
		return;
	}

	ParamIntegrated itg = gParam->getInteg();
	itg.mWorkModel = ob.mData.mIdt;
	gParam->setInteg(itg);
	gConfig->saveToRestore();

	QStringList list = QStringList() << "自动模式" << "手动模式";
	writeLog(QString("设置综合管理-工作模式为%1。").arg(list[itg.mWorkModel]));

	//模式切换
	gSynAuto->procWorkModel();
}

void AcstRotAnalyzeCore::synPrmAuto(const QByteArray& data)
{
	PrtSynPrmAuto ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	ParamIntegrated itg = gParam->getInteg();
	itg.mPrmAuto.mTimeStart = ob.mData.mSecStart;
	itg.mPrmAuto.mTimeEnd = ob.mData.mSecEnd;

	itg.mPrmAuto.mAziStart = XKCommon::angleToPos(ob.mData.mAziStart * 0.01);
	itg.mPrmAuto.mAziEnd = XKCommon::angleToPos(ob.mData.mAziEnd * 0.01);
	itg.mPrmAuto.mAziStep = XKCommon::angleToPos(ob.mData.mAziStep * 0.01);

	itg.mPrmAuto.mPitStart = XKCommon::angleToNeg(ob.mData.mPitStart * 0.01);
	itg.mPrmAuto.mPitEnd = XKCommon::angleToNeg(ob.mData.mPitEnd * 0.01);
	itg.mPrmAuto.mPitStep = XKCommon::angleToPos(ob.mData.mPitStep * 0.01);

	itg.mPrmAuto.mVelAzi = ob.mData.mVelAzi * 0.01;
	itg.mPrmAuto.mVelPit = ob.mData.mVelPit * 0.01;

	itg.mPrmAuto.mSecStay = ob.mData.mSecResident;
	gParam->setInteg(itg);
	gConfig->saveToRestore();

	QString slog = QString("设置综合管理-自动模式参数：开始工作时间%1，结束工作时间%2；")
		.arg(itg.mPrmAuto.minuteToTime(itg.mPrmAuto.mTimeStart).toString("hh:mm:ss"))
		.arg(itg.mPrmAuto.minuteToTime(itg.mPrmAuto.mTimeEnd).toString("hh:mm:ss"));

	slog.append(QString("方位扫描%1°至%2°，步进%3°；")
		.arg(itg.mPrmAuto.mAziStart, 0, 'f', 2).arg(itg.mPrmAuto.mAziEnd, 0, 'f', 2)
		.arg(itg.mPrmAuto.mAziStep, 0, 'f', 2));

	slog.append(QString("俯仰扫描%1°至%2°，步进%3°（当前俯仰不扫描，仅驻留于起始角）；")
		.arg(itg.mPrmAuto.mPitStart, 0, 'f', 2).arg(itg.mPrmAuto.mPitEnd, 0, 'f', 2)
		.arg(itg.mPrmAuto.mPitStep, 0, 'f', 2));

	slog.append(QString("方位转速%1°/s，俯仰转速%2°/s；")
		.arg(itg.mPrmAuto.mVelAzi, 0, 'f', 2).arg(itg.mPrmAuto.mVelPit, 0, 'f', 2));

	slog.append(QString("驻留时间：%1s。").arg(itg.mPrmAuto.mSecStay));

	writeLog(slog);

	//重新处理
	gSynAuto->resetPrmAuto();
}

void AcstRotAnalyzeCore::synSetAutoRep(const QByteArray& data)
{
	PrtSynSetAutoRep ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	ParamIntegrated itg = gParam->getInteg();
	itg.mIsRepAuto.mAcst = (ob.mData.mAcst == ob.RepAutoOn);
	itg.mIsRepAuto.mRot = (ob.mData.mRot == ob.RepAutoOn);
	itg.mIsRepAuto.mInteg = (ob.mData.mSyn == ob.RepAutoOn);
	gParam->setInteg(itg);

	QString slog = QString("设置状态自动上报：声能设备%1；转台%2；综合状态%3。")
		.arg(itg.mIsRepAuto.mAcst ? "开" : "关")
		.arg(itg.mIsRepAuto.mRot ? "开" : "关")
		.arg(itg.mIsRepAuto.mInteg ? "开" : "关");
	writeLog(slog);
}

void AcstRotAnalyzeCore::synPowerOffPC(const QByteArray& data)
{
	writeLog("【指令】关闭工控机。");

	//app清理，延时关机
	gLog->write("执行：清理软件，并于5秒后关机。");
	
	gConfig->saveToRestore();
	qApp->quit();
	gSysCtrl->shutdown(5);
}

void AcstRotAnalyzeCore::synQueryState(const QByteArray& data)
{
	PrtSynQueryState ob;
	memcpy(&ob, data.constData(), sizeof(ob));
	send(gPackAR->synRepState(ob.mPrep.mAddrSrc));
}

void AcstRotAnalyzeCore::synQueryPrmAuto(const QByteArray& data)
{
	PrtSynQueryPrmAuto ob;
	memcpy(&ob, data.constData(), sizeof(ob));
	send(gPackAR->synRepPrmAuto(ob.mPrep.mAddrSrc));
}

void AcstRotAnalyzeCore::synTiming(const QByteArray & data)
{
	PrtSynTiming ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	QDateTime dtLoc = QDateTime::currentDateTime();
	QDateTime dtRem = QDateTime(
		QDate(ob.mData.mYear, ob.mData.mMon, ob.mData.mDay), 
		QTime(ob.mData.mHour, ob.mData.mMinute, ob.mData.mSec));

	ParameterSoft ps = gParam->getSoft();
	ps.mDtTimingRemote = dtRem;
	ps.mDtTimingLocal = dtLoc;
	gParam->setSoft(ps);

	gConfig->saveToRestore();

	QString slog = QString("校时：基准时间%1，此时本地时间%2。")
		.arg(ps.mDtTimingRemote.toString("yyyy-MM-dd hh:mm:ss"))
		.arg(ps.mDtTimingLocal.toString("yyyy-MM-dd hh:mm:ss"));
	gLog->write(slog, true);
}



//统一处理：权限处理、指令应答、IP及port更新
bool AcstRotAnalyzeCore::accessAndResp(const QByteArray& data, const QString& ipSrc, const ushort& portSrc)
{
	//更新ip，port
	mIpSrc = ipSrc;
	mPortSrc = portSrc;

	ProtcARFmt::Prep prep;
	memcpy(&prep, &data.constData()[0], sizeof(prep));

	//握手心跳：不需权限；单独应答
	if (prep.mType == ProtcARFmt::MsgNet)
	{
		return true;
	}

	//提取用户信息
	ParamAccess::UserInfo user(mIpSrc, mPortSrc, prep.mAddrSrc);

	QString slog = QString("收到%1：%2发来的指令【%3】")
		.arg(mIpSrc).arg(mPortSrc).arg(gARCodeDescr->codeToStr(prep.mCmdCode));


	//需要权限的指令，若当前用户无权限，则不响应
	if (false/*gARCodeDescr->needAccess(prep.mCmdCode)*/)
	{
		if (!gParam->isAccessed(user))
		{
			slog.append("，当前用户无控制权限，指令被忽略。");
			gLog->write(slog, true);
			return false;
		}

		//更新控制权用户的最新指令操作时间
		bool levelChanged = gParam->updateDtAccCommLast( QDateTime::currentDateTime() );
		if (levelChanged)
		{
			gLog->write(QString("权限用户%1由弱权限变更为正常权限。").arg(user.mIp), true);
		}
	}

	//当前处于自动模式下，则忽略转台控制指令
	if (gParam->isWorkAuto() &&  gARCodeDescr->ignoreWhenAuto(prep.mCmdCode))
	{
		slog.append("，当前处于自动工作模式，转台控制指令被忽略。");
		gLog->write(slog, true);
		return false;
	}

	slog.append("。");
	gLog->write(slog, true);

	//指令应答
	PrtARCmdReponse cmdResp(prep);
	send(cmdResp.pack());

	//告知调用方，该指令需要继续处理
	return true;
}


//往accessAndResp更新的IP、port发送数据
bool AcstRotAnalyzeCore::send(const QByteArray& cmd) const
{
	return gSrvCtrl->send(cmd, mIpSrc, mPortSrc);
}

void AcstRotAnalyzeCore::writeLog(const QString & slog) const
{
	QString str = QString("用户（%1：%2）指令：").arg(mIpSrc).arg(mPortSrc);
	str.append(slog);
	gLog->write(str, true);
}

