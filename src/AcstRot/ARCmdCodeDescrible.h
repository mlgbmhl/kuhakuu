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
	QMap<ushort, QString>   mMap;    //下行指令：指令码 --> 指令描述
	QVector<ushort>         mVcAcc;  //需要权限控制的指令 的指令码集合（下行）
	QVector<ushort>         mVcAutoIgn;  //自动模式下呗忽略的指令指令码集合（下行）
	QMutex                 *mMutex;
};

#endif  //ARCMDCODEDESCRIBLE_H_20201123_1504