
.\test.o:     file format pe-i386


Disassembly of section .text:

00000000 <___tcf_0>:
   0:	83 ec 0c             	sub    $0xc,%esp
   3:	b9 04 00 00 00       	mov    $0x4,%ecx
   8:	e8 00 00 00 00       	call   d <___tcf_0+0xd>
   d:	83 c4 0c             	add    $0xc,%esp
  10:	c3                   	ret    

00000011 <_main>:
  11:	8d 4c 24 04          	lea    0x4(%esp),%ecx
  15:	83 e4 f0             	and    $0xfffffff0,%esp
  18:	ff 71 fc             	pushl  -0x4(%ecx)
  1b:	55                   	push   %ebp
  1c:	89 e5                	mov    %esp,%ebp
  1e:	51                   	push   %ecx
  1f:	83 ec 14             	sub    $0x14,%esp
  22:	e8 00 00 00 00       	call   27 <_main+0x16>
  27:	c7 04 24 18 00 00 00 	movl   $0x18,(%esp)
  2e:	e8 00 00 00 00       	call   33 <_main+0x22>
  33:	c7 00 01 00 00 00    	movl   $0x1,(%eax)
  39:	c7 40 04 02 00 00 00 	movl   $0x2,0x4(%eax)
  40:	c7 40 08 03 00 00 00 	movl   $0x3,0x8(%eax)
  47:	c7 40 0c 04 00 00 00 	movl   $0x4,0xc(%eax)
  4e:	c7 40 10 05 00 00 00 	movl   $0x5,0x10(%eax)
  55:	c7 40 14 06 00 00 00 	movl   $0x6,0x14(%eax)
  5c:	a3 00 00 00 00       	mov    %eax,0x0
  61:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
  68:	b9 00 00 00 00       	mov    $0x0,%ecx
  6d:	e8 00 00 00 00       	call   72 <_main+0x61>
  72:	83 ec 04             	sub    $0x4,%esp
  75:	89 04 24             	mov    %eax,(%esp)
  78:	e8 00 00 00 00       	call   7d <_main+0x6c>
  7d:	b8 00 00 00 00       	mov    $0x0,%eax
  82:	8b 4d fc             	mov    -0x4(%ebp),%ecx
  85:	c9                   	leave  
  86:	8d 61 fc             	lea    -0x4(%ecx),%esp
  89:	c3                   	ret    

0000008a <__GLOBAL__sub_I_dat>:
  8a:	83 ec 1c             	sub    $0x1c,%esp
  8d:	b9 04 00 00 00       	mov    $0x4,%ecx
  92:	e8 00 00 00 00       	call   97 <__GLOBAL__sub_I_dat+0xd>
  97:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
  9e:	e8 00 00 00 00       	call   a3 <__GLOBAL__sub_I_dat+0x19>
  a3:	83 c4 1c             	add    $0x1c,%esp
  a6:	c3                   	ret    
  a7:	90                   	nop
