#ifndef ARCMDCODEDESCRIBLE_H_20201123_1504
#define ARCMDCODEDESCRIBLE_H_20201123_1504

#include <QMap>
#include <QVector>
#include <QMutex>
#include <QString>
#include "ProtocolAcstRot.h"

#define gARCodeDescr    ARCmdCodeDescrible::getInstance()

class ARCmdCodeDescrible
{
private:
	ARCmdCodeDescrible();

public:
	~ARCmdCodeDescrible();
	static ARCmdCodeDescrible* getInstance();

	QString codeToStr(const ushort& code) const;
	bool needAccess(const ushort& cmdCode) const;
	bool ignoreWhenAuto(const ushort& cmdCode) const;

private:
	void initMap();
	void initVcAcc();
	void initVcAutoIgn();

private:
	QMap<ushort, QString>   mMap;    //����ָ�ָ���� --> ָ������
	QVector<ushort>         mVcAcc;  //��ҪȨ�޿��Ƶ�ָ�� ��ָ���뼯�ϣ����У�
	QVector<ushort>         mVcAutoIgn;  //�Զ�ģʽ���º��Ե�ָ��ָ���뼯�ϣ����У�
	QMutex                 *mMutex;
};

#endif  //ARCMDCODEDESCRIBLE_H_20201123_1504