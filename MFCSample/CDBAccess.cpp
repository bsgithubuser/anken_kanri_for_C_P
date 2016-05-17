/**
* @file CDBAccess.cpp
* @brief DB���䋤��
* @date 2016/04/25
*/
#include "stdafx.h"
#include "afxwin.h"
#include "CDBAccess.h"
#include "MFCSample.h"

CDBAccess::CDBAccess()
{
}

CDBAccess::~CDBAccess()
{
}

CDBAccess &CDBAccess::getInstance()
{
	static CDBAccess instance;
	return instance;
}

void CDBAccess::StrConv(std::string& strConvString, std::wstring& strWString)
{
	//wstring��string
	//strConvString = m_Convert.to_bytes(strWString);

	char *mbs = new char[strWString.length() * MB_CUR_MAX + 1];

	wcstombs(mbs, strWString.c_str(), strWString.length() * MB_CUR_MAX + 1);

	strConvString = mbs;

	delete[] mbs;
}

void CDBAccess::WStrConv(std::wstring& strConvWString, std::string& strString)
{
	//string��wstring
	//strConvWString = m_Convert.from_bytes(strString);

	// SJIS �� wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, strString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// �o�b�t�@�̎擾
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS �� wstring
	MultiByteToWideChar(CP_ACP, 0, strString.c_str(), -1, cpUCS2
		, iBufferSize);

	// string�̐���
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpUCS2;

	strConvWString = oRet;

	// �ϊ����ʂ�Ԃ�
	return;
}

std::wstring CDBAccess::OutErrorMsg()
{
	std::wstring strErrorMsg;

	WStrConv(strErrorMsg, m_strDBErrorMessage);

	return strErrorMsg;
}

/************************************************************/
/* �ڑ����ݒ菈��											*/
/************************************************************/
void CDBAccess::DBConnection(PGconn** pConn)
{
	char      conninfo[256];

	sprintf(conninfo, "host=%s port=%s dbname=%s user=%s password=%s"
//		, "localhost"         //�z�X�g��
		, "192.168.251.34"    //�z�X�g��(���L�T�[�o)
		, "5432"              //�|�[�g 
		, "db_ANKEN_KANRI"    //�f�[�^�x�[�X��
		, "postgres"          //�ڑ����[�U��
		, "postgres");        //�p�X���[�h

	*pConn = PQconnectdb(conninfo);

	return ;
}

/************************************************************/
/* ���O�C�����菈��											*/
/************************************************************/
int CDBAccess::DBLoginChk(std::string& strUserName, std::string& strPassword)
{

	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;

	/* DB�ڑ� */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}

	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}
	/* �g�����U�N�V�����u���b�N���J�n����B */
	//res = PQexec(conn, "BEGIN");
	//if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	//	PQerrorMessage(conn);
	//	PQclear(res);
	//	PQfinish(conn);
	//	return 0;
	//}

	/* �s�v�ɂȂ�����A���������[�N��h�����߂�PGresult��PQclear���ׂ��B */
	//PQclear(res);

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
			"\"USER_NAME\","
			"\"USER_ID\","
			"\"PASSWARD\","
			"\"ADMIN\" "
		"FROM "
			"\"M_USERS\" "
		"WHERE "
			"\"USER_NAME\" = '" + strUserName + "' AND "
			"\"PASSWARD\" = '" + strPassword + "'";

	/* �e�[�u������s�����o�� */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	//PQclear(res);
	//res = PQexec(conn, "FETCH ALL in myportal");
	//if (PQresultStatus(res) != PGRES_TUPLES_OK) {
	//	PQerrorMessage(conn);
	//	PQclear(res);
	//	PQfinish(conn);
	//	return 0;
	//}

	///* select�����t�B�[���h�� */
	//nFields = PQnfields(res);
	///* �����čs��\������B */
	//for (i = 0; i < PQntuples(res); i++)
	//{
	//	for (j = 0; j < nFields; j++)
	//	{
	//		//PQgetvalue(res, i, j);
	//		for (j = 0; j < nFields; j++)
	//		{
	//			strLog.assign(PQgetvalue(res, i, j));
	//			strLogW = cv.from_bytes(strLog);
	//			//MessageBox(strLogW);
	//		}
	//	}
	//}

	/* ���ʍs�����Q�� */
	int nCnt = PQntuples(res);

	switch (nCnt)
	{
		/* ���ʂȂ� */
		case 0:
			return DB_NOTUSERDATA;
			break;

		/* ���ʂ��� */
		default:
			m_strLoginUserName = PQgetvalue(res, 0, 0);
			m_nLoginUserID = atol(PQgetvalue(res, 0, 1));
			m_strAdmin = PQgetvalue(res, 0, 3);

			break;
	}

	PQclear(res);

	/* �|�[�^��������B */
	//res = PQexec(conn, "CLOSE myportal");
	//if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	//	PQerrorMessage(conn);
	//	PQclear(res);
	//	PQfinish(conn);
	//	return 0;
	//}
	//PQclear(res);

	/* �f�[�^��ǉ�����ɂ͈ȉ������s����΂悢�B */
	//res = PQexec(conn, "insert into aaa values ('����������','����������')");
	//if (PQresultStatus(res) != PGRES_COMMAND_OK){
	//    MessageBox(PQerrorMessage(conn));
	//    PQclear(res);
	//    PQfinish(conn);
	//    return ;
	//}

	/* �g�����U�N�V�������I������ */
	//res = PQexec(conn, "END");
	//PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* ���[�U���Q�Ə���(�}�X�^)								*/
