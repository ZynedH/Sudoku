#include<iostream>

#include<windows.h>
using namespace std;
#define VALUEINIT 223092870;
SYSTEMTIME sys;

struct elem {
public:
	int value;
	elem* r_next;//行的next结点
	elem* l_next;//列的next结点
	elem* b_next;//块的next结点
};

int two_num[36] = { 6,10,14,15,21,22,26,33,34,35,38,39,46,51,55,57,65,69,77,85,91,95,113,119,133,143,161,187,209,221,247,253,299,323,391,437 };
int map[24] = { -1,0,1,2,0,3,0,4,0,0,0,5,0,6,0,0,0,7,0,8,0,0,0,9 };
int chongtu[9] = { 4,9,25,49,121,169,289,361,529 };
elem element[81];//最终形成的结构，由81个elem结构组成，内含27个交叉的循环链表是一个图//

//*将数据输入放入到input，作为init参数*/
void num_input(int table[81]);
//*init，数组数据全部填入数独9x9矩阵中*/
void init_step_one(int table[81],int input[81]);
//*将数独9x9矩阵的框架搭好*/
void init_step_two(int table[81],elem element[81]);
//*由于对一块一块的9个数据不好用循环来操作，故写在此函数中，被init_step_two调用*/
void step_two_block_init(elem element[81]);
//*对横向row指针的操作*/
void row_operation(elem e);
//*对纵向list指针的操作*/
void list_operation(elem e);
//*对块内block指针的操作*/
void block_operation(elem e);
//*一级操作的集合,调用三个一级操作函数对数独矩阵数据操作*/
void sudoku_operation(elem e);
//*sudoku_operation的启动函数，逻辑上规定只有它能调用sudoku_operation*/
void start_operation(elem table[81]);
//*第二级的行row指针操作*//
void row_deap_operation(elem table[81]);
//*第二级的列list指针操作*//
void list_deap_operation(elem table[81]);
//*第二级的块block指针操作*//
void block_deap_operation(elem table[81]);
//**求公因数*/
int get_gcd(int i, int j);
//复制数独
elem* sudo_copy(elem table[81]);
//*尝试函数，递归方式*/
void try_field(elem table[81], int location, int t[10]);

//*标准输出*/
//1/
void standard_print(elem element[81]) {
	for (int i = 0; i < 81; i++) {
		if (i % 9) {
			if (element[i].value <= 23) {
				cout << map[element[i].value] << "  ";
			}
			else { cout << map[1] << "  "; }
		}
		else {
			if (element[i].value <= 23) {
				cout << endl << map[element[i].value] << "  ";
			}
			else { cout << endl << map[1] << "  "; }
		}
	}
	cout << endl << "------------------------------------标准输出--------------------------------" << endl;
}
//*实际输出*/
//1/
void real_print(elem element[81]) {
	for (int i = 0; i < 81; i++) {
		if (i % 9) {
			cout << element[i].value << "  ";
		}
		else {
			cout << endl << element[i].value << "  ";
		}
	}
	cout << endl << "-----------------------------------实际输出---------------------------------" << endl;
}
//*测试输入函数*/
//1/
int* gcd_array(int value);
void num_input_test(int input[81]) {
	int mapping[10] = { 223092870,2,3,5,7,11,13,17,19,23 };
	for (int i = 0; i < 81; i++) {
		input[i] = mapping[input[i]];
	}
}
bool _check_three(long int  num) {
	for (int i = 0; i < 9; i++) {
		if (num % chongtu[i] == 0) {
			return false;
		}
		else {}
	}
	return true;
}
//*检验函数，检验数独矩阵任一元素的value为23及23以内的质数，且每一行列块都是这样，则返回true*/
//1/
bool _check(elem table[81]) {
	int* array;
	int a[10]={ 0,3,6,27,30,33,54,57,60,61 };
	array = a;
	for (int i = 0; i < 9; i++) {
		long int k1 = 1;
		long int k2 = 1;
		for (int j = 0; j < 9; j++) {
			if (table[9 * i + j].value <= 23 && table[9 * i + j].value != 6 && table[9 * i + j].value != 10 && table[9 * i + j].value != 14 && table[9 * i + j].value != 22 && table[9 * i + j].value != 15 && table[9 * i + j].value != 21) {
				k1 *= table[9 * i + j].value;
			}
			else { return false; }
			if (table[9 * j + i].value <= 23 && table[9 * j + i].value != 6 && table[9 * j + i].value != 10 && table[9 * j + i].value != 14 && table[9 * j + i].value != 22 && table[9 * j + i].value != 15 && table[9 * j + i].value != 21) {
				k2 *= table[i + j * 9].value;
			}
			else { return false; }
		}
		if (!(_check_three(k1) && _check_three(k2))) {
			return false;
		}
		else {}
	}
	for (int k=0; k <= 60;) {
		long int k3 = 1;
		elem* p = &table[k];
		for (int m = 0; m < 9; m++) {
			if (p->value <= 23 && p->value != 6 && p->value != 10 && p->value != 14 && p->value != 22 && p->value != 15 && p->value != 21) {
				k3 *= p->value;
				p = p->b_next;
			}
			else { return false; }
		}
		if (!_check_three(k3)) {
			return false;
		}
		else {
			k = *array;
			array++;
		}
	}
	return true;
}
bool _check_check(elem table[81]) {
	for (int i = 0; i < 81; i++) {
		if (table[i].value <= 23 && table[i].value != 6 && table[i].value != 10 && table[i].value != 14 && table[i].value != 22 && table[i].value != 15 && table[i].value != 21) {}
		else { return false; }
	}
	return true;
}
bool _check_two(int value) {
	for (int i = 0; i < 36; i++) {
		if (value == two_num[i]) return true;
	}
	return false;
}

