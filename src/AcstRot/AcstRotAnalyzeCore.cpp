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
		writeLog("�������Ȩָ�������");
		return;
	}

	QStringList lst = QStringList()
		<< "�������Ȩ" << "ȡ���������Ȩ"
		<< "ͬ���ͷſ���Ȩ" << "�ܾ��ͷſ���Ȩ";
	writeLog(QString("%1��").arg(lst.at(ob.mData.mCmd)));

	//��ȡ�û���Ϣ
	ParamAccess::UserInfo user(mIpSrc, mPortSrc, ob.mPrep.mAddrSrc);

	//Ȩ�޹�����������
	gAccess->apply(user, ob.mData.mCmd);
}

void AcstRotAnalyzeCore::accQuery(const QByteArray& data)
{
	PrtAccQuery ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	//��ȡ�û���Ϣ
	ParamAccess::UserInfo user(mIpSrc, mPortSrc, ob.mPrep.mAddrSrc);

	//Ȩ�޲�ѯ��Ӧ
	gAccess->query(user);
}

//Acoustic
void AcstRotAnalyzeCore::acstSetFlName(const QByteArray& data)
{
	PrtAcstSetFlName ob;
	ob.fromBytes(data.constData(), data.size());

	if (!gParam->isMscExisted(ob.mData.mName))
	{
		writeLog(QString("���õ�ǰ��Ŀ�����ļ���%1�������ڣ�").arg(ob.mData.mName));
		return;
	}

	//������Ŀ����
	gParam->setMscCurr(ob.mData.mName);
	writeLog(QString("���õ�ǰ��ĿΪ��%1����").arg(ob.mData.mName));

	//���²�����������Ŀ
	gMscPlayerManag->setFile();
}

void AcstRotAnalyzeCore::acstPlayCtrl(const QByteArray& data)
{
	PrtAcstPlayCtrl ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mCmd > ob.PlayStop)
	{
		writeLog(QString("���Ʋ�����ָ���루%1������").arg(ob.mData.mCmd));
		return;
	}

	ParamDevAcst acst = gParam->getDevAcst();
	acst.mPlay = ob.mData.mCmd;
	gParam->setDevAcst(acst);

	QStringList lst = QStringList() << "����" << "��ͣ" << "����";
	writeLog(QString("������%1��").arg(lst.at(acst.mPlay)));

	//����������
	gMscPlayerManag->setPlay();
}

void AcstRotAnalyzeCore::acstMuteCtrl(const QByteArray& data)
{
	PrtAcstMuteCtrl ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mMute > ob.MuteOff)
	{
		writeLog(QString("��������ָ���루%1������").arg(ob.mData.mMute));
		return;
	}

	ParamDevAcst acst = gParam->getDevAcst();
	acst.mMute = ob.mData.mMute;
	gParam->setDevAcst(acst);

	QStringList lst = QStringList() << "��������" << "�رվ���" ;
	writeLog(QString("%1��").arg(lst.at(acst.mMute)));

	//��������������
	gMscPlayerManag->setMute();
}

void AcstRotAnalyzeCore::acstVolumeCtrl(const QByteArray& data)
{
	PrtAcstVolumeCtrl ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.VolBoth)
	{
		writeLog(QString("����������Ч���λ��%1������").arg(ob.mData.mIdt));
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
		slog.append(QString("���ò���������Ϊ%1��").arg(acst.mVolume));
		isSetPlayer = true;
		break;
	case ob.VolDevice:
		acst.mVolumeSys = qBound(0, (int)ob.mData.mVolDevice, 100);
		slog.append(QString("���ù��ػ�����Ϊ%1��").arg(acst.mVolumeSys));
		isSetDev = true;
		break;
	case ob.VolBoth:
		acst.mVolume = qBound(0, (int)ob.mData.mVolPlayer, 100);
		acst.mVolumeSys = qBound(0, (int)ob.mData.mVolDevice, 100);
		slog.append(QString("���ò���������Ϊ%1�����ػ�����Ϊ%2��")
			.arg(acst.mVolume).arg(acst.mVolumeSys));
		isSetPlayer = true;
		isSetDev = true;
		break;
	default:
		break;
	}
	gParam->setDevAcst(acst);

	writeLog(slog);

	//��������������
	if (isSetPlayer)
	{
		gMscPlayerManag->setVol();
	}
	
	//ϵͳ��������
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
		writeLog(QString("����ģʽָ���루%1������").arg(ob.mData.mLoop));
		return;
	}

	ParamDevAcst acst = gParam->getDevAcst();
	acst.mLoop = ob.mData.mLoop;
	gParam->setDevAcst(acst);

	QStringList lst = QStringList() << "�б�ѭ��" << "����ѭ��";
	writeLog(QString("���ò���ģʽΪ%1��").arg(lst.at(acst.mLoop)));
}