/************************************************************/
int CDBAccess::DBGetUserList(std::vector<CUserData>& vUserData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CUserData cTempUserData;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1)
	{
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
			"\"USER_ID\","
			"\"USER_NAME\","
			"\"PASSWARD\" ,"
			"\"ADMIN\" "
		"FROM "
			"\"M_USERS\" "
		"ORDER BY \"USER_ID\" DESC ";

	/* �e�[�u������s�����o�� */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		cTempUserData.nUserID = atol(PQgetvalue(res, i, 0));
		cTempUserData.strUserName = PQgetvalue(res, i, 1);
		cTempUserData.strPassword = PQgetvalue(res, i, 2);
		cTempUserData.strAdmin = PQgetvalue(res, i, 3);

		vUserData.push_back(cTempUserData);
	}

	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* ���[�U���Q�Ə���										*/
/************************************************************/
int CDBAccess::DBGetUserData(std::vector<CUserData>& vUserData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CUserData cTempUserData;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
			"\"USER_ID\","
			"\"USER_NAME\","
			"\"PASSWARD\" ,"
			"\"ADMIN\" "
		"FROM "
			"\"M_USERS\" "
		"ORDER BY "
			"\"USER_ID\" ASC ";

	/* �e�[�u������s�����o�� */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
			cTempUserData.nUserID = atol(PQgetvalue(res, i, 0));
			cTempUserData.strUserName = PQgetvalue(res, i, 1);
			cTempUserData.strPassword = PQgetvalue(res, i, 2);
			cTempUserData.strAdmin = PQgetvalue(res, i, 3);

			vUserData.push_back(cTempUserData);
	}

	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* �Č���񌟍�����											*/
/************************************************************/
int CDBAccess::DBGetProjectData(CProjectSearchData& cProjectSearch, std::vector<CProjectData>& vProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	std::string strWhere;
	CProjectData cTempProjectData;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1)
	{
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
			"\"PRJ_ID\", "
			"\"PRJ_NAME\","
			"\"GEN_DATE\","
			"\"LONG_TERM_FLG\","
			"\"SAME_DAY_FLG\","
			"\"ANY_TIME_FLG\","
			"\"EXTENTION_FLG\","
			"\"T_PROJ_INFO\".\"PRJ_SKL_ID\","
			"\"CMPN_NAME\","
			"\"USER_NAME\","
			"\"PERI_FROM\" "
		"FROM "
			"\"M_CMPN\","
			"\"M_USERS\","
			"\"T_PROJ_INFO\" "
		"WHERE "
			"\"T_PROJ_INFO\".\"CMPN_ID\" = \"M_CMPN\".\"CMPN_ID\" AND "
			"\"T_PROJ_INFO\".\"USER_ID\" = \"M_USERS\".\"USER_ID\" ";

	/* ����������ݒ� */

	/* �Č����w�肩 */
	if (cProjectSearch.bAnkenNameSet == true)
	{
		/* �Č����𕔕����� */
		strWhere += "AND \"T_PROJ_INFO\".\"PRJ_NAME\" LIKE '%" + cProjectSearch.strAnkenName + "%' ";
	}

	/* �S���Ҏw�肩*/
	if (cProjectSearch.bUserSet == true)
	{
		strWhere +=
			"AND "
			"\"T_PROJ_INFO\".\"USER_ID\" = " +
			std::to_string(cProjectSearch.nUserID) + " ";
	}

	strWhere += 
		"AND "
			"\"T_PROJ_INFO\".\"GEN_DATE\" BETWEEN "
				"'" + cProjectSearch.strToDate + "' AND "
				"'" + cProjectSearch.strFromDate + "' ";

	/* ��Ж��̌���*/
	if(cProjectSearch.strCmpName != "")
	{
		/* ��Ж��𕔕����� */
		strWhere += "AND \"M_CMPN\".\"CMPN_NAME\" LIKE '%"+ cProjectSearch.strCmpName + "%' ";
	}

	/* �ڍ׌��� */
	if (cProjectSearch.bDetailSearch == true)
	{
		/* ���Ԏw�肠�肩 */
		if (cProjectSearch.bValidDay == true)
		{
			strWhere +=
				"AND "
				"\"T_PROJ_INFO\".\"PERI_FROM\" BETWEEN "
				"'" + cProjectSearch.strPeriFrom + "' AND "
				"'" + cProjectSearch.strPeriTo + "' ";
		}

		/* �t���O�����ݒ� */
		if (cProjectSearch.strExtention == "t")
		{
			strWhere += "AND \"T_PROJ_INFO\".\"EXTENTION_FLG\" = 't' ";
		}
		if (cProjectSearch.strAnyTime == "t")
		{
			strWhere += "AND \"T_PROJ_INFO\".\"ANY_TIME_FLG\" = 't' ";
		}
		if (cProjectSearch.strSameDay == "t")
		{
			strWhere += "AND \"T_PROJ_INFO\".\"SAME_DAY_FLG\" = 't' ";
		}
		if (cProjectSearch.strLongTerm == "t")
		{
			strWhere += "AND \"T_PROJ_INFO\".\"LONG_TERM_FLG\" = 't' ";
		}
		
		//strWhere += "AND \"T_PROJ_INFO\".\"LONG_TERM_FLG\" = '" + cProjectSearch.strLongTerm + "' ";

		// �X�L���������������邩
		if(cProjectSearch.vSkillData.size() != 0)
		{ 
			bool bFirst = true;

			strWhere +=
				" AND \"T_PROJ_INFO\".\"PRJ_SKL_ID\" in "
				"(SELECT \"T_PROJ_SKILL\".\"PRJ_SKL_ID\" FROM \"T_PROJ_SKILL\" "
				" where ";

			/* �����X�L���ݒ� */
			for (std::vector<CSkillData>::iterator it = cProjectSearch.vSkillData.begin();
				it != cProjectSearch.vSkillData.end();
				it++)
			{
				if (it->bSetSkill == true)
				{
					if (bFirst == true)
					{
						strWhere += "\"T_PROJ_SKILL\".\"SKL_ID\" = " + std::to_string(it->nSkillID) + " ";
						bFirst = false;
					}
					else
					{
						strWhere += "OR \"T_PROJ_SKILL\".\"SKL_ID\" = " + std::to_string(it->nSkillID) + " ";
					}

				}
			}
			strWhere += ") ";
		}
	}

	strWhere = strWhere + "ORDER BY \"PRJ_ID\" ";

	/* ����������ǉ� */
	strSQL = strSQL + strWhere;

	/* �e�[�u������s�����o�� */
	res = PQexec(conn, strSQL.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);

		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		cTempProjectData.nProjectId = atol(PQgetvalue(res, i, 0));
		cTempProjectData.strProjectName = PQgetvalue(res, i, 1);
		cTempProjectData.strGenDay = PQgetvalue(res, i, 2);
		cTempProjectData.strLongTerm = PQgetvalue(res, i, 3);
		cTempProjectData.strSameDay = PQgetvalue(res, i, 4);
		cTempProjectData.strAnyTime = PQgetvalue(res, i, 5);
		cTempProjectData.strExtention = PQgetvalue(res, i, 6);
		cTempProjectData.nPrjSkillID = atol(PQgetvalue(res, i, 7));
		cTempProjectData.strCmpName = PQgetvalue(res, i, 8);
		cTempProjectData.strUserName = PQgetvalue(res, i, 9);
		cTempProjectData.strPeriFrom = PQgetvalue(res, i, 10);

		vProjectData.push_back(cTempProjectData);
	}
	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* �Č��ڍ׏�񌟍�����										*/
/************************************************************/
int CDBAccess::DBGetProjectDetail(CProjectData& cProjectData,CString& strProjectID)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;

	std::wstring strwProjectID((LPCTSTR)strProjectID);
	std::string strSQLID;

	StrConv(strSQLID, strwProjectID);
	
	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);

		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1)
	{
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);

		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
			"\"PRJ_ID\", "
			"\"PRJ_NAME\", "
			"\"USER_ID\", "
			"\"CMPN_ID\", "
			"\"PRJ_SKL_ID\", "
			"\"GEN_DATE\", "
			"\"PERI_FROM\", "
			"\"PERI_TO\", "
			"\"LONG_TERM_FLG\", "
			"\"SAME_DAY_FLG\", "
			"\"ANY_TIME_FLG\", "
			"\"EXTENTION_FLG\", "
			"\"ORVERVIEW\", "
			"\"OTHER\" "
		"FROM "
			"\"T_PROJ_INFO\" "
		"WHERE "
			"\"PRJ_ID\" = " + strSQLID + " ";

	/* �e�[�u������s�����o�� */
	res = PQexec(conn, strSQL.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);

		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		cProjectData.nProjectId = atol(PQgetvalue(res, i, 0));
		cProjectData.strProjectName = PQgetvalue(res, i, 1);
		cProjectData.nUserID = atol(PQgetvalue(res, i, 2));
		cProjectData.nCmpID = atol(PQgetvalue(res, i, 3));
		cProjectData.nPrjSkillID = atol(PQgetvalue(res, i, 4));
		cProjectData.strGenDay = PQgetvalue(res, i, 5);
		cProjectData.strPeriFrom = PQgetvalue(res, i, 6);
		cProjectData.strPeriTo = PQgetvalue(res, i, 7);
		cProjectData.strLongTerm = PQgetvalue(res, i, 8);
		cProjectData.strSameDay = PQgetvalue(res, i, 9);
		cProjectData.strAnyTime = PQgetvalue(res, i, 10);
		cProjectData.strExtention = PQgetvalue(res, i, 11);
		cProjectData.strOrverView = PQgetvalue(res, i, 12);
		cProjectData.strOther= PQgetvalue(res, i, 13);
	}

	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* �X�L���ꗗ��񌟍�����									*/
