#include <stdio.h>
#include <stdlib.h>



//常量定义

#define PROCESS_NAME_LEN 32   /*进程名称的最大长度*/
#define MIN_SLICE 10	/*最小碎片的大小*/
#define DEFAULT_MEM_SIZE  1024   /*默认内存的大小*/
#define DEFAULT_MEM_START 0     /*默认内存的起始位置*/

/*内存分配算法*/
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3

int mem_size = DEFAULT_MEM_SIZE;    /*内存大小*/
int ma_algorithm = MA_FF;    /*当前分配算法*/
static int pid = 0;    /*设置内存大小标志*/
int flag = 0;


//内存空闲分区的描述

/*描述每一个内存块的数据结构*/
struct free_block_type {
	int size;
	int start_addr;
	struct free_block_type *next;
};

/*指向内存中空闲块链表的首指针*/
struct free_block_type *free_block;



//描述已分配的内存块

/*每个进程分到的内存块的描述*/
struct allocated_block {
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	struct allocated_block *next;
};

/*进程分配内存块链表的首指针*/
struct allocated_block *allocated_block_head = NULL;




/*函数*/
struct free_block_type* init_free_block(int mem_size);  //初始化空闲块，指定大小和地址

void display_menu();   //显示菜单

int set_mem_size();  //设置内存大小

void set_algorithm();  //设置当前的分配算法

void rearrange(int algorithm);  //按指定的算法整理内存空间块链表

void rearrange_FF();  //按FF算法重新整理内存空间块链表，地址递增

void rearrange_BF();  //按BF算法重新整理内存空间块链表，容量大小递增

void rearrange_WF();  //按FF算法重新整理内存空间块链表，容量大小递减

int new_process();  //创建新的进程，主要获取内存的申请数量，分配内存，将分配成功的插入到allocated_block_head链表

int allocate_mem(struct allocated_block *ab);   //分配内存模块

void kill_process();  //删除进程，归还已分配的存储空间，并删除该进程内存分配的节点

int free_mem(struct allocated_block *ab);  //将ab所表示的已分配区归还，并进行可能的合并

int dispose(struct allocated_block *free_ab);  //释放ab数据结构节点

int display_mem_usage();  //显示当前内存使用情况

struct allocated_block* find_process(int pid);   //查找指定pid占用的节点，返回其指针

void swap (int *a, int *b);   //交换两数据



int main()
{
	char choice;
	//pid = 0;
	free_block = init_free_block(mem_size);   //初始化空闲区
	
	for(;;)
	{
		display_menu();  //显示菜单
		fflush(stdin);
		choice = getchar(); 	//获取用户输入
		switch(choice) {
			case '1': set_mem_size(); break;  //设置内存大小
			case '2': set_algorithm(); flag = 1; break; //设置分配算法
			case '3': new_process(); flag = 1; break;  //创建新进程
			case '4': kill_process(); flag = 1; break; //删除进程
			case '5': display_mem_usage(); flag = 1; break; //显示内存使用
			case '0':  exit(0);
			default: break;
		}
		getchar();
	}
}


/*初始化空闲块，默认为一块，可以指定大小及起始地址*/
struct free_block_type* init_free_block(int mem_size) {
	struct free_block_type* fb;

	fb = (struct free_block_type *)malloc(sizeof(struct free_block_type));
	if(fb == NULL) {
		printf ("NO MEM!\n");
		return 	NULL;
	}
	
	fb->size = mem_size;
	fb->start_addr = DEFAULT_MEM_START;
	fb->next = NULL;
	
	return fb;
}


/*显示菜单*/
void display_menu() {
	printf ("1-Set memory size(default = %d) --- 设置内存大小\n", DEFAULT_MEM_SIZE);
	printf ("2-Select memery allocation algorithm --- 选择内存分配算法\n");
	printf ("3-New process --- 新的进程\n");
	printf ("4-Terminate a process --- 结束一个进程\n");
	printf ("5-Display memory usage --- 列出内存使用情况\n");
	printf ("0-Exit\n");
}



