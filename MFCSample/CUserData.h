#pragma once
#include <string>

class CUserData
{
public:
	CUserData();
	~CUserData();

public:
	int nUserID;
	std::string strUserName;
	std::string strPassword;
	std::string strAdmin;

};

