#include <stdlib.h>
#include <stdio.h>
#include <time.h>
struct Block {
    int id;
    int size;
    int starAddr;
    int status;
    int pid;
    struct Block* prev;
    struct Block* next;
};
struct PCB {
    int pid;
    int needeMem;
    int status;
    int blockID;
    struct PCB* next;
};
int NumofFreeblock;//记录未分配内存块的数量
int NumofUsedblock;//记录已使用内存块的数量
int blockid;//标记所有进程块的id

int NumofNowblock;
int FF(struct PCB P[], int Numofproc, struct Block* bk1, struct Block* bk2) {
    struct Block* Freeblock = bk1;//Freeblock指针初始化指向未分配内存块的头结点
    struct Block* Usedblock = bk2;//Usedblock指针初始化指向已分配内存块的头结点
    for (int i = 0; i < Numofproc; i++)
    {
        printf("Allocating memory for process %d,memory requirement = %d\n", P[i].pid, P[i].needeMem);//输出正在分配空间的PCB的信息
        //面对新的进程重新初始化两个指针，都指向头结点
        Freeblock = bk1;
        Usedblock = bk2;
        int flag = 0;//记录PCB能否正常得到内存分配
        for (int j = 0; j < NumofFreeblock; j++)
        {
            if (Freeblock != NULL)
            {
                Freeblock = Freeblock->next;//跳过头结点，进行未分配内存块的遍历
            }
            if (Freeblock != NULL && P[i].needeMem <= Freeblock->size && Freeblock->status == 0)
            {//必须要对Freeblock空指针的情况进行排除，即Freeblock已经到达了最后一个，直接结束，进入下一个进程的分配
                flag = 1;//能正常分配
                if (P[i].needeMem == Freeblock->size)
                {//当此进程的所需内存空间和此内存块大小相同
                    while (Usedblock->next != NULL)
                    {//遍历到已分配内存空间块队列的最后，添加新的内存块进入列表
                        Usedblock = Usedblock->next;
                    }
                    Freeblock->status = 1;//将此内存块的状态改为已分配
                    //将此内存空间从Free列表中释放 
                    Freeblock->prev->next = Freeblock->next;
                    if (Freeblock->next != NULL)
                    {
                        Freeblock->next->prev = Freeblock->prev;
                    }
                    //将此内存空间块加入已分配块队列中
                    Usedblock->next = Freeblock;
                    Freeblock->prev = Usedblock;
                    Freeblock->next = NULL;
                    //输出两个列表的数据
                    struct Block* tmp = bk1; tmp = tmp->next;
                    struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                    NumofUsedblock++;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        tmp = tmp->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (tmp2->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        tmp2 = tmp2->next;
                    }
                    printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                    printf("********************************************************\n");
                    NumofFreeblock--;
                    break;
                }
                else {
                    //进程所需内存空间小于此内存块空间时
                    struct Block* newbk = (struct Block*)malloc(sizeof(struct Block));
                    int Starta = rand() % (Freeblock->size - P[i].needeMem) + Freeblock->starAddr;//随机化起始位置
                    if (Starta == Freeblock->starAddr)
                    {//当随机的分配空间起始位置和内存块的起始位置相同时
                        //此时只需要分割成两个块
                        //未分配空间块+1，生成一块新的为分配空间
                        NumofFreeblock++;
                        blockid++;
                        //新的空间大小等于此块大小-进程块所需内存空间大小
                        newbk->size = Freeblock->size - P[i].needeMem;
                        //源内存块分成两块，前一块的空间为进程块的所需内存空间大小
                        //设置变量表示此进程被进程占用
                        Freeblock->size = P[i].needeMem;
                        Freeblock->pid = P[i].pid;
                        Freeblock->status = 1;
                        //将新的内存块加入到未分配队列中
                        Freeblock->prev->next = newbk;
                        //设置新的内存块的信息
                        newbk->id = blockid;
                        newbk->prev = Freeblock->prev;
                        newbk->next = Freeblock->next;
                        newbk->status = 0;
                        newbk->pid = -1;
                        newbk->starAddr = Freeblock->starAddr + Freeblock->size;
                        P[i].blockID = Freeblock->id;
                        //将分配的内存块移到使用内存队列中
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = Freeblock;
                        Freeblock->next = NULL;
                        Freeblock->prev = Usedblock;
                        //相应队列数量增加减少
                        NumofFreeblock--;
                        NumofUsedblock++;
                        //显示每个队列的数据
                        struct Block* tmp = bk1; tmp = tmp->next;
                        struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                        NumofUsedblock++;
                        printf("FreeBlockID          StartAddr          Size\n");
                        while (tmp->next != NULL)
                        {
                            printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                            tmp = tmp->next;
                        }
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        printf("UsedBlockID          StartAddr          Size          ProcID\n");
                        while (tmp2->next != NULL)
                        {
                            printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                            tmp2 = tmp2->next;
                        }
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        printf("********************************************************\n");
                    }
                    else if (Starta + P[i].needeMem == Freeblock->size + Freeblock->starAddr)
                    {
                        //进程块正好处于块尾，则也只需要将此内存块分成两块，大致过程与上一个相似
                        Freeblock->size = Freeblock->size - P[i].needeMem;
                        NumofFreeblock++;
                        blockid++;
                        newbk->id = blockid;
                        P[i].blockID = newbk->id;
                        newbk->size = P[i].needeMem;
                        newbk->pid = P[i].pid;
                        newbk->starAddr = Starta;
                        newbk->status = 1;
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = newbk;
                        newbk->next = NULL;
                        newbk->prev = Usedblock;
                        //输出两个列表的信息
                        struct Block* tmp = bk1; tmp = tmp->next;
                        struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                        NumofUsedblock++;
                        printf("FreeBlockID          StartAddr          Size\n");
                        while (tmp->next != NULL)
                        {
                            printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                            tmp = tmp->next;
                        }
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        printf("UsedBlockID          StartAddr          Size          ProcID\n");
                        while (tmp2->next != NULL)
                        {
                            printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                            tmp2 = tmp2->next;
                        }
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        printf("********************************************************\n");
                        NumofFreeblock--;
                    }
                    else if (Starta + P[i].needeMem != Freeblock->size + Freeblock->starAddr)
                    {
                        //进程所处位置在块的中间，需要将块分成三个
                        struct Block* newbk2 = (struct Block*)malloc(sizeof(struct Block));
                        NumofFreeblock++;
                        blockid++;
                        //newbk为进程块占用的内存块
                        newbk->starAddr = Starta;
                        newbk->size = P[i].needeMem;
                        newbk->status = 1;//被占用的内存
                        newbk->pid = i;
                        newbk->id = blockid;
                        P[i].blockID = newbk->id;
                        P[i].status = 1;
                        //newbk2为分割的第三块内新内存块,此内存未被占用
                        NumofFreeblock++;
                        blockid++;
                        newbk2->starAddr = Starta + newbk->size;
                        newbk2->id = blockid;
                        newbk2->size = Freeblock->size + Freeblock->starAddr - newbk2->starAddr;
                        newbk2->pid = -1;
                        newbk2->status = 0;
                        newbk2->prev = Freeblock;
                        newbk2->next = Freeblock->next;
                        //更新原始内存块的大小，此块已被分成三份，第一份大小更新
                        Freeblock->size = Starta - Freeblock->starAddr;
                        Freeblock->next = newbk2;
                        //将分配的内存块加入使用分配块的列表中
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = newbk;
                        newbk->prev = Usedblock;
                        newbk->next = NULL;
                        NumofUsedblock++;
                        struct Block* tmp = bk1; tmp = tmp->next;
                        struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                        //输出两个列表的数据
                        printf("FreeBlockID          StartAddr          Size\n");
                        while (tmp->next != NULL)
                        {
                            printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                            tmp = tmp->next;
                        }
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        printf("UsedBlockID          StartAddr          Size          ProcID\n");
                        while (tmp2->next != NULL)
                        {
                            printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                            tmp2 = tmp2->next;
                        }
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        printf("********************************************************\n");
                        NumofFreeblock--;
                        break;
                    }
                }
            }
            else
            {//当所有内存块空间大小都不够此进程所需的内存空间时
                flag = 0;
                continue;
            }
        }
        if (flag == 0)
        {//输出无法分配的信息
            printf("the p%d allocation failed!\n", P[i].pid);
            printf("********************************************************\n");
            continue;
        }
    }
    //分配内存后释放空间
    struct Block* tmp = bk2; tmp = tmp->next;
    int count = 0;
    while (count < NumofUsedblock)
    {
        tmp = bk2; tmp = tmp->next;
        printf("Recycling used memory block for process %d\n", tmp->pid);
        struct Block* tmp1 = bk1;//未分配内存块的链表
        for (int i = 0; i < NumofFreeblock; i++)
        {
            tmp1 = tmp1->next;
            if (i == 0 && tmp->size + tmp->starAddr <= tmp1->starAddr)
            {//当此使用内存块处于未分配内存块的最前块
                if (tmp->size + tmp->starAddr == tmp1->starAddr)
                {//当此内存块与后面的内存块相连
                    tmp1->starAddr = tmp->starAddr;
                    tmp1->size = tmp->size + tmp1->size;
                    //将使用内存块从使用内存块链表中移除
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->size + tmp->starAddr < tmp1->starAddr)
                {//当此内存块与后面的内存块不相连
                    tmp->pid = -1;
                    tmp->status = 0;
                    tmp->next = tmp1;
                    tmp->prev = tmp1->prev;
                    tmp1->prev->next = tmp;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofFreeblock++;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
            }
            else if (tmp1->starAddr + tmp1->size <= tmp->starAddr && tmp->starAddr + tmp->size <= tmp1->next->starAddr)
            {
                if (tmp->starAddr > tmp1->starAddr + tmp1->size && tmp1->next->starAddr == tmp->starAddr + tmp->size)
                {//只与下面的合并
                    tmp1->next->starAddr = tmp->starAddr;
                    tmp1->next->size = tmp1->next->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->starAddr == tmp1->starAddr + tmp1->size && tmp1->next->starAddr > tmp->starAddr + tmp->size)
                {//只与上面的合并
                    tmp1->size = tmp1->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->starAddr == tmp1->starAddr + tmp1->size && tmp1->next->starAddr == tmp->starAddr + tmp->size)
                {//与两个一起合并
                    tmp1->size = tmp1->size + tmp->size + tmp1->next->size;
                    if (tmp1->next->next != NULL)
                    {
                        tmp1->next->next->prev = tmp1;
                        tmp1->next = tmp1->next->next;
                        tmp->prev->next = tmp->next;
                        if (tmp->next != NULL)
                        {
                            tmp->next->prev = tmp->prev;
                        }
                    }
                    else
                    {
                        tmp1->next = NULL;
                        tmp->prev->next = tmp->next;
                        if (tmp->next != NULL)
                        {
                            tmp->next->prev = tmp->prev;
                        }
                    }
                    NumofFreeblock--;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->starAddr > tmp1->starAddr + tmp1->size && tmp1->next->starAddr > tmp->starAddr + tmp->size)
                {//与两者都不合并
                    tmp->pid = -1;
                    tmp->status = 0;
                    tmp->next = tmp1->next;
                    tmp->prev = tmp1;
                    tmp1->next->prev = tmp;
                    tmp1->next = tmp;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofFreeblock++;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
            }
            else if (i == NumofFreeblock - 1 && tmp->starAddr >= tmp1->starAddr + tmp1->size)
            {
                if (tmp->size + tmp->starAddr == tmp1->starAddr)
                {
                    tmp1->size = tmp->size + tmp1->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->size + tmp->starAddr > tmp1->starAddr)
                {
                    tmp->pid = -1;
                    tmp->status = 0;
                    tmp->next = NULL;
                    tmp->prev = tmp1;
                    tmp1->next = tmp;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofFreeblock++;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
            }
        }
        count = 0;
    }
    return 1;
}
int NF(struct PCB P[], int Numofproc, struct Block* bk1, struct Block* bk2) {
    struct Block* Freeblock = bk1;//Freeblock指针初始化指向未分配内存块的头结点
    struct Block* Usedblock = bk2;//Usedblock指针初始化指向已分配内存块的头结点
    for (int i = 0; i < Numofproc; i++)
    {
        printf("Allocating memory for process %d,memory requirement = %d\n", P[i].pid, P[i].needeMem);//输出正在分配空间的PCB的信息
        //面对新的进程重新初始化两个指针，都指向头结点
        Freeblock = bk1;
        Usedblock = bk2;
        int flag = 0;//记录PCB能否正常得到内存分配
        for (int j = 0; j < NumofFreeblock; j++)
        {
            if (Freeblock != NULL)
            {
                Freeblock = Freeblock->next;//跳过头结点，进行未分配内存块的遍历
            }
            if (Freeblock != NULL && P[i].needeMem <= Freeblock->size && Freeblock->id == NumofNowblock)
            {//必须要对Freeblock空指针的情况进行排除，即Freeblock已经到达了最后一个，直接结束，进入下一个进程的分配
                flag = 1;//能正常分配
                if (P[i].needeMem == Freeblock->size)
                {//当此进程的所需内存空间和此内存块大小相同
                    while (Usedblock->next != NULL)
                    {//遍历到已分配内存空间块队列的最后，添加新的内存块进入列表
                        Usedblock = Usedblock->next;
                    }
                    Freeblock->status = 1;//将此内存块的状态改为已分配
                    //将此内存空间从Free列表中释放 
                    Freeblock->prev->next = Freeblock->next;
                    if (Freeblock->next != NULL)
                    {
                        Freeblock->next->prev = Freeblock->prev;
                        NumofNowblock = Freeblock->prev->next->id;
                    }
                    //将此内存空间块加入已分配块队列中
                    Usedblock->next = Freeblock;
                    Freeblock->prev = Usedblock;
                    Freeblock->next = NULL;
                    //输出两个列表的数据
                    struct Block* tmp = bk1; tmp = tmp->next;
                    struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                    NumofUsedblock++;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        tmp = tmp->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (tmp2->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        tmp2 = tmp2->next;
                    }
                    printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                    printf("********************************************************\n");
                    NumofFreeblock--;
                    break;
                }
                else {
                    //进程所需内存空间小于此内存块空间时
                    struct Block* newbk = (struct Block*)malloc(sizeof(struct Block));
                    int Starta = rand() % (Freeblock->size - P[i].needeMem) + Freeblock->starAddr;//随机化起始位置
                    if (Starta == Freeblock->starAddr)
                    {//当随机的分配空间起始位置和内存块的起始位置相同时
                        //此时只需要分割成两个块
                        //未分配空间块+1，生成一块新的为分配空间
                        NumofFreeblock++;
                        blockid++;
                        //新的空间大小等于此块大小-进程块所需内存空间大小
                        newbk->size = Freeblock->size - P[i].needeMem;
                        //源内存块分成两块，前一块的空间为进程块的所需内存空间大小
                        //设置变量表示此进程被进程占用
                        Freeblock->size = P[i].needeMem;
                        Freeblock->pid = P[i].pid;
                        Freeblock->status = 1;
                        //将新的内存块加入到未分配队列中
                        Freeblock->prev->next = newbk;
                        //设置新的内存块的信息
                        newbk->id = blockid;
                        NumofNowblock = newbk->id;
                        newbk->prev = Freeblock->prev;
                        newbk->next = Freeblock->next;
                        newbk->status = 0;
                        newbk->pid = -1;
                        newbk->starAddr = Freeblock->starAddr + Freeblock->size;
                        P[i].blockID = Freeblock->id;
                        //将分配的内存块移到使用内存队列中
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = Freeblock;
                        Freeblock->next = NULL;
                        Freeblock->prev = Usedblock;
                        //相应队列数量增加减少
                        NumofFreeblock--;
                        NumofUsedblock++;
                        //显示每个队列的数据
                        struct Block* tmp = bk1; tmp = tmp->next;
                        struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                        NumofUsedblock++;
                        printf("FreeBlockID          StartAddr          Size\n");
                        while (tmp->next != NULL)
                        {
                            printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                            tmp = tmp->next;
                        }
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        printf("UsedBlockID          StartAddr          Size          ProcID\n");
                        while (tmp2->next != NULL)
                        {
                            printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                            tmp2 = tmp2->next;
                        }
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        printf("********************************************************\n");
                    }
                    else if (Starta + P[i].needeMem == Freeblock->size + Freeblock->starAddr)
                    {
                        //进程块正好处于块尾，则也只需要将此内存块分成两块，大致过程与上一个相似
                        Freeblock->size = Freeblock->size - P[i].needeMem;
                        NumofNowblock = Freeblock->next->id;
                        NumofFreeblock++;
                        blockid++;
                        newbk->id = blockid;
                        P[i].blockID = newbk->id;
                        newbk->size = P[i].needeMem;
                        newbk->pid = P[i].pid;
                        newbk->starAddr = Starta;
                        newbk->status = 1;
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = newbk;
                        newbk->next = NULL;
                        newbk->prev = Usedblock;
                        //输出两个列表的信息
                        struct Block* tmp = bk1; tmp = tmp->next;
                        struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                        NumofUsedblock++;
                        printf("FreeBlockID          StartAddr          Size\n");
                        while (tmp->next != NULL)
                        {
                            printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                            tmp = tmp->next;
                        }
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        printf("UsedBlockID          StartAddr          Size          ProcID\n");
                        while (tmp2->next != NULL)
                        {
                            printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                            tmp2 = tmp2->next;
                        }
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        printf("********************************************************\n");
                        NumofFreeblock--;
                    }
                    else if (Starta + P[i].needeMem != Freeblock->size + Freeblock->starAddr)
                    {
                        //进程所处位置在块的中间，需要将块分成三个
                        struct Block* newbk2 = (struct Block*)malloc(sizeof(struct Block));
                        NumofFreeblock++;
                        blockid++;
                        //newbk为进程块占用的内存块
                        newbk->starAddr = Starta;
                        newbk->size = P[i].needeMem;
                        newbk->status = 1;//被占用的内存
                        newbk->pid = i;
                        newbk->id = blockid;
                        P[i].blockID = newbk->id;
                        P[i].status = 1;
                        //newbk2为分割的第三块内新内存块,此内存未被占用
                        NumofFreeblock++;
                        blockid++;
                        newbk2->starAddr = Starta + newbk->size;
                        newbk2->id = blockid;
                        NumofNowblock = newbk2->id;
                        newbk2->size = Freeblock->size + Freeblock->starAddr - newbk2->starAddr;
                        newbk2->pid = -1;
                        newbk2->status = 0;
                        newbk2->prev = Freeblock;
                        newbk2->next = Freeblock->next;
                        //更新原始内存块的大小，此块已被分成三份，第一份大小更新
                        Freeblock->size = Starta - Freeblock->starAddr;
                        Freeblock->next = newbk2;
                        //将分配的内存块加入使用分配块的列表中
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = newbk;
                        newbk->prev = Usedblock;
                        newbk->next = NULL;
                        NumofUsedblock++;
                        struct Block* tmp = bk1; tmp = tmp->next;
                        struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                        //输出两个列表的数据
                        printf("FreeBlockID          StartAddr          Size\n");
                        while (tmp->next != NULL)
                        {
                            printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                            tmp = tmp->next;
                        }
                        printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                        printf("UsedBlockID          StartAddr          Size          ProcID\n");
                        while (tmp2->next != NULL)
                        {
                            printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                            tmp2 = tmp2->next;
                        }
                        printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                        printf("********************************************************\n");
                        NumofFreeblock--;
                        break;
                    }
                }
            }
            else
            {//当所有内存块空间大小都不够此进程所需的内存空间时
                flag = 0;
                continue;
            }
        }
        if (flag == 0)
        {
            Freeblock = bk1;
            for (int j = 0; j < NumofFreeblock; j++)
            {
                if (Freeblock != NULL)
                {
                    Freeblock = Freeblock->next;
                }
                if (Freeblock != NULL && Freeblock->size >= P[i].needeMem)
                {
                    flag = 1;
                    NumofNowblock = Freeblock->id;
                    break;
                }
            }
            while (flag)
            {
                Freeblock = bk1;
                for (int j = 0; j < NumofFreeblock; j++)
                {
                    if (Freeblock != NULL)
                    {
                        Freeblock = Freeblock->next;//跳过头结点，进行未分配内存块的遍历
                    }
                    if (Freeblock != NULL && P[i].needeMem <= Freeblock->size && Freeblock->id == NumofNowblock)
                    {//必须要对Freeblock空指针的情况进行排除，即Freeblock已经到达了最后一个，直接结束，进入下一个进程的分配
                        flag = 1;//能正常分配
                        if (P[i].needeMem == Freeblock->size)
                        {//当此进程的所需内存空间和此内存块大小相同
                            while (Usedblock->next != NULL)
                            {//遍历到已分配内存空间块队列的最后，添加新的内存块进入列表
                                Usedblock = Usedblock->next;
                            }
                            Freeblock->status = 1;//将此内存块的状态改为已分配
                            //将此内存空间从Free列表中释放 
                            Freeblock->prev->next = Freeblock->next;
                            if (Freeblock->next != NULL)
                            {
                                Freeblock->next->prev = Freeblock->prev;
                                NumofNowblock = Freeblock->prev->next->id;
                            }
                            //将此内存空间块加入已分配块队列中
                            Usedblock->next = Freeblock;
                            Freeblock->prev = Usedblock;
                            Freeblock->next = NULL;
                            //输出两个列表的数据
                            struct Block* tmp = bk1; tmp = tmp->next;
                            struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                            NumofUsedblock++;
                            printf("FreeBlockID          StartAddr          Size\n");
                            while (tmp->next != NULL)
                            {
                                printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                                tmp = tmp->next;
                            }
                            printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                            printf("UsedBlockID          StartAddr          Size          ProcID\n");
                            while (tmp2->next != NULL)
                            {
                                printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                                tmp2 = tmp2->next;
                            }
                            printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                            printf("********************************************************\n");
                            NumofFreeblock--;
                            break;
                        }
                        else {
                            //进程所需内存空间小于此内存块空间时
                            struct Block* newbk = (struct Block*)malloc(sizeof(struct Block));
                            int Starta = rand() % (Freeblock->size - P[i].needeMem) + Freeblock->starAddr;//随机化起始位置
                            if (Starta == Freeblock->starAddr)
                            {//当随机的分配空间起始位置和内存块的起始位置相同时
                                //此时只需要分割成两个块
                                //未分配空间块+1，生成一块新的为分配空间
                                NumofFreeblock++;
                                blockid++;
                                //新的空间大小等于此块大小-进程块所需内存空间大小
                                newbk->size = Freeblock->size - P[i].needeMem;
                                //源内存块分成两块，前一块的空间为进程块的所需内存空间大小
                                //设置变量表示此进程被进程占用
                                Freeblock->size = P[i].needeMem;
                                Freeblock->pid = P[i].pid;
                                Freeblock->status = 1;
                                //将新的内存块加入到未分配队列中
                                Freeblock->prev->next = newbk;
                                //设置新的内存块的信息
                                newbk->id = blockid;
                                NumofNowblock = newbk->id;
                                newbk->prev = Freeblock->prev;
                                newbk->next = Freeblock->next;
                                newbk->status = 0;
                                newbk->pid = -1;
                                newbk->starAddr = Freeblock->starAddr + Freeblock->size;
                                P[i].blockID = Freeblock->id;
                                //将分配的内存块移到使用内存队列中
                                while (Usedblock->next != NULL)
                                {
                                    Usedblock = Usedblock->next;
                                }
                                Usedblock->next = Freeblock;
                                Freeblock->next = NULL;
                                Freeblock->prev = Usedblock;
                                //相应队列数量增加减少
                                NumofFreeblock--;
                                NumofUsedblock++;
                                //显示每个队列的数据
                                struct Block* tmp = bk1; tmp = tmp->next;
                                struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                                NumofUsedblock++;
                                printf("FreeBlockID          StartAddr          Size\n");
                                while (tmp->next != NULL)
                                {
                                    printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                                    tmp = tmp->next;
                                }
                                printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                                printf("UsedBlockID          StartAddr          Size          ProcID\n");
                                while (tmp2->next != NULL)
                                {
                                    printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                                    tmp2 = tmp2->next;
                                }
                                printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                                printf("********************************************************\n");
                            }
                            else if (Starta + P[i].needeMem == Freeblock->size + Freeblock->starAddr)
                            {
                                //进程块正好处于块尾，则也只需要将此内存块分成两块，大致过程与上一个相似
                                Freeblock->size = Freeblock->size - P[i].needeMem;
                                NumofNowblock = Freeblock->next->id;
                                NumofFreeblock++;
                                blockid++;
                                newbk->id = blockid;
                                P[i].blockID = newbk->id;
                                newbk->size = P[i].needeMem;
                                newbk->pid = P[i].pid;
                                newbk->starAddr = Starta;
                                newbk->status = 1;
                                while (Usedblock->next != NULL)
                                {
                                    Usedblock = Usedblock->next;
                                }
                                Usedblock->next = newbk;
                                newbk->next = NULL;
                                newbk->prev = Usedblock;
                                //输出两个列表的信息
                                struct Block* tmp = bk1; tmp = tmp->next;
                                struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                                NumofUsedblock++;
                                printf("FreeBlockID          StartAddr          Size\n");
                                while (tmp->next != NULL)
                                {
                                    printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                                    tmp = tmp->next;
                                }
                                printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                                printf("UsedBlockID          StartAddr          Size          ProcID\n");
                                while (tmp2->next != NULL)
                                {
                                    printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                                    tmp2 = tmp2->next;
                                }
                                printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                                printf("********************************************************\n");
                                NumofFreeblock--;
                            }
                            else if (Starta + P[i].needeMem != Freeblock->size + Freeblock->starAddr)
                            {
                                //进程所处位置在块的中间，需要将块分成三个
                                struct Block* newbk2 = (struct Block*)malloc(sizeof(struct Block));
                                NumofFreeblock++;
                                blockid++;
                                //newbk为进程块占用的内存块
                                newbk->starAddr = Starta;
                                newbk->size = P[i].needeMem;
                                newbk->status = 1;//被占用的内存
                                newbk->pid = i;
                                newbk->id = blockid;
                                P[i].blockID = newbk->id;
                                P[i].status = 1;
                                //newbk2为分割的第三块内新内存块,此内存未被占用
                                NumofFreeblock++;
                                blockid++;
                                newbk2->starAddr = Starta + newbk->size;
                                newbk2->id = blockid;
                                NumofNowblock = newbk2->id;
                                newbk2->size = Freeblock->size + Freeblock->starAddr - newbk2->starAddr;
                                newbk2->pid = -1;
                                newbk2->status = 0;
                                newbk2->prev = Freeblock;
                                newbk2->next = Freeblock->next;
                                //更新原始内存块的大小，此块已被分成三份，第一份大小更新
                                Freeblock->size = Starta - Freeblock->starAddr;
                                Freeblock->next = newbk2;
                                //将分配的内存块加入使用分配块的列表中
                                while (Usedblock->next != NULL)
                                {
                                    Usedblock = Usedblock->next;
                                }
                                Usedblock->next = newbk;
                                newbk->prev = Usedblock;
                                newbk->next = NULL;
                                NumofUsedblock++;
                                struct Block* tmp = bk1; tmp = tmp->next;
                                struct Block* tmp2 = bk2; tmp2 = tmp2->next;
                                //输出两个列表的数据
                                printf("FreeBlockID          StartAddr          Size\n");
                                while (tmp->next != NULL)
                                {
                                    printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                                    tmp = tmp->next;
                                }
                                printf("%11d          %9d          %4d\n", tmp->id, tmp->starAddr, tmp->size);
                                printf("UsedBlockID          StartAddr          Size          ProcID\n");
                                while (tmp2->next != NULL)
                                {
                                    printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                                    tmp2 = tmp2->next;
                                }
                                printf("%11d          %9d          %4d          %6d\n", tmp2->id, tmp2->starAddr, tmp2->size, tmp2->pid);
                                printf("********************************************************\n");
                                NumofFreeblock--;
                                break;
                            }
                        }
                    }
                    else
                    {//当所有内存块空间大小都不够此进程所需的内存空间时
                        flag = 0;
                        continue;
                    }
                }
                if (flag == 1)
                {//表示此进程块已经被分配，直接跳出while循环
                    break;
                }
                Freeblock = bk1;
                for (int j = 0; j < NumofFreeblock; j++)
                {
                    if (Freeblock != NULL)
                    {
                        Freeblock = Freeblock->next;
                    }
                    if (Freeblock != NULL && Freeblock->size >= P[i].needeMem)
                    {
                        flag = 1;
                        NumofNowblock = Freeblock->id;
                        break;
                    }
                }
            }
        }
        if (flag == 0)
        {
            printf("the p%d allocation failed!\n", P[i].pid);
            printf("********************************************************\n");
            continue;
        }
    }
    //分配内存后释放空间
    struct Block* tmp = bk2; tmp = tmp->next;
    int count = 0;
    while (count < NumofUsedblock)
    {
        tmp = bk2; tmp = tmp->next;
        printf("Recycling used memory block for process %d\n", tmp->pid);
        struct Block* tmp1 = bk1;//未分配内存块的链表
        for (int i = 0; i < NumofFreeblock; i++)
        {
            tmp1 = tmp1->next;
            if (i == 0 && tmp->size + tmp->starAddr <= tmp1->starAddr)
            {//当此使用内存块处于未分配内存块的最前块
                if (tmp->size + tmp->starAddr == tmp1->starAddr)
                {//当此内存块与后面的内存块相连
                    tmp1->starAddr = tmp->starAddr;
                    tmp1->size = tmp->size + tmp1->size;
                    //将使用内存块从使用内存块链表中移除
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->size + tmp->starAddr < tmp1->starAddr)
                {//当此内存块与后面的内存块不相连
                    tmp->pid = -1;
                    tmp->status = 0;
                    tmp->next = tmp1;
                    tmp->prev = tmp1->prev;
                    tmp1->prev->next = tmp;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofFreeblock++;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
            }
            else if (tmp1->starAddr + tmp1->size <= tmp->starAddr && tmp->starAddr + tmp->size <= tmp1->next->starAddr)
            {
                if (tmp->starAddr > tmp1->starAddr + tmp1->size && tmp1->next->starAddr == tmp->starAddr + tmp->size)
                {//只与下面的合并
                    tmp1->next->starAddr = tmp->starAddr;
                    tmp1->next->size = tmp1->next->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->starAddr == tmp1->starAddr + tmp1->size && tmp1->next->starAddr > tmp->starAddr + tmp->size)
                {//只与上面的合并
                    tmp1->size = tmp1->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->starAddr == tmp1->starAddr + tmp1->size && tmp1->next->starAddr == tmp->starAddr + tmp->size)
                {//与两个一起合并
                    tmp1->size = tmp1->size + tmp->size + tmp1->next->size;
                    if (tmp1->next->next != NULL)
                    {
                        tmp1->next->next->prev = tmp1;
                        tmp1->next = tmp1->next->next;
                        tmp->prev->next = tmp->next;
                        if (tmp->next != NULL)
                        {
                            tmp->next->prev = tmp->prev;
                        }
                    }
                    else
                    {
                        tmp1->next = NULL;
                        tmp->prev->next = tmp->next;
                        if (tmp->next != NULL)
                        {
                            tmp->next->prev = tmp->prev;
                        }
                    }
                    NumofFreeblock--;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (tmp21!= NULL && NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0 && tmp21!=NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->starAddr > tmp1->starAddr + tmp1->size && tmp1->next->starAddr > tmp->starAddr + tmp->size)
                {//与两者都不合并
                    tmp->pid = -1;
                    tmp->status = 0;
                    tmp->next = tmp1->next;
                    tmp->prev = tmp1;
                    tmp1->next->prev = tmp;
                    tmp1->next = tmp;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofFreeblock++;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
            }
            else if (i == NumofFreeblock - 1 && tmp->starAddr >= tmp1->starAddr + tmp1->size)
            {
                if (tmp->size + tmp->starAddr == tmp1->starAddr)
                {
                    tmp1->size = tmp->size + tmp1->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
                else if (tmp->size + tmp->starAddr > tmp1->starAddr)
                {
                    tmp->pid = -1;
                    tmp->status = 0;
                    tmp->next = NULL;
                    tmp->prev = tmp1;
                    tmp1->next = tmp;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofFreeblock++;
                    NumofUsedblock--;
                    //输出两个列表的信息
                    struct Block* tmp11 = bk1; tmp11 = tmp11->next;
                    struct Block* tmp21 = bk2; tmp21 = tmp21->next;
                    printf("FreeBlockID          StartAddr          Size\n");
                    while (tmp11->next != NULL)
                    {
                        printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                        tmp11 = tmp11->next;
                    }
                    printf("%11d          %9d          %4d\n", tmp11->id, tmp11->starAddr, tmp11->size);
                    printf("UsedBlockID          StartAddr          Size          ProcID\n");
                    while (NumofUsedblock != 0 && tmp21->next != NULL)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                        tmp21 = tmp21->next;
                    }
                    if (NumofUsedblock != 0)
                    {
                        printf("%11d          %9d          %4d          %6d\n", tmp21->id, tmp21->starAddr, tmp21->size, tmp21->pid);
                    }
                    printf("********************************************************\n");
                    break;
                }
            }
        }
        count = 0;
    }
    return 1;
}
int main() {
    srand((unsigned)time(NULL));
    //十个PCB生成
    struct PCB proc[10];
    for (int i = 0; i < 10; i++)
    {
        proc[i].pid = i;
        proc[i].blockID = -1;
        proc[i].status = 0;
        proc[i].needeMem = rand() % 100 + 100;
    }
    proc[0].next = &proc[1];
    proc[1].next = &proc[2];
    proc[2].next = &proc[3];
    proc[3].next = &proc[4];
    proc[4].next = &proc[5];
    proc[5].next = &proc[6];
    proc[6].next = &proc[7];
    proc[7].next = &proc[8];
    proc[8].next = &proc[9];
    proc[9].next = NULL;
    printf("process     needeMem     blockID    status\n");
    for (int i = 0; i < 10; i++)
    {
        printf("p%d          %5d       %5d         %d\n", proc[i].pid, proc[i].needeMem, proc[i].blockID, proc[i].status);
    }
    printf("********************************************************\n");
    //内存初始化
    //bk队列设置头结点
    struct Block* bkhead = (struct Block*)malloc(sizeof(struct Block));
    struct Block* bk = (struct Block*)malloc(sizeof(struct Block));
    bkhead->next = bk;
    NumofFreeblock = 1;
    blockid = 1;
    bkhead->prev = NULL;
    bk->id = blockid;//分区的序号
    bk->size = 1024;
    bk->starAddr = 0;
    bk->status = 0;//0表示空闲，1表示占用
    bk->pid = -1;
    bk->prev = bkhead;
    bk->next = NULL;
    printf("FreeBlockID          StartAddr          Size\n");
    printf("%11d          %9d          %4d\n", bk->id, bk->starAddr, bk->size);
    printf("********************************************************\n");
    struct Block* usedbkhead = (struct Block*)malloc(sizeof(struct Block));//使用队列的头指针
    usedbkhead->next = NULL;
    NumofUsedblock = 0;
    //FF(proc, 10, bkhead, usedbkhead);
    NumofNowblock = 1;//表示当前分配中的块
    NF(proc, 10, bkhead, usedbkhead);
    system("pause");
    return 1;
}