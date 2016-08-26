
#include "ClassDeclarations.h"
#include "StandardFuncs.h"
#include "ArraySupport.h"
#include "HashtableSupport.h"
#include "LogManager.h"
#include "Utils.h"
 
CUniVar logInfoProxy(CClassDataContainer* MY, CUniVar* op, CUniVar* val) 
{ 
	return logInfo(MY, *op, *val); 
} 
 
CUniVar logInfo(CClassDataContainer* MY, CUniVar& op, CUniVar& val)
{
	CUniVar result = getResultObject(false); LN(7); CMN(-1, __FUNC_ENUM__logInfo); 
	try { 
	print(op); LN(3);
	::print(CUniVar(" => ")); LN(4);
	::println(typeof(val)); LN(5);
	return getResultObject(true);
	} catch (std::exception& e) { Log.Warnings.AddLine2s("Function logInfo interrupted after line %s: %s", currentLine, e.what()); return getResultObject(false); }; return result; 
}
CUniVar EntryPointProto::run(EntryPointProto* MY)
{
	CUniVar result = getResultObject(false); LN(9); CMN(6, __FUNC_ENUM__run); 
	try { 
	CUniVar a = CUniVar(1.1); LN(11);
	// it is normal to save integer to double

	a = CUniVar(1); LN(13);
	println(a); LN(14);
	CUniVar a1; 
	a1++; LN(15);
	::logInfoProxy(MY, (CUniVar("++")).GetPtr(), (a1).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(17);
	CUniVar a2; 
	a2--; LN(17);
	::logInfoProxy(MY, (CUniVar("--")).GetPtr(), (a2).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(19);
	CUniVar a3; 
	a3 += CUniVar(2); LN(20);
	::logInfoProxy(MY, (CUniVar("+=")).GetPtr(), (a3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(21);
	CUniVar a4; 
	a4 *= CUniVar(2); LN(22);
	::logInfoProxy(MY, (CUniVar("*=")).GetPtr(), (a4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(23);
	CUniVar a5; 
	a5 /= CUniVar(2); LN(24);
	::logInfoProxy(MY, (CUniVar("/=")).GetPtr(), (a5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(25);
	CUniVar a6; 
	a6 -= CUniVar(2); LN(26);
	::logInfoProxy(MY, (CUniVar("-=")).GetPtr(), (a6).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(27);
	CUniVar a7; 
	a7 ^= CUniVar(101); LN(28);
	::logInfoProxy(MY, (CUniVar("^=")).GetPtr(), (a7).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(29);
	CUniVar a8; 
	a8 |= CUniVar(101); LN(30);
	::logInfoProxy(MY, (CUniVar("|=")).GetPtr(), (a8).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(31);
	CUniVar a9; 
	a9 ^= CUniVar(101); LN(32);
	::logInfoProxy(MY, (CUniVar("&=")).GetPtr(), (a9).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(33);
	CUniVar b1 = a3 + CUniVar(1); LN(35);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (b1).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(36);
	CUniVar b2 = a3 * CUniVar(2); LN(37);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (b2).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(38);
	CUniVar b3 = a3 / CUniVar(2); LN(39);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (b3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(40);
	CUniVar b4 = a3 - CUniVar(9); LN(41);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (b4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(42);
	CUniVar c1; 
	c1.CallMethod__(4, __FUNC_ENUM__push, false, 0, (CUniVar()).GetPtr()); c1 [ c1->GetProperty__(0, __PROP_ENUM__length) - CUniVar(1) ] = CUniVar(1); LN(44);
	::logInfoProxy(MY, (CUniVar("[]")).GetPtr(), (c1).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(45);
	CUniVar c2; 
	c2->CallMethod__(4, __FUNC_ENUM__push, 1, 0, (CUniVar(1)).GetPtr()); LN(46);
	::logInfoProxy(MY, (CUniVar("push")).GetPtr(), (c2).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(47);
	//			logInfo("clear" c3)

	CUniVar c4; 
	c4->CallMethod__(4, __FUNC_ENUM__resize, 1, 0, (CUniVar(2)).GetPtr()); LN(50);
	::logInfoProxy(MY, (CUniVar("resize")).GetPtr(), (c4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(51);
	CUniVar c5; 
	CUniVar cc5 = c5->CallMethod__(4, __FUNC_ENUM__implode, 1, 0, (CUniVar(",")).GetPtr()); LN(52);
	::logInfoProxy(MY, (CUniVar("implode")).GetPtr(), (cc5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(53);
	//			logInfo("insert" c6)

	CUniVar c7; 
	c7->CallMethod__(5, __FUNC_ENUM__extractNumeric, 1, 0, (CUniVar(0)).GetPtr(), (CUniVar(4)).GetPtr()); LN(56);
	::logInfoProxy(MY, (CUniVar("extractNumeric")).GetPtr(), (c7).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(57);
	CUniVar c8; 
	c8->CallMethod__(5, __FUNC_ENUM__extractFloat, 1, 0, (CUniVar(0)).GetPtr(), (CUniVar(4)).GetPtr()); LN(58);
	::logInfoProxy(MY, (CUniVar("extractFloat")).GetPtr(), (c8).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(59);
	CUniVar c9; 
	c9->CallMethod__(5, __FUNC_ENUM__extractBoolean, 1, 0, (CUniVar(0)).GetPtr(), (CUniVar(1)).GetPtr()); LN(60);
	::logInfoProxy(MY, (CUniVar("extractBoolean")).GetPtr(), (c9).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(61);
	CUniVar c10; 
	c10->CallMethod__(5, __FUNC_ENUM__extractString, 1, 0, (CUniVar(0)).GetPtr(), (CUniVar(4)).GetPtr()); LN(62);
	::logInfoProxy(MY, (CUniVar("extractString")).GetPtr(), (c10).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(63);
	CUniVar c11; 
	c11->CallMethod__(5, __FUNC_ENUM__binarizeNumeric, 1, 0, (CUniVar(0)).GetPtr(), (CUniVar(4)).GetPtr()); LN(64);
	::logInfoProxy(MY, (CUniVar("binarizeNumeric")).GetPtr(), (c11).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(65);
	CUniVar c12; 
	c12->CallMethod__(5, __FUNC_ENUM__binarizeFloat, 1, 0, (CUniVar(0.0)).GetPtr(), (CUniVar(4)).GetPtr()); LN(66);
	::logInfoProxy(MY, (CUniVar("binarizeFloat")).GetPtr(), (c12).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(67);
	CUniVar c13; 
	c13->CallMethod__(5, __FUNC_ENUM__binarizeBoolean, 1, 0, (CUniVar(false)).GetPtr(), (CUniVar(1)).GetPtr()); LN(68);
	::logInfoProxy(MY, (CUniVar("binarizeBoolean")).GetPtr(), (c13).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(69);
	CUniVar c14; 
	c14->CallMethod__(4, __FUNC_ENUM__binarizeString, 1, 0, (CUniVar("abcd")).GetPtr()); LN(70);
	::logInfoProxy(MY, (CUniVar("binarizeString")).GetPtr(), (c14).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(71);
	CUniVar c15; 
	c15->CallMethod__(5, __FUNC_ENUM__cut, 1, 0, (CUniVar(0)).GetPtr(), (CUniVar(1)).GetPtr()); LN(72);
	::logInfoProxy(MY, (CUniVar("cut")).GetPtr(), (c15).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(73);
	//			logInfo("erase" c16)


	CUniVar d1; 
	::logInfoProxy(MY, (CUniVar("[nop]")).GetPtr(), (d1).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(77);
	CUniVar d2; 
	d2->GetProperty__(0, __PROP_ENUM__x) = CUniVar(1); LN(78);
	::logInfoProxy(MY, (CUniVar("[]")).GetPtr(), (d2).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(79);
	CUniVar d3; 
	CUniVar tmp999 = d3->GetProperty__(0, __PROP_ENUM__length); LN(80);
	::logInfoProxy(MY, (CUniVar("[]")).GetPtr(), (d3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(81);
	CUniVar d4; 
	d4->CallMethod__(4, __FUNC_ENUM__convertable, 1, 0, (CUniVar("object")).GetPtr()); LN(82);
	::logInfoProxy(MY, (CUniVar("convertable")).GetPtr(), (d4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(83);
	CUniVar d5; 
	d5->CallMethod__(3, __FUNC_ENUM__clone, 1, 0); LN(84);
	::logInfoProxy(MY, (CUniVar("clone")).GetPtr(), (d5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(85);
	CUniVar e1; 
	e1[ CUniVar("hello") ] = CUniVar("world"); LN(87);
	::logInfoProxy(MY, (CUniVar("[\"hello\"]")).GetPtr(), (e1).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(88);
	CUniVar e2; 
	e2->CallMethod__(4, __FUNC_ENUM__keyExists, 1, 0, (CUniVar("key")).GetPtr()); LN(89);
	::logInfoProxy(MY, (CUniVar("keyExists")).GetPtr(), (e2).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(90);
	CUniVar e3; 
	e3->CallMethod__(4, __FUNC_ENUM__removeKey, 1, 0, (CUniVar("key")).GetPtr()); LN(91);
	::logInfoProxy(MY, (CUniVar("removeKey")).GetPtr(), (e3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(92);
	CUniVar e4; 
	e4->CallMethod__(3, __FUNC_ENUM__getValues, 1, 0); LN(93);
	::logInfoProxy(MY, (CUniVar("getValues")).GetPtr(), (e4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(94);
	CUniVar e5; 
	e5->CallMethod__(4, __FUNC_ENUM__keyExists, 1, 0, (CUniVar("getKeys")).GetPtr()); LN(95);
	::logInfoProxy(MY, (CUniVar("getKeys")).GetPtr(), (e5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(96);
	CUniVar f1; 
	f1 %= CUniVar("end"); LN(97);
	::logInfoProxy(MY, (CUniVar(".=")).GetPtr(), (f1).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(99);
	CUniVar f2; 
	f2 += CUniVar("end"); LN(100);
	::logInfoProxy(MY, (CUniVar("+=")).GetPtr(), (f2).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(101);
	CUniVar f3; 
	f3->CallMethod__(3, __FUNC_ENUM__trim, 1, 0); LN(102);
	::logInfoProxy(MY, (CUniVar("trim")).GetPtr(), (f3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(103);
	CUniVar f4; 
	f4->CallMethod__(3, __FUNC_ENUM__ltrim, 1, 0); LN(104);
	::logInfoProxy(MY, (CUniVar("ltrim")).GetPtr(), (f4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(105);
	CUniVar f5; 
	f5->CallMethod__(3, __FUNC_ENUM__rtrim, 1, 0); LN(106);
	::logInfoProxy(MY, (CUniVar("rtrim")).GetPtr(), (f5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(107);
	CUniVar f6; 
	f6->CallMethod__(5, __FUNC_ENUM__replace, 1, 0, (CUniVar("1")).GetPtr(), (CUniVar("2")).GetPtr()); LN(108);
	::logInfoProxy(MY, (CUniVar("replace")).GetPtr(), (f6).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(109);
	CUniVar f7; 
	f7->CallMethod__(4, __FUNC_ENUM__explode, 1, 0, (CUniVar(".")).GetPtr()); LN(110);
	::logInfoProxy(MY, (CUniVar("explode")).GetPtr(), (f7).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(111);
	CUniVar f8; 
	f8->CallMethod__(5, __FUNC_ENUM__substr, 1, 0, (CUniVar(0)).GetPtr(), (CUniVar(1)).GetPtr()); LN(112);
	::logInfoProxy(MY, (CUniVar("substr")).GetPtr(), (f8).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(113);
	CUniVar f9; 
	f9->CallMethod__(4, __FUNC_ENUM__regexMatch, 1, 0, (CUniVar("[0-9]")).GetPtr()); LN(114);
	::logInfoProxy(MY, (CUniVar("regexMatch")).GetPtr(), (f9).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(115);
	CUniVar f10; 
	f10->CallMethod__(4, __FUNC_ENUM__regexSearch, 1, 0, (CUniVar("[0-9]")).GetPtr()); LN(116);
	::logInfoProxy(MY, (CUniVar("regexSearch")).GetPtr(), (f10).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(117);
	CUniVar f11; 
	f11->CallMethod__(5, __FUNC_ENUM__regexReplace, 1, 0, (CUniVar("[0-9]")).GetPtr(), (CUniVar("!")).GetPtr()); LN(118);
	::logInfoProxy(MY, (CUniVar("regexReplace")).GetPtr(), (f11).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(119);
	CUniVar options(new Object()); LN(120);
	options->GetProperty__(0, __PROP_ENUM__icase) = CUniVar(false); LN(121);
	CUniVar f12; 
	f12->CallMethod__(5, __FUNC_ENUM__regexMatch, 1, 0, (CUniVar("[0-9]")).GetPtr(), (options).GetPtr()); LN(122);
	::logInfoProxy(MY, (CUniVar("regexMatch+")).GetPtr(), (f12).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(123);
	CUniVar f13; 
	f13->CallMethod__(5, __FUNC_ENUM__regexSearch, 1, 0, (CUniVar("[0-9]")).GetPtr(), (options).GetPtr()); LN(124);
	::logInfoProxy(MY, (CUniVar("regexSearch+")).GetPtr(), (f13).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(125);
	CUniVar f14; 
	f14->CallMethod__(6, __FUNC_ENUM__regexReplace, 1, 0, (CUniVar("[0-9]")).GetPtr(), (CUniVar("!")).GetPtr(), (options).GetPtr()); LN(126);
	::logInfoProxy(MY, (CUniVar("regexReplace+")).GetPtr(), (f14).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(127);
	CUniVar g1; 
	g1 ^= CUniVar(false); LN(129);
	::logInfoProxy(MY, (CUniVar("^=")).GetPtr(), (g1).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(130);
	CUniVar g2; 
	g2 |= CUniVar(false); LN(131);
	::logInfoProxy(MY, (CUniVar("|=")).GetPtr(), (g2).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(132);
	CUniVar g3; 
	g3 ^= CUniVar(false); LN(133);
	::logInfoProxy(MY, (CUniVar("&=")).GetPtr(), (g3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(134);
	CUniVar g4; 
	g4 *= CUniVar(true); LN(135);
	::logInfoProxy(MY, (CUniVar("^=")).GetPtr(), (g4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(136);
	CUniVar g5; 
	g5 /= CUniVar(true); LN(137);
	::logInfoProxy(MY, (CUniVar("^=")).GetPtr(), (g5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(138);
	CUniVar g6; 
	g6 += CUniVar(true); LN(139);
	::logInfoProxy(MY, (CUniVar("|=")).GetPtr(), (g6).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(140);
	CUniVar g7; 
	g7 -= CUniVar(true); LN(141);
	::logInfoProxy(MY, (CUniVar("&=")).GetPtr(), (g7).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(142);
	CUniVar h3; 
	h3 += CUniVar(2.1); LN(144);
	::logInfoProxy(MY, (CUniVar("+=")).GetPtr(), (h3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(145);
	CUniVar h4; 
	h4 *= CUniVar(2.1); LN(146);
	::logInfoProxy(MY, (CUniVar("*=")).GetPtr(), (h4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(147);
	CUniVar h5; 
	h5 /= CUniVar(2.1); LN(148);
	::logInfoProxy(MY, (CUniVar("/=")).GetPtr(), (h5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(149);
	CUniVar h6; 
	h6 -= CUniVar(2.1); LN(150);
	::logInfoProxy(MY, (CUniVar("-=")).GetPtr(), (h6).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(151);
	
	CUniVar h10 = h3 + CUniVar(1); LN(158);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h10).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(159);
	CUniVar h11 = h3 * CUniVar(2); LN(160);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h11).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(161);
	CUniVar h12 = h3 / CUniVar(2); LN(162);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h12).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(163);
	CUniVar h13 = h3 - CUniVar(9); LN(164);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h13).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(165);
	CUniVar h14 = a3 + CUniVar(1.0); LN(166);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h14).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(167);
	CUniVar h15 = a3 * CUniVar(2.0); LN(168);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h15).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(169);
	CUniVar h16 = a3 / CUniVar(2.0); LN(170);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h16).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(171);
	CUniVar h17 = a3 - CUniVar(9.0); LN(172);
	::logInfoProxy(MY, (CUniVar("int + 1")).GetPtr(), (h17).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(173);
	CUniVar k3; 
	k3 += CUniVar('a'); LN(175);
	::logInfoProxy(MY, (CUniVar("+=")).GetPtr(), (k3).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(176);
	CUniVar k4; 
	k4 *= CUniVar('a'); LN(177);
	::logInfoProxy(MY, (CUniVar("*=")).GetPtr(), (k4).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(178);
	CUniVar k5; 
	k5 /= CUniVar('a'); LN(179);
	::logInfoProxy(MY, (CUniVar("/=")).GetPtr(), (k5).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(180);
	CUniVar k6; 
	k6 -= CUniVar('a'); LN(181);
	::logInfoProxy(MY, (CUniVar("-=")).GetPtr(), (k6).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(182);
	CUniVar k7; 
	k7 ^= CUniVar('a'); LN(183);
	::logInfoProxy(MY, (CUniVar("^=")).GetPtr(), (k7).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(184);
	CUniVar k8; 
	k8 |= CUniVar('a'); LN(185);
	::logInfoProxy(MY, (CUniVar("|=")).GetPtr(), (k8).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(186);
	CUniVar k9; 
	k9 ^= CUniVar('a'); LN(187);
	::logInfoProxy(MY, (CUniVar("&=")).GetPtr(), (k9).GetPtr()).GetProperty__(1, __PROP_ENUM__result); LN(188);
	return getResultObject(true);
	} 
	catch (std::exception& e) 
	{ 
		Log.Warnings.AddLine2s("Function EntryPoint::run interrupted after line %s: %s", currentLine, e.what()); return getResultObject(false); 
	}; 
	return result; 
}
EntryPointProto::EntryPointProto() 
{ 
	const std::string className = "EntryPoint"; 
	GetProperty__(0, __PROP_ENUM__onCreated) = FunctionPointer(className, (int64_t)this, __FUNC_ENUM__onCreated); 
	GetProperty__(0, __PROP_ENUM__run) = FunctionPointer(className, (int64_t)this, __FUNC_ENUM__run); 
} 
 
CUniVar EntryPointProto::CallMethod___(EntryPointProto* MY, int count, ...) 
{ 
	va_list vl; 
	va_start(vl, count); 
	CUniVar result = CallMethod___(MY, count, vl); 
	va_end(vl);	 
	return result; 
} 
 
CUniVar EntryPointProto::CallMethod___(EntryPointProto* MY, int count, va_list& vl) 
{ 
	ClassMethodEnum method = (ClassMethodEnum)va_arg(vl, int); 
	bool defaultResultNeeded = (va_arg(vl, int) != 0);  
	int callStatus = va_arg(vl, int);  
	return CallMethod___(MY, count, vl, method, callStatus, defaultResultNeeded); 
}			 
 
CUniVar EntryPointProto::CallMethod___(EntryPointProto* MY, int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded) 
{ 
	CUniVar result; 
	if (CheckForBuiltInMethod(MY, method, count, vl, defaultResultNeeded, result))	 
	{  
		return result;  
	} 
	if ((method == __FUNC_ENUM__onCreated) && (count == 3) && (callStatus >= 0)) 
	{ 
		result = onCreated(MY);		 
		if (defaultResultNeeded) 
		{ 
			result.PermitChangeType(); 
			result = result.GetProperty__(1, __PROP_ENUM__result); 
			result.ProhibitChangeType(); 
		} 
	} 
	else if ((method == __FUNC_ENUM__run) && (count == 3) && (callStatus >= 0)) 
	{ 
		result = run(MY);		 
		if (defaultResultNeeded) 
		{ 
			result.PermitChangeType(); 
			result = result.GetProperty__(1, __PROP_ENUM__result); 
			result.ProhibitChangeType(); 
		} 
	} 
	else if (count == 1) 
	{ 
		return MY->GetProperty__(callStatus, (ClassPropertyEnum)method); 
	}			 
	else 
	{ 
		Log.Warnings.AddLine2s("Attempt to call unknown method %s which must use %s parameters", methodNames[(int)method], count - 3);  
	} 
	return result; 
} 
 
bool EntryPointProto::IsMethodExist(CUniVar _name) 
{ 
	std::string name = _name.ToString(); 
	if (name == "onCreated") return true; 
	if (name == "run") return true; 
 
	return false; 
}	 
 
CUniVar EntryPointProto::CallMethodByName___(EntryPointProto* MY, const std::string& name, CArrayDataContainer* adt) 
{ 
	CUniVar result = getResultObject(false); 
	if ((name == "onCreated") && (adt->GetLength() == 0)) 
	{ 
		result = onCreated(MY); 
	} 
	else if ((name == "run") && (adt->GetLength() == 0)) 
	{ 
		result = run(MY); 
	} 
	else  
	{ 
	} 
	return result; 
} 
 
int main(int argc, char* argv[]) 
{ 
	Log.SetCommandLine(argc, argv); 
	EntryPointProto entryPointObject; 
	EntryPointProto* entryPoint = &entryPointObject; 
 
	if ((argc == 5) && (strcmp(argv[1], "execute") == 0)) 
	{ 
		std::string className = argv[2]; 
		std::string methodName = argv[3]; 
		CArrayDataContainer* adt = new CArrayDataContainer(); 
		adt->UpdateStateFromJson(argv[4]); 
		if (className != ":global:") 
		{ 
			CClassDataContainer* cdt = GetClassInstance(argv[2]); 
			if (cdt) 
			{ 
				if (cdt->IsMethodExist(methodName)) 
				{ 
					println(cdt->CallMethodByName__(methodName, adt)); 
				} 
				else 
				{ 
					println(CUniVar("Method cannot be found")); 
				} 
			} 
			else 
			{ 
				println(CUniVar("Class cannot be found")); 
			} 
		} 
		else 
		{ 
			if (IsGlobalFunctionExist(methodName)) 
			{ 
				CClassDataContainer cdt; 
				println(CallGlobalFunctionByName__(&cdt, methodName, adt));				 
			} 
			else 
			{ 
				println(CUniVar("Global function cannot be found"));
			} 
		} 
		delete adt; 
	} 
	else 
	{ 
		CArrayDataContainer* cmdLineParams = new CArrayDataContainer(); 
		for (int i = 1; i < argc; i++) 
		{       	 
			cmdLineParams->Push(argv[i]); 
		} 
 
		entryPoint->GetProperty__(1, __PROP_ENUM__commandLineParams) = CUniVar(cmdLineParams); 
		entryPoint->GetProperty__(1, __PROP_ENUM__processPath) = CUniVar(argv[0]);  
		std::string exeDir, exeName;
		ParseExePath(argv[0], exeDir, exeName);		
		entryPoint->GetProperty__(1, __PROP_ENUM__processName) = CUniVar(exeName);  
		entryPoint->GetProperty__(1, __PROP_ENUM__processFolder) = CUniVar(exeDir);  
		entryPoint->CallMethod___(entryPoint, 3, __FUNC_ENUM__run, true, 0);  
		
	} 
	return 0; 
}