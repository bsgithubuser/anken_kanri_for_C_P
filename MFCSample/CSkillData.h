#pragma once
#include <string>

class CSkillData
{
public:
	CSkillData();
	~CSkillData();

public:
	int nSkillID;
	std::string strSkillName;
	bool bSetSkill = false;
};

