#include "SimpleRLAnalyzeCore.h"
#include "XKDirecAcousticProtocol.h"
#include "PD/DevManag/LaserManager.h"
#include "PD/DevManag/MusicPlayerManager.h"
#include "PD/DevManag/RotManager.h"
#include "PD/DevManag/LaserManager.h"
#include "PD/LogManag/Log.h"
#include "DM/Manag/ParameterManager.h"
#include "DM/File/ConfigManager.h"
#include "SI/Common/CoordConvert.h"
#include "SI/Common/XKCommon.h"
#include <QObject>

SimpleRLAnalyzeCore::SimpleRLAnalyzeCore()
{

}

SimpleRLAnalyzeCore::~SimpleRLAnalyzeCore()
{

}

void SimpleRLAnalyzeCore::acousticStrong(const QByteArray & data) const
{
	writeLog(QObject::tr("ǿ������������豸������Ϊǿ������"));

	ParamSimplePrtc smp = gParam->getSimple();
	ParamDevAcst acst = gParam->getDevAcst();
	acst.mVolume = smp.mPrmAcst.mVolumeStrong;
	gParam->setDevAcst(acst);

	gMscPlayerManag->setVol();
}

void SimpleRLAnalyzeCore::acousticWeak(const QByteArray & data) const
{
	writeLog(QObject::tr("��������������豸������Ϊ��������"));

	ParamSimplePrtc smp = gParam->getSimple();
	ParamDevAcst acst = gParam->getDevAcst();
	acst.mVolume = smp.mPrmAcst.mVolumeWeak;
	gParam->setDevAcst(acst);

	gMscPlayerManag->setVol();
}

void SimpleRLAnalyzeCore::acousticPlay(const QByteArray & data) const
{
	if (gParam->mscCount() < 1)
	{
		writeLog(QObject::tr("������Ƶ�ļ�ʧ�ܣ���Ч��Ƶ�ļ���Ϊ0��"));
		return;
	}

	XKCmdAcousticPlay ob;
	memcpy(&ob, data.constData(), sizeof(ob));

	int index = ob.mFileOrderNo;
	QString fileName = gParam->mscName(index);

	if (fileName.isEmpty())
	{
		writeLog(QObject::tr("���Բ������Ϊ%1����Ƶ�ļ�ʧ�ܣ�"
			"����Ŷ�Ӧ���ļ������ڣ�Ĭ���л�����һ����Ƶ�ļ���").arg(index));
		index = 0;
		fileName = gParam->mscName(index);
	}
	
	gLog->write(QObject::tr("�������%1���ļ���Ϊ��%2������Ƶ��")
		.arg(index).arg(fileName), true);

	gParam->setMscCurr(fileName);

	ParamDevAcst acst = gParam->getDevAcst();
	acst.mPlay = acst.Playing;
	gParam->setDevAcst(acst);

	gMscPlayerManag->setFile();
	gMscPlayerManag->setPlay();
}

void SimpleRLAnalyzeCore::acousticStop(const QByteArray & data) const
{
	writeLog(QObject::tr("ֹͣ������Ƶ��"));
	
	ParamDevAcst acst = gParam->getDevAcst();
	acst.mPlay = acst.PlayStop;
	gParam->setDevAcst(acst);

	gMscPlayerManag->setPlay();
}

void SimpleRLAnalyzeCore::lampWarning(const QByteArray & data) const
{
	writeLog(QObject::tr("ǿ�⾯ʾ��"));
	gLaser->warning(gParam->getSimple().mPrmLaser.mColorWarning);
}

void SimpleRLAnalyzeCore::lampBeat(const QByteArray & data) const
{
	writeLog(QObject::tr("ǿ��ֱ�䡣"));
	gLaser->beat(gParam->getSimple().mPrmLaser.mColorBeat);
}

void SimpleRLAnalyzeCore::lampBurstFlash(const QByteArray & data) const
{
	writeLog(QObject::tr("ǿ�ⱬ����"));
	gLaser->burstFlash(gParam->getSimple().mPrmLaser.mColorBurstFlash);
}

