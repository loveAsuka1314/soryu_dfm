#define SORYU_KSYM_NO_MACRO
#include <linux/kernel.h>
#include <linux/module.h>
#include "ksym.h"
#include "ksym_shim.h"

typeof(&kallsyms_lookup_name) soryu_p_kallsyms_lookup_name;
typeof(&filp_open) soryu_p_filp_open;
typeof(&kernel_read) soryu_p_kernel_read;
typeof(&kernel_write) soryu_p_kernel_write;
typeof(&override_creds) soryu_p_override_creds;
typeof(&revert_creds) soryu_p_revert_creds;
typeof(&set_security_override_from_ctx) soryu_p_set_sec_ctx;
typeof(&task_work_add) soryu_p_task_work_add;
typeof(&close_fd) soryu_p_close_fd;
typeof(&kern_path) soryu_p_kern_path;
typeof(&get_task_exe_file) soryu_p_get_task_exe_file;
typeof(&groups_search) soryu_p_groups_search;
typeof(&groups_alloc) soryu_p_groups_alloc;
typeof(&groups_sort) soryu_p_groups_sort;
typeof(&set_current_groups) soryu_p_set_current_groups;
typeof(&get_vm_area_caller) soryu_p_get_vm_area_caller;
typeof(&apply_to_page_range) soryu_p_apply_to_page_range;
typeof(&free_vm_area) soryu_p_free_vm_area;
typeof(&__sync_icache_dcache) soryu_p_sync_icache;
typeof(&copy_to_kernel_nofault) soryu_p_copy_nofault;
typeof(&dcache_clean_inval_poc) soryu_p_dcache_poc;
typeof(&kallsyms_lookup_size_offset) soryu_p_klso;
typeof(&xas_find_marked) soryu_p_xas_find_marked;
struct mm_struct *soryu_p_init_mm;
void (*soryu_p_set_fixmap)(enum fixed_addresses, phys_addr_t, pgprot_t);

void soryu_shim_resolve(void)
{
    soryu_p_kallsyms_lookup_name = soryu_ksym_raw;
    soryu_p_filp_open = (void *)soryu_ksym_raw("filp_open");
    soryu_p_kernel_read = (void *)soryu_ksym_raw("kernel_read");
    soryu_p_kernel_write = (void *)soryu_ksym_raw("kernel_write");
    soryu_p_override_creds = (void *)soryu_ksym_raw("override_creds");
    soryu_p_revert_creds = (void *)soryu_ksym_raw("revert_creds");
    soryu_p_set_sec_ctx = (void *)soryu_ksym_raw("set_security_override_from_ctx");
    soryu_p_task_work_add = (void *)soryu_ksym_raw("task_work_add");
    soryu_p_close_fd = (void *)soryu_ksym_raw("close_fd");
    soryu_p_kern_path = (void *)soryu_ksym_raw("kern_path");
    soryu_p_get_task_exe_file = (void *)soryu_ksym_raw("get_task_exe_file");
    soryu_p_groups_search = (void *)soryu_ksym_raw("groups_search");
    soryu_p_groups_alloc = (void *)soryu_ksym_raw("groups_alloc");
    soryu_p_groups_sort = (void *)soryu_ksym_raw("groups_sort");
    soryu_p_set_current_groups = (void *)soryu_ksym_raw("set_current_groups");
    soryu_p_get_vm_area_caller = (void *)soryu_ksym_raw("get_vm_area_caller");
    soryu_p_apply_to_page_range = (void *)soryu_ksym_raw("apply_to_page_range");
    soryu_p_free_vm_area = (void *)soryu_ksym_raw("free_vm_area");
    soryu_p_sync_icache = (void *)soryu_ksym_raw("__sync_icache_dcache");
    soryu_p_copy_nofault = (void *)soryu_ksym_raw("copy_to_kernel_nofault");
    soryu_p_dcache_poc = (void *)soryu_ksym_raw("dcache_clean_inval_poc");
    soryu_p_klso = (void *)soryu_ksym_raw("kallsyms_lookup_size_offset");
    soryu_p_xas_find_marked = (void *)soryu_ksym_raw("xas_find_marked");
    soryu_p_init_mm = (struct mm_struct *)soryu_ksym_raw("init_mm");
    soryu_p_set_fixmap = (void *)soryu_ksym_raw("__set_fixmap");
}