/*设置内存大小*/
int set_mem_size() {
	int size;
	if(flag != 0) {
		printf("Cannot set the memsize again!\n");
		return 0;
	}
	printf ("Total memory size = ");
	scanf ("%d", &size);
	if (size > 0) {
		mem_size = size;
		free_block->size = mem_size;
	}
	flag = 1;
	return 1;
}




/*设置当前的分配算法*/
void set_algorithm() {
	int algorithm;
	printf ("\t1-First Fit  //首次分配算法\n");
	printf ("\t2-Best Fit   //最优分配算法\n");
	printf ("\t3-Worst Fit  //最坏分配算法\n");
	scanf ("%d", &algorithm);
	if (algorithm >=1 && algorithm <=3) {
		ma_algorithm = algorithm;
	}
	rearrange(ma_algorithm);
}



/*按指定算法整理内存空闲块链表*/
void rearrange(int algorithm) {
	switch (algorithm) {
		case 1: rearrange_FF(); break;
		case 2: rearrange_BF(); break;
		case 3: rearrange_WF(); break;
		default: break;
	}
}



/*按FF算法重新整理内存空间块链表*/
void rearrange_FF() {
	struct free_block_type *tmp, *work;
	printf ("Rearrange free blocks for FF --- 按FF算法重新整理内存空闲块链表\n");
	tmp = free_block;
	while (tmp != NULL) {
		work = tmp -> next;
		while(work != NULL) {
			if (work -> start_addr < tmp -> start_addr) {
				swap (&work -> start_addr, &tmp -> start_addr);
				swap (&work -> size, &tmp -> size);
			}
			else {
				work = work -> next;
			}
		}
		tmp = tmp -> next;
	}
}



/*按BF算法重新整理内存空闲块链表*/
void rearrange_BF() {
	struct free_block_type *tmp, *work;
	printf ("Rearrange free blocks for BF //按BF算法重新整理内存空闲块链表\n");
	tmp = free_block;
	while (tmp != NULL) {
		work = tmp -> next;
		while (work != NULL) {
			if (work -> size < tmp -> size) {
				swap (&work -> start_addr, &tmp -> start_addr);
				swap (&work -> size, &tmp -> size);
			}
			else {
				work = work -> next;
			}
		}
		tmp = tmp -> next;
	}
} 


void swap (int *a, int *b) {
	int *t;
	t = a;
	a = b;
	b = t;
}


/*按WF算法重新整理内存空闲块链表*/
void rearrange_WF() {
	struct free_block_type *tmp, *work;
	printf ("Rearrange free blocks for WF //按WF算法重新整理内存空闲块链表\n");
	tmp = free_block;
	while (tmp != NULL) {
		work = tmp -> next;
		while (work != NULL) {
			if (work -> size > tmp -> size) {
				swap (&work -> start_addr, &tmp -> start_addr);
				swap (&work -> size, &tmp -> size);
			} 
			else {
				work = work -> next;
			}
		}
		tmp = tmp -> next;
	}
}



/*创建新的进程，主要获取内存的申请数量*/
int new_process() {
	struct allocated_block *ab;
	int size;
	int ret;
	ab = (struct allocated_block *)malloc(sizeof(struct allocated_block));
	if (ab == NULL) {
		exit(-5);
	}
	ab -> next = NULL;
	pid ++;
	sprintf (ab -> process_name,"PROCESS-%02d", pid);
	ab -> pid = pid;
	
	printf ("Memory for %s:", ab -> process_name);
	scanf ("%d", &size);
	if (size > 0) {
		ab -> size = size;
	}
	ret = allocate_mem(ab);     /*从空闲区分配内存，ret==1表示分配*/

	printf ("ret = %d \n", ret);
	
	if((ret == 1) && (allocated_block_head == NULL)) {
		allocated_block_head = ab;
		return 1;
	} else if (ret == 1) {
		ab -> next = allocated_block_head;
		allocated_block_head = ab;
		return 2;
	}else if (ret == -1) {
		printf("Allocated fail\n");
		free(ab);
		return -1;
	}
	return 3;
}



