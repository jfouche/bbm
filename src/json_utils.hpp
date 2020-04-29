#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <QJsonObject>
#include <QJsonArray>
#include <string>

namespace json {

class missing_field_exception : public std::exception
{
    std::string m_what;

public:
    missing_field_exception(const QString& field): m_what("Missing field : ") {
        m_what.append(field.toStdString().c_str());
    }
    virtual ~missing_field_exception() {}
    const char* what() const noexcept override { return m_what.c_str(); }
};

class invalid_type_exception : public std::exception
{
    std::string m_what;

public:
    invalid_type_exception(const QString& field): m_what("Invalid field type : ") {
        m_what.append(field.toStdString().c_str());
    }
    virtual ~invalid_type_exception() {}
    const char* what() const noexcept override { return m_what.c_str(); }
};

struct key_value {
    const QString field;
    const QJsonValue value;

    key_value(const QString& _field, const QJsonValue& _value)
        : field(_field)
        , value(_value)
    {}

    const QJsonValue& operator()() { return value; }
};

key_value get(const QJsonObject& jsonObj, const QString& field)
{
    if (!jsonObj.contains(field))
        throw new missing_field_exception(field);
    return key_value(field, jsonObj[field]);
}

template<class T>
T as(const key_value& pair);

template<class T>
T as(const QJsonObject& jsonObj, const QString& field)
{
    return json::as<T>(json::get(jsonObj, field));
}

template<>
QString as(const key_value& pair)
{
    if (!pair.value.isString()) {
        throw new invalid_type_exception(pair.field);
    }
    return pair.value.toString();
}

template<>
QJsonArray as(const key_value& pair)
{
    if (!pair.value.isArray()) {
        throw new invalid_type_exception(pair.field);
    }
    return pair.value.toArray();
}

template<>
QJsonObject as(const key_value& pair)
{
    if (!pair.value.isObject()) {
        throw new invalid_type_exception(pair.field);
    }
    return pair.value.toObject();
}

} // namespace json

#endif // JSON_UTILS_HPP
