#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>
#include "string_tools.h"

int get_proc_task(pid_t pid) {
	mach_port_t task;
	kern_return_t kr = task_for_pid(current_task(), pid, &task);

	if (kr != KERN_SUCCESS){
		printf("[ >>> ] %d -> %x [ error code: %d]\n", pid, task, kr);
		return -1;
	}
	printf("[ >>> ] %d -> %x [%d]\n", pid, task, kr);
	return task;
}

int has_aslr() {
    const struct mach_header *mach;
    mach = _dyld_get_image_header(0);
    
    if (mach->flags & MH_PIE) {
		return 0;
    } else {
		return -1;
    }
}

uint8_t read_mem(uint64_t addr, int task_port){
	kern_return_t kernel;
	int size = 8;
	unsigned char buffer[size];

	kernel = vm_read_overwrite(task_port,(vm_address_t)addr,size,(vm_address_t)&buffer,(vm_size_t *)&size);

	if(kernel != KERN_SUCCESS){
		printf("[ >>> ] failed to read! - %s\n",mach_error_string(kernel));
	}

	int a = 0;
	for(int i = 0; i < size; i+=8){
		printf("0x%.08llx: %.02x %.02x %.02x %.02x %.02x %.02x %.02x %.02x\n",addr+i,buffer[a],buffer[a+1],buffer[a+2],buffer[a+3],buffer[a+4],buffer[a+5],buffer[a+6],buffer[a+7]);  //@bellis1000
		a+=8;
	}
	return 0;
}

int main(int argc, char *argv[], char *envp[]) {
	uint64_t addr;
	pid_t pid = argc >= 2 ? atoi(argv[1]) : 6942069;

	sscanf(argv[2], "%llx", &addr);

	bool aslr = has_aslr();

	if(aslr == 0){
		printf("[ >>> ] has aslr? : True\n");
	} else {
		printf("[ >>> ] has aslr? : False\n");
	}	

	if(pid == 6942069){
		printf("[ >>> ] usage: vmreadtool [pid] [address]\n"); // include a bytes option
		return -1;
	}

	int task_port = get_proc_task(pid);

	if(task_port == -1){
		return -1;
	}
	
	printf("[ >>> ] init vm_read for task port %d with address 0x%llx\n", task_port, addr);

	int i;
	int re;

	for(i = 0; i < 5; i++){
		re = read_mem(addr, task_port);
		addr += 16;
	}


	return 0;

}
