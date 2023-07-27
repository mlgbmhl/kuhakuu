#pragma once

#include "WebCtrlUserInfo.h"
#include <QObject>
#include <QVector>

/*****************************************************************************
** 本类管理登录成功的用户：ip 和 port
** 登录成功则存；失去登录则移除
**
** 失去登录包括如下情况：
** case1：用户注销登录
** case2：用户断开连接
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

    //增加一个用户：用户登录成功后添加； 若用户已存在或超过最大允许连接数，则返回false
    bool appendUser(const QString& ip, const ushort& port);
    bool appendUser(const UserInfo& user);

    //移除一个已有用户：当已登录用户失去登录后（如注销、连接断开等），则移除；若用户不存在则返回false
    bool removeUser(const QString& ip, const ushort& port);
    bool removeUser(const UserInfo& user);

    //获取所有用户信息
    QVector<UserInfo> getUserInfoAll() const;

    //查询给定ip和port的连接是否存在
    bool isExistedUser(const QString& ip, ushort port) const;
    bool isExistedUser(const UserInfo& user) const;

    //查询给定ip和port的连接是否可用：已登录通过，处于可通信状态
    bool isAvailableUser(const QString& ip, const ushort& port) const;
    bool isAvailableUser(const UserInfo& user) const;

    //查询允许的最大链接数
    inline int getNumMax() const;

    //查询当前已有链接数
    inline int getNumCurrent() const;

    //查询当前链接数是否已达上限
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

