#pragma once

#include <QByteArray>
#include <QMap>
#include "SimpleRLAnalyzeCore.h"

#define gAnalyzeSRL SimpleRotLaserAnalyze::getInstance()

class SimpleRotLaserAnalyze
{
private:
	SimpleRotLaserAnalyze();

public:
	~SimpleRotLaserAnalyze();
	static SimpleRotLaserAnalyze* getInstance();

	void append(const QByteArray& data,
		const QString& ipSrc, const ushort& portSrc);

private:
	void init();
	void preAnalyze(const QByteArray& data, 
		int& lenRem, const QString& ipSrc, const ushort& portSrc);

private:
	QByteArray mCache;
	SimpleRLAnalyzeCore *mCore;

	typedef void (SimpleRLAnalyzeCore::*pFunSRLCore)(const QByteArray&) const;
	QMap<unsigned short, pFunSRLCore> mMap;
};