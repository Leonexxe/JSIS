/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/compile.cpp                                                            #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Friday, August 26th 2022, 1:57:53 am                                                         #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
############################################################################################################*/
#pragma once
#include "master.h"

#define EOO 1000000

string _namespace_ = "";
u64 EON = 0;
enum objType
{
    _type,_variable,_function
};

struct type;
struct var;
struct function;

enum scopeType
{
    st_namespace,st_class,st_function
};

struct scope
{
    u64 returnType = 0;
    u64 vCounter = 0;
    string name = "";
    scopeType scopeT = st_namespace;
    scope* parent = nullptr;
};

struct flags
{
    bool _engine = 0;
    bool _static = 0;
    bool _const = 0;
};
struct obj
{
    string name;
    u64 number;
    u64 subNumber;
    objType ObjType;
    flags fl;
    u64 T;

    obj(){}
    obj(string name,u64 number,u64 subNumber,u64 T,objType ObjType)
        :name(name),number(number),subNumber(subNumber),T(T),ObjType(ObjType){}
};

flags currentFlags;


bool isKeyWord(string s)
{
    if(s == "engine")
    {
        currentFlags._engine = 1;
        return 1;
    }
    if(s == "static")
    {
        currentFlags._static = 1;
        return 1;
    }
    if(s == "const")
    {
        currentFlags._const = 1;
        return 1;
    }
    return 0;
}

struct codeFile
{
    codeFile(string name,u64 line)
        :name(name),line(line){}
    string name;
    i64 line;
};
list<codeFile> CFlist;

struct function : public obj
{
    using obj::obj;
    list<var> args;
};

struct var : public obj
{
    using obj::obj;
};

struct type : public obj
{
    using obj::obj;

    list<u64> memberFunctions;
    list<var> memberVariables;
};

struct objD
{
    objType ot;
    u64 subNumber;
    u64 objNumber;

    objD(objType ot,u64 subNumber,u64 objNumber)
        :ot(ot),subNumber(subNumber),objNumber(objNumber){}
};

u64 LDT = 0;
u64 LDF = 0;
u64 cvn = 0;
bool isClassBody = false;
bool isFunctionBody = false;
list<function>  functions;
list<type>      types;
list<var>       vars;
list<objD>      objects;

u64 objNumber = EOO;//first n objects are defined and implemented by the engine

list<scope> scopeStack;
u64 LDO;
namespace original
{
    u64 codeSize;
    char* code;
}
char* code;
u64 codeSize;
i64 lineCounter = 1;
u64 CLnew = 0;
string line;
string assembly;
enum bodyType
{
    _c,_f,_l
};
bodyType BT;
string currentFile = "";
string getCodePos()
{
    return currentFile+":"+std::to_string(lineCounter);
}
string getFunctionLabel(string name,list<var>& args)
{
    string fLabel = "_function_"+name;
    for(var& I : args)
        fLabel+="_"+types[objects[I.T-EOO].subNumber].name;
    std::cout << "function label generated: " << fLabel << std::endl;
    return fLabel;
}
string getClassLabel(string name)
{
    return "_class_"+name;
}
#include "genAsm.cpp"


bool isLiteral(string s);
u64 resolveSymbol(string symbol);
u64 addVar(string name,u64 T);
std::stack<var> thisStack;
list<var> noArgs;
u64 cLevel = 0;
u64 LLType = 0;

u64 getTypeSize(u64 T)
{
    return 8;//64 bit default
}

