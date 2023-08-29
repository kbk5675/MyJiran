#pragma once

//검사 유형 선택(이메일, 전화, 주민번호, 계좌)
enum class ESENSITIVEINFO
{
	STINFO_RRN = 0,
	STINFO_EMAIL = 1,
	STINFO_PHONE = 2,
};