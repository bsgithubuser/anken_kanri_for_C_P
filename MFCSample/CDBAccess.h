#pragma once
#include "CUserData.h"
#include "CProjectSearchData.h"
#include "CProjectData.h"
#include "CCmpData.h"
#include "CSkillData.h"
#include "CCompanyData.h"

/* Postgres関連ヘッダ */
#include <libpq-fe.h>
#include <postgres_ext.h>

/* STL */
#include <string>
#include <locale> 
#include <codecvt> 
#include <cstdio>
#include <vector>

/* 戻り値 */
#define DB_OK 0
#define DB_NG 1
#define DB_NOTUSERDATA 2

class CDBAccess
{
private:
	CDBAccess(void);
	~CDBAccess();
	void DBConnection(PGconn** pConn);

public:
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> m_Convert;
	int m_nLoginUserID;
	std::string m_strLoginUserName;
	std::string m_strAdmin;
	std::string m_strDBErrorMessage;

public:
	static CDBAccess &getInstance();
	int DBLoginChk(std::string& strUserName, std::string& strPassword);
	void StrConv(std::string& strConvString, std::wstring& strWString);
	void WStrConv(std::wstring& strConvWString, std::string& strString);
	int DBGetUserData(std::vector<CUserData>& vUserData);
	int DBGetUserList(std::vector<CUserData>& vUserData);

	int DBGetProjectData(CProjectSearchData& cProjectSearch, std::vector<CProjectData>& vProjectData);
	int DBGetProjectDetail(CProjectData& cProjectData, CString& strProjectID);
	int DBGetSkillList(CProjectData& cProjectData);
	int DBGetCmpData(std::vector<CCmpData>& vCmpData);
	int DBGetSkillMasta(std::vector<CSkillData>& vSkillData);
	int DBSetNewProject(CProjectData& cProjectData);
	int DBUpdateProject(CProjectData& cProjectData);
	int DBDeleteProject(CProjectData& cProjectData);
	std::wstring OutErrorMsg();
	
	/**************************************/
	/* マスタ管理系                       */
	/**************************************/
	// 担当者管理マスタ取得処理
	int DBGetSkillData(std::vector<CSkillData>& vSkillData);				// スキル管理マスタ取得処理
	int DBGetCompanyData(std::vector<CCompanyData>& vCompanyData);			// 会社管理マスタ取得処理
	int DBSetUserData(CString, CString, CString, CString, int);							// 担当者管理マスタ登録・更新処理
	int DBSetSkillData(CString, CString, int);							// スキル管理マスタ登録・更新処理
	int DBSetCmpnData(CString, CString, CString, int);							// 会社管理マスタ登録・更新処理
	int DBGetSeqquence(std::string strDBName, int& nSequence);
	int DBSetMstDataUpdate(std::string strSql, std::string strSql2);		// マスタ登録・更新処理共通

	std::wstring ToWideStr(const std::string &strMult);
	std::string ToMultStr(const std::wstring &strWide);
};

