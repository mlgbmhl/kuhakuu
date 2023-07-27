#pragma once

#include "WebCtrlUserInfo.h"
#include <QObject>
#include <QVector>

/*****************************************************************************
** ��������¼�ɹ����û���ip �� port
** ��¼�ɹ���棻ʧȥ��¼���Ƴ�
**
** ʧȥ��¼�������������
** case1���û�ע����¼
** case2���û��Ͽ�����
******************************************************************************/

#define gWebCtrlUsers    WebCtrlUserManager::GetInstance()

class WebCtrlUserManager : public QObject
{
    Q_OBJECT;

private:
    WebCtrlUserManager();

public:
    using UserInfo = WebCtrlUserInfo;
    ~WebCtrlUserManager();
    static WebCtrlUserManager* GetInstance();

    //����һ���û����û���¼�ɹ�����ӣ� ���û��Ѵ��ڻ򳬹�����������������򷵻�false
    bool appendUser(const QString& ip, const ushort& port);
    bool appendUser(const UserInfo& user);

    //�Ƴ�һ�������û������ѵ�¼�û�ʧȥ��¼����ע�������ӶϿ��ȣ������Ƴ������û��������򷵻�false
    bool removeUser(const QString& ip, const ushort& port);
    bool removeUser(const UserInfo& user);

    //��ȡ�����û���Ϣ
    QVector<UserInfo> getUserInfoAll() const;

    //��ѯ����ip��port�������Ƿ����
    bool isExistedUser(const QString& ip, ushort port) const;
    bool isExistedUser(const UserInfo& user) const;

    //��ѯ����ip��port�������Ƿ���ã��ѵ�¼ͨ�������ڿ�ͨ��״̬
    bool isAvailableUser(const QString& ip, const ushort& port) const;
    bool isAvailableUser(const UserInfo& user) const;

    //��ѯ��������������
    inline int getNumMax() const;

    //��ѯ��ǰ����������
    inline int getNumCurrent() const;

    //��ѯ��ǰ�������Ƿ��Ѵ�����
    inline bool isFull() const;

private:
    inline int indexOfUserInfo(const QString& ip, const unsigned short& port) const;
    inline int indexOfUserInfo(const UserInfo& user) const;

private:
    QVector<UserInfo>                mUserInfos;
    const int                        mNumMax;
};


int WebCtrlUserManager::getNumMax() const
{
    return mNumMax;
}

inline int WebCtrlUserManager::getNumCurrent() const
{
    return mUserInfos.size();
}

inline bool WebCtrlUserManager::isFull() const
{
    return mUserInfos.size() >= mNumMax;
}

inline int WebCtrlUserManager::indexOfUserInfo(
    const QString & ip, const unsigned short & port) const
{
    return mUserInfos.indexOf(UserInfo(ip, port));
}

inline int WebCtrlUserManager::indexOfUserInfo(const UserInfo & user) const
{
    return mUserInfos.indexOf(user);
}

