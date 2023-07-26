#ifndef PROTOCOLACSTROT_H_20210312_1406
#define PROTOCOLACSTROT_H_20210312_1406

#include <QByteArray>
#include <Windows.h>
#include <QString>
#include <QStringList>
#include <QHostAddress>

#pragma execution_character_set("utf-8")
#pragma pack( push, 1 )


//protocol-format of Acoustic and rotary and so on
typedef struct ProtcARFmt
{
	enum ePackInfo
	{
		Head     = 0x7e7e,    //֡ͷ
		Tail     = 0x0a0d,    //֡β
	};

	//ָ����
	//ָ��Ӧ�� ָ�������·�һһ��Ӧ���������ݰ�
	//��������Ӧ�����·���Ӧ�������ݰ�
	//RESPONSE �� SET��GETӵ����ͬ��ָ����
	enum eCmdCode
	{
		//����
		NetHand = 0x0001,    //NET ��������DP

		//����ȨAccess
		AccApply       = 0x0101,   //SET �������Ȩ
		AccQuery       = 0x0102,   //GET ��ѯ����Ȩ
		AccRepApply    = 0x0181,   //REPORT ����Ȩ��Ӧresponse
		AccRepState    = 0x0182,   //REPORT �ϱ�����Ȩ״̬

		//����
		AcstSetFlName  = 0x0201,  //SET ���ò�����Ŀ
		AcstPlayCtrl   = 0x0202,  //SET ���ſ���
		AcstMuteCtrl   = 0x0203,  //SET ��������
		AcstVolumeCtrl = 0x0204,  //SET ��������
		AcstLoopModel  = 0x0205,  //SET ���ò���ģʽ
		AcstDeleteFile = 0x0206,  //SET ɾ��ָ���ļ�������Ŀ
		AcstQueryCount = 0x0207,  //GET ��ѯ��Ŀ����
		AcstQueryNames = 0x0208,  //GET ��ѯ��Ŀ�б�
		AcstQueryCurr  = 0x0209,  //GET ��ѯ��ǰ���ŵ���Ŀ��
		AcstQueryState = 0x020A,  //GET ��ѯ�����豸״̬
		AcstRepDelete  = 0x0281,  //REPORT ɾ����Ŀ�����Ӧ
		AcstRepCount   = 0x0282,  //REPORT �ϱ���Ŀ����
		AcstRepNames   = 0x0283,  //REPORT �ϱ���Ŀ�б�
		AcstRepCurr    = 0x0284,  //REPORT �ϱ���ǰ���ŵ���Ŀ����
		AcstRepState   = 0x0285,  //REPORT �ϱ������豸״̬

		//ת̨
		RotStop        = 0x0301,    //SET תֹ̨ͣ
		RotTurnDirec   = 0x0302,    //SET ��̬���ƣ���������
		RotSetVel      = 0x0303,    //SET ת������
		RotTurnTo      = 0x0304,    //SET ת��פ����
		RotZero        = 0x0305,    //SET ����
		RotQueryState  = 0x0306,    //GET ��ѯ״̬״̬
		RotRepState    = 0x0381,    //REPORT �ϱ�ת̨״̬

		//�ۺϹ���(synthetical �ۺϵ�)
		SynWorkModel   = 0x8101,    //SET ����ģʽ����
		SynPrmAuto     = 0x8102,    //SET �Զ�ģʽ��������
		SynSetAutoRep  = 0x8103,    //SET �Զ��ϱ�ʹ�ܿ���
		SynPowerOffPC  = 0x8104,    //SET ���ػ��ػ�
		SynQueryState  = 0x8105,    //GET ��ѯ�ۺ�״̬
		SynQueryPrmAuto = 0x8106,   //GET ��ѯ�Զ�����
		SynTiming       = 0x8107,   //SET ��ʱ
		SynRepState    = 0x8181,    //REPORT �ϱ��ۺ�״̬
		SynRepPrmAuto  = 0x8182,    //REPORT �ϱ��Զ�ģʽ����
	};

	//��������
	enum eMsgType
	{
		MsgNone     = 0,  //ȱʡ���ͣ���Э��Լ������ȱʡ
		MsgSet      = 1,  //���п���
		MsgGet      = 2,  //���в�ѯ
		MsgNet      = 3,  //����/���� ��������
		MsgResponse = 4,  //���У���SET��GET���ĵ�Ӧ��
		MsgReport   = 5,  //���У�ִ�н����Ӧ��״̬�ϱ��������ϱ���
	};

	typedef struct Prep
	{
		WORD    mHead;        //֡ͷ0x7e7e
		WORD    mLen;         //�������ݳ��ȣ���֡ͷ��֡β������

		//����������ȷ���շ���ʱ����setAddr��ֵ����Ҫ���͵Ľṹ�壩
		DWORD   mAddrSrc;     //Դ��ַ�����ͷ�IP
		DWORD   mAddrDst;     //Ŀ�ĵ�ַ�����շ�IP
		
		//���������ڽ���ᶨ��ʱ��ֵ����Ҫ���͵Ľṹ�壩
		WORD    mType;        //��������
		WORD    mCmdCode;     //�����룺���ֲ�ָͬ�����
		
		//��ȷ�зְ���������⣨���ܵ�����Ŀ�б��������������Ĭ��ֵ 1,1
		WORD    mPackCount;   //�ܰ�����δ�ְ���1���ְ�����ʵ�������д
		WORD    mPackNo;      //��ǰ���ţ���1��ʼ��ţ����ֵΪ�ܰ���

		Prep()
		{
			memset( this, 0, sizeof(*this) );
			mHead      = Head;
			mPackCount = 1;
			mPackNo    = 1;
		}

		int ipToAddr(const QString& ip) const
		{
			return QHostAddress(ip).toIPv4Address();
		}

		QString addrToIP(int addr) const
		{
			return QHostAddress(addr).toString();
		}

		void setAddr(const QString& ipSrc, const QString& ipDst)
		{
			mAddrSrc = ipToAddr(ipSrc);
			mAddrDst = ipToAddr(ipDst);
		}

	}Prep;

	typedef QByteArray DataArea;  //�����򣬱䳤

	typedef struct Post
	{
		WORD    mCheck;       //У��ͣ���֡ͷ�����������ݰ����ֽںͣ�������׺
		WORD    mTail;        //֡β��0x0a0d
		Post()
		{
			mCheck = 0;
			mTail  = Tail;
		}
	}Post;

	static const int gLenPrep = sizeof( ProtcARFmt::Prep );
	static const int gLenPost = sizeof( ProtcARFmt::Post );
	static const int gLenPrepPost = gLenPrep + gLenPost;

	static const int gLenMaxPack = 1024;  //������󳤶�
	static const int gLenMaxArea = gLenMaxPack - gLenPrepPost;  //������������󳤶�

	Prep     mPrep;
	DataArea mData;
	Post     mPost;

	//����ָ��ת��ָ����ʽ������У��͡������lenΪʵ�����ݳ��ȣ��ṹ�峤�ȣ�
	//ǰ׺����Ҫ��ǰ����pData�е�Դ��ַ��Ŀ�ĵ�ַ
    QByteArray pack( const void *pData, int len )
	{
		//����Ϸ����о�
		if (pData == NULL || len < gLenPrepPost)
		{
			return QByteArray();
		}

		//���ݸ�ֵ
		const char *p = (const char*)pData;
		memcpy( &mPrep, &p[0], gLenPrep );
		QByteArray dataArea = QByteArray( &p[gLenPrep], len - gLenPrepPost );
		memcpy( &mPost, &p[len - gLenPost], gLenPost );

		//����ְ���
		int countPack = dataArea.size() / gLenMaxArea;
		countPack = (dataArea.size() % gLenMaxArea == 0) ? countPack : (countPack + 1);

		//��������Ϊ��ʱ���ְ���Ϊ1
		countPack = countPack < 1 ? 1 : countPack;
		
		//�����ܰ���
		mPrep.mPackCount = countPack;

		//�ְ�����
		QByteArray baRes;
		int packNo = 0;
		int index = 0;
		//for ( int i = 0; i < dataArea.size(); i = i + gLenMaxArea )
		for(packNo=0; packNo < countPack; )
		{
			//����������
			mData = dataArea.mid(index, gLenMaxArea);
			index += mData.size();

			//���°���
			mPrep.mPackNo = (++packNo);

			//���³��ȣ����������������֮��
			mPrep.mLen = gLenPrepPost + mData.size();

			//����У��ͣ������ڳ�У������������ݸ������֮��
			mPost.mCheck = checkSum();

			//�����ƴ��
			baRes.append((const char*)&mPrep, gLenPrep);
			baRes.append(mData);
			baRes.append((const char*)&mPost, gLenPost);
		}

		//�������ݰ�����ƴ�Ӻ󷵻�
		return baRes;
	}

	//���أ�data���ṹ������װ��QByteArray
	QByteArray pack(const QByteArray& data)
	{
		return pack(data.constData(), data.size());
	}

	//�������ݰ�ʵ�ʳ���
	int getSize() const 
	{
		return (gLenPrepPost + mData.size());
	}

	//����У���
	static WORD checkSum(const char* p, int len)
	{
		WORD sum = 0;
		for (int i=0; i<len - gLenPost; i++)
		{
			sum += (WORD)(BYTE)p[i];
		}
		return sum;
	}

	//�����ݰ���ȡԴ��ַ
	static int getAddrSrc(const QByteArray& cmd)
	{
		if (cmd.size() < gLenPrepPost)
		{
			return 0;
		}

		Prep stP;
		memcpy(&stP, &cmd.constData()[0], gLenPrep);
		return stP.mAddrSrc;
	}

	//��һ�����ݰ���ȡָ����
	static WORD getCode( const QByteArray& cmd )
	{
		if (cmd.size() < gLenPrepPost)
		{
			return 0;
		}

		Prep stP;
		memcpy(&stP, &cmd.constData()[0], gLenPrep);
		return stP.mCmdCode;
	}

private:
	//����У��ͣ�ȱʡ����Ϊ�ṹ������
	WORD checkSum() const 
	{
		WORD sum = 0;

		char *p = (char*)&mPrep;
		for (int i=0; i<gLenPrep; i++)
		{
			sum += (WORD)(BYTE)p[i];
		}

		for (int i=0; i<mData.size(); i++)
		{
			sum += (WORD)(BYTE)mData.at(i);
		}

		return sum;
	}

}ProtcARFmt;


