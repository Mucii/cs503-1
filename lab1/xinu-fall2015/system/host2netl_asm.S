/* File for host2netl_asm and net2hostl_asm functions. Must be implemented in x86 assembly. */
.text

/* 
* This function will load the argument in the eax register
* Then it uses the bswap call to swap the bits 8-1 to 32-25 and 16-9 to 24-17
*/
.global host2netl_asm
host2netl_asm:
  
  movl 4(%esp), %eax
  bswapl %eax
  ret

/* 
* This function will load the argument in the eax register
* Then it uses the bswap call to swap the bits 8-1 to 32-25 and 16-9 to 24-17
* It is essentially same as the above method
*/
.global net2hostl_asm
net2hostl_asm:
  
  movl 4(%esp), %eax
  bswapl %eax
  ret
