#pragma once

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

enum Inst_Name {
	LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU,
	LB, LH, LW, LBU, LHU, SB, SH, SW, ADDI, SLTI, SLTIU, XORI, ORI,
	ANDI, SLLI, SRLI, SRAI, ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, ZERO
};
//hopefully my file will be accepted 

//--**--**--**--**--**--**--**

//所有的bool型：true为锁上，false为未锁
//处理器Control：存储当前各部分返回的指令
//1/**PC_lock
//******PC放在IF里，表示下一条要读取的指令位置
//******当发现有跳转指令时，PC--pause 直到Ex运算结束给IF返回通知：infor_unlock_PC
//2/**data hazard
//******ID阶段：
//*********1.判定锁：看rs1和rs2（如果有的话）是否被锁上
//******************如果不够两个，则写成rs1，0的形式
//***********若被锁上，则向control发出被锁的指令，control更改
//*********2.上锁：如果无需等待，则向control进行上锁
//******WB阶段：
//*********解锁：向control传输已经结束的rd的值
//**************由control进行解锁指令



//等等，，，我们不要乱写，，跑出来一堆0死循环算是什么，，，，，
//好的，以上注释全部作废，，程序里添加的也，，作废，，嘿嘿，，
//Forwarding+分支预测
//****Forwarding：：：
//Forwarding只要把计算结果及时的返回给EX即可
//最迟也可在本轮MA结束时知道结果，此时下一指令即将开始EX
//因为是倒着写出来的，所以不影响EX的执行
//唯一需要考虑load指令，取出来之后
//Forwarding是否需要lock？
//应该不需要吧？因为我只要把EX或MA计算结果返回给EX，EX看一下有没有重合的，这只需要考虑一个rd，，，
//如果前面两条：ID和EX都需要更新，而此时的指令是load，那么好像是，，需要把load到的东西用结构体的形式传输给EX和ID，，，，
//而如果此时是其他的在EX即可计算好的指令的话，，，直接更新后面的ID和自己EX（自己怎么跟自己传呢？）
//emmm似乎可以这样看：每轮EX或MA（视指令类型有所不同，一切以result为准）结束时，
//向controller上传自己计算的rd的值，之后ID立即调用contro的函数清除
//对应的，每次EX和ID在获取寄存器时，都要先从controller获取一个数据结构体，判断自己是否要更新
//Forwarding到此为止

//那么分支预测呢？
//分支预测应当是在IF读到跳转指令时进行的操作
//跳转包括JAL，JALR，B_Type
//其中JAL无条件跳转，不需要进行预测，，
//




//他喵的，，现在看，，我之前在想什么啊？？？
//woc写完再过来吐槽之前的神奇脑洞吧啊哈哈哈，，，
//“终有一日，现在的我们，也终将在未来成为某人手中的古典吧！”
//未完待续。。


//注意！！数据类型int /unsigned int？
//。。。。。。。。。。。。。。

//------------------------数据传输结构体：pass_oj-------------------**

class pass_oj
{
public:
	//所有可能用到的数据类型
	Inst_Name inst_name = ZERO;
	int imm = 0u;
	unsigned int opcode = 0u;
	unsigned int rd = 0u;
	unsigned int rs1 = 0u;
	int rs1_value = 0u;
	unsigned int rs2 = 0u;
	int rs2_value = 0u;
	unsigned int func3 = 0u;
	unsigned int func7 = 0u;
	unsigned int shamt = 0u;
	unsigned int instruction = 0u;
	unsigned int result = 0u;
	int tmp = 0u;
	int Pred_PC = 0u;
	int Real_PC = 0u;
	int jump_PC = 0u;
	int no_jump_PC = 0u;
	int cur_PC = 0u;

	pass_oj() {};
	~pass_oj() {};
	pass_oj operator=(const pass_oj &oj)
	{
		inst_name = oj.inst_name;
		imm = oj.imm;
		opcode = oj.opcode;
		rd = oj.rd;
		rs1 = oj.rs1;
		rs1_value = oj.rs1_value;
		rs2 = oj.rs2;
		rs2_value = oj.rs2_value;
		func3 = oj.func3;
		func7 = oj.func7;
		shamt = oj.shamt;
		instruction = oj.instruction;
		result = oj.result;
		tmp = oj.tmp;
		Pred_PC = oj.Pred_PC;
		Real_PC = oj.Real_PC;
		jump_PC = oj.jump_PC;
		no_jump_PC = oj.no_jump_PC;
		cur_PC = oj.cur_PC;
		return *this;
	}

};