list<var> getFunctionArgs(string& str)
{
    if(str[str.size()-1]==')'&&str[str.size()-2]=='(')
        return noArgs;
    string post;
    if(str.back() == ')')
        post = str.substr(0,str.size()-1);
    else
        post = str.substr(0,str.size());
    if(!strContains(':',post))
        post=_namespace_+post;
    list<string> tokens = strSplit(post,"(",0,0);
    list<var> args;
    if(tokens.size() > 1)
    {
        list<string> tArgs = strSplit(tokens[1],",",0,0);
        for(string I : tArgs)
        {
            if(isLiteral(I))
            {
                addVar("_var_"+std::to_string(objNumber),LLType);
                args.push_back(vars.back());
            }
            else
            {
                u64 v = resolveSymbol(I);
                if(objects[v].ot == _type)
                    std::cout << "[ERROR] can't pass type as argument to a function! at " << getCodePos() << std::endl;

                if(objects[v].ot == _function)
                {
                }

                if(objects[v].ot == _variable)
                {
                    args.push_back(vars[objects[v].subNumber]);
                }
            }
        }
        return args;
    }
    std::cout << "[ERROR] " << str << " is not a valid function call! at " << getCodePos() << std::endl;
    showASM();
}
void makeNameSpaceString()
{
    _namespace_ = "";
    for(scope& s : scopeStack)
    {
        _namespace_="::"+s.name+_namespace_;
    }
}

u64 getVar(string name)
{
    for(var& I : vars)
        if(I.name == name)
            return I.number;
    return 0;
}

u64 getFunc(string name,list<var>& args)
{
    for(function& I : functions){if(I.name == name)
    {
        if(args.size() == 0)
            continue;

        if(I.args.size() != args.size())
            continue;

        list<var>& fArgs = I.args;
        var* ea;
        var* na;
        bool argsEqual = 1;
        for(int I = 0;I<args.size();I++)
        {
            ea = &fArgs[I];na = &args[I];
            if(ea->T != na->T)
            {
                argsEqual = 0;
                break;
            }
        }
        if(argsEqual)
            return I.number;
    }}
    return 0;
}

bool objExists(string name,list<var>& args)
{
    for(var& I : vars)
        if(I.name == name)
            return 1;

    for(function& I : functions){if(I.name == name)
    {
        if(args.size() == 0)
            continue;

        if(I.args.size() != args.size())
            continue;

        list<var>& fArgs = I.args;
        var* ea;
        var* na;
        bool argsEqual = 1;
        for(int I = 0;I<args.size();I++)
        {
            ea = &fArgs[I];na = &args[I];
            if(ea->T != na->T)
            {
                argsEqual = 0;
                break;
            }
        }
        if(argsEqual)
            return 1;
    }}
    return 0;
}

u64 getType(string name)
{
    for(int I = 0;I<types.size();I++)
        if(types[I].name == name)
            return types[I].number;
    std::cout << "[ERROR] type \"" << name << "\" does not exist! at " << getCodePos() << std::endl;
    return 0;
}

objType getObjType(u64 objNum);
u64 getTypeOf(u64 o)
{
    if(getObjType(o) == _type)
        return o;
    if(getObjType(o) == _function)
        return functions[objects[o].subNumber].T;
    if(getObjType(o) == _variable)
        return vars[objects[o].subNumber].T;
    return 0;
}

string getObjName(u64 o);
objType getObjType(u64 objNum)
{
    for(objD& I : objects)
        if(I.objNumber == objNum)
            return I.ot;
        else
            if(f_Debug)
                std::cout << "DEBUG: " << I.objNumber <<"("<<getObjName(I.objNumber)<<")"<< " != " << objNum << std::endl;

    if(f_Debug)
        std::cout << "[ERROR] object " << objNum << " does not exist!" << std::endl;
    showASM();
}

string getObjName(u64 o)
{
    switch(getObjType(o))
    {
        case(_function):return functions[objects[o-EOO].subNumber].name;
        case(_variable):return vars[objects[o-EOO].subNumber].name;
        case(_type):return types[objects[o-EOO].subNumber].name;
    }
    return "### UNDEFINED OBJ NAME ###";
}

u64 findBodyEnd(u64 bs)
{
    return 0;
}

u64 getNext(u64 start,char c,string& str)
{
    for(u64 I = start;I<str.size();I++)
        if(str[I] == c)
            return I;
    return str.size();
}

