#pragma once
#include <vector>

class KeyboardHook;

class BlockingCapture
{
public:
	BlockingCapture();
	virtual ~BlockingCapture();

	void Start_blocking_capture();
	void Stop_blocking_capture();
	void UnhookingKeyboard();
	
	std::vector<CString>& Get_control_process_list() { return m_ctrlProces; }

protected:
	static UINT	ThreadBlockingCapture(LPVOID _mothod);
	static BOOL	CALLBACK EnumProc(HWND hwnd, LPARAM lParam);
	CString		GetProcName(_In_ HWND hwnd);

private:
	void StartKeyboardHook();
	void SearchControlWindow();

private:
	std::vector<CString>	m_ctrlProces;
	KeyboardHook*			m_keyboardHookMgr;
	HMODULE					m_hDll_keyboardHook;
};

