/**
* @file CDBAccess.cpp
* @brief DB制御共通
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
	//wstring→string
	//strConvString = m_Convert.to_bytes(strWString);

	char *mbs = new char[strWString.length() * MB_CUR_MAX + 1];

	wcstombs(mbs, strWString.c_str(), strWString.length() * MB_CUR_MAX + 1);

	strConvString = mbs;

	delete[] mbs;
}

void CDBAccess::WStrConv(std::wstring& strConvWString, std::string& strString)
{
	//string→wstring
	//strConvWString = m_Convert.from_bytes(strString);

	// SJIS → wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, strString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, strString.c_str(), -1, cpUCS2
		, iBufferSize);

	// stringの生成
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// バッファの破棄
	delete[] cpUCS2;

	strConvWString = oRet;

	// 変換結果を返す
	return;
}

std::wstring CDBAccess::OutErrorMsg()
{
	std::wstring strErrorMsg;

	WStrConv(strErrorMsg, m_strDBErrorMessage);

	return strErrorMsg;
}

/************************************************************/
/* 接続情報設定処理											*/
/************************************************************/
void CDBAccess::DBConnection(PGconn** pConn)
{
	char      conninfo[256];

	sprintf(conninfo, "host=%s port=%s dbname=%s user=%s password=%s"
//		, "localhost"         //ホスト名
		, "192.168.251.34"    //ホスト名(共有サーバ)
		, "5432"              //ポート 
		, "db_ANKEN_KANRI"    //データベース名
		, "postgres"          //接続ユーザ名
		, "postgres");        //パスワード

	*pConn = PQconnectdb(conninfo);

	return ;
}

/************************************************************/
/* ログイン判定処理											*/
/************************************************************/
int CDBAccess::DBLoginChk(std::string& strUserName, std::string& strPassword)
{

	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;

	/* DB接続 */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}

	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}
	/* トランザクションブロックを開始する。 */
	//res = PQexec(conn, "BEGIN");
	//if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	//	PQerrorMessage(conn);
	//	PQclear(res);
	//	PQfinish(conn);
	//	return 0;
	//}

	/* 不要になったら、メモリリークを防ぐためにPGresultをPQclearすべき。 */
	//PQclear(res);

	/* SQLを設定*/
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

	/* テーブルから行を取り出す */
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

	///* selectしたフィールド数 */
	//nFields = PQnfields(res);
	///* そして行を表示する。 */
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

	/* 結果行数を参照 */
	int nCnt = PQntuples(res);

	switch (nCnt)
	{
		/* 結果なし */
		case 0:
			return DB_NOTUSERDATA;
			break;

		/* 結果あり */
		default:
			m_strLoginUserName = PQgetvalue(res, 0, 0);
			m_nLoginUserID = atol(PQgetvalue(res, 0, 1));
			m_strAdmin = PQgetvalue(res, 0, 3);

			break;
	}

	PQclear(res);

	/* ポータルを閉ざす。 */
	//res = PQexec(conn, "CLOSE myportal");
	//if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	//	PQerrorMessage(conn);
	//	PQclear(res);
	//	PQfinish(conn);
	//	return 0;
	//}
	//PQclear(res);

	/* データを追加するには以下を実行すればよい。 */
	//res = PQexec(conn, "insert into aaa values ('あいうえお','かきくけこ')");
	//if (PQresultStatus(res) != PGRES_COMMAND_OK){
	//    MessageBox(PQerrorMessage(conn));
	//    PQclear(res);
	//    PQfinish(conn);
	//    return ;
	//}

	/* トランザクションを終了する */
	//res = PQexec(conn, "END");
	//PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* ユーザ情報参照処理(マスタ)								*/