u64 addVar(string name,u64 T)
{
    var v;
    if(currentFlags._engine)
        v = var(_namespace_+name,EON++,vars.size(),T,_variable);
    else
        v = var(_namespace_+name,objNumber++,vars.size(),T,_variable);
    v.fl = currentFlags;
    currentFlags = flags();
    vars.push_back(v);
    objects.push_back(objD(vars.back().ObjType,vars.back().subNumber,vars.back().number));
    LDO = vars.back().number;

    type vt = types[objects[T-EOO].subNumber];
    for(var& mv : vt.memberVariables)
        addVar(name+"::"+mv.name,mv.T);

    if(f_Verbose)
        std::cout << "variable \"" << _namespace_+name << "\""<<"("<<LDO<<")"<<" declared with type: \"" << types[T-EOO].name <<"\""<<" at "<<getCodePos()<< std::endl;

    return v.number;
}

u64 addFunction(string name,list<var>& args,u64 T)
{
    function f;
    if(currentFlags._engine)
        f = function(_namespace_+name,EON++,functions.size(),T,_function);
    else
        f = function(_namespace_+name,objNumber++,functions.size(),T,_function);
    f.args = args;
    f.fl = currentFlags;
    currentFlags = flags();
    functions.push_back(f);
    objects.push_back(objD(_function,f.subNumber,f.number));
    LDO = f.number;
    LDF = LDO;

    if(scopeStack.back().scopeT != st_namespace)
        scopeStack.pop_back();

    if(CLnew!=cLevel)
    {
        scope s;
        s.parent = &scopeStack.back();
        s.returnType = T;
        s.scopeT = st_function;
        s.name = getFunctionLabel(name,args);
        scopeStack.push_back(s);

        isFunctionBody = true;
    }

    if(f_Verbose)
    {
        std::cout << "function \"" << _namespace_+name<<"(";
        for(var I : args)
            std::cout << getObjName(I.T) << ",";
        if(args.size()>0)
            std::cout << "\b";
        std::cout << ")\""<<"("<<LDO<<")"<<" declared with type: \"" << getObjName(T) <<"\""<<" at "<<getCodePos()<< std::endl;
    }
    return f.number;
}

u64 addType(string name)
{
    type T;
    if(currentFlags._engine)
        T = type(name,EON++,(u64)types.size(),0,_type);
    else
        T = type(name,objNumber++,(u64)types.size(),0,_type);
    T.fl = currentFlags;
    currentFlags = flags();
    types.push_back(T);
    objects.push_back(objD(_type,types.back().subNumber,types.back().number));
    LDO=types.back().number;
    LDT=LDO;

    if(scopeStack.back().scopeT != st_namespace)
        scopeStack.pop_back();

    if(CLnew!=cLevel)
    {
        scope s;
        s.parent = &scopeStack.back();
        s.name = getClassLabel(name);
        s.scopeT = st_class;
        scopeStack.push_back(s);

        isClassBody = true;
    }

    if(f_Verbose)
        std::cout << "declared type " << name <<"("<<LDO<<")"<<" at "<< getCodePos() << std::endl;
    return T.number;
}

bool isInteger(string s)
{
    if(s[0]=='-')
        s=s.substr(1,s.size()-1);
    for(char I : s)
        if(!std::isdigit(I))
            return 0;

    LLType = getType("i64");
    return 1;
}

bool isFloat(string s)
{
    bool hasDecPoint = 0;
    if(s[0]=='-')
        s=s.substr(1,s.size()-1);
    for(char I : s)
        if(!std::isdigit(I) || (hasDecPoint && I == '.'))
            return 0;

    LLType = getType("f64");
    return 1;
}

bool isStringLiteral(string& s)
{
    if(s[0] == '"' && s.back() == '"')
        return 1;

    LLType = getType("string");
    return 0;
}

bool canCast(u64 from,u64 to)
{
    if(from==to)
        return 1;
    return 0;
}

