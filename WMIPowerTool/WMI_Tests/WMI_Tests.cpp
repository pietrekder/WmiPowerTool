// WMI_Tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
//Getting WMI Data from local computer

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hResult;

	// 1 Initialize COM
	hResult = CoInitializeEx( NULL, COINIT_MULTITHREADED );
	if( FAILED(hResult) )
	{
		cout<< "Failed to initialize COM. Error code = 0x"<< hex << hResult << endl;
		return 1;
	}

	// 2 Initialize Security
	hResult = CoInitializeSecurity( NULL,
					                -1,
									NULL,
									NULL,
									RPC_C_AUTHN_LEVEL_DEFAULT,
									RPC_C_IMP_LEVEL_IMPERSONATE,
									NULL,
									EOAC_NONE,
									NULL
									);

	if( FAILED(hResult) )
	{
		cout<< "Failed to initialize security. Error code = 0x"<< hex << hResult << endl;
		CoUninitialize();
		return 1;
	}

	// 3 WBem Locator
	IWbemLocator *pLocator = 0;
	hResult = CoCreateInstance( CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLocator );

	if( FAILED(hResult) )
	{
		cout<< "Failed to create Wbem Locator. Error code = 0x"<< hex << hResult << endl;
		CoUninitialize();
		return 1;
	}

	// 4 Services
	IWbemServices *pService = 0;
	hResult = pLocator->ConnectServer( _bstr_t(L"root\\cimv2"),
		                               NULL,
									   NULL,
									   0,
									   NULL,
									   0,
									   0,
									   &pService
									   );

	if( FAILED(hResult) )
	{
		cout<< "Could not connect. Error code = 0x"<< hex << hResult << endl;
		pLocator->Release();	//Release instance of COM object
		CoUninitialize();
		return 1;
	}

	cout<< "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	// 5 
	hResult = CoSetProxyBlanket( pService,
		                         RPC_C_AUTHN_WINNT,
								 RPC_C_AUTHZ_NONE,
								 NULL,
								 RPC_C_AUTHN_LEVEL_CALL,
								 RPC_C_IMP_LEVEL_IMPERSONATE,
								 NULL,
								 EOAC_NONE
								 );

	if( FAILED(hResult) )
	{
		cout<< "Could not set proxy blanket. Error code = 0x"<< hex << hResult << endl;
		pService->Release();
		pLocator->Release();	//Release instance of COM object
		CoUninitialize();
		return 1;
	}


	// 6 
	IEnumWbemClassObject *pEnumerator = 0;
	hResult = pService->ExecQuery( _bstr_t("WQL"),
		                           _bstr_t("SELECT * FROM Win32_OperatingSystem"),
								   WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
								   NULL,
								   &pEnumerator
								   );

	if( FAILED(hResult) )
	{
		cout<< "Query for operating system failed. Error code = 0x"<< hex << hResult << endl;
		pService->Release();
		pLocator->Release();	//Release instance of COM object
		CoUninitialize();
		return 1;
	}

	// 7 
	IWbemClassObject *pClsObj = NULL;
	ULONG uRetVal = 0;

	while( pEnumerator )
	{
		HRESULT hr = pEnumerator->Next( WBEM_INFINITE, 1, &pClsObj, &uRetVal );

		if( 0 == uRetVal )
			break;

		VARIANT vtProp;

		hr = pClsObj->Get( L"Name", 0, &vtProp, 0, 0 );
		wcout << "OS Name: " << vtProp.bstrVal << endl;
		VariantClear( &vtProp );

		pClsObj->Release();
	}

	pService->Release();
	pLocator->Release();
	pEnumerator->Release();
	CoUninitialize();

	return 0;
}

