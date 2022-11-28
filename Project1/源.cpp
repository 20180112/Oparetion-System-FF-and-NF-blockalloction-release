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
int NumofFreeblock;//��¼δ�����ڴ�������
int NumofUsedblock;//��¼��ʹ���ڴ�������
int blockid;//������н��̿��id

int NumofNowblock;
int FF(struct PCB P[], int Numofproc, struct Block* bk1, struct Block* bk2) {
    struct Block* Freeblock = bk1;//Freeblockָ���ʼ��ָ��δ�����ڴ���ͷ���
    struct Block* Usedblock = bk2;//Usedblockָ���ʼ��ָ���ѷ����ڴ���ͷ���
    for (int i = 0; i < Numofproc; i++)
    {
        printf("Allocating memory for process %d,memory requirement = %d\n", P[i].pid, P[i].needeMem);//������ڷ���ռ��PCB����Ϣ
        //����µĽ������³�ʼ������ָ�룬��ָ��ͷ���
        Freeblock = bk1;
        Usedblock = bk2;
        int flag = 0;//��¼PCB�ܷ������õ��ڴ����
        for (int j = 0; j < NumofFreeblock; j++)
        {
            if (Freeblock != NULL)
            {
                Freeblock = Freeblock->next;//����ͷ��㣬����δ�����ڴ��ı���
            }
            if (Freeblock != NULL && P[i].needeMem <= Freeblock->size && Freeblock->status == 0)
            {//����Ҫ��Freeblock��ָ�����������ų�����Freeblock�Ѿ����������һ����ֱ�ӽ�����������һ�����̵ķ���
                flag = 1;//����������
                if (P[i].needeMem == Freeblock->size)
                {//���˽��̵������ڴ�ռ�ʹ��ڴ���С��ͬ
                    while (Usedblock->next != NULL)
                    {//�������ѷ����ڴ�ռ����е��������µ��ڴ������б�
                        Usedblock = Usedblock->next;
                    }
                    Freeblock->status = 1;//�����ڴ���״̬��Ϊ�ѷ���
                    //�����ڴ�ռ��Free�б����ͷ� 
                    Freeblock->prev->next = Freeblock->next;
                    if (Freeblock->next != NULL)
                    {
                        Freeblock->next->prev = Freeblock->prev;
                    }
                    //�����ڴ�ռ������ѷ���������
                    Usedblock->next = Freeblock;
                    Freeblock->prev = Usedblock;
                    Freeblock->next = NULL;
                    //��������б������
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
                    //���������ڴ�ռ�С�ڴ��ڴ��ռ�ʱ
                    struct Block* newbk = (struct Block*)malloc(sizeof(struct Block));
                    int Starta = rand() % (Freeblock->size - P[i].needeMem) + Freeblock->starAddr;//�������ʼλ��
                    if (Starta == Freeblock->starAddr)
                    {//������ķ���ռ���ʼλ�ú��ڴ�����ʼλ����ͬʱ
                        //��ʱֻ��Ҫ�ָ��������
                        //δ����ռ��+1������һ���µ�Ϊ����ռ�
                        NumofFreeblock++;
                        blockid++;
                        //�µĿռ��С���ڴ˿��С-���̿������ڴ�ռ��С
                        newbk->size = Freeblock->size - P[i].needeMem;
                        //Դ�ڴ��ֳ����飬ǰһ��Ŀռ�Ϊ���̿�������ڴ�ռ��С
                        //���ñ�����ʾ�˽��̱�����ռ��
                        Freeblock->size = P[i].needeMem;
                        Freeblock->pid = P[i].pid;
                        Freeblock->status = 1;
                        //���µ��ڴ����뵽δ���������
                        Freeblock->prev->next = newbk;
                        //�����µ��ڴ�����Ϣ
                        newbk->id = blockid;
                        newbk->prev = Freeblock->prev;
                        newbk->next = Freeblock->next;
                        newbk->status = 0;
                        newbk->pid = -1;
                        newbk->starAddr = Freeblock->starAddr + Freeblock->size;
                        P[i].blockID = Freeblock->id;
                        //��������ڴ���Ƶ�ʹ���ڴ������
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = Freeblock;
                        Freeblock->next = NULL;
                        Freeblock->prev = Usedblock;
                        //��Ӧ�����������Ӽ���
                        NumofFreeblock--;
                        NumofUsedblock++;
                        //��ʾÿ�����е�����
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
                        //���̿����ô��ڿ�β����Ҳֻ��Ҫ�����ڴ��ֳ����飬���¹�������һ������
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
                        //��������б����Ϣ
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
                        //��������λ���ڿ���м䣬��Ҫ����ֳ�����
                        struct Block* newbk2 = (struct Block*)malloc(sizeof(struct Block));
                        NumofFreeblock++;
                        blockid++;
                        //newbkΪ���̿�ռ�õ��ڴ��
                        newbk->starAddr = Starta;
                        newbk->size = P[i].needeMem;
                        newbk->status = 1;//��ռ�õ��ڴ�
                        newbk->pid = i;
                        newbk->id = blockid;
                        P[i].blockID = newbk->id;
                        P[i].status = 1;
                        //newbk2Ϊ�ָ�ĵ����������ڴ��,���ڴ�δ��ռ��
                        NumofFreeblock++;
                        blockid++;
                        newbk2->starAddr = Starta + newbk->size;
                        newbk2->id = blockid;
                        newbk2->size = Freeblock->size + Freeblock->starAddr - newbk2->starAddr;
                        newbk2->pid = -1;
                        newbk2->status = 0;
                        newbk2->prev = Freeblock;
                        newbk2->next = Freeblock->next;
                        //����ԭʼ�ڴ��Ĵ�С���˿��ѱ��ֳ����ݣ���һ�ݴ�С����
                        Freeblock->size = Starta - Freeblock->starAddr;
                        Freeblock->next = newbk2;
                        //��������ڴ�����ʹ�÷������б���
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
                        //��������б������
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
            {//�������ڴ��ռ��С�������˽���������ڴ�ռ�ʱ
                flag = 0;
                continue;
            }
        }
        if (flag == 0)
        {//����޷��������Ϣ
            printf("the p%d allocation failed!\n", P[i].pid);
            printf("********************************************************\n");
            continue;
        }
    }
    //�����ڴ���ͷſռ�
    struct Block* tmp = bk2; tmp = tmp->next;
    int count = 0;
    while (count < NumofUsedblock)
    {
        tmp = bk2; tmp = tmp->next;
        printf("Recycling used memory block for process %d\n", tmp->pid);
        struct Block* tmp1 = bk1;//δ�����ڴ�������
        for (int i = 0; i < NumofFreeblock; i++)
        {
            tmp1 = tmp1->next;
            if (i == 0 && tmp->size + tmp->starAddr <= tmp1->starAddr)
            {//����ʹ���ڴ�鴦��δ�����ڴ�����ǰ��
                if (tmp->size + tmp->starAddr == tmp1->starAddr)
                {//�����ڴ���������ڴ������
                    tmp1->starAddr = tmp->starAddr;
                    tmp1->size = tmp->size + tmp1->size;
                    //��ʹ���ڴ���ʹ���ڴ���������Ƴ�
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //��������б����Ϣ
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
                {//�����ڴ���������ڴ�鲻����
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
                    //��������б����Ϣ
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
                {//ֻ������ĺϲ�
                    tmp1->next->starAddr = tmp->starAddr;
                    tmp1->next->size = tmp1->next->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //��������б����Ϣ
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
                {//ֻ������ĺϲ�
                    tmp1->size = tmp1->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //��������б����Ϣ
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
                {//������һ��ϲ�
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
                    //��������б����Ϣ
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
                {//�����߶����ϲ�
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
                    //��������б����Ϣ
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
                    //��������б����Ϣ
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
                    //��������б����Ϣ
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
    struct Block* Freeblock = bk1;//Freeblockָ���ʼ��ָ��δ�����ڴ���ͷ���
    struct Block* Usedblock = bk2;//Usedblockָ���ʼ��ָ���ѷ����ڴ���ͷ���
    for (int i = 0; i < Numofproc; i++)
    {
        printf("Allocating memory for process %d,memory requirement = %d\n", P[i].pid, P[i].needeMem);//������ڷ���ռ��PCB����Ϣ
        //����µĽ������³�ʼ������ָ�룬��ָ��ͷ���
        Freeblock = bk1;
        Usedblock = bk2;
        int flag = 0;//��¼PCB�ܷ������õ��ڴ����
        for (int j = 0; j < NumofFreeblock; j++)
        {
            if (Freeblock != NULL)
            {
                Freeblock = Freeblock->next;//����ͷ��㣬����δ�����ڴ��ı���
            }
            if (Freeblock != NULL && P[i].needeMem <= Freeblock->size && Freeblock->id == NumofNowblock)
            {//����Ҫ��Freeblock��ָ�����������ų�����Freeblock�Ѿ����������һ����ֱ�ӽ�����������һ�����̵ķ���
                flag = 1;//����������
                if (P[i].needeMem == Freeblock->size)
                {//���˽��̵������ڴ�ռ�ʹ��ڴ���С��ͬ
                    while (Usedblock->next != NULL)
                    {//�������ѷ����ڴ�ռ����е��������µ��ڴ������б�
                        Usedblock = Usedblock->next;
                    }
                    Freeblock->status = 1;//�����ڴ���״̬��Ϊ�ѷ���
                    //�����ڴ�ռ��Free�б����ͷ� 
                    Freeblock->prev->next = Freeblock->next;
                    if (Freeblock->next != NULL)
                    {
                        Freeblock->next->prev = Freeblock->prev;
                        NumofNowblock = Freeblock->prev->next->id;
                    }
                    //�����ڴ�ռ������ѷ���������
                    Usedblock->next = Freeblock;
                    Freeblock->prev = Usedblock;
                    Freeblock->next = NULL;
                    //��������б������
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
                    //���������ڴ�ռ�С�ڴ��ڴ��ռ�ʱ
                    struct Block* newbk = (struct Block*)malloc(sizeof(struct Block));
                    int Starta = rand() % (Freeblock->size - P[i].needeMem) + Freeblock->starAddr;//�������ʼλ��
                    if (Starta == Freeblock->starAddr)
                    {//������ķ���ռ���ʼλ�ú��ڴ�����ʼλ����ͬʱ
                        //��ʱֻ��Ҫ�ָ��������
                        //δ����ռ��+1������һ���µ�Ϊ����ռ�
                        NumofFreeblock++;
                        blockid++;
                        //�µĿռ��С���ڴ˿��С-���̿������ڴ�ռ��С
                        newbk->size = Freeblock->size - P[i].needeMem;
                        //Դ�ڴ��ֳ����飬ǰһ��Ŀռ�Ϊ���̿�������ڴ�ռ��С
                        //���ñ�����ʾ�˽��̱�����ռ��
                        Freeblock->size = P[i].needeMem;
                        Freeblock->pid = P[i].pid;
                        Freeblock->status = 1;
                        //���µ��ڴ����뵽δ���������
                        Freeblock->prev->next = newbk;
                        //�����µ��ڴ�����Ϣ
                        newbk->id = blockid;
                        NumofNowblock = newbk->id;
                        newbk->prev = Freeblock->prev;
                        newbk->next = Freeblock->next;
                        newbk->status = 0;
                        newbk->pid = -1;
                        newbk->starAddr = Freeblock->starAddr + Freeblock->size;
                        P[i].blockID = Freeblock->id;
                        //��������ڴ���Ƶ�ʹ���ڴ������
                        while (Usedblock->next != NULL)
                        {
                            Usedblock = Usedblock->next;
                        }
                        Usedblock->next = Freeblock;
                        Freeblock->next = NULL;
                        Freeblock->prev = Usedblock;
                        //��Ӧ�����������Ӽ���
                        NumofFreeblock--;
                        NumofUsedblock++;
                        //��ʾÿ�����е�����
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
                        //���̿����ô��ڿ�β����Ҳֻ��Ҫ�����ڴ��ֳ����飬���¹�������һ������
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
                        //��������б����Ϣ
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
                        //��������λ���ڿ���м䣬��Ҫ����ֳ�����
                        struct Block* newbk2 = (struct Block*)malloc(sizeof(struct Block));
                        NumofFreeblock++;
                        blockid++;
                        //newbkΪ���̿�ռ�õ��ڴ��
                        newbk->starAddr = Starta;
                        newbk->size = P[i].needeMem;
                        newbk->status = 1;//��ռ�õ��ڴ�
                        newbk->pid = i;
                        newbk->id = blockid;
                        P[i].blockID = newbk->id;
                        P[i].status = 1;
                        //newbk2Ϊ�ָ�ĵ����������ڴ��,���ڴ�δ��ռ��
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
                        //����ԭʼ�ڴ��Ĵ�С���˿��ѱ��ֳ����ݣ���һ�ݴ�С����
                        Freeblock->size = Starta - Freeblock->starAddr;
                        Freeblock->next = newbk2;
                        //��������ڴ�����ʹ�÷������б���
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
                        //��������б������
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
            {//�������ڴ��ռ��С�������˽���������ڴ�ռ�ʱ
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
                        Freeblock = Freeblock->next;//����ͷ��㣬����δ�����ڴ��ı���
                    }
                    if (Freeblock != NULL && P[i].needeMem <= Freeblock->size && Freeblock->id == NumofNowblock)
                    {//����Ҫ��Freeblock��ָ�����������ų�����Freeblock�Ѿ����������һ����ֱ�ӽ�����������һ�����̵ķ���
                        flag = 1;//����������
                        if (P[i].needeMem == Freeblock->size)
                        {//���˽��̵������ڴ�ռ�ʹ��ڴ���С��ͬ
                            while (Usedblock->next != NULL)
                            {//�������ѷ����ڴ�ռ����е��������µ��ڴ������б�
                                Usedblock = Usedblock->next;
                            }
                            Freeblock->status = 1;//�����ڴ���״̬��Ϊ�ѷ���
                            //�����ڴ�ռ��Free�б����ͷ� 
                            Freeblock->prev->next = Freeblock->next;
                            if (Freeblock->next != NULL)
                            {
                                Freeblock->next->prev = Freeblock->prev;
                                NumofNowblock = Freeblock->prev->next->id;
                            }
                            //�����ڴ�ռ������ѷ���������
                            Usedblock->next = Freeblock;
                            Freeblock->prev = Usedblock;
                            Freeblock->next = NULL;
                            //��������б������
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
                            //���������ڴ�ռ�С�ڴ��ڴ��ռ�ʱ
                            struct Block* newbk = (struct Block*)malloc(sizeof(struct Block));
                            int Starta = rand() % (Freeblock->size - P[i].needeMem) + Freeblock->starAddr;//�������ʼλ��
                            if (Starta == Freeblock->starAddr)
                            {//������ķ���ռ���ʼλ�ú��ڴ�����ʼλ����ͬʱ
                                //��ʱֻ��Ҫ�ָ��������
                                //δ����ռ��+1������һ���µ�Ϊ����ռ�
                                NumofFreeblock++;
                                blockid++;
                                //�µĿռ��С���ڴ˿��С-���̿������ڴ�ռ��С
                                newbk->size = Freeblock->size - P[i].needeMem;
                                //Դ�ڴ��ֳ����飬ǰһ��Ŀռ�Ϊ���̿�������ڴ�ռ��С
                                //���ñ�����ʾ�˽��̱�����ռ��
                                Freeblock->size = P[i].needeMem;
                                Freeblock->pid = P[i].pid;
                                Freeblock->status = 1;
                                //���µ��ڴ����뵽δ���������
                                Freeblock->prev->next = newbk;
                                //�����µ��ڴ�����Ϣ
                                newbk->id = blockid;
                                NumofNowblock = newbk->id;
                                newbk->prev = Freeblock->prev;
                                newbk->next = Freeblock->next;
                                newbk->status = 0;
                                newbk->pid = -1;
                                newbk->starAddr = Freeblock->starAddr + Freeblock->size;
                                P[i].blockID = Freeblock->id;
                                //��������ڴ���Ƶ�ʹ���ڴ������
                                while (Usedblock->next != NULL)
                                {
                                    Usedblock = Usedblock->next;
                                }
                                Usedblock->next = Freeblock;
                                Freeblock->next = NULL;
                                Freeblock->prev = Usedblock;
                                //��Ӧ�����������Ӽ���
                                NumofFreeblock--;
                                NumofUsedblock++;
                                //��ʾÿ�����е�����
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
                                //���̿����ô��ڿ�β����Ҳֻ��Ҫ�����ڴ��ֳ����飬���¹�������һ������
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
                                //��������б����Ϣ
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
                                //��������λ���ڿ���м䣬��Ҫ����ֳ�����
                                struct Block* newbk2 = (struct Block*)malloc(sizeof(struct Block));
                                NumofFreeblock++;
                                blockid++;
                                //newbkΪ���̿�ռ�õ��ڴ��
                                newbk->starAddr = Starta;
                                newbk->size = P[i].needeMem;
                                newbk->status = 1;//��ռ�õ��ڴ�
                                newbk->pid = i;
                                newbk->id = blockid;
                                P[i].blockID = newbk->id;
                                P[i].status = 1;
                                //newbk2Ϊ�ָ�ĵ����������ڴ��,���ڴ�δ��ռ��
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
                                //����ԭʼ�ڴ��Ĵ�С���˿��ѱ��ֳ����ݣ���һ�ݴ�С����
                                Freeblock->size = Starta - Freeblock->starAddr;
                                Freeblock->next = newbk2;
                                //��������ڴ�����ʹ�÷������б���
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
                                //��������б������
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
                    {//�������ڴ��ռ��С�������˽���������ڴ�ռ�ʱ
                        flag = 0;
                        continue;
                    }
                }
                if (flag == 1)
                {//��ʾ�˽��̿��Ѿ������䣬ֱ������whileѭ��
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
    //�����ڴ���ͷſռ�
    struct Block* tmp = bk2; tmp = tmp->next;
    int count = 0;
    while (count < NumofUsedblock)
    {
        tmp = bk2; tmp = tmp->next;
        printf("Recycling used memory block for process %d\n", tmp->pid);
        struct Block* tmp1 = bk1;//δ�����ڴ�������
        for (int i = 0; i < NumofFreeblock; i++)
        {
            tmp1 = tmp1->next;
            if (i == 0 && tmp->size + tmp->starAddr <= tmp1->starAddr)
            {//����ʹ���ڴ�鴦��δ�����ڴ�����ǰ��
                if (tmp->size + tmp->starAddr == tmp1->starAddr)
                {//�����ڴ���������ڴ������
                    tmp1->starAddr = tmp->starAddr;
                    tmp1->size = tmp->size + tmp1->size;
                    //��ʹ���ڴ���ʹ���ڴ���������Ƴ�
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //��������б����Ϣ
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
                {//�����ڴ���������ڴ�鲻����
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
                    //��������б����Ϣ
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
                {//ֻ������ĺϲ�
                    tmp1->next->starAddr = tmp->starAddr;
                    tmp1->next->size = tmp1->next->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //��������б����Ϣ
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
                {//ֻ������ĺϲ�
                    tmp1->size = tmp1->size + tmp->size;
                    tmp->prev->next = tmp->next;
                    if (tmp->next != NULL)
                    {
                        tmp->next->prev = tmp->prev;
                    }
                    NumofUsedblock--;
                    //��������б����Ϣ
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
                {//������һ��ϲ�
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
                    //��������б����Ϣ
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
                {//�����߶����ϲ�
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
                    //��������б����Ϣ
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
                    //��������б����Ϣ
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
                    //��������б����Ϣ
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
    //ʮ��PCB����
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
    //�ڴ��ʼ��
    //bk��������ͷ���
    struct Block* bkhead = (struct Block*)malloc(sizeof(struct Block));
    struct Block* bk = (struct Block*)malloc(sizeof(struct Block));
    bkhead->next = bk;
    NumofFreeblock = 1;
    blockid = 1;
    bkhead->prev = NULL;
    bk->id = blockid;//���������
    bk->size = 1024;
    bk->starAddr = 0;
    bk->status = 0;//0��ʾ���У�1��ʾռ��
    bk->pid = -1;
    bk->prev = bkhead;
    bk->next = NULL;
    printf("FreeBlockID          StartAddr          Size\n");
    printf("%11d          %9d          %4d\n", bk->id, bk->starAddr, bk->size);
    printf("********************************************************\n");
    struct Block* usedbkhead = (struct Block*)malloc(sizeof(struct Block));//ʹ�ö��е�ͷָ��
    usedbkhead->next = NULL;
    NumofUsedblock = 0;
    //FF(proc, 10, bkhead, usedbkhead);
    NumofNowblock = 1;//��ʾ��ǰ�����еĿ�
    NF(proc, 10, bkhead, usedbkhead);
    system("pause");
    return 1;
}