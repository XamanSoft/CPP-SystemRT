#include <CppSystemRT.hpp>

namespace CppSystemRT {

namespace Daemon {
	
/// structure contains status information for a service
static SERVICE_STATUS        g_ServiceStatus = { 0 };
/// handle to the status information structure for the current service.
static SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
/// handle for a stop event
static HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

thread_create_t threadCreate{nullptr};

#define SERVICE_NAME  _T("LoggerWindowsService")  

static VOID WINAPI ServiceDaemonMain(DWORD argc, LPTSTR *argv);
static VOID WINAPI ServiceDaemonCtrlHandler(DWORD);
static DWORD WINAPI ServiceDaemonWorkerThread(LPVOID lpParam);

int exec(thread_create_t thread_create) {
	SERVICE_TABLE_ENTRY ServiceDaemonTable[] =
	{
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceDaemonMain },
		{ NULL, NULL }
	};
	
	threadCreate = thread_create;

	if (StartServiceCtrlDispatcher(ServiceDaemonTable) == FALSE)
	{
		return GetLastError();
	}

	return 0;
}

/// The entry point for a service.
VOID WINAPI ServiceDaemonMain(DWORD argc, LPTSTR *argv)
{
	DWORD Status = E_FAIL;
	/// Registers a function to handle service control requests.
	g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceDaemonCtrlHandler);

	if (g_StatusHandle == NULL) {
		return;
	}

	/// Informing the smc that the service is starting
	ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	/// Creating the stop event
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	/// if SetServiceStatus returned error
	if (g_ServiceStopEvent == NULL)
	{
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = GetLastError();
		g_ServiceStatus.dwCheckPoint = 1;
		SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
		return;
	}

	/// Informing the smc that the service started
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

	/// Starting the  worker thread
	HANDLE hThread = CreateThread(NULL, 0, ServiceDaemonWorkerThread, (LPVOID)threadCreate, 0, NULL);

	// Waiting for worker thread exit signal
	WaitForSingleObject(hThread, INFINITE);

	/// Cleanup
	CloseHandle(g_ServiceStopEvent);

	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 3;
	SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

}

/// Controller for the service, managing the stop operation
VOID WINAPI ServiceDaemonCtrlHandler(DWORD CtrlCode)
{
	switch (CtrlCode)
	{
		case SERVICE_CONTROL_STOP:
		{
			if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
				break;

			/// Stopping the service
			g_ServiceStatus.dwControlsAccepted = 0;
			g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			g_ServiceStatus.dwWin32ExitCode = 0;
			g_ServiceStatus.dwCheckPoint = 4;
			SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

			/// Signalling the worker thread to shut down
			SetEvent(g_ServiceStopEvent);

			break;
		}

		default:
		{
			break;
		}
	}

}

/// ServiceWorkerThread function starts writing the timestamp to a file every 3 seconds
DWORD WINAPI ServiceDaemonWorkerThread(LPVOID lpParam)
{
	std::unique_ptr<Thread> worker(static_cast<thread_create_t>(lpParam)());

	///  Checking if the services has issued a stop request
	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{
		if (!worker || worker->exec())
			break;

		Sleep(1000);
	}

	return ERROR_SUCCESS;
}

}

}
