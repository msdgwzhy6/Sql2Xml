#include "SqlToXml.h"
#include <stdio.h>
#include "SimpleLog.h"
#include <iostream>
SqlToXml::SqlToXml()
{
	
}

SqlToXml::~SqlToXml()
{
}

bool SqlToXml::GetStatementBySql(string path)
{
	if (!path.empty())
	{
		m_sqlPath = path;
	}

	ifstream fin(m_sqlPath.c_str());
	if (!fin.is_open())
	{
		return false;
	}
	
	writeFile("INFO",(m_sqlPath+" File Dispose Start...").c_str());
	string statement="";
	while (!fin.eof())
	{
		string temp;

		getline(fin,temp);

		if (!temp.empty())	//字符串不为空
		{
			//注释
			if (StartsWith(temp, note))
			{
				continue;
			}

			//是语句
			if (EndsWith(temp, semicolon))
			{
				statement += temp;
				ToLowerCase(statement);	//转换成小写 来处理

				if (StartsWith(statement,DBFlag) || StartsWith(statement,TABLEFlag))
				{
					m_vecStatement.push_back(statement);
				}
				
				statement = "";
			}
			else
			{
				statement += temp;
				statement += fieldIsolate;		//添加分隔符
			}
		}
	}
	if(m_vecStatement.size()>0)
		writeFile("SUCCESS",(m_sqlPath+" File Load Success!").c_str());
	else writeFile("WARN",(m_sqlPath+" File Load Error!").c_str());
	return true;
}

void SqlToXml::CreateXml()
{
	writeFile("INFO",(m_sqlPath +"'s XML-File Create Start...").c_str());
	TiXmlDocument* doc = new TiXmlDocument();
	TiXmlElement* root = new TiXmlElement("root");
	doc->LinkEndChild(root);

	TiXmlElement* database = NULL;
	TiXmlElement* table = NULL;
	TiXmlElement* field = NULL;

	for (int i = 0; i < m_vecStatement.size(); i++)
	{
		string statement = m_vecStatement[i];

		//判断 是否 是数据库
		if (StartsWith(statement, DBFlag))
		{
			database = new TiXmlElement("database");

			DatabaseName(statement, database);		//数据库信息保存

			root->LinkEndChild(database);
		}
		else
		{
			int DataNum = 0;			//表中字段 非String类型的数量	
			int StringNum = 0;			//表中字段 string类型的数量
			int StringOffset = 0;		//当前字段的偏移量（string类型）
			int Offset = 0;				//当前字段的偏移量（非string类型）
			char temp[512] = "";

			int first = statement.find_first_of(bracketsL);
			int last = statement.find_last_of(bracketsR);

			//获取没有处理的信息
			string tableName = statement.substr(0, first);	//表名
			string tableComment = statement.substr(last + bracketsR.size(), statement.size() - last - bracketsR.size());	//表的评论
			string fieldsAndPermary = statement.substr(first + bracketsL.size(), last - first - bracketsL.size());

			string fieldsInfo = "";
			string tablePrimary = "";

			if (Find(fieldsAndPermary, primary))	//表 有主键
			{
				first = fieldsAndPermary.find(primary);

				fieldsInfo = fieldsAndPermary.substr(0, first);	//表所有字段
				tablePrimary = fieldsAndPermary.substr(first, fieldsAndPermary.size() - first);	//表的主键
			}
			else
			{
				fieldsInfo = fieldsAndPermary;	//表所有字段
				tablePrimary = "";
			}

			//处理表table信息
			table = new TiXmlElement("table");

			TableInfo(tableName, tableComment, tablePrimary, table);	//表信息保存	

			database->LinkEndChild(table);

			//处理表field信息
			Trim(fieldsInfo, space);
			Trim(fieldsInfo, comma);
			Trim(fieldsInfo, fieldIsolate);

			vector<string> vecField = Split(fieldsInfo, fieldIsolate);

			for (int i = 0; i < vecField.size(); i++)
			{
				string info = vecField[i];
				string fname = "";
				string ftype = "";
				string fcomment = "";

				field = new TiXmlElement("field");

				FieldInfo(info, fname, ftype, fcomment, field); //表中field保存

				if (ftype == "string")
				{
					snprintf(temp, 512, "%d", StringOffset);
					field->SetAttribute("offset", temp);
					StringOffset++;

					StringNum++;
				}
				else
				{
					snprintf(temp, 512, "%d", Offset);
					field->SetAttribute("offset", temp);

					if (ftype == "double" || ftype == "int64" || ftype == "uint64")
					{
						Offset += 2;
					}
					else
					{
						Offset += 1;
					}

					DataNum++;
				}

				table->LinkEndChild(field);
			}

			snprintf(temp, 512, "%d", DataNum);
			table->SetAttribute("DataNum", temp);
			snprintf(temp, 512, "%d", StringNum);
			table->SetAttribute("StringNum", temp);
		}
	}

	string xmlPath = m_sqlPath;

	if (Find(xmlPath, sql))
	{
		Trim(xmlPath, sql);
	}

	xmlPath = xmlPath + xml;

	doc->SaveFile(xmlPath.c_str());
	writeFile("SUCCESS",(xmlPath+" File Create Success!").c_str());
}