/************************************************************/
int CDBAccess::DBGetSkillList(CProjectData& cProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;

	std::wstring strwWork;
	std::string strPorjectID;
	std::string strSkillName;
	CString strTemp;
	
	strTemp.Format(_T("%d"), cProjectData.nPrjSkillID);
	strwWork = strTemp.GetBuffer();
	StrConv(strPorjectID, strwWork);

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
			"\"SKL_ID\", "
			"\"SKILL_NAME\" "
		"FROM "
			"\"T_PROJ_SKILL\", "
			"\"M_SKILL\" "
		"WHERE "
			"\"SKL_ID\" = \"M_SKILL\".\"SKILL_ID\" AND "
			"\"PRJ_SKL_ID\" = " + strPorjectID + " ";

	/* �e�[�u������s�����o�� */
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		strSkillName = PQgetvalue(res, i, 1);
		cProjectData.vSkillList.push_back(strSkillName);
	}

	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* ��Џ��Q�Ə���											*/
/************************************************************/
int CDBAccess::DBGetCmpData(std::vector<CCmpData>& vCmpData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CCmpData cCmpData;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("�G���R�[�h�w��Ɏ��s���܂���");
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL =
	"SELECT "
		"\"CMPN_ID\","
		"\"CMPN_NAME\","
		"\"CMPN_NAME_FURI\" "
	"FROM "
		"\"M_CMPN\" "
	"ORDER BY "
		"\"CMPN_ID\" ASC ";

	/* �e�[�u������s�����o�� */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		cCmpData.nCmpID = atol(PQgetvalue(res, i, 0));
		cCmpData.strCmpName = PQgetvalue(res, i, 1);
		cCmpData.strCmpKana = PQgetvalue(res, i, 2);

		vCmpData.push_back(cCmpData);
	}

	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* �X�L���}�X�^���Q�Ə���									*/