/************************************************************/
int CDBAccess::DBGetUserList(std::vector<CUserData>& vUserData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CUserData cTempUserData;

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1)
	{
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQLを設定*/
	strSQL =
		"SELECT "
			"\"USER_ID\","
			"\"USER_NAME\","
			"\"PASSWARD\" ,"
			"\"ADMIN\" "
		"FROM "
			"\"M_USERS\" "
		"ORDER BY \"USER_ID\" DESC ";

	/* テーブルから行を取り出す */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
	for (i = 0; i < PQntuples(res); i++)
	{
		cTempUserData.nUserID = atol(PQgetvalue(res, i, 0));
		cTempUserData.strUserName = PQgetvalue(res, i, 1);
		cTempUserData.strPassword = PQgetvalue(res, i, 2);
		cTempUserData.strAdmin = PQgetvalue(res, i, 3);

		vUserData.push_back(cTempUserData);
	}

	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* ユーザ情報参照処理										*/
/************************************************************/
int CDBAccess::DBGetUserData(std::vector<CUserData>& vUserData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CUserData cTempUserData;

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQLを設定*/
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

	/* テーブルから行を取り出す */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
	for (i = 0; i < PQntuples(res); i++)
	{
			cTempUserData.nUserID = atol(PQgetvalue(res, i, 0));
			cTempUserData.strUserName = PQgetvalue(res, i, 1);
			cTempUserData.strPassword = PQgetvalue(res, i, 2);
			cTempUserData.strAdmin = PQgetvalue(res, i, 3);

			vUserData.push_back(cTempUserData);
	}

	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* 案件情報検索処理											*/
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

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1)
	{
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQLを設定*/
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

	/* 検索条件を設定 */

	/* 案件名指定か */
	if (cProjectSearch.bAnkenNameSet == true)
	{
		/* 案件名を部分検索 */
		strWhere += "AND \"T_PROJ_INFO\".\"PRJ_NAME\" LIKE '%" + cProjectSearch.strAnkenName + "%' ";
	}

	/* 担当者指定か*/
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

	/* 会社名の検索*/
	if(cProjectSearch.strCmpName != "")
	{
		/* 会社名を部分検索 */
		strWhere += "AND \"M_CMPN\".\"CMPN_NAME\" LIKE '%"+ cProjectSearch.strCmpName + "%' ";
	}

	/* 詳細検索 */
	if (cProjectSearch.bDetailSearch == true)
	{
		/* 期間指定ありか */
		if (cProjectSearch.bValidDay == true)
		{
			strWhere +=
				"AND "
				"\"T_PROJ_INFO\".\"PERI_FROM\" BETWEEN "
				"'" + cProjectSearch.strPeriFrom + "' AND "
				"'" + cProjectSearch.strPeriTo + "' ";
		}

		/* フラグ条件設定 */
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

		// スキル検索条件があるか
		if(cProjectSearch.vSkillData.size() != 0)
		{ 
			bool bFirst = true;

			strWhere +=
				" AND \"T_PROJ_INFO\".\"PRJ_SKL_ID\" in "
				"(SELECT \"T_PROJ_SKILL\".\"PRJ_SKL_ID\" FROM \"T_PROJ_SKILL\" "
				" where ";

			/* 検索スキル設定 */
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

	/* 検索条件を追加 */
	strSQL = strSQL + strWhere;

	/* テーブルから行を取り出す */
	res = PQexec(conn, strSQL.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);

		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
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

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* 案件詳細情報検索処理										*/
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
	
	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);

		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1)
	{
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);

		return DB_NG;
	}

	/* SQLを設定*/
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

	/* テーブルから行を取り出す */
	res = PQexec(conn, strSQL.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);

		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
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

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* スキル一覧情報検索処理									*/
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

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQLを設定*/
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

	/* テーブルから行を取り出す */
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
	for (i = 0; i < PQntuples(res); i++)
	{
		strSkillName = PQgetvalue(res, i, 1);
		cProjectData.vSkillList.push_back(strSkillName);
	}

	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* 会社情報参照処理											*/
/************************************************************/
int CDBAccess::DBGetCmpData(std::vector<CCmpData>& vCmpData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CCmpData cCmpData;

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("エンコード指定に失敗しました");
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQLを設定*/
	strSQL =
	"SELECT "
		"\"CMPN_ID\","
		"\"CMPN_NAME\","
		"\"CMPN_NAME_FURI\" "
	"FROM "
		"\"M_CMPN\" "
	"ORDER BY "
		"\"CMPN_ID\" ASC ";

	/* テーブルから行を取り出す */
	//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from \"T_PROJ_INFO\"");
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
	for (i = 0; i < PQntuples(res); i++)
	{
		cCmpData.nCmpID = atol(PQgetvalue(res, i, 0));
		cCmpData.strCmpName = PQgetvalue(res, i, 1);
		cCmpData.strCmpKana = PQgetvalue(res, i, 2);

		vCmpData.push_back(cCmpData);
	}

	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* スキルマスタ情報参照処理									*/
/************************************************************/
int CDBAccess::DBGetSkillMasta(std::vector<CSkillData>& vSkillData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	int       nFields;
	int       i;
	std::string strSQL;
	CSkillData cSkillData;

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) 
	{
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQLを設定*/
	strSQL =
		"SELECT "
			"\"SKILL_ID\","
			"\"SKILL_NAME\" "
		"FROM "
			"\"M_SKILL\" ";

	/* テーブルから行を取り出す */
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) 
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
	for (i = 0; i < PQntuples(res); i++)
	{
		cSkillData.nSkillID = atol(PQgetvalue(res, i, 0));
		cSkillData.strSkillName = PQgetvalue(res, i, 1);

		vSkillData.push_back(cSkillData);
	}

	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* 案件情報新規登録処理										*/
/************************************************************/
int CDBAccess::DBSetNewProject(CProjectData& cProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;
	std::string strNewSkillID;

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("エンコード指定に失敗しました");
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}

	/* SQLを設定*/
	strSQL = "select nextval('prj_skill_id_seq')";

	/* テーブルから行を取り出す */
	res = PQexec(conn, strSQL.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}

	/* 最新のスキルIDを参照 */
	strNewSkillID = PQgetvalue(res, 0, 0);

	PQclear(res);

	/* 案件スキルテーブルの更新 */
	for(std::vector<CSkillData>::iterator it = cProjectData.vSkillData.begin();
		it != cProjectData.vSkillData.end();
		it++)
	{
		if(it->bSetSkill == true)
		{
			/* 更新コマンド実行 */
			strSQL =
				"INSERT INTO \"T_PROJ_SKILL\"("
				"\"PRJ_SKL_ID\", \"SKL_ID\", \"OTHER\", \"CREATE_DATE\", \"UPDATE_DATE\") "
				"VALUES("
				+ strNewSkillID + "," +
				std::to_string(it->nSkillID) + "," +
				"' '," +
				"now(),now()) ";

			/* 更新処理実行 */
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

	/* 案件情報登録 */

	/* 更新コマンド実行 */
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

	/* 更新処理実行 */
	res = PQexec(conn, strSQL.c_str());
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}
	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* 案件情報更新処理											*/
/************************************************************/
int CDBAccess::DBUpdateProject(CProjectData& cProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;
	std::string strNewSkillID;

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) 
	{
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}
	/* スキル更新を行ったか */
	if (cProjectData.bSkillUpdateFlg == true)
	{
		/* 更新前の案件スキル情報を削除 */
		strSQL =
			"DELETE FROM \"T_PROJ_SKILL\" "
			"WHERE "
				"\"PRJ_SKL_ID\" = " + std::to_string(cProjectData.nPrjSkillID) + " ";

		/*削除処理実行 */
		res = PQexec(conn, strSQL.c_str());
		if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			m_strDBErrorMessage = PQerrorMessage(conn);
			PQclear(res);
			PQfinish(conn);
			return DB_NG;
		}
		PQclear(res);

		/* 新しいスキル情報を作成 */
		for (std::vector<CSkillData>::iterator it = cProjectData.vSkillData.begin();
			it != cProjectData.vSkillData.end();
			it++)
		{
			if (it->bSetSkill == true)
			{
				/* 更新コマンド実行 */
				strSQL =
					"INSERT INTO \"T_PROJ_SKILL\"("
					"\"PRJ_SKL_ID\", \"SKL_ID\", \"OTHER\", \"CREATE_DATE\", \"UPDATE_DATE\") "
					"VALUES("
					+ std::to_string(cProjectData.nPrjSkillID) + "," +
					std::to_string(it->nSkillID) + "," +
					"' '," +
					"now(),now()) ";

				/* 更新処理実行 */
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

	/* 更新コマンド実行 */
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

	/* 更新処理実行 */
	res = PQexec(conn, strSQL.c_str());
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}
	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return DB_OK;
}

/************************************************************/
/* 案件情報削除処理											*/
/************************************************************/
int CDBAccess::DBDeleteProject(CProjectData& cProjectData)
{
	PGconn    *conn = NULL;
	PGresult  *res;
	std::string strSQL;
	std::string strNewSkillID;

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQfinish(conn);
		return DB_NG;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) 
	{
		m_strDBErrorMessage = "エンコード指定に失敗しました";
		PQfinish(conn);
		return DB_NG;
	}
	/* 削除対象の案件スキル情報を削除 */
	strSQL =
		"DELETE FROM \"T_PROJ_SKILL\" "
		"WHERE "
		"\"PRJ_SKL_ID\" = " + std::to_string(cProjectData.nPrjSkillID) + " ";

	/*削除処理実行 */
	res = PQexec(conn, strSQL.c_str());
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		m_strDBErrorMessage = PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return DB_NG;
	}
	PQclear(res);

	/* 削除対象の案件情報を削除 */
	strSQL =
		"DELETE FROM \"T_PROJ_INFO\" "
		"WHERE "
		"\"PRJ_ID\" = " + std::to_string(cProjectData.nProjectId) + " ";

	/*削除処理実行 */
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
/* マスタ管理系													*/
/****************************************************************/

//***********************************************************//
// スキル情報マスタの取得
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

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("エンコード指定に失敗しました");
		PQfinish(conn);
		return 0;
	}

	/* SQLを設定*/
	strSQL =
		"SELECT "
		"\"\SKILL_ID\","
		"\"SKILL_NAME\" "
		"FROM "
		"\"M_SKILL\" "
		"ORDER BY \"SKILL_ID\" DESC ";

	/* テーブルから行を取り出す */
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

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
	for (i = 0; i < PQntuples(res); i++)
	{
		cTempSkillData.nSkillID = atol(PQgetvalue(res, i, 0));
		cTempSkillData.strSkillName = PQgetvalue(res, i, 1);

		vSkillData.push_back(cTempSkillData);
	}

	PQclear(res);

	/* ポータルを閉ざす。 */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* トランザクションを終了する */
	res = PQexec(conn, "END");
	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return 0;
}

		
//***********************************************************//
// 担当者情報マスタの登録・更新
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
		


		/* SQLを設定*/
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
// スキル情報マスタの登録・更新
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

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("エンコード指定に失敗しました");
		PQfinish(conn);
		return 0;
	}

	/* SQLを設定*/
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
	// テーブルにデータを登録する //
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

	// ポータルを閉ざす。 //
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	// トランザクションを終了する //
	res = PQexec(conn, "END");
	PQclear(res);

	// データベースとの接続を閉じ、後始末を行なう。//
	PQfinish(conn);