void SimpleRLAnalyzeCore::lampStop(const QByteArray & data) const
{
	writeLog(QObject::tr("ǿ��رա�"));
	gLaser->turnOffBeingOn();
}

void SimpleRLAnalyzeCore::rotaryUp(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnUp);
}

void SimpleRLAnalyzeCore::rotaryDown(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnDown);
}

void SimpleRLAnalyzeCore::rotaryLeft(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnLeft);
}

void SimpleRLAnalyzeCore::rotaryRight(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnRight);
}

void SimpleRLAnalyzeCore::rotaryLeftUp(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnLeftUp);
}

void SimpleRLAnalyzeCore::rotaryLeftDown(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnLeftDown);
}

void SimpleRLAnalyzeCore::rotaryRightUp(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnRightUp);
}

void SimpleRLAnalyzeCore::rotaryRightDown(const QByteArray & data) const
{
	rotaryTurnDirc(ParamDevRot::TurnRightDown);
}

void SimpleRLAnalyzeCore::rotaryLocation(const QByteArray & data) const
{
	XKCmdRotaryLocation ob(0, 0);
	memcpy(&ob, data.constData(), sizeof(ob));

	//���ݽ���
	double lon = XKDAPFrame::ToLon(ob.mLon);
	double lat = XKDAPFrame::ToLat(ob.mLat);
	double alt = XKDAPFrame::ToAlt(ob.mAlt);

    //λ������
    gRot->locationGuide(lon, lat, alt);
}

void SimpleRLAnalyzeCore::rotaryStop(const QByteArray & data) const
{
	writeLog("תֹ̨ͣ��");
	gRot->stop();
}

void SimpleRLAnalyzeCore::devWgs(const QByteArray & data) const
{
	XKCmdDevWGS ob(0.0, 0.0, 0.0);
	memcpy(&ob, data.constData(), sizeof(ob));

	//���ݽ���
	double lon = XKDAPFrame::ToLon(ob.mLon);
	double lat = XKDAPFrame::ToLat(ob.mLat);
	double alt = XKDAPFrame::ToAlt(ob.mAlt);

	ParamDevRot rot = gParam->getDevRot();
	rot.mLon = qBound(-180.0, lon, 180.0);
	rot.mLat = qBound(-90.0, lat, 90.0);
	rot.mAlt = alt;
	gParam->setDevRot(rot);

	gConfig->saveToRestoreSimple();

	QString slog = QString("�����豸λ����Ϣ������%1�ȣ�γ��%2�ȣ�����%3�ס�")
		.arg(lon, 0, 'f', 6).arg(lat, 0, 'f', 6).arg(alt, 0, 'f', 2);
	writeLog(slog);
}

void SimpleRLAnalyzeCore::devAzi(const QByteArray & data) const
{
	XKCmdDevDriftAngle ob(0.0);
	memcpy(&ob, data.constData(), sizeof(ob));

	double azi = XKDAPFrame::ToAngle(ob.mAzi);
	ParamDevRot rot = gParam->getDevRot();
	rot.mAziDelt = qBound(0.0, azi, 359.99);
	gParam->setDevRot(rot);

	gConfig->saveToRestoreSimple();

	QString slog = QString("�����豸����ƫ�ǣ�ƫ��%1�ȡ�")
		.arg(azi, 0, 'f', 2);
	writeLog(slog);
}

void SimpleRLAnalyzeCore::writeLog(const QString& slog) const
{
	QString str = QObject::tr("ƽָ̨�HZ52����");
	str.append(slog);
	gLog->write(str, true);
}

void SimpleRLAnalyzeCore::rotaryTurnDirc(int direc) const
{
	if (direc < ParamDevRot::TurnUp || direc > ParamDevRot::TurnRightDown)
	{
		return;
	}

	static const QString Direcs[8] = {
		"��", "��", "��", "��", "����", "����", "����", "����" };

	writeLog(QObject::tr("ת̨%1ת��").arg(Direcs[direc]));
	ParamDevRot rot = gParam->getDevRot();
	rot.mIdtDirec = direc;
	gParam->setDevRot(rot);
	gRot->turnDirec();
}
