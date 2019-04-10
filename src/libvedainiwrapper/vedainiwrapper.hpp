#ifndef VEDAINIWRAPPER_H
#define VEDAINIWRAPPER_H
/*!
    \file
    \brief Класс для работы с INI файлами
    \details Класс для работы с INI файлами
    \author Veda team
    \version 1.0
    \date Июль 2018 года
*/
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include<cstdlib>

using namespace std;

struct VedaIniEntry{
    VedaIniEntry():isComment(false){}
    void clear(){index = ""; name = ""; value = ""; comment = ""; isComment = false;}

    string index;
    string name;
    string value;
    string comment;
    bool isComment;
};

class VedaIniWrapper
{
public:

    /// \brief Открытие INI файла
    /// \details Открытие INI файла
    /// \param[in]  fileName имя файла
    /// \param[in]  autoCreate создавать файл или нет (при его отсутствии)
    /*! \code
    \endcode
    */
    VedaIniWrapper(const char *fileName, bool autoCreate=false);

    /// \brief Запись созданных секций в файл
    /// \details Запись созданных секций в файл
    /// \param[in]  fileName имя файла
    /*! \code
    \endcode
    */
    void writeConfigFile(const char *fileName=NULL);

    ~VedaIniWrapper();

    /// \brief Получение значения из INI файла
    /// \details Получение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \return false, в случае успеха - содержимое секции
    /*! \code
    \endcode
    */
    bool getBoolValue(const char* index, const char *name);    

    /// \brief Получение значения из INI файла
    /// \details Получение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \return -1, в случае успеха - содержимое секции
    /*! \code
    \endcode
    */
    int getIntValue(const char* index, const char *name);

    /// \brief Получение значения из INI файла
    /// \details Получение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \return -1.0 , в случае успеха - содержимое секции
    /*! \code
    \endcode
    */
    float getFloatValue(const char* index, const char *name);

    /// \brief Получение значения из INI файла
    /// \details Получение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \return NULL, в случае успеха - содержимое секции
    /*! \code
    \endcode
    */
    const char* getStringValue(const char* index, const char *name);


    /// \brief Получение содержимого секции по ее имени
    /// \details Получение содержимого секции по ее имени
    /// \param[in]  entryName наименование секции
    /// \param[out]  entryList Вектор секций
    /// \return 0, если ничего не найдено, в случае успеха - количество секций
    /*! \code
    \endcode
    */
    int getAllEntrysByName(std::string entryName, vector<VedaIniEntry> &entryList);

    /// \brief Присвоение значения из INI файла
    /// \details Присвоение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \param[in]  value значение параметра
    /*! \code
    \endcode
    */
    void setBoolValue(const char* index, const char *name, bool value);


    /// \brief Присвоение значения из INI файла
    /// \details Присвоение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \param[in]  value значение параметра
    /*! \code
    \endcode
    */
    void setIntValue(const char* index, const char *name, int value);


    /// \brief Присвоение значения из INI файла
    /// \details Присвоение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \param[in]  value значение параметра
    /*! \code
    \endcode
    */
    void setFloatValue(const char* index, const char *name, float value);

    /// \brief Присвоение значения из INI файла
    /// \details Присвоение значения из INI файла
    /// \param[in]  index наименование секции
    /// \param[in]  name наименование параметра
    /// \param[in]  value значение параметра
    /*! \code
    \endcode
    */
    void setStringValue(const char *index, const char* name, const char* value);

    // для тестирования
    void printAll();
    
    // все секции
    vector<VedaIniEntry> datas;

private:
    char str[4096]; //для временных данных
    void setStringValueWithIndex(const char *index, const char* name, const char* value);
    char iniFileName[4096];
    char *data;
    const char (*lineData)[4096];
    void loadConfigFile();
    fstream *fStream;
    bool autoSave;
    bool autoCreate;
};

#endif // VEDAINIWRAPPER_H