*/

	return 0;
}
//***********************************************************//
// 会社情報マスタの登録・更新
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

	/* SQLを設定*/
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
// 会社情報管理マスタの取得
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

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("エンコード指定に失敗しました");
		PQfinish(conn);
		return 0;
	}

	/* SQLを設定*/
	strSQL =
		"SELECT "
		"\"CMPN_ID\","
		"\"CMPN_NAME\","
		"\"CMPN_NAME_FURI\" "
		"FROM "
		"\"M_CMPN\" "
		"ORDER BY \"CMPN_ID\" DESC ";

	/* テーブルから行を取り出す */
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

	///* selectしたフィールド数 */
	nFields = PQnfields(res);

	/* そして行を表示する。 */
	for (i = 0; i < PQntuples(res); i++)
	{
		cTempCompanyData.nCompanyID = atol(PQgetvalue(res, i, 0));
		cTempCompanyData.strCompanyName = PQgetvalue(res, i, 1);
		cTempCompanyData.strCompanyName_Furi = PQgetvalue(res, i, 2);

		vCompanyData.push_back(cTempCompanyData);
	}

	PQclear(res);

	/* ポータルを閉ざす。 */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* トランザクションを終了する */
	res = PQexec(conn, "END");
	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return 0;
}

//***********************************************************//
// シーケンスの取得
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

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("エンコード指定に失敗しました");
		PQfinish(conn);
		return 0;
	}


