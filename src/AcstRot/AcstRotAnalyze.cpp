#include "AcstRotAnalyze.h"
#include "ProtocolAcstRot.h"

AcstRotAnalyze::AcstRotAnalyze()
{
	mMutex = new QMutex();
	mCore = new AcstRotAnalyzeCore();
	initPrm();
}

AcstRotAnalyze* AcstRotAnalyze::getInstance()
{
	static AcstRotAnalyze ins;
	return &ins;
}

AcstRotAnalyze::~AcstRotAnalyze()
{
	delete mCore;
	delete mMutex;
}

void AcstRotAnalyze::initPrm()
{
	CoreInfo ci;

	//netHand
	ci = CoreInfo(sizeof(PrtARNetHand), &AcstRotAnalyzeCore::netHand);
	mMap.insert( (ushort)ProtcARFmt::NetHand, ci );


	//access
	ci = CoreInfo( sizeof(PrtAccApply), &AcstRotAnalyzeCore::accApply );
	mMap.insert( (ushort)ProtcARFmt::AccApply, ci );

	ci = CoreInfo( sizeof(PrtAccQuery), &AcstRotAnalyzeCore::accQuery );
	mMap.insert( (ushort)ProtcARFmt::AccQuery, ci );


	//acoustic
	ci = CoreInfo( sizeof(PrtAcstSetFlName), &AcstRotAnalyzeCore::acstSetFlName );
	mMap.insert( (ushort)ProtcARFmt::AcstSetFlName, ci );

	ci = CoreInfo( sizeof(PrtAcstPlayCtrl), &AcstRotAnalyzeCore::acstPlayCtrl );
	mMap.insert( (ushort)ProtcARFmt::AcstPlayCtrl, ci );

	ci = CoreInfo(sizeof(PrtAcstMuteCtrl), &AcstRotAnalyzeCore::acstMuteCtrl);
	mMap.insert((ushort)ProtcARFmt::AcstMuteCtrl, ci);

	ci = CoreInfo(sizeof(PrtAcstVolumeCtrl), &AcstRotAnalyzeCore::acstVolumeCtrl);
	mMap.insert((ushort)ProtcARFmt::AcstVolumeCtrl, ci);

	ci = CoreInfo(sizeof(PrtAcstLoopModel), &AcstRotAnalyzeCore::acstLoopModel);
	mMap.insert((ushort)ProtcARFmt::AcstLoopModel, ci);

	ci = CoreInfo(sizeof(PrtAcstDeleteFile), &AcstRotAnalyzeCore::acstDeleteFile);
	mMap.insert((ushort)ProtcARFmt::AcstDeleteFile, ci);

	ci = CoreInfo(sizeof(PrtAcstQueryCount), &AcstRotAnalyzeCore::acstQueryCount);
	mMap.insert((ushort)ProtcARFmt::AcstQueryCount, ci);

	ci = CoreInfo(sizeof(PrtAcstQueryNames), &AcstRotAnalyzeCore::acstQueryNames);
	mMap.insert((ushort)ProtcARFmt::AcstQueryNames, ci);

	ci = CoreInfo(sizeof(PrtAcstQueryCurr), &AcstRotAnalyzeCore::acstQueryCurr);
	mMap.insert((ushort)ProtcARFmt::AcstQueryCurr, ci);

	ci = CoreInfo(sizeof(PrtAcstQueryState), &AcstRotAnalyzeCore::acstQueryState);
	mMap.insert((ushort)ProtcARFmt::AcstQueryState, ci);

	//rot
	ci = CoreInfo(sizeof(PrtRotStop), &AcstRotAnalyzeCore::rotStop);
	mMap.insert((ushort)ProtcARFmt::RotStop, ci);

	ci = CoreInfo(sizeof(PrtRotTurnDirec), &AcstRotAnalyzeCore::rotTurnDirec);
	mMap.insert((ushort)ProtcARFmt::RotTurnDirec, ci);

	ci = CoreInfo(sizeof(PrtRotSetVel), &AcstRotAnalyzeCore::rotSetVel);
	mMap.insert((ushort)ProtcARFmt::RotSetVel, ci);

	ci = CoreInfo(sizeof(PrtRotTurnTo), &AcstRotAnalyzeCore::rotTurnTo);
	mMap.insert((ushort)ProtcARFmt::RotTurnTo, ci);

	ci = CoreInfo(sizeof(PrtRotZero), &AcstRotAnalyzeCore::rotZero);
	mMap.insert((ushort)ProtcARFmt::RotZero, ci);

	ci = CoreInfo(sizeof(PrtRotQueryState), &AcstRotAnalyzeCore::rotQueryState);
	mMap.insert((ushort)ProtcARFmt::RotQueryState, ci);


	//Synthetical-Management
	ci = CoreInfo(sizeof(PrtSynWorkModel), &AcstRotAnalyzeCore::synWorkModel);
	mMap.insert((ushort)ProtcARFmt::SynWorkModel, ci);

	ci = CoreInfo(sizeof(PrtSynPrmAuto), &AcstRotAnalyzeCore::synPrmAuto);
	mMap.insert((ushort)ProtcARFmt::SynPrmAuto, ci);

	ci = CoreInfo(sizeof(PrtSynSetAutoRep), &AcstRotAnalyzeCore::synSetAutoRep);
	mMap.insert((ushort)ProtcARFmt::SynSetAutoRep, ci);

	ci = CoreInfo(sizeof(PrtSynPowerOffPC), &AcstRotAnalyzeCore::synPowerOffPC);
	mMap.insert((ushort)ProtcARFmt::SynPowerOffPC, ci);

	ci = CoreInfo(sizeof(PrtSynQueryState), &AcstRotAnalyzeCore::synQueryState);
	mMap.insert((ushort)ProtcARFmt::SynQueryState, ci);

	ci = CoreInfo(sizeof(PrtSynQueryPrmAuto), &AcstRotAnalyzeCore::synQueryPrmAuto);
	mMap.insert((ushort)ProtcARFmt::SynQueryPrmAuto, ci);

	ci = CoreInfo(sizeof(PrtSynTiming), &AcstRotAnalyzeCore::synTiming);
	mMap.insert((ushort)ProtcARFmt::SynTiming, ci);

	mVcLenUnknown.append(ProtcARFmt::AcstSetFlName);
	mVcLenUnknown.append(ProtcARFmt::AcstDeleteFile);



	QVector<ushort> vcNet, vcSet, vcGet;
	vcNet << ProtcARFmt::NetHand;

	vcSet
		<< ProtcARFmt::AccApply
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
		<< ProtcARFmt::SynPowerOffPC
		<< ProtcARFmt::SynTiming;

	vcGet
		<< ProtcARFmt::AccQuery
		<< ProtcARFmt::AcstQueryCount
		<< ProtcARFmt::AcstQueryNames
		<< ProtcARFmt::AcstQueryCurr
		<< ProtcARFmt::AcstQueryState
		<< ProtcARFmt::RotQueryState
		<< ProtcARFmt::SynQueryState
		<< ProtcARFmt::SynQueryPrmAuto;

	mMapTypeToCodes.insert(ProtcARFmt::MsgNet, vcNet);
	mMapTypeToCodes.insert(ProtcARFmt::MsgSet, vcSet);
	mMapTypeToCodes.insert(ProtcARFmt::MsgGet, vcGet);
}