//-----------------------创建一个空指令
//--------------------读到空指令时，五个步骤应该什么都不做，返回空指令

pass_oj reset()
{
	pass_oj zero_status;
	return zero_status;
}
void reset(pass_oj &cleaner)
{
	if (!(cleaner.inst_name == ZERO))
	{
		cleaner.inst_name = ZERO;
		cleaner.imm = 0u;
		cleaner.opcode = 0u;
		cleaner.rd = 0u;
		cleaner.rs1 = 0u;
		cleaner.rs1_value = 0u;
		cleaner.rs2 = 0u;
		cleaner.rs2_value = 0u;
		cleaner.func3 = 0u;
		cleaner.func7 = 0u;
		cleaner.shamt = 0u;
		cleaner.instruction = 0u;
		cleaner.result = 0u;
		cleaner.tmp = 0u;
		cleaner.Pred_PC = 0u;
		cleaner.Real_PC = 0u;
		cleaner.jump_PC = 0u;
		cleaner.no_jump_PC = 0u;
		cleaner.cur_PC = 0u;
	};
	return;

}


//寄存器 类
class TempStorage
{
public:
	int Register[32] = { 0 };
};
TempStorage reg;

int PC = 0;

int counter = 0;
bool flag = 1;//是否出现退出指令

//内存 类
class InterMem
{
public:
	uint8_t* Memery;
	//输入函数
	void readin()
	{
		char ch1;
		int pos = 0;
		int ins = 0;
		char read[20] = { 0 };
		while (cin >> read)
		{
			if (read[0] == '@')
			{
				pos = 0;
				int len = strlen(read);
				for (int i = 1; i < len; ++i)
				{
					if (i != 1)pos <<= 4;
					ch1 = read[i];
					if (ch1 >= '0'&&ch1 <= '9')
					{
						pos += (ch1 - '0');
					}
					else if (ch1 >= 'A'&&ch1 <= 'F')
					{
						pos += (ch1 - 'A') + 10;
					}

				}
				counter = pos;
			}
			else
			{
				ins = 0;
				int len = strlen(read);
				for (int i = 0; i < len; ++i)
				{
					if (i != 0)ins <<= 4;
					ch1 = read[i];
					if (ch1 >= '0'&&ch1 <= '9')
					{
						ins += (ch1 - '0');
					}
					else if (ch1 >= 'A'&&ch1 <= 'F')
					{
						ins += (ch1 - 'A') + 10;
					}


				}
				Memery[pos] = ins;
				/*cout << pos << " ";
				cout << ((Memery[pos] >>7)&1) << ((Memery[pos] >> 6) & 1) << ((Memery[pos] >> 5) & 1) << ((Memery[pos] >> 4) & 1) << ((Memery[pos] >> 3) & 1)
				<< ((Memery[pos] >> 2) & 1) << ((Memery[pos] >> 1) & 1)<< ((Memery[pos] ) & 1) << endl;*/
				pos++;
				counter++;
			}
			memset(read, 0, sizeof(read));
		}
		//cout << "4204 " << (int)Memery[4204] << endl;
		return;

	}
	InterMem()
	{
		Memery = new uint8_t[0x400000];
		memset(Memery, 0, 0x400000 * sizeof(uint8_t));
		//cout << "size " <<  sizeof(Memery) << endl;
		//cout << "4204 " << (int)Memery[4204] << endl;
	}
	~InterMem() {};
};

class RD_Renew
{
public:
	RD_Renew() {};
	~RD_Renew() {};

	class RD_Forward
	{
	public:
		RD_Forward() {};
		~RD_Forward() {};

		int rd = 0;
		int rd_value = 0;
	};
	RD_Forward cur_RD;

	void Check_And_Renew(pass_oj &id_end)
	{
		if (id_end.rs1 == cur_RD.rd&&id_end.rs1 != 0)
		{
			id_end.rs1_value = cur_RD.rd_value;
		}
		if (id_end.rs2 == cur_RD.rd&&id_end.rs2 != 0)
		{
			id_end.rs2_value = cur_RD.rd_value;
		}
		return;
	}

