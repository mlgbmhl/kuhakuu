#pragma once

#include <QObject>
#include "WebCtrlPack.h"

#define gWebCtrlSend    WebCtrlSend::GetInstance()

class WebCtrlSend : public QObject
{
    Q_OBJECT;

private:
    WebCtrlSend();

public:
    ~WebCtrlSend();
    static WebCtrlSend* GetInstance();

    //ͨ��Ӧ��
    void response(const QString& cmdCode, const QString& ip, ushort port) const;

    //����Ӧ��
    void repNtNetHand(ushort heartbeat, const QString& ip, ushort port) const;

    //��¼����
    //Ӧ���û���¼
    void repLgLoginRequst(bool succ, const QString& ip, ushort port) const;

    //Ӧ���޸��û���������
    void repLgUserNamePasswordMod(bool succ, const QString& ip, ushort port) const;

    //Ӧ�������û���������
    void repLgUserNamePasswordForg(bool succ, const QString& ip, ushort port) const;

    //Ӧ�𣺵�¼״̬ 
    void repLgLoginStat(bool succ, const QString& ip, ushort port) const;
    //�Զ��ϱ�����¼״̬ 
    void autoRepLgLoginStat() const;



    //�����豸����
    //Ӧ��ɾ��ָ���ļ�����Ŀ����succΪfalseʱ��������ȷ��д������
    //1-�ļ�������2-�ļ����ڱ�ռ�ã� 3-��������
    void repAcDelFile(WebCtrlPack::DelFileReslt resualt, const QString& ip, ushort port) const;

    //Ӧ����Ŀ����
    void repAcFileNum(bool succ, const QString& ip, ushort port) const;

    //Ӧ����Ŀ�б�
    void repAcFileList(bool succ, const QString& ip, ushort port) const;

    //Ӧ�𣺵�ǰ��Ŀ��
    void repAcFileCurrent(bool succ,  const QString& ip, ushort port) const;
    //�ض������Զ��ϱ�����ǰ��Ŀ��
    void autoRepAcFileCurrent() const;

    //Ӧ�������豸״̬
    void repAcStat(bool succ, const QString& ip, ushort port) const;
    //�ض������Զ��ϱ��������豸״̬
    void autoRepAcStat() const;


    //ת̨�豸����
    //Ӧ��ת̨״̬
    void repRtStat(bool succ, const QString& ip, ushort port) const;
    //�ض������Զ��ϱ���ת̨״̬
    void autoRepRtStat() const;


    //ǿ���豸����
    //Ӧ�� ǿ���豸״̬
    void repLsStat(bool succ, const QString& ip, ushort port) const;
    //�ض������Զ��ϱ��� ǿ���豸״̬
    void autoRepLsStat() const;


    //�ۺϹ���
    //Ӧ�𣺹���ģʽ
    void repCoWorkModel(bool succ, const QString& ip, ushort port) const;

private:
    bool send(const QByteArray& data, const QString& ip, ushort port) const;
    bool sendAll(const QByteArray& data) const;
    void log(QString slog, const QString& ip, ushort port) const;


    QByteArray acStat(int flag) const;
    QByteArray rtStat(int flag) const;
    QByteArray lsStat(int flag) const;

private:
    WebCtrlPack    *mPack;
};