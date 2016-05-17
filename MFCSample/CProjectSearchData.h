#pragma once
#include "CSkillData.h"

#include <string>
#include <vector>

class CProjectSearchData
{
public:
	CProjectSearchData();
	~CProjectSearchData();

public:
	/* ������� */
	int nUserID;
	std::string strUserName;
	std::string strToDate;
	std::string strFromDate;
	std::string strCmpName;
	bool bUserSet = false;
	bool bAnkenNameSet = false;
	std::string strAnkenName;

	/* �ڍ׌������ */
	bool bDetailSearch = false;
	std::vector<CSkillData> vSkillData;
	std::string strPeriFrom;
	std::string strPeriTo;
	bool bValidDay = false;
	std::string strLongTerm;
	std::string strSameDay;
	std::string strAnyTime;
	std::string strExtention;

};

