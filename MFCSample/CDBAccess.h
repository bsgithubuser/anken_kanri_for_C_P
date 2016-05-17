#pragma once
#include "CUserData.h"
#include "CProjectSearchData.h"
#include "CProjectData.h"
#include "CCmpData.h"
#include "CSkillData.h"
#include "CCompanyData.h"

/* Postgres�֘A�w�b�_ */
#include <libpq-fe.h>
#include <postgres_ext.h>

/* STL */
#include <string>
#include <locale> 
#include <codecvt> 
#include <cstdio>
#include <vector>

/* �߂�l */
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
	/* �}�X�^�Ǘ��n                       */
	/**************************************/
	// �S���ҊǗ��}�X�^�擾����
	int DBGetSkillData(std::vector<CSkillData>& vSkillData);				// �X�L���Ǘ��}�X�^�擾����
	int DBGetCompanyData(std::vector<CCompanyData>& vCompanyData);			// ��ЊǗ��}�X�^�擾����
	int DBSetUserData(CString, CString, CString, CString, int);							// �S���ҊǗ��}�X�^�o�^�E�X�V����
	int DBSetSkillData(CString, CString, int);							// �X�L���Ǘ��}�X�^�o�^�E�X�V����
	int DBSetCmpnData(CString, CString, CString, int);							// ��ЊǗ��}�X�^�o�^�E�X�V����
	int DBGetSeqquence(std::string strDBName, int& nSequence);
	int DBSetMstDataUpdate(std::string strSql, std::string strSql2);		// �}�X�^�o�^�E�X�V��������

	std::wstring ToWideStr(const std::string &strMult);
	std::string ToMultStr(const std::wstring &strWide);
};

