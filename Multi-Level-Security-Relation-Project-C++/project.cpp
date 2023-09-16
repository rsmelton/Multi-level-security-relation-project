#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
using namespace std;

class TableColumnsObject {
    public:
        string tableName;
        string columnName;
        string columnDataType;
        int columnID;
};

class WhereClauseFields {
    public:
        string columnName;
        string relationalOperator;
        int referenceValueInt = 0;
        string referenceValueStr;
        float referenceValueFloat = 0.0;
        char referenceValueChar;
};

class OrderByClauseFields {
    public:
        string columnName;
        bool isAscendingOrder;
};

class ParsedRowPlusInfo {
    public:
        vector<string> parsedRowFromFile;
        vector<TableColumnsObject> sortedTabColumns;
        vector<OrderByClauseFields> orderbyOrdering;
        string columnValueForRow1;
        string columnValueForRow2;
        string tableName;
};

class SelectOperationFields {
    public: 
        vector<string> columnChoice;
        bool isInclusionList;
        string tableName;
        vector<WhereClauseFields> conditions;
        vector<OrderByClauseFields> ordering;
};

string delimitColon(string str) {
    int start = 0;
    int end = str.find(':');
    string token;
    token = str.substr(start, end);
    return token;
};

string parseString(string str, char delimiter) {
    stringstream ss(str);
    string word;
    getline(ss, word, delimiter);
    return word;
};

string parseReferenceValue(string str, char delimiter) {
    string word = "";
    int start = str.find(delimiter) + 1;
    while (start != str.length()) {
        word.push_back(str.at(start));
        start = start + 1;
    }
    return word;
}

string findColumnType(string columnName, vector<TableColumnsObject> tab_columns, string tableName) {
    for (int i = 0; i < tab_columns.size(); i++) {
        if (tab_columns.at(i).columnName == columnName && tab_columns.at(i).tableName == tableName) {
            return tab_columns.at(i).columnDataType;
        }
    }
    return "Error: Cannot find the column.";
};

int findColumnID(string columnName, vector<TableColumnsObject> tab_columns, string tableName) {
    for (int i = 0; i < tab_columns.size(); i++) {
        // cout << "Column Name: " << tab_columns.at(i).columnName << endl;
        string currentColumnName = tab_columns.at(i).columnName;
        string currentTableName = tab_columns.at(i).tableName;
        if ((currentColumnName == columnName) && (currentTableName == tableName)) {
            int columnIDtoReturn = tab_columns.at(i).columnID - 1;
            return columnIDtoReturn;
        }
    }
    cout << "Error: Could not find the Column ID for column name = " << columnName << " and table name = " << tableName << endl;
    return -1;
};

