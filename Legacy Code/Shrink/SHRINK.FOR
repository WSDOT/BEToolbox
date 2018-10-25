C///////////////////////////////////////////////////////////////////////
C// BEToolbox - Bridge Engineering Toolbox
C// Copyright (C) 2000, Washington State Department of Transportation
C//                     Bridge and Structures Office
C//
C// This software was developed as part of the Alternate Route Project
C//
C// This program is free software; you can redistribute it and/or modify
C// it under the terms of the Alternate Route Open Source License as
C// published by the Washington State Department of Transportation,
C// Bridge and Structures Office.
C//
C// This program is distributed in the hope that it will be useful,
C// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
C// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
C// PURPOSE.  See the Alternate Route Open Source License for more details.
C//
C// You should have received a copy of the Alternate Open Source License
C// along with this program; if not, write to the Washington State
C// Department of Transportation, Bridge and Structures Office,
C// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
C// Bridge_Support@wsdot.wa.gov
C///////////////////////////////////////////////////////////////////////

C*** LAT. FORCE TEMP AND SHRINK PROGRAM ***  'SHRINK'  ***  *RFF720913*    00010
C                                                                          00020
C    .A. - GARY CHRISTOPHER -    TSO CONVERSION    - 07/20/73              00030
C                                                                          00040
C    .B. - DICK SIMKINS   -   REVISE OUTPUT FORMAT - 07/27/82              00050
C                                                                       .C.
C    .C. - NEAL CAMPBELL  -  PC CONVERSION   -4/1/86                    .C.
C                                                                          00060
C    .T. - TERRY BEEBE    -  VMS CONVERSION  - JANUARY 1990             .T.
C
C    RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04.26.93
C                                                                          00130
      IMPLICIT REAL*8 (A-H),REAL*8 (P-Z)                                .C.
      CHARACTER*8 TIME, REVDAT                                          .T.00140
      CHARACTER*11 DATE                                                 .T.
      CHARACTER*23 DATIME                                               .T. 
      REAL*8 S19(5),C19(5),C29(5),D19(5),D29(5),Y19(5),C18(6),C28(6),      00150
     1   C38(6),D18(6),D28(6),Y18(6),F18(6),F28(6),F91(6),F92(6),F9A(5),   00160
     2   F9B(5),V91(6),D91(6),F94(6),F95(6),F8A(5),F8B(5),V92(6),          00170
     3   IL1(5),IL2(6),IL3(8),ILO1(1),                                     00180
     4   V1H,V2H,V3H,V4H,V5H,V6H,F1BH,F2BH,F3BH,F4BH,F5BH,F6BH,            00190
     5   FAH,FBH,FCH,FDH,FEH,FFH,FABH,FBAH,FBCH,FCBH,FCDH,FDCH,FDEH,       00200
     6   FEDH,FEFH,FFEH                                                    00210
      CHARACTER*4 IBOT(6),ITOP(6),IRT(6),ILT(6)                         .C.00220
      CHARACTER*4 A,B,C                                                 .C.00220
      INTEGER*4 OUNIT, ERUNIT                                           .C.00230
      CHARACTER*1 CARCNT                                                .A.00250
      EQUIVALENCE (S19(1),S1),(S19(2),S2),(S19(3),S3),(S19(4),S4),         00290
     1   (S19(5),S5),(C19(1),C12),(C19(2),C23),(C19(3),C34),(C19(4),C45)   00300
     2   ,  (C19(5),C56),(C29(1),C21),(C29(2),C32),(C29(3),C43),(C29(4),   00310
     3   C54), (C29(5),C65),(D19(1),D12),(D19(2),D23),(D19(3),D34),        00320
     4(D19(4),D45), (D19(5),D56),(D29(1),D21),(D29(2),D32),(D29(3),D43),.C.00330
     5 (D29(4),D54), (D29(5),D65),(Y19(1),Y1),(Y19(2),Y2),(Y19(3),Y3),  .C.00340
     6 (Y19(4),Y4),     (Y19(5),Y5)                                     .C.00350
      EQUIVALENCE (C18(1),C1),(C18(2),C2),(C18(3),C3),(C18(4),C4),         00360
     1   (C18(5),C5),(C18(6),C6),(C28(1),C1T),(C28(2),C2T),(C28(3),C3T),   00370
     2   (C28(4),C4T),(C28(5),C5T),(C28(6),C6T),(C38(1),C1B),(C38(2),C2B   00380
     3   ), (C38(3),C3B),(C38(4),C4B),(C38(5),C5B),(C38(6),C6B),(D18(1),   00390
     4   D1T), (D18(2),D2T),(D18(3),D3T),(D18(4),D4T),(D18(5),D5T),        00400
     5   (D18(6),D6T),(D28(1),D1B),(D28(2),D2B),(D28(3),D3B),(D28(4),   .C.00410
     6   D4B), (D28(5),D5B),(D28(6),D6B),(Y18(1),Y1C),(Y18(2),Y2C),     .C.00420
     7 (Y18(3),Y3C), (Y18(4),Y4C),(Y18(5),Y5C),(Y18(6),Y6C),(F18(1),F1),.C.00430
     8   (F18(2),F2),   (F18(3),F3),(F18(4),F4),(F18(5),F5),(F18(6),F6),   00440
     9  (F28(1),F1T),     (F28(2),F2T),(F28(3),F3T),(F28(4),F4T),(F28(5).C.00450
     A    ,F5T),(F28(6),F6T)                                            .C.00460
      EQUIVALENCE (F91(1),F1BH),(F91(2),F2BH),(F91(3),F3BH),               00470
     1   (F91(4),F4BH),(F91(5),F5BH),(F91(6),F6BH),                        00480
     2 (F92(1),FAH),(F92(2),FBH),(F92(3),FCH),(F92(4),FDH),(F92(5),FEH),.C.00490
     3      (F92(6),FFH),(F9A(1),FABH),(F9A(2),FBCH),(F9A(3),FCDH),     .C.00500
     4      (F9A(4),FDEH),(F9A(5),FEFH),(F9B(1),FBAH),(F9B(2),FCBH),       00510
     5      (F9B(3),FDCH),(F9B(4),FEDH),(F9B(5),FFEH),(V91(1),V1H),        00520
     6      (V91(2),V2H),(V91(3),V3H),(V91(4),V4H),(V91(5),V5H),(V91(6),   00530
     7V6H), (D91(1),D1),(D91(2),D2),(D91(3),D3),(D91(4),D4),(D91(5),D5),.C.00540
     8            (D91(6),D6)                                           .C.00550
      EQUIVALENCE (F94(1),F1B),(F94(2),F2B),(F94(3),F3B),(F94(4),F4B),     00560
     1   (F94(5),F5B),(F94(6),F6B),(F95(1),FA),(F95(2),FB),(F95(3),FC),    00570
     2   (F95(4),FD),(F95(5),FE),(F95(6),FF),(F8A(1),FAB),(F8A(2),FBC),    00580
     3 (F8A(3),FCD),(F8A(4),FDE),(F8A(5),FEF),(F8B(1),FBA),(F8B(2),FCB),.C.00590
     4   (F8B(3),FDC),(F8B(4),FED),(F8B(5),FFE),(V92(1),V1),(V92(2),V2),.C.00600
     5         (V92(3),V3),(V92(4),V4),(V92(5),V5),(V92(6),V6)          .C.00610
      DATA IBOT(1),IBOT(2),IBOT(3),IBOT(4),IBOT(5),IBOT(6)/'1BOT',         00640
     1   '2BOT','3BOT','4BOT','5BOT','6BOT'/                               00650
      DATA ITOP(1),ITOP(2),ITOP(3),ITOP(4),ITOP(5),ITOP(6)/'1TOP',         00660
     1   '2TOP','3TOP','4TOP','5TOP','6TOP'/                               00670
      DATA IRT(1),IRT(2),IRT(3),IRT(4),IRT(5),IRT(6)/'1-RT','2-RT',        00680
     1   '3-RT','4-RT','5-RT','6-RT'/                                      00690
      DATA ILT(1),ILT(2),ILT(3),ILT(4),ILT(5),ILT(6)/'1-LT','2-LT',        00700
     1   '3-LT','4-LT','5-LT','6-LT'/,A/'A'/,B/'B'/,C/'C'/                 00710
