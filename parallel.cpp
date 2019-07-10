#include "parallel.h"
using namespace std;
InterMem im;
Instruction_Fetch my_fet;
Instruction_Decode my_dec;
Execution my_ex;
Memory_Access my_ma;
Write_Back my_wb;


int main()
{
	freopen("naive.data", "r", stdin);
	im.readin();
	int cnt = 1;
	//交接结构
	pass_oj if_end, id_end, ex_end, ma_end;

	//五级流水
	while (1)
	{

		RD.Check_And_Renew(id_end);
		RD.Check_And_Renew(if_end);

		my_wb.WB(ma_end);
		reset(ma_end);

		ma_end = my_ma.MA(ex_end, im);	
		reset(ex_end);

		ex_end = my_ex.Ex(id_end);

		FeedBack(if_jump, ex_end, id_end, if_end);

		reset(id_end);

		id_end = my_dec.Decode(if_end);
		reset(if_end);

		if (wait_time <= 0)
		{
			if_end = my_fet.fetcher(im);
		}
		if (wait_time > 0)wait_time--;
		else wait_time = 0;

		RD.update(ex_end);
		RD.Check_And_Renew(if_end);
		
		if (!flag)
		{
			cout << (reg.Register[10] & 255u);
			break;
		}

		cout << cnt << " ";

		cout<< PC << " ";

		for (int i = 1; i <= 31; ++i)

			cout << reg.Register[i] << " ";

		cout << endl;
		++cnt;
	}

	return 0;
}