void SqlToXml::DatabaseName(string &name, TiXmlElement* database)
{
	Trim(name, DBFlag);
	Trim(name, space);
	Trim(name, semicolon);
	Trim(name, quotes1);

	database->SetAttribute("name", name.c_str());
	database->SetAttribute("comment", "");
}

void SqlToXml::TableInfo(string &name, string &comment, string &primary, TiXmlElement* table)
{
	TableName(name);
	TableComment(comment);

	table->SetAttribute("name", name.c_str());
	table->SetAttribute("comment", comment.c_str());

	if (!primary.empty())
	{
		TablePrimary(primary);

		if (Find(primary, comma))
		{
			//多个主键
			vector<string> vecPrimary = Split(primary, comma);

			for (int i = 0; i < vecPrimary.size(); i++)
			{
				Trim(vecPrimary[i], quotes1);
			}

			table->SetAttribute("primary", vecPrimary[0].c_str());

			for (int i = 1; i < vecPrimary.size(); i++)
			{
				char buffer[512] = "";
				snprintf(buffer, 512, "primary%d", i + 1);
				table->SetAttribute(buffer, vecPrimary[i].c_str());
			}
		}
		else
		{
			table->SetAttribute("primary", primary.c_str());
		}
	}


}

void SqlToXml::FieldInfo(string &info, string &name, string &type, string &comment, TiXmlElement* field)
{
	Trim(info, space);
	Trim(info, comma);
	//name
	int first = info.find_first_of(space);
	name = info.substr(0, first);
	Trim(name, quotes1);

	info = info.substr(first, info.size() - first);
	//type

	//string
	if (Find(info, tText) || Find(info, tTinytext) || Find(info, tMediumtext) || Find(info, tLongtext)
		|| Find(info, tVarchar) || Find(info, tDate) || Find(info, tDatetime) || Find(info, tTimestamp) || Find(info, tTime)
		|| Find(info, tYear) || Find(info, tChar))
	{
		type = "string";
	}
	else if (Find(info, tDouble) || Find(info, tDecimal) || Find(info, tReal))//double
	{
		type = "double";
	}
	else if (Find(info, tFloat))//float
	{
		type = "float";
	}
	else if (Find(info, tBigint))//int64
	{
		if (Find(info, tUnsigned))
		{
			type = "uint64";
		}
		else
		{
			type = "int64";
		}
	}
	else if (Find(info, tInt) || Find(info, tBoolean))//int32
	{
		if (Find(info, tUnsigned))
		{
			type = "uint32";
		}
		else
		{
			type = "int32";
		}
	}

	//comment
	if (Find(info, desc))
	{
		first = info.find(desc);
		comment = info.substr(first + desc.size(), info.size() - first - desc.size());
		Trim(comment, space);
		Trim(comment, quotes2);
	}

	field->SetAttribute("name", name.c_str());
	field->SetAttribute("type", type.c_str());
	field->SetAttribute("comment", comment.c_str());
}

