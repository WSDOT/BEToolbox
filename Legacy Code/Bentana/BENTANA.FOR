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

C       ***  BENT ANALYSIS PROGRAM  ***  'BENTANA' ***  RFF720728  ***  00000100
C                   P63060                                              00000200
C                                                                       00000300
C    THIS PROGRAM COMPUTES THE MOMENT AND SHEAR STRESSES ON A SINGLE    00000400
C    STORY BENT OR FRAME DUE TO UNIFORM AND CONCENTRATED LOADS.         00000500
C                                                                       00000600
C                                                                       00000700
C    REVISION LOG:                                                      00000800
C                                                                       00000900
C         T - CONVERSION TO VMS BY TERRY BEEBE JANUARY 1990              T
C         RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C                                                                       00001000
C    NAME     TYPE    SOURCE                     DESCRIPTION            00001100
C    AB       ARRAY   CALCULATED   MOMENTS AT POINTS OF SPAN 1          00001200
C    AL       REAL    READ         LT CONCENTRATED LOAD CANTILEVER ARM  00001300
C    ALT      REAL    READ         SET EQUAL TO AL                      00001400
C    AMOM     REAL    CALCULATED   FACTOR FOR FINDING AXPOS AND AXNEG   00001500
C    AMOM2    REAL    CALCULATED   FACTOR FOR FINDING AXPOS AND AXNEG   00001600
C    AO       REAL    CALCULATED   INTERMED. VALUE FOR FINDING B(I)     00001700
C    AR       REAL    READ         RT CONCENTRATED LOAD CANTILEVER ARM  00001800
C    ART      REAL    READ         SET EQUAL TO AR                      00001900
C    AXNEG    REAL    CALCULATED   SCALE SETTER FOR PLOTTING            00002000
C    AXPOS    REAL    CALCULATED   SCALE SETTER FOR PLOTTING            00002100
C    A1       ARRAY   READ         CONCENTRATED LOAD LOCATION OF SPAN1  00002200
C    A2       ARRAY   READ         CONCENTRATED LOAD LOCATION OF SPAN2  00002300
C    A3       ARRAY   READ         CONCENTRATED LOAD LOCATION OF SPAN3  00002400
C    A4       ARRAY   READ         CONCENTRATED LOAD LOCATION OF SPAN4  00002500
C    A5       ARRAY   READ         CONCENTRATED LOAD LOCATION OF SPAN5  00002600
C    B        ARRAY   CALCULATED   MOMENTS OF CONCENTRATED LOADS        00002700
C    BA       REAL    CALCULATED   INTERMED. VALUE FOR FINDING FAB & FBA00002800
C    BB       REAL    CALCULATED   INTERMED. VALUE FOR FINDING FBC & FCB00002900
C    BC       REAL    CALCULATED   INTERMED. VALUE FOR FINDING FCD & FDC00003000
C    BD       REAL    CALCULATED   INTERMED. VALUE FOR FINDING FDE & FED00003100
C    BE       REAL    CALCULATED   INTERMED. VALUE FOR FINDING FEF & FFE00003200
C    BF       REAL    CALCULATED   INTERMED. VALUE FOR FINDING FEF & FFE00003300
C    BO       REAL    CALCULATED   INTERMED. VALUE FOR FINDING B(I)     00003400
C    B2       ARRAY   CALCULATED   MOMENTS AT POINTS OF SPAN2           00003500
C    CD       ARRAY   CALCULATED   MOMENTS AT POINTS OF SPAN3           00003600
C    CL       REAL    CALCULATED   LEFT CANTILEVER MOMENT               00003700
C    CR       REAL    CALCULATED   RIGHT CALTILEVER MOMENT              00003800
C    DA       REAL    READ         COLUMN JOINT DISTRIBUTION FACTOR     00003900
C    DAB      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00004000
C    DAT      REAL    CALCULATED   SET EQUAL TO DA                      00004100
C    DB       REAL    READ         COLUMN JOINT DISTRIBUTION FACTOR     00004200
C    DBA      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00004300
C    DBC      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00004400
C    DBT      REAL    CALCULATED   SET EQUAL TO DB                      00004500
C    DC       REAL    READ         COLUMN JOINT DISTRIBUTION FACTOR     00004600
C    DCB      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00004700
C    DCD      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00004800
C    DCT      REAL    CALCULATED   SET EQUAL TO DC                      00004900
C    DD       REAL    READ         COLUMN JOINT DISTRIBUTION FACTOR     00005000
C    DDC      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00005100
C    DDE      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00005200
C    DDT      REAL    CALCULATED   SET EQUAL TO DD                      00005300
C    DE       REAL    READ         COLUMN JOINT DISTRIBUTION FACTOR     00005400
C    DED      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00005500
C    DEF      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00005600
C    DET      REAL    CALCULATED   SET EQUAL TO DE                      00005700
C    DF       REAL    READ         COLUMN JOINT DISTRIBUTION FACTOR     00005800
C    DFE      REAL    READ         SPAN JOINT DISTRIBUTION FACTOR       00005900
C    DFT      REAL    CALCULATED   SET EQUAL TO DF                      00006000
C    D2       ARRAY   CALCULATED   MOMENTS AT POINTS OF SPAN4           00006100
C    EF       ARRAY   CALCULATED   MOMENTS AT POINTS OF SPAN5           00006200
C    FA       REAL    CALCULATED   INTERMED. VALUE FOR FINDING END MOMS.00006300
C    FAB      REAL    CALCULATED   LEFT END MOMENT OF SPAN1             00006400
C    FB       REAL    CALCULATED   INTERMED. VALUE FOR FINDING END MOMS.00006500
C    FBA      REAL    CALCULATED   RIGHT END MOMENT OF SPAN1            00006600
C    FBC      REAL    CALCULATED   LEFT END MOMENT OF SPAN2             00006700
C    FC       REAL    CALCULATED   INTERMED. VALUE FOR FINDING END MOMS.00006800
C    FCB      REAL    CALCULATED   RIGHT END MOMENT OF SPAN2            00006900
C    FCD      REAL    CALCULATED   LEFT END MOMENT OF SPAN3             00007000
C    FD       REAL    CALCULATED   INTERMED. VALUE FOR FINDING END MOMS.00007100
C    FDC      REAL    CALCULATED   RIGHT END MOMENT OF SPAN3            00007200
C    FDE      REAL    CALCULATED   LEFT END MOMENT OF SPAN4             00007300
C    FE       REAL    CALCULATED   INTERMED. VALUE FOR FINDING END MOMS.00007400
C    FED      REAL    CALCULATED   RIGHT END MOMENT OF SPAN4            00007500
C    FEF      REAL    CALCULATED   LEFT END MOMENT OF SPAN5             00007600
C    FF       REAL    CALCULATED   INTERMED. VALUE FOR FINDING END MOMS.00007700
C    FFE      REAL    CALCULATED   RIGHT END MOMENT OF SPAN5            00007800
C    PL       REAL    READ         LEFT CANTILEVER CONCENTRATED LOAD    00007900
C    PLT      REAL    READ         SET EQUAL TO PL                      00008000
C    PR       REAL    READ         RIGHT CANTILEVER CONCENTRATED LOAD   00008100
C    PRT      REAL    READ         SET EQUAL TO PR                      00008200
C    P1       ARRAY   READ         CONCENTRATED LOAD VALUE OF SPAN1     00008300
C    P2       ARRAY   READ         CONCENTRATED LOAD VALUE OF SPAN2     00008400
C    P3       ARRAY   READ         CONCENTRATED LOAD VALUE OF SPAN3     00008500
C    P4       ARRAY   READ         CONCENTRATED LOAD VALUE OF SPAN4     00008600
C    P5       ARRAY   READ         CONCENTRATED LOAD VALUE OF SPAN5     00008700
C    S1       REAL    READ         LENGTH OF SPAN1                      00008800
C    S2       REAL    READ         LENGTH OF SPAN2                      00008900
C    S3       REAL    READ         LENGTH OF SPAN3                      00009000
C    S4       REAL    READ         LENGTH OF SPAN4                      00009100
C    S5       REAL    READ         LENGTH OF SPAN5                      00009200
C    VLS1     REAL    CALCULATED   LEFT SHEAR OF SPAN1                  00009300
C    VLS2     REAL    CALCULATED   LEFT SHEAR OF SPAN2                  00009400
C    VLS3     REAL    CALCULATED   LEFT SHEAR OF SPAN3                  00009500
C    VLS4     REAL    CALCULATED   LEFT SHEAR OF SPAN4                  00009600
C    VLS5     REAL    CALCULATED   LEFT SHEAR OF SPAN5                  00009700
C    VRS1     REAL    CALCULATED   RIGHT SHEAR OF SPAN1                 00009800
C    VRS2     REAL    CALCULATED   RIGHT SHEAR OF SPAN2                 00009900
C    VRS3     REAL    CALCULATED   RIGHT SHEAR OF SPAN3                 00010000
C    VRS4     REAL    CALCULATED   RIGHT SHEAR OF SPAN4                 00010100
C    VRS5     REAL    CALCULATED   RIGHT SHEAR OF SPAN5                 00010200
C    WL       REAL    READ         LEFT CANTILEVER UNIFORM LOAD         00010300
C    WR       REAL    READ         RIGHT CANTILEVER UNIFORM LOAD        00010400
C    W1       REAL    READ         UNIFORM LOAD OF SPAN1                00010500
C    W2       REAL    READ         UNIFORM LOAD OF SPAN2                00010600
C    W3       REAL    READ         UNIFORM LOAD OF SPAN3                00010700
C    W4       REAL    READ         UNIFORM LOAD OF SPAN4                00010800
C    W5       REAL    READ         UNIFORM LOAD OF SPAN5                00010900
C    X        ARRAY   CALCULATED   RETAINS VALUES OF MOMENTS AT POINTS  00011000
C    XL       REAL    READ         LEFT UNIFORM LOAD CANTILEVER ARM     00011100
C    XR       REAL    READ         RIGHT UNIFORM LOAD CANTILEVER ARM    00011200
C                                                                       00011300
C                                                                       00011400
      LOGICAL*1 CCHAR                                                   00011500
      CHARACTER*1 CARCNT                                                00011600
      CHARACTER*4 IMODE, FREE                                           00011700
      CHARACTER*4 NU, NW                                                00011800
      INTEGER*4 OUNIT, ERUNIT                                           00011900
      CHARACTER*8 REVDAT, TIME                                            T12000
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T 
      DIMENSION AB(10),B2(10),IMO(20)                                   00012100
      DIMENSION CD(10),D2(10),WORK(2000)                                00012200
      DIMENSION EF(10),A1(10)                                           00012300
      DIMENSION P1(10),A2(10)                                           00012400
      DIMENSION P2(10),A3(10)                                           00012500
      DIMENSION P3(10),A4(10)                                           00012600
      DIMENSION P4(10),A5(10)                                           00012700
      DIMENSION P5(10),B(10)                                            00012800
      DIMENSION X(10),NU(10),NW(10)                                     00012900
      COMMON IEND,NPAGE,ICNT,SCAY,DY1                                   00013000
      COMMON /SPAC1/ OUNIT                                              00013100
      COMMON /SPAC2/ CARCNT                                             00013200
      DATA NU(1),NU(2),NU(3),NU(4),NU(5)/'M.1','M.2','M.3','M.4','M.5'/ 00013300
      DATA NU(6),NU(7),NU(8),NU(9),NU(10)/'M.6','M.7','M.8','M.9','M10'/00013400
      DATA NW(1),NW(2),NW(3),NW(4),NW(5)/'ML1','ML2','ML3','ML4','ML5'/ 00013500
      DATA NW(6),NW(7),NW(8),NW(9),NW(10)/'ML6','ML7','ML8','ML9','M10'/00013600
C                                                                       00013700
C                                                                       00013800
C     CALL OSDATE(DATE)                                                  T013900
      CALL LIB$DATE_TIME (DATIME)                                        T
      TIME = DATIME(13:20)                                               T  
      DATE = DATIME(1:11)                                                T
C     CALL GETIME(TIME)                                                 00014000
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                         00014100
C                                                                       00014200
      REVDAT=' 1/30/90'                                                 0T014300
C                                                                       00014400
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                         00014500
C                                                                       00014600
C                                                                       00014700
      ISW1 = 0                                                          00014800
      K = 0                                                             00014900
      IEND=0                                                            00015000
      X00=0.                                                            00015100
      N33=3                                                             00015200
      N22=2                                                             00015300
      JO=0                                                              00015400
      SCAX=0.                                                           00015500
      SCAY=0.                                                           00015600
      DY1=0.                                                            00015700
      NPAGE=0                                                           00015800
      ICNT=50                                                           00015900
      NMO=0                                                             00016000
      JSW=0                                                             00016100
      ISW2=0                                                            00016200
      ISW=0                                                             00016300
      NP = 0                                                            00016400
