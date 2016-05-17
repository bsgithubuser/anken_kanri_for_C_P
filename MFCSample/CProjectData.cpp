#include "stdafx.h"
#include "CProjectData.h"
/**
* @file CProjectData.cpp
* @brief 案件情報クラス
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
* 期間情報参照処理
* @brief 期間情報参照処理
* @return なし
* @detail 期間情報参照処理
*/
void CProjectData::GetPeriod(std::string& strPeriod)
{
	strPeriod.clear();

	if (strLongTerm == "t")
	{
		strPeriod = strPeriod + "長期 ";
	}

	if (strSameDay == "t")
	{
		strPeriod = strPeriod + "即日 ";
	}

	if (strAnyTime == "t")
	{
		strPeriod = strPeriod + "随時 ";
	}
}

/**
* @fn
* 延長情報参照処理
* @brief 延長情報参照処理
* @return なし
* @detail 延長情報参照処理
*/
void CProjectData::GetExtension(std::string& strExt)
{
	if (strExtention == "t")
	{
		strExt = "○";
	}
}

/**
* @fn
* 日付文字取り換え処理
* @brief 日付文字取り換え処理
* @return なし
* @detail 日付文字取り換え処理
*/
void CProjectData::DateReplace()
{
	strReplace(strGenDay, "/", "-");
	strReplace(strPeriFrom, "/", "-");
	strReplace(strPeriTo, "/", "-");
}

/**
* @fn
* 文字列中から文字列を検索して別の文字列に置換する
* @brief 文字列中から文字列を検索して別の文字列に置換する
* @return なし
* @detail 文字列中から文字列を検索して別の文字列に置換する
*/
void CProjectData::strReplace(std::string& str, const std::string& from, const std::string& to) {
	std::string::size_type pos = 0;
	while (pos = str.find(from, pos), pos != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
}
