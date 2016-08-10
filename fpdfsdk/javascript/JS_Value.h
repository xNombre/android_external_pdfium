// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FPDFSDK_JAVASCRIPT_JS_VALUE_H_
#define FPDFSDK_JAVASCRIPT_JS_VALUE_H_

#include <vector>

#include "core/fxcrt/include/fx_basic.h"
#include "fxjs/include/fxjs_v8.h"

class CJS_Array;
class CJS_Date;
class CJS_Document;
class CJS_Object;
class CJS_Runtime;

class CJS_Value {
 public:
  enum Type {
    VT_unknown,
    VT_string,
    VT_number,
    VT_boolean,
    VT_date,
    VT_object,
    VT_null,
    VT_undefined
  };

  explicit CJS_Value(CJS_Runtime* pRuntime);
  CJS_Value(CJS_Runtime* pRuntime, v8::Local<v8::Value> pValue);
  CJS_Value(CJS_Runtime* pRuntime, const int& iValue);
  CJS_Value(CJS_Runtime* pRuntime, const double& dValue);
  CJS_Value(CJS_Runtime* pRuntime, const float& fValue);
  CJS_Value(CJS_Runtime* pRuntime, const bool& bValue);
  CJS_Value(CJS_Runtime* pRuntime, CJS_Object* pObj);
  CJS_Value(CJS_Runtime* pRuntime, const FX_CHAR* pStr);
  CJS_Value(CJS_Runtime* pRuntime, const FX_WCHAR* pWstr);
  CJS_Value(CJS_Runtime* pRuntime, const CJS_Array& array);
  CJS_Value(CJS_Runtime* pRuntime, const CJS_Date& date);
  CJS_Value(CJS_Runtime* pRuntime, const CJS_Object* object);
  CJS_Value(const CJS_Value& other);

  ~CJS_Value();

  void SetNull(CJS_Runtime* pRuntime);
  void SetValue(const CJS_Value& other);
  void Attach(v8::Local<v8::Value> pValue);
  void Detach();

  static Type GetValueType(v8::Local<v8::Value> value);
  Type GetType() const { return GetValueType(m_pValue); }

  int ToInt(v8::Isolate* pIsolate) const;
  bool ToBool(v8::Isolate* pIsolate) const;
  double ToDouble(v8::Isolate* pIsolate) const;
  float ToFloat(v8::Isolate* pIsolate) const;
  CJS_Object* ToCJSObject(v8::Isolate* pIsolate) const;
  CFX_WideString ToCFXWideString(v8::Isolate* pIsolate) const;
  CFX_ByteString ToCFXByteString(v8::Isolate* pIsolate) const;
  v8::Local<v8::Object> ToV8Object(v8::Isolate* pIsolate) const;
  v8::Local<v8::Array> ToV8Array(v8::Isolate* pIsolate) const;
  v8::Local<v8::Value> ToV8Value(v8::Isolate* pIsolate) const;

  // Replace the current |m_pValue| with a v8::Number if possible
  // to make one from the current |m_pValue|.
  void MaybeCoerceToNumber(v8::Isolate* pIsolate);

  bool IsArrayObject() const;
  bool IsDateObject() const;
  bool ConvertToArray(v8::Isolate* pIsolate, CJS_Array&) const;
  bool ConvertToDate(v8::Isolate* pIsolate, CJS_Date&) const;

 protected:
  v8::Local<v8::Value> m_pValue;
};

class CJS_PropValue {
 public:
  explicit CJS_PropValue(CJS_Runtime* pRuntime);
  CJS_PropValue(CJS_Runtime* pRuntime, const CJS_Value&);
  ~CJS_PropValue();

  void StartSetting() { m_bIsSetting = true; }
  void StartGetting() { m_bIsSetting = false; }
  bool IsSetting() const { return m_bIsSetting; }
  bool IsGetting() const { return !m_bIsSetting; }
  CJS_Runtime* GetJSRuntime() const { return m_pJSRuntime; }
  CJS_Value* GetJSValue() { return &m_Value; }