C    ******** REVISION DATE ********                                       00080
C    *                             *                                       00090
      REVDAT='01/30/90'                                                 .T.00100
C    *                             *                                       00110
C    *******************************                                       00120
C                                                                          00750
C                                                                          00760
C                    PARM INFO PASSED TO THE PROGRAM                       00770
C                                                                          00780
C    OUNIT  - THE UNIT UPON WHICH THE OUTPUT IS PRODUCED                   00800
C    ERUNIT - THE UNIT UPON WHICH THE ERROR MESSAGES ARE PRINTED           00810
C    CARCNT - '1' FOR FILE OUTPUT                                       .C.00820
C    IMODE  - 1 FOR FREE FORMAT                                         .C.
C                                                                          00860
      CALL PARM(IMODE,OUNIT,ERUNIT,CARCNT)                              .C.00870
C                                                                          00880
C                                                                          00910
C                                                                          00950
      CALL LIB$DATE_TIME (DATIME)                                       .T.00960
      TIME = DATIME(13:20)                                              .T.
      DATE = DATIME(1:11)                                               .T.
C                                                                          00970
      INUNIT=5                                                          .C.
20    S1=0.                                                                01010
      S2=0.                                                                01020
      S3=0.                                                                01030
      S4=0.                                                                01040
      S5=0.                                                                01050
      C1=0.                                                                01060
      C2=0.                                                                01070
      C3=0.                                                                01080
      C4=0.                                                                01090
      C5=0.                                                                01100
      C6=0.                                                                01110
      P=0.                                                                 01120
      EC=0.                                                                01130
      T=0.                                                                 01140
      ET=0.                                                                01150
      D1=0.                                                                01160
      D2=0.                                                                01170
      D3=0.                                                                01180
      D4=0.                                                                01190
      D5=0.                                                                01200
      D6=0.                                                                01210
      D3T=0.                                                               01220
      D4T=0.                                                               01230
      D5T=0.                                                               01240
      D6T=0.                                                               01250
      DCB=0.                                                               01260
      DDC=0.                                                               01270
      DED=0.                                                               01280
      DFE=0.                                                               01290
      C32=0.                                                               01300
      C43=0.                                                               01310
      C54=0.                                                               01320
      C65=0.                                                               01330
      MPB=1
      J=0                                                               .C.
      LNUM=1                                                            .C.
      READ (INUNIT,END=1190,ERR=1190,FMT=*)LNUM,IDEN,P,EC,T,ET          .C.01650
      IF(LNUM.NE.1)GO TO 1190                                           .C.
      WRITE (OUNIT,830) CARCNT,REVDAT,IDEN,TIME,DATE                    .C.01800
      WRITE (OUNIT,840)                                                 .C.01800
      DO 150 M = 1,7                                                    .C.
      READ(INUNIT,*)LNUM                                                .C.
      BACKSPACE INUNIT                                                  .C.
      IF(LNUM.EQ.1)GO TO 1225                                           .C.
      IF(LNUM.EQ.3.AND.M.EQ.1)GO TO 1225                                .C.
      IF(LNUM.EQ.3)GO TO 170                                            .C.
      IF(M.EQ.7)THEN                                                    .C.
      WRITE(ERUNIT,*)'*** ERROR!!  TOO MANY SPANS, CHECK INPUT!! ***'   .C.
      STOP                                                              .C.
      END IF                                                            .C.
      IF(LNUM.EQ.2) THEN                                                .C.
      READ(INUNIT,END=1190,ERR=1190,FMT=*)LNUM,S19(M),C19(M),C29(M),    .C.
     1     D19(M),D29(M),Y19(M)                                         .C.
      WRITE(OUNIT,850) M,S19(M),C19(M),C29(M),D19(M),D29(M),Y19(M)      .C.
      GO TO 150                                                         .C.
      END IF                                                            .C.
      GO TO 1190                                                        .C.