/************************************
** ͨ�ýṹ��
*************************************/
//�ṩ��̬����ӿ�
struct PrtARPack
{
	static QByteArray pack(const void* pData, int len)
	{
		ProtcARFmt cmd;
		return cmd.pack(pData, len);
	}

	static QByteArray pack(const QByteArray& data)
	{
		ProtcARFmt cmd;
		return cmd.pack(data);
	}
};

//��������ͨ�ã������ڽ���
struct PrtARNoDataUniversal
{
	ProtcARFmt::Prep mPrep;
	ProtcARFmt::Post mPost;
};

//ָ��Ӧ��ͨ�ýṹ��
struct PrtARCmdReponse
{
	ProtcARFmt::Prep mPrep;
	ProtcARFmt::Post mPost;

	PrtARCmdReponse(const ProtcARFmt::Prep& cmdPrep)
	{
		//mPrep.mLen = sizeof(PrtARCmdReponse); //���ȴ��ʱ�Զ�����
		mPrep.mType = ProtcARFmt::MsgResponse;
		mPrep.mCmdCode = cmdPrep.mCmdCode;
		mPrep.mAddrSrc = cmdPrep.mAddrDst;
		mPrep.mAddrDst = cmdPrep.mAddrSrc;
	}

	PrtARCmdReponse(const char* pData)
	{
		memcpy(&mPrep, &pData[0], ProtcARFmt::gLenPrep);
		mPrep.mType = ProtcARFmt::MsgResponse;

		DWORD tmp = mPrep.mAddrSrc;
		mPrep.mAddrSrc = mPrep.mAddrDst;
		mPrep.mAddrDst = tmp;
	}

