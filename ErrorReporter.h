#pragma once

enum class ErrorCodeType { Nonzero, False, Custom };
enum class ErrorReportType { Error, Warning };

void Report(ErrorReportType ReportType, int ErrorCode, ErrorCodeType ErrorCodeType,
    const wchar_t* FileName, size_t LineNumber, const wchar_t* Function, const wchar_t* Command, const wchar_t* Message);



// Macro Bodies
#define _NONZERO(x, report_type, msg) \
do \
{ \
    int __code = (int)(x); \
if (__code) \
    ::Report(report_type, __code, ::ErrorCodeType::Nonzero, __FILEW__, __LINE__, __FUNCTIONW__, L#x, msg); \
} \
while (false)

#define _FALSE(x, report_type, msg) \
do \
{ \
    int __code = (int)(x); \
if (!(__code)) \
    ::Report(report_type, __code, ::ErrorCodeType::False, __FILEW__, __LINE__, __FUNCTIONW__, L#x, msg); \
} \
while (false)

#define _CUSTOM(x, report_type, msg) \
    ::Report(report_type, 0, ::ErrorCodeType::Custom, __FILEW__, __LINE__, __FUNCTIONW__, L"", msg);



// Interface Macros
#define NONZERO_ERROR_MSG(x, msg) _NONZERO(x, ::ErrorReportType::Error, msg)
#define NONZERO_ERROR(x) NONZERO_ERROR_MSG(x, nullptr)
#define NONZERO_WARNING_MSG(x, msg) _NONZERO(x, ::ErrorReportType::Warning, msg)
#define NONZERO_WARNING(x) NONZERO_WARNING_MSG(x, nullptr)

#define FALSE_ERROR_MSG(x, msg) _FALSE(x, ::ErrorReportType::Error, msg)
#define FALSE_ERROR(x) FALSE_ERROR_MSG(x, nullptr)
#define FALSE_WARNING_MSG(x, msg) _FALSE(x, ::ErrorReportType::Warning, msg)
#define FALSE_WARNING(x) FALSE_WARNING_MSG(x, nullptr)

#define ERROR_MSG(msg) _CUSTOM(0, ::ErrorReportType::Error, msg)
#define WARNING_MSG(msg) _CUSTOM(0, ::ErrorReportType::Warning, msg)