	void update(pass_oj &ex_end)
	{
		if (ex_end.rd != 0)
		{
			cur_RD.rd = ex_end.rd;
			cur_RD.rd_value = ex_end.result;
		}
		return;
	}

};
RD_Renew RD;

int wait_time = 0;

//----------------------------分支预测-------------------------**
int if_jump = 1;

int Pred_True = 0;
int Pred_False = 0;


int Guess(int jump, int no_jump)
{
	if (if_jump >= 0)return jump;
	return no_jump;
}

void FeedBack(int &if_jump,pass_oj &ex_end,pass_oj &id_end,pass_oj &if_end)
{
	if (ex_end.inst_name!=ZERO&&ex_end.Pred_PC != ex_end.Real_PC)
	{
		Pred_False++;
		if (ex_end.Pred_PC == ex_end.jump_PC)
		{
			PC = ex_end.no_jump_PC;
		}
		else if (ex_end.Pred_PC == ex_end.no_jump_PC)
		{
			PC = ex_end.jump_PC;
		}
		reset(id_end);
		reset(if_end);
	}
	else if(ex_end.inst_name != ZERO)
	{
		Pred_True++;
	}
	return;
}


//----------------------------符号扩展函数-------------------------**
int SignExtended(int data, int bits)//bits表示实际位数而非0-base
{
	if (data&(1 << (bits - 1)))
	{
		data |= (0xffffffff >> bits << bits);
	}
	return data;
}

//-------------------------------   Write_Back   过程------------------------**
class Write_Back
{
public:
	Write_Back() {};
	~Write_Back() {};

	void WB(pass_oj WB_end)
	{
		if (WB_end.inst_name == ZERO)
		{
			return;
		}
		switch (WB_end.inst_name)
		{
		case ADD:case SUB:case SLT:case SLTU:case XOR:
		case OR:case AND:case SLL:case SRL:case SRA:
		case LB:case LH:case LW:case LBU:case LHU:
		case ADDI:case SLTI:case SLTIU:case XORI:case ORI:
		case ANDI:case SLLI:case SRLI:case SRAI:case JALR:
		case LUI:case AUIPC:
		{
			if (WB_end.rd != 0)
			{
				reg.Register[WB_end.rd] = WB_end.result;
			}
			break;
		}
		case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:
		{
			PC = WB_end.result;
			break;
		}
		default:break;
		}
		return;
	}
};

//-------------------------------  Memory_Access 过程------------------------**
class Memory_Access
{
public:
	Memory_Access() {};
	~Memory_Access() {};

	pass_oj MA(pass_oj MA_end, InterMem &im)
	{
		if (MA_end.inst_name == ZERO)
		{
			return MA_end;
		}
		switch (MA_end.inst_name)
		{
			case SB:
			{
				if (MA_end.tmp != 0x30004)
					im.Memery[MA_end.tmp] = (MA_end.result & 255);
				else flag = 0;
				break;
			}
			case SH:
			{
				int tmp1 = MA_end.result;
				im.Memery[MA_end.tmp] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 1] = (tmp1 & 255);
				
				
				break;
			}
			case SW:
			{
				int tmp1 = MA_end.result;
				im.Memery[MA_end.tmp] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 1] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 2] = (tmp1 & 255);
				tmp1 >>= 8;
				im.Memery[MA_end.tmp + 3] = (tmp1 & 255);
				break;
			}
			case LB:
			{
				MA_end.result = (int)im.Memery[MA_end.tmp];
				break;
			}
			case LH:
			{
				MA_end.result = (int)((im.Memery[MA_end.tmp + 1] << 8) + im.Memery[MA_end.tmp]);
				break;
			}
			case LW:
			{
				MA_end.result = (int)((im.Memery[MA_end.tmp + 3] << 24) + (im.Memery[MA_end.tmp + 2] << 16) +
					(im.Memery[MA_end.tmp + 1] << 8) + im.Memery[MA_end.tmp]);
				break;
			}
			case LBU:
			{
				MA_end.result = ((uint32_t)im.Memery[MA_end.tmp]);
			}
			case LHU:
			{
				MA_end.result = ((uint32_t)(im.Memery[MA_end.tmp + 1] << 8) + (uint32_t)im.Memery[MA_end.tmp]);
				break;
			}
			default: break;
		}
			return MA_end;
	}

};