void AcstRotAnalyzeCore::acstDeleteFile(const QByteArray& data)
{
	PrtAcstDeleteFile ob;
	ob.fromBytes(data.constData(), data.size());

	QString name = ob.mData.mName;
	if (!gParam->isMscExisted(name))
	{
		writeLog(QString("ɾ����Ŀ�����ļ���%1�������ڣ�").arg(name));
		send(gPackAR->acstRepDelete(PrtAcstRepDelete::ErrorName, ob.mPrep.mAddrSrc));
		return;
	}

	writeLog(QString("ɾ����Ŀ��%1����").arg(name));

	//ɾ����Ŀ
	QString path = gParam->mscPath(name);
	bool succ = XKCommon::removeFile(path);

	QString slog = QString("ɾ����Ŀ��%1��%2��")
		.arg(name).arg(succ ? "�ɹ�" : "ʧ�ܣ��ļ���ռ�á�");
	int idt = succ ? PrtAcstRepDelete::Succeed : PrtAcstRepDelete::ErrorUsing;
	
	send(gPackAR->acstRepDelete(idt, ob.mPrep.mAddrSrc));
	writeLog(slog);

	//���±�����Ŀ�б�
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
	writeLog("תֹ̨ͣ��");

	//�·�תָֹ̨ͣ��
	gRot->stop();
}

void AcstRotAnalyzeCore::rotTurnDirec(const QByteArray& data)
{
	PrtRotTurnDirec ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.TurnRight)
	{
		writeLog(QString("ת̨��̬����ָ���루%1������").arg(ob.mData.mIdt));
		return;
	}

	QStringList lst = QStringList() << "��ת" << "��ת" << "��ת" << "��ת";
	writeLog(QString("����ת̨��ʼ%1��").arg(lst.at(ob.mData.mIdt)));

	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtDirec = ob.mData.mIdt;
	gParam->setDevRot(rot);

	//ת̨��̬����
	gRot->turnDirec();
}

void AcstRotAnalyzeCore::rotSetVel(const QByteArray& data)
{
	PrtRotSetVel ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.AziAndPit)
	{
		writeLog(QString("ת̨ת�����ñ��λ��%1������").arg(ob.mData.mIdt));
		return;
	}

	QString slog = QString("����ת̨");
	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtVel = ob.mData.mIdt;
	switch (ob.mData.mIdt)
	{
	case ob.AziOnly:
		rot.mVelAzi = ob.mData.mVelAzi * 0.01;
		slog.append(QString("��λת��%1��/s��").arg(rot.mVelAzi, 0, 'f', 2));
		break;
	case ob.PitOnly:
		rot.mVelPit = ob.mData.mVelPit * 0.01;
		slog.append(QString("����ת��%1��/s��").arg(rot.mVelPit, 0, 'f', 2));
		break;
	case ob.AziAndPit:
		rot.mVelAzi = ob.mData.mVelAzi * 0.01;
		rot.mVelPit = ob.mData.mVelPit * 0.01;
		slog.append(QString("��λת��%1��/s������ת��%2��/s��")
			.arg(rot.mVelAzi, 0, 'f', 2).arg(rot.mVelPit, 0, 'f', 2));
		break;
	default:
		break;
	}
	gParam->setDevRot(rot);

	writeLog(slog);

	//����ת̨ת��
	//��ǰ��̨��ת������ָ������������ģ���̬����ʱ���ø�ת��
}

