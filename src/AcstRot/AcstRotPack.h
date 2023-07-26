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


	//����Ȩ��ӦPrtAccRepApply::eCmdIdt��֪ͨ��0-�����ͷţ�1-ȡ�������ͷţ�2-��ã�3-ʧȥ��������Ч
	QByteArray accRepApply(const int& idt, int addrDst) const;

	//�ϱ�����ȨPrtAccRepState::eCmdIdt��0-�ѻ�ã�1-��ʧȥ��2-�����У���һ˳λ��3-�����У��ǵ�һ˳λ
	QByteArray accRepState(const int& idt, int addrDst) const;


	//ɾ����Ŀ�����ӦPrtAcstRepDelete::eIdtResualt��0ɾ���ɹ���1ɾ��ʧ�ܣ��ļ�������2ɾ��ʧ�ܣ��ļ���ռ�ã�3ɾ��ʧ�ܣ���������
	QByteArray acstRepDelete(const int& idt, int addrDst) const;

	//�ϱ���Ŀ����
	QByteArray acstRepCount(int addrDst) const;

	//�ϱ���Ŀ�б�
	QByteArray acstRepNames(int addrDst) const;

	//�ϱ���ǰ���ŵ���Ŀ��Ϣ
	QByteArray acstRepCurr(int addrDst) const;

	//�ϱ������豸״̬
	QByteArray acstRepState(int addrDst) const;


	//�ϱ�ת̨״̬
	QByteArray rotRepState(int addrDst) const;


	//�ϱ��ۺ�״̬
	QByteArray synRepState(int addrDst) const;

	//�ϱ��Զ�ģʽ����
	QByteArray synRepPrmAuto(int addrDst) const;

private:
	uint    mAddrSrc;  //���Դ��ַ������
};

