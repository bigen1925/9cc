
tmp:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	callq  *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	retq   

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 92 2f 00 00    	pushq  0x2f92(%rip)        # 3fb8 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 94 2f 00 00    	jmpq   *0x2f94(%rip)        # 3fc0 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001030 <calloc@plt>:
    1030:	ff 25 92 2f 00 00    	jmpq   *0x2f92(%rip)        # 3fc8 <calloc@GLIBC_2.2.5>
    1036:	68 00 00 00 00       	pushq  $0x0
    103b:	e9 e0 ff ff ff       	jmpq   1020 <.plt>

0000000000001040 <fprintf@plt>:
    1040:	ff 25 8a 2f 00 00    	jmpq   *0x2f8a(%rip)        # 3fd0 <fprintf@GLIBC_2.2.5>
    1046:	68 01 00 00 00       	pushq  $0x1
    104b:	e9 d0 ff ff ff       	jmpq   1020 <.plt>

Disassembly of section .plt.got:

0000000000001050 <__cxa_finalize@plt>:
    1050:	ff 25 a2 2f 00 00    	jmpq   *0x2fa2(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1056:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000001060 <_start>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	31 ed                	xor    %ebp,%ebp
    1066:	49 89 d1             	mov    %rdx,%r9
    1069:	5e                   	pop    %rsi
    106a:	48 89 e2             	mov    %rsp,%rdx
    106d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1071:	50                   	push   %rax
    1072:	54                   	push   %rsp
    1073:	4c 8d 05 66 06 00 00 	lea    0x666(%rip),%r8        # 16e0 <__libc_csu_fini>
    107a:	48 8d 0d ef 05 00 00 	lea    0x5ef(%rip),%rcx        # 1670 <__libc_csu_init>
    1081:	48 8d 3d c1 00 00 00 	lea    0xc1(%rip),%rdi        # 1149 <main>
    1088:	ff 15 52 2f 00 00    	callq  *0x2f52(%rip)        # 3fe0 <__libc_start_main@GLIBC_2.2.5>
    108e:	f4                   	hlt    
    108f:	90                   	nop

0000000000001090 <deregister_tm_clones>:
    1090:	48 8d 3d 79 2f 00 00 	lea    0x2f79(%rip),%rdi        # 4010 <__TMC_END__>
    1097:	48 8d 05 72 2f 00 00 	lea    0x2f72(%rip),%rax        # 4010 <__TMC_END__>
    109e:	48 39 f8             	cmp    %rdi,%rax
    10a1:	74 15                	je     10b8 <deregister_tm_clones+0x28>
    10a3:	48 8b 05 2e 2f 00 00 	mov    0x2f2e(%rip),%rax        # 3fd8 <_ITM_deregisterTMCloneTable>
    10aa:	48 85 c0             	test   %rax,%rax
    10ad:	74 09                	je     10b8 <deregister_tm_clones+0x28>
    10af:	ff e0                	jmpq   *%rax
    10b1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10b8:	c3                   	retq   
    10b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010c0 <register_tm_clones>:
    10c0:	48 8d 3d 49 2f 00 00 	lea    0x2f49(%rip),%rdi        # 4010 <__TMC_END__>
    10c7:	48 8d 35 42 2f 00 00 	lea    0x2f42(%rip),%rsi        # 4010 <__TMC_END__>
    10ce:	48 29 fe             	sub    %rdi,%rsi
    10d1:	48 89 f0             	mov    %rsi,%rax
    10d4:	48 c1 ee 3f          	shr    $0x3f,%rsi
    10d8:	48 c1 f8 03          	sar    $0x3,%rax
    10dc:	48 01 c6             	add    %rax,%rsi
    10df:	48 d1 fe             	sar    %rsi
    10e2:	74 14                	je     10f8 <register_tm_clones+0x38>
    10e4:	48 8b 05 05 2f 00 00 	mov    0x2f05(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable>
    10eb:	48 85 c0             	test   %rax,%rax
    10ee:	74 08                	je     10f8 <register_tm_clones+0x38>
    10f0:	ff e0                	jmpq   *%rax
    10f2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    10f8:	c3                   	retq   
    10f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001100 <__do_global_dtors_aux>:
    1100:	f3 0f 1e fa          	endbr64 
    1104:	80 3d 1d 2f 00 00 00 	cmpb   $0x0,0x2f1d(%rip)        # 4028 <completed.8059>
    110b:	75 2b                	jne    1138 <__do_global_dtors_aux+0x38>
    110d:	55                   	push   %rbp
    110e:	48 83 3d e2 2e 00 00 	cmpq   $0x0,0x2ee2(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1115:	00 
    1116:	48 89 e5             	mov    %rsp,%rbp
    1119:	74 0c                	je     1127 <__do_global_dtors_aux+0x27>
    111b:	48 8b 3d e6 2e 00 00 	mov    0x2ee6(%rip),%rdi        # 4008 <__dso_handle>
    1122:	e8 29 ff ff ff       	callq  1050 <__cxa_finalize@plt>
    1127:	e8 64 ff ff ff       	callq  1090 <deregister_tm_clones>
    112c:	c6 05 f5 2e 00 00 01 	movb   $0x1,0x2ef5(%rip)        # 4028 <completed.8059>
    1133:	5d                   	pop    %rbp
    1134:	c3                   	retq   
    1135:	0f 1f 00             	nopl   (%rax)
    1138:	c3                   	retq   
    1139:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001140 <frame_dummy>:
    1140:	f3 0f 1e fa          	endbr64 
    1144:	e9 77 ff ff ff       	jmpq   10c0 <register_tm_clones>

0000000000001149 <main>:
    1149:	55                   	push   %rbp
    114a:	48 89 e5             	mov    %rsp,%rbp
    114d:	48 81 ec d0 00 00 00 	sub    $0xd0,%rsp
    1154:	6a 00                	pushq  $0x0
    1156:	58                   	pop    %rax
    1157:	48 89 e8             	mov    %rbp,%rax
    115a:	48 83 e8 08          	sub    $0x8,%rax
    115e:	50                   	push   %rax
    115f:	5f                   	pop    %rdi
    1160:	48 c7 c0 09 00 00 00 	mov    $0x9,%rax
    1167:	50                   	push   %rax
    1168:	5e                   	pop    %rsi
    1169:	48 c7 c0 0a 00 00 00 	mov    $0xa,%rax
    1170:	50                   	push   %rax
    1171:	5a                   	pop    %rdx
    1172:	48 c7 c0 0b 00 00 00 	mov    $0xb,%rax
    1179:	50                   	push   %rax
    117a:	59                   	pop    %rcx
    117b:	48 c7 c0 0c 00 00 00 	mov    $0xc,%rax
    1182:	50                   	push   %rax
    1183:	41 58                	pop    %r8
    1185:	e8 28 03 00 00       	callq  14b2 <alloc4>
    118a:	50                   	push   %rax
    118b:	58                   	pop    %rax
    118c:	6a 00                	pushq  $0x0
    118e:	58                   	pop    %rax
    118f:	48 89 e8             	mov    %rbp,%rax
    1192:	48 83 e8 10          	sub    $0x10,%rax
    1196:	50                   	push   %rax
    1197:	48 89 e8             	mov    %rbp,%rax
    119a:	48 83 e8 08          	sub    $0x8,%rax
    119e:	50                   	push   %rax
    119f:	58                   	pop    %rax
    11a0:	48 8b 00             	mov    (%rax),%rax
    11a3:	50                   	push   %rax
    11a4:	5f                   	pop    %rdi
    11a5:	58                   	pop    %rax
    11a6:	48 89 38             	mov    %rdi,(%rax)
    11a9:	57                   	push   %rdi
    11aa:	58                   	pop    %rax
    11ab:	48 89 e8             	mov    %rbp,%rax
    11ae:	48 83 e8 10          	sub    $0x10,%rax
    11b2:	50                   	push   %rax
    11b3:	58                   	pop    %rax
    11b4:	48 8b 00             	mov    (%rax),%rax
    11b7:	50                   	push   %rax
    11b8:	58                   	pop    %rax
    11b9:	48 8b 00             	mov    (%rax),%rax
    11bc:	50                   	push   %rax
    11bd:	5f                   	pop    %rdi
    11be:	e8 42 04 00 00       	callq  1605 <print_int>
    11c3:	50                   	push   %rax
    11c4:	58                   	pop    %rax
    11c5:	48 89 e8             	mov    %rbp,%rax
    11c8:	48 83 e8 10          	sub    $0x10,%rax
    11cc:	50                   	push   %rax
    11cd:	58                   	pop    %rax
    11ce:	48 8b 00             	mov    (%rax),%rax
    11d1:	50                   	push   %rax
    11d2:	58                   	pop    %rax
    11d3:	48 8b 00             	mov    (%rax),%rax
    11d6:	50                   	push   %rax
    11d7:	48 c7 c0 09 00 00 00 	mov    $0x9,%rax
    11de:	50                   	push   %rax
    11df:	5f                   	pop    %rdi
    11e0:	58                   	pop    %rax
    11e1:	48 39 f8             	cmp    %rdi,%rax
    11e4:	0f 94 c0             	sete   %al
    11e7:	48 0f b6 c0          	movzbq %al,%rax
    11eb:	50                   	push   %rax
    11ec:	5f                   	pop    %rdi
    11ed:	e8 13 04 00 00       	callq  1605 <print_int>
    11f2:	50                   	push   %rax
    11f3:	58                   	pop    %rax
    11f4:	48 89 e8             	mov    %rbp,%rax
    11f7:	48 83 e8 10          	sub    $0x10,%rax
    11fb:	50                   	push   %rax
    11fc:	48 89 e8             	mov    %rbp,%rax
    11ff:	48 83 e8 08          	sub    $0x8,%rax
    1203:	50                   	push   %rax
    1204:	58                   	pop    %rax
    1205:	48 8b 00             	mov    (%rax),%rax
    1208:	50                   	push   %rax
    1209:	48 c7 c0 01 00 00 00 	mov    $0x1,%rax
    1210:	50                   	push   %rax
    1211:	5f                   	pop    %rdi
    1212:	58                   	pop    %rax
    1213:	48 6b ff 04          	imul   $0x4,%rdi,%rdi
    1217:	48 01 f8             	add    %rdi,%rax
    121a:	50                   	push   %rax
    121b:	5f                   	pop    %rdi
    121c:	58                   	pop    %rax
    121d:	48 89 38             	mov    %rdi,(%rax)
    1220:	57                   	push   %rdi
    1221:	58                   	pop    %rax
    1222:	48 89 e8             	mov    %rbp,%rax
    1225:	48 83 e8 10          	sub    $0x10,%rax
    1229:	50                   	push   %rax
    122a:	58                   	pop    %rax
    122b:	48 8b 00             	mov    (%rax),%rax
    122e:	50                   	push   %rax
    122f:	58                   	pop    %rax
    1230:	48 8b 00             	mov    (%rax),%rax
    1233:	50                   	push   %rax
    1234:	5f                   	pop    %rdi
    1235:	e8 cb 03 00 00       	callq  1605 <print_int>
    123a:	50                   	push   %rax
    123b:	58                   	pop    %rax
    123c:	48 89 e8             	mov    %rbp,%rax
    123f:	48 83 e8 10          	sub    $0x10,%rax
    1243:	50                   	push   %rax
    1244:	58                   	pop    %rax
    1245:	48 8b 00             	mov    (%rax),%rax
    1248:	50                   	push   %rax
    1249:	58                   	pop    %rax
    124a:	48 8b 00             	mov    (%rax),%rax
    124d:	50                   	push   %rax
    124e:	48 c7 c0 0a 00 00 00 	mov    $0xa,%rax
    1255:	50                   	push   %rax
    1256:	5f                   	pop    %rdi
    1257:	58                   	pop    %rax
    1258:	48 39 f8             	cmp    %rdi,%rax
    125b:	0f 94 c0             	sete   %al
    125e:	48 0f b6 c0          	movzbq %al,%rax
    1262:	50                   	push   %rax
    1263:	5f                   	pop    %rdi
    1264:	e8 9c 03 00 00       	callq  1605 <print_int>
    1269:	50                   	push   %rax
    126a:	58                   	pop    %rax
    126b:	48 89 e8             	mov    %rbp,%rax
    126e:	48 83 e8 10          	sub    $0x10,%rax
    1272:	50                   	push   %rax
    1273:	48 89 e8             	mov    %rbp,%rax
    1276:	48 83 e8 08          	sub    $0x8,%rax
    127a:	50                   	push   %rax
    127b:	58                   	pop    %rax
    127c:	48 8b 00             	mov    (%rax),%rax
    127f:	50                   	push   %rax
    1280:	48 c7 c0 02 00 00 00 	mov    $0x2,%rax
    1287:	50                   	push   %rax
    1288:	5f                   	pop    %rdi
    1289:	58                   	pop    %rax
    128a:	48 6b ff 04          	imul   $0x4,%rdi,%rdi
    128e:	48 01 f8             	add    %rdi,%rax
    1291:	50                   	push   %rax
    1292:	5f                   	pop    %rdi
    1293:	58                   	pop    %rax
    1294:	48 89 38             	mov    %rdi,(%rax)
    1297:	57                   	push   %rdi
    1298:	58                   	pop    %rax
    1299:	48 89 e8             	mov    %rbp,%rax
    129c:	48 83 e8 10          	sub    $0x10,%rax
    12a0:	50                   	push   %rax
    12a1:	58                   	pop    %rax
    12a2:	48 8b 00             	mov    (%rax),%rax
    12a5:	50                   	push   %rax
    12a6:	58                   	pop    %rax
    12a7:	48 8b 00             	mov    (%rax),%rax
    12aa:	50                   	push   %rax
    12ab:	5f                   	pop    %rdi
    12ac:	e8 54 03 00 00       	callq  1605 <print_int>
    12b1:	50                   	push   %rax
    12b2:	58                   	pop    %rax
    12b3:	48 89 e8             	mov    %rbp,%rax
    12b6:	48 83 e8 10          	sub    $0x10,%rax
    12ba:	50                   	push   %rax
    12bb:	58                   	pop    %rax
    12bc:	48 8b 00             	mov    (%rax),%rax
    12bf:	50                   	push   %rax
    12c0:	58                   	pop    %rax
    12c1:	48 8b 00             	mov    (%rax),%rax
    12c4:	50                   	push   %rax
    12c5:	48 c7 c0 0b 00 00 00 	mov    $0xb,%rax
    12cc:	50                   	push   %rax
    12cd:	5f                   	pop    %rdi
    12ce:	58                   	pop    %rax
    12cf:	48 39 f8             	cmp    %rdi,%rax
    12d2:	0f 94 c0             	sete   %al
    12d5:	48 0f b6 c0          	movzbq %al,%rax
    12d9:	50                   	push   %rax
    12da:	5f                   	pop    %rdi
    12db:	e8 25 03 00 00       	callq  1605 <print_int>
    12e0:	50                   	push   %rax
    12e1:	58                   	pop    %rax
    12e2:	48 89 e8             	mov    %rbp,%rax
    12e5:	48 83 e8 10          	sub    $0x10,%rax
    12e9:	50                   	push   %rax
    12ea:	48 89 e8             	mov    %rbp,%rax
    12ed:	48 83 e8 08          	sub    $0x8,%rax
    12f1:	50                   	push   %rax
    12f2:	58                   	pop    %rax
    12f3:	48 8b 00             	mov    (%rax),%rax
    12f6:	50                   	push   %rax
    12f7:	48 c7 c0 03 00 00 00 	mov    $0x3,%rax
    12fe:	50                   	push   %rax
    12ff:	5f                   	pop    %rdi
    1300:	58                   	pop    %rax
    1301:	48 6b ff 04          	imul   $0x4,%rdi,%rdi
    1305:	48 01 f8             	add    %rdi,%rax
    1308:	50                   	push   %rax
    1309:	5f                   	pop    %rdi
    130a:	58                   	pop    %rax
    130b:	48 89 38             	mov    %rdi,(%rax)
    130e:	57                   	push   %rdi
    130f:	58                   	pop    %rax
    1310:	48 89 e8             	mov    %rbp,%rax
    1313:	48 83 e8 10          	sub    $0x10,%rax
    1317:	50                   	push   %rax
    1318:	58                   	pop    %rax
    1319:	48 8b 00             	mov    (%rax),%rax
    131c:	50                   	push   %rax
    131d:	58                   	pop    %rax
    131e:	48 8b 00             	mov    (%rax),%rax
    1321:	50                   	push   %rax
    1322:	5f                   	pop    %rdi
    1323:	e8 dd 02 00 00       	callq  1605 <print_int>
    1328:	50                   	push   %rax
    1329:	58                   	pop    %rax
    132a:	48 89 e8             	mov    %rbp,%rax
    132d:	48 83 e8 10          	sub    $0x10,%rax
    1331:	50                   	push   %rax
    1332:	58                   	pop    %rax
    1333:	48 8b 00             	mov    (%rax),%rax
    1336:	50                   	push   %rax
    1337:	58                   	pop    %rax
    1338:	48 8b 00             	mov    (%rax),%rax
    133b:	50                   	push   %rax
    133c:	48 c7 c0 0c 00 00 00 	mov    $0xc,%rax
    1343:	50                   	push   %rax
    1344:	5f                   	pop    %rdi
    1345:	58                   	pop    %rax
    1346:	48 39 f8             	cmp    %rdi,%rax
    1349:	0f 94 c0             	sete   %al
    134c:	48 0f b6 c0          	movzbq %al,%rax
    1350:	50                   	push   %rax
    1351:	5f                   	pop    %rdi
    1352:	e8 ae 02 00 00       	callq  1605 <print_int>
    1357:	50                   	push   %rax
    1358:	58                   	pop    %rax
    1359:	48 89 e8             	mov    %rbp,%rax
    135c:	48 83 e8 08          	sub    $0x8,%rax
    1360:	50                   	push   %rax
    1361:	58                   	pop    %rax
    1362:	48 8b 00             	mov    (%rax),%rax
    1365:	50                   	push   %rax
    1366:	5f                   	pop    %rdi
    1367:	48 89 e8             	mov    %rbp,%rax
    136a:	48 83 e8 10          	sub    $0x10,%rax
    136e:	50                   	push   %rax
    136f:	58                   	pop    %rax
    1370:	48 8b 00             	mov    (%rax),%rax
    1373:	50                   	push   %rax
    1374:	5e                   	pop    %rsi
    1375:	e8 ad 01 00 00       	callq  1527 <temp_debug>
    137a:	50                   	push   %rax
    137b:	58                   	pop    %rax
    137c:	48 89 e8             	mov    %rbp,%rax
    137f:	48 83 e8 10          	sub    $0x10,%rax
    1383:	50                   	push   %rax
    1384:	58                   	pop    %rax
    1385:	48 8b 00             	mov    (%rax),%rax
    1388:	50                   	push   %rax
    1389:	58                   	pop    %rax
    138a:	48 8b 00             	mov    (%rax),%rax
    138d:	50                   	push   %rax
    138e:	48 c7 c0 04 00 00 00 	mov    $0x4,%rax
    1395:	50                   	push   %rax
    1396:	5f                   	pop    %rdi
    1397:	58                   	pop    %rax
    1398:	48 39 f8             	cmp    %rdi,%rax
    139b:	0f 94 c0             	sete   %al
    139e:	48 0f b6 c0          	movzbq %al,%rax
    13a2:	50                   	push   %rax
    13a3:	58                   	pop    %rax
    13a4:	48 83 f8 00          	cmp    $0x0,%rax
    13a8:	74 2c                	je     13d6 <main+0x28d>
    13aa:	48 89 e8             	mov    %rbp,%rax
    13ad:	48 83 e8 10          	sub    $0x10,%rax
    13b1:	50                   	push   %rax
    13b2:	58                   	pop    %rax
    13b3:	48 8b 00             	mov    (%rax),%rax
    13b6:	50                   	push   %rax
    13b7:	58                   	pop    %rax
    13b8:	48 8b 00             	mov    (%rax),%rax
    13bb:	50                   	push   %rax
    13bc:	5f                   	pop    %rdi
    13bd:	e8 43 02 00 00       	callq  1605 <print_int>
    13c2:	50                   	push   %rax
    13c3:	58                   	pop    %rax
    13c4:	48 c7 c0 01 00 00 00 	mov    $0x1,%rax
    13cb:	50                   	push   %rax
    13cc:	58                   	pop    %rax
    13cd:	48 89 ec             	mov    %rbp,%rsp
    13d0:	5d                   	pop    %rbp
    13d1:	c3                   	retq   
    13d2:	58                   	pop    %rax
    13d3:	50                   	push   %rax
    13d4:	eb 00                	jmp    13d6 <main+0x28d>
    13d6:	6a 00                	pushq  $0x0
    13d8:	58                   	pop    %rax
    13d9:	48 89 e8             	mov    %rbp,%rax
    13dc:	48 83 e8 10          	sub    $0x10,%rax
    13e0:	50                   	push   %rax
    13e1:	48 89 e8             	mov    %rbp,%rax
    13e4:	48 83 e8 08          	sub    $0x8,%rax
    13e8:	50                   	push   %rax
    13e9:	58                   	pop    %rax
    13ea:	48 8b 00             	mov    (%rax),%rax
    13ed:	50                   	push   %rax
    13ee:	48 c7 c0 03 00 00 00 	mov    $0x3,%rax
    13f5:	50                   	push   %rax
    13f6:	5f                   	pop    %rdi
    13f7:	58                   	pop    %rax
    13f8:	48 6b ff 04          	imul   $0x4,%rdi,%rdi
    13fc:	48 01 f8             	add    %rdi,%rax
    13ff:	50                   	push   %rax
    1400:	5f                   	pop    %rdi
    1401:	58                   	pop    %rax
    1402:	48 89 38             	mov    %rdi,(%rax)
    1405:	57                   	push   %rdi
    1406:	58                   	pop    %rax
    1407:	48 89 e8             	mov    %rbp,%rax
    140a:	48 83 e8 08          	sub    $0x8,%rax
    140e:	50                   	push   %rax
    140f:	58                   	pop    %rax
    1410:	48 8b 00             	mov    (%rax),%rax
    1413:	50                   	push   %rax
    1414:	5f                   	pop    %rdi
    1415:	48 89 e8             	mov    %rbp,%rax
    1418:	48 83 e8 10          	sub    $0x10,%rax
    141c:	50                   	push   %rax
    141d:	58                   	pop    %rax
    141e:	48 8b 00             	mov    (%rax),%rax
    1421:	50                   	push   %rax
    1422:	5e                   	pop    %rsi
    1423:	e8 ff 00 00 00       	callq  1527 <temp_debug>
    1428:	50                   	push   %rax
    1429:	58                   	pop    %rax
    142a:	48 89 e8             	mov    %rbp,%rax
    142d:	48 83 e8 10          	sub    $0x10,%rax
    1431:	50                   	push   %rax
    1432:	58                   	pop    %rax
    1433:	48 8b 00             	mov    (%rax),%rax
    1436:	50                   	push   %rax
    1437:	58                   	pop    %rax
    1438:	48 8b 00             	mov    (%rax),%rax
    143b:	50                   	push   %rax
    143c:	48 c7 c0 08 00 00 00 	mov    $0x8,%rax
    1443:	50                   	push   %rax
    1444:	5f                   	pop    %rdi
    1445:	58                   	pop    %rax
    1446:	48 39 f8             	cmp    %rdi,%rax
    1449:	0f 94 c0             	sete   %al
    144c:	48 0f b6 c0          	movzbq %al,%rax
    1450:	50                   	push   %rax
    1451:	5f                   	pop    %rdi
    1452:	e8 ae 01 00 00       	callq  1605 <print_int>
    1457:	50                   	push   %rax
    1458:	58                   	pop    %rax
    1459:	48 89 e8             	mov    %rbp,%rax
    145c:	48 83 e8 10          	sub    $0x10,%rax
    1460:	50                   	push   %rax
    1461:	58                   	pop    %rax
    1462:	48 8b 00             	mov    (%rax),%rax
    1465:	50                   	push   %rax
    1466:	58                   	pop    %rax
    1467:	48 8b 00             	mov    (%rax),%rax
    146a:	50                   	push   %rax
    146b:	48 c7 c0 08 00 00 00 	mov    $0x8,%rax
    1472:	50                   	push   %rax
    1473:	5f                   	pop    %rdi
    1474:	58                   	pop    %rax
    1475:	48 39 f8             	cmp    %rdi,%rax
    1478:	0f 94 c0             	sete   %al
    147b:	48 0f b6 c0          	movzbq %al,%rax
    147f:	50                   	push   %rax
    1480:	58                   	pop    %rax
    1481:	48 83 f8 00          	cmp    $0x0,%rax
    1485:	74 12                	je     1499 <main+0x350>
    1487:	48 c7 c0 02 00 00 00 	mov    $0x2,%rax
    148e:	50                   	push   %rax
    148f:	58                   	pop    %rax
    1490:	48 89 ec             	mov    %rbp,%rsp
    1493:	5d                   	pop    %rbp
    1494:	c3                   	retq   
    1495:	58                   	pop    %rax
    1496:	50                   	push   %rax
    1497:	eb 00                	jmp    1499 <main+0x350>
    1499:	6a 00                	pushq  $0x0
    149b:	58                   	pop    %rax
    149c:	48 c7 c0 00 00 00 00 	mov    $0x0,%rax
    14a3:	50                   	push   %rax
    14a4:	58                   	pop    %rax
    14a5:	48 89 ec             	mov    %rbp,%rsp
    14a8:	5d                   	pop    %rbp
    14a9:	c3                   	retq   
    14aa:	58                   	pop    %rax
    14ab:	50                   	push   %rax
    14ac:	58                   	pop    %rax
    14ad:	48 89 ec             	mov    %rbp,%rsp
    14b0:	5d                   	pop    %rbp
    14b1:	c3                   	retq   

00000000000014b2 <alloc4>:
#include <stdio.h>
#include <stdlib.h>

void alloc4(int **p, int a, int b, int c, int d) {
    14b2:	f3 0f 1e fa          	endbr64 
    14b6:	55                   	push   %rbp
    14b7:	48 89 e5             	mov    %rsp,%rbp
    14ba:	48 83 ec 20          	sub    $0x20,%rsp
    14be:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    14c2:	89 75 f4             	mov    %esi,-0xc(%rbp)
    14c5:	89 55 f0             	mov    %edx,-0x10(%rbp)
    14c8:	89 4d ec             	mov    %ecx,-0x14(%rbp)
    14cb:	44 89 45 e8          	mov    %r8d,-0x18(%rbp)
  *p = (int *)calloc(4, 4);
    14cf:	be 04 00 00 00       	mov    $0x4,%esi
    14d4:	bf 04 00 00 00       	mov    $0x4,%edi
    14d9:	e8 52 fb ff ff       	callq  1030 <calloc@plt>
    14de:	48 89 c2             	mov    %rax,%rdx
    14e1:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    14e5:	48 89 10             	mov    %rdx,(%rax)
  (*p)[0] = a;
    14e8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    14ec:	48 8b 00             	mov    (%rax),%rax
    14ef:	8b 55 f4             	mov    -0xc(%rbp),%edx
    14f2:	89 10                	mov    %edx,(%rax)
  (*p)[1] = b;
    14f4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    14f8:	48 8b 00             	mov    (%rax),%rax
    14fb:	48 8d 50 04          	lea    0x4(%rax),%rdx
    14ff:	8b 45 f0             	mov    -0x10(%rbp),%eax
    1502:	89 02                	mov    %eax,(%rdx)
  (*p)[2] = c;
    1504:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1508:	48 8b 00             	mov    (%rax),%rax
    150b:	48 8d 50 08          	lea    0x8(%rax),%rdx
    150f:	8b 45 ec             	mov    -0x14(%rbp),%eax
    1512:	89 02                	mov    %eax,(%rdx)
  (*p)[3] = d;
    1514:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1518:	48 8b 00             	mov    (%rax),%rax
    151b:	48 8d 50 0c          	lea    0xc(%rax),%rdx
    151f:	8b 45 e8             	mov    -0x18(%rbp),%eax
    1522:	89 02                	mov    %eax,(%rdx)
}
    1524:	90                   	nop
    1525:	c9                   	leaveq 
    1526:	c3                   	retq   

0000000000001527 <temp_debug>:

void temp_debug(int *p, int *q) {
    1527:	f3 0f 1e fa          	endbr64 
    152b:	55                   	push   %rbp
    152c:	48 89 e5             	mov    %rsp,%rbp
    152f:	48 83 ec 10          	sub    $0x10,%rsp
    1533:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    1537:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  fprintf(stderr, "p: %p -> %d, p[1]: %p -> %d\n", p, *p, p + 1, *(p + 1));
    153b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    153f:	48 83 c0 04          	add    $0x4,%rax
    1543:	8b 30                	mov    (%rax),%esi
    1545:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1549:	48 8d 78 04          	lea    0x4(%rax),%rdi
    154d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1551:	8b 08                	mov    (%rax),%ecx
    1553:	48 8b 05 c6 2a 00 00 	mov    0x2ac6(%rip),%rax        # 4020 <stderr@@GLIBC_2.2.5>
    155a:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    155e:	41 89 f1             	mov    %esi,%r9d
    1561:	49 89 f8             	mov    %rdi,%r8
    1564:	48 8d 35 9d 0a 00 00 	lea    0xa9d(%rip),%rsi        # 2008 <_IO_stdin_used+0x8>
    156b:	48 89 c7             	mov    %rax,%rdi
    156e:	b8 00 00 00 00       	mov    $0x0,%eax
    1573:	e8 c8 fa ff ff       	callq  1040 <fprintf@plt>
  fprintf(stderr, "p[2]: %p -> %d, p[3]: %p -> %d\n", p + 2, *(p + 2), p + 3,
          *(p + 3));
    1578:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    157c:	48 83 c0 0c          	add    $0xc,%rax
  fprintf(stderr, "p[2]: %p -> %d, p[3]: %p -> %d\n", p + 2, *(p + 2), p + 3,
    1580:	8b 08                	mov    (%rax),%ecx
    1582:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1586:	48 8d 78 0c          	lea    0xc(%rax),%rdi
    158a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    158e:	48 83 c0 08          	add    $0x8,%rax
    1592:	8b 10                	mov    (%rax),%edx
    1594:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1598:	48 8d 70 08          	lea    0x8(%rax),%rsi
    159c:	48 8b 05 7d 2a 00 00 	mov    0x2a7d(%rip),%rax        # 4020 <stderr@@GLIBC_2.2.5>
    15a3:	41 89 c9             	mov    %ecx,%r9d
    15a6:	49 89 f8             	mov    %rdi,%r8
    15a9:	89 d1                	mov    %edx,%ecx
    15ab:	48 89 f2             	mov    %rsi,%rdx
    15ae:	48 8d 35 73 0a 00 00 	lea    0xa73(%rip),%rsi        # 2028 <_IO_stdin_used+0x28>
    15b5:	48 89 c7             	mov    %rax,%rdi
    15b8:	b8 00 00 00 00       	mov    $0x0,%eax
    15bd:	e8 7e fa ff ff       	callq  1040 <fprintf@plt>

  fprintf(stderr, "q: %p ", q);
    15c2:	48 8b 05 57 2a 00 00 	mov    0x2a57(%rip),%rax        # 4020 <stderr@@GLIBC_2.2.5>
    15c9:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    15cd:	48 8d 35 74 0a 00 00 	lea    0xa74(%rip),%rsi        # 2048 <_IO_stdin_used+0x48>
    15d4:	48 89 c7             	mov    %rax,%rdi
    15d7:	b8 00 00 00 00       	mov    $0x0,%eax
    15dc:	e8 5f fa ff ff       	callq  1040 <fprintf@plt>
  fprintf(stderr, "-> %d\n", *q);
    15e1:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    15e5:	8b 10                	mov    (%rax),%edx
    15e7:	48 8b 05 32 2a 00 00 	mov    0x2a32(%rip),%rax        # 4020 <stderr@@GLIBC_2.2.5>
    15ee:	48 8d 35 5a 0a 00 00 	lea    0xa5a(%rip),%rsi        # 204f <_IO_stdin_used+0x4f>
    15f5:	48 89 c7             	mov    %rax,%rdi
    15f8:	b8 00 00 00 00       	mov    $0x0,%eax
    15fd:	e8 3e fa ff ff       	callq  1040 <fprintf@plt>
}
    1602:	90                   	nop
    1603:	c9                   	leaveq 
    1604:	c3                   	retq   

0000000000001605 <print_int>:

void print_int(int n) { fprintf(stderr, "print_int:: n -> %d\n", n); }
    1605:	f3 0f 1e fa          	endbr64 
    1609:	55                   	push   %rbp
    160a:	48 89 e5             	mov    %rsp,%rbp
    160d:	48 83 ec 10          	sub    $0x10,%rsp
    1611:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1614:	48 8b 05 05 2a 00 00 	mov    0x2a05(%rip),%rax        # 4020 <stderr@@GLIBC_2.2.5>
    161b:	8b 55 fc             	mov    -0x4(%rbp),%edx
    161e:	48 8d 35 31 0a 00 00 	lea    0xa31(%rip),%rsi        # 2056 <_IO_stdin_used+0x56>
    1625:	48 89 c7             	mov    %rax,%rdi
    1628:	b8 00 00 00 00       	mov    $0x0,%eax
    162d:	e8 0e fa ff ff       	callq  1040 <fprintf@plt>
    1632:	90                   	nop
    1633:	c9                   	leaveq 
    1634:	c3                   	retq   

0000000000001635 <print_int_2>:
void print_int_2(int n, int m) {
    1635:	f3 0f 1e fa          	endbr64 
    1639:	55                   	push   %rbp
    163a:	48 89 e5             	mov    %rsp,%rbp
    163d:	48 83 ec 10          	sub    $0x10,%rsp
    1641:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1644:	89 75 f8             	mov    %esi,-0x8(%rbp)
  fprintf(stderr, "print_int:: n -> %d, m -> %d\n", n, m);
    1647:	48 8b 05 d2 29 00 00 	mov    0x29d2(%rip),%rax        # 4020 <stderr@@GLIBC_2.2.5>
    164e:	8b 4d f8             	mov    -0x8(%rbp),%ecx
    1651:	8b 55 fc             	mov    -0x4(%rbp),%edx
    1654:	48 8d 35 10 0a 00 00 	lea    0xa10(%rip),%rsi        # 206b <_IO_stdin_used+0x6b>
    165b:	48 89 c7             	mov    %rax,%rdi
    165e:	b8 00 00 00 00       	mov    $0x0,%eax
    1663:	e8 d8 f9 ff ff       	callq  1040 <fprintf@plt>
    1668:	90                   	nop
    1669:	c9                   	leaveq 
    166a:	c3                   	retq   
    166b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001670 <__libc_csu_init>:
    1670:	f3 0f 1e fa          	endbr64 
    1674:	41 57                	push   %r15
    1676:	4c 8d 3d 33 27 00 00 	lea    0x2733(%rip),%r15        # 3db0 <__frame_dummy_init_array_entry>
    167d:	41 56                	push   %r14
    167f:	49 89 d6             	mov    %rdx,%r14
    1682:	41 55                	push   %r13
    1684:	49 89 f5             	mov    %rsi,%r13
    1687:	41 54                	push   %r12
    1689:	41 89 fc             	mov    %edi,%r12d
    168c:	55                   	push   %rbp
    168d:	48 8d 2d 24 27 00 00 	lea    0x2724(%rip),%rbp        # 3db8 <__do_global_dtors_aux_fini_array_entry>
    1694:	53                   	push   %rbx
    1695:	4c 29 fd             	sub    %r15,%rbp
    1698:	48 83 ec 08          	sub    $0x8,%rsp
    169c:	e8 5f f9 ff ff       	callq  1000 <_init>
    16a1:	48 c1 fd 03          	sar    $0x3,%rbp
    16a5:	74 1f                	je     16c6 <__libc_csu_init+0x56>
    16a7:	31 db                	xor    %ebx,%ebx
    16a9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    16b0:	4c 89 f2             	mov    %r14,%rdx
    16b3:	4c 89 ee             	mov    %r13,%rsi
    16b6:	44 89 e7             	mov    %r12d,%edi
    16b9:	41 ff 14 df          	callq  *(%r15,%rbx,8)
    16bd:	48 83 c3 01          	add    $0x1,%rbx
    16c1:	48 39 dd             	cmp    %rbx,%rbp
    16c4:	75 ea                	jne    16b0 <__libc_csu_init+0x40>
    16c6:	48 83 c4 08          	add    $0x8,%rsp
    16ca:	5b                   	pop    %rbx
    16cb:	5d                   	pop    %rbp
    16cc:	41 5c                	pop    %r12
    16ce:	41 5d                	pop    %r13
    16d0:	41 5e                	pop    %r14
    16d2:	41 5f                	pop    %r15
    16d4:	c3                   	retq   
    16d5:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
    16dc:	00 00 00 00 

00000000000016e0 <__libc_csu_fini>:
    16e0:	f3 0f 1e fa          	endbr64 
    16e4:	c3                   	retq   

Disassembly of section .fini:

00000000000016e8 <_fini>:
    16e8:	f3 0f 1e fa          	endbr64 
    16ec:	48 83 ec 08          	sub    $0x8,%rsp
    16f0:	48 83 c4 08          	add    $0x8,%rsp
    16f4:	c3                   	retq   