/************************************************************/
int CDBAccess::DBGetSkillMasta(std::vector<CSkillData>& vSkillData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CSkillData cSkillData;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) 
	{
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
			"\"SKILL_ID\","
			"\"SKILL_NAME\" "
		"FROM "
			"\"M_SKILL\" ";

	/* �e�[�u������s�����o�� */
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		cSkillData.nSkillID = atol(PQgetvalue(res, i, 0));
		cSkillData.strSkillName = PQgetvalue(res, i, 1);

		vSkillData.push_back(cSkillData);
	}

	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* �Č����V�K�o�^����										*/
/************************************************************/
int CDBAccess::DBSetNewProject(CProjectData& cProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;
	std::string strNewSkillID;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("�G���R�[�h�w��Ɏ��s���܂���");
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQL��ݒ�*/
	strSQL = "select nextval('prj_skill_id_seq')";

	/* �e�[�u������s�����o�� */
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	/* �ŐV�̃X�L��ID���Q�� */
	strNewSkillID = PQgetvalue(res, 0, 0);

	PQclear(res);

	/* �Č��X�L���e�[�u���̍X�V */
	for(std::vector<CSkillData>::iterator it = cProjectData.vSkillData.begin();
		it != cProjectData.vSkillData.end();
		it++)
	{
		if(it->bSetSkill == true)
		{
			/* �X�V�R�}���h���s */
			strSQL =
				"INSERT INTO \"T_PROJ_SKILL\"("
				"\"PRJ_SKL_ID\", \"SKL_ID\", \"OTHER\", \"CREATE_DATE\", \"UPDATE_DATE\") "
				"VALUES("
				+ strNewSkillID + "," +
				std::to_string(it->nSkillID) + "," +
				"' '," +
				"now(),now()) ";

			/* �X�V�������s */
			res = PQexec(conn, strSQL.c_str());
			if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK) 
			{
				m_strDBErrorMessage = PQerrorMessage(conn);
				PQclear(res);
				PQfinish(conn);
				return DB_NG;
			}
			PQclear(res);
		}
	}

	/* �Č����o�^ */

	/* �X�V�R�}���h���s */
	strSQL =
	"INSERT INTO \"T_PROJ_INFO\"("
		"\"PRJ_ID\", \"PRJ_NAME\", \"USER_ID\", \"CMPN_ID\", \"PRJ_SKL_ID\", \"GEN_DATE\","
		"\"PERI_FROM\", \"PERI_TO\", \"LONG_TERM_FLG\", \"SAME_DAY_FLG\", \"ANY_TIME_FLG\","
		"\"EXTENTION_FLG\", \"ORVERVIEW\", \"OTHER\", \"DEL_FLG\", \"CREATE_DATE\",\"UPDATE_DATE\") "
	"VALUES("
		"nextval('prj_id_seq'),"
		"'" + cProjectData.strProjectName + "'," + 
		std::to_string(cProjectData.nUserID) + "," +
		std::to_string(cProjectData.nCmpID) + "," +
		strNewSkillID + "," +
		"'" + cProjectData.strGenDay + "'," +
		"'" + cProjectData.strPeriFrom + "'," +
		"'" + cProjectData.strPeriTo + "'," +
		"'" + cProjectData.strLongTerm + "'," +
		"'" + cProjectData.strSameDay + "'," +
		"'" + cProjectData.strAnyTime + "'," +
		"'" + cProjectData.strExtention + "'," +
		"'" + cProjectData.strOrverView + "'," +
		"'" + cProjectData.strOther + "'," +
		"'f'," +
		"now(),now()) ";

	/* �X�V�������s */
	res = PQexec(conn, strSQL.c_str());
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}
	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* �Č����X�V����											*/