int main() {
	GetLocalTime(&sys);
	cout <<"起始时间："<< sys.wSecond <<'.'<< sys.wMilliseconds << endl;
	int sudoku[81] = { 0 };
	
	//int input[81] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0,0,0,0,0,0,8,2,9,0,0,3,0,0,6,0,0,0,0,0,1,0,0,3,0,4,0,0,0,2,9,5,0,6,0,0,0,3,0,0,0,4,0,0,0,0,8,5,0,1,0,0,3,4,6,1,0,0,2,0,9,0,0 };//input是输入，例子如下1
	//int input[81] = { 0,6,7,0,9,4,0,8,2,0,9,4,1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,0,0,0,0,0,6,1,0,5,0,7,0,0,6,0,0,0,0,0,0,5,2,0,0,9,0,0,0,0,6,0,7,9,0,2,3 };//input是输入，例子如下2
	//int input[81] = { 0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,9,0,0,0,8,0,3,0,0,0,5,0,0,0,0,0,0,0,0,3,0,9,0,0,0,2,0,0,0,0,0,3,5,0,8,7,0,4,3,0,0,2,0,0,0,5,7,0,8,4,0,6,3,0,0,0,0,0,0,0,0,0,0,0,0 };//input是输入，例子如下3
	//int input[81] = { 0,0,0,0,0,0,0,0,0,0,5,0,0,0,9,0,1,3,0,0,8,7,0,1,9,2,0,0,0,4,0,8,0,0,0,0,0,8,5,0,0,0,0,6,1,0,9,2,3,0,4,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,9,2,0,0,0,9,0,0,0,8,6 };//input是输入，例子如下4
	//int input[81] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,1,0,2,0,0,0,0,6,0,9,8,7,0,0,8,0,0,2,0,0,3,0,0,7,0,0,5,0,2,0,0,0,2,0,9,3,0,1,0,0,5,0,0,1,0,0,0,6,0,2,6,0,5,0,8,3,0,7,9,4,0,0,0,0,0,0 };//input是输入，例子如下5
	//int input[81] = { 0,0,9,0,0,2,0,0,3,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,4,0,0,2,0,9,0,3,0,5,0,0,1,3,0,8,9,0,0,0,4,0,0,0,5,8,9,1,0,5,3,1,7,0,0,2,0,0,0,0,0,0,0,0,0,0 };//input是输入，例子如下6
	//int input[81] = { 0,0,0,0,0,7,0,5,6,0,0,6,4,0,0,3,2,0,0,5,0,3,0,2,9,8,0,8,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,5,0,0,3,0,2,0,0,0,0,0,0,0,0,0,9,4,0,0,0,6,0,8,4,0,0,3,0 };//input是输入，例子如下7
	//int input[81] = { 0,0,0,0,7,0,5,0,0,0,5,7,8,0,0,0,3,9,2,3,0,9,0,5,4,0,0,8,0,5,2,0,3,0,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,7,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,9,6,0,0,0,4,0,0,3,0,8 };//input是输入，例子如下8
	//int input[81] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,4,0,0,0,0,5,9,2,7,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,7,0,1,0,0,4,0,5,3,0,0,9,2,0,7,6,0,2,4,9,0,5,0,3,2,0,7,1,0,4,0,0,0,0,0,0,0,0,0,0 };//input是输入，例子如下9
	//int input[81] = { 0,0,1,0,8,0,0,0,7,3,0,0,0,0,0,4,0,9,4,0,0,0,1,0,5,3,0,0,0,6,3,0,9,8,0,4,5,4,0,1,0,0,0,0,0,0,0,9,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,3,8,0,0,0,0,0,3,5,1,0,0 };//input是输入，例子如下10
	//int input[81] = { 5,0,4,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,5,1,0,0,0,0,0,6,0,0,0,0,0,0,0,8,0,0,0,9,0,6,0,5,0,0,3,0,7,0,0,8,0,9,0,0,6,0,0,2,0,0,4,0,0,3,0,0,0,9,0,6,0,4,0,5 };//input是输入，例子如下11
	//int input[81] = { 0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,4,8,0,2,0,9,0,0,8,1,0,7,6,0,5,0,0,9,0,0,4,0,0,0,3,0,4,2,0,5,1,8,2,0,6,7,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5 };//input是输入int input[81] = { 0,9,0,0,6,0,0,4,1,0,4,3,0,2,0,8,7,0,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,8,0,0,0,0,0,9,0,5,6,0,0,8,0,0,0,0,0,7,0,0,9,0,0,4,5,6,0,0,0,5,7,0,3,9,0,4 };//input是输入，例子如下13
	//int input[81] = { 0,0,8,0,0,4,0,0,0,0,0,0,0,0,9,0,0,7,0,0,0,5,0,0,0,4,6,0,3,1,0,4,0,0,6,0,0,8,0,0,2,1,0,7,0,0,0,6,0,3,0,0,9,5,2,9,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,2 };//input是输入，例子如下14
	int input[81] = { 0,6,4,5,7,1,2,3,0,1,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,7,8,1,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,3,0,0,5,0,8,0,0,0,0,0,0,0,0,0,6,0,0,5,0,0,9,4,1,0,0,0,0,0,7,0,0,5,0,9,6,0 };//input是输入，例子如下15
	
	
	num_input_test(input);
	//num_input(input);//
	init_step_one(sudoku,input);
	init_step_two(sudoku,element);
	//标准输出//
	cout << "Question :"<<endl;
	standard_print(element);
	for (int i = 0; i < 7; i++) {
		start_operation(element);
		row_deap_operation(element);
		list_deap_operation(element);
		block_deap_operation(element);
	}
	if (_check(element) && _check_check(element)) {
		cout << "Solution :" << endl;
		standard_print(element);
		GetLocalTime(&sys);
		cout << "结束时间：" << sys.wSecond << '.' << sys.wMilliseconds << endl;
		exit(0);
	}
	
	int* gcd = {};	
	for (int i = 0; i < 81; i++) {
		if(_check_two(element[i].value)==0) {}
		else { 
			gcd = gcd_array(element[i].value); 
			/*cout << "此时尝试的位置为"<<i << endl;*/
			try_field(element,i, gcd);
		}
	}

	return 0;
}

