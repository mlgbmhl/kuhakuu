#include "ARCmdCodeDescrible.h"

ARCmdCodeDescrible::ARCmdCodeDescrible()
{
	mMutex = new QMutex();
	initMap();
	initVcAcc();
	initVcAutoIgn();
}

ARCmdCodeDescrible::~ARCmdCodeDescrible()
{

}

ARCmdCodeDescrible* ARCmdCodeDescrible::getInstance()
{
	static ARCmdCodeDescrible ins;
	return &ins;
}

void ARCmdCodeDescrible::initMap()
{
	mMap.insert( (ushort)ProtcARFmt::NetHand, QString("网络心跳") );

	
	mMap.insert( (ushort)ProtcARFmt::AccApply, QString("请求控制权") );
	mMap.insert( (ushort)ProtcARFmt::AccQuery, QString("查询控制权") );

	mMap.insert( (ushort)ProtcARFmt::AcstSetFlName, QString("设置播放曲目") );
	mMap.insert( (ushort)ProtcARFmt::AcstPlayCtrl, QString("播放控制") );
	mMap.insert( (ushort)ProtcARFmt::AcstMuteCtrl, QString("静音控制") );
	mMap.insert( (ushort)ProtcARFmt::AcstVolumeCtrl, QString("音量控制") );
	mMap.insert( (ushort)ProtcARFmt::AcstLoopModel, QString("设置播放模式") );
	mMap.insert( (ushort)ProtcARFmt::AcstDeleteFile, QString("删除指定文件名的曲目") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryCount, QString("查询曲目总数") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryNames, QString("查询曲目列表") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryCurr, QString("查询当前播放的曲目名") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryState, QString("查询声能设备状态") );

	mMap.insert( (ushort)ProtcARFmt::RotStop, QString("转台停止") );
	mMap.insert( (ushort)ProtcARFmt::RotTurnDirec, QString("转台姿态控制") );
	mMap.insert( (ushort)ProtcARFmt::RotSetVel, QString("转速设置") );
	mMap.insert( (ushort)ProtcARFmt::RotTurnTo, QString("转到驻留角") );
	mMap.insert( (ushort)ProtcARFmt::RotZero, QString("转台归零") );
	mMap.insert( (ushort)ProtcARFmt::RotQueryState, QString("查询转台状态") );

	mMap.insert( (ushort)ProtcARFmt::SynWorkModel, QString("工作模式控制") );
	mMap.insert( (ushort)ProtcARFmt::SynPrmAuto, QString("自动模式参数设置") );
	mMap.insert( (ushort)ProtcARFmt::SynSetAutoRep, QString("自动上报使能控制") );
	mMap.insert( (ushort)ProtcARFmt::SynPowerOffPC, QString("工控机关机") );
	mMap.insert((ushort)ProtcARFmt::SynQueryState, QString("查询综合状态"));
	mMap.insert((ushort)ProtcARFmt::SynQueryPrmAuto, QString("查询自动参数"));
	mMap.insert((ushort)ProtcARFmt::SynTiming, QString("校时"));
}

void ARCmdCodeDescrible::initVcAcc()
{
	//不需要控制权：网络握手；权限管理；所有查询
	mVcAcc
		<< ProtcARFmt::AcstSetFlName
		<< ProtcARFmt::AcstPlayCtrl
		<< ProtcARFmt::AcstMuteCtrl
		<< ProtcARFmt::AcstVolumeCtrl
		<< ProtcARFmt::AcstLoopModel
		<< ProtcARFmt::AcstDeleteFile

		<< ProtcARFmt::RotStop
		<< ProtcARFmt::RotTurnDirec
		<< ProtcARFmt::RotSetVel
		<< ProtcARFmt::RotTurnTo
		<< ProtcARFmt::RotZero

		<< ProtcARFmt::SynWorkModel
		<< ProtcARFmt::SynPrmAuto
		<< ProtcARFmt::SynSetAutoRep
		<< ProtcARFmt::SynPowerOffPC;
}

void ARCmdCodeDescrible::initVcAutoIgn()
{
	//自动模式下，所有上位机直接下发的转台控制指令均无效
	mVcAutoIgn
		<< ProtcARFmt::RotStop
		<< ProtcARFmt::RotTurnDirec
		<< ProtcARFmt::RotSetVel
		<< ProtcARFmt::RotTurnTo
		<< ProtcARFmt::RotZero;
}

QString ARCmdCodeDescrible::codeToStr(const ushort& code) const 
{
	QString s;

	mMutex->lock();
	if (mMap.contains(code))
	{
		s = mMap.value(code);
	}
	mMutex->unlock();

	return s;
}

bool ARCmdCodeDescrible::needAccess(const ushort & cmdCode) const
{
	bool res = false;

	mMutex->lock();
	res = mVcAcc.contains(cmdCode);
	mMutex->unlock();

	return res;
}

bool ARCmdCodeDescrible::ignoreWhenAuto(const ushort & cmdCode) const
{
	bool res = false;

	mMutex->lock();
	res = mVcAutoIgn.contains(cmdCode);
	mMutex->unlock();

	return res;
}
