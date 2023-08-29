#pragma once

class KeyboardHook
{
public:
	KeyboardHook() {}
	virtual ~KeyboardHook() {}

	virtual void HookingKeyboard(int hookType) = 0;
	virtual void UnhookingKeyboard() = 0;
	virtual bool GetbIsCapture() = 0;
	virtual void SetbIsCapture(bool value) = 0;

};

inline const char* const BoolToString(bool b)
{
	return b ? "true" : "false";
}