	QByteArray pack()
	{
		return PrtARPack::pack(this, sizeof(*this));
	}
};


/************************************
** ����Ϊ����ָ��ṹ��
*************************************/
//��������
struct PrtARNetHand
{
	ProtcARFmt::Prep mPrep;

	struct 
	{
		unsigned int mNo;    //���ֱ��
	}mData;

	ProtcARFmt::Post mPost;

	PrtARNetHand()
	{
		mData.mNo = 0;
		mPrep.mType = ProtcARFmt::NetHand;
		mPrep.mCmdCode = ProtcARFmt::NetHand;
	}
};


/***************************************************
** ����Ȩ����
****************************************************/
//����Ȩ����
struct PrtAccApply
{
	enum eCmdIdt
	{
		Apply         = 0,  //�������Ȩ
		ApplyCancel   = 1,  //ȡ������Ȩ����
		FreeAccept    = 2,  //ͬ���ͷſ���Ȩ
		FreeReject    = 3,  //�ܾ��ͷſ���Ȩ
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mCmd;    //ָ����
	}mData;

	ProtcARFmt::Post mPost;
};

//��ѯ����Ȩ
typedef PrtARNoDataUniversal PrtAccQuery;

//����Ȩ��Ӧ
struct PrtAccRepApply
{
	ProtcARFmt::Prep mPrep;