//*将数据输入放入到input，作为init参数*/
//1/
void num_input(int input[81]) {
	//123456789九个数字的质数映射/
	int mapping[9] = { 2,3,5,7,11,13,17,19,23 };
	for (int i = 0; i < 9; i++) {
		int b[9] = { 0,0,0,0,0,0,0,0,0 };
		cout << "输入数字" << i+1 << "的坐标：" << endl;
		for (int j = 0; j < 9; j++) {
			cin >> b[j];
			if (!b[j]) { break; }
		}//当输入为0时跳到下一个输入/
		for (int k = 0; b[k]&&k<9;k++) {
			input[b[k]-1] = mapping[i];
		}
	}
}

//*init*/
//1/
void init_step_one(int table[81],int input[81]) {
	int i = 0;
	while (i < 81) {
		if (input[i]) {
			table[i] = input[i];
			i++;
		}
		else {
			table[i] = VALUEINIT;
			i++;
		}
	}
}

//*将数据的结构组合好*/
//1/
void init_step_two(int table[81],elem element[81]) {
	for (int i = 0; i < 81; i++) {
		element[i].value = table[i];
	}//给结构体赋输入后的值
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			element[i * 9 + j].r_next = &element[i * 9 + j + 1];//写入冲突
		}
		element[i * 9 + 8].r_next = &element[i * 9];
	}//每一行形成环形链表，有危险
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			element[j * 9 + i].l_next = &element[(j+1) * 9 + i ];
		}
		element[i + 72].l_next = &element[i];
	}//每一列形成环形链表，有危险
	step_two_block_init(element);//每一块形成环形列表，有危险
}