void AcstRotAnalyzeCore::rotTurnTo(const QByteArray& data)
{
	PrtRotTurnTo ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.AziAndPit)
	{
		writeLog(QString("ת̨פ�������ñ��λ��%1������").arg(ob.mData.mIdt));
		return;
	}

	QString slog = QString("����ת̨ת��");
	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtDeg = ob.mData.mIdt;
	switch (ob.mData.mIdt)
	{
	case ob.AziOnly:
		rot.mAzi = XKCommon::angleToPos(ob.mData.mAzi * 0.01);
		slog.append(QString("��λ%1�㡣").arg(rot.mAzi, 0, 'f', 2));
		break;
	case ob.PitOnly:
		rot.mPit = XKCommon::angleToNeg(ob.mData.mPit * 0.01);
		slog.append(QString("����%1�㡣").arg(rot.mPit, 0, 'f', 2));
		break;
	case ob.AziAndPit:
		rot.mAzi = XKCommon::angleToPos(ob.mData.mAzi * 0.01);
		rot.mPit = XKCommon::angleToNeg(ob.mData.mPit * 0.01);
		slog.append(QString("��λ%1�㣬����%2�㡣")
			.arg(rot.mAzi, 0, 'f', 2).arg(rot.mPit, 0, 'f', 2));
		break;
	default:
		break;
	}
	gParam->setDevRot(rot);

	writeLog(slog);

	//����ת̨פ����
	gRot->resident();
}

void AcstRotAnalyzeCore::rotZero(const QByteArray& data)
{
	PrtRotZero ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	if (ob.mData.mIdt > ob.AziAndPit)
	{
		writeLog(QString("ת̨������λ��%1������").arg(ob.mData.mIdt));
		return;
	}

	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtZero = ob.mData.mIdt;
	gParam->setDevRot(rot);

	QStringList list = QStringList() << "��λ" << "����" << "��λ������";
	QString slog = QString("����ת̨%1���㡣").arg(list[ob.mData.mIdt]);
	
	writeLog(slog);

	//���ƹ���
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
		writeLog(QString("�ۺϹ���-����ģʽ��%1������").arg(ob.mData.mIdt));
		return;
	}

	ParamIntegrated itg = gParam->getInteg();
	itg.mWorkModel = ob.mData.mIdt;
	gParam->setInteg(itg);
	gConfig->saveToRestore();

	QStringList list = QStringList() << "�Զ�ģʽ" << "�ֶ�ģʽ";
	writeLog(QString("�����ۺϹ���-����ģʽΪ%1��").arg(list[itg.mWorkModel]));

	//ģʽ�л�
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

	QString slog = QString("�����ۺϹ���-�Զ�ģʽ��������ʼ����ʱ��%1����������ʱ��%2��")
		.arg(itg.mPrmAuto.minuteToTime(itg.mPrmAuto.mTimeStart).toString("hh:mm:ss"))
		.arg(itg.mPrmAuto.minuteToTime(itg.mPrmAuto.mTimeEnd).toString("hh:mm:ss"));

	slog.append(QString("��λɨ��%1����%2�㣬����%3�㣻")
		.arg(itg.mPrmAuto.mAziStart, 0, 'f', 2).arg(itg.mPrmAuto.mAziEnd, 0, 'f', 2)
		.arg(itg.mPrmAuto.mAziStep, 0, 'f', 2));

	slog.append(QString("����ɨ��%1����%2�㣬����%3�㣨��ǰ������ɨ�裬��פ������ʼ�ǣ���")
		.arg(itg.mPrmAuto.mPitStart, 0, 'f', 2).arg(itg.mPrmAuto.mPitEnd, 0, 'f', 2)
		.arg(itg.mPrmAuto.mPitStep, 0, 'f', 2));

	slog.append(QString("��λת��%1��/s������ת��%2��/s��")
		.arg(itg.mPrmAuto.mVelAzi, 0, 'f', 2).arg(itg.mPrmAuto.mVelPit, 0, 'f', 2));

	slog.append(QString("פ��ʱ�䣺%1s��").arg(itg.mPrmAuto.mSecStay));

	writeLog(slog);

	//���´���
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

	QString slog = QString("����״̬�Զ��ϱ��������豸%1��ת̨%2���ۺ�״̬%3��")
		.arg(itg.mIsRepAuto.mAcst ? "��" : "��")
		.arg(itg.mIsRepAuto.mRot ? "��" : "��")
		.arg(itg.mIsRepAuto.mInteg ? "��" : "��");
	writeLog(slog);
}

