#ifndef SORYU_KSYM_SHIM_H
#define SORYU_KSYM_SHIM_H

#include <linux/fs.h>
#include <linux/cred.h>
#include <linux/namei.h>
#include <linux/fdtable.h>
#include <linux/task_work.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/xarray.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>
#include <linux/security.h>
#include <asm/cacheflush.h>

unsigned long soryu_ksym_probe(const char *name);
void soryu_shim_resolve(void);

#ifndef SORYU_KSYM_NO_MACRO
extern typeof(&kallsyms_lookup_name) soryu_p_kallsyms_lookup_name;
extern typeof(&filp_open) soryu_p_filp_open;
extern typeof(&kernel_read) soryu_p_kernel_read;
extern typeof(&kernel_write) soryu_p_kernel_write;
extern typeof(&override_creds) soryu_p_override_creds;
extern typeof(&revert_creds) soryu_p_revert_creds;
extern typeof(&set_security_override_from_ctx) soryu_p_set_sec_ctx;
extern typeof(&task_work_add) soryu_p_task_work_add;
extern typeof(&close_fd) soryu_p_close_fd;
extern typeof(&kern_path) soryu_p_kern_path;
extern typeof(&get_task_exe_file) soryu_p_get_task_exe_file;
extern typeof(&groups_search) soryu_p_groups_search;
extern typeof(&groups_alloc) soryu_p_groups_alloc;
extern typeof(&groups_sort) soryu_p_groups_sort;
extern typeof(&set_current_groups) soryu_p_set_current_groups;
extern typeof(&get_vm_area_caller) soryu_p_get_vm_area_caller;
extern typeof(&apply_to_page_range) soryu_p_apply_to_page_range;
extern typeof(&free_vm_area) soryu_p_free_vm_area;
extern typeof(&__sync_icache_dcache) soryu_p_sync_icache;
extern typeof(&copy_to_kernel_nofault) soryu_p_copy_nofault;
extern typeof(&dcache_clean_inval_poc) soryu_p_dcache_poc;
extern typeof(&kallsyms_lookup_size_offset) soryu_p_klso;
extern typeof(&xas_find_marked) soryu_p_xas_find_marked;
extern struct mm_struct *soryu_p_init_mm;
#endif

#ifndef SORYU_KSYM_NO_MACRO
#define kallsyms_lookup_name soryu_p_kallsyms_lookup_name
#define filp_open soryu_p_filp_open
#define kernel_read soryu_p_kernel_read
#define kernel_write soryu_p_kernel_write
#define override_creds soryu_p_override_creds
#define revert_creds soryu_p_revert_creds
#define set_security_override_from_ctx soryu_p_set_sec_ctx
#define task_work_add soryu_p_task_work_add
#define close_fd soryu_p_close_fd
#define kern_path soryu_p_kern_path
#define get_task_exe_file soryu_p_get_task_exe_file
#define groups_search soryu_p_groups_search
#define groups_alloc soryu_p_groups_alloc
#define groups_sort soryu_p_groups_sort
#define set_current_groups soryu_p_set_current_groups
#define get_vm_area_caller soryu_p_get_vm_area_caller
#define apply_to_page_range soryu_p_apply_to_page_range
#define free_vm_area soryu_p_free_vm_area
#define __sync_icache_dcache soryu_p_sync_icache
#define copy_to_kernel_nofault soryu_p_copy_nofault
#define dcache_clean_inval_poc soryu_p_dcache_poc
#define kallsyms_lookup_size_offset soryu_p_klso
#define xas_find_marked soryu_p_xas_find_marked
#define init_mm (*soryu_p_init_mm)
#endif

#endif
