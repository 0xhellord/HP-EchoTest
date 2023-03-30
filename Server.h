#pragma once
#include <cstdint>
#include <hpsocket/HPTypeDef.h>
#include <iostream>
#include <atomic>
#include <hpsocket/GlobalDef.h>
#include <hpsocket/HPSocket.h>

/* Listener Class */
class CRPCServer : public CTcpServerListener
{

    private:
        CTcpPackServerPtr m_ServerPtr;
        std::atomic<uint64_t> m_TotalPkts;

      public:
        CRPCServer():m_ServerPtr(this)
        {
            m_TotalPkts = 0;
        }

        uint64_t GetTotalPkts()
        {
            return m_TotalPkts.load();
        }

    public:
        // 5. process network events
        virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
        {
            return  HR_OK;
        }
        virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
        {
            return  HR_OK;
        }
        virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID)
        {
            return  HR_OK;
        }
        virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE *pBuffer, int iLength)
        {
            m_TotalPkts.fetch_add(1);
            return  pSender->Send(dwConnID, pBuffer, iLength) ? HR_OK : HR_ERROR;
        }
        virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
        {
            return  HR_OK;
        }
        virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
        {
            std::cout << "Server OnClose:" << dwConnID << std::endl;
            return  HR_OK;
        }
        virtual EnHandleResult OnShutdown(ITcpServer* pSender)
        {
            return  HR_OK;
        }

    public:
        BOOL StartServer(USHORT port = 5555)
        {
            //m_ServerPtr->SetWorkerThreadCount(32);
            //m_ServerPtr->SetMaxPackSize(0x3FFFFF);
            //m_ServerPtr->SetSendPolicy(SP_DIRECT);
            //m_ServerPtr->SetNoDelay(TRUE);
            return m_ServerPtr->Start("0.0.0.0", port);
        }

};