	enum eCmdIdt
	{
		NotifApply    = 0,  //֪ͨ�������ͷſ���Ȩ��
		NotifCancel   = 1,  //֪ͨ��ȡ�����롱
		NotifAcquire  = 2,  //֪ͨ����ÿ���Ȩ��
		NotifLost     = 3,  //֪ͨ��ʧȥ����Ȩ��
	};

	struct
	{
		BYTE mCmd;    //ָ����
	}mData;

	ProtcARFmt::Post mPost;

	PrtAccRepApply()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AccRepApply;
		mData.mCmd = NotifApply;
	}
};

//�ϱ�����Ȩ
struct PrtAccRepState
{
	ProtcARFmt::Prep mPrep;

	enum eCmdIdt
	{
		Acquired     = 0,  //�ѻ�ÿ���Ȩ
		Lost         = 1,  //��ʧȥ����Ȩ
		ApplyFirst   = 2,  //�����У���һ˳λ
		ApplyWait    = 3,  //�����У��ǵ�һ˳λ
	};

	struct
	{
		BYTE mCmd;    //ָ����
	}mData;

	ProtcARFmt::Post mPost;

	PrtAccRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AccRepState;
		mData.mCmd = Acquired;
	}
};




/***************************************************
** �����豸
****************************************************/
//���ò�����Ŀ��ע��ýṹ�岻��������memcpy��memset
struct PrtAcstSetFlName
{
	ProtcARFmt::Prep mPrep;
	
	struct
	{
		BYTE    mLen;    //��Ŀ���ֽ�����ע�⣬���ֽ���
		QString mName;   //��Ŀ��

		//�ļ�����С��Ч����
		int nameMinLenValid() const
		{
			return 5;  //"1.mp3" "1.wav"
		}

		//��������С��Ч����
		int dataMinLenValid() const
		{
			return sizeof(mLen) + nameMinLenValid();  
		}
	}mData;

	ProtcARFmt::Post mPost;

	//���ڽ�������ֵ
	bool fromBytes(const char* pData, int len)
	{
		if (len < ProtcARFmt::gLenPrepPost + mData.dataMinLenValid())
		{
			return false;
		}

		int index = 0;
		memcpy(&mPrep, &pData[index], ProtcARFmt::gLenPrep);
		index += ProtcARFmt::gLenPrep;
		if (mPrep.mLen > len)
		{
			return false;
		}

		//����mLen
		memcpy(&mData.mLen, &pData[index], sizeof(mData.mLen));
		index += sizeof(mData.mLen);
		if (mData.mLen < mData.nameMinLenValid() ||
			mPrep.mLen != ProtcARFmt::gLenPrepPost + mData.mLen + sizeof(mData.mLen))
		{
			return false;
			
		}

		mData.mName = QString::fromUtf8( QByteArray(&pData[index], mData.mLen) );
		index += mData.mLen;

		memcpy(&mPost, &pData[index], ProtcARFmt::gLenPost);
		return true;
	}
};

//���ſ���
struct PrtAcstPlayCtrl
{
	enum eIdtCmd
	{
		Playing    = 0,  //����
		PlayPause  = 1,  //��ͣ
		PlayStop   = 2,  //����
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mCmd;  //ָ���룺0-���ţ�1-��ͣ��2-����
	}mData;

	ProtcARFmt::Post mPost;
};

//��������
struct PrtAcstMuteCtrl
{
	enum eIdtMute
	{
		MuteOn  = 0,  //����
		MuteOff = 1,  //�Ǿ���
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE  mMute;  //0-������1-�Ǿ���
	}mData;

	ProtcARFmt::Post mPost;
};

//��������
struct PrtAcstVolumeCtrl
{
	enum eIdtVolume
	{
		VolPlayer    = 0, //����������
		VolDevice    = 1, //�豸����
		VolBoth      = 2, //�������������豸����
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mIdt;  //0-������������1-�豸������ 2-�������������豸����
		BYTE    mVolPlayer;  //0-100
		BYTE    mVolDevice;  //0-100
	}mData;