bool isLiteral(string s)
{
    s = strSplit(s,"::",0,0).back();
    if(isInteger(s)||isFloat(s)||isStringLiteral(s))
        return 1;
    else
    {
        std::cout << "not literal: " << s << std::endl;
        return 0;
    }
}

u64 resolveSymbol(string symbol)
{
    string post;
    if(symbol.back() == ')')
        post = symbol.substr(0,symbol.size()-1);
    else
        post = symbol.substr(0,symbol.size());
    if(!strContains(':',post))
        post=_namespace_+post;
    list<string> tokens = strSplit(post,"(",0,0);
    list<var> args;
    if(tokens.size() > 1)
    {
        list<string> tArgs = strSplit(tokens[1],",",0,0);
        for(string I : tArgs)
        {
            if(isLiteral(I))
            {
                addVar("_var_"+std::to_string(objNumber),LLType);
                args.push_back(vars.back());
            }
            else
            {
                u64 v = resolveSymbol(I);
                if(objects[v].ot == _type)
                    std::cout << "[ERROR] can't pass type as argument to a function! at " << getCodePos() << std::endl;

                if(objects[v].ot == _function)
                {
                }

                if(objects[v].ot == _variable)
                {
                    args.push_back(vars[objects[v].subNumber]);
                }
            }
        }
        for(function& I : functions)
        {
            if(I.name == tokens[0])
            {
                if(I.args.size() == args.size())
                {
                    for(u64 II = 0;II<args.size();II++)
                        if(I.args[II].T != args[II].T)
                            goto nextFunction;
                    return I.number;
                }
            }
            nextFunction: ;
        }
    }
    if(isLiteral(symbol))
        return addVar("_var_"+std::to_string(objNumber),LLType);
    // vars,functions and types
    for(var& I : vars)
        if(I.name == tokens[0])
            return I.number;

    for(function& I : functions)
        if(I.name == tokens[0])
            return I.number;

    for(type& I : types)
        if(I.name == tokens[0])
            return I.number;

    std::cout << "[ERROR] use of undefined identifier \"" << tokens[0] << "\" at " << getCodePos() << std::endl;
    showASM();
    return 0;
}

string getBeforeFirst(string& str,char c)
{
    string r;
    for(char I : str)
    {
        if(I==c)
            break;
        r.push_back(I);
    }
    return r;   
}

//?
//? body sub compile
//?

void compile_body_functionCallTarget(list<string>& tokens,u64 returnObj)
{
    string targetObj = "";
    for(string I : tokens)
    {
        if(I.back() == ')')
        {
            //I is function
            if(targetObj.size() > 0)
                targetObj.pop_back();

            {
                list<var> args = getFunctionArgs(I);
                if(objExists(getBeforeFirst(I,'('),args))
                {
                }
            }
        }
        else
        {
            //I is variable
            targetObj+=I+".";
        }
    }
}

void compile_body_functionCall(list<string>& tokens)
{
    if(tokens.size() == 1)
    {
        list<string> st = strSplit(tokens[0],".",0,0);
        if(st.size() == 1)
            genFunctionCall(0,0,getBeforeFirst(tokens[0],'('),getFunctionArgs(tokens[0]));
        return compile_body_functionCallTarget(st,0);
    }
}
/*
    void _ = str.substr().substr().clear();

    _0 = str.substr();
    _1 = _0.substr();
    _ = _1.clear();
*/

void compile_body_declareVarValue(list<string>& tokens)
{
    u64 vType = getType(tokens[0]);
    addVar(tokens[1],vType);
    if(isLiteral(tokens[3]))
    {
        genMalloc(LDO,vType);
        genWriteLiteral(LDO,tokens[3]);
    }
    u64 v = resolveSymbol(tokens[3]);
    if(objects[v-EOO].ot == _type)
    {
        std::cout << "[ERROR] can't assign variable to a type! at: " << getCodePos() << std::endl;
        showASM();
    }

    if(objects[v-EOO].ot == _function)
    {
        //resolve function call
    }

    if(objects[v-EOO].ot == _variable)
    {
        if(canCast(vars[objects[v-EOO].subNumber].T,vType))
        {
            genMalloc(LDO,getTypeSize(vType));
            genValueAssign(LDO,v);
        }
        else
        {
            std::cout << "[ERROR] can't assign value of type \"" << getObjName(vars[objects[v-EOO].subNumber].T) << "\" to object of type \"" << getObjName(vType) << "\"! at " << getCodePos() << std::endl;
        }
    }
}

