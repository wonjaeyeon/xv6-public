// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

uint num_free_pages; // declare num_free_pages
//uint pgrefcount[PHYSTOP >> PGSHIFT]; // declare pgrefcount

void freerange(void *vstart, void *vend);

extern char end[]; // first address after kernel loaded from ELF file
// defined by the kernel linker script in kernel.ld

struct {
    struct spinlock lock;
    int use_lock;
    int numfreepages;
    uint pgrefcount[PHYSTOP >> PGSHIFT];
} pmem;

struct run {
    struct run *next;
};

struct {
    struct spinlock lock;
    int use_lock;
    struct run *freelist;
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend) {
    initlock(&pmem.lock, "pmemlock");
    pmem.use_lock = 0;
    initlock(&kmem.lock, "kmem");
    kmem.use_lock = 0;
    freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend) {
    pmem.use_lock = 1;
    memset(&pmem.pgrefcount, 0, sizeof(uint) * (PHYSTOP >> PGSHIFT));
    acquire(&pmem.lock);
    pmem.numfreepages = 0;
    release(&pmem.lock);
    freerange(vstart, vend);
    kmem.use_lock = 1;
}

void free_page(char *v) {
    struct run *r;

    if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
        panic("free_page");

    // Fill with junk to catch dangling refs.
    memset(v, 1, PGSIZE);

    if(kmem.use_lock)
        acquire(&kmem.lock);

    r = (struct run*)v;
    r->next = kmem.freelist;
    kmem.freelist = r;

    if(kmem.use_lock)
        release(&kmem.lock);
}

void
freerange(void *vstart, void *vend) {
    char *p;
    p = (char *) PGROUNDUP((uint) vstart);
    for (; p + PGSIZE <= (char *) vend; p += PGSIZE)
        kfree(p);
}

int freemem()
{
    int numfreepages;
    acquire(&pmem.lock);
    numfreepages = pmem.numfreepages;
    release(&pmem.lock);
    return numfreepages;
}

uint
get_ref(uint pa)
{
    if (pa >= PHYSTOP)
    {
        cprintf("pa >= PHYSTOP");
    }

    return pmem.pgrefcount[pa >> PGSHIFT];

}

void
inc_ref(uint pa)
{
    if (pa >= PHYSTOP)
    {
        cprintf("pa >= PHYSTOP");
    }

    pmem.pgrefcount[pa >> PGSHIFT]++;

}

void
dec_ref(uint pa)
{

    if (pa >= PHYSTOP)
    {
        cprintf("pa >= PHYSTOP");
    }

    pmem.pgrefcount[pa >> PGSHIFT]--;
}

//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
    struct run *r;

    if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
        panic("kfree");

    r = (struct run*)v;


    uint pa = V2P(v);

    // ref_count 가 0보다 크면 공유하고있는 페이지가 있는것이므로
    // ref_count --;
    if (pmem.pgrefcount[pa >> PGSHIFT] > 0)
        pmem.pgrefcount[pa >> PGSHIFT]--;

    // ref_count 가 0보다 작거나 같으면 해당 페이지를 공유하는 프로세스가 없으므로
    // 할당 해제해주고 freelist에 추가시킴.
    if (pmem.pgrefcount[pa >> PGSHIFT] <= 0) {

        memset(v, 1, PGSIZE);
        r->next = kmem.freelist;
        pmem.numfreepages++;
        kmem.freelist = r;


    }


}
// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
// In kalloc function
char*
kalloc(void)
{
    struct run *r;

    if(kmem.use_lock)
        acquire(&kmem.lock);
    r = kmem.freelist;
    if(r){
        kmem.freelist = r->next;
        uint address = V2P((uint)r);
        pmem.numfreepages--;
        pmem.pgrefcount[address >> PGSHIFT]++;
    }
    if(kmem.use_lock)
        release(&kmem.lock);

    return (char*)r;
}