	ProtcARFmt::Post mPost;
};

//����ģʽ
struct PrtAcstLoopModel
{
	enum eIdtLoop
	{
		LoopAll = 0,  //�б�ѭ��
		LoopOne = 1,  //����ѭ��
	};

	ProtcARFmt::Prep mPrep;

	struct 
	{
		BYTE    mLoop;    //0-�б�ѭ���� 1-����ѭ��
	}mData;

	ProtcARFmt::Post mPost;
};

//ɾ��ָ����Ŀ�������ã������ò�����Ŀ�ṹ����ͬ
typedef PrtAcstSetFlName PrtAcstDeleteFile;

//��ѯ��Ŀ����
typedef PrtARNoDataUniversal PrtAcstQueryCount;

//��ѯ��Ŀ�б�
typedef PrtARNoDataUniversal PrtAcstQueryNames;

//��ѯ��ǰ���ŵ���Ŀ����
typedef PrtARNoDataUniversal PrtAcstQueryCurr;

//��ѯ�����豸״̬
typedef PrtARNoDataUniversal PrtAcstQueryState;

//ɾ����Ŀ�����Ӧ
struct PrtAcstRepDelete
{
	enum eIdtResualt
	{
		Succeed      = 0,  //ɾ���ɹ�
		ErrorName    = 1,  //ɾ��ʧ�ܣ��ļ�������
		ErrorUsing   = 2,  //ɾ��ʧ�ܣ��ļ����ڱ�ռ��
		ErrorUndef   = 3,  //ɾ��ʧ�ܣ���������
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mResult;   //������룺0ɾ���ɹ�������ɾ��ʧ��
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepDelete(eIdtResualt idt = Succeed)
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepDelete;
		mData.mResult = Succeed;
	}
};

//�ϱ���Ŀ����
struct PrtAcstRepCount
{
	ProtcARFmt::Prep mPrep;

	struct 
	{
		WORD mCount;    //��Ŀ����
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepCount()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepCount;
		mData.mCount = 0;
	}
};

//�ϱ���Ŀ�б�����ṹ�壬�ýṹ�岻��������memcpy��memset
struct PrtAcstRepNames
{
	ProtcARFmt::Prep mPrep;

	struct
	{
		WORD             mCount;
		QStringList      mNames;
	}mData;

	ProtcARFmt::Post mPost;

	typedef struct MscUnit
	{
		WORD        mOrder;    //��Ŀ���
		BYTE        mLen;      //�ļ�������
		QByteArray  mName;     //�ļ���

		MscUnit(int order, const QString& name)
		{
			mOrder = order;
			mName = name.toUtf8();

			//��������ʱ���ļ����ַ��� �� �ֽ��� �����
			mLen = mName.size();  
		}

		QByteArray toByteArray() const
		{
			int len = sizeof(MscUnit) - sizeof(QByteArray);
			const char *p = (const char*)this;
			QByteArray res = QByteArray(&p[0], len);
			return res.append(mName);
		}
	}MscUnit;

	PrtAcstRepNames()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepNames;
		mData.mCount = 0;
	}

	QByteArray toByteArray()
	{
		QByteArray baRes;

		//ѹ��ǰ׺
		baRes.append((const char*)&mPrep, sizeof(mPrep));

		//������Ŀ������ѹ�뻺��
		mData.mCount = mData.mNames.size();
		baRes.append((const char*)&mData.mCount, sizeof(mData.mCount));

		//��������Ŀ����Ϣƴ������
		for (int i = 0; i < mData.mCount; i++)
		{
			baRes.append( MscUnit(i + 1, mData.mNames.at(i)).toByteArray() );
		}

		//ѹ���׺
		baRes.append((const char*)&mPost, sizeof(mPost));

		//������������֡���ݼ�ƴ�Ӻ󷵻أ��ȴ���һ���������
		return baRes;
	}
};

//�ϱ���ǰ���ŵ���Ŀ���ƣ��ýṹ�岻��������memcpy��memset
struct PrtAcstRepCurr
{
	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mLen;   //�ļ������ȣ��ֽ���
		QString mName;  //�ļ���
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepCurr()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepCurr;
		mData.mLen = 0;
	}

	QByteArray toByteArray()
	{
		//���³���
		QByteArray baName = mData.mName.toUtf8();
		mData.mLen = baName.size();

		QByteArray baData;
		baData.append((const char*)&mPrep, ProtcARFmt::gLenPrep);
		baData.append((const char*)&mData.mLen, sizeof(mData.mLen));
		baData.append(baName);
		baData.append((const char*)&mPost, ProtcARFmt::gLenPost);
		
		//���ؼ�ƴ�Ӻ�����ݣ��ȴ���һ�����
		return baData;
	}
};

