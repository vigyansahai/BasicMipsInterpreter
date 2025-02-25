#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<unsigned int> RF = vector<unsigned int>(32,0);
vector<unsigned int> preRF = vector<unsigned int>(32,0);
unsigned int HI = 0;
unsigned int LO = 0;
// unsigned int PC = 0x00400000;
unsigned int PC = 0;
//xxd, od
int main(){
    RF[0]=0;
    ifstream input("input.txt", ifstream::binary);
    ostringstream temp;
    temp << input.rdbuf();
    const string &s = temp.str();
    vector<char> imem(s.begin(),s.end());
    

    while(PC<imem.size()){
        unsigned int command=0;
        for(int i=0;i<32;i++){
            if(imem[i+PC]=='1'){
                command += 1<<(31-i);
            }
        }
        cout << "cmd: "<<command << endl;
//0x2, 0x3
        unsigned int OPCODE = (command >> 26) & 0x3f;
        cout << "PC: " << PC << " -- index: "<< (PC/32) << endl;
        // cout << "op: "<<OPCODE << endl;
        if(OPCODE==0b000000){
            // cout << "R type" << endl;
            unsigned int FUNCT = command & 0x3f;
            unsigned int rs = (command >> 21) & 0x1f;
            unsigned int rt = (command >> 16) & 0x1f;
            unsigned int rd = (command >> 11) & 0x1f;
            unsigned int shamt = (command >> 6) & 0x1f;
            cout << "rs: " << rs << endl;
            cout << "rt: " << rt << endl;
            cout << "rd: " << rd << endl;
            cout << "shamt: " << shamt << endl;
            cout << "func: " << FUNCT << endl;
            switch (FUNCT) {
                case 0xc:
                    {
                        //SYSCALLs
                        cout << "syscall ";
                        if((int)RF[2]==10){
                            cout << "die" << endl;
                            return 0;
                        }
                        else if((int)RF[2]==1){
                            cout << "pint" << endl;
                            cout << (int)RF[4];
                        }
                        else if((int)RF[2]==4){
                            cout << "pstring" << endl;
                            string output = "";
                            char letter=0;
                            unsigned int location = RF[4];
                            for(int i=0;i<8;i++){
                                if(imem[i+location]=='1'){
                                    letter += 1<<(7-i);
                                }
                            }
                            cout << "first location: " << location << " char: " << (int)letter << endl;
                            while(letter!=0){
                                output+=letter;
                                cout << "location: " << location << endl;
                                cout << "letter: " << letter << " total output: " << output << endl;
                                letter = 0;
                                location+=8;
                                for(int i=0;i<8;i++){
                                    if(imem[i+location]=='1'){
                                        letter += 1<<(7-i);
                                    }
                                }
                            }
                            cout << output << endl;
                        }
                        else if(RF[2]==5){
                            cout << "take int" << endl;
                            cin >> RF[2];
                        }
                        else if(RF[2]==8){
                            cout << "take string" << endl;
                            string input = "";
                            if(RF[5]<1) break;
                            cin.ignore();
                            getline(cin, input);
                            if(input.size()>RF[5]){
                                input = input.substr(0,RF[5]);
                            }
                            unsigned int location = RF[4];
                            for(int k=0;k<input.size();k++){
                                unsigned char c = input[k];
                                cout << "cur char: " << c << endl;
                                for(int i=0;i<8;i++){
                                    cout << "before: " << (c>>(7-i))%2 << endl;
                                    imem[location+i]=((c>>(7-i))%2)+48;
                                    cout << "location: " << location+i << " bit: "<<imem[location+i] << endl;
                                }
                                location+=8;
                            }
                            for(int i=0;i<8;i++){
                                imem[location+i]=0;
                            }
                        }
                        break;
                    }
                case 0x21:
                    {//addu
                    cout << "addu" << endl;
                    if(rd==0) break;
                    RF[rd] = RF[rs]+RF[rt];
                    break;
                    }
                case 0x22:
                    {//sub
                        cout << "sub " << RF[rd] << ":= " << RF[rs] << " - " << RF[rt] << endl;
                        if(rd==0) break;
                        int temp1 = RF[rs];
                        int temp2 = (-1)*(RF[rt]);
                        RF[rd] = RF[rs]+(-1)*(RF[rt]);
                        if(temp1>0&&temp2>0&&RF[rd]<0){
                            cout << "overflowwwww" << endl;
                        }
                        else if(temp1<0&&temp2<0&&RF[rd]>0){
                            cout << "overflowwwww" << endl;
                        }
                        break;
                    }
                case 0x20:
                    {//ADD
                    cout << "add " << RF[rd] << ":= " << RF[rs] << " + " << RF[rt] << endl;
                    if(rd==0) break;
                    int temp1 = RF[rs];
                    int temp2 = RF[rt];
                    RF[rd] = RF[rs]+RF[rt];
                    if(temp1>0&&temp2>0&&RF[rd]<0){
                        cout << "overflowwwww" << endl;
                    }
                    else if(temp1<0&&temp2<0&&RF[rd]>0){
                        cout << "overflowwwww" << endl;
                    }
                    break;
                    }
                case 0x23:
                    {//subu
                    cout << "subu " << RF[rd] << ":= " << RF[rs] << " - " << RF[rt] << endl;
                    if(rd==0) break;
                    RF[rd] = RF[rs] - RF[rt];
                    break;
                    }
                case 0x24:
                    {//and
                    cout << "AND " << RF[rd] << ":= " << RF[rs] << " & " << RF[rt] << endl;
                    if(rd==0) break;
                    RF[rd] = RF[rs]&RF[rt];
                    break;
                    }
                case 0x08:
                    {//jr
                    cout << "jr " << RF[rs]<<endl;
                    PC = RF[rs];
                    continue;
                    }
                case 0x27:
                    {//nor
                    cout << "nor " << RF[rd] << ":= ~(" << RF[rs] << " | " << RF[rt] <<")"<< endl;
                    if(rd==0) break;
                    RF[rd] = ~(RF[rs]|RF[rt]);
                    break;
                    }
                case 0x26:
                    {//xor
                        cout << "xor " << RF[rd] << ":= " << RF[rs] << " ^ " << RF[rt] << endl;
                        RF[rd] = RF[rs]^RF[rt];
                        break;
                    }
                case 0x25:
                    {//or
                    cout << "or " << RF[rd] << ":= " << RF[rs] << " | " << RF[rt] << endl;
                    if(rd==0) break;
                    RF[rd] = (RF[rs]|RF[rt]);
                    break;
                    }
                case 0x2a:
                    {//set less than
                    cout << "slt " << RF[rd] << "=1 if " << (int)RF[rs] << " < " << (int)RF[rt] << endl;
                    if(rd==0) break;
                    if(int(RF[rs])<int(RF[rt])) RF[rd] = 1;
                    else RF[rd] = 0;
                    break;
                    }
                case 0x2b:
                    {//set less than unsigned
                    cout << "sltu " << RF[rd] << "=1 if " << RF[rs] << " < " << RF[rt] << endl;
                    if(rd==0) break;
                    if((RF[rs])<(RF[rt])) RF[rd] = 1;
                    else RF[rd] = 0;
                    break;
                    }
                case 0:
                    {//shift left
                    cout << "sll " << RF[rd] << "= " << (int)RF[rt] << "^" << shamt << endl;
                    if(rd==0) break;
                    RF[rd] = RF[rt]<<shamt;
                    break;
                    }
                case 0x02:
                    {//shift right
                    cout << "srl " << RF[rd] << "= " << (int)RF[rt] << "/2stuff " << shamt << endl;
                    if(rd==0) break;
                    RF[rd] = RF[rt] >> shamt;
                    break;
                    }
                case 0x1a:
                    {//divide
                    cout << "divide " << (int)RF[rs] << "/ " << (int)RF[rt]  << endl;
                    LO=((int)RF[rs])/((int)RF[rt]); 
                    HI=((int)RF[rs])%((int)RF[rt]); 
                    break;
                    }
                case 0x1b:
                    {//divide unisgned 
                    cout << "divideu " << RF[rs] << "/ " << RF[rt]  << endl;
                    LO=(RF[rs])/(RF[rt]); 
                    HI=(RF[rs])%(RF[rt]); 
                    break;
                    }
                case 0x10:
                    {//move hi
                    cout << "movehi " << RF[rd] << " = " << HI <<endl;
                    RF[rd] = HI;
                    break;
                    }
                case 0x12:
                    {//move lo
                    cout << "movelo " << RF[rd] << " = " << LO <<endl;
                    RF[rd] = LO;
                    break;
                    }
                case 0x18:
                    {//multiply
                    cout << "mult " << (int)RF[rs] << "* " << (int)RF[rt]  << endl;
                    long temp = ((long)RF[rs] * (long)RF[rt]);
                    LO = (int)temp;
                    HI = (int)(temp>>32);
                    break;
                    }
                case 0x19:
                    {//multiplyu
                    cout << "multu " << RF[rs] << "* " << RF[rt]  << endl;
                    unsigned long temp = ((unsigned long)RF[rs] * (unsigned long)RF[rt]);
                    LO = (unsigned int)temp;
                    HI = (unsigned int)(temp>>32);
                    break;
                    }
                default:
                    cout << "not implemented yet or doesn't exist ;-;\n";
                    break;
            }
        }
        else{
            // cout << "not R" << endl;
            unsigned int rs = (command >> 21) & 0x1f;
            unsigned int rt = (command >> 16) & 0x1f;
            unsigned int immi = (command) & 0xffff;
            unsigned int address = (command) & 0x03ffffff;
            // cout << "rs: " << rs << endl;
            // cout << "rt: " << rt << endl;
            // cout << "immi: " << immi << endl;
            // cout << "address: " << address << endl;
            switch (OPCODE) {

                case 0x08:{
                    //addi
                    // immi = (immi <<16)>>16;
                    cout << "addi " << RF[rt] << " = " << (int)RF[rs] << " + "<<(short)immi << endl;
                    int temp1 = (int)RF[rs];
                    int temp2 = (short)immi;
                    if(rt==0) break;
                    RF[rt] = (int)RF[rs]+(short)immi;
                    if(temp1>0&&temp2>0&&RF[rt]<0){
                        cout << "overflowwwww" << endl;
                    }
                    else if(temp1<0&&temp2<0&&RF[rt]>0){
                        cout << "overflowwwww" << endl;
                    }
                    break;
                }
                case 0x09: {
                    //addiu
                    cout << "addiu " << RF[rt] << " = " << (unsigned int)RF[rs]<<" + "<<(unsigned short)immi << endl;
                    if(rt==0) break;
                    RF[rt] = (unsigned int)RF[rs]+(unsigned short)immi;
                    break;
                }
                case 0xc:{
                    //andi
                    cout << "addiu " << RF[rt] << " = " << (unsigned int)RF[rs]<<" & "<<(unsigned short)immi << endl;
                    if(rt==0) break;
                    RF[rt] = (unsigned int)RF[rs] & (unsigned short)immi;
                    break;
                }
                case 0x4:{
                    //branch on equal
                    cout << "beq " << RF[rs] << "=?" << RF[rt] <<endl;
                    if(RF[rs]==RF[rt]){
                        PC=PC+(short)immi;
                        continue;
                    }
                    break;
                }
                case 0x5:{
                    //branch on not equal
                    cout << "bne " << RF[rs] << "!=?" << RF[rt] <<endl;
                    if(RF[rs]!=RF[rt]){
                        PC=PC+(short)immi;
                        cout << "new PC: " << PC << endl;
                        continue;
                    }
                    break;
                }
                case 0x24:{
                    //lbu
                    cout << "lbu" << endl;
                    if(rt==0) break;
                    unsigned int byte=0;
                    for(int i=0;i<8;i++){
                        if(imem[i+RF[rs]+(short)immi]=='1'){
                            byte += 1<<(7-i);
                        }
                    }
                    RF[rt] = byte;
                    break;
                }
                case 0x23:{
                    //lw
                    cout << "lw" << endl;
                    unsigned int word=0;
                    for(int i=0;i<32;i++){
                        if(imem[i+RF[rs]+((short)immi)]=='1'){
                            word += 1<<(31-i);
                        }
                    }
                    if(rt==0) break;
                    RF[rt] = word;
                    break;
                }
                case 0xd:{
                    //ori
                    cout << "ori" << endl;
                    if(rt==0) break;
                    RF[rt] = (RF[rs]|immi);
                    break;
                }
                case 0xa:{
                    //set less than immi
                    cout << "slti" << endl;
                    if(rt==0) break;
                    if((int)RF[rs]<(short)immi){
                        RF[rt] = 1;
                    }
                    else RF[rt] = 0;
                    break;
                }
                case 0xb:{
                    //sltiu
                    cout << "sltiu" << endl;
                    if(rt==0) break;
                    if(RF[rs]<(unsigned short)immi){
                        RF[rt] = 1;
                    }
                    else RF[rt] = 0;
                    break;
                }
                case 0x28:{
                    //store byte
                    cout << "sb" << endl;
                    for(int i=0;i<8;i++){
                        imem[RF[rs]+immi+i]=(RF[rt]>>(7-i))%2;
                    }
                    break;
                }
                case 0x2b:{
                    //store word
                    cout << "sw" << endl;
                    for(int i=0;i<32;i++){
                        imem[RF[rs]+immi+i]=(RF[rt]>>(31-i))%2;
                    }
                    break;
                }
                //jump stuff
                case 0x2:{
                    //j
                    cout << "j" << endl;
                    PC = address;
                    continue;
                }
                case 0x3:{
                    //jal
                    cout << "jal" << endl;
                    RF[31] = PC+64;
                    PC = address;
                    continue;
                }
                default: {
                    cout << "not implemented yet or doesn't exist ;-;\n";
                    break;
                }
            }
        }
        cout <<endl<< "All regs changed: " << endl;
        for(int i=0;i<32;i++){
            if(preRF[i]!=RF[i]) cout << "reg" << i << ": " << RF[i] << endl;
            preRF[i] = RF[i];
        }
        
        cout << endl;
        PC+=32;
    }
    cout <<endl<< "All regs: " << endl;
    for(int i=0;i<32;i++){
        cout << "reg" << i << ": " << RF[i] << endl;
    }
    cout << endl;
}