150   CONTINUE                                                          .C.
170   JO = M - 1                                                        .C.
      JO1 = M                                                           .C.
      MDO = JO + 4                                                      .C.
      MTST = JO + 2                                                     .C.
      WRITE(OUNIT,860)                                                  .C.
      DO 200 M = 1,MDO                                                  .C.
      READ(INUNIT,END=180,ERR=1190,FMT=*)LNUM                           .C.
      BACKSPACE INUNIT                                                  .C.
      IF(LNUM.EQ.1.AND.M.EQ.MTST)GO TO 500                              .C.
      IF(LNUM.EQ.1.AND.M.NE.MTST)GO TO 1225                             .C.
      GO TO 190                                                         .C.
180   MPB=0                                                             .C.
      BACKSPACE INUNIT
      IF(M.EQ.MTST)GO TO 500                                            .C.
      IF(M.LT.MTST) THEN                                                .C.
      WRITE(*,*)'*** ERROR!!  TOO FEW COLUMNS,  CHECK INPUT!! ***'      .T.
      STOP                                                              .C.
      END IF                                                            .C.
190   IF(LNUM.EQ.3.AND.M.LT.MTST)THEN                                   .C.
      READ(INUNIT,END=1190,ERR=1190,FMT=*)LNUM,                         .C.
     1  C18(M),C28(M),C38(M),D18(M),D28(M),Y18(M),F18(M),F28(M)         .C.
      WRITE(OUNIT,870)M,                                                .C.
     1  C18(M),C28(M),C38(M),D18(M),D28(M),Y18(M),F18(M),F28(M)         .C.
      GO TO 200                                                         .C.
      END IF                                                            .C.
      IF(LNUM.EQ.3.AND.M.GE.MTST)THEN                                   .C.
      WRITE(*,*)'*** ERROR!!  TOO MANY COLUMNS,  CHECK INPUT!! ***'     .T.
      STOP                                                              .C.
      END IF                                                            .C.
      GO TO 1190                                                        .C.
200   CONTINUE                                                          .C.
C                                                                          02610
C        COMPUTE MEMBER STIFFNESSES                                        02620
C                                                                          02630
  500 D12=D12*Y1/S1                                                        02640
      D21=D21*Y1/S1                                                        02650
      IF (S2) 550,510,550                                                  02660
  510 D23=0.                                                               02670
      D32=0.                                                               02680
  520 D34=0.                                                               02690
      D43=0.                                                               02700
  530 D45=0.                                                               02710
      D54=0.                                                               02720
  540 D56=0.                                                               02730
      D65=0.                                                               02740
      GO TO 590                                                            02750
  550 D23=D23*Y2/S2                                                        02760
      D32=D32*Y2/S2                                                        02770
      IF (S3) 560,520,560                                                  02780
  560 D34=D34*Y3/S3                                                        02790
      D43=D43*Y3/S3                                                        02800
      IF (S4) 570,530,570                                                  02810
  570 D45=D45*Y4/S4                                                        02820
      D54=D54*Y4/S4                                                        02830
      IF (S5) 580,540,580                                                  02840
  580 D56=D56*Y5/S5                                                        02850
      D65=D65*Y5/S5                                                        02860
  590 C1=C1+.0001                                                          02870
      C2=C2+.0001                                                          02880
      C3=C3+.0001                                                          02890
      C4=C4+.0001                                                          02900
      C5=C5+.0001                                                          02910
      C6=C6+.0001                                                          02920
      FA=0.                                                                02930
      F1B=0.                                                               02940
      FB=0.                                                                02950
      F2B=0.                                                               02960
      FC=0.                                                                02970
      F3B=0.                                                               02980
      FD=0.                                                                02990
      F4B=0.                                                               03000
      FE=0.                                                                03010
      F5B=0.                                                               03020
      FF=0.                                                                03030
      F6B=0.                                                               03040
