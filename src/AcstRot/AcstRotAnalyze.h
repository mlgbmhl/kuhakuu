#ifndef ACSTROTANALYZE_H_20210318_1342
#define ACSTROTANALYZE_H_20210318_1342

#include <QString>
#include <QByteArray>
#include <QMap>
#include <QVector>
#include <QMutex>
#include "AcstRotAnalyzeCore.h"
#include "PD/LogManag/Log.h"

#include "SI/Common/XKCommon.h"

#define gAnalyzeAR    AcstRotAnalyze::getInstance()

class AcstRotAnalyze
{
private:
	AcstRotAnalyze();

public:
	static AcstRotAnalyze* getInstance();
	~AcstRotAnalyze();

	void append(const QByteArray& data, 
		const QString& ipSrc, const ushort& portSrc);

private:
	void initPrm();
	void preAnalyze(const QByteArray& data, 
		int& lenRem, const QString& ipSrc, const ushort& portSrc);

private:
	AcstRotAnalyzeCore *mCore;
	QMutex *mMutex;

	//IP:port to data
	QMap<QString, QByteArray> mCaches;

	typedef void (AcstRotAnalyzeCore::*pFun)(const QByteArray&);
	typedef struct CoreInfo
	{
		int      mLen;    //指令码对应结构体长度，若该值为-1，则结构体长度不定
		pFun     mFun;    //指令码对应的解析函数
		CoreInfo(){};
		CoreInfo(int len, pFun fun)
		{
			mLen = len;
			mFun = fun;
		}
		CoreInfo(const CoreInfo& other)
		{
			_copy(*this, other);
		}
		CoreInfo& operator= (const CoreInfo& other)
		{
			_copy(*this, other);
			return *this;
		}
	private:
		void _copy(CoreInfo& ob, const CoreInfo& other)
		{
			ob.mLen = other.mLen;
			ob.mFun = other.mFun;
		}
	}CoreInfo;
	QMap<ushort, CoreInfo> mMap;  //resp: code -> len

	//不需要判断帧长的指令
	QVector<ushort> mVcLenUnknown;

	//下行指令类型 --> 指令集和
	QMap<ushort, QVector<ushort>> mMapTypeToCodes;
};

#endif  //ACSTROTANALYZE_H_20210318_1342