void compile_body_declareVarConstructor(list<string>& tokens)
{
    //addVar(strSplit(tokens[1],"(",0,0)[0],getType(tokens[0]));
}

void compile_body_return(list<string>& tokens)
{
    if(isLiteral(tokens[1]))
        return genReturnLiteral(tokens[1]);
    u64 returnValue = resolveSymbol(tokens[1]);
    u64 returnValueType = getTypeOf(returnValue);
    if(returnValue == returnValueType)
    {
        std::cout << "r: " << returnValue << " == " << returnValueType << std::endl;
        std::cout << "[ERROR] can't return a type from a function! at "<<getCodePos()<< std::endl;
        showASM();
    }
    if(returnValueType != scopeStack.back().returnType)
    {
        std::cout << "[ERROR] can't return a value of type " << getObjName(returnValueType) << " from a function of type " << getObjName(scopeStack.back().returnType) <<"! at "<<getCodePos()<< std::endl;
        showASM();
    }
    genReturn(returnValue);
}

//?
//? top level compile
//?
void compileLine_topLevel()
{
    if(f_Debug)
        std::cout << "compiling line in root level" << std::endl;
    u64 ze = 0;
    list<string> tokens = strSplit(line," ",0,0);

    goto _2_skip;
    _3_jmp: ;
    ze++;
    _2_skip: ;
    if(isKeyWord(tokens[ze]))
        goto _3_jmp;
    //? 
    //? class declarations
    //? 
    if(tokens[ze] == "class")
    {
        //type
        addType(tokens[ze+1]);/*
            i literally spent like 10 or so minutes trying to figure out why types weren't 
            being declared anymore, turns out i just full on forgot to add this line*/
        return;
    }
    u64 T = getType(tokens[ze]);
    if(strContains('(',tokens[ze+1]))
    {
        //function declaration
        string decl;
        for(int I = ze+1;I<tokens.size();I++)
            decl+=tokens[I];  
        
        list<string> st = strSplit(tokens[ze+1],"(",0,0);
        list<var> TPargs;
        string fName = st[0];
        if(st[1][0] != ')')
        {
            string sArgs = strSplit(st[1],")",0,0)[0];
            list<string> args = strSplit(sArgs,",",0,0);
            for(string I : args)
            {
                list<string> at = strSplit(I," ",0,0);
                stripList(at);
                u64 argType = getType(at[0]);
                string argName = at[1];
                TPargs.push_back(var(argName,0,0,argType,_variable));
            }
        }
        if(objExists(fName,TPargs))
        {
            std::cout << "[ERROR] object with name \"" << fName << "\" already exists!" << std::endl;
            showASM();
        }
        addFunction(fName,TPargs,T);
        if(CLnew != cLevel)
            genFunc(fName,TPargs);
        return;
    }
    //variable declaration
    if(objExists(tokens[ze+1],noArgs))
    {
        std::cout << "[ERROR] object with name \"" << tokens[1] << "\" already exists!" << std::endl;
        showASM();
    }

    if(!isClassBody || currentFlags._static)
        addVar(tokens[1],T);
    else
        types[objects[LDT-EOO].subNumber].memberVariables.push_back(var(tokens[1],objNumber++,vars.size(),T,_variable));
}