/*!!!!!!!!分配内存模块*/
int allocate_mem(struct allocated_block *ab)
{
	struct free_block_type *fbt, *pre;

	int request_size = ab -> size;
	fbt = pre = free_block;
	
	while (fbt != NULL) {
		if(fbt -> size >= request_size + 10) {   //分配的空闲空间足够大，则分割
			fbt -> size -= request_size;
			ab -> start_addr = fbt -> start_addr;
			fbt -> start_addr += request_size;
			return 1;
		}
		else if(fbt -> size >= request_size) {
			//分割后的空闲区成为小碎片，一起分配
			if(pre == fbt) {
				 ab-> start_addr = fbt -> start_addr;
				 free_block = pre -> next;
				 free(fbt);
			} else {
				ab -> start_addr = fbt -> start_addr;
				pre -> next = fbt -> next;
				free(fbt);
			}
			return 1;
		}
		pre = fbt;
		fbt = fbt -> next;
	}
	return -1;
}





/*删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点*/
void kill_process() {
	struct allocated_block *ab;
	int pid;
	printf ("Kill Process, pid = ");
	scanf ("%d", &pid);
	ab = find_process(pid);
	if (ab != NULL) {
		free_mem(ab);    /*释放ab所表示的分配区*/
		dispose(ab);     /*释放ab数据结构节点*/
	}
}



/*查找指定pid占用的节点，返回其指针*/
struct allocated_block* find_process(int pid) {
	struct allocated_block *pre;
	pre = allocated_block_head;
	while (pre != NULL) {
		if (pre -> pid == pid) {
			return pre;
		}
		pre = pre -> next;
	}
	return NULL;

}


/*将ab所表示的已分配区归还，并进行可能的合并*/
int free_mem(struct allocated_block *ab) {
	int algorithm = ma_algorithm;
	struct free_block_type *fbt, *pre, *work;

	fbt = (struct free_block_type *) malloc (sizeof(struct free_block_type));
	if (!fbt) {
		return -1;
	}
	fbt -> size = ab -> size;
	fbt -> start_addr = ab -> start_addr;
	fbt -> next = free_block;    //将归还的内存加入链表头部
	free_block = fbt;
	//rearrange(MA_FF);         //将归还后的分区链重新排序

	fbt = free_block;
	while (fbt != NULL) {
		work = fbt -> next;
		if (work != NULL) {
			if (fbt -> start_addr + fbt -> size == work -> start_addr) {
				fbt -> size += work -> size;
				fbt -> next = work -> next;
				free(work);
				continue;
			}
		}
		fbt = fbt -> next;
	}
	rearrange(algorithm);      //重新按当前的算法排列空闲区
	return 1;
}



/*释放ab数据结构节点*/
int dispose (struct allocated_block *free_ab) {
	struct allocated_block *pre, *ab;

	if (free_ab == allocated_block_head) {
		allocated_block_head = allocated_block_head -> next;
		free(free_ab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head -> next;

	while (ab != free_ab) {
		pre = ab;
		ab = ab -> next;
	}
	pre -> next = ab -> next;
	free(ab);
	return 2;
}



/*显示当前内存使用情况，包括空闲区的情况和已经分配的情况*/
int display_mem_usage() {
	struct free_block_type  *fbt = free_block;
	struct allocated_block *ab = allocated_block_head;

	if(fbt == NULL) {
		return -1;
	}
	printf ("\n----------------------------------------------------------\n");    //空闲区
	printf ("Free Memory:\n");
	printf ("%20s %20s\n","start_addr", "free_block -> size");
	while (fbt != NULL) {
		printf ("%20d %20d\n", fbt -> start_addr, fbt -> size);
		fbt = fbt -> next;
	}

	printf ("\n----------------------------------------------------------\n");  //已分配区
	printf ("Allocated Memory:\n");
	printf ("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", "size");

	while (ab != NULL) {
		printf ("%10d %20s %10d %10d\n", ab -> pid, ab -> process_name, ab -> start_addr, ab -> size);
		ab = ab -> next;
	}
	printf ("\n----------------------------------------------------------\n");
	return 0;
}