C                                                                          03050
C     COMPUTE FIXED END MOMENTS AND DIST VALUES                            03060
C                                                                          03070
      IF (P) 610,600,610                                                   03080
  600 EC=174.E3                                                            03090
C     AT 1                                                                 03100
  610 FA1=D1T*EC*Y1C*(1.+C1T)*.1/(C1*C1)                                   03110
      F1B1=D1B*EC*Y1C*(1.+C1B)*.1/(C1*C1)                                  03120
      FA11=(FA1-F1B1*C1B*(1.-F1))*F1T                                      03130
      FA=FA11                                                              03140
      F1B1=(F1B1-FA1*(C1T)*(1.-F1T))*F1                                    03150
      F1B=F1B1                                                             03160
      D1T=(D1T*Y1C/C1)*(1.-(C1T*C1B)*(1.-F1))*F1T                          03170
      C1T=(C1T*F1*F1T)/(1.-(C1T*C1B)*(1.-F1))                              03180
C     AT  2                                                                03190
      FB1=D2T*EC*Y2C*(1.+C2T)*.1/(C2*C2)                                   03200
      F2B1=D2B*EC*Y2C*(1.+C2B)*.1/(C2*C2)                                  03210
      FB11=(FB1-F2B1*C2B*(1.-F2))*F2T                                      03220
      FB=FB11                                                              03230
      F2B1=(F2B1-FB1*(C2T)*(1.-F2T))*F2                                    03240
      F2B=F2B1                                                             03250
      D2T=(D2T*Y2C/C2)*(1.-(C2T*C2B)*(1.-F2))*F2T                          03260
      C2T=(C2T*F2*F2T)/(1.-(C2T*C2B)*(1.-F2))                              03270
      IF (S2) 620,660,620                                                  03280
C     AT  3                                                                03290
  620 FC1=D3T*EC*Y3C*(1.+C3T)*.1/(C3*C3)                                   03300
      F3B1=D3B*EC*Y3C*(1.+C3B)*.1/(C3*C3)                                  03310
      FC11=(FC1-F3B1*C3B*(1.-F3))*F3T                                      03320
      FC=FC11                                                              03330
      F3B1=(F3B1-FC1*(C3T)*(1.-F3T))*F3                                    03340
      F3B=F3B1                                                             03350
      D3T=(D3T*Y3C/C3)*(1.-(C3T*C3B)*(1.-F3))*F3T                          03360
      C3T=(C3T*F3*F3T)/(1.-(C3T*C3B)*(1.-F3))                              03370
      IF (S3) 630,660,630                                                  03380
C     AT  4                                                                03390
  630 FD1=D4T*EC*Y4C*(1.+C4T)*.1/(C4*C4)                                   03400
      F4B1=D4B*EC*Y4C*(1.+C4B)*.1/(C4*C4)                                  03410
      FD11=(FD1-F4B1*C4B*(1.-F4))*F4T                                      03420
      FD=FD11                                                              03430
      F4B1=(F4B1-FD1*(C4T)*(1.-F4T))*F4                                    03440
      F4B=F4B1                                                             03450
      D4T=(D4T*Y4C/C4)*(1.-(C4T*C4B)*(1.-F4))*F4T                          03460
      C4T=(C4T*F4*F4T)/(1.-(C4T*C4B)*(1.-F4))                              03470
      IF (S4) 640,660,640                                                  03480
C     AT  5                                                                03490
  640 FE1=D5T*EC*Y5C*(1.+C5T)*.1/(C5*C5)                                   03500
      F5B1=D5B*EC*Y5C*(1.+C5B)*.1/(C5*C5)                                  03510
      FE11=(FE1-F5B1*C5B*(1.-F5))*F5T                                      03520
      FE=FE11                                                              03530
      F5B1=(F5B1-FE1*(C5T)*(1.-F5T))*F5                                    03540
      F5B=F5B1                                                             03550
      D5T=(D5T*Y5C/C5)*(1.-(C5T*C5B)*(1.-F5))*F5T                          03560
      C5T=(C5T*F5*F5T)/(1.-(C5T*C5B)*(1.-F5))                              03570
      IF (S5) 650,660,650                                                  03580
C     AT 6                                                                 03590
  650 FF1=D6T*EC*Y6C*(1.+C6T)*.1/(C6*C6)                                   03600
      F6B1=D6B*EC*Y6C*(1.+C6B)*.1/(C6*C6)                                  03610
      FF11=(FF1-F6B1*C6B*(1.-F6))*F6T                                      03620
      FF=FF11                                                              03630
      F6B1=(F6B1-FF1*(C6T)*(1.-F6T))*F6                                    03640
      F6B=F6B1                                                             03650
      D6T=(D6T*Y6C/C6)*(1.-(C6T*C6B)*(1.-F6))*F6T                          03660
      C6T=(C6T*F6*F6T)/(1.-(C6T*C6B)*(1.-F6))                              03670
