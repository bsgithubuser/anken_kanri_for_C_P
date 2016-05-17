#include "stdafx.h"
#include "CProjectData.h"
/**
* @file CProjectData.cpp
* @brief �Č����N���X
* @date 2016/04/25
*/

CProjectData::CProjectData()
{
}


CProjectData::~CProjectData()
{
}

/**
* @fn
* ���ԏ��Q�Ə���
* @brief ���ԏ��Q�Ə���
* @return �Ȃ�
* @detail ���ԏ��Q�Ə���
*/
void CProjectData::GetPeriod(std::string& strPeriod)
{
	strPeriod.clear();

	if (strLongTerm == "t")
	{
		strPeriod = strPeriod + "���� ";
	}

	if (strSameDay == "t")
	{
		strPeriod = strPeriod + "���� ";
	}

	if (strAnyTime == "t")
	{
		strPeriod = strPeriod + "���� ";
	}
}

/**
* @fn
* �������Q�Ə���
* @brief �������Q�Ə���
* @return �Ȃ�
* @detail �������Q�Ə���
*/
void CProjectData::GetExtension(std::string& strExt)
{
	if (strExtention == "t")
	{
		strExt = "��";
	}
}

/**
* @fn
* ���t������芷������
* @brief ���t������芷������
* @return �Ȃ�
* @detail ���t������芷������
*/
void CProjectData::DateReplace()
{
	strReplace(strGenDay, "/", "-");
	strReplace(strPeriFrom, "/", "-");
	strReplace(strPeriTo, "/", "-");
}

/**
* @fn
* �����񒆂��當������������ĕʂ̕�����ɒu������
* @brief �����񒆂��當������������ĕʂ̕�����ɒu������
* @return �Ȃ�
* @detail �����񒆂��當������������ĕʂ̕�����ɒu������
*/
void CProjectData::strReplace(std::string& str, const std::string& from, const std::string& to) {
	std::string::size_type pos = 0;
	while (pos = str.find(from, pos), pos != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
}