void AcstRotAnalyzeCore::synPowerOffPC(const QByteArray& data)
{
	writeLog("��ָ��رչ��ػ���");

	//app������ʱ�ػ�
	gLog->write("ִ�У��������������5���ػ���");
	
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

	QString slog = QString("Уʱ����׼ʱ��%1����ʱ����ʱ��%2��")
		.arg(ps.mDtTimingRemote.toString("yyyy-MM-dd hh:mm:ss"))
		.arg(ps.mDtTimingLocal.toString("yyyy-MM-dd hh:mm:ss"));
	gLog->write(slog, true);
}



//ͳһ����Ȩ�޴���ָ��Ӧ��IP��port����
bool AcstRotAnalyzeCore::accessAndResp(const QByteArray& data, const QString& ipSrc, const ushort& portSrc)
{
	//����ip��port
	mIpSrc = ipSrc;
	mPortSrc = portSrc;

	ProtcARFmt::Prep prep;
	memcpy(&prep, &data.constData()[0], sizeof(prep));

	//��������������Ȩ�ޣ�����Ӧ��
	if (prep.mType == ProtcARFmt::MsgNet)
	{
		return true;
	}

	//��ȡ�û���Ϣ
	ParamAccess::UserInfo user(mIpSrc, mPortSrc, prep.mAddrSrc);

	QString slog = QString("�յ�%1��%2������ָ�%3��")
		.arg(mIpSrc).arg(mPortSrc).arg(gARCodeDescr->codeToStr(prep.mCmdCode));


	//��ҪȨ�޵�ָ�����ǰ�û���Ȩ�ޣ�����Ӧ
	if (false/*gARCodeDescr->needAccess(prep.mCmdCode)*/)
	{
		if (!gParam->isAccessed(user))
		{
			slog.append("����ǰ�û��޿���Ȩ�ޣ�ָ����ԡ�");
			gLog->write(slog, true);
			return false;
		}

		//���¿���Ȩ�û�������ָ�����ʱ��
		bool levelChanged = gParam->updateDtAccCommLast( QDateTime::currentDateTime() );
		if (levelChanged)
		{
			gLog->write(QString("Ȩ���û�%1����Ȩ�ޱ��Ϊ����Ȩ�ޡ�").arg(user.mIp), true);
		}
	}

	//��ǰ�����Զ�ģʽ�£������ת̨����ָ��
	if (gParam->isWorkAuto() &&  gARCodeDescr->ignoreWhenAuto(prep.mCmdCode))
	{
		slog.append("����ǰ�����Զ�����ģʽ��ת̨����ָ����ԡ�");
		gLog->write(slog, true);
		return false;
	}

	slog.append("��");
	gLog->write(slog, true);

	//ָ��Ӧ��
	PrtARCmdReponse cmdResp(prep);
	send(cmdResp.pack());

	//��֪���÷�����ָ����Ҫ��������
	return true;
}


//��accessAndResp���µ�IP��port��������
bool AcstRotAnalyzeCore::send(const QByteArray& cmd) const
{
	return gSrvCtrl->send(cmd, mIpSrc, mPortSrc);
}

void AcstRotAnalyzeCore::writeLog(const QString & slog) const
{
	QString str = QString("�û���%1��%2��ָ�").arg(mIpSrc).arg(mPortSrc);
	str.append(slog);
	gLog->write(str, true);
}