C                                                                          03680
C        COMPUTE JOINT DIST FACTORS                                        03690
C                                                                          03700
  660 DAB=-D12/(D12+D1T)                                                   03710
      DA=-D1T/(D12+D1T)                                                    03720
      DBA=-D21/(D21+D23+D2T)                                               03730
      DBC=-D23/(D21+D23+D2T)                                               03740
      DB=-D2T/(D21+D23+D2T)                                                03750
      IF (S2) 670,710,670                                                  03760
  670 DCB=-D32/(D32+D34+D3T)                                               03770
      DCD=-D34/(D32+D34+D3T)                                               03780
      DC=-D3T/(D32+D34+D3T)                                                03790
      IF (S3) 680,710,680                                                  03800
  680 DDC=-D43/(D43+D45+D4T)                                               03810
      DDE=-D45/(D43+D45+D4T)                                               03820
      DD=-D4T/(D43+D45+D4T)                                                03830
      IF (S4) 690,710,690                                                  03840
  690 DED=-D54/(D54+D56+D5T)                                               03850
      DEF=-D56/(D54+D56+D5T)                                               03860
      DE=-D5T/(D54+D56+D5T)                                                03870
      IF (S5) 700,710,700                                                  03880
  700 DFE=-D65/(D65+D6T)                                                   03890
      DF=-D6T/(D65+D6T)                                                    03900
C                                                                          03910
C        SET VALUES TO ZERO                                                03920
C                                                                          03930
  710 FAB=0.                                                               03940
      FBA=0.                                                               03950
      FBC=0.                                                               03960
      FCB=0.                                                               03970
      FCD=0.                                                               03980
      FDC=0.                                                               03990
      FDE=0.                                                               04000
      FED=0.                                                               04010
      FEF=0.                                                               04020
      FFE=0.                                                               04030
      N=0                                                                  04040
      BB=0.                                                                04050
      BC=0.                                                                04060
      BD=0.                                                                04070
      BE=0.                                                                04080
      BF=0.                                                                04090
C                                                                          04100
C        BALANCE DISTRIBUTION AND CARRYOVER FACTORS AT PIER 1              04110
C                                                                          04120
  720 FAB=FAB+BB*DBA*C21                                                   04130
      BA=FA+FAB                                                            04140
      FA=FA+BA*DA                                                          04150
      FAB=FAB+BA*DAB                                                       04160
      F1B=F1B+BA*DA*C1T                                                    04170
      FBA=FBA+BA*DAB*C12                                                   04180
      N=N+1                                                                04190
C                                                                          04200
C        BALANCE DISTRIBUTION AND CARRYOVER FACTORS AT PIER 2              04210
C                                                                          04220
      FBC=FBC+BC*DCB*C32                                                   04230
      BB=FBA+FB+FBC                                                        04240
      FBA=FBA+BB*DBA                                                       04250
      FB=FB+BB*DB                                                          04260
      F2B=F2B+BB*DB*C2T                                                    04270
      FBC=FBC+BB*DBC                                                       04280
      FCB=FCB+BB*DBC*C23                                                   04290
      IF (S2) 730,770,730                                                  04300
C                                                                          04310
C        BALANCE DISTRIBUTION AND CARRYOVER FACTORS AT PIER 3              04320
C                                                                          04330
  730 FCD=FCD+BD*DDC*C43                                                   04340
      BC=FCB+FCD+FC                                                        04350
      FCB=FCB+BC*DCB                                                       04360
      FC=FC+BC*DC                                                          04370
      F3B=F3B+BC*DC*C3T                                                    04380
      FCD=FCD+BC*DCD                                                       04390
      FDC=FDC+BC*DCD*C34                                                   04400
      IF (S3) 740,770,740                                                  04410
C                                                                          04420
C        BALANCE DISTRIBUTION AND CARRYOVER FACTORS AT PIER 4              04430
C                                                                          04440
  740 FDE=FDE+BE*DED*C54                                                   04450
      BD=FDC+FDE+FD                                                        04460
      FDC=FDC+BD*DDC                                                       04470
      FD=FD+BD*DD                                                          04480
      F4B=F4B+BD*DD*C4T                                                    04490
      FDE=FDE+BD*DDE                                                       04500
      FED=FED+BD*DDE*C45                                                   04510
      IF (S4) 750,770,750                                                  04520
C                                                                          04530
C        BALANCE DISTRIBUTION AND CARRYOVER FACTORS AT PIER 5              04540
C                                                                          04550
  750 FEF=FEF+BF*DFE*C65                                                   04560
      BE=FED+FEF+FE                                                        04570
      FED=FED+BE*DED                                                       04580
      FE=FE+BE*DE                                                          04590
      F5B=F5B+BE*DE*C5T                                                    04600
      FEF=FEF+BE*DEF                                                       04610
      FFE=FFE+BE*DEF*C56                                                   04620
      IF (S5) 760,770,760                                                  04630
C                                                                          04640
C        BALANCE DISTRIBUTION AND CARRYOVER FACTORS AT PIER 6              04650
C                                                                          04660
  760 BF=FFE+FF                                                            04670
      FFE=FFE+BF*DFE                                                       04680
      FF=FF+BF*DF                                                          04690
      F6B=F6B+BF*DF*C6T                                                    04700
  770 IF (N-7) 720,780,720                                                 04710
C                                                                          04720
C        COMPUTE RELATIVE SHEARS AT COLUMN TOPS                            04730
C                                                                          04740
  780 V1=(F1B+FA)/C1                                                       04750
      V2=(F2B+FB)/C2                                                       04760
      V3=(F3B+FC)/C3                                                       04770
      V4=(F4B+FD)/C4                                                       04780
      V5=(F5B+FE)/C5                                                       04790
      V6=(F6B+FF)/C6                                                       04800
      J=J+1                                                                04810
      IF (J-1) 1030,790,1030                                               04820
