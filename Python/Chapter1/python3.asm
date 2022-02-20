
python3:     file format elf64-x86-64


Disassembly of section .init:

0000000000000998 <.init>:
 998:	f3 0f 1e fa          	endbr64 
 99c:	48 83 ec 08          	sub    $0x8,%rsp
 9a0:	48 8b 05 49 16 20 00 	mov    0x201649(%rip),%rax        # 201ff0 <__gmon_start__>
 9a7:	48 85 c0             	test   %rax,%rax
 9aa:	74 02                	je     9ae <PyMem_RawMalloc@plt-0x22>
 9ac:	ff d0                	callq  *%rax
 9ae:	48 83 c4 08          	add    $0x8,%rsp
 9b2:	c3                   	retq   

Disassembly of section .plt:

00000000000009c0 <PyMem_RawMalloc@plt-0x10>:
 9c0:	ff 35 aa 15 20 00    	pushq  0x2015aa(%rip)        # 201f70 <_IO_stdin_used@@Base+0x201108>
 9c6:	ff 25 ac 15 20 00    	jmpq   *0x2015ac(%rip)        # 201f78 <_IO_stdin_used@@Base+0x201110>
 9cc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000009d0 <PyMem_RawMalloc@plt>:
 9d0:	ff 25 aa 15 20 00    	jmpq   *0x2015aa(%rip)        # 201f80 <PyMem_RawMalloc>
 9d6:	68 00 00 00 00       	pushq  $0x0
 9db:	e9 e0 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

00000000000009e0 <__cxa_finalize@plt>:
 9e0:	ff 25 a2 15 20 00    	jmpq   *0x2015a2(%rip)        # 201f88 <__cxa_finalize@GLIBC_2.2.5>
 9e6:	68 01 00 00 00       	pushq  $0x1
 9eb:	e9 d0 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

00000000000009f0 <Py_Main@plt>:
 9f0:	ff 25 9a 15 20 00    	jmpq   *0x20159a(%rip)        # 201f90 <Py_Main>
 9f6:	68 02 00 00 00       	pushq  $0x2
 9fb:	e9 c0 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a00 <_PyMem_RawStrdup@plt>:
 a00:	ff 25 92 15 20 00    	jmpq   *0x201592(%rip)        # 201f98 <_PyMem_RawStrdup>
 a06:	68 03 00 00 00       	pushq  $0x3
 a0b:	e9 b0 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a10 <PyMem_RawFree@plt>:
 a10:	ff 25 8a 15 20 00    	jmpq   *0x20158a(%rip)        # 201fa0 <PyMem_RawFree>
 a16:	68 04 00 00 00       	pushq  $0x4
 a1b:	e9 a0 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a20 <setlocale@plt>:
 a20:	ff 25 82 15 20 00    	jmpq   *0x201582(%rip)        # 201fa8 <setlocale@GLIBC_2.2.5>
 a26:	68 05 00 00 00       	pushq  $0x5
 a2b:	e9 90 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a30 <_PyMem_SetupAllocators@plt>:
 a30:	ff 25 7a 15 20 00    	jmpq   *0x20157a(%rip)        # 201fb0 <_PyMem_SetupAllocators>
 a36:	68 06 00 00 00       	pushq  $0x6
 a3b:	e9 80 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a40 <Py_DecodeLocale@plt>:
 a40:	ff 25 72 15 20 00    	jmpq   *0x201572(%rip)        # 201fb8 <Py_DecodeLocale>
 a46:	68 07 00 00 00       	pushq  $0x7
 a4b:	e9 70 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a50 <__fprintf_chk@plt>:
 a50:	ff 25 6a 15 20 00    	jmpq   *0x20156a(%rip)        # 201fc0 <__fprintf_chk@GLIBC_2.3.4>
 a56:	68 08 00 00 00       	pushq  $0x8
 a5b:	e9 60 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a60 <_Py_LegacyLocaleDetected@plt>:
 a60:	ff 25 62 15 20 00    	jmpq   *0x201562(%rip)        # 201fc8 <_Py_LegacyLocaleDetected>
 a66:	68 09 00 00 00       	pushq  $0x9
 a6b:	e9 50 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