C     REWIND 7                                                          00016500
C     REWIND 8                                                          00016600
      IMODE='FREE'                                                      00016700
      OUNIT=9                                                           00016800
      ERUNIT=9                                                          00016900
      INUNIT=5                                                          00017000
      CARCNT='1'                                                        00017100
      FREE='FREE'                                                       00017200
      CCHAR=.TRUE.                                                      00017300
C                                                                       00017400
C                  PARM INFO PASSED TO THE PROGRAM                      00017500
C    IMODE = 'FREE' NOT USED ANYMORE                                    00017600
C    OUNIT = (6 FOR PC, 9 FOR MAINFRAME)                                00017700
C    ERUNIT = (3 FOR PC, 9 FOR MAINFRAME)                               00017800
C    CCHAR = .TRUE. NOT USED ANYMORE                                    00017900
C                                                                       00018000
      CALL PARM(IMODE,OUNIT,ERUNIT,CCHAR)                               00018100
      IF(OUNIT.EQ.6)WRITE(OUNIT,*)'+'                                  00018200
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC           00018300
C                                                                       00018400
      READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK                          00018500
      BACKSPACE INUNIT                                                  00018600
   40 IF (ICHK-1) 70,90,70                                              00018700
   70 ISW=ICHK                                                          00018800
      WRITE (*,80)ISW                                                   00018900
      WRITE ( OUNIT,80)ISW                                              00019000
   80 FORMAT(//' DATA ERROR IN YOUR LINE ',I2)                          00019100
      GO TO 3560                                                        00019200
C                                                                       00019300
C    BEGIN READING DATA FOR UNIFORM LOAD PROBLEM                        00019400
C                                                                       00019500
   90 READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK,                         00019600
     1                IDEN,DA,DB,DC,DD,DE,DF,K                          00019700
      IF (K.NE.0) THEN                                                  00019800
      K=0                                                               00019900
      WRITE(*,*)' PLOTS NOT AVAILABLE ON BENTANA AT THIS TIME. PROG     00020000
     1RAM CONTINUES.'                                                   00020100
C     CALL PLOTS(WORK,2000)                                             00020200
      END IF                                                            00020300
  130 IDENT = IDEN                                                      00020400
      MO=0                                                              00020500
      READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK                          00020600
      BACKSPACE INUNIT                                                  00020700
  150 IF (ICHK-2) 160,170,160                                           00020800
  160 ISW=ICHK                                                          00020900
      GO TO 70                                                          00021000
  170 READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK,                         00021100
     1                DAB,DBA,DBC,DCB,DCD,DDC,DDE,DED,DEF,DFE           00021200
  200 NS = 0                                                            00021300
      IF (DAB .GT. 0.0) NS = NS + 1                                     00021400
      IF (DBA .GT. 0.0) NS = NS + 1                                     00021500
      IF (DBC .GT. 0.0) NS = NS + 1                                     00021600
      IF (DCB .GT. 0.0) NS = NS + 1                                     00021700
      IF (DCD .GT. 0.0) NS = NS + 1                                     00021800
      IF (DDC .GT. 0.0) NS = NS + 1                                     00021900
      IF (DDE .GT. 0.0) NS = NS + 1                                     00022000
      IF (DED .GT. 0.0) NS = NS + 1                                     00022100
      IF (DEF .GT. 0.0) NS = NS + 1                                     00022200
      IF (DFE .GT. 0.0) NS = NS + 1                                     00022300
      READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK                          00022400
      BACKSPACE INUNIT                                                  00022500
  220 IF (ICHK-3) 230,240,230                                           00022600
  230 ISW=ICHK                                                          00022700
      GO TO 70                                                          00022800
  240 READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK,                         00022900
     1                XL,S1,S2,S3,S4,S5,XR                              00023000
  270 READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK                          00023100
      BACKSPACE INUNIT                                                  00023200
  290 IF (ICHK-4) 300,350,300                                           00023300
  300 IF (ICHK-5) 310,320,310                                           00023400
C                                                                       00023500
C    READS FIRST CARD OF CONCENTRATED LOAD PROBLEM (NOT ALWAYS USED)    00023600
C                                                                       00023700
  310 ISW=ICHK                                                          00023800
      GO TO 70                                                          00023900
  320 READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK,                         00024000
     1                 ALT,PLT,ART,PRT,NO2                              00024100
  340 KEY=1                                                             00024200
      GO TO 370                                                         00024300
  350 READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK,                         00024400
     1     WL,W1,W2,W3,W4,W5,WR                                         00024500
      GO TO 380                                                         00024600
C                                                                       00024700
C    COMPUTE UNIFORM LOAD FIXED END MOMENTS                             00024800
C                                                                       00024900
  370 WL=0.                                                             00025000
      W1=0.                                                             00025100
      W2=0.                                                             00025200
      W3=0.                                                             00025300
      W4=0.                                                             00025400
      W5=0.                                                             00025500
      WR=0.                                                             00025600
  380 IF (JSW) 410,390,410                                              00025700
  390 AXNEG=0.                                                          00025800
      AXPOS=0.                                                          00025900
      IF (K.EQ.0) GO TO 410                                             00026000
      WRITE (7,400) IDEN,XL,S1,S2,S3,S4,S5,XR,JO,SCAX,SCAY              00026100
  400 FORMAT(A4,7F7.2,I1,2F3.0)                                         00026200
  410 IF (WL) 490,420,490                                               00026300
  420 IF (W1) 490,430,490                                               00026400
  430 IF (W2) 490,440,490                                               00026500
  440 IF (W3) 490,450,490                                               00026600
  450 IF (W4) 490,460,490                                               00026700
  460 IF (W5) 490,470,490                                               00026800
  470 IF (WR) 490,480,490                                               00026900
  480 J=0                                                               00027000
      GO TO 1480                                                        00027100
C                                                                       00027200
C    BEGIN CALCULATIONS FOR UNIFORM LOAD PROBLEM                        00027300
C                                                                       00027400
  490 J=1                                                               00027500
      NMO=NMO+1                                                         00027600
      IMO(NMO)=1                                                        00027700
      CL=-WL*XL*XL/2.                                                   00027800
      FAB=W1*S1*S1/12.                                                  00027900
      FBA=-FAB                                                          00028000
      FBC=W2*S2*S2/12.                                                  00028100
      FCB=-FBC                                                          00028200
      FCD=W3*S3*S3/12.                                                  00028300
      FDC=-FCD                                                          00028400
      FDE=W4*S4*S4/12.                                                  00028500
      FED=-FDE                                                          00028600
      FEF=W5*S5*S5/12.                                                  00028700
      FFE=-FEF                                                          00028800
      CR=WR*XR*XR/2.                                                    00028900
C                                                                       00029000
C    MOMENT DISTRIBUTION ROUTINE                                        00029100
C                                                                       00029200
      KEY=0                                                             00029300
  500 N=0                                                               00029400
      FA=0.                                                             00029500
      FB=0.                                                             00029600
      FC=0.                                                             00029700
      FD=0.                                                             00029800
      FE=0.                                                             00029900
      FF=0.                                                             00030000
C                                                                       00030100
C    BAL DIST CO AT JT A                                                00030200
C                                                                       00030300
  510 BA=-(CL+FAB+FA)                                                   00030400
      FA=FA+BA*DA                                                       00030500
      FAB=FAB+BA*DAB                                                    00030600
      FBA=FBA+BA*DAB*0.5                                                00030700
C                                                                       00030800
C    BAL DIST CO AT JT B                                                00030900
C                                                                       00031000
      IF (S2) 530,520,530                                               00031100
  520 BB=-(CR+FBA+FB)                                                   00031200
      FBA=FBA+BB*DBA                                                    00031300
      FAB=FAB+BB*DBA*.5                                                 00031400
      FB=FB+BB*DB                                                       00031500
      GO TO 600                                                         00031600
  530 BB=-(FBA+FBC+FB)                                                  00031700
      FBA=FBA+BB*DBA                                                    00031800
      FAB=FAB+BB*DBA*.5                                                 00031900
      FB=FB+BB*DB                                                       00032000
      FBC=FBC+BB*DBC                                                    00032100
      FCB=FCB+BB*DBC*.5                                                 00032200
C                                                                       00032300
C    BAL DIST CO AT JT C                                                00032400
C                                                                       00032500
      IF (S3) 550,540,550                                               00032600
  540 BC=-(CR+FCB+FC)                                                   00032700
      FCB=FCB+BC*DCB                                                    00032800
      FBC=FBC+BC*DCB*.5                                                 00032900
      FC=FC+BC*DC                                                       00033000
      GO TO 600                                                         00033100
  550 BC=-(FCB+FCD+FC)                                                  00033200
      FCB=FCB+BC*DCB                                                    00033300
      FBC=FBC+BC*DCB*.5                                                 00033400
      FC=FC+BC*DC                                                       00033500
      FCD=FCD+BC*DCD                                                    00033600
      FDC=FDC+BC*DCD*.5                                                 00033700
C                                                                       00033800
C    BAL DIST CO AT JT D                                                00033900
C                                                                       00034000
      IF (S4) 570,560,570                                               00034100
  560 BD=-(CR+FDC+FD)                                                   00034200
      FDC=FDC+BD*DDC                                                    00034300
      FCD=FCD+BD*DDC*.5                                                 00034400
      FD=FD+BD*DD                                                       00034500
      GO TO 600                                                         00034600
  570 BD=-(FDC+FDE+FD)                                                  00034700
      FDC=FDC+BD*DDC                                                    00034800
      FCD=FCD+BD*DDC*.5                                                 00034900
      FD=FD+BD*DD                                                       00035000
      FDE=FDE+BD*DDE                                                    00035100
      FED=FED+BD*DDE*.5                                                 00035200
C                                                                       00035300
C    BAL DIST CO AT JT E                                                00035400
C                                                                       00035500
      IF (S5) 590,580,590                                               00035600
  580 BE=-(CR+FED+FE)                                                   00035700
      FED=FED+BE*DED                                                    00035800
      FDE=FDE+BE*DED*.5                                                 00035900
      FE=FE+BE*DE                                                       00036000
      GO TO 600                                                         00036100
  590 BE=-(FED+FEF+FE)                                                  00036200
      FED=FED+BE*DED                                                    00036300
      FDE=FDE+BE*DED*.5                                                 00036400
      FE=FE+BE*DE                                                       00036500
      FEF=FEF+BE*DEF                                                    00036600
      FFE=FFE+BE*DEF*.5                                                 00036700
C                                                                       00036800
C    BAL DIST CO AT JT F                                                00036900
C                                                                       00037000
      BF=-(CR+FFE+FF)                                                   00037100
      FFE=FFE+BF*DFE                                                    00037200
      FEF=FEF+BF*DFE*.5                                                 00037300
      FF=FF+BF*DF                                                       00037400
  600 N=N+1                                                             00037500
      IF (N-5) 510,610,510                                              00037600
  610 IF (J) 620,2250,620                                               00037700
  620 J=0                                                               00037800
      IF (CR) 640,640,630                                               00037900
  630 CR=-CR                                                            00038000
C                                                                       00038100
C    BEGIN WRITING ANSWERS FOR UNIFORM LOAD PROBLEM                     00038200
C                                                                       00038300
  640 ICNT=ICNT+6                                                       00038400
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00038500
      WRITE (OUNIT,650)                                                 00038600
  650 FORMAT(/10X,'L CANT',6X,'SPAN 1',7X,'SPAN 2',7X,                  00038700
     1            'SPAN 3',7X,'SPAN 4',7X,'SPAN 5',6X,'R CANT')         00038800
      WRITE (OUNIT,660) XL,S1,S2,S3,S4,S5,XR                            00038900
  660 FORMAT(' LENGTH   ',F6.2,5(5X,F6.2,2X),5X,F6.2)                   00039000
      WRITE (OUNIT,670) WL,W1,W2,W3,W4,W5,WR                            00039100
  670 FORMAT(' UNI WT   ',F6.2,5(5X,F6.2,2X),5X,F6.2)                   00039200
      WRITE (OUNIT,680) DAB,DBA,DBC,DCB,DCD,DDC,DDE,DED,DEF,DFE         00039300
  680 FORMAT(/' SPAN D. F.',7X,5(1X,F5.3,2X,F5.3))                      00039400
      WRITE (OUNIT,690) DA,DB,DC,DD,DE,DF                               00039500
  690 FORMAT(' COLUMN D. F.   ',F5.3,5(8X,F5.3))                        00039600
C                                                                       00039700
      IF (XL.EQ.0.) GO TO 730                                           00039800
      WRITE (OUNIT,720) CL                                              00039900
      IF (K.EQ.0) GO TO 730                                             00040000
      CL1=4.*CL/9.                                                      00040100
      WRITE (7,700) X00,CL1,CL                                          00040200
  700 FORMAT(3F7.1)                                                     00040300
      XL1=2.*XL/3.                                                      00040400
      WRITE (7,710) X00,XL1,XL                                          00040500
  710 FORMAT(3F7.2)                                                     00040600
C                                                                       00040700
  720 FORMAT(/' UNIF LOAD CANT MOM LEFT  = ',F7.1)                      00040800
  730 IF (XR.EQ.0.) GO TO 750                                           00040900
      WRITE (OUNIT,740) CR                                              00041000
  740 FORMAT(/' UNIF LOAD CANT MOM RIGHT = ',F7.1)                      00041100
  750 IF (JSW) 3400,760,3400                                            00041200
  760 IF (CL-CR) 780,770,770                                            00041300
  770 AXNEG=CR                                                          00041400
      GO TO 790                                                         00041500
  780 AXNEG=CL                                                          00041600
  790 VLS1=(W1*S1/2.)+(FAB+FBA)/S1                                      00041700
      VRS1=-(W1*S1)+VLS1                                                00041800
      FAB=-FAB                                                          00041900
      ICNT=ICNT+5                                                       00042000
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00042100
      WRITE (OUNIT,800)                                                 00042200
  800 FORMAT(' UNIF LOAD SHEAR AND MOMENT VALUES'//' SPAN 1',5X,        00042300
     1    'V LT',5X,'V RT',5X,'M LT',5X,'M RT')                         00042400
      WRITE (OUNIT,810) VLS1,VRS1,FAB,FBA                               00042500
  810 FORMAT(7X,4F9.1)                                                  00042600
      WRITE (24) FAB, FBA, VLS1, VRS1, S1, W1                           00042700
      IF (W1) 820,940,820                                               00042800
  820 AB(1)=FAB                                                         00042900
      DO 830 I=1,9                                                      00043000
      AB(I+1)=AB(I)+(VLS1*S1/10.)-(W1*S1*S1/200.)                       00043100
  830 VLS1=VLS1-W1*S1/10.                                               00043200
      IF (K.NE.1) GO TO 860                                             00043300
      WRITE (7,840) N33,FAB,AB(6),FBA                                   00043400
  840 FORMAT(I2,12F7.1)                                                 00043500
      S11=S1/2.                                                         00043600
      WRITE (7,850) X00,S11,S1                                          00043700
  850 FORMAT(2X,12F7.2)                                                 00043800
C                                                                       00043900
  860 ICNT=ICNT+3                                                       00044000
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00044100
  890 WRITE (OUNIT,900)                                                 00044200
  900 FORMAT(/13X,'M.1      M.2      M.3      M.4      M.5      M.6',   00044300
     1      '      M.7      M.8      M.9')                              00044400
      WRITE (OUNIT,910) (AB(LL),LL=2,10)                                00044500
  910 FORMAT(7X,9F9.1)                                                  00044600
  920 AMOM1=FAB                                                         00044700
      AMOM2=FBA                                                         00044800
      DO 930 LL=2,10                                                    00044900
  930 X(LL)=AB(LL)                                                      00045000
      ISW10=1                                                           00045100
      NX=2                                                              00045200
      GO TO 3240                                                        00045300
  940 IF (K.EQ.0) GO TO 950                                             00045400
      WRITE (7,840) N22,FAB,FBA                                         00045500
      WRITE (7,850) X00,S1                                              00045600
  950 IF (S2) 960,1350,960                                              00045700
  960 VLS2=(W2*S2/2.)+(FBC+FCB)/S2                                      00045800
      VRS2=-(W2*S2)+VLS2                                                00045900
      FBC=-FBC                                                          00046000
      ICNT=ICNT+3                                                       00046100
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00046200
      WRITE (OUNIT,970)                                                 00046300
  970 FORMAT(/' SPAN 2     V LT     V RT     M LT     M RT')            00046400
      WRITE (OUNIT,810) VLS2,VRS2,FBC,FCB                               00046500
      WRITE (24) FBC, FCB, VLS2, VRS2, S2, W2                           00046600
      IF (W2) 980,1040,980                                              00046700
  980 B2(1)=FBC                                                         00046800
      DO 990  I=1,9                                                     00046900
      B2(I+1)=B2(I)+(VLS2*S2/10.)-(W2*S2*S2/200.)                       00047000
  990 VLS2=VLS2-W2*S2/10.                                               00047100
      IF (K.NE.1) GO TO 1000                                            00047200
      WRITE (7,840) N33,FBC,B2(6),FCB                                   00047300
      S22=S2/2.                                                         00047400
      WRITE (7,850) X00,S22,S2                                          00047500
C                                                                       00047600
 1000 ICNT=ICNT+3                                                       00047700
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00047800
 1010 WRITE (OUNIT,900)                                                 00047900
      WRITE (OUNIT,910) (B2(LL),LL=2,10)                                00048000
 1020 AMOM1=FBC                                                         00048100
      AMOM2=FCB                                                         00048200
      DO 1030 LL=2,10                                                   00048300
 1030 X(LL)=B2(LL)                                                      00048400
      ISW10=2                                                           00048500
      NX=2                                                              00048600
      GO TO 3240                                                        00048700
 1040 IF (K.EQ.0) GO TO 1050                                            00048800
      WRITE (7,840) N22,FBC,FCB                                         00048900
      WRITE (7,850) X00,S2                                              00049000
 1050 IF (S3) 1060,1350,1060                                            00049100
 1060 VLS3=(W3*S3/2.)+(FCD+FDC)/S3                                      00049200
      VRS3=-(W3*S3)+VLS3                                                00049300
      FCD=-FCD                                                          00049400
      ICNT=ICNT+3                                                       00049500
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00049600
      WRITE (OUNIT,1070)                                                00049700
 1070 FORMAT(/' SPAN 3     V LT     V RT     M LT     M RT')            00049800
      WRITE (OUNIT,810) VLS3,VRS3,FCD,FDC                               00049900
      WRITE (24) FCD, FDC, VLS3, VRS3, S3, W3                           00050000
      IF (W3) 1080,1140,1080                                            00050100
 1080 CD(1)=FCD                                                         00050200
      DO 1090 I=1,9                                                     00050300
      CD(I+1)=CD(I)+(VLS3*S3/10.)-(W3*S3*S3/200.)                       00050400
 1090 VLS3=VLS3-W3*S3/10.                                               00050500
      IF (K.NE.1) GO TO 1100                                            00050600
      WRITE (7,840) N33,FCD,CD(6),FDC                                   00050700
      S33=S3/2.                                                         00050800
      WRITE (7,850) X00,S33,S3                                          00050900
C                                                                       00051000
 1100 ICNT=ICNT+3                                                       00051100
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00051200
 1110 WRITE (OUNIT,900)                                                 00051300
      WRITE (OUNIT,910) (CD(LL),LL=2,10)                                00051400
 1120 AMOM1=FCD                                                         00051500
      AMOM2=FDC                                                         00051600
      DO 1130 LL=2,10                                                   00051700
 1130 X(LL)=CD(LL)                                                      00051800
      ISW10=3                                                           00051900
      NX=2                                                              00052000
      GO TO 3240                                                        00052100
 1140 IF (K.EQ.0) GO TO 1150                                            00052200
      WRITE (7,840) N22,FCD,FDC                                         00052300
      WRITE (7,850) X00,S3                                              00052400
 1150 IF (S4) 1160,1350,1160                                            00052500
 1160 VLS4=(W4*S4/2.)+(FDE+FED)/S4                                      00052600
      VRS4=-(W4*S4)+VLS4                                                00052700
      FDE=-FDE                                                          00052800
      ICNT=ICNT+3                                                       00052900
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00053000
      WRITE (OUNIT,1170)                                                00053100
 1170 FORMAT(/' SPAN 4     V LT     V RT     M LT     M RT')            00053200
      WRITE (OUNIT,810) VLS4,VRS4,FDE,FED                               00053300
      WRITE (24) FDE, FED, VLS4, VRS4, S4, W4                           00053400
      IF (W4) 1180,1240,1180                                            00053500
 1180 D2(1)=FDE                                                         00053600
      DO 1190 I=1,9                                                     00053700
      D2(I+1)=D2(I)+(VLS4*S4/10.)-(W4*S4*S4/200.)                       00053800
 1190 VLS4=VLS4-W4*S4/10.                                               00053900
      IF (K.NE.1) GO TO 1200                                            00054000
      WRITE (7,840) N33,FDE,D2(6),FED                                   00054100
      S44=S4/2.                                                         00054200
      WRITE (7,850) X00,S44,S4                                          00054300
C                                                                       00054400
 1200 ICNT=ICNT+3                                                       00054500
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00054600
 1210 WRITE (OUNIT,900)                                                 00054700
      WRITE (OUNIT,910) (D2(LL),LL=2,10)                                00054800
 1220 AMOM1=FDE                                                         00054900
      AMOM2=FED                                                         00055000
      DO 1230 LL=2,10                                                   00055100
 1230 X(LL)=D2(LL)                                                      00055200
      ISW10=4                                                           00055300
      NX=2                                                              00055400
      GO TO 3240                                                        00055500
 1240 IF (K.EQ.0) GO TO 1250                                            00055600
      WRITE (7,840) N22,FDE,FED                                         00055700
      WRITE (7,850) X00,S4                                              00055800
 1250 IF (S5) 1260,1350,1260                                            00055900
 1260 VLS5=(W5*S5/2.)+(FEF+FFE)/S5                                      00056000
      VRS5=-(W5*S5)+VLS5                                                00056100
      FEF=-FEF                                                          00056200
      ICNT=ICNT+3                                                       00056300
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00056400
      WRITE (OUNIT,1270)                                                00056500
 1270 FORMAT(/' SPAN 5     V LT     V RT     M LT     M RT')            00056600
      WRITE (OUNIT,810) VLS5,VRS5,FEF,FFE                               00056700
      WRITE (24) FEF, FFE, VLS5, VRS5, S5, W5                           00056800
      IF (W5) 1280,1340,1280                                            00056900
 1280 EF(1)=FEF                                                         00057000
      DO 1290 I=1,9                                                     00057100
      EF(I+1)=EF(I)+(VLS5*S5/10.)-(W5*S5*S5/200.)                       00057200
 1290 VLS5=VLS5-W5*S5/10.                                               00057300
      IF (K.NE.1) GO TO 1300                                            00057400
      WRITE (7,840) N33,FEF,EF(6),FFE                                   00057500
      S55=S5/2.                                                         00057600
      WRITE (7,850) X00,S55,S5                                          00057700
C                                                                       00057800
 1300 ICNT=ICNT+3                                                       00057900
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00058000
 1310 WRITE (OUNIT,900)                                                 00058100
      WRITE (OUNIT,910) (EF(LL),LL=2,10)                                00058200
 1320 AMOM1=FEF                                                         00058300
      AMOM2=FFE                                                         00058400
      DO 1330 LL=2,10                                                   00058500
 1330 X(LL)=EF(LL)                                                      00058600
      ISW10=5                                                           00058700
      NX=2                                                              00058800
      GO TO 3240                                                        00058900
 1340 IF (K.EQ.0) GO TO 1350                                            00059000
      WRITE (7,840) N22,FEF,FFE                                         00059100
      WRITE (7,850) X00,S5                                              00059200
C                                                                       00059300
C    SET VALUES TO 0                                                    00059400
C    CHECKS NEXT CARD FOR A BLANK IN COLUMN 51. IF THERE IS A BLANK,    00059500
C    THE JOB IS COMPLETED AND CONTAINED ONLY A UNIFORM LOAD PROBLEM.    00059600
C    CONTROL WOULD HAVE BYPASSED THE READING AND CALCULATING OF A       00059700
C    CONCENTRATED LOAD PROBLEM                                          00059800
C                                                                       00059900
 1350 IF (XR.EQ.0.) GO TO 1360                                          00060000
      IF (K.EQ.0) GO TO 1360                                            00060100
      CR1=4.*CR/9.                                                      00060200
      WRITE (7,700) CR,CR1,X00                                          00060300
      XR1=XR/3.                                                         00060400
      WRITE (7,710) X00,XR1,XR                                          00060500
C                                                                       00060600
 1360 READ(INUNIT,END=3440,ERR=3550,FMT=*)ICHK                          00060700
      BACKSPACE INUNIT                                                  00060800
 1390 IF (ICHK-1) 1480,1400,1480                                        00060900
 1400 READ(INUNIT,END=3450,ERR=3550,FMT=*)ICHK,                         00061000
     1                 IDEN,DA,DB,DC,DD,DE,DF                           00061100
      IF (K.NE.1) GO TO 1440                                            00061200
      WRITE (8,1430) NMO,(IMO(I),I=1,NMO)                               00061300
 1430 FORMAT(I3,20I1)                                                   00061400
      CALL BENTPL                                                       00061500
 1440 CONTINUE                                                          00061600
C     REWIND 7                                                          00061700
C     REWIND 8                                                          00061800
 1470 ISW=0                                                             00061900
      IDENT=IDEN                                                        00062000
      JSW=0                                                             00062100
      ISW1=1                                                            00062200
      ICNT=50                                                           00062300
      GO TO 130                                                         00062400
 1480 CL=0.                                                             00062500
      NMO=NMO+1                                                         00062600
      CR=0.                                                             00062700
      IMO(NMO)=2                                                        00062800
      FAB=0.                                                            00062900
      FBA=0.                                                            00063000
      FBC=0.                                                            00063100
      FCB=0.                                                            00063200
      FCD=0.                                                            00063300
      FDC=0.                                                            00063400
      FDE=0.                                                            00063500
      FED=0.                                                            00063600
      FEF=0.                                                            00063700
      FFE=0.                                                            00063800
      VLS1=0.                                                           00063900
      VRS1=0.                                                           00064000
      VLS2=0.                                                           00064100
      VRS2=0.                                                           00064200
      VLS3=0.                                                           00064300
      VRS3=0.                                                           00064400
      VLS4=0.                                                           00064500
      VRS4=0.                                                           00064600
      VLS5=0.                                                           00064700
      VRS5=0.                                                           00064800
      M=M+1                                                             00064900
      P1(1)=0.                                                          00065000
      P2(1)=0.                                                          00065100
      P3(1)=0.                                                          00065200
      P4(1)=0.                                                          00065300
      P5(1)=0.                                                          00065400
C                                                                       00065500
C    READ A & P VALUES.  COMPUTE CONCENTRATED LOAD FIXED END MOMENTS    00065600
C    AND SIMPLE SPAN SHEARS                                             00065700
C                                                                       00065800
      IF (KEY-1) 1500,1490,1500                                         00065900
 1490 AL=ALT                                                            00066000
      PL=PLT                                                            00066100
      AR=ART                                                            00066200
      PR=PRT                                                            00066300
      KEY=0                                                             00066400
      GO TO 1560                                                        00066500
 1500 IF (ICHK-4) 1510,270,1510                                         00066600
 1510 IF (ICHK-5) 1520,1530,1520                                        00066700
 1520 ISW=ICHK                                                          00066800
      GO TO 70                                                          00066900
C                                                                       00067000
C    BEGIN READING DATA AND DOING CALCS FOR CONCENTRATED LOAD PROBLEM   00067100
C                                                                       00067200
 1530 READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK,                         00067300
     1                  AL,PL,AR,PR,NO2                                 00067400
 1550 FORMAT (F4.2,F5.2,F4.2,F5.2,I2)                                   00067500
 1560 NP = NP + 1                                                       00067600
      CL=-PL*AL                                                         00067700
      CR=PR*AR                                                          00067800
      READ(INUNIT,END=1640,ERR=3550,FMT=*)ICHK                          00067900
      BACKSPACE INUNIT                                                  00068000
 1580 IF (ICHK-6) 1720,1590,1720                                        00068100
 1590 READ(INUNIT,END=1640,ERR=3550,FMT=*)ICHK,                         00068200
     1     (A1(LL),LL=1,10),(P1(LL),LL=1,10)                            00068300
      GO TO 1660                                                        00068400
 1640 ISW=ICHK                                                          00068500
      GO TO 70                                                          00068600
 1660 DO 1680 I=1,10                                                    00068700
      IF (P1(I)) 1670,1690,1670                                         00068800
 1670 FAB=P1(I)*(S1-A1(I))**2*A1(I)/(S1*S1)+FAB                         00068900
      FBA=-P1(I)*(A1(I)**2)*(S1-A1(I))/(S1*S1)+FBA                      00069000
      VLS1=P1(I)*(S1-A1(I))/S1+VLS1                                     00069100
 1680 VRS1=P1(I)*(A1(I))/S1+VRS1                                        00069200
 1690 IF (S2) 1700,2220,1700                                            00069300
 1700 READ(INUNIT,END=2240,ERR=3550,FMT=*)ICHK                          00069400
      BACKSPACE INUNIT                                                  00069500
 1720 IF (ICHK-7) 1850,1730,1850                                        00069600
 1730 READ(INUNIT,END=1770,ERR=3550,FMT=*)ICHK,                         00069700
     1     (A2(LL),LL=1,10),(P2(LL),LL=1,10)                            00069800
      GO TO 1790                                                        00069900
 1770 ISW=ICHK                                                          00070000
      GO TO 70                                                          00070100
 1790 DO 1810 I=1,10                                                    00070200
      IF (P2(I)) 1800,1820,1800                                         00070300
 1800 FBC=P2(I)*(S2-A2(I))**2*A2(I)/(S2*S2)+FBC                         00070400
      FCB=-P2(I)*(A2(I)**2)*(S2-A2(I))/(S2*S2)+FCB                      00070500
      VLS2=P2(I)*(S2-A2(I))/S2+VLS2                                     00070600
 1810 VRS2=P2(I)*(A2(I))/S2+VRS2                                        00070700
 1820 IF (S3) 1830,2220,1830                                            00070800
 1830 READ(INUNIT,END=2240,ERR=3550,FMT=*)ICHK                          00070900
      BACKSPACE INUNIT                                                  00071000
 1850 IF (ICHK-8) 1980,1860,1980                                        00071100
 1860 READ(INUNIT,END=1900,ERR=3550,FMT=*)ICHK,                         00071200
     1     (A3(LL),LL=1,10),(P3(LL),LL=1,10)                            00071300
      GO TO 1920                                                        00071400
 1900 ISW=ICHK                                                          00071500
      GO TO 70                                                          00071600
 1920 DO 1940 I=1,10                                                    00071700
      IF (P3(I)) 1930,1950,1930                                         00071800
 1930 FCD=P3(I)*(S3-A3(I))**2*A3(I)/(S3*S3)+FCD                         00071900
      FDC=-P3(I)*(A3(I)**2)*(S3-A3(I))/(S3*S3)+FDC                      00072000
      VLS3=P3(I)*(S3-A3(I))/S3+VLS3                                     00072100
 1940 VRS3=P3(I)*(A3(I))/S3+VRS3                                        00072200
 1950 IF (S4) 1960,2220,1960                                            00072300
 1960 READ(INUNIT,END=2240,ERR=3550,FMT=*)ICHK                          00072400
      BACKSPACE INUNIT                                                  00072500
 1980 IF (ICHK-9) 2120,1990,2120                                        00072600
 1990 READ(INUNIT,END=2030,ERR=3550,FMT=*)ICHK,                         00072700
     1     (A4(LL),LL=1,10),(P4(LL),LL=1,10)                            00072800
      GO TO 2050                                                        00072900
 2030 ISW=ICHK                                                          00073000
      GO TO 70                                                          00073100
 2050 DO 2070 I=1,10                                                    00073200
      IF (P4(I)) 2060,2080,2060                                         00073300
 2060 FDE=P4(I)*(S4-A4(I))**2*A4(I)/(S4*S4)+FDE                         00073400
      FED=-P4(I)*(A4(I)**2)*(S4-A4(I))/(S4*S4)+FED                      00073500
      VLS4=P4(I)*(S4-A4(I))/S4+VLS4                                     00073600
 2070 VRS4=P4(I)*(A4(I))/S4+VRS4                                        00073700
 2080 IF (S5) 2090,2220,2090                                            00073800
 2090 READ(INUNIT,END=2240,ERR=3550,FMT=*)ICHK                          00073900
      BACKSPACE INUNIT                                                  00074000
 2120 IF (ICHK) 500,2130,500                                            00074100
 2130 READ(INUNIT,END=2170,ERR=3550,FMT=*)ICHK,                         00074200
     1     (A5(LL),LL=1,10),(P5(LL),LL=1,10)                            00074300
      GO TO 2190                                                        00074400
 2170 ISW=ICHK                                                          00074500
      GO TO 70                                                          00074600
 2190 DO 2210 I=1,10                                                    00074700
      IF (P5(I)) 2200,2220,2200                                         00074800
 2200 FEF=P5(I)*(S5-A5(I))**2*A5(I)/(S5*S5)+FEF                         00074900
      FFE=-P5(I)*(A5(I)**2)*(S5-A5(I))/(S5*S5)+FFE                      00075000
      VLS5=P5(I)*(S5-A5(I))/S5+VLS5                                     00075100
 2210 VRS5=P5(I)*(A5(I))/S5+VRS5                                        00075200
 2220 READ(INUNIT,END=2240,ERR=3550,FMT=*)ICHK                          00075300
      BACKSPACE INUNIT                                                  00075400
      GO TO 500                                                         00075500
 2240 IEND=1                                                            00075600
      GO TO 500                                                         00075700
C                                                                       00075800
C    COMPUTE CONCENTRATED LOAD MOMENTS AND SHEARS                       00075900
C                                                                       00076000
C    CONTINUES CALCULATIONS AND BEGINS WRITING RESULTS OF CONCENTRATED  00076100
C    LOAD PROBLEM.                                                      00076200
C                                                                       00076300
 2250 IF (CR) 2270,2270,2260                                            00076400
 2260 CR=-CR                                                            00076500
 2270 ICNT=ICNT+5                                                       00076600
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00076700
      WRITE (OUNIT,2280) NO2                                            00076800
 2280 FORMAT(/' PROBLEM NO.',I2)                                        00076900
C                                                                       00077000
      IF (PL) 2300,2290,2300                                            00077100
 2290 IF (PR) 2300,2320,2300                                            00077200
C                                                                       00077300
 2300 ICNT=ICNT+2                                                       00077400
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00077500
      WRITE (OUNIT,2310) PL,AL,PR,AR                                    00077600
 2310 FORMAT(/' CANTILEVER LOADS   LEFT LOAD =',F6.2,'   DIST =',F6.2,  00077700
     1                        '   RIGHT LOAD =',F6.2,'   DIST =',F6.2)  00077800
C                                                                       00077900
 2320 IF (S1) 2330,2350,2330                                            00078000
 2330 IF (P1(1)) 2340,2350,2340                                         00078100
 2340 ICNT=ICNT+4                                                       00078200
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00078300
      ISPAN=1                                                           00078400
      WRITE (OUNIT,2470) ISPAN,(A1(II), II=1,10),(P1(II), II=1,10)      00078500
C                                                                       00078600
 2350 IF (S2) 2360,2380,2360                                            00078700
 2360 IF (P2(1)) 2370,2380,2370                                         00078800
 2370 ICNT=ICNT+4                                                       00078900
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00079000
      ISPAN=2                                                           00079100
      WRITE (OUNIT,2470) ISPAN,(A2(II), II=1,10),(P2(II), II=1,10)      00079200
C                                                                       00079300
 2380 IF (S3) 2390,2410,2390                                            00079400
 2390 IF (P3(1)) 2400,2410,2400                                         00079500
 2400 ICNT=ICNT+4                                                       00079600
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00079700
      ISPAN=3                                                           00079800
      WRITE (OUNIT,2470) ISPAN,(A3(II), II=1,10),(P3(II), II=1,10)      00079900
C                                                                       00080000
 2410 IF (S4) 2420,2440,2420                                            00080100
 2420 IF (P4(1)) 2430,2440,2430                                         00080200
 2430 ICNT=ICNT+4                                                       00080300
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00080400
      ISPAN=4                                                           00080500
      WRITE (OUNIT,2470) ISPAN,(A4(II), II=1,10),(P4(II), II=1,10)      00080600
C                                                                       00080700
 2440 IF (S5) 2450,2480,2450                                            00080800
 2450 IF (P5(1)) 2460,2480,2460                                         00080900
 2460 ICNT=ICNT+4                                                       00081000
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00081100
      ISPAN=5                                                           00081200
      WRITE (OUNIT,2470) ISPAN,(A5(II), II=1,10),(P5(II), II=1,10)      00081300
C                                                                       00081400
 2470 FORMAT(/' SPAN',I2,'  LOAD 1  LOAD 2  LOAD 3  LOAD 4  LOAD 5',    00081500
     1                   '  LOAD 6  LOAD 7  LOAD 8  LOAD 9  LOAD 10'    00081600
     2       /' DISTANCE ',F6.2,9(2X,F6.2)                              00081700
     3       /' LOADS P  ',F6.2,9(2X,F6.2))                             00081800
C                                                                       00081900
 2480 IF (PL.EQ.0.) GO TO 2500                                          00082000
      ICNT=ICNT+2                                                       00082100
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00082200
      WRITE (OUNIT,2490) CL                                             00082300
 2490 FORMAT(/' CANT MOM LEFT  = ',F7.1)                                00082400
      IF (K.EQ.0) GO TO 2500                                            00082500
      WRITE (7,700) X00,X00,CL                                          00082600
      XL1=XL-AL                                                         00082700
      WRITE (7,710) X00,XL1,XL                                          00082800
 2500 IF (PR.EQ.0) GO TO 2520                                           00082900
      ICNT=ICNT+2                                                       00083000
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00083100
      WRITE (OUNIT,2510) CR                                             00083200
 2510 FORMAT(/' CANT MOM RIGHT = ',F7.1)                                00083300
 2520 IF (CL-AXNEG) 2530,2540,2540                                      00083400
 2530 AXNEG=CL                                                          00083500
      GO TO 2560                                                        00083600
 2540 IF (CR-AXNEG) 2550,2560,2560                                      00083700
 2550 AXNEG=CR                                                          00083800
 2560 VLS1=VLS1+(FAB+FBA)/S1                                            00083900
      VRS1=VRS1+(-FAB-FBA)/S1                                           00084000
      VRS1=-VRS1                                                        00084100
      FAB=-FAB                                                          00084200
      ICNT=ICNT+3                                                       00084300
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00084400
      WRITE (OUNIT,2570)                                                00084500
 2570 FORMAT(/' SPAN 1     V LT     V RT     M LT     M RT')            00084600
      WRITE (OUNIT,810) VLS1,VRS1,FAB,FBA                               00084700
      NOPL = 0                                                          00084800
      DO 2575 I=1,10                                                    00084900
          IF (P1(I) .GT. 0.0) NOPL = NOPL + 1                           00085000
 2575 CONTINUE                                                          00085100
      WRITE (24) FAB, FBA, VLS1, VRS1, NOPL                             00085200
      IF (NOPL .GT. 0) WRITE (24) (A1(I),I=1,10), (P1(I),I=1,10)        00085300
      AO=0.                                                             00085400
      DO 2580 I=1,10                                                    00085500
 2580 B(I)=0.                                                           00085600
      BO=FAB                                                            00085700
      IF (P1(1)) 2590,2710,2590                                         00085800
 2590 DO 2610 I=1,10                                                    00085900
      IF (P1(I)) 2600,2620,2600                                         00086000
 2600 B(I)=BO+VLS1*(A1(I)-AO)                                           00086100
      NSP=I                                                             00086200
      VLS1=VLS1-(P1(I))                                                 00086300
      AO=A1(I)                                                          00086400
 2610 BO=B(I)                                                           00086500
 2620 IF (K.NE.1) GO TO 2630                                            00086600
      NSP1=NSP+2                                                        00086700
      WRITE (7,840) NSP1,FAB,(B(LL),LL=1,NSP),FBA                       00086800
      WRITE (7,850) X00,(A1(LL),LL=1,NSP),S1                            00086900
 2630 ICNT=ICNT+4                                                       00087000
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00087100
 2650 WRITE (OUNIT,2660) (NW(LL),LL=1,NSP)                              00087200
 2660 FORMAT(/' AT LD PTS.',2X,10(A3,6X))                               00087300
      WRITE (OUNIT,2670) (B(LL),LL=1,NSP)                               00087400
 2670 FORMAT(/8X,10F9.1)                                                00087500
 2680 AMOM1=FAB                                                         00087600
      AMOM2=FBA                                                         00087700
      ISW10=6                                                           00087800
 2690 DO 2700 LL=1,10                                                   00087900
 2700 X(LL)=B(LL)                                                       00088000
      NX=1                                                              00088100
      GO TO 3240                                                        00088200
 2710 IF (K.EQ.0) GO TO 2720                                            00088300
      WRITE (7,840) N22,FAB,FBA                                         00088400
      WRITE (7,850) X00,S1                                              00088500
 2720 IF (S2) 2730,3160,2730                                            00088600
 2730 VLS2=VLS2+(FBC+FCB)/S2                                            00088700
      VRS2=VRS2+(-FBC-FCB)/S2                                           00088800
      VRS2=-VRS2                                                        00088900
      FBC=-FBC                                                          00089000
      ICNT=ICNT+3                                                       00089100
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00089200
      WRITE (OUNIT,970)                                                 00089300
      WRITE (OUNIT,810) VLS2,VRS2,FBC,FCB                               00089400
      NOPL = 0                                                          00089500
      DO 2735 I=1,10                                                    00089600
          IF (P2(I) .GT. 0.0) NOPL = NOPL + 1                           00089700
 2735 CONTINUE                                                          00089800
      WRITE (24) FBC, FCB, VLS2, VRS2, NOPL                             00089900
      IF (NOPL .GT. 0) WRITE (24) (A2(I),I=1,10), (P2(I),I=1,10)        00090000
      IF (P2(1)) 2740,2820,2740                                         00090100
 2740 DO 2750 I=1,10                                                    00090200
 2750 B(I)=0.                                                           00090300
      BO=FBC                                                            00090400
      AO=0.                                                             00090500
      DO 2770 I=1,10                                                    00090600
      IF (P2(I)) 2760,2780,2760                                         00090700
 2760 B(I)=BO+VLS2*(A2(I)-AO)                                           00090800
      NSP=I                                                             00090900
      VLS2=VLS2-(P2(I))                                                 00091000
      AO=A2(I)                                                          00091100
 2770 BO=B(I)                                                           00091200
 2780 IF (K.NE.1) GO TO 2790                                            00091300
      NSP1=NSP+2                                                        00091400
      WRITE (7,840) NSP1,FBC,(B(LL),LL=1,NSP),FCB                       00091500
      WRITE (7,850) X00,(A2(LL),LL=1,NSP),S2                            00091600
 2790 ICNT=ICNT+3                                                       00091700
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00091800
 2800 WRITE (OUNIT,2660) (NW(LL),LL=1,NSP)                              00091900
      WRITE (OUNIT,2670) (B(LL),LL=1,NSP)                               00092000
 2810 AMOM1=FBC                                                         00092100
      AMOM2=FCB                                                         00092200
      ISW10=7                                                           00092300
      GO TO 2690                                                        00092400
 2820 IF (K.EQ.0) GO TO 2830                                            00092500
      WRITE (7,840) N22,FBC,FCB                                         00092600
      WRITE (7,850) X00,S2                                              00092700
 2830 IF (S3) 2840,3160,2840                                            00092800
 2840 VLS3=VLS3+(FCD+FDC)/S3                                            00092900
      VRS3=VRS3+(-FCD-FDC)/S3                                           00093000
      VRS3=-VRS3                                                        00093100
      FCD=-FCD                                                          00093200
      ICNT=ICNT+3                                                       00093300
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00093400
      WRITE (OUNIT,1070)                                                00093500
      WRITE (OUNIT,810) VLS3,VRS3,FCD,FDC                               00093600
      NOPL = 0                                                          00093700
      DO 2845 I=1,10                                                    00093800
          IF (P3(I) .GT. 0.0) NOPL = NOPL + 1                           00093900
 2845 CONTINUE                                                          00094000
      WRITE (24) FCD, FDC, VLS3, VRS3, NOPL                             00094100
      IF (NOPL .GT. 0) WRITE (24) (A3(I),I=1,10), (P3(I),I=1,10)        00094200
      IF (P3(1)) 2850,2930,2850                                         00094300
 2850 DO 2860 I=1,10                                                    00094400
 2860 B(I)=0.                                                           00094500
      BO=FCD                                                            00094600
      AO=0.                                                             00094700
      DO 2880 I=1,10                                                    00094800
      IF (P3(I)) 2870,2890,2870                                         00094900
 2870 B(I)=BO+VLS3*(A3(I)-AO)                                           00095000
      NSP=I                                                             00095100
      VLS3=VLS3-P3(I)                                                   00095200
      AO=A3(I)                                                          00095300
 2880 BO=B(I)                                                           00095400
 2890 IF (K.NE.1) GO TO 2900                                            00095500
      NSP1=NSP+2                                                        00095600
      WRITE (7,840) NSP1,FCD,(B(LL),LL=1,NSP),FDC                       00095700
      WRITE (7,850) X00,(A3(LL),LL=1,NSP),S3                            00095800
 2900 ICNT=ICNT+3                                                       00095900
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00096000
 2910 WRITE (OUNIT,2660) (NW(LL),LL=1,NSP)                              00096100
      WRITE (OUNIT,2670) (B(LL),LL=1,NSP)                               00096200
 2920 AMOM1=FCD                                                         00096300
      AMOM2=FDC                                                         00096400
      ISW10=8                                                           00096500
      GO TO 2690                                                        00096600
 2930 IF (K.EQ.0) GO TO 2940                                            00096700
      WRITE (7,840) N22,FCD,FDC                                         00096800
      WRITE (7,850) X00,S3                                              00096900
 2940 IF (S4) 2950,3160,2950                                            00097000
 2950 VLS4=VLS4+(FDE+FED)/S4                                            00097100
      VRS4=VRS4+(-FDE-FED)/S4                                           00097200
      VRS4=-VRS4                                                        00097300
      FDE=-FDE                                                          00097400
      ICNT=ICNT+3                                                       00097500
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00097600
      WRITE (OUNIT,1170)                                                00097700
      WRITE (OUNIT,810) VLS4,VRS4,FDE,FED                               00097800
      NOPL = 0                                                          00097900
      DO 2955 I=1,10                                                    00098000
          IF (P4(I) .GT. 0.0) NOPL = NOPL + 1                           00098100
 2955 CONTINUE                                                          00098200
      WRITE (24) FDE, FED, VLS4, VRS4, NOPL                             00098300
      IF (NOPL .GT. 0) WRITE (24) (A4(I),I=1,10), (P4(I),I=1,10)        00098400
      IF (P4(1)) 2960,3040,2960                                         00098500
 2960 DO 2970 I=1,10                                                    00098600
 2970 B(I)=0.                                                           00098700
      BO=FDE                                                            00098800
      AO=0.                                                             00098900
      DO 2990 I=1,10                                                    00099000
      IF (P4(I)) 2980,3000,2980                                         00099100
 2980 B(I)=BO+VLS4*(A4(I)-AO)                                           00099200
      NSP=I                                                             00099300
      VLS4=VLS4-P4(I)                                                   00099400
      AO=A4(I)                                                          00099500
 2990 BO=B(I)                                                           00099600
 3000 IF (K.NE.1) GO TO 3010                                            00099700
      NSP1=NSP+2                                                        00099800
      WRITE (7,840) NSP1,FDE,(B(LL),LL=1,NSP),FED                       00099900
      WRITE (7,850) X00,(A4(LL),LL=1,NSP),S4                            00100000
 3010 ICNT=ICNT+3                                                       00100100
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00100200
 3020 WRITE (OUNIT,2660) (NW(LL),LL=1,NSP)                              00100300
      WRITE (OUNIT,2670) (B(LL),LL=1,NSP)                               00100400
 3030 AMOM1=FDE                                                         00100500
      AMOM2=FED                                                         00100600
      ISW10=9                                                           00100700
      GO TO 2690                                                        00100800
 3040 IF (K.EQ.0) GO TO 3050                                            00100900
      WRITE (7,840) N22,FDE,FED                                         00101000
      WRITE (7,850) X00,S4                                              00101100
 3050 IF (S5) 3060,3160,3060                                            00101200
 3060 VLS5=VLS5+(FEF+FFE)/S5                                            00101300
      VRS5=VRS5+(-FEF-FFE)/S5                                           00101400
      VRS5=-VRS5                                                        00101500
      FEF=-FEF                                                          00101600
      ICNT=ICNT+3                                                       00101700
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00101800
      WRITE (OUNIT,1270)                                                00101900
      WRITE (OUNIT,810) VLS5,VRS5,FEF,FFE                               00102000
      NOPL = 0                                                          00102100
      DO 3065 I=1,10                                                    00102200
          IF (P5(I) .GT. 0.0) NOPL = NOPL + 1                           00102300
 3065 CONTINUE                                                          00102400
      WRITE (24) FEF, FFE, VLS5, VRS5, NOPL                             00102500
      IF (NOPL .GT. 0) WRITE (24) (A5(I),I=1,10), (P5(I),I=1,10)        00102600
      IF (P5(1)) 3070,3150,3070                                         00102700
 3070 DO 3080 I=1,10                                                    00102800
 3080 B(I)=0.                                                           00102900
      BO=FEF                                                            00103000
      AO=0.                                                             00103100
      DO 3100 I=1,10                                                    00103200
      IF (P5(I)) 3090,3110,3090                                         00103300
 3090 B(I)=BO+VLS5*(A5(I)-AO)                                           00103400
      NSP=I                                                             00103500
      VLS5=VLS5-P5(I)                                                   00103600
      AO=A5(I)                                                          00103700
 3100 BO=B(I)                                                           00103800
 3110 IF (K.NE.1) GO TO 3120                                            00103900
      NSP1=NSP+2                                                        00104000
      WRITE (7,840) NSP1,FEF,(B(LL),LL=1,NSP),FFE                       00104100
      WRITE (7,850) X00,(A5(LL),LL=1,NSP),S5                            00104200
 3120 ICNT=ICNT+3                                                       00104300
      CALL NEWPG(REVDAT,DATE,TIME,IDEN)                                 00104400
 3130 WRITE (OUNIT,2660) (NW(LL),LL=1,NSP)                              00104500
      WRITE (OUNIT,2670) (B(LL),LL=1,NSP)                               00104600
 3140 AMOM1=FEF                                                         00104700
      AMOM2=FFE                                                         00104800
      ISW10=10                                                          00104900
      GO TO 2690                                                        00105000
 3150 IF (K.EQ.0) GO TO 3160                                            00105100
      WRITE (7,840) N22,FEF,FFE                                         00105200
      WRITE (7,850) X00,S5                                              00105300
 3160 IF (K.EQ.0) GO TO 3170                                            00105400
      IF (XR.EQ.0.) GO TO 3170                                          00105500
      WRITE (7,700) CR,X00,X00                                          00105600
      WRITE (7,710) X00,AR,XR                                           00105700
C                                                                       00105800
C                                                                       00105900
 3170 CONTINUE                                                          00106000
 3180 IF (IEND.EQ.1) GO TO 3440                                         00106100
C                                                                       00106200
C    CHECKS FOR ANOTHER CONCENTRATED LOAD PROBLEM.                      00106300
C                                                                       00106400
      IF (ICHK-5) 3210,3190,3210                                        00106500
 3190 JSW=1                                                             00106600
      KEY=1                                                             00106700
      READ(INUNIT,END=3530,ERR=3550,FMT=*)ICHK,                         00106800
     1                 ALT,PLT,ART,PRT,NO2                              00106900
      GO TO 370                                                         00107000
 3210 IF (ICHK-4) 3230,3220,3230                                        00107100
 3220 JSW=1                                                             00107200
      GO TO 350                                                         00107300
 3230 IF (ICHK-1) 70,1400,70                                            00107400
 3240 IF (AMOM1) 3270,3290,3250                                         00107500
 3250 IF (AMOM1-AXPOS) 3290,3290,3260                                   00107600
 3260 AXPOS=AMOM1                                                       00107700
      GO TO 3290                                                        00107800
 3270 IF (AMOM1-AXNEG) 3280,3290,3290                                   00107900
 3280 AXNEG=AMOM1                                                       00108000
 3290 IF (AMOM2) 3320,3340,3300                                         00108100
 3300 IF (AMOM2-AXPOS) 3340,3340,3310                                   00108200
 3310 AXPOS=AMOM2                                                       00108300
      GO TO 3340                                                        00108400
 3320 IF (AMOM2-AXNEG) 3330,3340,3340                                   00108500
 3330 AXNEG=AMOM2                                                       00108600
 3340 DO 3390 LL=NX,10                                                  00108700
      IF (X(LL)) 3370,3390,3350                                         00108800
 3350 IF (X(LL)-AXPOS) 3390,3390,3360                                   00108900
 3360 AXPOS=X(LL)                                                       00109000
      GO TO 3390                                                        00109100
 3370 IF (X(LL)-AXNEG) 3380,3390,3390                                   00109200
 3380 AXNEG=X(LL)                                                       00109300
 3390 CONTINUE                                                          00109400
      GO TO (950,1050,1150,1250,1350,2720,2830,2940,3050,3160),ISW10    00109500
 3400 IF (CL-AXNEG) 3410,3420,3420                                      00109600
 3410 AXNEG=CL                                                          00109700
 3420 IF (CR-AXNEG) 3430,790,790                                        00109800
 3430 AXNEG=CR                                                          00109900
      GO TO 790                                                         00110000
 3440 IF (K.NE.1) GO TO 3450                                            00110100
      WRITE (8,1430) NMO,(IMO(I),I=1,NMO)                               00110200
      CALL BENTPL                                                       00110300
      CALL PLOT(0.,0.,999)                                              00110400
 3450 IF (K.EQ.0) GO TO 3510                                            00110500
      IF (SCAY.GT.34.) GO TO 3510                                       00110600
      N11=11                                                            00110700
      N34=34                                                            00110800
      IF (OUNIT.EQ.9) WRITE (OUNIT,3460)                                00110900
C                                                                       00111000
C    CARCNT - CARRIAGE CONTROL CHARACTER                                00111100
C                                                                       00111200
      WRITE (3,3460) CARCNT                                             00111300
 3460 FORMAT(A1//' **********'///' PLOTTING REQUESTED WITH THIS OUTPUT' 00111400
     1        ///' **********'//)                                       00111500
      IF (SCAY.GT.9.) GO TO 3480                                        00111600
      WRITE (3,3470) N11                                                00111700
 3470 FORMAT (/'      USE ',I2,' INCH WIDE PAPER',                      00111800
     1      ' (CROSS SECTION UNLESS OTHERWISE SPECIFIED)')              00111900
      GO TO 3490                                                        00112000
 3480 WRITE (OUNIT,3470) N34                                            00112100
 3490 WRITE (3,3500)                                                    00112200
 3500 FORMAT(/'       START PEN AT THE LOWEST LINE OF GRID',            00112300
     1              ' (OR .5 INCH ABOVE LOWER EDGE OF PAPER)'//)        00112400
 3510 WRITE (OUNIT,3520)                                                00112500
 3520 FORMAT(/' PROBLEM COMPLETED')                                     00112600
      REWIND 24                                                         00112700
      CALL BENT2 (NP, NS, REVDAT, DATE, TIME, OUNIT)                    00112800
      GO TO 3560                                                        00112900
 3530 WRITE (*,3540)                                                    00113000
      WRITE ( OUNIT,3540)                                               00113100
 3540 FORMAT(//' INSUFFICIENT DATA - TERMINATING RUN ')                 00113200
      STOP                                                              00113300
 3550 WRITE (*,3555)                                                    00113400
      WRITE ( OUNIT,3555)                                               00113500
 3555 FORMAT(//' INPUT ERROR!! CHECK YOUR DATA SET!! ')                 00113600
 3560 STOP                                                              00113700
      END                                                               00113800
C BENTANA P63060 SUBROUTINES                                            00113900
      SUBROUTINE NEWPG(REVDAT,DATE,TIME,IDEN)                           00114000
      CHARACTER*1 CARCNT                                                00114100
      CHARACTER*8 REVDAT,TIME                                           0T114200
      CHARACTER*11 DATE
      INTEGER*4 OUNIT                                                   00114300
      COMMON /SPAC1/ OUNIT                                              00114400
      COMMON /SPAC2/ CARCNT                                             00114500
      COMMON IEND,IPAGE,ICNT,SCAY                                       00114600
      IF (ICNT-50) 80,10,10                                             00114700
   10 IPAGE=IPAGE+1                                                     00114800
      IF (IPAGE.GT.1) GO TO 50                                          00114900
C                                                                       00115000
C                                                                       00115100
 1000 FORMAT(A1,//,' WASHINGTON STATE DEPT. OF TRANSPORTATION  ',       0T115200
     1  A11,'  TIME ',A8)                                               0T115300
 1001 FORMAT(' *** BENT ANALYSIS ***   '                                00115400
     1,4X,'REV ',A8,'  PROBLEM NO. ',I4,/)                              00115500
   30  WRITE(OUNIT,1000)CARCNT,DATE,TIME                                00115600
       WRITE(OUNIT,1001)REVDAT,IDEN                                     00115700
C                                                                       00115800
      GO TO 70                                                          00115900
C                                                                       00116000
   50 WRITE (OUNIT,60) CARCNT                                           00116100
   60 FORMAT (A1//)                                                     00116200
   70 ICNT=0                                                            00116300
   80 RETURN                                                            00116400
      END                                                               00116500
C                                                                       00116600
C                                                                       00116700
C                                                                       00116800
C                                                                       00116900
C                                                                       00117000
C                                                                       00117100
      SUBROUTINE BENTPL                                                 00117200
C                                                                       00117300
      INTEGER*4 OUNIT                                                   00117400
      CHARACTER*1 CARCNT                                                00117500
      DIMENSION SMAX(5),SSX(7),SPN(7),SPAN(5),IMO(20)                   00117600
      DIMENSION XS(600),YM(600),YMM(600),XMM(600),IN(20,5)              00117700
      COMMON IEND,IPAGE,ICNT,SCAY,IHH,DY1                               00117800
      COMMON /SPAC1/ OUNIT                                              00117900
      COMMON /SPAC2/ CARCNT                                             00118000
      DATA IZ0,IZ1,IZ2,IZ3,IZ4,IZ5,IZ6/0,1,2,3,4,5,6/                   00118100
      REWIND 7                                                          00118200
      REWIND 8                                                          00118300
C                                                                       00118400
      READ (7,10,END=700) IDEN,SLCANT,SPAN,SRCANT,MARK,SCAX,SCAY        00118500
   10 FORMAT(A4,7F7.2,I1,2F3.0)                                         00118600
      DO 20 J=1,5                                                       00118700
      IF (SPAN(J)) 30,30,20                                             00118800
   20 NOSPNS=J                                                          00118900
   30 READ (8,40,END=700) NMO,(IMO(I),I=1,NMO)                          00119000
   40 FORMAT(I3,20I1)                                                   00119100
C                                                                       00119200
      DO 50 I=1,600                                                     00119300
      YMM(I)=0.                                                         00119400
   50 XMM(I)=0.                                                         00119500
      M=4                                                               00119600
      IF (SLCANT.EQ.0.) GO TO 60                                        00119700
      M=1                                                               00119800
C                                                                       00119900
   60 DO 150 J=1,NMO                                                    00120000
      TSPAN=0.                                                          00120100
      IF (SLCANT.EQ.0.) GO TO 90                                        00120200
      L=M+1                                                             00120300
      M=L+2                                                             00120400
      READ (7,70) (YMM(I),I=L,M)                                        00120500
   70 FORMAT(3F7.1)                                                     00120600
      READ (7,80) (XMM(I),I=L,M)                                        00120700
   80 FORMAT(3F7.2)                                                     00120800
      TSPAN=XMM(M)                                                      00120900
C                                                                       00121000
   90 DO 130 N=1,NOSPNS                                                 00121100
      READ (7,100) NLD,(YM(I),I=1,NLD)                                  00121200
  100 FORMAT(I2,12F7.1)                                                 00121300
      READ (7,110) (XS(I),I=1,NLD)                                      00121400
  110 FORMAT(2X,12F7.2)                                                 00121500
      L=M+1                                                             00121600
      M=L+NLD-1                                                         00121700
      I0=0                                                              00121800
      DO 120 I=L,M                                                      00121900
      I0=I0+1                                                           00122000
      YMM(I)=YM(I0)                                                     00122100
  120 XMM(I)=TSPAN+XS(I0)                                               00122200
      TSPAN=XMM(M)                                                      00122300
  130 IN(J,N)=NLD                                                       00122400
C                                                                       00122500
      IF (SRCANT.EQ.0.) GO TO 150                                       00122600
      L=M+1                                                             00122700
      M=L+2                                                             00122800
      READ (7,70) (YMM(I),I=L,M)                                        00122900
      READ (7,80) (XS(I),I=L,M)                                         00123000
      DO 140 I=L,M                                                      00123100
  140 XMM(I)=TSPAN+XS(I)                                                00123200
      TSPAN=XMM(M)                                                      00123300
  150 CONTINUE                                                          00123400
C                                                                       00123500
      YMAX=YMM(1)                                                       00123600
      YMIN=YMM(1)                                                       00123700
      DO 160 I=2,M                                                      00123800
      YMAX=AMAX1(YMAX,YMM(I))                                           00123900
      YMIN=AMIN1(YMIN,YMM(I))                                           00124000
  160 CONTINUE                                                          00124100
      IF (SCAX.LE.5.) GO TO 170                                         00124200
      GO TO 180                                                         00124300
  170 SCAX=5.                                                           00124400
  180 IF (SCAY.LE.9.) GO TO 190                                         00124500
      GO TO 200                                                         00124600
  190 SCAY=9.                                                           00124700
  200 CONTINUE                                                          00124800
C                                                                       00124900
      YMIN=AINT(YMIN-.99)                                               00125000
      IF (DY1.EQ.0.) GO TO 210                                          00125100
      DY=DY1                                                            00125200
      GO TO 290                                                         00125300
  210 DY=(YMAX-YMIN)/SCAY                                               00125400
      DY=AINT(DY+.99)                                                   00125500
C                                                                       00125600
      IF (DY.GT.10.) GO TO 220                                          00125700
      DY=10.                                                            00125800
      GO TO 290                                                         00125900
  220 IF (DY.GT.20.) GO TO 230                                          00126000
      DY=20.                                                            00126100
      GO TO 290                                                         00126200
  230 IF (DY.GT.30.) GO TO 240                                          00126300
      DY=30.                                                            00126400
      GO TO 290                                                         00126500
  240 IF (DY.GT.40.) GO TO 250                                          00126600
      DY=40.                                                            00126700
      GO TO 290                                                         00126800
  250 IF (DY.GT.50.) GO TO 260                                          00126900
      DY=50.                                                            00127000
      GO TO 290                                                         00127100
  260 IF (DY.GT.60.) GO TO 270                                          00127200
      DY=60.                                                            00127300
      GO TO 290                                                         00127400
  270 IF (DY.GT.100.) GO TO 280                                         00127500
      DY=100.                                                           00127600
      GO TO 290                                                         00127700
  280 SCAY=SCAY+3.                                                      00127800
      GO TO 210                                                         00127900
  290 SCAY1=AINT((YMAX-YMIN)/(DY-.99)+.99)                              00128000
      IF (SCAY1.LT.9.) GO TO 300                                        00128100
      SCAY=SCAY1                                                        00128200
      IF (SCAY1.GT.34.) GO TO 310                                       00128300
  300 GO TO 330                                                         00128400
C                                                                       00128500
C     CARCNT - CARRIAGE CONTROL CHARACTER                               00128600
C                                                                       00128700
  310 WRITE (OUNIT,320) CARCNT,DY                                       00128800
  320 FORMAT(A1//' SCALE Y =',F5.0,' WILL PLOT BEYOND A 34 IN. PAPER')  00128900
      GO TO 700                                                         00129000
C                                                                       00129100
  330 XMIN=SPAN(1)                                                      00129200
      I=1                                                               00129300
      IF (NOSPNS.EQ.1) GO TO 345                                        00129400
      DO 340 I=2,NOSPNS                                                 00129500
  340 XMIN=AMIN1(XMIN,SPAN(I))                                          00129600
  345 CONTINUE                                                          00129700
      DX=XMIN/SCAX                                                      00129800
      DX=AINT(DX+.99)                                                   00129900
C                                                                       00130000
      DO 350 I=1,M                                                      00130100
      XS(I)=XMM(I)/DX                                                   00130200
  350 YM(I)=YMM(I)/DY                                                   00130300
C                                                                       00130400
      YOR=(YMM(1)-YMIN)/DY                                              00130500
      YO=AINT(YOR*10.+.99)                                              00130600
      YOR=YO/10.                                                        00130700
      XOR=3.                                                            00130800
C                                                                       00130900
      CALL PLOT(0.,1.,3)                                                00131000
      CALL PLOT(1.,1.,2)                                                00131100
      CALL SYMBOL(0.,1.05,.14,'XXXX',0.,4)                              00131200
      CALL SYMBOL(0.,.7,.1,'SCALE X;= ',0.,10)                          00131300
      CALL NUMBER(999.,999.,.1,DX,0.,-1)                                00131400
      CALL SYMBOL(999.,999.,.1,' FEET PER INCH',0.,14)                  00131500
      CALL SYMBOL(0.,.4,.1,'SCALE Y:= ',0.,10)                          00131600
      CALL NUMBER(999.,999.,.1,DY,0.,-1)                                00131700
      CALL SYMBOL(999.,999.,.1,' KIP-INCH PER INCH',0.,18)              00131800
C                                                                       00131900
      CALL SYMBOL(0.,8.,.1,'MOMENTS:',0.,8)                             00132000
      IF (NMO.GE.2) GO TO 360                                           00132100
      NM1=NMO                                                           00132200
      GO TO 370                                                         00132300
  360 NM1=2                                                             00132400
  370 DO 440 I=1,NM1                                                    00132500
      GO TO (380,390,400),I                                             00132600
  380 Z=7.6                                                             00132700
      GO TO 410                                                         00132800
  390 Z=7.2                                                             00132900
      GO TO 410                                                         00133000
  400 Z=6.8                                                             00133100
  410 IF (IMO(I)-2) 420,430,430                                         00133200
  420 CALL SYMBOL(.3,Z,.1,'---UNIF  LOAD  MOMENT',0.,21)                00133300
      GO TO 440                                                         00133400
  430 CALL SYMBOL(.3,Z,.1,'---CONC  LOAD  MOMENT',0.,21)                00133500
  440 CONTINUE                                                          00133600
C                                                                       00133700
      HI1=SCAY+.7                                                       00133800
      HI22=SCAY+.65                                                     00133900
      CALL SYMBOL(0.,HI1,.14,'BENTANA.PLOT',0.,12)                      00134000
      CALL PLOT(0.,HI22,3)                                              00134100
      CALL PLOT(2.95,HI22,2)                                            00134200
      CALL SYMBOL(3.0,HI22,.1,'WASHINGTON DEPT. OF TRANSPORTATION',     0T134300
     1     0.,39)                                                       00134400
C                                                                       00134500
      CALL PLOT(XOR,YOR,23)                                             00134600
C                                                                       00134700
C    PLOT03                                                             00134800
C                                                                       00134900
      K=5                                                               00135000
      IF (SLCANT.EQ.0.) GO TO 450                                       00135100
      K=2                                                               00135200
  450 DO 650 J=1,NMO                                                    00135300
      IR=0                                                              00135400
      IF (SLCANT.EQ.0.) GO TO 520                                       00135500
C                                                                       00135600
C    WITH LEFT CANTILEVER                                               00135700
C                                                                       00135800
  460 X1=XS(K)                                                          00135900
      X2=XS(K+1)                                                        00136000
      X3=XS(K+2)                                                        00136100
      Y1=YM(K)                                                          00136200
      Y2=YM(K+1)                                                        00136300
      Y3=YM(K+2)                                                        00136400
      IF (IR.EQ.1) GO TO 620                                            00136500
  470 IF (IMO(J).EQ.1) GO TO 480                                        00136600
      CALL PLOT(X1,Y1,3)                                                00136700
      CALL PLOT(X2,Y2,2)                                                00136800
      CALL PLOT(X3,Y3,2)                                                00136900
      GO TO 490                                                         00137000
  480 C11=(Y1-Y2)*(X1-X3)-(Y1-Y3)*(X1-X2)                               00137100
      C12=(X1*X1-X2*X2)*(X1-X3)-(X1*X1-X3*X3)*(X1-X2)                   00137200
      C1=C11/C12                                                        00137300
      C2=((Y1-Y2)-C1*(X1*X1-X2*X2))/(X1-X2)                             00137400
      C3=Y1-C1*X1*X1-C2*X1                                              00137500
      CALL CURVEX(X1,X3,C1,2.,C2,1.,C3,0.,0.,0.)                        00137600
  490 CONTINUE                                                          00137700
C                                                                       00137800
      IF (IR.EQ.0) GO TO 500                                            00137900
      GO TO 630                                                         00138000
  500 X31=X3-.4                                                         00138100
      CY3=Y3*DY                                                         00138200
      IF (MARK.EQ.1) GO TO 510                                          00138300
      CALL NUMBER(X31,Y3,.07,CY3,0.,0)                                  00138400
  510 CALL PLOT(X3,Y3,3)                                                00138500
      CALL PLOT(X3,0.,2)                                                00138600
      K=K+3                                                             00138700
C                                                                       00138800
C    WITHOUT LEFT CANTILEVER                                            00138900
C                                                                       00139000
  520 CALL SYMBOL(XS(K),0.,.20,'003',0.,-1)                             00139100
C                                                                       00139200
      DO 610 I=1,NOSPNS                                                 00139300
      CALL PLOT(XS(K),YM(K),2)                                          00139400
      XL1=XS(K)+.1                                                      00139500
      CYM=YM(K)*DY                                                      00139600
      IF (MARK.EQ.1) GO TO 530                                          00139700
      CALL NUMBER(XL1,YM(K),.07,CYM,0.,0)                               00139800
  530 CALL PLOT(XS(K),YM(K),3)                                          00139900
      IF (IMO(J).EQ.1) GO TO 550                                        00140000
      N9=IN(J,I)-1                                                      00140100
      DO 540 N=1,N9                                                     00140200
      X1=XS(K)                                                          00140300
      X3=XS(K+1)                                                        00140400
      Y1=YM(K)                                                          00140500
      Y3=YM(K+1)                                                        00140600
      K=K+1                                                             00140700
      CALL PLOT(X1,Y1,3)                                                00140800
      CALL PLOT(X3,Y3,2)                                                00140900
      IF (N9.EQ.N) GO TO 580                                            00141000
      Y31=Y3+.1                                                         00141100
      CY=Y3*DY                                                          00141200
      IF (MARK.EQ.1) GO TO 540                                          00141300
      CALL NUMBER(X3,Y31,.07,CY,0.,0)                                   00141400
  540 CONTINUE                                                          00141500
      GO TO 580                                                         00141600
  550 CONTINUE                                                          00141700
C                                                                       00141800
      IF (IN(J,I).EQ.2) GO TO 560                                       00141900
      X1=XS(K)                                                          00142000
      X2=XS(K+1)                                                        00142100
      X3=XS(K+2)                                                        00142200
      Y1=YM(K)                                                          00142300
      Y2=YM(K+1)                                                        00142400
      Y3=YM(K+2)                                                        00142500
      K=K+2                                                             00142600
      C11=(Y1-Y2)*(X1-X3)-(Y1-Y3)*(X1-X2)                               00142700
      C12=(X1*X1-X2*X2)*(X1-X3)-(X1*X1-X3*X3)*(X1-X2)                   00142800
      C1=C11/C12                                                        00142900
      C2=((Y1-Y2)-C1*(X1*X1-X2*X2))/(X1-X2)                             00143000
      C3=Y1-C1*X1*X1-C2*X1                                              00143100
      CALL CURVEX(X1,X3,C1,2.,C2,1.,C3,0.,0.,0.)                        00143200
C                                                                       00143300
      SMAX(I)=-1.*C2/(C1*2.)                                            00143400
      IF (SMAX(I).LE.X1) GO TO 570                                      00143500
      IF (SMAX(I).GT.X3) GO TO 570                                      00143600
      YMAX=C1*SMAX(I)**2.+C2*SMAX(I)+C3                                 00143700
      CALL SYMBOL(SMAX(I),YMAX,.07,'014',0.,-1)                         00143800
      YMMAX=YMAX+.1                                                     00143900
      CYMAX=YMAX*DY                                                     00144000
      IF (MARK.EQ.1) GO TO 570                                          00144100
      CALL NUMBER(SMAX(I),YMMAX,.07,CYMAX,0.,0)                         00144200
      GO TO 570                                                         00144300
  560 X1=XS(K)                                                          00144400
      X3=XS(K+1)                                                        00144500
      Y1=YM(K)                                                          00144600
      Y3=YM(K+1)                                                        00144700
      CALL PLOT(X1,Y1,3)                                                00144800
      CALL PLOT(X3,Y3,2)                                                00144900
      K=K+1                                                             00145000
  570 CONTINUE                                                          00145100
C                                                                       00145200
  580 X31=X3-.4                                                         00145300
      CY3=Y3*DY                                                         00145400
      IF (MARK.EQ.1) GO TO 590                                          00145500
      CALL NUMBER(X31,Y3,.07,CY3,0.,0)                                  00145600
  590 CALL PLOT(X3,Y3,3)                                                00145700
      CALL PLOT(X3,0.,2)                                                00145800
      IF (J-3) 600,600,610                                              00145900
  600 CALL SYMBOL(X3,0.,.20,'003',0.,-1)                                00146000
  610 K=K+1                                                             00146100
C                                                                       00146200
      IF (SRCANT.EQ.0.) GO TO 640                                       00146300
      IR=1                                                              00146400
      GO TO 460                                                         00146500
  620 CALL PLOT(X1,Y1,2)                                                00146600
      X11=X1+.1                                                         00146700
      CYM=Y1*DY                                                         00146800
      IF (MARK.EQ.1) GO TO 470                                          00146900
      CALL NUMBER(X11,Y1,.07,CYM,0.,0)                                  00147000
      GO TO 470                                                         00147100
  630 K=K+3                                                             00147200
  640 CALL PLOT(0.,0.,2)                                                00147300
C                                                                       00147400
  650 CONTINUE                                                          00147500
C                                                                       00147600
      MS=0                                                              00147700
      IF (SLCANT.EQ.0.) GO TO 660                                       00147800
      MS=MS+1                                                           00147900
      SSX(MS)=SLCANT/DX                                                 00148000
      SPN(MS)=SLCANT                                                    00148100
  660 DO 670 I=1,NOSPNS                                                 00148200
      MS=MS+1                                                           00148300
      SPN(MS)=SPAN(I)                                                   00148400
  670 SSX(MS)=SPAN(I)/DX                                                00148500
      IF (SRCANT.EQ.0.) GO TO 680                                       00148600
      MS=MS+1                                                           00148700
      SPN(MS)=SRCANT                                                    00148800
      SSX(MS)=SRCANT/DX                                                 00148900
  680 CONTINUE                                                          00149000
      XIN=0.                                                            00149100
      DO 690 I=1,MS                                                     00149200
      DIMX=XIN+SSX(I)/2.                                                00149300
      XIN=XIN+SSX(I)                                                    00149400
      DIMX2=DIMX-.4                                                     00149500
      CALL NUMBER(DIMX2,-.2,.1,SPN(I),0.,2)                             00149600
      CALL SYMBOL(999.,999.,.1,' FEET',0.,5)                            00149700
  690 CONTINUE                                                          00149800
C                                                                       00149900
      TOT=TSPAN/DX+5.                                                   00150000
      CALL PLOT(-XOR,-YOR,23)                                           00150100
      CALL PLOT(-.5,0.,3)                                               00150200
      HI3=SCAY+1.                                                       00150300
      CALL PLOT(-.5,HI3,2)                                              00150400
      CALL PLOT(TOT,HI3,2)                                              00150500
      CALL PLOT(TOT,0.,2)                                               00150600
      CALL PLOT(-.5,0.,2)                                               00150700
      TOT2=TOT+5.                                                       00150800
      CALL PLOT(TOT2,0.,23)                                             00150900
C                                                                       00151000
  700 CONTINUE                                                          00151100
      RETURN                                                            00151200
      END                                                               00151300
C                                                                       00151400
C                                                                       00151500
C                                                                       00151600
C                                                                       00151700
C                                                                       00151800
C                                                                       00151900
C                                                                       00152000
C                                                                       00152100
      SUBROUTINE BENT2 (NP, NS, REVDAT, DATE, TIME, OUNIT)              00152200
C                                                                       00152300
      CHARACTER*8 REVDAT,TIME                                           0T152400
      CHARACTER*11 DATE                                                  T
      INTEGER*4 OUNIT                                                   00152500
C                                                                       00152600
      REAL*4 MT(11,5), VT(11,5), L(5), W(5), M(11), VMIN(11,5)          00152700
      REAL*4 A(10), P(10), V(11), M3, M4, MMAX(11,5), MMIN(11,5)        00152800
      REAL*4 VMAX(11,5), MPL, MU, MB                                    00152900
C                                                                       00153000
C                                                                       00153100
      NS = NS / 2                                                       00153200
C                                                                       00153300
      DO 4 J=1,5                                                        00153400
          DO 3 I=1,11                                                   00153500
              MMAX(I,J) = -210000.                                      00153600
              MMIN(I,J) = 210000.                                       00153700
              VMAX(I,J) = -210000.                                      00153800
              VMIN(I,J) = 210000.                                       00153900
              MT(I,J)   = 0.                                            00154000
              VT(I,J)   = 0.                                            00154100
3         CONTINUE                                                      00154200
4     CONTINUE                                                          00154300
C                                                                       00154400
      DO 5 I=1,NS                                                       00154500
          READ (24) MT(1,I), MT(11,I), VT(1,I), VT(11,I), L(I), W(I)    00154600
5     CONTINUE                                                          00154700
C                                                                       00154800
      DO 85 J=1,NP                                                      00154900
          DO 75 I=1,NS                                                  00155000
              READ (24) M3, M4, V3, V4, NPL                             00155100
              IF (NPL .GT. 0) READ (24) (A(K),K=1,10), (P(K),K=1,10)    00155200
              M(1)  = MT(1,I) + M3                                      00155300
              V(1)  = VT(1,I) + V3                                      00155400
              M(11) = MT(11,I) + M4                                     00155500
              V(11) = VT(11,I) + V4                                     00155600
C                                                                       00155700
              IF (J .NE. 1) GO TO 10                                    00155800
              MT(1,I)  = M(1)                                           00155900
              VT(1,I)  = V(1)                                           00156000
              MT(11,I) = M(11)                                          00156100
              VT(11,I) = V(11)                                          00156200
              M3       = M(1)                                           00156300
              V3       = V(1)                                           00156400
              M4       = M(11)                                          00156500
              V4       = V(11)                                          00156600
C                                                                       00156700
10            DO 55 LT=2,10                                             00156800
                  X  = (LT - 1) * L(I) / 10.                            00156900
                  MB = M3 + (M4 - M3) * X / L(I)                        00157000
                  VB = V3                                               00157100
C                                                                       00157200
                  IF (J .NE. 1) GO TO 11                                00157300
                  MU    = W(I) * X / 2. * (L(I) - X)                    00157400
                  VU    = -X * W(I)                                     00157500
                  M(LT) = MU + MB                                       00157600
                  V(LT) = VU + VB                                       00157700
                  GO TO 12                                              00157800
C                                                                       00157900
11                M(LT) = MT(LT,I) + MB                                 00158000
                  V(LT) = VT(LT,I) + VB                                 00158100
C                                                                       00158200
12                IF (NPL .EQ. 0) GO TO 50                              00158300
                  DO 45 K=1,NPL                                         00158400
                      IF (X-A(K)) 15,15,25                              00158500
15                    MPL = P(K) * X * (L(I) - A(K)) / L(I)             00158600
                      VPL = 0.                                          00158700
                      GO TO 35                                          00158800
C                                                                       00158900
25                    MPL   = P(K) * A(K) * (L(I) - X) / L(I)           00159000
                      VPL   = -P(K)                                     00159100
35                    M(LT) = M(LT) + MPL                               00159200
                      V(LT) = V(LT) + VPL                               00159300
45                CONTINUE                                              00159400
C                                                                       00159500
50                IF (J .NE. 1) GO TO 55                                00159600
                  MT(LT,I) = M(LT)                                      00159700
                  VT(LT,I) = V(LT)                                      00159800
55            CONTINUE                                                  00159900
C                                                                       00160000
              DO 60 LT=1,11                                             00160100
                  IF (MMAX(LT,I) .LT. M(LT)) MMAX(LT,I) = M(LT)         00160200
                  IF (MMIN(LT,I) .GT. M(LT)) MMIN(LT,I) = M(LT)         00160300
                  IF (VMAX(LT,I) .LT. V(LT)) VMAX(LT,I) = V(LT)         00160400
                  IF (VMIN(LT,I) .GT. V(LT)) VMIN(LT,I) = V(LT)         00160500
60            CONTINUE                                                  00160600
75        CONTINUE                                                      00160700
85    CONTINUE                                                          00160800
C                                                                       00160900
      WRITE (OUNIT,100) DATE, TIME                                      00161000
100   FORMAT('1',5X,'DATE: ',A11,16X,'WASHINGTON STATE DEPT. OF ',      0T161100
     1       'TRANSPORTATION',22X,A8)                                   00161200
      WRITE (OUNIT,110) REVDAT                                          00161300
110   FORMAT(' ',5X,'REV.: ',A8,19X,'***  BENT ANALYSIS MINIMUM',       00161400
     1       'S AND MAXIMUMS  ***')                                     00161500
C                                                                       00161600
      DO 150 I=1,NS                                                     00161700
      IF(I.NE.5)THEN                                                    00161800
          WRITE (OUNIT,120) I                                           00161900
          ELSE                                                          00162000
          WRITE (OUNIT,100) DATE, TIME                                  00162100
          WRITE (OUNIT,110) REVDAT                                      00162200
          WRITE (OUNIT,120) I                                           00162300
          END IF                                                        00162400
120       FORMAT(/,17X,'SPAN ',I1,4X,'LOCATION',10X,'MMAX',12X,         00162500
     1           'MMIN',12X,'VMAX',12X,'VMIN')                          00162600
C                                                                       00162700
          DO 140 LT=1,11                                                00162800
              XLT = (LT - 1) / 10.                                      00162900
              WRITE (OUNIT,130) XLT, MMAX(LT,I), MMIN(LT,I), VMAX(LT,I),00163000
     1                      VMIN(LT,I)                                  00163100
130           FORMAT(' ',23X,5(F10.2,6X))                               00163200
140       CONTINUE                                                      00163300
150   CONTINUE                                                          00163400
      IF(OUNIT.EQ.6)WRITE(OUNIT,*)'+'                                  00163500
CCCCCCCCCCCCCCCCCCC                                                     00163600
      RETURN                                                            00163700
      END                                                               00163800
C                                                                       00163900
C                                                                       00164000
C                                                                       00164100
C                                                                       00164200
C                                                                       00164300
C                                                                       00164400
      SUBROUTINE PARM(IMODE,OUNIT,ERUNIT,CCHAR)                         00164500
      CHARACTER*16 FLNM1, FLNM2                                         00164600
      CHARACTER*4 IMODE                                                 00164700
      LOGICAL*1 CCHAR                                                   00164800
      INTEGER OUNIT, ERUNIT                                             00164900
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00165000
C  STANDARD INTERACTIVE INPUT UNIT                                      00165100
      IIUNIT=1                                                          00165200
C  STANDARD INTERACTIVE OUTPUT UNIT                                     00165300
      IOUNIT=3                                                          00165400
C  STANDARD REPORT OUTPUT UNIT                                          00165500
      OUNIT=6                                                           00165600
C  STANDARD FILE INPUT UNIT                                             00165700
      INUNIT=5                                                          00165800
C  STANDARD ERROR OUTPUT UNIT                                           00165900
      ERUNIT=3                                                          00166000
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00166100
      IMODE='FREE'                                                      00166200
      CCHAR=.TRUE.                                                      00166300
C     OPEN(IIUNIT,FILE='CON')                                           00166400
C     OPEN(IOUNIT,FILE='CON')                                           00166500
10    WRITE(*,FMT='(//,'' Enter input file name:'',/)')                  T
           READ(*,ERR=10,FMT='(A)')FLNM1                                0T166900
           IF(FLNM1.EQ.'CON')GO TO 25                                   0T167000
           IF(FLNM1.EQ.'con')GO TO 25                                   0T167100
              DO 15 I=1,16                                              00167200
               IF(FLNM1(I:I).EQ.'.')GO TO 25                            00167300
15             CONTINUE                                                 00167400
              DO 20 I=1,16                                              00167500
                 IF(FLNM1(I:I).EQ.' ')THEN                              00167600
                  IE = I+3                                              00167700
                  FLNM1(I:IE) = '.dat'                                  00167800
                  GO TO 25                                              00167900
                 END IF                                                 00168000
20             CONTINUE                                                 00168100
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1      0T168200
35    WRITE(*,FMT='(//,'' Enter output file name:'',/)')                 T        
           READ(*,ERR=35,FMT='(A)')FLNM2                                0T168600
           IF(FLNM2.EQ.'CON')GO TO 60                                   0T168700
           IF(FLNM2.EQ.'con')GO TO 60                                   0T168800
              DO 40 I=1,16                                              00168900
               IF(FLNM2(I:I).EQ.'.')GO TO 60                            00169000
40             CONTINUE                                                 00169100
              DO 50 I=1,16                                              00169200
                 IF(FLNM2(I:I).EQ.' ')THEN                              00169300
                  IE = I+3                                              00169400
                  FLNM2(I:IE) = '.out'                                  00169500
                  GO TO 60                                              00169600
                 END IF                                                 00169700
50             CONTINUE                                                 00169800
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2    0T169900
          OPEN(OUNIT,FILE=FLNM2)                                        00170000
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                          00170100
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                      00170200
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                      00170300
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                      00170400
      OPEN(24,STATUS='SCRATCH',FORM='UNFORMATTED')                      00170500
      RETURN                                                            00170600
      END                                                               00170700
C                                                                       00170800
C                                                                       00170900
C                                                                       00174100
C   FAKE PLOT ROUTINES FOR TEMPORARY COMPILING                          00174200
C                                                                       00174300
C                                                                       00174400
C                                                                       00174500
      SUBROUTINE PLOT(REAL1,REAL2,INT1)                                 00174600
      RETURN                                                            00174700
      END                                                               00174800
      SUBROUTINE SYMBOL(REAL1,REAL2,REAL3,A40,REAL4,INT1)               00174900
      CHARACTER*40 A40                                                  00175000
      RETURN                                                            00175100
      END                                                               00175200
      SUBROUTINE NUMBER(REAL1,REAL2,REAL3,REAL4,REAL5,INT1)             00175300
      RETURN                                                            00175400
      END                                                               00175500
      SUBROUTINE CURVEX(RL1,RL2,RL3,RL4,RL5,RL6,RL7,RL8,RL9,RL10)       00175600
      RETURN                                                            00175700
      END                                                               00175800