//�ϱ������豸״̬
struct PrtAcstRepState
{
	enum eIdtPlay
	{
		Playing    = 0,  //����
		PlayPause  = 1,  //��ͣ
		PlayEnd    = 2,  //����
	};

	enum eIdtMute
	{
		MuteOn = 0,  //����
		MuteOff = 1,  //�Ǿ���
	};

	enum eIdtLoop
	{
		LoopAll = 0,  //�б�ѭ��
		LoopOne = 1,  //����ѭ��
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mPlay;  //����״̬��0���ţ�1��ͣ��2����
		BYTE    mMute;  //����״̬��0������1�Ǿ���
		BYTE    mVolPlayer;  //����������0-100
		BYTE    mVolDevice;  //���ػ�����0-100
		BYTE    mLoop;       //����ģʽ��0�б�ѭ����1����ѭ��
		DataArea()
		{
			memset(this, 0, sizeof(*this));
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepState;
	}
};




/***************************************************
** �ŷ��豸
****************************************************/
//תֹ̨ͣ
typedef PrtARNoDataUniversal PrtRotStop;

//ת̨��̬����
struct PrtRotTurnDirec
{
	enum eIdtCmd
	{
		TurnUp    = 0,
		TurnDown,
		TurnLeft,
		TurnRight,
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mIdt;  //0�ϣ�1�£�2��3��
	}mData;

	ProtcARFmt::Post mPost;
};

//ת̨ת������
struct PrtRotSetVel
{
	enum eIdt
	{
		AziOnly   = 0,      //����λ��Ч
		PitOnly   = 1,      //��������Ч
		AziAndPit = 2,      //��λ����������Ч
		//�������Ч
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mIdt;       //���λ��0��λ��1������2��λ����
		WORD mVelAzi;    //��λת�٣�������λ0.01��ÿ��
		WORD mVelPit;    //����ת�٣�������λ0.01��ÿ��
	}mData;

	ProtcARFmt::Post mPost;
};

//ת̨ת��פ����
struct PrtRotTurnTo
{
	enum eIdt
	{
		AziOnly = 0,      //����λ��Ч
		PitOnly = 1,      //��������Ч
		AziAndPit = 2,    //��λ����������Ч
		//�������Ч
	};

	ProtcARFmt::Prep mPrep;

	struct 
	{
		BYTE  mIdt;    //���λ����eIdt
		WORD  mAzi;    //��λ�ǣ�������λ0.01��
		short mPit;    //�����ǣ�������λ0.01��
	}mData;

	ProtcARFmt::Post mPost;
};

//ת̨����
struct PrtRotZero
{
	enum eIdt
	{
		AziOnly = 0,      //����λ��Ч
		PitOnly = 1,      //��������Ч
		AziAndPit = 2,    //��λ����������Ч
		//�������Ч
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mIdt;    //0��λ��1������2��λ����
	}mData;

	ProtcARFmt::Post mPost;
};

//��ѯת̨״̬
typedef PrtARNoDataUniversal PrtRotQueryState;

//�ϱ�ת̨״̬
struct PrtRotRepState
{
	enum eIdtMove
	{
		MoveArrived = 0,     //��λ
		Moving = 1,          //�˶���
		MoveInvalid = 0xFF,  //�˶����λ��Ч������
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mIdtMove;  //��ǰ���ã���Ч��0xFF
		WORD    mAzi;      //��λפ���ǣ�0-359.99��������λ0.01��
		short   mPit;      //����פ���ǣ�-90��90��������λ0.01��
		WORD    mAziDelt;  //��װʱ��λƫ�ǣ�0-359.99��������λ0.01��
		short   mPitDelt;  //��װʱ����ƫ�ǣ�-60��60��������λ0.01��
		WORD    mVelAzi;   //��λת�٣�������λ0.01��/s
		WORD    mVelPit;   //����ת�٣�������λ0.01��/s

