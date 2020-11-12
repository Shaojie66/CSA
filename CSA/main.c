#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include<algorithm>
#include<iostream>
#include<string.h>
#include<string>
//#define NUM 10*1024
#define ERROR -1
#define l 1000
char* buff;
char* start_point;
int size;

char* get_seq(char* p, int n) {
	static char* m, * t;
	m = (char*)malloc(size- 1);
	t = (char*)malloc(size - 1);
	if (m == NULL || n == NULL)
		return NULL;
	t = m;
	p = p + n;
	while ((n < size) && (*p)) {
		if (*p == '\n')
			*p += 1;
		*m++ = *p++;
		n++;
	}
	*(m - 1) = '\0';
	return t;
}
/*后缀排序数组*/
int cmp(const void* a, const void* b) {
	//return strcmp((const char *)a,(const char *)b);
	return strcmp((char*)a, (char*)b);
}
/*base_step的函数，计算最后一小段的ψ值*/
int* base_step(char T[]) {
	char a[l][l];
	strcpy(a[0], T);
	int len = strlen(a[0]);
	for (int i = 0; i < len-1; i++)
	{
		int k = 0;
		for (int j = i + 1; j < len; j++) {
			a[i + 1][k++] = a[0][j];
		}
		a[i + 1][k] = '\0';
	}
	qsort(a, len, sizeof(a[0]), cmp);
	//构建SA数组
	int SA[l];
	for (int p = 0; p < l; p++)
	{
		SA[p] = len - strlen(a[p]);
	}
	//构建SA_1数组
	int SA_1[l];
	for (int q = 0; q < l; q++)
	{
		SA_1[SA[q]] == q;
	}
	//构建ψ函数
	int ψ[l];
	for (int r = 0; r < l; r++)
	{
		ψ[r] = SA_1[SA[r] + 1];
	}
	ψ[0] = SA_1[0];
	return	&ψ[0];
}

//计算suffix序列的函数
int* base_sort(char T[], int x)
{
	char a[l][l];
	strcpy(a[0], T);
	int len = strlen(a[0]);
	for (int i = 0; i < len - 1; i++) {
		int k = 0;
		for (int j = 0; j < len; j++)
		{
			a[j + 1][k++] = a[0][j];

		}
		a[i + 1][k] = '\0';
	}
	qsort(a, len, sizeof(a[0]), cmp);
	//构建SA数组
	int SA[l];
	for (int p = 0; p < l; p++)
	{
		SA[p] = len - strlen(a[p]);
	}
	//构建SA_1数组
	int SA_1[l];
	for (int q = 0; q < l; q++)
	{
		SA_1[SA[q]] == q;
	}
	return &SA_1[0];
	
}
//计算lc
int* comulate_lc(int lc[], int T_SA[], char T3[], char T2[], int a, int b) {
	for (int i = 0; i < b; i++)
	{
		lc[i] = 0;
	}
	for (int j = 0; j < b; j++)
	{
		for (int i = 0; i < a; i++)
		{
			if (T2[b-1-j]>T3[T_SA[i]])
			{
				lc[b - 1 - j]++;
			}
		}
	}
	return &lc[0];
}
//计算rc
int* comulate_rc(int rc[], int T_SA[], char T3[], char T2[], int a, int b) {
	for (int i = 0; i < b; i++)
	{
		rc[i] = a;
	}
	for (int j = 0; j < b; j++) {
		for (int i = 0; i < a; i++)
		{
			if (T2[b-1-j]<T3[T_SA[a-1-i]])
			{
				rc[b - 1 - j]--;
			}
		}
		rc[b - 1 - j]--;
	}
	return &rc[0];
}
//merge b
int* merge_b(int order[], int Ψ_[], int lc[], int rc[], int order_suf[], int a, int b)
{
	for (int j = 0; j < b; j++)
	{
		int b1;
		bool flag = false;
		for (int i = 0; i < a; i++)
		{
			if (Ψ_[i] <= order[b - 1 - j])
			{
				if (lc[b - 1 - j] <= i && rc[b - 1 - j] >= i)
				{
					b1 = i;
					flag = true;
				}
			}
			//空集
			if (j != b - 1)
			{
				if (!flag)
				{
					//计算这个order时有点问题
					order[b - 1 - j - 1] = lc[b - 1 - j] - 1;
					order_suf[b - 1 - j] = lc[b - 1 - j] - 1;
				}
				else
				{
					//计算这个order时有点问题
					order[b - 1 - j - 1] = b1;
					order_suf[b - 1 - j] = b1;
					//cout << 6 - j<< endl;
				}
			}
			else
			{
				if (!flag)
				{
					//计算这个order时有点问题
					order_suf[b - 1 - j] = lc[b - 1 - j] - 1;
				}
				else
				{
					//计算这个order时有点问题
					order_suf[b - 1 - j] = b1;
					//cout << 6 - j<< endl;
				}
			}
		}
	}
	return &order_suf[0];
}

