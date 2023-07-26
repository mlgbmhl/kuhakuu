#ifndef ACSTROTANALYZECORE_H_20210318_1350
#define ACSTROTANALYZECORE_H_20210318_1350

#include <QByteArray>
#include <QString>

class AcstRotAnalyzeCore
{
public:
	AcstRotAnalyzeCore();
	~AcstRotAnalyzeCore();

	//netHand
	void netHand(const QByteArray& data);

	//Access
	void accApply(const QByteArray& data);
	void accQuery(const QByteArray& data);

	//Acoustic
	void acstSetFlName(const QByteArray& data);  //�䳤֡���ṹ�峤�Ȳ���
	void acstPlayCtrl(const QByteArray& data);
	void acstMuteCtrl(const QByteArray& data);
	void acstVolumeCtrl(const QByteArray& data);
	void acstLoopModel(const QByteArray& data);
	void acstDeleteFile(const QByteArray& data);  //�䳤֡���ṹ�峤�Ȳ���
	void acstQueryCount(const QByteArray& data);
	void acstQueryNames(const QByteArray& data);
	void acstQueryCurr(const QByteArray& data);
	void acstQueryState(const QByteArray& data);

	//Rotary
	void rotStop(const QByteArray& data);
	void rotTurnDirec(const QByteArray& data);
	void rotSetVel(const QByteArray& data);
	void rotTurnTo(const QByteArray& data);
	void rotZero(const QByteArray& data);
	void rotQueryState(const QByteArray& data);

	//Synthetical-Management
	void synWorkModel(const QByteArray& data);
	void synPrmAuto(const QByteArray& data);
	void synSetAutoRep(const QByteArray& data);
	void synPowerOffPC(const QByteArray& data);
	void synQueryState(const QByteArray& data);
	void synQueryPrmAuto(const QByteArray& data);
	void synTiming(const QByteArray& data);


	//Ȩ�޴�����Ȩ����true����Ȩ����false�� + ָ��Ӧ���������ֳ��⣩
	bool accessAndResp(const QByteArray& data, const QString& ipSrc, const ushort& portSrc);

private:
	bool send(const QByteArray& cmd) const;
	void writeLog(const QString& slog)const;

private:
	//����Դ��ַ��Զ�˿ͻ��˵�ַ
	QString   mIpSrc;
	ushort    mPortSrc;
};


#endif  //ACSTROTANALYZECORE_H_20210318_1350