/************************************************************/
int CDBAccess::DBUpdateProject(CProjectData& cProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;
	std::string strNewSkillID;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) 
	{
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}
	/* �X�L���X�V���s������ */
	if (cProjectData.bSkillUpdateFlg == true)
	{
		/* �X�V�O�̈Č��X�L�������폜 */
		strSQL =
			"DELETE FROM \"T_PROJ_SKILL\" "
			"WHERE "
				"\"PRJ_SKL_ID\" = " + std::to_string(cProjectData.nPrjSkillID) + " ";

		/*�폜�������s */
		res = PQexec(conn, strSQL.c_str());
		if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			m_strDBErrorMessage = PQerrorMessage(conn);
			PQclear(res);
			PQfinish(conn);
			return DB_NG;
		}
		PQclear(res);

		/* �V�����X�L�������쐬 */
		for (std::vector<CSkillData>::iterator it = cProjectData.vSkillData.begin();
			it != cProjectData.vSkillData.end();
			it++)
		{
			if (it->bSetSkill == true)
			{
				/* �X�V�R�}���h���s */
				strSQL =
					"INSERT INTO \"T_PROJ_SKILL\"("
					"\"PRJ_SKL_ID\", \"SKL_ID\", \"OTHER\", \"CREATE_DATE\", \"UPDATE_DATE\") "
					"VALUES("
					+ std::to_string(cProjectData.nPrjSkillID) + "," +
					std::to_string(it->nSkillID) + "," +
					"' '," +
					"now(),now()) ";

				/* �X�V�������s */
				res = PQexec(conn, strSQL.c_str());
				if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
				{
					m_strDBErrorMessage = PQerrorMessage(conn);
					PQclear(res);
					PQfinish(conn);
					return DB_NG;
				}
				PQclear(res);
			}
		}
	}

	/* �X�V�R�}���h���s */
	strSQL =
		"UPDATE \"T_PROJ_INFO\" SET "
			"\"PRJ_NAME\" = '" + cProjectData.strProjectName + "', "
			"\"USER_ID\" = " + std::to_string(cProjectData.nUserID) + ", "
			"\"CMPN_ID\" = " + std::to_string(cProjectData.nCmpID) + ", "
			"\"GEN_DATE\" = '" + cProjectData.strGenDay + "',"
			"\"PERI_FROM\" = '" + cProjectData.strPeriFrom + "',"
			"\"PERI_TO\" = '" + cProjectData.strPeriTo + "',"
			"\"LONG_TERM_FLG\" = '" + cProjectData.strLongTerm + "',"
			"\"SAME_DAY_FLG\" = '" + cProjectData.strSameDay + "',"
			"\"ANY_TIME_FLG\" = '" + cProjectData.strAnyTime + "',"
			"\"EXTENTION_FLG\" = '" + cProjectData.strExtention + "',"
			"\"ORVERVIEW\" = '" + cProjectData.strOrverView + "', "
			"\"OTHER\" = '" + cProjectData.strOther + "', "
			"\"UPDATE_DATE\" = now() "
		"WHERE "
			"\"PRJ_ID\" = " + std::to_string(cProjectData.nProjectId) + " ";

	/* �X�V�������s */
	res = PQexec(conn, strSQL.c_str());
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}
	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* �Č����폜����											*/
/************************************************************/
int CDBAccess::DBDeleteProject(CProjectData& cProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;
	std::string strNewSkillID;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) 
	{
		m_strDBErrorMessage = "�G���R�[�h�w��Ɏ��s���܂���";
		PQfinish(conn);
		return DB_NG;
	}
	/* �폜�Ώۂ̈Č��X�L�������폜 */
	strSQL =
		"DELETE FROM \"T_PROJ_SKILL\" "
		"WHERE "
		"\"PRJ_SKL_ID\" = " + std::to_string(cProjectData.nPrjSkillID) + " ";

	/*�폜�������s */
	res = PQexec(conn, strSQL.c_str());
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}
	PQclear(res);

	/* �폜�Ώۂ̈Č������폜 */
	strSQL =
		"DELETE FROM \"T_PROJ_INFO\" "
		"WHERE "
		"\"PRJ_ID\" = " + std::to_string(cProjectData.nProjectId) + " ";

	/*�폜�������s */
	res = PQexec(conn, strSQL.c_str());
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}
	PQclear(res);

	return DB_OK;
}

/****************************************************************/
/* �}�X�^�Ǘ��n													*/
/****************************************************************/