void compileLine_body()
{
    list<string> tokens = strSplit(line," ",0,0);
    if(!f_Debug)
        goto CLBDebugEnd;
    std::cout << "compiling line in body" << std::endl;
    std::cout << "tokens: " << std::endl;
    for(string token : tokens)
        std::cout << token << std::endl;
    std::cout << "tokens: " << tokens.size() << std::endl;
    CLBDebugEnd: ;
    switch(tokens.size())
    {
        case(1):
            if(f_Verbose)
                std::cout << "function call" << std::endl;
            compile_body_functionCall(tokens);
            break;
        case(2):
            if(tokens[0] == "return")
            {
                if(f_Verbose)
                    std::cout << "return call" << std::endl;
                compile_body_return(tokens);
                break;
            }
            if(f_Verbose)
                std::cout << "var declare constructor" << std::endl;
                compile_body_declareVarConstructor(tokens);
            break;
        case(4):
            if(f_Verbose)
                std::cout << "var declare value" << std::endl;
                compile_body_declareVarValue(tokens);
            break;
        default:
            if(f_Verbose)
                std::cout << "invalid instruction" << std::endl;
            break;
    }
    return;
}

bool empty(string& str)
{
    for(char I : str)
        if(!(std::isspace(I) || I == '\n'))
            return 0;
    return 1;
}

void compileLine()
{
    strip(line);
    if(empty(line))
        return;

    makeNameSpaceString();
    if(f_Debug)
        std::cout << "namespace: " << _namespace_ << std::endl;

    if(f_Debug)
    {
        std::cout << "line: " << line << std::endl;
        std::cout << "line:";
        for(char I : line)
            std::cout << " " << (i64)I;
        std::cout << std::endl;
    }
    strip(line);
    if(line[0] == '/' && line[1] == '/')
    {
        if(line.size()>sizeof("//@file "))
        {
            if(line.substr(0,sizeof("//@file ")-1) == "//@file ")
            {
                //save old file
                for(codeFile& cf : CFlist)
                {
                    if(cf.name == currentFile)
                    {
                        cf.line = lineCounter;
                        break;
                    }
                }

                //set new file
                currentFile = strSplit(line," ",0,0)[1];
                for(codeFile& cf : CFlist)
                {
                    if(cf.name == currentFile)
                    {
                        lineCounter = cf.line;
                        goto _1_skip;
                    }
                }
                lineCounter = 0;
                CFlist.push_back(codeFile(currentFile,lineCounter));
                _1_skip: ;
                if(f_Verbose)   
                    std::cout << "compiling file: " << currentFile << std::endl;
            }
        }
        return;
    }
    if(scopeStack.back().name.substr(0,sizeof("_class_")-1) == "_class_")
    {
        isClassBody=true;
        return compileLine_topLevel();
    }
    else
        isClassBody=false;
    if(cLevel == 0)
        return compileLine_topLevel();
    return compileLine_body();
}

bool makeLine()
{
    cLevel = CLnew;
    line = "";
    u64 I;
    u64 ecc = 0;
    for(I = 0;I<codeSize;I++)
    {
        switch(code[I])
        {
            case(';'):
            {
                ecc++;
                goto endLoop;
            }
            case('{'):
            {
                CLnew++;
                ecc++;
                goto endLoop;
            }
            case('}'):
            {
                ecc++;
                CLnew--;
                if(scopeStack.size() > 1)
                    scopeStack.pop_back();
                goto endLoop;
            }
            case('\n'):
            {
                lineCounter++;//fall through to default
            }
            default:
            {
                line.push_back(code[I]);
            }
        }
    }
    endLoop: ;
    I+=ecc;
    codeSize-=I;
    code+=I;
    if(codeSize<1)
        return 0;
    return 1;
}

void compile(string& str)
{
    scopeStack.push_back(scope());
    
    original::code      = (char*)malloc(str.size());
    memcpy(original::code,str.c_str(),str.size());
    original::codeSize  = str.size();
    code     = original::code;
    codeSize = original::codeSize;
    
    //setup engine for coming code
    assembly+="set EOO "+std::to_string(EOO)+"\n";

    //compile code
    while(makeLine())
        compileLine();

    free(original::code);
}
