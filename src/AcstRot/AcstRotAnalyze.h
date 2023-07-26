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
		int      mLen;    //ָ�����Ӧ�ṹ�峤�ȣ�����ֵΪ-1����ṹ�峤�Ȳ���
		pFun     mFun;    //ָ�����Ӧ�Ľ�������
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

	//����Ҫ�ж�֡����ָ��
	QVector<ushort> mVcLenUnknown;

	//����ָ������ --> ָ���
	QMap<ushort, QVector<ushort>> mMapTypeToCodes;
};

#endif  //ACSTROTANALYZE_H_20210318_1342