//***********************************************************//
// �X�L�����}�X�^�̎擾
//***********************************************************//
int CDBAccess::DBGetSkillData(std::vector<CSkillData>& vSkillData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	int       j;
	std::string strSQL;
	CSkillData cTempSkillData;

	char      msg[300];
	std::string strLog;
	std::wstring strLogW;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

	memset(msg, 0, 300);

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("�G���R�[�h�w��Ɏ��s���܂���");
		PQfinish(conn);
		return 0;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
		"\"\SKILL_ID\","
		"\"SKILL_NAME\" "
		"FROM "
		"\"M_SKILL\" "
		"ORDER BY \"SKILL_ID\" DESC ";

	/* �e�[�u������s�����o�� */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());
	int a = PQresultStatus(res);
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		sprintf(msg,
			"%s",
			PQerrorMessage(conn));

		PQclear(res);
		PQfinish(conn);
		return 0;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		cTempSkillData.nSkillID = atol(PQgetvalue(res, i, 0));
		cTempSkillData.strSkillName = PQgetvalue(res, i, 1);

		vSkillData.push_back(cTempSkillData);
	}

	PQclear(res);

	/* �|�[�^��������B */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* �g�����U�N�V�������I������ */
	res = PQexec(conn, "END");
	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return 0;
}

		
//***********************************************************//
// �S���ҏ��}�X�^�̓o�^�E�X�V
//***********************************************************//
int CDBAccess::DBSetUserData(CString strId, CString strName, CString strPass, CString strAdmin, int nUpdate)
{
		PGconn    *conn = NULL;
		PGresult  *res;
		int       nFields;
		int       i;
		int       j;
		std::string strSQL;
		std::string strSQL2;
		CCompanyData cTempCmpnData;

		char      msg[300];
		std::string strLog;
		std::wstring strLogW;
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
		
		std::wstring strwId;
		std::wstring strwName;
		std::wstring strwPass;
		std::wstring strwAdmin;

		std::string strDBId;
		std::string strDBName;
		std::string strDBPass;
		std::string strDBAdmin;

		strwId = strId;
		strwName = strName;
		strwPass = strPass;
		strwAdmin = strAdmin;

		StrConv(strDBId, strwId);
//		StrConv(strDBName, strwName);

		strDBName = ToMultStr(strwName);
		StrConv(strDBPass, strwPass);
		StrConv(strDBAdmin, strwAdmin);

		memset(msg, 0, 300);
		


		/* SQL��ݒ�*/
		if (nUpdate == 0)
		{

			strSQL =
				"INSERT INTO"
				"\"M_USERS\"("
				"\"USER_ID\", \"USER_NAME\", "
				"\"PASSWARD\", \"ADMIN\", "
				"\"LOGIN_STATE\", "
				"\"CREATE_DATE\", \"UPDATE_DATE\" "
				" )"
				"VALUES(";
			strSQL += strDBId;
			strSQL += ", '";
			strSQL += strDBName;
			strSQL += "', '";
			strSQL += strDBPass;
			strSQL += "', ";
			strSQL += strDBAdmin;
			strSQL += ", ";
			strSQL += "false";
			strSQL += ", ";
			strSQL += "'now()', 'now()'";
			strSQL += ");";

			strSQL2 = "SELECT SETVAL(";
			strSQL2 += "'users_id_seq',";
			strSQL2 += strDBId.c_str();
			strSQL2 += ")";

		}
		else if (nUpdate == 1)
		{
			strSQL =
				"UPDATE "
				"\"M_USERS\" "
				"SET \"USER_NAME\" = '";
			strSQL += strDBName;
			strSQL += "', ";
			strSQL += "\"PASSWARD\" = '";
			strSQL += strDBPass;
			strSQL += "', ";
			strSQL += "\"ADMIN\" = '";
			strSQL += strDBAdmin;
			strSQL += "', ";
			strSQL += "\"UPDATE_DATE\" = 'now()'";
			strSQL += "WHERE \"USER_ID\" = '";
			strSQL += strDBId;
			strSQL += "' ;";

			strSQL2 = "";
		}
		else if (nUpdate == 2)
		{
			strSQL =
				"DELETE FROM "
				"\"M_USERS\" ";
			strSQL += "WHERE \"USER_ID\" = '";
			strSQL += strDBId;
			strSQL += "' ;";

			strSQL2 = "";
		}



		int nRet;
		nRet = DBSetMstDataUpdate(strSQL, strSQL2);

		return 0;
}