C                                                                          04830
C        COMPUTE ACTUAL SHEARS AT COLUMN TOPS                              04840
C                                                                          04850
  790 IF (P) 810,800,810                                                   04860
  800 R=100./(V1+V2+V3+V4+V5+V6)                                           04870
      GO TO 820                                                            04880
  810 R=P/(V1+V2+V3+V4+V5+V6)                                              04890
  820 V1H=-(R*V1)                                                          04900
      V2H=-(R*V2)                                                          04910
      V3H=-(R*V3)                                                          04920
      V4H=-(R*V4)                                                          04930
      V5H=-(R*V5)                                                          04940
      V6H=-(R*V6)                                                          04950
C                                                                          04960
C        COMPUTE ACTUAL DEFLECTION AT COLUMN TOPS                          04970
C                                                                          04980
      DEFL=R*.1                                                            04990
C                                                                          05000
C        COMPUTE ACTUAL MOMENTS AT BOTTOM OF COLUMNS                       05010
C                                                                          05020
      F1BH=-(R*F1B)                                                        05030
      F2BH=-(R*F2B)                                                        05040
      F3BH=-(R*F3B)                                                        05050
      F4BH=-(R*F4B)                                                        05060
      F5BH=-(R*F5B)                                                        05070
      F6BH=-(R*F6B)                                                        05080
C                                                                          05090
C        COMPUTE ACTUAL MOMENTS AT TOP OF COLUMNS                          05100
C                                                                          05110
      FAH=R*FA                                                             05120
      FBH=R*FB                                                             05130
      FCH=R*FC                                                             05140
      FDH=R*FD                                                             05150
      FEH=R*FE                                                             05160
      FFH=R*FF                                                             05170
C                                                                          05180
C        COMPUTE ACTUAL MOMENTS AT SPAN ENDS                               05190
C                                                                          05200
      FABH=-(R*FAB)                                                        05210
      FBAH=R*FBA                                                           05220
      FBCH=-(R*FBC)                                                        05230
      FCBH=R*FCB                                                           05240
      FCDH=-(R*FCD)                                                        05250
      FDCH=R*FDC                                                           05260
      FDEH=-(R*FDE)                                                        05270
      FEDH=R*FED                                                           05280
      FEFH=-(R*FEF)                                                        05290
      FFEH=R*FFE                                                           05300
      IF (P) 880,980,880                                                   05310
C                                                                          05320
C     CARCNT - CARRIAGE CONTROL CHARACTER                                  05330
C                                                                          05340
  830 FORMAT(A1,13X,'WASHINGTON DEPARTMENT OF TRANSPORTATION'//         .B.05350
     1          11X,'LATERAL FORCE AND SHRINKAGE + TEMPERATURE DROP'/   .B.05360
     2          27X,'REV. ',A8//' **  IDENT NO. ',I4, 5X,'TIME: ',A8,   .T.05370
     3           5X,'DATE: ',A11,'  **',/)                              .T.05370
  840 FORMAT(/                                                          .B.05380
     1   '         LGTH.    C. O. FACTORS    STIFFNESS        I'/       .B.05390
     2   '         (FT.)   L TO R   R TO L   LT.   RT.      (FT.)'/)    .B.05400
  850 FORMAT(' SPAN',I2,F7.2,F8.3,F9.3,F8.2, F6.2,F11.2)                .B.05410
  860 FORMAT(//                                                         .B.05420
     1 9X,'LGTH.    C. O. FACTORS    STIFFNESS        I       FIXITY'/  .B.05430
     2 9X,'(FT.)   T TO B   B TO T   TOP   BOT      (FT.)   BOT.  TOP'/).B.05440
  870 FORMAT(' COL',I2,F7.2,F9.3,F9.3,F8.2,F6.2,F10.2,F6.2,F6.2)        .B.05450
  880 IEF=IDINT(EC+0.5)                                                 .B.05460
      WRITE (OUNIT,890) P,IEF,DEFL                                      .B.05470
  890 FORMAT(/' **LATERAL FORCE   P =',F8.2,'  E =',I7,                 .B.05480
     1   '  DEFL(FT) =',F11.8,'**')                                     .B.05490
      WRITE (OUNIT,900) (ITOP(LL),LL=1,JO1)                                05500
  900 FORMAT(/' COL. MOMENTS    ',6(4X,A4))                             .B.05510
      WRITE (OUNIT,910) (F92(LL),LL=1,JO1)                                 05520
  910 FORMAT('   (KIP-FT.)     ',6F8.1)                                 .B.05530
      WRITE (OUNIT,920) (IBOT(LL),LL=1,JO1)                                05540
  920 FORMAT(/17X,6(4X,A4)/)                                               05550
      WRITE (OUNIT,930) (F91(LL),LL=1,JO1)                                 05560
  930 FORMAT(17X,6F8.1)                                                 .B.05570
      WRITE (OUNIT,940) (ILT(LL),IRT(LL),LL=1,JO)                          05590
  940 FORMAT(/' SPAN MOMENTS    ',10(4X,A4))                            .B.05600
      WRITE (OUNIT,950) (F9A(LL),F9B(LL),LL=1,JO)                          05610
  950 FORMAT('   (KIP-FT.)     ',10F8.1)                                .B.05620
  970 WRITE (OUNIT,1160) (ITOP(LL),LL=1,JO1)                               05660
      WRITE (OUNIT,910) (V91(LL),LL=1,JO1)                                 05670
      IF (T) 980,1170,980                                                  05680
