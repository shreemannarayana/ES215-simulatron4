#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <utility>
#define st string  
using namespace std;
enum Instr_Type {R_type,I_type,J_type, Invalid_type} ;
map<char,st> HexMap()
{
    map<char,st> Hex_Map;
    Hex_Map['0']  = "0000";Hex_Map['1']  = "0001";Hex_Map['2']  = "0010";Hex_Map['3']  = "0011";
    Hex_Map['4']  = "0100";Hex_Map['5']  = "0101";Hex_Map['6']  = "0110";Hex_Map['7']  = "0111";
    Hex_Map['8']  = "1000";Hex_Map['9']  = "1001";Hex_Map['a']  = "1010";Hex_Map['b']  = "1011";
    Hex_Map['c']  = "1100";Hex_Map['d']  = "1101";Hex_Map['e']  = "1110";Hex_Map['f']  = "1111";   
    return Hex_Map;        
}
map<st,st> R_Map()
{
    map<st,st> R_type_map;
    R_type_map["100000"] = "add";R_type_map["100100"] = "and";//R_type_map["100001"] = "addu";
    R_type_map["001000"] = "jr"; R_type_map["100111"] = "nor"; R_type_map["100101"] = "or";
    R_type_map["101010"] = "slt";R_type_map["101011"] = "sltu";R_type_map["000000"] = "sll";
    R_type_map["000010"] = "srl";R_type_map["100010"] = "sub";// R_type_map["100011"] = "subu";
    return R_type_map;
}
map<st,st> I_Map()
{
   map<st,st> I_type_map;
   I_type_map["001000"] = "addi"; I_type_map["100001"] = "lh";   I_type_map["001100"] = "andi";I_type_map["000100"] = "beq";
   I_type_map["000101"] = "bne";  I_type_map["100100"] = "lbu";  I_type_map["100101"] = "lhu"; I_type_map["110000"] = "ll";
   I_type_map["001111"] = "lui";  I_type_map["100011"] = "lw";   I_type_map["001101"] = "ori"; I_type_map["001010"] = "slti";
   I_type_map["001011"] = "sltiu";I_type_map["101000"] = "sb";   I_type_map["111000"] = "sc";  I_type_map["101001"] = "sh";
   I_type_map["101011"] = "sw";   I_type_map["100000"] = "lb";//I_type_map["001001"] = "addiu";
   return I_type_map;
}
st intToStrHex(int val)
{
    stringstream strmVal;
    strmVal << setfill('0') << setw(4) << hex << val;
    return strmVal.str();
}
map<st,st> register_Map()
{
    map<st,st> Rgstr_Map;
    Rgstr_Map["00000"] = "$zero";Rgstr_Map["00001"] = "$at";Rgstr_Map["00010"] = "$v0";Rgstr_Map["00011"] = "$v1";

    Rgstr_Map["00100"] = "$a0";  Rgstr_Map["00101"] = "$a1";Rgstr_Map["00110"] = "$a2";Rgstr_Map["00111"] = "$a3";

    Rgstr_Map["01000"] = "$t0";  Rgstr_Map["01001"] = "$t1";Rgstr_Map["01010"] = "$t2";Rgstr_Map["01011"] = "$t3";
    Rgstr_Map["01100"] = "$t4";  Rgstr_Map["01101"] = "$t5";Rgstr_Map["01110"] = "$t6";Rgstr_Map["01111"] = "$t7";
    
    Rgstr_Map["10000"] = "$s0";  Rgstr_Map["10001"] = "$s1";Rgstr_Map["10010"] = "$s2";Rgstr_Map["10011"] = "$s3";
    Rgstr_Map["10100"] = "$s4";  Rgstr_Map["10101"] = "$s5";Rgstr_Map["10110"] = "$s6";Rgstr_Map["10111"] = "$s7";

    Rgstr_Map["11000"] = "$t8";  Rgstr_Map["11001"] = "$t9";

    Rgstr_Map["11010"] = "$k0";  Rgstr_Map["11011"] = "$k1";

    Rgstr_Map["11100"] = "$gp";  Rgstr_Map["11101"] = "$sp";Rgstr_Map["11110"] = "$fp";Rgstr_Map["11111"] = "$ra";
    return Rgstr_Map;
}
st toTwoComplement (st val)
{   
    st retVar = "";
    unsigned int i = val.size() - 1;
    while (val[i] != '1' && i >= 0) 
        i--;
    st first = val.substr(0,i);
    st second = val.substr(i,val.size() - i);
    for (auto i : first)
    {
        if (i == '1') 
            retVar.push_back('0');
        if (i == '0') 
            retVar.push_back('1');
    }
    retVar += second;
    return retVar;
}
bool flagUnsigned(st op)
{
    bool retVar = false;
    if (op == "lbu" || op == "lhu" || op == "sltiu" || op == "sltu" || op == "subu")//op == "addiu" || op == "addu" || 
        retVar = true;
    return retVar;
}
st convertHexToBi(st iptLine)
{
    st binVal = "";
    map<char,st> ASCII = HexMap();
    for (int i = 0; i < iptLine.size(); i++)
    {        
        auto found = ASCII.find(iptLine[i]);
        binVal += found->second;
    }
    return binVal;
}
Instr_Type CheckType (st binVal)
{
    st opcode = "";
    Instr_Type type;
    opcode = binVal.substr(0,6);
    map<st,st> Imap = I_Map();
    if (opcode == "000000")
        type = R_type;
    else if (opcode == "000010" || opcode == "000011")
        type = J_type;
    else if (Imap.find(opcode) != Imap.end())
        type = I_type;
    else
        type = Invalid_type;
    return type;
}
bool validRtype (st val)
{
    bool retVar = true;
     if ( val == "$fp" || val == "$gp"|| val == "$sp" || val == "$k0"|| val == "$k1")//|| val == "$ra" 
     retVar = false;
     return retVar;
}
st getSignNo(st val)
{
    st retVar = "";
    if (val[0] == '1')
    {
        st complement = toTwoComplement (val);
        int numInt = stoi(complement, nullptr, 2) * -1;
        retVar = to_string(numInt); 
    }
    else
    {
        int numInt = stoi(val, nullptr, 2);
        retVar = to_string(numInt);
    }
    return retVar;
}
st getUnsignNo(st val)
{
    int numInt = stoi(val, nullptr, 2);
    return to_string(numInt);    
}
bool hasWhiteSpace(st val){
    int count=0;
    for (auto i: val) {
        int ch = i;
        if (isspace(ch))
            count++;
    }//cout<<count<<"\n";
    return count==val.size();
}
void forRtype(st binVal, vector <st> & validOpt, vector <st> & inValidOpt, int linCnt)
{
    st rs = "", rt = "", rd = "", shamt = "", funct = "", ans = "";
    rs = binVal.substr(6,5);
    rt = binVal.substr(11,5);
    rd = binVal.substr(16,5);
    shamt = binVal.substr(21,5);
    funct = binVal.substr(26,6);
    auto regMap = register_Map();
    auto rTypeMap = R_Map();
    st found_rs = regMap.find(rs)->second;
    st found_rt = regMap.find(rt)->second;
    st found_rd = regMap.find(rd)->second;
    auto foundFunct  = rTypeMap.find(funct);
    // checks for any errorful line at the start before forming the assembly code
    if (foundFunct == rTypeMap.end())
    {
        inValidOpt.push_back("Error, cant find function " + funct + " at line " + to_string(linCnt));
    }
    else if (!validRtype(found_rs))
    {
        inValidOpt.push_back("rs can't be in " + found_rs + " at line " + to_string(linCnt));
    }
    else if (!validRtype(found_rt))
    {
        inValidOpt.push_back("rt can't be in " + found_rt + " at line " + to_string(linCnt));  
    }
    else if (!validRtype(found_rd))
    {
        inValidOpt.push_back("rd can't be in " + found_rd + " at line " + to_string(linCnt)); 
    }
    else if (foundFunct->second == "jr")
    {
        ans = foundFunct->second + "\t" + found_rs;
        validOpt.push_back(ans);
    }
    else if (foundFunct->second == "sll" || foundFunct->second == "srl")
    {
        ans = foundFunct->second + "\t" + found_rd + ", " + found_rt + ", " +getUnsignNo(shamt);
        validOpt.push_back(ans);
    }
    else
    {
        ans = foundFunct->second + "\t" + found_rd + ", " + found_rs + ", " + found_rt;
        validOpt.push_back(ans);
    }
}
bool optFrmt_ITypeConst (st val)
{
    bool retVar = false;
    if (val == "addi" || val == "andi" || val == "ori" || val == "slti" || val == "sltiu" || val == "lui")//|| val == "addiu"
        retVar = true;
    return retVar;
}
void forItype(st binVal, vector <st> & validOpt, vector <st> & inValidOpt, int linCnt, map <st,int> & addrFile)
{
    st rs = "", rt = "", imm = "", ans = "", op = "";
    op = binVal.substr(0,6);
    rs = binVal.substr(6,5);
    rt = binVal.substr(11,5);
    imm = binVal.substr(16,16);
    auto regMap = register_Map();
    auto iTypeMap = I_Map();
    auto foundFunct  = iTypeMap.find(op);
    st found_rs = regMap.find(rs)->second;
    st found_rt = regMap.find(rt)->second;
    if (foundFunct == iTypeMap.end())
        inValidOpt.push_back("Error, cant find function " + op + " at line " + to_string(linCnt));
    else if (foundFunct->second == "beq" || foundFunct->second == "bne")
    {
        // int pos = stoi(getSignNo(imm)); // get the number it wants to jump 3
        // pos = 1 + pos + linCnt; // this will be actual location were it shoud be (not memory address) -17
        // int addrPos = pos * 4; // 68
        // st address = "Addr_" + intToStrHex(addrPos);
        ans = foundFunct->second + "\t" + found_rs + ", " + found_rt + ", " + getSignNo(imm);//address;
        // pair <st,int> checking (address,pos);
        // addrFile.insert(checking);
        validOpt.push_back(ans);
    }
    else if (optFrmt_ITypeConst(foundFunct->second))
    {
        if (flagUnsigned(foundFunct->second))
            imm =getUnsignNo(imm);
        else 
            imm = getSignNo(imm);
        ans = foundFunct->second + "\t" + found_rt + ", " + found_rs + ", " + imm;
        validOpt.push_back(ans);
    }
    else
    {
        if (flagUnsigned(foundFunct->second))
            imm =getUnsignNo(imm);
        else 
            imm = getSignNo(imm);
        ans = foundFunct->second + "\t" + found_rt + ", " + imm + "(" + found_rs + ")";
        validOpt.push_back(ans);
    }
}
void forJtype(st binVal, vector <st> & validOpt, vector <st> & inValidOpt, int linCnt)
{
    st addr = "", op = "", ans = "";
    op = binVal.substr(0,6);
    addr = binVal.substr(6,26);
    if (op=="000010")
        ans = "j\t" + getUnsignNo(addr);
    else
        ans = "jal\t" + getUnsignNo(addr);
    validOpt.push_back(ans);
}
void evalCurrLine(st iptLine, vector <st> & validOpt, vector <st> & inValidOpt, int linCnt, map <st,int> & addrFile)
{
    if ((iptLine.size() != 8) &&(iptLine.size() != 32) && !hasWhiteSpace(iptLine))
        inValidOpt.push_back("Cannot disassemble " + iptLine + " at line " + to_string(linCnt));  
    else
        {
            st binVal;int prevSize=0;
            if(iptLine.size() == 8&&(prevSize==0||prevSize==8)) {
                binVal =  convertHexToBi(iptLine);// cout<<binVal<<"\n";
                prevSize=8;
            }
            else if(iptLine.size() == 32&&(prevSize==0||prevSize==32))
                binVal = iptLine;
            Instr_Type type = CheckType(binVal);
            //cout<<iptLine<<hasWhiteSpace(iptLine)<<"g\n";
            if (type == R_type)
                forRtype(binVal, validOpt, inValidOpt, linCnt);
             else if (type == I_type)
                forItype(binVal, validOpt, inValidOpt, linCnt, addrFile);
             else if (type == J_type)
                forJtype(binVal, validOpt, inValidOpt, linCnt);
             else if (!hasWhiteSpace(iptLine)){
                
                inValidOpt.push_back("Cannot disassemble " + iptLine + " at line " + to_string(linCnt) + " not of R, I or J type");
             }
        }   
}
st getIptName(st val)
{
    unsigned int i = val.size() - 1;
    while (val[i] != '.' && i >= 0) 
        i--;   
    return val.substr(0,i);
}
int main(int argc, char const *argVal[] )
{
    st currLine;
    vector <st> validOpt;
    vector <st> inValidOpt;
    map <st,int> addrFile;
    int linCnt = 0;
    if (argc == 2)
    {
    ifstream in (argVal[1]);
    if (!in)
    {
        cout << "Cannot open the file" << endl;
        return EXIT_FAILURE;
    }
    else
    {
        // int flagHex;
        // if (argVal[2]=="hex")
        //     flagHex=1;
        // else if (argVal[2]=="bin")
        //     flagHex=2;
        // cout<<"Machine code is in:\n1.Hexadecimal form\n2.Binary form\nEnter choice:";
        // cin>>flagHex;
        // if(flagHex!=1&&flagHex!=2){
        //     cout<<"Invalid choice, assuming Binary form";
        //     flagHex=2;
        // }
        while (!in.eof())
        {
           getline(in,currLine,'\n');
           //cout << currLine <<" "<<in.eof()<< '\n';
           evalCurrLine(currLine, validOpt, inValidOpt, linCnt, addrFile);
           linCnt++;
        }
    }
    // for (auto i : validOpt)
    //         cout << i << endl;
    map <int,st> swappedAddrFile;
    if (inValidOpt.empty())
    {
        st optName =  getIptName(argVal[1]) + ".s";
        ofstream out (optName);
        if (!addrFile.empty())
        {
            for (auto i: addrFile)
                swappedAddrFile[i.second] = i.first;
            int cntr = 0;
            for (auto i : swappedAddrFile)
            {
                auto vectBeg = validOpt.begin();
                validOpt.insert(vectBeg + cntr + i.first,i.second);
                cntr++;
            }
        }
        for (auto i : validOpt)
            out << i << endl;
    }
    else 
    {
        for (auto i : inValidOpt)
            cout << i << endl;
    }
    }
    else
        cout << "Please Enter the Object File" << endl;
    return EXIT_SUCCESS;
}