  void operator<<(int val);
  void operator>>(int&) const;
  void operator<<(bool val);
  void operator>>(bool&) const;
  void operator<<(double val);
  void operator>>(double&) const;
  void operator<<(CJS_Object* pObj);
  void operator>>(CJS_Object*& ppObj) const;
  void operator<<(CJS_Document* pJsDoc);
  void operator>>(CJS_Document*& ppJsDoc) const;
  void operator<<(CFX_ByteString);
  void operator>>(CFX_ByteString&) const;
  void operator<<(CFX_WideString);
  void operator>>(CFX_WideString&) const;
  void operator<<(const FX_WCHAR* c_string);
  void operator<<(v8::Local<v8::Object>);
  void operator>>(v8::Local<v8::Object>&) const;
  void operator>>(CJS_Array& array) const;
  void operator<<(CJS_Array& array);
  void operator<<(CJS_Date& date);
  void operator>>(CJS_Date& date) const;

 private:
  bool m_bIsSetting;
  CJS_Value m_Value;
  CJS_Runtime* const m_pJSRuntime;
};

class CJS_Array {
 public:
  CJS_Array();
  CJS_Array(const CJS_Array& other);
  virtual ~CJS_Array();

  void Attach(v8::Local<v8::Array> pArray);
  void GetElement(v8::Isolate* pIsolate,
                  unsigned index,
                  CJS_Value& value) const;
  void SetElement(v8::Isolate* pIsolate,
                  unsigned index,
                  const CJS_Value& value);
  int GetLength() const;

  v8::Local<v8::Array> ToV8Array(v8::Isolate* pIsolate) const;

 private:
  mutable v8::Local<v8::Array> m_pArray;
};

class CJS_Date {
 public:
  CJS_Date();
  CJS_Date(v8::Isolate* pIsolate, double dMsec_time);
  CJS_Date(v8::Isolate* pIsolate,
           int year,
           int mon,
           int day,
           int hour,
           int min,
           int sec);
  virtual ~CJS_Date();

  void Attach(v8::Local<v8::Date> pDate);
  bool IsValidDate(v8::Isolate* pIsolate) const;

  int GetYear(v8::Isolate* pIsolate) const;
  void SetYear(v8::Isolate* pIsolate, int iYear);

  int GetMonth(v8::Isolate* pIsolate) const;
  void SetMonth(v8::Isolate* pIsolate, int iMonth);

  int GetDay(v8::Isolate* pIsolate) const;
  void SetDay(v8::Isolate* pIsolate, int iDay);

  int GetHours(v8::Isolate* pIsolate) const;
  void SetHours(v8::Isolate* pIsolate, int iHours);

  int GetMinutes(v8::Isolate* pIsolate) const;
  void SetMinutes(v8::Isolate* pIsolate, int minutes);

  int GetSeconds(v8::Isolate* pIsolate) const;
  void SetSeconds(v8::Isolate* pIsolate, int seconds);

  v8::Local<v8::Date> ToV8Date(v8::Isolate* pIsolate) const;
  double ToDouble(v8::Isolate* pIsolate) const;
  CFX_WideString ToString(v8::Isolate* pIsolate) const;

 protected:
  v8::Local<v8::Date> m_pDate;
};

double JS_GetDateTime();
int JS_GetYearFromTime(double dt);
int JS_GetMonthFromTime(double dt);
int JS_GetDayFromTime(double dt);
int JS_GetHourFromTime(double dt);
int JS_GetMinFromTime(double dt);
int JS_GetSecFromTime(double dt);
double JS_DateParse(const CFX_WideString& str);
double JS_MakeDay(int nYear, int nMonth, int nDay);
double JS_MakeTime(int nHour, int nMin, int nSec, int nMs);
double JS_MakeDate(double day, double time);
bool JS_PortIsNan(double d);
double JS_LocalTime(double d);

// Some JS methods have the bizarre convention that they may also be called
// with a single argument which is an object containing the actual arguments
// as its properties. The varying arguments to this method are the property
// names as wchar_t string literals corresponding to each positional argument.
// The result will always contain |nKeywords| value, with unspecified ones
// being set to type VT_unknown.
std::vector<CJS_Value> JS_ExpandKeywordParams(
    CJS_Runtime* pRuntime,
    const std::vector<CJS_Value>& originals,
    size_t nKeywords,
    ...);

#endif  // FPDFSDK_JAVASCRIPT_JS_VALUE_H_