C                                                                          05690
C        COMPUTE INITIAL TEMP/SHRINK DEFLECTIONS AND FIXED END MOMENTS     05700
C           (ASSUMING PT 1 DOES NOT MOVE)                                  05710
C                                                                          05720
  980 TO=.6E-5*T+.0002                                                     05730
      FA=0.                                                                05740
      F1B=0.                                                               05750
      D1=0.                                                                05760
      D2=-TO*S1                                                            05770
      FB=FB11*(ET/EC)*(D2/.1)                                              05780
      F2B=F2B1*(ET/EC)*(D2/.1)                                             05790
      IF (S2) 990,710,990                                                  05800
  990 D3=-TO*(S1+S2)                                                       05810
      FC=FC11*(ET/EC)*(D3/.1)                                              05820
      F3B=F3B1*(ET/EC)*(D3/.1)                                             05830
      IF (S3) 1000,710,1000                                                05840
 1000 D4=-TO*(S1+S2+S3)                                                    05850
      FD=FD11*(ET/EC)*(D4/.1)                                              05860
      F4B=F4B1*(ET/EC)*(D4/.1)                                             05870
      IF (S4) 1010,710,1010                                                05880
 1010 D5=-TO*(S1+S2+S3+S4)                                                 05890
      FE=FE11*(ET/EC)*(D5/.1)                                              05900
      F5B=F5B1*(ET/EC)*(D5/.1)                                             05910
      IF (S5) 1020,710,1020                                                05920
 1020 D6=-TO*(S1+S2+S3+S4+S5)                                              05930
      FF=FF11*(ET/EC)*(D6/.1)                                              05940
      F6B=F6B1*(ET/EC)*(D6/.1)                                             05950
      GO TO 710                                                            05960
C                                                                          05970
C        COMPUTE TEMP/SHRINK CORRECTION FACTOR                             05980
C                                                                          05990
 1030 Z=(V1+V2+V3+V4+V5+V6)/(V1H+V2H+V3H+V4H+V5H+V6H)                      06000
C                                                                          06010
C        COMPUTE TEMP/SHRINK FINAL DEFLECTIONS INCLUDING SIDESWAY          06020
C                                                                          06030
      DEF=-DEFL*((V1+V2+V3+V4+V5+V6)/P)*(EC/ET)                            06040
      D1=D1+DEF                                                            06050
      D2=D2+DEF                                                            06060
      IF (S2) 1040,1080,1040                                               06070
 1040 D3=D3+DEF                                                            06080
      IF (S3) 1050,1080,1050                                               06090
 1050 D4=D4+DEF                                                            06100
      IF (S4) 1060,1080,1060                                               06110
 1060 D5=D5+DEF                                                            06120
      IF (S5) 1070,1080,1070                                               06130
 1070 D6=D6+DEF                                                            06140
C                                                                          06150
C        COMPUTE TEMP/SHRINK FINAL SHEARS                                  06160
C                                                                          06170
 1080 V1=-V1+Z*V1H                                                         06180
      V2=-V2+Z*V2H                                                         06190
      V3=-V3+Z*V3H                                                         06200
      V4=-V4+Z*V4H                                                         06210
      V5=-V5+Z*V5H                                                         06220
      V6=-V6+Z*V6H                                                         06230
C                                                                          06240
C        COMPUTE TEMP/SHRINK FINAL MOMENTS AT BOTTOM OF COLUMNS            06250
C                                                                          06260
      F1B=-F1B+Z*F1BH                                                      06270
      F2B=-F2B+Z*F2BH                                                      06280
      F3B=-F3B+Z*F3BH                                                      06290
      F4B=-F4B+Z*F4BH                                                      06300
      F5B=-F5B+Z*F5BH                                                      06310
      F6B=-F6B+Z*F6BH                                                      06320
C                                                                          06330
C        COMPUTE TEMP/SHRINK FINAL MOMENTS AT TOP OF COLUMNS               06340
C                                                                          06350
      FA=FA+Z*FAH                                                          06360
      FB=FB+Z*FBH                                                          06370
      FC=FC+Z*FCH                                                          06380
      FD=FD+Z*FDH                                                          06390
      FE=FE+Z*FEH                                                          06400
      FF=FF+Z*FFH                                                          06410
