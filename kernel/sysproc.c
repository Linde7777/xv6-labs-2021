#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 page_va;
  int size;
  uint64 user_buffer_addr;
  
  if(argaddr(0, &page_va) < 0){
    return -1;
  }
  if(argint(1, &size) < 0){
    return -1;
  }
  if(argaddr(2, &user_buffer_addr) < 0){
    return -1;
  }
  
  struct proc *p = myproc();
  uint32 local_buffer = 0;
  uint64 true_page_va;
  true_page_va = PGROUNDDOWN(page_va);
  for(int i = 0; i < size && true_page_va < MAXVA; i++, true_page_va += PGSIZE){
    pte_t *pte = walk(p->pagetable, true_page_va, 0);
    if(pte){
      if(*pte & PTE_A){
        local_buffer |= (1 << i);
        *pte ^= PTE_A;
      }
    }
  }
  
  copyout(p->pagetable, user_buffer_addr, (char*)&local_buffer, sizeof(local_buffer));
  return 0;
}
#endif

int
temp(void){
  /*
  uint64 va;
  int num_of_page;
  uint64 user_buffer_addr;
  if (argaddr(0, &va) < 0) {
    return -1;
  }

  if (argint(1, &num_of_page) < 0) {
    return -1;
  }

  if (argaddr(2, &user_buffer_addr) < 0) {
    return -1;
  }

  if (num_of_page > 32 || num_of_page < 0) {
    return -1;
  }

  int local_buffer;
  struct proc *p = myproc();
  uint64 s;
  s = PGROUNDDOWN(va);
  for(int i = 0; i < num_of_page&& s < MAXVA; i++, s += PGSIZE){
    pte_t *pte = walk(p->pagetable, s, 0);
    if(pte){
      if(*pte & PTE_A){
        local_buffer |= (1 << i);
        *pte ^= PTE_A;
      }
    }
  }
  char *p_local_buffer = (char *)&local_buffer;
  copyout(p->pagetable, user_buffer_addr, p_local_buffer, sizeof(local_buffer));

  // todo: copy the newest code to sys_pgaccess()
  return 0;
  */
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
