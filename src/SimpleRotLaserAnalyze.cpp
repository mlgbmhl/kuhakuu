#include "SimpleRotLaserAnalyze.h"
#include "XKDirecAcousticProtocol.h"


SimpleRotLaserAnalyze::SimpleRotLaserAnalyze()
{
	mCore = new SimpleRLAnalyzeCore();
	init();
}

SimpleRotLaserAnalyze::~SimpleRotLaserAnalyze()
{
	delete mCore;
}

SimpleRotLaserAnalyze* SimpleRotLaserAnalyze::getInstance()
{
	static SimpleRotLaserAnalyze ins;
	return &ins;
}

void SimpleRotLaserAnalyze::append(
	const QByteArray & data, const QString & ipSrc, const ushort & portSrc)
{
	mCache.append(data);
	int lenRem = 0;
	preAnalyze(mCache, lenRem, ipSrc, portSrc);
	mCache = mCache.right(lenRem);
}

void SimpleRotLaserAnalyze::preAnalyze(const QByteArray & data,
	int & lenRem, const QString & ipSrc, const ushort & portSrc)
{
	//Э�鶨��֡����֡��18�ֽ�
	int lenFrm = 18;
	int index = 0;
	int temp = 0;
	int lenData = data.size();
	const char* pData = data.constData();

	unsigned short cmdCode = 0;
	int lenCmdCode = sizeof(cmdCode);

	while (lenData - index >= lenFrm)
	{
		//��֡ͷ
		if (pData[index] != (char)0x7e || pData[index + 1] != (char)0x7e)
		{
			index++;
			continue;
		}

		//��֡β
		temp = index + lenFrm;
		if (pData[temp - 2] != (char)0xed || pData[temp - 1] != (char)0xed)
		{
			index++;
			continue;
		}

		//��ָ����
		memcpy(&cmdCode, &pData[index + 2], lenCmdCode);
		if (!mMap.contains(cmdCode))
		{
			index++;
			continue;
		}

		//ȡһ֡
		QByteArray baFrm = data.mid(index, lenFrm);
		index += lenFrm;

		//�������
		(mCore->*mMap.value(cmdCode))(baFrm);
		
	}

	lenRem = data.size() - index;
}


void SimpleRotLaserAnalyze::init()
{
	using Frm = XKDAPFrame;
	using Core = SimpleRLAnalyzeCore;

	mMap.insert(Frm::CodeAcousticStrong, &Core::acousticStrong);
	mMap.insert(Frm::CodeAcousticWeak, &Core::acousticWeak);
	mMap.insert(Frm::CodeAcousticPlay, &Core::acousticPlay);
	mMap.insert(Frm::CodeAcousticStop, &Core::acousticStop);


	mMap.insert(Frm::CodeLampWarning, &Core::lampWarning);
	mMap.insert(Frm::CodeLampBeat, &Core::lampBeat);
	mMap.insert(Frm::CodeLampBurstFlash, &Core::lampBurstFlash);
	mMap.insert(Frm::CodeLampStop, &Core::lampStop);


	mMap.insert(Frm::CodeRotaryUp, &Core::rotaryUp);
	mMap.insert(Frm::CodeRotaryDown, &Core::rotaryDown);
	mMap.insert(Frm::CodeRotaryLeft, &Core::rotaryLeft);
	mMap.insert(Frm::CodeRotaryRight, &Core::rotaryRight);

	mMap.insert(Frm::CodeRotaryLeftUp, &Core::rotaryLeftUp);
	mMap.insert(Frm::CodeRotaryLeftDown, &Core::rotaryLeftDown);
	mMap.insert(Frm::CodeRotaryRightUp, &Core::rotaryRightUp);
	mMap.insert(Frm::CodeRotaryRightDown, &Core::rotaryRightDown);

	mMap.insert(Frm::CodeRotaryLocation, &Core::rotaryLocation);
	mMap.insert(Frm::CodeRotaryStop, &Core::rotaryStop);


	mMap.insert(Frm::CodeDevWGS, &Core::devWgs);
	mMap.insert(Frm::CodeDevDriftAngle, &Core::devAzi);
}