//*块内元素形成环链*/
//1/
void step_two_block_init( elem element[81]) {
	for (int k = 0; k <= 54; k += 27) {
		for (int j = 0; j <= 6; j += 3) {
			int i = k + j;
			element[i].b_next = &element[i + 1];
			element[i + 1].b_next = &element[i + 2];
			element[i + 2].b_next = &element[i + 9];
			i = i + 9;
			element[i].b_next = &element[i + 1];
			element[i + 1].b_next = &element[i + 2];
			element[i + 2].b_next = &element[i + 9];
			i = i + 9;
			element[i].b_next = &element[i + 1];
			element[i + 1].b_next = &element[i + 2];
			element[i + 2].b_next = &element[i - 18];
		}
	}
};



//*对横向row指针的操作*/
//1/
void row_operation(elem e) {
	elem* point=e.r_next;
	int dev = e.value;
	for (int i = 0; i < 8; i++) {
		if (point->value != dev &&( point->value % dev == 0))
		{
			point->value = point->value / dev;
			point = point->r_next;
		}
		else {
			point = point->r_next;
		}
	}
}

//*对纵向list指针的操作*/
//1/
void list_operation(elem e) {
	elem* point = e.l_next;
	int dev = e.value;
	for (int i = 0; i < 8; i++) {
		if (point->value != dev && (point->value % dev == 0))
		{
			point->value = point->value / dev;
			point = point->l_next;
		}
		else {
			point = point->l_next;
		}
	}
}

//*对块内block指针的操作*/
//1/
void block_operation(elem e) {
	elem* point = e.b_next;
	int dev = e.value;
	for (int i = 0; i < 8; i++) {
		if (point->value != dev && (point->value % dev == 0))
		{
			point->value = point->value / dev;
			point = point->b_next;
		}
		else { 
			point = point->b_next; 
		}
	}
}

//*操作函数sudoku_operation的启动函数*/
//1/
void start_operation(elem table[81]) {
	for (int i = 0; i < 81; i++)
	{
		if (table[i].value <= 23 && table[i].value != 6 && table[i].value != 10 && table[i].value != 14 && table[i].value != 22 && table[i].value != 15 && table[i].value != 21) {
			sudoku_operation(table[i]);
		}
		else {}
	}
}

//*一级操作的集合*/
//1/
void sudoku_operation(elem e) {
	row_operation(e);
	list_operation(e);
	block_operation(e);
}


//*第二级的行row指针操作*/
//1/
void row_deap_operation(elem table[81]) {
	for (int i = 0; i < 81; i++) {
			if (table[i].value <= 23 && table[i].value != 6 && table[i].value != 10 && table[i].value != 14 && table[i].value != 22 && table[i].value != 15 && table[i].value != 21) {}
			else {
				int loney = table[i].value;
				elem* p = table[i].r_next;
				for (int i = 0; i < 8; i++) {
					int gcd = get_gcd(p->value, loney);
					loney /= gcd;
					p = p->r_next;
				}
				if (loney <= 23 && loney != 1 && loney != 6 && loney != 10 && loney != 14 && loney != 22 && loney != 15 && loney != 21) {
					p->value = loney;
				}
				else {}
			}
		}
	}