//***********************************************************//
// �X�L�����}�X�^�̓o�^�E�X�V
//***********************************************************//
int CDBAccess::DBSetSkillData(CString strSkillId, CString strSkillName, int nUpdate)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	int       j;
	std::string strSQL;
	std::string strSQL2;
	CSkillData cTempSkillData;

	char      msg[300];
	std::string strLog;
	std::wstring strLogW;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

	std::wstring strwSkillId;
	std::wstring strwSkillName;
	std::string strDBSkillId;
	std::string strDBSkillName;

	strwSkillId = strSkillId;
	strwSkillName = strSkillName;
	StrConv(strDBSkillId, strwSkillId);
	strDBSkillName = ToMultStr(strwSkillName);


	memset(msg, 0, 300);

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("�G���R�[�h�w��Ɏ��s���܂���");
		PQfinish(conn);
		return 0;
	}

	/* SQL��ݒ�*/
	if (nUpdate == 0)
	{

		strSQL =
			"INSERT INTO"
			"\"M_SKILL\"("
			"\"SKILL_ID\", \"SKILL_NAME\", "
			"\"CREATE_DATE\", \"UPDATE_DATE\" "
			" )"
			"VALUES(";
		strSQL += strDBSkillId;
		strSQL += ", '" ;
		strSQL += strDBSkillName;
		strSQL += "', ";
		strSQL += "'now()', 'now()'";
		strSQL += ");";

		strSQL2 = "SELECT SETVAL(";
		strSQL2 += "'skill_id_seq',";
		strSQL2 += strDBSkillId.c_str();
		strSQL2 += ")";

	}
	else if (nUpdate == 1)
	{
		strSQL =
			"UPDATE "
			"\"M_SKILL\" "
			"SET \"SKILL_NAME\" = '";
		strSQL += strDBSkillName;
		strSQL += "', ";
		strSQL += "\"CREATE_DATE\" = 'now()'";
		strSQL += "WHERE \"SKILL_ID\" = '";
		strSQL += strDBSkillId;
		strSQL += "' ";

		strSQL2 = "";
	}
	else if (nUpdate == 2)
	{
		strSQL =
			"DELETE FROM "
			"\"M_SKILL\" ";
		strSQL += "WHERE \"SKILL_ID\" = '";
		strSQL += strDBSkillId;
		strSQL += "' ";

		strSQL2 = "";
	}

	int nRet;
	nRet = DBSetMstDataUpdate(strSQL, strSQL2);

/*
	// �e�[�u���Ƀf�[�^��o�^���� //
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());
	int a = PQresultStatus(res);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		sprintf(msg,
			"%s",
			PQerrorMessage(conn));

		PQclear(res);
		PQfinish(conn);
		return 0;
	}


	PQclear(res);

	if (bUpdate == FALSE)
	{
		strSQL = "SELECT SETVAL(";
		strSQL += "'skill_id_seq',";
		strSQL += strDBSkillId.c_str();
		strSQL += ")";

		res = PQexec(conn, strSQL.c_str());

	}

	// �|�[�^��������B //
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	// �g�����U�N�V�������I������ //
	res = PQexec(conn, "END");
	PQclear(res);

	// �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B//
	PQfinish(conn);
*/

	return 0;
}
//***********************************************************//
// ��Џ��}�X�^�̓o�^�E�X�V
//***********************************************************//
int CDBAccess::DBSetCmpnData(CString strId, CString strName, CString strFuri, int nUpdate)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	int       j;
	std::string strSQL;
	std::string strSQL2;
	CCompanyData cTempCmpnData;

	char      msg[300];
	std::string strLog;
	std::wstring strLogW;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

	std::wstring strwId;
	std::wstring strwName;
	std::wstring strwFuri;
	std::string strDBId;
	std::string strDBName;
	std::string strDBFuri;

	strwId = strId;
	strwName = strName;
	strwFuri = strFuri;
	StrConv(strDBId, strwId);
	strDBName = ToMultStr(strwName);
	strDBFuri = ToMultStr(strwFuri);


	memset(msg, 0, 300);

	/* SQL��ݒ�*/
	if (nUpdate == 0)
	{

		strSQL =
			"INSERT INTO"
			"\"M_CMPN\"("
			"\"CMPN_ID\", \"CMPN_NAME\", \"CMPN_NAME_FURI\", "
			"\"CREATE_DATE\", \"UPDATE_DATE\" "
			" )"
			"VALUES(";
		strSQL += strDBId;
		strSQL += ", '";
		strSQL += strDBName;
		strSQL += "', '";
		strSQL += strDBFuri;
		strSQL += "', ";
		strSQL += "'now()', 'now()'";
		strSQL += ");";

		strSQL2 = "SELECT SETVAL(";
		strSQL2 += "'cmpn_id_seq',";
		strSQL2 += strDBId.c_str();
		strSQL2 += ")";

	}
	else if (nUpdate == 1)
	{
		strSQL =
			"UPDATE "
			"\"M_CMPN\" "
			"SET \"CMPN_NAME\" = '";
		strSQL += strDBName;
		strSQL += "', ";
		strSQL += "\"CMPN_NAME_FURI\" = '";
		strSQL += strDBFuri;
		strSQL += "', ";
		strSQL += "\"UPDATE_DATE\" = 'now()'";
		strSQL += "WHERE \"CMPN_ID\" = '";
		strSQL += strDBId;
		strSQL += "' ;";

		strSQL2 = "";
	}
	else if (nUpdate == 2)
	{
		strSQL =
			"DELETE FROM "
			"\"M_CMPN\" ";
		strSQL += "WHERE \"CMPN_ID\" = '";
		strSQL += strDBId;
		strSQL += "' ;";

		strSQL2 = "";
	}
	int nRet;
	nRet = DBSetMstDataUpdate(strSQL, strSQL2);

	return 0;
}


