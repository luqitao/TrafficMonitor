#pragma once
//  
//  NetworkRequest.cpp  
//  
//  18/5/12.  修改PostRequest方法： http为https SSL环境设置参考：http://www.jb51.net/article/119025.htm
//  
#include "stdafx.h"
#include "NetworkRequest.h"  
#include<tchar.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
//#include<openssl\ssl.h>
#pragma comment(lib,"ws2_32.lib")
//#pragma comment(lib,"libssl.lib")
//#pragma comment(lib,"libcrypto.lib")
CONST INT RECV_SIZE = 8192;

using namespace std;
//using namespace boost;
//using boost::asio::ip::tcp;

// POST请求  

/*
CString PostRequest(CString CShost, CString path, CString form)//PostHttpSSL  来自http://www.jb51.net/article/119025.htm
{
	//启动wsa
	WSADATA wsadData;
	WSAStartup(MAKEWORD(2, 2), &wsadData);

	//获取Host的IP地址等信息
	ADDRINFOT aiHints;
	ZeroMemory(&aiHints, sizeof(ADDRINFOT));
	aiHints.ai_family = AF_INET;
	aiHints.ai_flags = AI_PASSIVE;
	aiHints.ai_protocol = 0;
	aiHints.ai_socktype = SOCK_STREAM;
	// std::string wstrHost = CShost;
	PADDRINFOT paiResult;
	// GetAddrInfo(wstrHost.c_str(), NULL, &aiHints, &paiResult);
	GetAddrInfo(CShost, NULL, &aiHints, &paiResult);

	//创建套接字
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (sSocket == SOCKET_ERROR)
	{
		std::wcout << "Error socket" << std::endl;
		return "socket_error";
	}

	//连接Host
	SOCKADDR_IN sinHost;
	sinHost.sin_addr = ((LPSOCKADDR_IN)paiResult->ai_addr)->sin_addr;
	sinHost.sin_family = AF_INET;
	sinHost.sin_port = htons(443);
	if (connect(sSocket, (LPSOCKADDR)&sinHost, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		std::wcout << "Error connect" << std::endl;
		return "connect_error";
	}

	//初始化OpenSSL库
	//（虽然不知道为什么，但是不加这三行似乎并不会导致什么问题，在不加这3行的情况下测试了几个网站并没有发现任何问题喵）
	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();

	//创建SSL会话环境等
	SSL_CTX *pctxSSL = SSL_CTX_new(TLSv1_2_client_method());
	if (pctxSSL == NULL)
	{
		std::wcout << "Error SSL_CTX_new" << std::endl;
		return "Error SSL_CTX_new";
	}
	SSL *psslSSL = SSL_new(pctxSSL);
	if (psslSSL == NULL)
	{
		std::wcout << "Error SSL_new" << std::endl;
		return "Error SSL_new";
	}
	SSL_set_fd(psslSSL, sSocket);
	INT iErrorConnect = SSL_connect(psslSSL);
	if (iErrorConnect < 0)
	{
		std::wcout << "Error SSL_connect, iErrorConnect=" << iErrorConnect << std::endl;
		return "Error SSL_connect";
	}
	std::wcout << "SSL connection using " << SSL_get_cipher(psslSSL) << std::endl;

	//发包

	//char *host = CShost.GetBuffer(CShost.GetLength());
	//long length = form.GetLength();
	// string host = CShost.GetBuffer(0);
	// string str = form.GetBuffer(0);
	//int slength = str.length();
	// ostringstream slength;
	// slength << str.length();
	
	long length = form.GetLength();
	CString formLength;
	formLength.Format(_T("%d"), form.GetLength());
	// std::string strWrite = "POST "; 
	CString strWrite = "POST ";
	strWrite += path + " HTTP/1.1\r\n";
	strWrite += "Host: " + CShost + "\r\n";
	strWrite += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.71 Safari/537.36\r\n";
	//request_stream << "Accept: *\/*\r\n";
	strWrite += "Content-Type:application/json\r\n";
	strWrite += "Content-Length: " + formLength + "\r\n";
	strWrite += "Connection: close\r\n\r\n"; // 注意这里是两个空行  
	strWrite += form; //POST 发送的数据本身不包含多于空行   
	INT iErrorWrite = SSL_write(psslSSL, strWrite.GetBuffer(), strWrite.GetLength()) < 0;
	if (iErrorWrite < 0)
	{
		std::wcout << "Error SSL_write" << std::endl;
		return "Error SSL_write";
	}

	//收包并输出
	//这里接受的是char形式的，所以中文会乱码
	//如果要正常显示中文，需要再转换为wchar_t或std::wstring
	LPSTR lpszRead = new CHAR[RECV_SIZE];
	CString CstrRead;
	INT iLength = 1;
	while (iLength >= 1)
	{
		iLength = SSL_read(psslSSL, lpszRead, RECV_SIZE - 1);
		if (iLength < 0)
		{
			std::wcout << "Error SSL_read" << std::endl;
			delete[] lpszRead;
			return "Error SSL_read";
		}
		lpszRead[iLength] = TEXT('\0');
		//std::wcout << lpszRead;
		CstrRead+= lpszRead;
	}
	delete[] lpszRead;

	return CstrRead;
}
*/

CString GetRequest(CString sendmsg)
{
	HINTERNET internetopen;
	HINTERNET internetopenurl;
	BOOL bRet = TRUE;
	DWORD byteread = 0;
	char buffer[500] = {0};
	internetopen = InternetOpen(_T("Testing"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (internetopen == NULL) {
		//cout << "InternetOpen初始化失败!" << endl;
		return _T("Can't InternetOpen!");
	}
	internetopenurl = InternetOpenUrl(internetopen, sendmsg, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (internetopenurl == NULL) {
		//cout << "InternetOpenUrl打开Url失败!" << endl;
		InternetCloseHandle(internetopen);
		return _T("Can't InternetOpenUrl!");
	}
	CString buffs;
	do
	{
		ZeroMemory(buffer, sizeof(buffer));
		bRet = InternetReadFile(internetopenurl, buffer, sizeof(buffer), &byteread);
		for (DWORD dw = 0; dw < byteread; dw++)
		{
			buffs += buffer[dw];
		}
	} while (byteread);
	
	//while (1) {
	//	internetreadfile = InternetReadFile(internetopenurl, buffer, sizeof(char), &byteread);
	//	if (byteread == 0) {
	//		InternetCloseHandle(internetopenurl);
	//		break;
	//	}
	//	buffs += buffer[0];
	//}

	InternetCloseHandle(internetopenurl);
	if (bRet == FALSE)
	{		
		return _T("Can't InternetReadFile!");
	}
	if (buffs.GetLength() == 0)
	{
		buffs = _T("error");
	}
	return buffs;
}