void AcstRotAnalyze::append(
    const QByteArray& data, const QString& ipSrc, const ushort& portSrc)
{
	mMutex->lock();

	QByteArray cache;
	QString addr = QString("%1:%2").arg(ipSrc).arg(portSrc);
	if (mCaches.contains(addr))
	{
		cache = mCaches.take(addr);
	}

	cache.append(data);
	int lenRem = 0;
	preAnalyze(cache, lenRem, ipSrc, portSrc);
	if (lenRem > 0)
	{
		cache = cache.right(lenRem);
		mCaches.insert(addr, cache);
	}
	
	mMutex->unlock();
}

void AcstRotAnalyze::preAnalyze(
	const QByteArray& data, int& lenRem, const QString& ipSrc, const ushort& portSrc)
{
	int lenMin  = ProtcARFmt::gLenPrepPost;
	int lenPrep = ProtcARFmt::gLenPrep;
	int lenPost = ProtcARFmt::gLenPost;

	int lenData = data.size();
	int index   = 0;
	lenRem      = lenData - index;

	const char* pData = data.constData();
	while (lenData - index >= lenMin)
	{
		//尝试取前缀
		ProtcARFmt::Prep stPrep;
		memcpy(&stPrep, &pData[index], lenPrep);

		//判帧头
		if ( stPrep.mHead != ProtcARFmt::Head )
		{
			index++;
			continue;
		}

		//判指令码
		if ( !mMap.contains(stPrep.mCmdCode) )
		{
			//qDebug() << "IP_Src = " << XKCommon::addrToIP(stPrep.mAddrSrc);
			//qDebug() << "IP_Dst = " << XKCommon::addrToIP(stPrep.mAddrDst);
			gLog->write(QString("Error_CmdCode: 0x%1.").arg(stPrep.mCmdCode, 4, 16, QChar('0')), true);
			gLog->write(QString("IP_Src: 0x%1.").arg(XKCommon::addrToIP(stPrep.mAddrSrc)), true);
			gLog->write(QString("IP_Dst: 0x%1.").arg(XKCommon::addrToIP(stPrep.mAddrDst)), true);
			index++;
			continue;
		}

		//判断指令类型
		if (!mMapTypeToCodes.contains(stPrep.mType))
		{
			//qDebug() << "Error_Type: " << stPrep.mType;
			gLog->write(QString("Error_Type: %1.").arg(stPrep.mType), true);
			index++;
			continue;
		}

		//校验指令类型与指令码是否匹配
		if (!mMapTypeToCodes[stPrep.mType].contains(stPrep.mCmdCode))
		{
			gLog->write(QString("Error_Type And Code Not Compare: "
                "cmdCode = 0x%1  type = %2.")
                .arg(stPrep.mCmdCode, 4, 16, QChar('0')).arg(stPrep.mType), true);
			index++;
			continue;
		}

		//判帧长是否与指令码匹配
		CoreInfo ci = mMap.value(stPrep.mCmdCode);
		if (!mVcLenUnknown.contains(stPrep.mCmdCode))
		{
			if (stPrep.mLen != ci.mLen)
			{
				//qDebug() << "Error_Len: " << stPrep.mLen;
				gLog->write(QString("Error_Len: %1.").arg(stPrep.mLen), true);
				index++;
				continue;
			}
		}
		

		//余量不足，等待拼包
		if ( (lenData - index) < stPrep.mLen )
		{
			break;
		}

		//判帧尾
		int indPostStart  = index + stPrep.mLen - lenPost;
		ProtcARFmt::Post stPost;
		memcpy(&stPost, &pData[indPostStart], lenPost);
		if ( stPost.mTail != (WORD)ProtcARFmt::Tail )
		{
			index++;
			continue;
		}

		//判校验和
		if ( stPost.mCheck != ProtcARFmt::checkSum(&pData[index], stPrep.mLen) )
		{
			index++;
			WORD sum = ProtcARFmt::checkSum(&pData[index], stPrep.mLen);
			gLog->write(QString("Error_CheckSum: sum_recv = %1  sum_calc = %2.").arg(stPost.mCheck).arg(sum), true);
			continue;
		}

		//取出一帧
		QByteArray frm(&pData[index], stPrep.mLen);
		index += stPrep.mLen;

		//权限判别及指令应答；若需要进一步处理，则扔到解析函数
		if (mCore->accessAndResp(frm, ipSrc, portSrc))
		{
			(mCore->*ci.mFun)(frm);

			/*if (stPrep.mType != ProtcARFmt::MsgNet)
			{
				QString stemp = QString("【Debug】Recv: AddrSrc = %1, AddrDst = %2"
				).arg(XKCommon::addrToIP(stPrep.mAddrSrc)).arg(XKCommon::addrToIP(stPrep.mAddrDst));
				gLog->write(stemp, true);
			}*/
		}
	}

	lenRem = lenData - index;
}

