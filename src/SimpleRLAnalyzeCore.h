#pragma once

#include <QByteArray>
#include <QString>

class SimpleRLAnalyzeCore
{
public:
	SimpleRLAnalyzeCore();
	~SimpleRLAnalyzeCore();

	void acousticStrong(const QByteArray& data) const;
	void acousticWeak(const QByteArray& data) const;
	void acousticPlay(const QByteArray& data) const;
	void acousticStop(const QByteArray& data) const;

	void lampWarning(const QByteArray& data) const;
	void lampBeat(const QByteArray& data) const;
	void lampBurstFlash(const QByteArray& data) const;
	void lampStop(const QByteArray& data) const;

	void rotaryUp(const QByteArray& data) const;
	void rotaryDown(const QByteArray& data) const;
	void rotaryLeft(const QByteArray& data) const;
	void rotaryRight(const QByteArray& data) const;
	void rotaryLeftUp(const QByteArray& data) const;
	void rotaryLeftDown(const QByteArray& data) const;
	void rotaryRightUp(const QByteArray& data) const;
	void rotaryRightDown(const QByteArray& data) const;
	void rotaryLocation(const QByteArray& data) const;
	void rotaryStop(const QByteArray& data) const;

	void devWgs(const QByteArray& data) const;
	void devAzi(const QByteArray& data) const;

private:
	void writeLog(const QString& slog) const;
	void rotaryTurnDirc(int direc) const;
};