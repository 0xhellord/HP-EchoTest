#pragma once
#include <atomic>
#include <cstdint>
#include <hpsocket/HPTypeDef.h>
#include <hpsocket/SocketInterface.h>
#include <iostream>

#include <hpsocket/GlobalDef.h>
#include <hpsocket/HPSocket.h>

/* Listener Class */
class CRPCClient : public CTcpAgentListener {

private:
  CTcpPackAgentPtr m_ClientPtr;
  std::atomic<uint64_t> m_TotalPkts;

public:
  CRPCClient() : m_ClientPtr(this) { m_TotalPkts = 0; }

  uint64_t GetTotalPkts() { return m_TotalPkts.load(); }

public:
  virtual EnHandleResult OnPrepareListen(ITcpAgent *pSender, SOCKET soListen) {
    return HR_OK;
  }

  virtual EnHandleResult OnHandShake(ITcpAgent *pSender, CONNID dwConnID) {
    return HR_OK;
  }
  
  virtual EnHandleResult OnReceive(ITcpAgent *pSender, CONNID dwConnID,
                                   const BYTE *pBuffer, int iLength) {
    m_TotalPkts.fetch_add(1);
    return pSender->Send(dwConnID, pBuffer, iLength) ? HR_OK : HR_ERROR;
  }

  virtual EnHandleResult OnSend(ITcpAgent *pSender, CONNID dwConnID,
                                const BYTE *pData, int iLength) {
    return HR_OK;
  }

  virtual EnHandleResult OnClose(ITcpAgent *pSender, CONNID dwConnID,
                                 EnSocketOperation enOperation,
                                 int iErrorCode) {
    std::cout << "Client OnClose:" << dwConnID << std::endl;
    return HR_OK;
  }

  virtual EnHandleResult OnShutdown(ITcpAgent *pSender) { return HR_OK; }

public:
  BOOL StartClient() {
    // m_ClientPtr->SetWorkerThreadCount(32);
    // m_ClientPtr->SetMaxPackSize(0x3FFFFF);
    // m_ClientPtr->SetNoDelay(TRUE);
    // m_ClientPtr->SetSendPolicy(SP_DIRECT);

    return m_ClientPtr->Start(_T("0.0.0.0"), false);
  }

  VOID TestSendEcho(LPCTSTR host = _T("127.0.0.1"), USHORT port = 5555) {
    CONNID ID = 0;
    if (m_ClientPtr->Connect(host, port, &ID)) {
      m_ClientPtr->Send(ID, (BYTE *)"Hello World!", 9);
    }
  }
};