0000000000000a70 <_Py_CoerceLegacyLocale@plt>:
 a70:	ff 25 5a 15 20 00    	jmpq   *0x20155a(%rip)        # 201fd0 <_Py_CoerceLegacyLocale>
 a76:	68 0a 00 00 00       	pushq  $0xa
 a7b:	e9 40 ff ff ff       	jmpq   9c0 <PyMem_RawMalloc@plt-0x10>

Disassembly of section .text:

0000000000000a80 <main@@Base>:
 a80:	41 57                	push   %r15
 a82:	41 56                	push   %r14
 a84:	41 55                	push   %r13
 a86:	49 89 f5             	mov    %rsi,%r13
 a89:	41 54                	push   %r12
 a8b:	41 89 fc             	mov    %edi,%r12d
 a8e:	48 8d 3d d7 03 00 00 	lea    0x3d7(%rip),%rdi        # e6c <_IO_stdin_used@@Base+0x4>
 a95:	55                   	push   %rbp
 a96:	53                   	push   %rbx
 a97:	41 8d 5c 24 01       	lea    0x1(%r12),%ebx
 a9c:	4c 63 f3             	movslq %ebx,%r14
 a9f:	49 c1 e6 03          	shl    $0x3,%r14
 aa3:	48 83 ec 18          	sub    $0x18,%rsp
 aa7:	e8 84 ff ff ff       	callq  a30 <_PyMem_SetupAllocators@plt>
 aac:	4c 89 f7             	mov    %r14,%rdi
 aaf:	e8 1c ff ff ff       	callq  9d0 <PyMem_RawMalloc@plt>
 ab4:	4c 89 f7             	mov    %r14,%rdi
 ab7:	48 89 c5             	mov    %rax,%rbp
 aba:	e8 11 ff ff ff       	callq  9d0 <PyMem_RawMalloc@plt>
 abf:	48 85 ed             	test   %rbp,%rbp
 ac2:	0f 84 ac 01 00 00    	je     c74 <main@@Base+0x1f4>
 ac8:	48 89 c3             	mov    %rax,%rbx
 acb:	48 85 c0             	test   %rax,%rax
 ace:	0f 84 a0 01 00 00    	je     c74 <main@@Base+0x1f4>
 ad4:	31 f6                	xor    %esi,%esi
 ad6:	bf 06 00 00 00       	mov    $0x6,%edi
 adb:	e8 40 ff ff ff       	callq  a20 <setlocale@plt>
 ae0:	48 89 c7             	mov    %rax,%rdi
 ae3:	e8 18 ff ff ff       	callq  a00 <_PyMem_RawStrdup@plt>
 ae8:	48 89 04 24          	mov    %rax,(%rsp)
 aec:	48 85 c0             	test   %rax,%rax
 aef:	0f 84 d4 01 00 00    	je     cc9 <main@@Base+0x249>
 af5:	48 8d 35 85 03 00 00 	lea    0x385(%rip),%rsi        # e81 <_IO_stdin_used@@Base+0x19>
 afc:	bf 06 00 00 00       	mov    $0x6,%edi
 b01:	e8 1a ff ff ff       	callq  a20 <setlocale@plt>
 b06:	e8 55 ff ff ff       	callq  a60 <_Py_LegacyLocaleDetected@plt>
 b0b:	85 c0                	test   %eax,%eax
 b0d:	0f 85 57 01 00 00    	jne    c6a <main@@Base+0x1ea>
 b13:	45 85 e4             	test   %r12d,%r12d
 b16:	7e 46                	jle    b5e <main@@Base+0xde>
 b18:	45 8d 74 24 ff       	lea    -0x1(%r12),%r14d
 b1d:	45 31 ff             	xor    %r15d,%r15d
 b20:	eb 11                	jmp    b33 <main@@Base+0xb3>
 b22:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
 b29:	00 00 00 00 
 b2d:	0f 1f 00             	nopl   (%rax)
 b30:	49 89 c7             	mov    %rax,%r15
 b33:	4b 8b 7c fd 00       	mov    0x0(%r13,%r15,8),%rdi
 b38:	31 f6                	xor    %esi,%esi
 b3a:	e8 01 ff ff ff       	callq  a40 <Py_DecodeLocale@plt>
 b3f:	41 8d 4f 01          	lea    0x1(%r15),%ecx
 b43:	4a 89 44 fd 00       	mov    %rax,0x0(%rbp,%r15,8)
 b48:	48 85 c0             	test   %rax,%rax
 b4b:	0f 84 45 01 00 00    	je     c96 <main@@Base+0x216>
 b51:	4a 89 04 fb          	mov    %rax,(%rbx,%r15,8)
 b55:	49 8d 47 01          	lea    0x1(%r15),%rax
 b59:	4d 39 f7             	cmp    %r14,%r15
 b5c:	75 d2                	jne    b30 <main@@Base+0xb0>
 b5e:	49 63 d4             	movslq %r12d,%rdx
 b61:	4c 8b 2c 24          	mov    (%rsp),%r13
 b65:	bf 06 00 00 00       	mov    $0x6,%edi
 b6a:	48 c7 44 d5 00 00 00 	movq   $0x0,0x0(%rbp,%rdx,8)
 b71:	00 00 
 b73:	48 c7 04 d3 00 00 00 	movq   $0x0,(%rbx,%rdx,8)
 b7a:	00 
 b7b:	4c 89 ee             	mov    %r13,%rsi
 b7e:	e8 9d fe ff ff       	callq  a20 <setlocale@plt>
 b83:	4c 89 ef             	mov    %r13,%rdi
 b86:	e8 85 fe ff ff       	callq  a10 <PyMem_RawFree@plt>
 b8b:	44 89 e7             	mov    %r12d,%edi
 b8e:	48 89 ee             	mov    %rbp,%rsi
 b91:	e8 5a fe ff ff       	callq  9f0 <Py_Main@plt>
 b96:	48 8d 3d cf 02 00 00 	lea    0x2cf(%rip),%rdi        # e6c <_IO_stdin_used@@Base+0x4>
 b9d:	41 89 c7             	mov    %eax,%r15d
 ba0:	e8 8b fe ff ff       	callq  a30 <_PyMem_SetupAllocators@plt>
 ba5:	45 85 e4             	test   %r12d,%r12d
 ba8:	0f 8e 9a 00 00 00    	jle    c48 <main@@Base+0x1c8>
 bae:	48 8b 3b             	mov    (%rbx),%rdi
 bb1:	e8 5a fe ff ff       	callq  a10 <PyMem_RawFree@plt>
 bb6:	41 83 fc 01          	cmp    $0x1,%r12d
 bba:	0f 84 88 00 00 00    	je     c48 <main@@Base+0x1c8>
 bc0:	48 8b 7b 08          	mov    0x8(%rbx),%rdi
 bc4:	e8 47 fe ff ff       	callq  a10 <PyMem_RawFree@plt>
 bc9:	41 83 fc 02          	cmp    $0x2,%r12d
 bcd:	74 79                	je     c48 <main@@Base+0x1c8>
 bcf:	48 8b 7b 10          	mov    0x10(%rbx),%rdi
 bd3:	e8 38 fe ff ff       	callq  a10 <PyMem_RawFree@plt>
 bd8:	41 83 fc 03          	cmp    $0x3,%r12d
 bdc:	74 6a                	je     c48 <main@@Base+0x1c8>
 bde:	48 8b 7b 18          	mov    0x18(%rbx),%rdi
 be2:	e8 29 fe ff ff       	callq  a10 <PyMem_RawFree@plt>
 be7:	41 83 fc 04          	cmp    $0x4,%r12d
 beb:	74 5b                	je     c48 <main@@Base+0x1c8>
 bed:	48 8b 7b 20          	mov    0x20(%rbx),%rdi
 bf1:	e8 1a fe ff ff       	callq  a10 <PyMem_RawFree@plt>
 bf6:	41 83 fc 05          	cmp    $0x5,%r12d
 bfa:	74 4c                	je     c48 <main@@Base+0x1c8>
 bfc:	48 8b 7b 28          	mov    0x28(%rbx),%rdi
 c00:	e8 0b fe ff ff       	callq  a10 <PyMem_RawFree@plt>
 c05:	41 83 fc 06          	cmp    $0x6,%r12d
 c09:	74 3d                	je     c48 <main@@Base+0x1c8>
 c0b:	48 8b 7b 30          	mov    0x30(%rbx),%rdi
 c0f:	e8 fc fd ff ff       	callq  a10 <PyMem_RawFree@plt>
 c14:	41 83 fc 07          	cmp    $0x7,%r12d
 c18:	74 2e                	je     c48 <main@@Base+0x1c8>
 c1a:	48 8b 7b 38          	mov    0x38(%rbx),%rdi
 c1e:	e8 ed fd ff ff       	callq  a10 <PyMem_RawFree@plt>
 c23:	41 83 fc 08          	cmp    $0x8,%r12d
 c27:	74 1f                	je     c48 <main@@Base+0x1c8>
 c29:	41 8d 74 24 f7       	lea    -0x9(%r12),%esi
 c2e:	4c 8d 73 40          	lea    0x40(%rbx),%r14
 c32:	4c 8d 64 f3 48       	lea    0x48(%rbx,%rsi,8),%r12
 c37:	49 8b 3e             	mov    (%r14),%rdi
 c3a:	49 83 c6 08          	add    $0x8,%r14
 c3e:	e8 cd fd ff ff       	callq  a10 <PyMem_RawFree@plt>
 c43:	4d 39 e6             	cmp    %r12,%r14
 c46:	75 ef                	jne    c37 <main@@Base+0x1b7>
 c48:	48 89 ef             	mov    %rbp,%rdi
 c4b:	e8 c0 fd ff ff       	callq  a10 <PyMem_RawFree@plt>
 c50:	48 89 df             	mov    %rbx,%rdi
 c53:	e8 b8 fd ff ff       	callq  a10 <PyMem_RawFree@plt>
 c58:	48 83 c4 18          	add    $0x18,%rsp
 c5c:	44 89 f8             	mov    %r15d,%eax
 c5f:	5b                   	pop    %rbx
 c60:	5d                   	pop    %rbp
 c61:	41 5c                	pop    %r12
 c63:	41 5d                	pop    %r13
 c65:	41 5e                	pop    %r14
 c67:	41 5f                	pop    %r15
 c69:	c3                   	retq   
 c6a:	e8 01 fe ff ff       	callq  a70 <_Py_CoerceLegacyLocale@plt>
 c6f:	e9 9f fe ff ff       	jmpq   b13 <main@@Base+0x93>
 c74:	48 8b 3d a5 13 20 00 	mov    0x2013a5(%rip),%rdi        # 202020 <stderr@@GLIBC_2.2.5>
 c7b:	48 8d 15 f1 01 00 00 	lea    0x1f1(%rip),%rdx        # e73 <_IO_stdin_used@@Base+0xb>
 c82:	be 01 00 00 00       	mov    $0x1,%esi
 c87:	31 c0                	xor    %eax,%eax
 c89:	41 bf 01 00 00 00    	mov    $0x1,%r15d
 c8f:	e8 bc fd ff ff       	callq  a50 <__fprintf_chk@plt>
 c94:	eb c2                	jmp    c58 <main@@Base+0x1d8>
 c96:	48 8b 3c 24          	mov    (%rsp),%rdi
 c9a:	89 4c 24 0c          	mov    %ecx,0xc(%rsp)
 c9e:	41 bf 01 00 00 00    	mov    $0x1,%r15d
 ca4:	e8 67 fd ff ff       	callq  a10 <PyMem_RawFree@plt>
 ca9:	8b 4c 24 0c          	mov    0xc(%rsp),%ecx
 cad:	be 01 00 00 00       	mov    $0x1,%esi
 cb2:	31 c0                	xor    %eax,%eax
 cb4:	48 8b 3d 65 13 20 00 	mov    0x201365(%rip),%rdi        # 202020 <stderr@@GLIBC_2.2.5>
 cbb:	48 8d 15 c6 01 00 00 	lea    0x1c6(%rip),%rdx        # e88 <_IO_stdin_used@@Base+0x20>
 cc2:	e8 89 fd ff ff       	callq  a50 <__fprintf_chk@plt>
 cc7:	eb 8f                	jmp    c58 <main@@Base+0x1d8>
 cc9:	48 8b 3d 50 13 20 00 	mov    0x201350(%rip),%rdi        # 202020 <stderr@@GLIBC_2.2.5>
 cd0:	48 8d 15 9c 01 00 00 	lea    0x19c(%rip),%rdx        # e73 <_IO_stdin_used@@Base+0xb>
 cd7:	be 01 00 00 00       	mov    $0x1,%esi
 cdc:	41 bf 01 00 00 00    	mov    $0x1,%r15d
 ce2:	e8 69 fd ff ff       	callq  a50 <__fprintf_chk@plt>
 ce7:	e9 6c ff ff ff       	jmpq   c58 <main@@Base+0x1d8>
 cec:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000cf0 <_start@@Base>:
 cf0:	f3 0f 1e fa          	endbr64 
 cf4:	31 ed                	xor    %ebp,%ebp
 cf6:	49 89 d1             	mov    %rdx,%r9
 cf9:	5e                   	pop    %rsi
 cfa:	48 89 e2             	mov    %rsp,%rdx
 cfd:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 d01:	50                   	push   %rax
 d02:	54                   	push   %rsp
 d03:	4c 8d 05 46 01 00 00 	lea    0x146(%rip),%r8        # e50 <__libc_csu_fini@@Base>
 d0a:	48 8d 0d cf 00 00 00 	lea    0xcf(%rip),%rcx        # de0 <__libc_csu_init@@Base>
 d11:	48 8d 3d 68 fd ff ff 	lea    -0x298(%rip),%rdi        # a80 <main@@Base>
 d18:	ff 15 ca 12 20 00    	callq  *0x2012ca(%rip)        # 201fe8 <__libc_start_main@GLIBC_2.2.5>
 d1e:	f4                   	hlt    
 d1f:	90                   	nop
 d20:	48 8d 3d e1 12 20 00 	lea    0x2012e1(%rip),%rdi        # 202008 <_edata@@Base+0x4>
 d27:	48 8d 05 da 12 20 00 	lea    0x2012da(%rip),%rax        # 202008 <_edata@@Base+0x4>
 d2e:	48 39 f8             	cmp    %rdi,%rax
 d31:	74 15                	je     d48 <_start@@Base+0x58>
 d33:	48 8b 05 a6 12 20 00 	mov    0x2012a6(%rip),%rax        # 201fe0 <_ITM_deregisterTMCloneTable>
 d3a:	48 85 c0             	test   %rax,%rax
 d3d:	74 09                	je     d48 <_start@@Base+0x58>
 d3f:	ff e0                	jmpq   *%rax
 d41:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 d48:	c3                   	retq   
 d49:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 d50:	48 8d 3d b1 12 20 00 	lea    0x2012b1(%rip),%rdi        # 202008 <_edata@@Base+0x4>
 d57:	48 8d 35 aa 12 20 00 	lea    0x2012aa(%rip),%rsi        # 202008 <_edata@@Base+0x4>
 d5e:	48 29 fe             	sub    %rdi,%rsi
 d61:	48 c1 fe 03          	sar    $0x3,%rsi
 d65:	48 89 f0             	mov    %rsi,%rax
 d68:	48 c1 e8 3f          	shr    $0x3f,%rax
 d6c:	48 01 c6             	add    %rax,%rsi
 d6f:	48 d1 fe             	sar    %rsi
 d72:	74 14                	je     d88 <_start@@Base+0x98>
 d74:	48 8b 05 7d 12 20 00 	mov    0x20127d(%rip),%rax        # 201ff8 <_ITM_registerTMCloneTable>
 d7b:	48 85 c0             	test   %rax,%rax
 d7e:	74 08                	je     d88 <_start@@Base+0x98>
 d80:	ff e0                	jmpq   *%rax
 d82:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
 d88:	c3                   	retq   
 d89:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 d90:	f3 0f 1e fa          	endbr64 
 d94:	80 3d 8d 12 20 00 00 	cmpb   $0x0,0x20128d(%rip)        # 202028 <stderr@@GLIBC_2.2.5+0x8>
 d9b:	75 2b                	jne    dc8 <_start@@Base+0xd8>
 d9d:	55                   	push   %rbp
 d9e:	48 83 3d 32 12 20 00 	cmpq   $0x0,0x201232(%rip)        # 201fd8 <__cxa_finalize@GLIBC_2.2.5>
 da5:	00 
 da6:	48 89 e5             	mov    %rsp,%rbp
 da9:	74 0c                	je     db7 <_start@@Base+0xc7>
 dab:	48 8d 3d 5e 0f 20 00 	lea    0x200f5e(%rip),%rdi        # 201d10 <_IO_stdin_used@@Base+0x200ea8>
 db2:	e8 29 fc ff ff       	callq  9e0 <__cxa_finalize@plt>
 db7:	e8 64 ff ff ff       	callq  d20 <_start@@Base+0x30>
 dbc:	c6 05 65 12 20 00 01 	movb   $0x1,0x201265(%rip)        # 202028 <stderr@@GLIBC_2.2.5+0x8>
 dc3:	5d                   	pop    %rbp
 dc4:	c3                   	retq   
 dc5:	0f 1f 00             	nopl   (%rax)
 dc8:	c3                   	retq   
 dc9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 dd0:	f3 0f 1e fa          	endbr64 
 dd4:	e9 77 ff ff ff       	jmpq   d50 <_start@@Base+0x60>
 dd9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000000de0 <__libc_csu_init@@Base>:
 de0:	f3 0f 1e fa          	endbr64 
 de4:	41 57                	push   %r15
 de6:	49 89 d7             	mov    %rdx,%r15
 de9:	41 56                	push   %r14
 deb:	49 89 f6             	mov    %rsi,%r14
 dee:	41 55                	push   %r13
 df0:	41 89 fd             	mov    %edi,%r13d
 df3:	41 54                	push   %r12
 df5:	4c 8d 25 04 0f 20 00 	lea    0x200f04(%rip),%r12        # 201d00 <_IO_stdin_used@@Base+0x200e98>
 dfc:	55                   	push   %rbp
 dfd:	48 8d 2d 04 0f 20 00 	lea    0x200f04(%rip),%rbp        # 201d08 <_IO_stdin_used@@Base+0x200ea0>
 e04:	53                   	push   %rbx
 e05:	4c 29 e5             	sub    %r12,%rbp
 e08:	48 83 ec 08          	sub    $0x8,%rsp
 e0c:	e8 87 fb ff ff       	callq  998 <PyMem_RawMalloc@plt-0x38>
 e11:	48 c1 fd 03          	sar    $0x3,%rbp
 e15:	74 1f                	je     e36 <__libc_csu_init@@Base+0x56>
 e17:	31 db                	xor    %ebx,%ebx
 e19:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 e20:	4c 89 fa             	mov    %r15,%rdx
 e23:	4c 89 f6             	mov    %r14,%rsi
 e26:	44 89 ef             	mov    %r13d,%edi
 e29:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 e2d:	48 83 c3 01          	add    $0x1,%rbx
 e31:	48 39 dd             	cmp    %rbx,%rbp
 e34:	75 ea                	jne    e20 <__libc_csu_init@@Base+0x40>
 e36:	48 83 c4 08          	add    $0x8,%rsp
 e3a:	5b                   	pop    %rbx
 e3b:	5d                   	pop    %rbp
 e3c:	41 5c                	pop    %r12
 e3e:	41 5d                	pop    %r13
 e40:	41 5e                	pop    %r14
 e42:	41 5f                	pop    %r15
 e44:	c3                   	retq   
 e45:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
 e4c:	00 00 00 00 

0000000000000e50 <__libc_csu_fini@@Base>:
 e50:	f3 0f 1e fa          	endbr64 
 e54:	c3                   	retq   

Disassembly of section .fini:

0000000000000e58 <.fini>:
 e58:	f3 0f 1e fa          	endbr64 
 e5c:	48 83 ec 08          	sub    $0x8,%rsp
 e60:	48 83 c4 08          	add    $0x8,%rsp
 e64:	c3                   	retq   
