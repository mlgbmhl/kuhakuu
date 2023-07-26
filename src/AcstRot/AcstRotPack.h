#pragma once

#include <QByteArray>
#include <QString>
#include "ProtocolAcstRot.h"

#define gPackAR    AcstRotPack::getInstance()

class AcstRotPack
{
private:
	AcstRotPack();

public:
	static AcstRotPack* getInstance();
	~AcstRotPack();

	void setAddrSrc(uint addr);


	//控制权响应PrtAccRepApply::eCmdIdt：通知：0-申请释放；1-取消申请释放；2-获得；3-失去；其余无效
	QByteArray accRepApply(const int& idt, int addrDst) const;

	//上报控制权PrtAccRepState::eCmdIdt：0-已获得；1-已失去；2-申请中，第一顺位；3-申请中，非第一顺位
	QByteArray accRepState(const int& idt, int addrDst) const;


	//删除曲目结果响应PrtAcstRepDelete::eIdtResualt：0删除成功；1删除失败，文件名错误；2删除失败，文件被占用；3删除失败，其他错误
	QByteArray acstRepDelete(const int& idt, int addrDst) const;

	//上报曲目总数
	QByteArray acstRepCount(int addrDst) const;

	//上报曲目列表
	QByteArray acstRepNames(int addrDst) const;

	//上报当前播放的曲目信息
	QByteArray acstRepCurr(int addrDst) const;

	//上报声能设备状态
	QByteArray acstRepState(int addrDst) const;


	//上报转台状态
	QByteArray rotRepState(int addrDst) const;


	//上报综合状态
	QByteArray synRepState(int addrDst) const;

	//上报自动模式参数
	QByteArray synRepPrmAuto(int addrDst) const;

private:
	uint    mAddrSrc;  //打包源地址：本地
};