void SqlToXml::TableName(string &name)
{
	Trim(name, TABLEFlag);
	Trim(name, space);
	Trim(name, quotes1);
}

void SqlToXml::TableComment(string &comment)
{
	if (Find(comment, desc))
	{
		int index = comment.find(desc) + desc.size();
		int size = comment.size() - index;
		comment = comment.substr(index, size);

		Trim(comment, semicolon);
		Trim(comment, equalSign);
		Trim(comment, quotes2);
	}
	else
	{
		comment = "";
	}

}

void SqlToXml::TablePrimary(string &key)
{
	int first = key.find_first_of(bracketsL);
	int last = key.find_first_of(bracketsR);
	key = key.substr(first + bracketsL.size(), last - first - bracketsL.size());

	Trim(key, quotes1);
}

void SqlToXml::ToLowerCase(string& str)
{
	int size = str.size()+1;
	char* buffer = new char[size];
	memset(buffer,'\0',size);
	snprintf(buffer, size, "%s", str.c_str());

	for(int i=0;i<size;i++)
	{
	    buffer[i] = (char)tolower(buffer[i]);
	}

	str = buffer;

	delete[] buffer;
}

void SqlToXml::ToUpperCase(string& str)
{

	int size = str.size()+1;
	char* buffer = new char[size];
	snprintf(buffer, size, "%s", str.c_str());
	
	for(int i=0;i<size;i++)
	{
	    buffer[i] = (char)toupper(buffer[i]);
	}
	str = buffer;

	delete[] buffer;
}

bool SqlToXml::StartsWith(const string& str, const string& pattern)
{
	if (str.length() < pattern.length())
		return false;

	return (str.compare(0, pattern.length(), pattern) == 0);
}

bool SqlToXml::EndsWith(const string& str, const string& pattern)
{
    if (str.length() >= pattern.length())
    {
    	//int compare(int pos, int n,const string &s)const;
		//比较当前字符串从pos开始的n个字符组成的字符串与s的大小
        return (0 == str.compare(str.length() - pattern.length(), pattern.length(), pattern));
    }
     return false;
}

vector<string> SqlToXml::Split(const string& str,const string &separateStr)
{
	vector<string> strs;

	int separateLen = separateStr.size();
	int lastPosition = 0, index;
	while (string::npos != (index = str.find(separateStr, lastPosition)))
	{
		strs.push_back(str.substr(lastPosition, index - lastPosition));
		lastPosition = index + separateLen;
	}
	string lastStr = str.substr(lastPosition);
	if (!lastStr.empty())
		strs.push_back(lastStr);
	return strs;
}

bool SqlToXml::Find(const string &str,const string &substr)
{
	if (string::npos == str.find(substr))
	{
		return false;
	}

	return true;
}

void SqlToXml::Trim(string &str, const string &pattern)
{
	if (!str.empty())
	{
		str.erase(0, str.find_first_not_of(pattern));
		str.erase(str.find_last_not_of(pattern) + 1);
	}
}

void SqlToXml::clear()
{
	m_vecStatement.clear();
	m_sqlPath = "";
}

void SqlToXml::DisplayStatement()
{
	char buffer[512] = "";

	cout << "所有sql语句：" << endl;
	for (int i = 0; i < m_vecStatement.size(); i++)
	{
		snprintf(buffer,512,"第%d条语句：",i+1); 
		cout << buffer << endl;
		cout << m_vecStatement[i] << endl;
	}
}