//	strDBName = 'skill_id_seq'
	/* SQLを設定*/
	strSQL = "SELECT LAST_VALUE FROM \"";
	strSQL += strSeqName;
	strSQL += "\"";

//	strSQL = "SELECT nextval('";
//	strSQL += strSeqName;
//	strSQL += "')";

	/* テーブルから行を取り出す */
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

	///* selectしたフィールド数 */
	nFields = PQnfields(res);
	if(nFields != 0)
	{
		nSequence = atol(PQgetvalue(res, 0, 0));
		nSequence ++;
	}

	PQclear(res);

	/* ポータルを閉ざす。 */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* トランザクションを終了する */
	res = PQexec(conn, "END");
	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
	PQfinish(conn);

	return 0;
}


//***********************************************************//
// マスタの登録・更新
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

	/* データベースとの接続を確立する */
	DBConnection(&conn);

	/* バックエンドとの接続確立に成功したかを確認する */
	if (PQstatus(conn) != CONNECTION_OK) {
		PQerrorMessage(conn);
		PQfinish(conn);
		return 0;
	}
	/* clientのencoding指定 */
	if (PQsetClientEncoding(conn, "SJIS") == -1) {
		//MessageBox("エンコード指定に失敗しました");
		PQfinish(conn);
		return 0;
	}


	// テーブルにデータを登録する //
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

	/* ポータルを閉ざす。 */
	res = PQexec(conn, "CLOSE myportal");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		return 0;
	}
	PQclear(res);

	/* トランザクションを終了する */
	res = PQexec(conn, "END");
	PQclear(res);

	/* データベースとの接続を閉じ、後始末を行なう。 */
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