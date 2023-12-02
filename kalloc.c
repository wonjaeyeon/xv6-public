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
uint pgrefcount[PHYSTOP >> PGSHIFT]; // declare pgrefcount

void freerange(void *vstart, void *vend);

extern char end[]; // first address after kernel loaded from ELF file
// defined by the kernel linker script in kernel.ld


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
    initlock(&kmem.lock, "kmem");
    kmem.use_lock = 0;
    num_free_pages = 0;
    freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend) {
    //memset(pgrefcount, 0, sizeof(uint) * (PHYSTOP >> PGSHIFT));
    //acquire(&pmem.lock);
    //release(&pmem.lock);
    freerange(vstart, vend);
    kmem.use_lock = 1;
}


void
freerange(void *vstart, void *vend) {
    char *p;
    p = (char *) PGROUNDUP((uint) vstart);
    memset(&pgrefcount, 0, sizeof(uint) * (PHYSTOP >> PGSHIFT)); // initialize pgrefcount array to 0
    for (; p + PGSIZE <= (char *) vend; p += PGSIZE)
        kfree(p);
}

int
freemem()
{
    return num_free_pages;
}

uint
get_refcount(uint pa)
{
    if (pa >= PHYSTOP)
    {
        cprintf("pa is over PHYSTOP(pa >= PHYSTOP)");
    }

    return pgrefcount[pa >> PGSHIFT];
}

void
inc_refcount(uint pa)
{
    if (pa >= PHYSTOP)
    {
        cprintf("pa is over PHYSTOP(pa >= PHYSTOP)");
    }

    pgrefcount[pa >> PGSHIFT]++;
}

void
dec_refcount(uint pa)
{
    if (pa >= PHYSTOP)
    {
        cprintf("pa is over PHYSTOP(pa >= PHYSTOP)");
    }

    pgrefcount[pa >> PGSHIFT]--;
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

    // if ref_count is greater than 0, then there is a process that shares the page
    // so the ref_count is decremented.
    if (pgrefcount[pa >> PGSHIFT] > 0)
        dec_refcount(pa);

    // if ref_count is 0, then there is no process that shares the page
    // so the page is added to the freelist
    if (pgrefcount[pa >> PGSHIFT] <= 0) {

        memset(v, 1, PGSIZE);
        r->next = kmem.freelist;
        num_free_pages++;
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
        num_free_pages--;
        pgrefcount[address >> PGSHIFT]=1;
    }
    if(kmem.use_lock)
        release(&kmem.lock);

    return (char*)r;
}