C                                                                          06420
C        COMPUTE TEMP/SHRINK FINAL MOMENTS AT SPAN ENDS                    06430
C                                                                          06440
      FAB=-FAB+Z*FABH                                                      06450
      FBA=FBA+Z*FBAH                                                       06460
      FBC=-FBC+Z*FBCH                                                      06470
      FCB=FCB+Z*FCBH                                                       06480
      FCD=-FCD+Z*FCDH                                                      06490
      FDC=FDC+Z*FDCH                                                       06500
      FDE=-FDE+Z*FDEH                                                      06510
      FED=FED+Z*FEDH                                                       06520
      FEF=-FEF+Z*FEFH                                                      06530
      FFE=FFE+Z*FFEH                                                       06540
      IET=IDINT(ET+0.5)                                                 .B.06550
      WRITE (OUNIT,1090) T,IET                                          .B.06560
 1090 FORMAT(/' **  SHRINKAGE PLUS TEMPERATURE DROP OF',F6.2,           .B.06570
     1        '        E =',I7,'  **')                                  .B.06580
      WRITE (OUNIT,1100) (ITOP(LL),LL=1,JO1)                               06590
 1100 FORMAT(/' COL. DEFLS.    ',6(4X,A4))                              .B.06600
      WRITE (OUNIT,1110) (D91(LL),LL=1,JO1)                                06610
 1110 FORMAT('     (FT)        ',6F8.5)                                    06620
      WRITE (OUNIT,900) (ITOP(LL),LL=1,JO1)                                06630
      WRITE (OUNIT,910) (F95(LL),LL=1,JO1)                                 06640
      WRITE (OUNIT,920) (IBOT(LL),LL=1,JO1)                                06650
      WRITE (OUNIT,930) (F94(LL),LL=1,JO1)                                 06660
      WRITE (OUNIT,940) (ILT(LL),IRT(LL),LL=1,JO)                          06680
      WRITE (OUNIT,950) (F8A(LL),F8B(LL),LL=1,JO)                          06690
 1150 WRITE (OUNIT,1160) (ITOP(LL),LL=1,JO1)                               06750
 1160 FORMAT(/' COL. SHEARS     ',6(4X,A4))                             .B.06760
      WRITE (OUNIT,910) (V92(LL),LL=1,JO1)                                 06770
 1170 WRITE (OUNIT,1180) IDEN                                              06780
 1180 FORMAT(/' END OF PROBLEM IDENT NO ',I4)                              06790
      IF(MPB)20,1220,20                                                 .C.
 1190 WRITE (*,1200) LNUM                                               .T.06810
 1200 FORMAT(/' !!!!!!!!  INPUT ERROR IN LINE ',I3,'!!! CHECK DATA!!')  .C.06820
 1220 STOP                                                              .C.06840
 1225 WRITE (*,1230)LNUM                                                .T.
 1230 FORMAT('!!!!!!!  LINE ',I3,'  IS OUT OF ORDER!! CHECK DATA!!')    .C.
      STOP                                                              .C.
      END                                                               .C.06850
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
      SUBROUTINE PARM(IMODE,OUNIT,ERUNIT,CARCNT)                        .C.
      CHARACTER*16 FLNM1, FLNM2                                         .C.
      CHARACTER*1 CARCNT                                                .C.
      INTEGER OUNIT, ERUNIT                                             .C.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   .C.
C  STANDARD INTERACTIVE INPUT UNIT                                      .C.
      IIUNIT=1                                                          .C.
C  STANDARD INTERACTIVE OUTPUT UNIT                                     .C.
      IOUNIT=3                                                          .C.
C  STANDARD REPORT OUTPUT UNIT                                          .C.
      OUNIT=6                                                           .C.
C  STANDARD FILE INPUT UNIT                                             .C.
      INUNIT=5                                                          .C.
C  STANDARD ERROR OUTPUT UNIT                                           .C.
      ERUNIT=3                                                          .C.
C  IMODE=1 FOR FREE FORMAT                                              .C.
      IMODE=1                                                           .C.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   .C.
C     OPEN(IIUNIT,FILE='CON')                                           .T.
C     OPEN(IOUNIT,FILE='CON')                                           .T.
10    WRITE(*,210)                                                      .T.
210   FORMAT(//,                                                        .C.
     1' Enter input file name:',/)                                      .T.
           READ(*,ERR=10,FMT='(A)')FLNM1                                .T.
           IF(FLNM1.EQ.'CON')GO TO 25                                   .T.
           IF(FLNM1.EQ.'con')GO TO 25                                   .T.
              DO 15 I=1,16                                              .C.
               IF(FLNM1(I:I).EQ.'.')GO TO 25                            .C.
15             CONTINUE                                                 .C.
              DO 20 I=1,16                                              .C.
                 IF(FLNM1(I:I).EQ.' ')THEN                              .C.
                  IE = I+3                                              .C.
                  FLNM1(I:IE) = '.dat'                                  .C.
                  GO TO 25                                              .C.
                 END IF                                                 .C.
20             CONTINUE                                                 .C.
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1      .T.
          CARCNT='1'                                                    .C.
35    WRITE(*,235)                                                      .T.
235   FORMAT(//,                                                        .C.
     1' Enter output file name:',/)                                     .T.
           READ(*,ERR=35,FMT='(A)')FLNM2                                .T.
           IF(FLNM2.EQ.'CON')GO TO 60                                   .T.
           IF(FLNM2.EQ.'con')GO TO 60                                   .T.
              DO 40 I=1,16                                              .C.
               IF(FLNM2(I:I).EQ.'.')GO TO 60                            .C.
40             CONTINUE                                                 .C.
              DO 50 I=1,16                                              .C.
                 IF(FLNM2(I:I).EQ.' ')THEN                              .C.
                  IE = I+3                                              .C.
                  FLNM2(I:IE) = '.out'                                  .C.
                  GO TO 60                                              .C.
                 END IF                                                 .C.
50             CONTINUE                                                 .C.
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2    .T.
          OPEN(OUNIT,FILE=FLNM2)                                        .C.
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                          .C.
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                      .C.
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                      .C.
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                      .C.
      RETURN                                                            .C.
      END                                                               .C.
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