//*第二级的列list指针操作*/
//1/
void list_deap_operation(elem table[81]) {
	for (int i = 0; i < 81; i++) {
		if (table[i].value <= 23 && table[i].value != 6 && table[i].value != 10 && table[i].value != 14 && table[i].value != 22 && table[i].value != 15 && table[i].value != 21) {}
		else {
			int loney = table[i].value;
			elem* p = table[i].l_next;
			for (int i = 0; i < 8; i++) {
				int gcd = get_gcd(p->value, loney);
				loney /= gcd;
				p = p->l_next;
			}
			if (loney <= 23 && loney != 1 && loney != 6 && loney != 10 && loney != 14 && loney != 22 && loney != 15 && loney != 21) {
				p->value = loney;
			}
			else {}
		}
	}
}

//*第二级块block指针操作*/
//1/
void block_deap_operation(elem table[81]) {
	for (int i = 0; i < 81; i++) {
		if (table[i].value <= 23 && table[i].value != 6 && table[i].value != 10 && table[i].value != 14 && table[i].value != 22 && table[i].value != 15 && table[i].value != 21) {}
		else {
			int loney = table[i].value;
			elem* p = table[i].b_next;
			for (int i = 0; i < 8; i++) {
				int gcd = get_gcd(p->value, loney);
				loney /= gcd;
				p = p->b_next;
			}
			if (loney <= 23 && loney != 1 && loney != 6 && loney != 10 && loney != 14 && loney != 22 && loney != 15 && loney != 21) {
				p->value = loney;
			}
			else {}
		}
	}
}

//*求公因数*/
//1/
int get_gcd(int k, int l) {
	int i = k;
	int j = l;
	if (i > j) {
		static int a;
		a = i % j;
		i = j;
		j = a;
		if (!a) {
			return i;
		}
		else {
			get_gcd(i, j);
		}
	}
	else if (i == j) { return i; }
	else {
		get_gcd(j, i);
	}
}


//*第三级解决方案函数集*/

//*递归检验方法*/
//1/
void try_field(elem table[81],int location, int t[10]) {
	elem* try_element;
	try_element = sudo_copy(table);
	int i = 0;
	while (!_check(try_element) && t[i] != 0) {
		try_element = sudo_copy(table);
		(try_element + location)->value = t[i];
		i++;
		for (int j = 0; j < 8; j++) {
			//一级操作
			start_operation(try_element);
			//二级操作
			row_deap_operation(try_element);
			list_deap_operation(try_element);
			block_deap_operation(try_element);
		}
		if (_check(try_element)) { 
			if (_check_check(try_element)) {
				elem* d = element;
				elem* s = try_element;
				for (int j = 0; j < 81; j++) {
					d++->value = s++->value;
				}
				cout << "Solution :" << endl;
				standard_print(element);
				GetLocalTime(&sys);
				cout << "结束时间：" << sys.wSecond << '.' << sys.wMilliseconds << endl;
				exit(0);
			}
		}
	}
}

//复制函数/
//1/
elem* sudo_copy(elem table[81]) {
	int elem_copy[81];
	elem* s = table;
	int* d = elem_copy;
	for (d; d - elem_copy < 81; d++) {
		*d = s->value;
		s++;
	}
	static elem element_copy[81];
	init_step_two(elem_copy, element_copy);
	return element_copy;
}

//公因数形成int数组/
//1/
int* gcd_array(int value) {
	int dev[9] = { 2,3,5,7,11,13,17,19,23 };
	static int gcd[10] = {0};
	static int* gcd_point = gcd;
	for (int i = 0; i < 9; i++) {
		if (value % dev[i]==0) {
			*gcd_point = dev[i];
			gcd_point++;
		}
		else {}
	}
	return gcd;
}