//***********************************************************//
// ��Џ��Ǘ��}�X�^�̎擾
//***********************************************************//
int CDBAccess::DBGetCompanyData(std::vector<CCompanyData>& vCompanyData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	int       j;
	std::string strSQL;
	CCompanyData cTempCompanyData;

	char      msg[300];
	std::string strLog;
	std::wstring strLogW;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

	memset(msg, 0, 300);

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("�G���R�[�h�w��Ɏ��s���܂���");
		PQfinish(conn);
		return 0;
	}

	/* SQL��ݒ�*/
	strSQL =
		"SELECT "
		"\"CMPN_ID\","
		"\"CMPN_NAME\","
		"\"CMPN_NAME_FURI\" "
		"FROM "
		"\"M_CMPN\" "
		"ORDER BY \"CMPN_ID\" DESC ";

	/* �e�[�u������s�����o�� */
	res = PQexec(conn, strSQL.c_str());
	int a = PQresultStatus(res);
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		sprintf(msg,
			"%s",
			PQerrorMessage(conn));

		PQclear(res);
		PQfinish(conn);
		return 0;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);

	/* �����čs��\������B */
	for (i = 0; i < PQntuples(res); i++)
	{
		cTempCompanyData.nCompanyID = atol(PQgetvalue(res, i, 0));
		cTempCompanyData.strCompanyName = PQgetvalue(res, i, 1);
		cTempCompanyData.strCompanyName_Furi = PQgetvalue(res, i, 2);

		vCompanyData.push_back(cTempCompanyData);
	}

	PQclear(res);

	/* �|�[�^��������B */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* �g�����U�N�V�������I������ */
	res = PQexec(conn, "END");
	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return 0;
}

//***********************************************************//
// �V�[�P���X�̎擾
//***********************************************************//
int CDBAccess::DBGetSeqquence(std::string strSeqName, int& nSequence)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	int       j;
	std::string strSQL;
	CCompanyData cTempCompanyData;

	char      msg[300];
	std::string strLog;
	std::wstring strLogW;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

	memset(msg, 0, 300);

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("�G���R�[�h�w��Ɏ��s���܂���");
		PQfinish(conn);
		return 0;
	}


//	strDBName = 'skill_id_seq'
	/* SQL��ݒ�*/
	strSQL = "SELECT LAST_VALUE FROM \"";
	strSQL += strSeqName;
	strSQL += "\"";

//	strSQL = "SELECT nextval('";
//	strSQL += strSeqName;
//	strSQL += "')";

	/* �e�[�u������s�����o�� */
	res = PQexec(conn, strSQL.c_str());
	int a = PQresultStatus(res);
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		sprintf(msg,
			"%s",
			PQerrorMessage(conn));

		PQclear(res);
		PQfinish(conn);
		return 0;
	}

	///* select�����t�B�[���h�� */
	nFields = PQnfields(res);
	if(nFields != 0)
	{
		nSequence = atol(PQgetvalue(res, 0, 0));
		nSequence ++;
	}

	PQclear(res);

	/* �|�[�^��������B */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* �g�����U�N�V�������I������ */
	res = PQexec(conn, "END");
	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return 0;
}


//***********************************************************//
// �}�X�^�̓o�^�E�X�V
//***********************************************************//
int CDBAccess::DBSetMstDataUpdate(std::string strSQL, std::string strSQL2)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	int       j;
//	std::string strSQL;
//	CSkillData cTempSkillData;

	char      msg[300];
	std::string strLog;
	std::wstring strLogW;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

/*
	std::wstring strwSkillId;
	std::wstring strwSkillName;
	std::string strDBSkillId;
	std::string strDBSkillName;

	strwSkillId = strSkillId;
	strwSkillName = strSkillName;
	StrConv(strDBSkillId, strwSkillId);
	StrConv(strDBSkillName, strwSkillName);
*/

	memset(msg, 0, 300);

	/* �f�[�^�x�[�X�Ƃ̐ڑ����m������ */
	DBConnection(&conn);

	/* �o�b�N�G���h�Ƃ̐ڑ��m���ɐ������������m�F���� */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* client��encoding�w�� */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("�G���R�[�h�w��Ɏ��s���܂���");
		PQfinish(conn);
		return 0;
	}


	// �e�[�u���Ƀf�[�^��o�^���� //
	res = PQexec(conn, strSQL.c_str());
	int a = PQresultStatus(res);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		sprintf(msg,
			"%s",
			PQerrorMessage(conn));

		PQclear(res);
		PQfinish(conn);
		return 0;
	}


	PQclear(res);

	if (strSQL2.length() > 0)
	{
		res = PQexec(conn, strSQL2.c_str());
	}

	/* �|�[�^��������B */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* �g�����U�N�V�������I������ */
	res = PQexec(conn, "END");
	PQclear(res);

	/* �f�[�^�x�[�X�Ƃ̐ڑ�����A��n�����s�Ȃ��B */
	PQfinish(conn);

	return 0;
}

std::string CDBAccess::ToMultStr(const std::wstring &strWide)
{
	USES_CONVERSION;
	return W2A(strWide.c_str());
}


std::wstring CDBAccess::ToWideStr(const std::string &strMult)
{
	USES_CONVERSION;
	return A2W(strMult.c_str());
}