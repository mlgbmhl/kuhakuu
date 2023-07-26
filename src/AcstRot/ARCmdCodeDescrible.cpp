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
	mMap.insert( (ushort)ProtcARFmt::NetHand, QString("��������") );

	
	mMap.insert( (ushort)ProtcARFmt::AccApply, QString("�������Ȩ") );
	mMap.insert( (ushort)ProtcARFmt::AccQuery, QString("��ѯ����Ȩ") );

	mMap.insert( (ushort)ProtcARFmt::AcstSetFlName, QString("���ò�����Ŀ") );
	mMap.insert( (ushort)ProtcARFmt::AcstPlayCtrl, QString("���ſ���") );
	mMap.insert( (ushort)ProtcARFmt::AcstMuteCtrl, QString("��������") );
	mMap.insert( (ushort)ProtcARFmt::AcstVolumeCtrl, QString("��������") );
	mMap.insert( (ushort)ProtcARFmt::AcstLoopModel, QString("���ò���ģʽ") );
	mMap.insert( (ushort)ProtcARFmt::AcstDeleteFile, QString("ɾ��ָ���ļ�������Ŀ") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryCount, QString("��ѯ��Ŀ����") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryNames, QString("��ѯ��Ŀ�б�") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryCurr, QString("��ѯ��ǰ���ŵ���Ŀ��") );
	mMap.insert( (ushort)ProtcARFmt::AcstQueryState, QString("��ѯ�����豸״̬") );

	mMap.insert( (ushort)ProtcARFmt::RotStop, QString("תֹ̨ͣ") );
	mMap.insert( (ushort)ProtcARFmt::RotTurnDirec, QString("ת̨��̬����") );
	mMap.insert( (ushort)ProtcARFmt::RotSetVel, QString("ת������") );
	mMap.insert( (ushort)ProtcARFmt::RotTurnTo, QString("ת��פ����") );
	mMap.insert( (ushort)ProtcARFmt::RotZero, QString("ת̨����") );
	mMap.insert( (ushort)ProtcARFmt::RotQueryState, QString("��ѯת̨״̬") );

	mMap.insert( (ushort)ProtcARFmt::SynWorkModel, QString("����ģʽ����") );
	mMap.insert( (ushort)ProtcARFmt::SynPrmAuto, QString("�Զ�ģʽ��������") );
	mMap.insert( (ushort)ProtcARFmt::SynSetAutoRep, QString("�Զ��ϱ�ʹ�ܿ���") );
	mMap.insert( (ushort)ProtcARFmt::SynPowerOffPC, QString("���ػ��ػ�") );
	mMap.insert((ushort)ProtcARFmt::SynQueryState, QString("��ѯ�ۺ�״̬"));
	mMap.insert((ushort)ProtcARFmt::SynQueryPrmAuto, QString("��ѯ�Զ�����"));
	mMap.insert((ushort)ProtcARFmt::SynTiming, QString("Уʱ"));
}

void ARCmdCodeDescrible::initVcAcc()
{
	//����Ҫ����Ȩ���������֣�Ȩ�޹������в�ѯ
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
	//�Զ�ģʽ�£�������λ��ֱ���·���ת̨����ָ�����Ч
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