int* comulate_f(int f[], int order_suf[], int T_SA_1[], int a, int b)
{
	for (int m = 0; m < a; m++)
	{
		int j1 = T_SA_1[m];
		int number = 0;
		for (int k = 0; k < b; k++)
		{
			if (order_suf[k] < j1)
			{
				number++;
			}
		}
		f[j1] = j1 + number;

	}
	f[0] = 0;

	return &f[0];
}
int* comulate_g(int g[], int order_suf[], int suffix_sort[], int a, int b)
{
	for (int x = 0; x < b; x++)
	{
		g[x] = order_suf[x] + suffix_sort[x] + 1;
	}

	return &g[0];
}
int* merge_c(int a, int b, int g[], int f[], int A[], int Ψ_[])
{
	for (int j = 1; j < a; j++)
	{
		A[f[j]] = f[Ψ_[j]];
	}
	A[g[b]] = f[Ψ_[0]];
	for (int i = 1; i < b; i++)
	{
		A[g[i]] = g[i + 1];
	}
	
	A[0] = g[1];
	return &A[0];
}
int* merge(int a, int b, char T2[], char T3[], int A[], int Ψ_[], int T_SA_1[])
{
	int* suffix_sortt = base_sort( T2, b);
	int suffix_sort[l*2];
	for (int i = 0; i < b; i++)
	{
		suffix_sort[i] = suffix_sortt[i];
		//cout << suffix_sort[i]<<endl;
	}
	int lllc[n];
	int* llc = comulate_lc(lllc, T_SA_1, T3, T2, a, b);
	int lc[n];
	for (int i = 0; i < b; i++)
	{
		lc[i] = llc[i];
		//cout << lc[i] << endl;
	}
	int rrrc[n];
	int* rrc = comulate_rc(rrrc, T_SA_1, T3, T2, a, b);
	int rc[n];
	for (int i = 0; i < b; i++)
	{
		rc[i] = rrc[i];
		//cout << rc[i] << endl;
	}

	//Merge的b步

	int order[n];
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
	order[b - 1] = T_SA_1[0];
	int order_suf[n];
	int* oorder_suf = merge_b(order, Ψ_, lc, rc, order_suf, a, b);
	for (int i = 0; i < b; i++)
	{
		//order_suf[i] = oorder_suf[i];
		cout << "看这里" << endl;
		cout << order_suf[i] << endl;
	}

	//cout << "f" << endl;
	//merge c(计算f函数)
	//Suppose SA−1 T[m] = j.Then SA−1 TiT[m] is equal to
		//f(j) = j + #(order(sufk, T) ≤ j),
	int f[n];
	int* ff = comulate_f(f, order_suf, T_SA_1, a, b);
	for (int i = 0; i < a; i++)
	{
		f[i] = ff[i];
		//cout << f[i] << endl;
	}

	//cout << "g" << endl;
	//merge c(计算g函数)
	//For all j ∈ [1, ], SA−1 TiT [j] is equal to
	//g(j) = order(sufj, T) + #(sufk ≤ sufj)
	int ggg[n];
	int g[n];
	int* gg = comulate_g(g, order_suf, suffix_sort, a, b);
	for (int i = 0; i < b; i++)
	{
		ggg[i] = gg[i];
		//cout << g[i] << endl;
	}
	cout << "这里" << endl;
	for (int i = 0; i < b + 1; i++)
	{
		g[0] = 0;
		g[i + 1] = ggg[i];
		//cout << g[i] << endl;
	}


	//merge合并
	cout << "从这里开始" << endl;
	//int A[n];
	int* AA = merge_c(a, b, g, f, A, Ψ_);
	for (int i = 0; i < a + b; i++)
	{
		A[i] = AA[i];
		//cout << A[i] << endl;
	}
	return &A[0];
}

int main(int argc, char* argv[])
{
	FILE* file;
	size = 0;
	file = fopen("NC_008253.fna", "rt");
	/*argv[1]为命令行传入的地址，"rt"表示以只读的方式打开一个文本文件*/
	if (file == NULL)
	{
		printf("ERROR:cannot retrieve this file.\n");
		return ERROR;
	}
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	/*通过定位到文件末尾，读出文件大小size，或者也可通过下面注释掉的for循环读取文件大小size*/
	rewind(file);
	
	printf("The file size is %d\n", size);
	buff = (char*)malloc(size - 1);
	start_point = (char*)malloc(size - 1);
	if (buff == NULL || start_point == NULL)
		return ERROR;
	fread(buff, size - 1, 1, file);
	/*将file指向的文本文件内容读入buff缓冲区中*/
	start_point = buff;
	/*start_point用于存储buff指向的首地址，用于free等*/
	//printf("%s\n", buff);
	/*打印出文本文件内容，此处用于调试，printf是个很好的调试方法，此处可检查文本是否读出，以及是否正确等*/
	static int i;
	//unsigned short *aa;
	//printf("%c\n",*buff);
	static int pos;
	static int seq_pos;
	for (; *buff; buff++) {
		//printf("%p\n",buff);
		i++;
		if ((*buff == '|') && (*(buff + 1) == ' ')) {
			pos = i;
			//buff--;
			printf("The value of pos is %d\n", pos);
		}
		if ((*buff == 'A' || *buff == 'T' || *buff == 'C' || *buff == 'G') && (*(buff + 1) == 'A' || *(buff + 1) == 'T' || *(buff + 1) == 'C' || *(buff + 1) == 'G')\
			&& (*(buff + 2) == 'A' || *(buff + 2) == 'T' || *(buff + 2) == 'C' || *(buff + 2) == 'G')) {
			seq_pos = i - 1;
			printf("The value of seq_pos is %d\n", seq_pos);
			break;
		}
	}
	/*for循环中记录了标识符的结束位置和核酸序列的起始位置，这里的标识符是指的第一个空格前面的字符*/
	char* seq = get_seq(start_point, seq_pos);
	//printf("%d", strlen(seq));
	/*printf("SEQ=\n%s\n", seq);*/
	buff = start_point;

	free(buff);
	//free(mm);
	//free(seq);
	fclose(file);
	//getchar();
	return 0;


}