WhereClauseFields parseAndStoreWhereClauseFields(string str, vector<TableColumnsObject> tab_columns, string tableName) {
    // this function deals specifically with a conditon that has been mashed together
    // for example: SALARY>=30000
    WhereClauseFields whereClause;
    stringstream ss(str);
    string word;

    // if we make it to this point, then we know that the string is mashed together
    // and now we have to parse and store accordingly.
    while (ss.eof() == false) {
        ss >> word;
        // takes care of the first piece of the word.
        // next we need to take care of the relational operator.
        if (word.find('<') != std::string::npos) {
            whereClause.columnName = parseString(word, '<');
            string columnType = findColumnType(whereClause.columnName, tab_columns, tableName);
            // cout << "Column Type: " << columnType << endl;
            if ((word.at(word.find('<') + 1)) == '>') {
                whereClause.relationalOperator = "<>";
                if (columnType == "INT") {
                    whereClause.referenceValueInt = stoi(parseReferenceValue(word, '>'));
                }
                else if (columnType == "FLOAT") {
                    whereClause.referenceValueFloat = stof(parseReferenceValue(word, '>'));
                }
                else if (columnType == "CHAR") {
                    whereClause.referenceValueChar = parseReferenceValue(word, '>').at(0);
                }
                else {
                    whereClause.referenceValueStr = parseReferenceValue(word, '>');
                }
            }
            else if ((word.at(word.find('<') + 1)) == '=') {
                whereClause.relationalOperator = "<=";
                if (columnType == "INT") {
                    whereClause.referenceValueInt = stoi(parseReferenceValue(word, '='));
                }
                else if (columnType == "FLOAT") {
                    whereClause.referenceValueFloat = stof(parseReferenceValue(word, '='));
                }
                else if (columnType == "CHAR") {
                    whereClause.referenceValueChar = parseReferenceValue(word, '=').at(0);
                }
                else {
                    whereClause.referenceValueStr = parseReferenceValue(word, '=');
                }
            }
            else {
                whereClause.relationalOperator = "<";
                if (columnType == "INT") {
                    whereClause.referenceValueInt = stoi(parseReferenceValue(word, '<'));
                }
                else if (columnType == "FLOAT") {
                    whereClause.referenceValueFloat = stof(parseReferenceValue(word, '<'));
                }
                else if (columnType == "CHAR") {
                    whereClause.referenceValueChar = parseReferenceValue(word, '<').at(0);
                }
                else {
                    whereClause.referenceValueStr = parseReferenceValue(word, '<');
                }
            }
        }
        else if (word.find('>') != std::string::npos) {
            whereClause.columnName = parseString(word, '>');
            string columnType = findColumnType(whereClause.columnName, tab_columns, tableName);
            // cout << "Column Type: " << columnType << endl;
            if ((word.at(word.find('>') + 1)) == '=') {
                whereClause.relationalOperator = ">=";
                if (columnType == "INT") {
                    whereClause.referenceValueInt = stoi(parseReferenceValue(word, '='));
                }
                else if (columnType == "FLOAT") {
                    whereClause.referenceValueFloat = stof(parseReferenceValue(word, '='));
                }
                else if (columnType == "CHAR") {
                    whereClause.referenceValueChar = parseReferenceValue(word, '=').at(0);
                }
                else {
                    whereClause.referenceValueStr = parseReferenceValue(word, '=');
                }
            }
            else {
                whereClause.relationalOperator = ">";
                if (columnType == "INT") {
                    whereClause.referenceValueInt = stoi(parseReferenceValue(word, '>'));
                }
                else if (columnType == "FLOAT") {
                    whereClause.referenceValueFloat = stof(parseReferenceValue(word, '>'));
                }
                else if (columnType == "CHAR") {
                    whereClause.referenceValueChar = parseReferenceValue(word, '>').at(0);
                }
                else {
                    whereClause.referenceValueStr = parseReferenceValue(word, '>');
                }
            }
        }
        else if (word.find('=') != std::string::npos) {
            whereClause.columnName = parseString(word, '=');
            string columnType = findColumnType(whereClause.columnName, tab_columns, tableName);
            // cout << "Column Type: " << columnType << endl;
            whereClause.relationalOperator = "=";
            if (columnType == "INT") {
                whereClause.referenceValueInt = stoi(parseReferenceValue(word, '='));
            }
            else if (columnType == "FLOAT") {
                whereClause.referenceValueFloat = stof(parseReferenceValue(word, '='));
            }
            else if (columnType == "CHAR") {
                whereClause.referenceValueChar = parseReferenceValue(word, '=').at(0);
            }
            else {
                whereClause.referenceValueStr = parseReferenceValue(word, '=');
            }
        }
    }
    return whereClause;
};

bool isColumnName(string str, vector<TableColumnsObject> tab_columns) {
    for (int i = 0; i < tab_columns.size(); i++) {
        if (tab_columns.at(i).columnName == str) {
            return true;
        }
    }
    return false;
};

bool isAscendingOrderOrNot(string str) {
    if ((str.at(str.find(':') + 1)) == '1') {
        return true;
    }
    return false;
};

