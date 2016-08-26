
#pragma once

#include "UniVar.h"
#include <memory>
#include <stdarg.h>
#include <assert.h>

#define ME			(*MY)

#define error		1
#define trace		2
#define info		3
#define warnings	4

#define ASSERT(b, fileName, line)				zassert(CUniVar(b).ToBoolean(), fileName, line);
#define ASSERT_EXT(b, text, fileName, line)		zassert(CUniVar(b).ToBoolean(), text.ToString(), fileName, line);

#define print(v)	print_((v).InitByDefaultIfNeeded()) 
#define println(v)	println_((v).InitByDefaultIfNeeded())

#define print2(v1, v2)		print_(v1, (v2).InitByDefaultIfNeeded())
#define print3(v1, v2, v3)	print_(v1, (v2).InitByDefaultIfNeeded(), v3)

#define println2(v1, v2)		println_(v1, (v2).InitByDefaultIfNeeded())
#define println3(v1, v2, v3)	println_(v1, (v2).InitByDefaultIfNeeded(), v3)

extern unsigned int currentLine;
extern int currentClass;
extern unsigned int currentMethod;
#define LN(line) currentLine = line;
#define CMN(classID, methodID) currentClass = classID; currentMethod = methodID;

CUniVar print_(const CUniVar& var);
CUniVar print_(CUniVar stream, CUniVar var);
CUniVar print_(CUniVar stream, CUniVar var, CUniVar& byteCount);

CUniVar println_(const CUniVar& var);
CUniVar println_(CUniVar stream, CUniVar var);
CUniVar println_(CUniVar stream, CUniVar var, CUniVar& byteCount);

CUniVar read(CUniVar& var);
CUniVar read(CUniVar stream, CUniVar& var);
CUniVar read(CUniVar stream, CUniVar& var, CUniVar& byteCount);

CUniVar append(CUniVar& var);
CUniVar append(CUniVar stream, CUniVar& var);

CUniVar openStream(CUniVar path, CUniVar streamType);
CUniVar createStream(CUniVar path, CUniVar streamType);
CUniVar clearStream(CUniVar type);
CUniVar seekStream(CUniVar stream, CUniVar shift);

void zassert(bool cond, const std::string& fileName, const unsigned int line);
void zassert(bool cond, const std::string& errorText, const std::string& fileName, const unsigned int line);

CUniVar getResultObject(bool resultFlag = false);
CUniVar getFullResultObject(bool resultFlag);
CUniVar getResultObject(bool resultFlag, CUniVar& result);
CUniVar getResultObjectV(bool resultFlag, CUniVar result);

CUniVar parseInt(const CUniVar& var);
CUniVar parseFloat(const CUniVar& var);
CUniVar parseBoolean(const CUniVar& var);

std::string getCurrentMethodName();

void unset(CUniVar& a);
CUniVar typeof(CUniVar& a);