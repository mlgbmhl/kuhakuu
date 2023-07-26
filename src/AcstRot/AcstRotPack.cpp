#include "AcstRotPack.h"
#include "ProtocolAcstRot.h"
#include "DM/Manag/ParameterManager.h"
#include "DM/Manag/StatusManager.h"
#include "SI/Common/XKCommon.h"

AcstRotPack::AcstRotPack()
{
	mAddrSrc = 0;
}

AcstRotPack * AcstRotPack::getInstance()
{
	static AcstRotPack ins;
	return &ins;
}

AcstRotPack::~AcstRotPack()
{

}

void AcstRotPack::setAddrSrc(uint addr)
{
	mAddrSrc = addr;
}

QByteArray AcstRotPack::accRepApply(const int& idt, int addrDst) const
{
	if (idt < 0 || idt > PrtAccRepApply::NotifLost)
	{
		return QByteArray();
	}

	PrtAccRepApply ob;
	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;
	ob.mData.mCmd = idt;
	return PrtARPack::pack(&ob, sizeof(ob));
}

QByteArray AcstRotPack::accRepState(const int & idt, int addrDst) const
{
	if (idt < 0 || idt > PrtAccRepState::ApplyWait)
	{
		return QByteArray();
	}

	PrtAccRepState ob;
	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;
	ob.mData.mCmd = idt;
	return PrtARPack::pack(&ob, sizeof(ob));
}

QByteArray AcstRotPack::acstRepDelete(const int & idt, int addrDst) const
{
	if (idt < 0 || idt > PrtAcstRepDelete::ErrorUndef)
	{
		return QByteArray();
	}

	PrtAcstRepDelete ob;
	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;
	ob.mData.mResult = idt;
	return PrtARPack::pack(&ob, sizeof(ob));
}

QByteArray AcstRotPack::acstRepCount(int addrDst) const
{
	PrtAcstRepCount ob;
	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;
	ob.mData.mCount = gParam->mscCount();
	return PrtARPack::pack(&ob, sizeof(ob));
}

QByteArray AcstRotPack::acstRepNames(int addrDst) const
{
	ParamMusicList mscs = gParam->getMscList();
	PrtAcstRepNames ob;

	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;

	ob.mData.mCount = mscs.mNames.size();
	for (int i = 0; i < mscs.mNames.size(); i++)
	{
		ob.mData.mNames.append(mscs.mNames.at(i));
	}

	return PrtARPack::pack(ob.toByteArray());
}

QByteArray AcstRotPack::acstRepCurr(int addrDst) const
{
	PrtAcstRepCurr ob;
	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;

	ob.mData.mName = gParam->mscCurr();

	return PrtARPack::pack(ob.toByteArray());
}

QByteArray AcstRotPack::acstRepState(int addrDst) const
{
	StatDevAcst acst = gStatus->getAcst();
	PrtAcstRepState ob;

	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;

	ob.mData.mPlay = acst.mPlay;
	ob.mData.mMute = acst.mMute;
	ob.mData.mVolPlayer = acst.mVolume;
	ob.mData.mVolDevice = acst.mVolumeSys;
	ob.mData.mLoop = acst.mLoop;

	return PrtARPack::pack(&ob, sizeof(ob));
}

QByteArray AcstRotPack::rotRepState(int addrDst) const
{
	StatDevRot rot = gStatus->getRot();
	PrtRotRepState ob;

	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;

	/*rot.mAzi = 330.12;
	rot.mPit = -5.67;
	rot.mAziDelt = 0.54;
	rot.mPitDelt = -0.45;*/

	ob.mData.mAzi = XKCommon::angleToPos(rot.mAzi) * 100;
	ob.mData.mPit = XKCommon::angleToNeg(rot.mPit) * 100;
	ob.mData.mAziDelt = XKCommon::angleToPos(rot.mAziDelt) * 100;
	ob.mData.mPitDelt = XKCommon::angleToNeg(rot.mPitDelt) * 100;
	ob.mData.mVelAzi = rot.mVelAzi * 100;
	ob.mData.mVelPit = rot.mVelPit * 100;

	return PrtARPack::pack(&ob, sizeof(ob));
}

QByteArray AcstRotPack::synRepState(int addrDst) const
{
	StatIntegrated syn = gStatus->getInteg();
	PrtSynRepState ob;

	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;

	ob.mData.mWorkModel.mIdt = syn.mWorkModel;
	ob.mData.mRepAuto.mAcst = syn.mIsRepAuto.mAcst;
	ob.mData.mRepAuto.mRot = syn.mIsRepAuto.mRot;
	ob.mData.mRepAuto.mSyn = syn.mIsRepAuto.mInteg;

	return PrtARPack::pack(&ob, sizeof(ob));
}

QByteArray AcstRotPack::synRepPrmAuto(int addrDst) const
{
	StatIntegrated itg = gStatus->getInteg();
	PrtSynRepPrmAuto ob;

	ob.mPrep.mAddrSrc = mAddrSrc;
	ob.mPrep.mAddrDst = addrDst;

	ob.mData.mSecStart = itg.mPrmAuto.mTimeStart;
	ob.mData.mSecEnd = itg.mPrmAuto.mTimeEnd;

	ob.mData.mAziStart = XKCommon::angleToPos(itg.mPrmAuto.mAziStart) * 100;
	ob.mData.mAziEnd = XKCommon::angleToPos(itg.mPrmAuto.mAziEnd) * 100;
	ob.mData.mAziStep = XKCommon::angleToPos(itg.mPrmAuto.mAziStep) * 100;

	ob.mData.mPitStart = XKCommon::angleToNeg(itg.mPrmAuto.mPitStart) * 100;
	ob.mData.mPitEnd = XKCommon::angleToNeg(itg.mPrmAuto.mPitEnd) * 100;
	ob.mData.mPitStep = XKCommon::angleToPos(itg.mPrmAuto.mPitStep) * 100;

	ob.mData.mVelAzi = itg.mPrmAuto.mVelAzi * 100;
	ob.mData.mVelPit = itg.mPrmAuto.mVelPit * 100;

	ob.mData.mSecResident = itg.mPrmAuto.mSecStay;

	return PrtARPack::pack(&ob, sizeof(ob));
}





