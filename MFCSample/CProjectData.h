#pragma once
#include "CSkillData.h"
#include <string>
#include <vector>

class CProjectData
{
public:
	CProjectData();
	~CProjectData();

public:
	int nProjectId;
	std::string strProjectName;
	std::string strGenDay;
	std::string strLongTerm;
	std::string strSameDay;
	std::string strAnyTime;
	std::string strExtention;
	std::string strSkillName;
	std::string strCmpName;
	std::string strUserName;

	int nUserID;
	int nCmpID;
	int nPrjSkillID;
	std::string strPeriFrom;
	std::string strPeriTo;
	std::string strOrverView;
	std::string strOther;

	std::vector<std::string> vSkillList;
	std::vector<CSkillData> vSkillData;
	bool bSkillUpdateFlg = false;

	void GetPeriod(std::string& strPeriod);
	void GetExtension(std::string& strExt);
	void DateReplace();
	void strReplace(std::string& str, const std::string& from, const std::string& to);
};