		DataArea()
		{
			memset(this, 0, sizeof(*this));
			mIdtMove = 0xFF;
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtRotRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::RotRepState;
	}
};




/************************************
** �ۺϹ���
*************************************/
//����ģʽ����
struct PrtSynWorkModel
{
	enum eIdtModel
	{
		WorkAuto    = 0,  //�Զ�ģʽ
		WorkManul   = 1,  //�ֶ�ģʽ
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mIdt;    //0�Զ���1�ֶ�
	}mData;

	ProtcARFmt::Post mPost;
};

//�Զ�ģʽ��������
struct PrtSynPrmAuto
{
	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		WORD    mSecStart;    //ÿ�칤����ʼʱ��0-1439��Ĭ��0
		WORD    mSecEnd;      //ÿ�칤������ʱ��0-1439��Ĭ��0

		WORD    mAziStart;    //��λ��ʼ�ǣ�0-359.99��������λ0.01��
		WORD    mAziEnd;      //��λ��ֹ�ǣ�0-359.99��������λ0.01��
		WORD    mAziStep;     //��λɨ�貽����5-180��������λ0.01��
		 
		short   mPitStart;    //������ʼ�ǣ�������λ0.01�㣨��ǰ���øýǶ���Ϊ����פ���ǣ�
		short   mPitEnd;      //������ֹ�ǣ�������λ0.01�㣨��ǰ��Ч�����ã���0��
		WORD    mPitStep;     //����ɨ�貽����������λ0.01�㣨��ǰ��Ч�����ã���0��

		WORD    mVelAzi;      //��λת�٣�������λ0.01��ÿ��
		WORD    mVelPit;	  //����ת�٣�������λ0.01��ÿ��

		WORD    mSecResident; //פ��ʱ�䣬s

		DataArea()
		{
			memset(this, 0, sizeof(*this));
		}
	}mData;

	ProtcARFmt::Post mPost;
};

//�Զ��ϱ�ʹ�ܿ���
struct PrtSynSetAutoRep
{
	enum eIdtRepAuto
	{
		RepAutoOn   = 0,  //�Զ��ϱ���
		RepAutoOff  = 1,  //�Զ��ϱ���
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mAcst;  //0�Զ��ϱ�����1�Զ��ϱ���
		BYTE    mRot;   //0�Զ��ϱ�����1�Զ��ϱ���
		BYTE    mSyn;   //0�Զ��ϱ�����1�Զ��ϱ���
	}mData;

	ProtcARFmt::Post mPost;
};

//���ػ��ػ�
typedef PrtARNoDataUniversal PrtSynPowerOffPC;

//��ѯ�ۺ�״̬
typedef PrtARNoDataUniversal PrtSynQueryState;

//��ѯ�Զ�����
typedef PrtARNoDataUniversal PrtSynQueryPrmAuto;

//��ʱ
struct PrtSynTiming
{
	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		unsigned short    mYear;
		unsigned char     mMon;
		unsigned char     mDay;
		unsigned char     mHour;    //0-23
		unsigned char     mMinute;  //0-59
		unsigned char     mSec;     //0-59

		DataArea()
		{
			memset(this, 0, sizeof(DataArea));
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtSynTiming()
	{
		mPrep.mType = ProtcARFmt::MsgSet;
		mPrep.mCmdCode = ProtcARFmt::SynTiming;
	}
};

//�ϱ��ۺ�״̬
struct PrtSynRepState
{
	enum eIdtModel
	{
		WorkAuto  = 0,  //�Զ�ģʽ
		WorkManul = 1,  //�ֶ�ģʽ
	};

	enum eIdtRepAuto
	{
		RepAutoOn = 0,  //�Զ��ϱ���
		RepAutoOff = 1,  //�Զ��ϱ���
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		PrtSynWorkModel::DataArea    mWorkModel;  //����ģʽ��0�Զ���1�ֶ�
		PrtSynSetAutoRep::DataArea   mRepAuto;    //�Զ��ϱ�ʹ�ܣ�0�Զ��ϱ�����1�Զ��ϱ���
		DataArea()
		{
			memset(this, 0, sizeof(*this));
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtSynRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::SynRepState;
	}
};

//�ϱ��Զ�ģʽ����
struct PrtSynRepPrmAuto
{
	ProtcARFmt::Prep          mPrep;
	PrtSynPrmAuto::DataArea   mData;
	ProtcARFmt::Post          mPost;

	PrtSynRepPrmAuto()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::SynRepPrmAuto;
	}
};


#pragma pack(pop)

#endif  //PROTOCOLACSTROT_H_20210312_1406