//-------------------------------    Execution   过程------------------------**
class Execution
{
public:
	Execution() {};
	~Execution() {};

	pass_oj Ex(pass_oj execution_end)
	{
		if (execution_end.inst_name == ZERO)
		{
			return execution_end;
		}
		switch (execution_end.inst_name)
		{
		//R_Type 10*****
		case ADD:
		{
			execution_end.result = execution_end.rs1_value + execution_end.rs2_value;
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SUB:
		{
			execution_end.result = execution_end.rs1_value - execution_end.rs2_value;
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SLT:
		{
			execution_end.result = (execution_end.rs1_value < execution_end.rs2_value) ? 1 : 0;
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SLTU:
		{
			execution_end.result = ((uint32_t)execution_end.rs1_value < (uint32_t)execution_end.rs2_value) ? 1 : 0;
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case XOR:
		{
			execution_end.result = (execution_end.rs1_value ^ execution_end.rs2_value);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case OR:
		{
			execution_end.result = (execution_end.rs1_value | execution_end.rs2_value);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case AND:
		{
			execution_end.result = (execution_end.rs1_value & execution_end.rs2_value);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SLL:
		{
			execution_end.tmp = (execution_end.rs2_value & 31u);
			execution_end.result = (execution_end.rs1_value << execution_end.tmp);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SRL:
		{
			execution_end.tmp = (execution_end.rs2_value & 31u);
			execution_end.result = ((uint32_t)execution_end.rs1_value >> execution_end.tmp);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;

			break;
		}
		case SRA:
		{
			execution_end.tmp = (execution_end.rs2_value & 31u);
			execution_end.result = SignExtended((execution_end.rs1_value >> execution_end.tmp), 32 - execution_end.tmp);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}

		//S_Type 9
		case BEQ:
		{
			execution_end.result = (execution_end.rs1_value == execution_end.rs2_value) ?
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			execution_end.Real_PC = execution_end.result;
			
			PC = execution_end.Real_PC;

			break;
		}
		case BNE:
		{
			execution_end.result = (execution_end.rs1_value == execution_end.rs2_value) ?
				(execution_end.no_jump_PC) : (execution_end.jump_PC);
			
			PC = execution_end.Real_PC;
			break;
		}
		case BLT:
		{
			execution_end.result = (execution_end.rs1_value < execution_end.rs2_value) ? 
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			
			PC = execution_end.Real_PC;
			break;
		}
		case BGE:
		{
			execution_end.result = (execution_end.rs1_value >= execution_end.rs2_value) ? 
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			
			PC = execution_end.Real_PC;
			break;
		}
		case BLTU:
		{
			unsigned int tmp1 = execution_end.rs1_value,
				tmp2 = execution_end.rs2_value;
			execution_end.result = (tmp1 < tmp2) ?
				(execution_end.jump_PC) : (execution_end.no_jump_PC);
			
			PC = execution_end.Real_PC;
			break;
		}
		case BGEU:
		{
			unsigned int tmp1 = execution_end.rs1_value,
				tmp2 = execution_end.rs2_value;
			execution_end.result = (tmp1 >= tmp2) ?
				(execution_end.jump_PC) : (execution_end.no_jump_PC);

			PC = execution_end.Real_PC;
			break;

		}
		case SB:
		{
			execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			execution_end.result = (execution_end.rs2_value & 255);
			break;
		}
		case SH:
		{
			execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			execution_end.result = (execution_end.rs2_value & 0xffff);
			break;
		}
		case SW:
		{
			execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			execution_end.result = (execution_end.rs2_value & 0xffffffff);
			break;
		}
		//I_Type 16
		case JAL:
		{
			break;
		}
		case JALR:
		{
			execution_end.result = execution_end.cur_PC + 4;
			PC = ((execution_end.rs1_value + execution_end.imm) >> 1 << 1);
			break;
		}
		case LB:case LH:case LW:case LBU:case LHU:
		{
			if (execution_end.rd != 0)
				execution_end.tmp = execution_end.rs1_value + execution_end.imm;
			break;
		}
		case ADDI:
		{
			execution_end.result = (execution_end.rs1_value +execution_end.imm);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SLTI:
		{
			execution_end.result = (execution_end.rs1_value < execution_end.imm)?1:0;
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SLTIU:
		{
			execution_end.result = ((uint32_t)execution_end.rs1_value < (uint32_t)execution_end.imm) ? 1 : 0;
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case XORI:
		{
			execution_end.result = (execution_end.rs1_value & execution_end.imm);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case ORI:
		{
			execution_end.result = (execution_end.rs1_value | execution_end.imm);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case ANDI:
		{
			execution_end.result = (execution_end.rs1_value & execution_end.imm);
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SLLI:
		{
			execution_end.result = (execution_end.rs1_value << (execution_end.shamt&31u));
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SRLI:
		{
			execution_end.result = (execution_end.rs1_value >> (execution_end.shamt & 31u));
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case SRAI:
		{
			execution_end.result = SignExtended(execution_end.rs1_value >> (execution_end.shamt & 31u), 32-(execution_end.shamt & 31u));
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		//U_Type 2******
		case LUI:
		{
			execution_end.result = (execution_end.imm );
			RD.cur_RD.rd = execution_end.rd;
			RD.cur_RD.rd_value = execution_end.result;
			break;
		}
		case AUIPC:
		{
			if (execution_end.rd != 0)
			{
				PC = execution_end.cur_PC + (execution_end.imm << 12);
				execution_end.result = PC;

				RD.cur_RD.rd = execution_end.rd;
				RD.cur_RD.rd_value = execution_end.result;
			}

		}
		}
		return execution_end;
	}

};

//-------------------------------     Decode     过程------------------------**

class Instruction_Decode
{
public:
	Instruction_Decode() {};
	~Instruction_Decode() {};

	unsigned int instruction = 0;
	
	pass_oj Decode(pass_oj decode_end)
	{
		if (decode_end.inst_name == ZERO)
		{
			return decode_end;
		}

		switch (decode_end.inst_name)
		{
		case ADD:case SUB:case SLT:case SLTU:case XOR:
		case OR:case AND:case SLL:case SRL:case SRA:
		case BEQ:case BNE:case BLT:case BGE:
		case BLTU:case BGEU:
		case SB:case SH:case SW:
				decode_end.rs1_value = reg.Register[decode_end.rs1];
				decode_end.rs2_value = reg.Register[decode_end.rs2];
				
			break;

		case JALR:case LB:case LH:case LW:case LBU:case LHU:
		case ADDI:case SLTI:case SLTIU:case XORI:case ORI:case ANDI:
		case SLLI:case SRLI:case SRAI:
				decode_end.rs1_value = reg.Register[decode_end.rs1];
			
			break;
		default :break;
		}

		return decode_end;
	}

};

//-------------------------------      Fetch     过程------------------------**
class Instruction_Fetch
{
public:
	unsigned int instruction = 0;

	Instruction_Fetch() {};
	~Instruction_Fetch() {};

	void Fetch(InterMem &im)
	{
		instruction = (im.Memery[PC + 3] << 24) + (im.Memery[PC + 2] << 16)
				+ (im.Memery[PC + 1] << 8) + im.Memery[PC];
		PC += 4;
		return;
	}

	pass_oj R_()
	{
		unsigned int tmp = instruction;

		unsigned int opcode = (tmp & 127);
		tmp >>= 7;

		unsigned int rd = (tmp & 31);
		tmp >>= 5;

		unsigned int func3 = (tmp & 7);
		tmp >>= 3;

		unsigned int rs1 = (tmp & 31);
		tmp >>= 5;

		unsigned int rs2 = (tmp & 31);
		tmp >>= 5;

		unsigned int func7 = (tmp & 127);

		pass_oj fetch_end;
		fetch_end.cur_PC = PC - 4;
		fetch_end.rd = rd;
		fetch_end.rd = rs1;
		fetch_end.rd = rs2;

		if (func3 == 0)
		{
			if (func7 == 0)fetch_end.inst_name = ADD;
			else fetch_end.inst_name = SUB;
		}
		else if (func3 == 1)fetch_end.inst_name = SLL;
		else if (func3 == 2)fetch_end.inst_name = SLT;
		else if (func3 == 3)fetch_end.inst_name = SLTU;
		else if (func3 == 4)fetch_end.inst_name = XOR;
		else if (func3 == 5)
		{
			if (func7 == 0)fetch_end.inst_name = SRL;
			else fetch_end.inst_name = SRA;
		}
		else if (func3 == 6)fetch_end.inst_name = OR;
		else if (func3 == 7)fetch_end.inst_name = AND;
		return fetch_end;
	}
	pass_oj I_()
	{
		unsigned int tmp = instruction;

		unsigned int opcode = (tmp & 127);
		tmp = (tmp >> 7);
		pass_oj fetch_end ;
		fetch_end.cur_PC = PC - 4;

		if (opcode == 3)
		{
			unsigned int rd = (tmp & 31);
			tmp >>= 5;

			unsigned int func3 = (tmp & 7);
			tmp >>= 3;

			unsigned int rs1 = (tmp & 31);
			tmp >>= 5;

			unsigned int imm1 = (tmp&(4095));
			int imm = SignExtended(imm1, 12);
			
			fetch_end.rd = rd;
			fetch_end.rs1 = rs1;
			fetch_end.imm = imm;

			wait_time = 4;

			switch (func3)
			{
			case 0:fetch_end.inst_name = LB; break;
			case 1:fetch_end.inst_name = LH; break;
			case 2:fetch_end.inst_name = LW; break;
			case 4:fetch_end.inst_name = LBU; break;
			case 5:fetch_end.inst_name = LHU; break;
			}
		}
		else if (opcode == 19)
		{
			unsigned int rd = (tmp & 31);
			tmp >>= 5;

			unsigned int func3 = (tmp & 7);
			tmp >>= 3;

			unsigned int rs1 = (tmp & 31);
			tmp >>= 5;

			if (func3 == 0)
			{
				unsigned int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = ADDI;
			}
			else if (func3 == 2)
			{
				unsigned int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = SLTI;
			}
			else if (func3 == 3)
			{
				unsigned int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = SLTIU;
			}
			else if (func3 == 4)
			{
				unsigned int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = XORI;
			}
			else if (func3 == 6)
			{
				unsigned int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = ORI;
			}
			else if (func3 == 7)
			{
				unsigned int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				fetch_end.imm = imm;
				fetch_end.rs1 = rs1;
				fetch_end.rd = rd;
				fetch_end.inst_name = ANDI;
			}
			else
			{
				unsigned int shamt1 = (tmp & 31);
				int shamt = SignExtended(shamt1, 5);
				tmp >>= 5;
				int func7 = (tmp & 127);
				if (func3 == 1)
				{
					fetch_end.rd = rd;
					fetch_end.rs1 = rs1;
					fetch_end.shamt = shamt;
					fetch_end.inst_name = SLLI;
				}
				else if (func7 == 0)
				{
					fetch_end.rd = rd;
					fetch_end.rs1 = rs1;
					fetch_end.shamt = shamt;
					fetch_end.inst_name = SRLI;
				}
				else
				{
					fetch_end.rd = rd;
					fetch_end.rs1 = rs1;
					fetch_end.shamt = shamt;
					fetch_end.inst_name = SRAI;
				}

			}

		}
		else if (opcode == 111)
		{
			unsigned int rd = (tmp & 31);
			tmp >>= 5;
			unsigned int imm1 = (tmp & 255);
			tmp >>= 8;
			unsigned int imm2 = (tmp & 1);
			tmp >>= 1;
			unsigned int imm3 = (tmp & 1023);
			tmp >>= 10;
			unsigned int imm4 = (tmp & 1);
			unsigned int imm0 = 0;
			imm0 += (imm4 << 20);
			imm0 += (imm3 << 1);
			imm0 += (imm2 << 11);
			imm0 += (imm1 << 12);
			int imm = SignExtended(imm0, 21);

			fetch_end.rd = rd;
			fetch_end.imm = imm;
			fetch_end.inst_name = JAL;

			fetch_end.result = fetch_end.cur_PC + 4;

			reg.Register[fetch_end.rd] = fetch_end.result;

			PC = fetch_end.cur_PC + fetch_end.imm;
		}
		else
		{
			unsigned int rd = (tmp & 31);
			tmp >>= 5;
			tmp >>= 3;
			unsigned int rs1 = (tmp & 31);
			tmp >>= 5;
			unsigned int imm1 = (tmp & 4095);
			int imm = SignExtended(imm1, 12);

			wait_time = 4;
			fetch_end.rd = rd;
			fetch_end.rs1 = rs1;
			fetch_end.imm = imm;
			
			fetch_end.inst_name = JALR;
		}
		return fetch_end;
	}
	pass_oj U_()
	{
		unsigned int tmp = instruction;
		unsigned int opcode = (tmp & 127);
		tmp >>= 7;

		unsigned int rd = (tmp & 31);
		tmp >>= 5;

		int imm = (((tmp & 0xfffff) << 12)&(0xffffffff >> 12 << 12));

		pass_oj fetch_end;
		fetch_end.cur_PC = PC - 4;
		if (opcode == 55)
		{
			fetch_end.rd = rd;
			fetch_end.imm = imm;
			fetch_end.inst_name = LUI;
		}
		else
		{
			wait_time = 4;

			fetch_end.rd = rd;
			fetch_end.imm = imm;
			fetch_end.inst_name = AUIPC;
		}
		return fetch_end;

	};
	pass_oj S_()
	{
		unsigned int tmp = instruction;
		unsigned int opcode = (tmp & 127);
		tmp >>= 7;

		unsigned int imm1 = (tmp & 31);
		tmp >>= 5;

		unsigned int func3 = (tmp & 7);
		tmp >>= 3;

		unsigned int rs1 = (tmp & 31);
		tmp >>= 5;

		unsigned int rs2 = (tmp & 31);
		tmp >>= 5;

		unsigned int imm2 = (tmp & 127);

		pass_oj fetch_end;
		fetch_end.cur_PC = PC - 4;
		if (opcode == 99)
		{
			unsigned int imm0 = 0;
			imm0 += ((imm1 & 1) << 11);
			imm1 >>= 1;
			imm0 += ((imm1 & 15) << 1);
			imm0 += ((imm2 & 63) << 5);
			imm2 >>= 6;
			imm0 += ((imm2 & 1) << 12);
			imm0 = (imm0&(0xffffffff >> 1 << 1));
			int imm = SignExtended(imm0, 13);

			fetch_end.rs1 = rs1;
			fetch_end.rs2 = rs2;
			fetch_end.imm = imm;

			//分支预测
			fetch_end.jump_PC = fetch_end.cur_PC + fetch_end.imm;
			fetch_end.no_jump_PC = fetch_end.cur_PC + 4;
			PC = Guess(fetch_end.jump_PC, fetch_end.no_jump_PC);
			fetch_end.Pred_PC = PC;

			switch (func3)
			{
			case 0:fetch_end.inst_name = BEQ; break;
			case 1:fetch_end.inst_name = BNE; break;
			case 4:fetch_end.inst_name = BLT; break;
			case 5:fetch_end.inst_name = BGE; break;
			case 6:fetch_end.inst_name = BLTU; break;
			case 7:fetch_end.inst_name = BGEU; break;
			}
			
		}
		else
		{
			unsigned int imm0 = 0;
			imm0 += (imm1 & 31);
			imm0 += ((imm2 & 127) << 5);
			int imm = SignExtended(imm0, 12);

			fetch_end.rs1 = rs1;
			fetch_end.rs2 = rs2;
			fetch_end.imm = imm;

			switch (func3)
			{
			case 0:fetch_end.inst_name = SB; break;
			case 1:fetch_end.inst_name = SH; break;
			case 2:fetch_end.inst_name = SW; break;
			}
		}
		return fetch_end;
	};

	pass_oj fetcher(InterMem &im)
	{
		pass_oj fetch_end;
		Fetch(im);
		fetch_end.opcode = (instruction & 127);

		switch (fetch_end.opcode)
		{
		case 51:fetch_end = R_(); break;
		case 111:case 103:case 3:case 19:fetch_end = I_(); break;
		case 99:case 35:fetch_end = S_(); break;
		case 55:case 23:fetch_end = U_(); break;
		}	
		return fetch_end;
	}
};