SelectOperationFields parseAndStoreSelectOperationFields(string text, vector<TableColumnsObject> tab_columns) {
    SelectOperationFields selectOperation;
    stringstream ss(text);
    string word;

    while (ss.eof() == false) {
        ss >> word;
        if (word == "SELECT") {
            ss >> word;
            while (word != "FROM") {
                if (word == "*") {
                    selectOperation.columnChoice.push_back(word);
                }
                else if (word.find(',') != std::string::npos) {
                    string str;
                    stringstream st(word);
                    while (getline(st, str, ',')) {
                        //cout << "Unmodified: " << str << " ";
                        // while we still have a string left to separate.
                        // deal with whether or not the list of columns is an inclusion or exclusion list.
                        if ((str.at(str.find(':') + 1)) == '1') {
                            selectOperation.isInclusionList = true;
                        }
                        else {
                            selectOperation.isInclusionList = false;
                        }
                        // send str to helper function to delimit colon.
                        // finally push column name to vector.
                        //cout << "Modified: " << delimitColon(str) << " ";
                        selectOperation.columnChoice.push_back(delimitColon(str));
                    }
                }
                else {
                    // if we get to this point we know there may have been whitespace in between
                    // the list of columns, or there was only one column to begin with.
                    // so we simply delimit the colon and deal with the 1 or 0 and push the 
                    // column name to the vector.
                    if ((word.at(word.find(':') + 1)) == '1') {
                        selectOperation.isInclusionList = true;
                    }
                    else {
                        selectOperation.isInclusionList = false;
                    }
                    //cout << "Unmodified: " << word << " ";
                    //cout << "Modified: " << delimitColon(word) << " ";
                    selectOperation.columnChoice.push_back(delimitColon(word));
                }
                ss >> word;
            }
        }
        // we know the word before was FROM so we can get the next word which will be the table name.
        ss >> word;
        if (word.find(';') != std::string::npos) { // the case where there is a semicolon in the table name so we must parse it.
            string currentPieceOfString = parseString(word, ';');
            selectOperation.tableName = currentPieceOfString;
        }
        else {
            selectOperation.tableName = word;
        }
        // now deal with where and orderby:
        while (ss.eof() == false) {
            if ((word != "WHERE") && (word != "ORDERBY")) {
                ss >> word;
            }
            if (word == "WHERE") {
                //cout << word << endl;
                WhereClauseFields whereClause;
                while ((word != "ORDERBY") && (ss.eof() == false)) {
                    ss >> word;
                    if (word == "ORDERBY") {
                        break;
                    }
                    if (isColumnName(word, tab_columns)) {
                        string columnType = findColumnType(word, tab_columns, selectOperation.tableName);
                        // this is the case where we have spaces in between pieces of the condition.
                        // such as, SALARY <= 30000
                        whereClause.columnName = word;
                        ss >> word;
                        whereClause.relationalOperator = word;
                        ss >> word;
                        if (word.find(',') != std::string::npos) { // parses if the word has a comma
                            string currentPieceOfString = parseString(word, ',');
                            if (columnType == "INT") {
                                whereClause.referenceValueInt = stoi(currentPieceOfString);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else if (columnType == "FLOAT") {
                                whereClause.referenceValueFloat = stof(currentPieceOfString);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else if (columnType == "CHAR") {
                                whereClause.referenceValueChar = currentPieceOfString.at(0);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else {
                                whereClause.referenceValueStr = currentPieceOfString;
                                selectOperation.conditions.push_back(whereClause);
                            }
                        }
                        else if (word.find(';') != std::string::npos) { // parses if the last word has a semicolon
                            string currentPieceOfString = parseString(word, ';');
                            if (columnType == "INT") {
                                whereClause.referenceValueInt = stoi(currentPieceOfString);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else if (columnType == "FLOAT") {
                                whereClause.referenceValueFloat = stof(currentPieceOfString);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else if (columnType == "CHAR") {
                                whereClause.referenceValueChar = currentPieceOfString.at(0);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else {
                                whereClause.referenceValueStr = currentPieceOfString;
                                selectOperation.conditions.push_back(whereClause);
                            }
                        }
                        else { // if the word doesn't have a comma or semicolon then don't worry about parsing
                            if (columnType == "INT") {
                                whereClause.referenceValueInt = stoi(word);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else if (columnType == "FLOAT") {
                                whereClause.referenceValueFloat = stof(word);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else if (columnType == "CHAR") {
                                whereClause.referenceValueChar = word.at(0);
                                selectOperation.conditions.push_back(whereClause);
                            }
                            else {
                                whereClause.referenceValueStr = word;
                                selectOperation.conditions.push_back(whereClause);
                            }
                        }
                    }
                    else if (word.find(',') != std::string::npos) { // meaning there is a comma in the word, meaning we have a list of conditions.
                        string currentPieceOfString = parseString(word, ',');
                        whereClause = parseAndStoreWhereClauseFields(currentPieceOfString, tab_columns, selectOperation.tableName);
                        selectOperation.conditions.push_back(whereClause);
                    }
                    else if (word.find(';') != std::string::npos) { // meaning there is a semicolon in the word, so we must parse it.
                        string currentPieceOfString = parseString(word, ';');
                        whereClause = parseAndStoreWhereClauseFields(currentPieceOfString, tab_columns, selectOperation.tableName);
                        selectOperation.conditions.push_back(whereClause);
                    }
                    else {
                        // if we get to this point then we know the condition is a chunk with no comma or semicolon.
                        whereClause = parseAndStoreWhereClauseFields(word, tab_columns, selectOperation.tableName);
                        selectOperation.conditions.push_back(whereClause);
                    }
                }
            }
            else if (word == "ORDERBY") {
                OrderByClauseFields orderByClause;
                while ((word != "WHERE") && (ss.eof() == false)) {
                    ss >> word;
                    if (word == "WHERE") {
                        break;
                    }
                    //cout << word << endl;
                    if (word.find(',') != std::string::npos) {
                        // parses off the comma 
                        string currentPieceOfString = parseString(word, ',');
                        // extract whether it is ascending or not
                        orderByClause.isAscendingOrder = isAscendingOrderOrNot(currentPieceOfString);
                        // exract column name from the currentPieceOfString
                        orderByClause.columnName = delimitColon(currentPieceOfString);
                        // store values in data structure
                        selectOperation.ordering.push_back(orderByClause);
                    }
                    else if (word.find(';') != std::string::npos) {
                        string currentPieceOfString = parseString(word, ';');
                        orderByClause.isAscendingOrder = isAscendingOrderOrNot(currentPieceOfString);
                        orderByClause.columnName = delimitColon(currentPieceOfString);
                        selectOperation.ordering.push_back(orderByClause);
                    }
                    else { // if we get to this point we know there wasn't a comma or semicolon in the string
                        orderByClause.isAscendingOrder = isAscendingOrderOrNot(word);
                        orderByClause.columnName = delimitColon(word);
                        selectOperation.ordering.push_back(orderByClause);
                    }
                }
            }
        }
    }
    // cout << "Select Clause Column Choices: " << endl;
    // for (int i = 0; i < selectOperation.columnChoice.size(); i++) {
    //     cout << "Column Name: " << selectOperation.columnChoice.at(i) << endl;
    // }
    // cout << "From Clause Table Name: " << selectOperation.tableName << endl;
    // cout << "Where Clause Conditions: " << endl;
    // for (int i = 0; i < selectOperation.conditions.size(); i++) {
    //     cout << "Column Name: " << selectOperation.conditions.at(i).columnName << ", ";
    //     cout << "Relational Opeartor: " << selectOperation.conditions.at(i).relationalOperator << ", ";
    //     cout << "Int Reference Val: " << selectOperation.conditions.at(i).referenceValueInt << ", ";
    //     cout << "String Reference Val: " << selectOperation.conditions.at(i).referenceValueStr << ", ";
    //     cout << "Float Reference Val: " << selectOperation.conditions.at(i).referenceValueFloat << ", ";
    //     cout << "Character Reference Val: " << selectOperation.conditions.at(i).referenceValueChar << endl;
    // }
    // cout << "OrderBy Clause Column Names: " << endl;
    // for (int i = 0; i < selectOperation.ordering.size(); i++) {
    //     cout << "Column Name: " << selectOperation.ordering.at(i).columnName << endl;
    //     if (selectOperation.ordering.at(i).isAscendingOrder) {
    //         cout << "Is Ascending Order? Yes" << endl;
    //     }
    //     else {
    //         cout << "Is Ascending Order? No" << endl;
    //     }
    // }
    return selectOperation;
};

TableColumnsObject parseAndStoreTableColumnFields(string text) {
    TableColumnsObject tableColumnFields;
    stringstream ss(text);
    string word;
    int i = 0;
    while (ss.eof() == false) {
        getline(ss, word, ',');
        if (i == 0) {
            tableColumnFields.tableName = word;
        }
        else if (i == 1) {
            tableColumnFields.columnName = word;
        }
        else if (i == 2) {
            tableColumnFields.columnDataType = word;
        }
        else {
            tableColumnFields.columnID = stoi(word);
        }
        i++;
    }
    return tableColumnFields;
}

bool compareColumnIDs(TableColumnsObject x, TableColumnsObject y) {
    return (x.columnID < y.columnID);
};

vector<TableColumnsObject> printIncludedColumns(SelectOperationFields selectOperation, vector<TableColumnsObject> tab_columns) {
    vector<TableColumnsObject> sortedTabColumns;
    if (selectOperation.columnChoice.at(0) == "*") { // all columns from the specific table name need to be printed.
        for (int i = 0; i < tab_columns.size(); i++) {
            if (selectOperation.tableName == tab_columns.at(i).tableName) {
                sortedTabColumns.push_back(tab_columns.at(i));
            }
        }
    }
    else if (selectOperation.isInclusionList) { // if it is an inclusive list then we store included columns.
        for (int i = 0; i < selectOperation.columnChoice.size(); i++) {
            for (int j = 0; j < tab_columns.size(); j++) {
                if (selectOperation.tableName == tab_columns.at(j).tableName) {
                    if (selectOperation.columnChoice.at(i) == tab_columns.at(j).columnName) {
                        sortedTabColumns.push_back(tab_columns.at(j));
                        if (i == selectOperation.columnChoice.size() - 1) { // we know we are at the last column name, so print a newline instead of a comma.
                            // cout << sortedTabColumns.at(i).columnName << endl;
                            // cout << endl;
                            for (int i = 0; i < sortedTabColumns.size(); i++) {
                                if (i == sortedTabColumns.size() - 1) { // we know we are at the last column name, so print a newline instead of a comma.
                                    cout << sortedTabColumns.at(i).columnName << endl;
                                    break;
                                }
                                cout << sortedTabColumns.at(i).columnName << ",";
                            }
                            sort(sortedTabColumns.begin(), sortedTabColumns.end(), compareColumnIDs);
                            return sortedTabColumns;
                        }
                        // cout << sortedTabColumns.at(i).columnName << ",";
                        break;
                    }   
                }
            }
        }
    }
    else { // if we reach this point it must be an exclude list, meaning exclude certain columns
        bool addToVector = true;
        for (int i = 0; i < tab_columns.size(); i++) {
            while ((i < tab_columns.size()) && (selectOperation.tableName == tab_columns.at(i).tableName)) {
                // cout << "Table name: " << tab_columns.at(i).tableName << endl;
                // cout << "Column name: " << tab_columns.at(i).columnName << endl;
                for (int j = 0; j < selectOperation.columnChoice.size(); j++) {
                    // cout << "Column Choice: " << selectOperation.columnChoice.at(j) << " " << "Compared to Column Name: " << tab_columns.at(i).columnName << endl;
                    if (selectOperation.columnChoice.at(j) == tab_columns.at(i).columnName) {
                        // cout << "Column Choice: " << selectOperation.columnChoice.at(j) << " " << "Compared to Column Name: " << tab_columns.at(i).columnName << endl;
                        addToVector = false;
                        break;
                    }
                }
                if (addToVector) {
                    sortedTabColumns.push_back(tab_columns.at(i));
                }
                addToVector = true;
                i++;
            }   
        }
    }
    // cout << endl;
    sort(sortedTabColumns.begin(), sortedTabColumns.end(), compareColumnIDs);
    for (int i = 0; i < sortedTabColumns.size(); i++) {
        if (i == sortedTabColumns.size() - 1) { // we know we are at the last column name, so print a newline instead of a comma.
            cout << sortedTabColumns.at(i).columnName << endl;
            break;
        }
        cout << sortedTabColumns.at(i).columnName << ",";
    }
    return sortedTabColumns;
};

bool compareTwoColumnValues(WhereClauseFields condition, string columnValue, string typeOfVariable) {
    if (typeOfVariable == "INT") {
        int columnVal = stoi(columnValue);
        // cout << "Column Val: " << columnVal << endl;
        if (condition.relationalOperator == "=") {
            if (columnVal == condition.referenceValueInt) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<>") {
            if (columnVal != condition.referenceValueInt) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">=") {
            if (columnVal >= condition.referenceValueInt) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<=") {
            if (columnVal <= condition.referenceValueInt) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">") {
            if (columnVal > condition.referenceValueInt) {
                return true;
            }
            return false;
        }
        else {
            if (columnVal < condition.referenceValueInt) {
                return true;
            }
            return false;
        }
    }
    else if (typeOfVariable == "FLOAT") {
        float columnVal = stof(columnValue);
        if (condition.relationalOperator == "=") {
            if (columnVal == condition.referenceValueFloat) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<>") {
            if (columnVal != condition.referenceValueFloat) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">=") {
            // cout << "Column Val: " << columnVal << endl;
            if (columnVal >= condition.referenceValueFloat) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<=") {
            if (columnVal <= condition.referenceValueFloat) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">") {
            if (columnVal > condition.referenceValueFloat) {
                return true;
            }
            return false;
        }
        else {
            if (columnVal < condition.referenceValueFloat) {
                return true;
            }
            return false;
        }
    }
    else if (typeOfVariable == "CHAR") {
        int columnVal = int(columnValue.at(0));
        if (condition.relationalOperator == "=") {
            if (columnVal == int(condition.referenceValueChar)) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<>") {
            if (columnVal != int(condition.referenceValueChar)) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">=") {
            if (columnVal >= int(condition.referenceValueChar)) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<=") {
            if (columnVal <= int(condition.referenceValueChar)) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">") {
            if (columnVal > int(condition.referenceValueChar)) {
                return true;
            }
            return false;
        }
        else {
            if (columnVal < int(condition.referenceValueChar)) {
                return true;
            }
            return false;
        }
    }
    else {
        int comparisonOfStrings = columnValue.compare(condition.referenceValueStr);
        if (condition.relationalOperator == "=") {
            if (columnValue == condition.referenceValueStr) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<>") {
            if (columnValue != condition.referenceValueStr) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">=") {
            // cout << "Is " << columnValue << " >= " << condition.referenceValueStr << endl;
            if (comparisonOfStrings > 0 || columnValue == condition.referenceValueStr) {
                // cout << "YES IT IS" << endl;
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == "<=") {
            if (comparisonOfStrings < 0 || columnValue == condition.referenceValueStr) {
                return true;
            }
            return false;
        }
        else if (condition.relationalOperator == ">") {
            if (comparisonOfStrings > 0) {
                return true;
            }
            return false;
        }
        else {
            if (comparisonOfStrings < 0) {
                return true;
            }
            return false;
        }
    }
};

bool isValidLine(vector<string> parsedRow, WhereClauseFields condition, vector<TableColumnsObject> sortedTabColumns, string tableName) {
    int positionOfColumn;
    for (int i = 0; i < sortedTabColumns.size(); i++) { // find which column name has the condition
        if (condition.columnName == sortedTabColumns.at(i).columnName) {
            // cout << "Column Name: " << condition.columnName << endl;
            positionOfColumn = sortedTabColumns.at(i).columnID;
            break;
        }
    }
    if (parsedRow.at(positionOfColumn - 1) == " ") {
        return false;
    }

    string columnValue = parsedRow.at(positionOfColumn - 1);
    // cout << "Column Value From Parsed Row: " << columnValue << endl;
    string columnType = findColumnType(condition.columnName, sortedTabColumns, tableName);
    // cout << "Column Type: " << columnType << endl;
    if (columnType == "INT") {
        // cout << "Did we make it" << endl;
        if (compareTwoColumnValues(condition, columnValue, columnType)) {
            return true;
        } 
        return false;
    }
    else if (columnType == "FLOAT") {
        // cout << "Did we make it" << endl;
        if (compareTwoColumnValues(condition, columnValue, columnType)) {
            return true;
        } 
        return false;
    }
    else if (columnType == "CHAR") {
        if (compareTwoColumnValues(condition, columnValue, columnType)) {
            return true;
        } 
        return false;
    }
    else {
        if (compareTwoColumnValues(condition, columnValue, columnType)) {
            return true;
        } 
        return false;
    }
};

bool compareTwoRows(ParsedRowPlusInfo row1, ParsedRowPlusInfo row2) {
    // cout << "Inside Comparator : " << endl;
    vector<OrderByClauseFields> orderbyOrdering = row1.orderbyOrdering;
    vector<TableColumnsObject> sortedTabColumns = row1.sortedTabColumns;
    string tableName = row1.tableName;

    // int nameID = findColumnID("LNAME", sortedTabColumns, tableName);
    // cout << "Name ID: " << nameID << endl;
    // string person1 = row1.parsedRowFromFile.at(nameID);
    // string person2 = row2.parsedRowFromFile.at(nameID);
    // cout << "Person 1: " << person1 << endl;
    // cout << "Person 2: " << person2 << endl;
    for (int i = 0; i < orderbyOrdering.size(); i++) {
        string columnType = findColumnType(orderbyOrdering.at(i).columnName, sortedTabColumns, tableName);
        // cout << "  Looking at column name " << orderbyOrdering.at(i).columnName << "(" << columnType << ")" << endl;
        int columnID = findColumnID(orderbyOrdering.at(i).columnName, sortedTabColumns, tableName); // subtract 1 since vectors are 0 based
        // cout << "  Column ID: " << columnID << endl;
        row1.columnValueForRow1 = row1.parsedRowFromFile.at(columnID);
        row2.columnValueForRow2 = row2.parsedRowFromFile.at(columnID);

        // cout << "Row 1 Column Value: " << row1.columnValueForRow1 << endl;
        // cout << "Row 2 Column Value: " << row2.columnValueForRow2 << endl;

        // Empty String Case:
        if ((row1.columnValueForRow1 == " ") && (orderbyOrdering.at(i).isAscendingOrder == true)) {
            return (row1.columnValueForRow1 > row2.columnValueForRow2); 
        }
        else if ((row1.columnValueForRow1 == " ") && (orderbyOrdering.at(i).isAscendingOrder == false)) {
            return (row1.columnValueForRow1 < row2.columnValueForRow2);
        }
        else if ((row2.columnValueForRow2 == " ") && (orderbyOrdering.at(i).isAscendingOrder == true)) {
            return (row2.columnValueForRow2 > row1.columnValueForRow1); 
        }
        else if ((row2.columnValueForRow2 == " ") && (orderbyOrdering.at(i).isAscendingOrder == false)) {
            return (row2.columnValueForRow2 < row1.columnValueForRow1);
        }

        // 3 cases: <, >, or = ... if = then we have to go to the next specified ordering.
        // After we check the > case, if it is not that case then we know the two column values are equal. 
        if (columnType == "INT") {
            int row1ColumnValue = stoi(row1.columnValueForRow1);
            int row2ColumnValue = stoi(row2.columnValueForRow2);
            if (row1ColumnValue == row2ColumnValue) {
                // dont do anything -- but go to next specified ordering.
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == true) {
                return (row1ColumnValue < row2ColumnValue);
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == false) {
                return (row1ColumnValue > row2ColumnValue);
            }
        }
        else if (columnType == "FLOAT") {
            // cout << "    Column Type: " << columnType << endl;
            float row1ColumnValue = stof(row1.columnValueForRow1);
            float row2ColumnValue = stof(row2.columnValueForRow2);
            // cout << "    Comparing Column 1: " << row1ColumnValue << " to Column 2: " << row2ColumnValue << endl;
            if (row1ColumnValue == row2ColumnValue) {
                // dont do anything -- but go to next specified ordering.
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == true) {
                return (row1ColumnValue < row2ColumnValue);
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == false) {
                // cout << "    Inside else: " << endl;
                // cout << "    Is " << row1ColumnValue << " > " << row2ColumnValue << "? " << (row1ColumnValue > row2ColumnValue) << endl;
                return (row1ColumnValue > row2ColumnValue);
            }
        }
        else if (columnType == "CHAR") {
            char row1ColumnValue = row1.columnValueForRow1.at(0);
            char row2ColumnValue = row2.columnValueForRow2.at(0);
            if (row1ColumnValue == row2ColumnValue) {
                // dont do anything -- but go to next specified ordering.
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == true) {
                return (row1ColumnValue < row2ColumnValue);
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == false) {
                return (row1ColumnValue > row2ColumnValue);
            }
        }
        else if (columnType == "STRING") {
            string row1ColumnValue = row1.columnValueForRow1;
            string row2ColumnValue = row2.columnValueForRow2;
            if (row1ColumnValue == row2ColumnValue) {
                // dont do anything -- but go to next specified ordering.
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == true) {
                return (row1ColumnValue < row2ColumnValue);
            }
            else if (orderbyOrdering.at(i).isAscendingOrder == false) {
                return (row1ColumnValue > row2ColumnValue);
            }
        }
    }   
    return false;
};

vector<string> parsesRowIntoColumns(string rowFromFile) {
    stringstream ss(rowFromFile);
    string columnFromRow;
    vector<string> parsedRow;
    // *** Only trims leading whitespace *** 
    while (getline(ss, columnFromRow, ',')) {
        string columnVal = "";
        bool haveSeenFirstCharacter = false;
        for (int i = 0; i < columnFromRow.length(); i++) {
            if (isspace(columnFromRow.at(i)) == false) {
                haveSeenFirstCharacter = true;
            }
            if (haveSeenFirstCharacter) {
                columnVal.push_back(columnFromRow.at(i));
            }
        }
        if (columnFromRow == " ") {
            string emptyString = " ";
            parsedRow.push_back(emptyString);
        }
        else {
            parsedRow.push_back(columnVal);
        }
       // parsedRow.push_back(columnVal);
    }
    // *** Now trim trailing whitespace ***

    return parsedRow;
};

string getColumnValueFromRow(string columnName, vector<string> parsedRowFromFile, string tableName, vector<TableColumnsObject> sortedTabColumns) {
    int columnIndex = findColumnID(columnName, sortedTabColumns, tableName);
    string columnValueFromRow = parsedRowFromFile.at(columnIndex);
    return columnValueFromRow;
};

bool foundColumnName(string columnName, vector<TableColumnsObject> sortedTabColumns) {
    for (int i = 0; i < sortedTabColumns.size(); i++) {
        string sortedTabColumnsColumnName = sortedTabColumns.at(i).columnName;
        if (sortedTabColumnsColumnName == columnName) {
            return true;
        }
    }
    return false;
};

void processSelectOperation(SelectOperationFields selectOperation, vector<TableColumnsObject> tab_columns, int tc) {
    vector<TableColumnsObject> sortedTabColumns = printIncludedColumns(selectOperation, tab_columns);
    string fileName = selectOperation.tableName + ".csv";
    ifstream fileNameStream(fileName);
    // Checks to make sure the file opened successfully.
    if (fileNameStream.is_open() == false) { 
        cout << fileName << " file could not be opened." << endl;
        return;
    }
    string rowFromFile;
    bool keepRowFromFile = true;
    vector<ParsedRowPlusInfo> allParsedRowsPlusAllInfo;
    ParsedRowPlusInfo parsedRowPlusInfo;
    parsedRowPlusInfo.tableName = selectOperation.tableName;

    while (getline(fileNameStream, rowFromFile)) {
        parsedRowPlusInfo.parsedRowFromFile = parsesRowIntoColumns(rowFromFile);
        if (selectOperation.conditions.empty() == false) { 
            for (int i = 0; i < selectOperation.conditions.size(); i++) { // check the line from the file to see if we keep it or not depending on the condition.
                if (isValidLine(parsedRowPlusInfo.parsedRowFromFile, selectOperation.conditions.at(i), sortedTabColumns, selectOperation.tableName) == false) {
                    keepRowFromFile = false;
                    // break;
                }
            }
            // int columnIndex = findColumnID("TC", sortedTabColumns, selectOperation.tableName);
            int columnIndex = findColumnID("TC", tab_columns, selectOperation.tableName);
            int tupleClassificationFromRow = stoi(parsedRowPlusInfo.parsedRowFromFile.at(columnIndex));
            if ((keepRowFromFile) && (tupleClassificationFromRow <= tc)) {
                parsedRowPlusInfo.orderbyOrdering = selectOperation.ordering;
                // parsedRowPlusInfo.sortedTabColumns = sortedTabColumns;
                parsedRowPlusInfo.sortedTabColumns = tab_columns;
                allParsedRowsPlusAllInfo.push_back(parsedRowPlusInfo);
            }
            keepRowFromFile = true;
        }
        else {
            int columnIndex = findColumnID("TC", tab_columns, selectOperation.tableName);
            int tupleClassificationFromRow = stoi(parsedRowPlusInfo.parsedRowFromFile.at(columnIndex));
            if (tupleClassificationFromRow <= tc) {
                parsedRowPlusInfo.orderbyOrdering = selectOperation.ordering;
                // parsedRowPlusInfo.sortedTabColumns = sortedTabColumns;
                parsedRowPlusInfo.sortedTabColumns = tab_columns;
                allParsedRowsPlusAllInfo.push_back(parsedRowPlusInfo);
            }
        }
    }
    if (selectOperation.ordering.empty() == false) { // checks to see if we have a specific orderby clause ordering.
        sort(allParsedRowsPlusAllInfo.begin(), allParsedRowsPlusAllInfo.end(), compareTwoRows);
    }

    if (selectOperation.columnChoice.at(0) == "*" && selectOperation.columnChoice.size() == 1) {
        for (int i = 0; i < allParsedRowsPlusAllInfo.size(); i++) {
            for (int j = 0; j < allParsedRowsPlusAllInfo.at(i).parsedRowFromFile.size(); j++) {
                if (j == allParsedRowsPlusAllInfo.at(i).parsedRowFromFile.size() - 1) {
                    cout << allParsedRowsPlusAllInfo.at(i).parsedRowFromFile.at(j) << endl;
                    break;
                }   
                cout << allParsedRowsPlusAllInfo.at(i).parsedRowFromFile.at(j) << ",";
            }
        }
    }
    else if (selectOperation.isInclusionList == true) {
        string tableName = selectOperation.tableName;
        for (int i = 0; i < allParsedRowsPlusAllInfo.size(); i++) {
            vector<string> parsedRowFromFile = allParsedRowsPlusAllInfo.at(i).parsedRowFromFile;
            for (int j = 0; j < selectOperation.columnChoice.size(); j++) {
                string currentColumnName = selectOperation.columnChoice.at(j);
                bool isLastColumnName = (j == selectOperation.columnChoice.size() - 1);
                string columnValueFromRow = getColumnValueFromRow(currentColumnName, parsedRowFromFile, tableName, sortedTabColumns);
                cout << columnValueFromRow;
                if (isLastColumnName) {
                    cout << endl;
                    break;
                }
                cout << ",";
            }
        }
    }
    else if (selectOperation.isInclusionList == false) {
        string tableName = selectOperation.tableName;
        for (int i = 0; i < allParsedRowsPlusAllInfo.size(); i++) {
            vector<string> parsedRowFromFile = allParsedRowsPlusAllInfo.at(i).parsedRowFromFile;
            for (int j = 0; j < sortedTabColumns.size(); j++) {
                string currentColumnName = sortedTabColumns.at(j).columnName;
                bool isLastColumn = (j == sortedTabColumns.size() - 1);
                string columnValueFromRow = getColumnValueFromRow(currentColumnName, parsedRowFromFile, tableName, sortedTabColumns);
                cout << columnValueFromRow;
                if (isLastColumn) {
                    cout << endl;
                    break;
                }
                cout << ",";
            }
        }
    }
    // cout << endl;
    return;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // We exit since this program requires 2 arguments, the Executable, and the Tuple
        // Classification number. 
        printf("Please enter only 2 arguments on command line, the Executable, and the TC(Tuple Classification number).\n");
        return -1;
    }
    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 4) {
        // We are expecting the TC number to be in the range between 1-4, if it is not then exit.
        printf("Please enter a number (1-4) for the Tuple Classification.\n");
        return -1;
    }
    // Tuple Classification number
    int tc = atoi(argv[1]); 

    ifstream tab_columns_file("TAB_COLUMNS.csv");
    // Checks to make sure the file opened successfully.
    if (tab_columns_file.is_open() == false) { 
        printf("File could not be opened.\n");
        return -1;
    }

    vector<TableColumnsObject> tab_columns;
    string text;
    // Storing every field from the tab columns CSV file into our tab columns vector of 
    // tab columns objects.
    while (getline(tab_columns_file, text)) {
        TableColumnsObject tableColumnFields = parseAndStoreTableColumnFields(text);
        tab_columns.push_back(tableColumnFields);
    }
    tab_columns_file.close();
    
    cout << "MLS> ";
    while (getline(cin, text)) {
        if (text == "EXIT" || text == "exit") {
            return 1;
        }
        SelectOperationFields selectOperation;
        // call some function that handles everything needed for a select operation.
        selectOperation = parseAndStoreSelectOperationFields(text, tab_columns);
        // now call some function that processes select statements.
        processSelectOperation(selectOperation, tab_columns, tc);
        cout << "MLS> ";
    }
   return 0;
}