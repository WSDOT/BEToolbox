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

C**** BEAM DEFLECTION A N-TH POINTS***'BEAMDEF(X)'***        RFF*730319*P6300010
C                                                                          00020
C   A - REVISED BY GARY CHRISTOPHER  7/17/73                              A00030
C   B - PC CONVERSION BY NEAL CAMPBELL 4/11/86                            A00030
C   T - VMS CONVERSION BY TERRY BEEBE JANUARY 1990                        T 
C       RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26.93
C                                                                          00040
      IMPLICIT REAL*8 (A-H,O-Z)                                           B00050
      LOGICAL*1 CCHAR                                                     A00120
      CHARACTER*1 CARCNT                                                  B
      INTEGER*4 OUNIT, ERUNIT                                             B
      CHARACTER*8 REVDAT, TIME                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T
      DIMENSION XM(50),XI(50),R(50),THETA(50)                              00140
      DIMENSION XN(50),Y(50),PL(11),IPT(11)                               B00150
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
      REVDAT = ' 1/30/90'                                                 T
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
C                                                                         B
C     CALL OSDATE(DATE)                                                   T
      CALL LIB$DATE_TIME (DATIME)                                         T
      TIME = DATIME(13:20)                                                T
      DATE = DATIME(1:11)                                                 T  
C     CALL GETIME(TIME)                                                   T
C                                                                         B
      INUNIT=5                                                            B
      CARCNT='1'                                                          B
      ERUNIT=9                                                            B
      OUNIT=9                                                             B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C     IMODE= 1 BUT IS NOT USED ANYWHERE                                   B
C     OUNIT = (6 FOR FILE OUTPUT)                                         T
C             (9 FOR PRINTER OUTPUT)                                      T 
C     ERUNIT = (3 FOR FILEOUTPUT)                                         T
C              (9 FOR PRINTER OUTPUT)                                     T
C     INUNIT = (5 FOR FILE INPUT)                                         B
C     CCHAR = .TRUE. BUT IS NOT USED ANYWHERE                             B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C                                                                         B
C                                                                         B
      CALL PARM (IMODE,OUNIT,ERUNIT,CCHAR)                                A00320
C                                                                          00340
   80 PL(11)=0.0                                                          A00350
      IPT(11)=0                                                          RT00360
      READ(INUNIT,END=1640,ERR=1625,FMT=*)IDENT1,IDENT2,N1,NU,INERT,      B
     1           (PL(II),IPT(II),II=1,10),M                               B
C                                                                          00370
C                                                                          00460
  110 NE=NU+1                                                           BEA00470
C                                                                          00480
C                                                                          00590
 120  READ(INUNIT,END=1640,ERR=1625,FMT=*)NO,XL,XM(1),XM(NE),             B
     1        XI(1),XI(NE),W,E,NCAN                                       B
      K=1                                                               BEA00600
      IF (NO-2)160,200,160                                              BEA00610
C                                                                          00620
  160 WRITE(ERUNIT,170)                                                    00630
  170 FORMAT (/,'  INPUT NOT IN CORRECT SEQUENCE',/)                       00640
      STOP                                                              BEA00650
C     CARCNT IS THE CARRIAGE CONTROL CHARACTER                            A00660
 5000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION    ',    T
     1  A11,'  TIME ',A8)                                                 T
 5001 FORMAT('        *** BEAM DEFLECTION ***                             B
     1' ,/,8X,'REV ',A8,///)                                              B
 200   WRITE(OUNIT,5000)CARCNT,DATE,TIME                                  B
       WRITE(OUNIT,5001)REVDAT                                            B
      WRITE(OUNIT,210)IDENT1,IDENT2                                        00690
  210 FORMAT(/,7X,' ','IDENT. ',I5,'  BEAM NO. ',I3,/)                     00700
  205 WRITE(OUNIT,215)XL,W,E                                            RT900710
  215 FORMAT(13X,F6.2,' FT. SPAN   W=',F9.3,3X,'E=',F10.0/)                00720
C                                                                          00730
      IF (XL)160,230,270                                                BEA00740
  230 THET2=0.                                                          BEA00750
      THET1=0.                                                          BEA00760
      THETA(NE)=0.                                                      BEA00770
      GO TO 1150                                                        BEA00780
C                                                                          00790
  270 DO 310 I=2,NU                                                     BEA00800
         READ(INUNIT,END=395,ERR=1625,FMT=*)NO,XM(I),XI(I)
         IF (NO+1-I)160,310,160                                            00850
  310 CONTINUE                                                          BEA00860
C                                                                          00870
      DO 350 I=2,NU                                                     BEA00880
      IF (XM(I) .NE. 0) GO TO 750                                          00890
  350 CONTINUE                                                             00900
C                                                                          00910
  360 IF (W)410,370,410                                                 BEA00920
  370 IF (PL(1))380,700,380                                             BEA00930
C                                                                          00940
  380 DO 390 I=2,NU                                                     BEA00950
  390 XM(I)=0                                                           BEA00960
C                                                                          00970
      GO TO 500                                                         BEA00980
  395 WRITE(ERUNIT,*)' NOT ENOUGH M AND I VALUES GIVEN, TERMINATING RUN'
      STOP
  410 UN=NU                                                             BEA00990
      C=1./UN                                                           BEA01000
      CU=C                                                              BEA01010
C                                                                          01020
      DO 480 I=2,NU                                                     BEA01030
      XU=CU*XL                                                          BEA01040
      XM(I)=((W*XL/2.)*XU-XU*XU*(W/2.)+XM(1)+CU*(XM(NE)-XM(1)))         BEA01050
  480 CU=CU+C                                                              01060
C                                                                          01070
      GO TO 750                                                         BEA01080
  500 PT=IPT(1)                                                         BEA01090
      UN=NU                                                             BEA01100
      C=1./UN                                                           BEA01110
C                                                                          01120
      WRITE(OUNIT,540)PL(1),IPT(1)                                      RT901130
  540 FORMAT (32H           CONCENTRATED LOAD OF ,F6.1,                   B01140
     1 11H AT POINT  ,I4/)                                                B01145
C                                                                          01150
      XP=XL*PT/UN                                                       BEA01160
      CU=C                                                              BEA01170
C                                                                          01180
      DO 630 I=2,NU                                                     BEA01190
      IF (I-IPT(1))590,590,610                                          BEA01200
  590 XM(I)=((XL-XP)/XL)*CU*XL*PL(1)+XM(I)                              BEA01210
      GO TO 620                                                         BEA01220
  610 XM(I)=(XP/XL)*(XL-CU*XL)*PL(1)+XM(I)                              BEA01230
  620 CU=CU+C                                                           BEA01240
  630 CONTINUE                                                          BEA01250
C                                                                          01260
      IF (PL(2))650,700,650                                             BEA01270
C                                                                          01280
  650 DO 680 J=1,10                                                     RT101290
      PL(J)=PL(J+1)                                                     BEA01300
  680 IPT(J)=IPT(J+1)                                                      01310
C                                                                          01320
      GO TO 500                                                         BEA01330
  700 CU=C                                                                 01340
C                                                                          01350
      DO 740 I=2,NU                                                     BEA01360
      XM(I)=XM(I)+XM(1)+CU*(XM(NE)-XM(1))                               BEA01370
  740 CU=CU+C                                                           BEA01380
C                                                                          01390
  750 UN=NU                                                                01400
      N=1                                                               BEA01410
C                                                                          01420
C     IN FOLLOWING   STATMENTS ,XL,XLL IS MULT.   BY 12 TO CONVERT INTO BEA01430
C        INCHES                                                         BEA01440
C     FOR CONVERTING MOMENT K-FT TO LB-INCH USE FACTOR OF 12000         BEA01450
C     CI=1/12000 AND  MOMENT OF INERTIA FT-4 TO INCH-4 TO BE            BEA01460
C     CONSIDERED CI=20736/12000=1.728                                   BEA01470
C                                                                          01480
      C=1./UN                                                           BEA01490
      IF (INERT)160,830,835                                             BEA01500
C                                                                          01510
  830 WRITE(ERUNIT,832)                                                    01520
  832 FORMAT(/,' ',7X,'MOMENT OF INERTIA UNITS NOT DEFINED IN CARD 1-ASS  B01530
     1UME D AS IN**4',/)                                                  B01540
C                                                                          01550
      INERT=2                                                           BEA01560
  835 GO TO (840,860),INERT                                             BEA01570
  840 CI=1.728                                                          BEA01580
      GO TO 870                                                         BEA01590
  860 CI=1./12000.                                                      BEA01600
  870 IF (PL(1))880,950,880                                             BEA01610
C                                                                          01620
  880 DO 890 I=1,NE                                                     BEA01630
  890 XN(I)=XM(I)/(E*XI(I)*CI)                                          BEA01640
C                                                                          01650
      R(1)=(XN(1)*2.+XN(2))*C*XL*2.                                     BEA01660
      R(NE)=(XN(NE)*2.+XN(NE-1))*C*XL*2.                                BEA01670
C                                                                          01680
      DO 930 I=2,NU                                                     BEA01690
  930 R(I)=(XN(I-1)+4.*XN(I)+XN(I+1))*C*XL*2.                           BEA01700
C                                                                          01710
      GO TO 1010                                                        BEA01720
C                                                                          01730
  950 DO 960 I=1,NE                                                     BEA01740
  960 XN(I)=XM(I)/(E*XI(I)*CI)                                          BEA01750
C                                                                          01760
      R(1)=(XL*(3.5*XN(1)+3.*XN(2)-.5*XN(3)))*C                         BEA01770
      R(NE)=XL*C*(3.5*XN(NE)+3.*XN(NE-1)-.5*XN(NE-2))                   BEA01780
C                                                                          01790
      DO 1000 I=2,NU                                                    BEA01800
 1000 R(I)=XL*C*(XN(I-1)+10.*XN(I)+XN(I+1))                             BEA01810
C                                                                          01820
 1010 THETA(1)=R(1)                                                     BEA01830
C                                                                          01840
      DO 1030 I=2,NU                                                    BEA01850
 1030 THETA(I)=THETA(I-1)+R(I)                                          BEA01860
C                                                                          01870
      Y(1)=XL*THETA(1)*12.*C                                            BEA01880
C                                                                          01890
      DO 1060 I=2,NU                                                    BEA01900
 1060 Y(I)=Y(I-1)+THETA(I)*12.*C*XL                                     BEA01910
C                                                                          01920
      CU=C                                                              BEA01930
      NA=NU-1                                                           BEA01940
C                                                                          01950
      DO 1110 I=1,NA                                                    BEA01960
      Y(I)=Y(I)-CU*Y(NU)                                                BEA01970
 1110 CU=CU+C                                                           BEA01980
C                                                                          01990
      THETA(1)=Y(1)/(XL*12.*C)-R(1)                                     BEA02000
      THET2=-THETA(1)                                                   BEA02010
      THETA(NE)=R(NE)-Y(NA)/(XL*12.*C)                                  BEA02020
 1114 GO TO (1150,1115,1118),N                                          BEA02030
 1115 ALPHA=-THETA(1)                                                   BEA02040
      GAMMA=THETA(NE)                                                   BEA02050
      GO TO 1605                                                        BEA02060
 1118 GAMMA=(GAMMA-THETA(1))/2.                                         BEA02070
      BETHA=THETA(NE)                                                   BEA02080
      CAB=GAMMA/BETHA                                                   BEA02090
      CBA=GAMMA/ALPHA                                                   BEA02100
      XKAB=(1000./(ALPHA-CAB*GAMMA))*12.*XL/(E*XIMIN*CI)                BEA02110
      XKBA=(1000./(BETHA-CBA*GAMMA))*12.*XL/(E*XIMIN*CI)                BEA02120
C                                                                          02130
      WRITE(OUNIT,1125)XKAB,XKBA                                        RT902140
 1125 FORMAT(32H        STIFFNESS FACTORS   KAB=,F8.4,8H    KBA=,F8.4/)    02150
      WRITE(OUNIT,1127) CAB,CBA                                         RT902160
 1127 FORMAT(32H        CARRY-OVER FACTORS  CAB= ,F8.4,8H    CBA=,F8.4/)  B02170
C                                                                          02180
      FEML=(XKAB*SLL+CBA*XKBA*SLR)/12.                                  BEA02190
      FEMR=(XKBA*SLR+CAB*XKAB*SLL)/12.                                  BEA02200
      FEMR=FEMR*E*XIMIN*CI/XL                                           BEA02210
      FEMR=-FEMR                                                        BEA02220
      FEML=FEML*E*XIMIN*CI/XL                                           BEA02230
C                                                                          02240
      WRITE(OUNIT,1131)FEML,FEMR                                        RT902250
 1131 FORMAT(28H        FIXED END MOM. LEFT=,F10.2,6H K-FT.,/,            A02260
     1 29H        FIXED END MOM. RIGHT=,F9.2,6H K-FT./)                   B02270
C                                                                          02280
      GO TO 1619                                                        BEA02290
 1150 IF (NCAN)160,1470,1160                                            BEA02300
 1160 NCAN=NCAN-1                                                       BEA02310
C                                                                          02320
      READ(INUNIT,END=1625,ERR=1625,FMT=*)                                B
     1     NO,XLL,XM2,XM1,XM0,XI2,XI1,XI0                                 B
C                                                                          02420
      IF (NO-4)1200,1230,160                                            BEA02430
 1200 IF (NO-3)160,1210,160                                             BEA02440
 1210 XONE=-1.                                                          BEA02450
      GO TO 1250                                                        BEA02460
 1230 THET2=THETA(NE)                                                   BEA02470
      XONE=1.                                                           BEA02480
 1250 GO TO (1260,1280),INERT                                           BEA02490
 1260 CI=1.728                                                          BEA02500
      GO TO 1290                                                        BEA02510
 1280 CI=1.728/20736.                                                   BEA02520
 1290 XN2=XM2/(E*XI2*CI)                                                BEA02530
      XN1=XM1/(E*XI1*CI)                                                BEA02540
      XN0=XM0/(E*XI0*CI)                                                BEA02550
      RE=(3.*XN2-.5*XN1)*XLL/3.                                         BEA02560
      R0=(3.5*XN0+3.*XN1-.5*XN2)*XLL/3.                                 BEA02570
      R2=(10.*XN2+XN1)*XLL/3.                                           BEA02580
      R1=(XN2+10.*XN1+XN0)*XLL/3.                                       BEA02590
      DEL1=(THET2+R0)*XLL*4.                                            BEA02600
      DEL2=DEL1+(THET2+R0+R1)*XLL*4.                                    BEA02610
      DEL3=DEL2+(THET2+R0+R1+R2)*XLL*4.                                 BEA02620
      THETE=XONE*(THET2+R0+R1+R2+RE)                                    BEA02630
      GO TO (1410,1430),K                                               BEA02640
C                                                                          02650
 1410 WRITE(OUNIT,1420)                                                 RT902660
 1420 FORMAT (11X,'END SLOPE      YEND      Y2/3      Y1/3    LENGTH',/)   02670
 1430 K=K+1                                                             BEA02680
      WRITE(OUNIT,1440)THETE,DEL3,DEL2,DEL1,XLL                         RT902690
 1440 FORMAT (11X,F10.7,3(2X,F8.3),2X,F7.2,/)                              02700
C                                                                          02710
      GO TO (160,1150,1480),K                                           BEA02720
 1470 IF(XL) 1619,1619,1475                                             RT102730
 1475 SLL=THETA(1)                                                      BEA02740
      SLR=THETA(NE)                                                     BEA02750
C                                                                          02760
 1480 WRITE(OUNIT,1490)THETA(1),THETA(NE)                               RT902770
 1490 FORMAT(24H             SLOPE LEFT=,F10.8,13H SLOPE RIGHT=,F10.8/)    02780
      WRITE(OUNIT,1510)                                                 RT902790
 1510 FORMAT (46H        DEFLECTION AT NTH POINTS IN INCHES    /)       BEA02800
      WRITE(OUNIT,1520)XM(1),XI(1)                                      RT902810
 1520 FORMAT(40X,3H M=,F12.2,4H, I=,F12.2/)                                02820
      NNU=NU-1                                                          BEA02830
C                                                                          02840
      DO 1560 I=1,NNU                                                   BEA02850
      WRITE(OUNIT,1550)I,NU,Y(I),XM(I+1),XI(I+1)                        RT902860
 1550 FORMAT(12X,I2,1H/,I2,12H,DEFLECTION=,F10.5,                         B02870
     1           4H  M=,F12.2,4H, I=,F12.2/)                              B02875
 1560 CONTINUE                                                          BEA02880
C                                                                          02890
      WRITE(OUNIT,1520)XM(NE),XI(NE)                                    RT902900
      WRITE(OUNIT,1580)                                                 RT902910
 1580 FORMAT (2H  )                                                     BEA02920
C                                                                          02930
 1600 XIMIN=XI(1)                                                       BEA02940
C                                                                          02950
      DO 1604 I=1,NE                                                    BEA02960
      IF (XI(I)-XIMIN)1603,1604,1604                                    BEA02970
 1603 XIMIN=XI(I)                                                       BEA02980
 1604 CONTINUE                                                          BEA02990
C                                                                          03000
 1605 M=M+1                                                             BEA03010
      GO TO (1619,1607,1613),M                                          BEA03020
C                                                                          03030
 1607 DO 1610 I=1,NE                                                    BEA03040
      YY=I-1                                                            BEA03050
      UN=NU                                                             BEA03060
 1610 XM(I)=1000.-1000.*YY/UN                                           BEA03070
C                                                                          03080
      N=2                                                               BEA03090
      GO TO 880                                                         BEA03100
C                                                                          03110
 1613 DO 1615 I=1,NE                                                    BEA03120
      YY=I-1                                                            BEA03130
 1615 XM(I)=1000.*YY/UN                                                 BEA03140
C                                                                          03150
      N=3                                                               BEA03160
      GO TO 880                                                         BEA03170
C                                                                          03180
 1619 WRITE(OUNIT,1620)                                                    03190
 1620 FORMAT (29H        PROCESSING COMPLETED./)                        BEA03200
      GO TO 80                                                            B03210
 1625 WRITE (ERUNIT,*)' INPUT ERROR!!  CHECK YOUR INPUT FILE!!!'          B03220
 1640 STOP                                                                A03260
C                                                                         B
      END                                                               BEA03270
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
      SUBROUTINE PARM(IMODE,OUNIT,ERUNIT,CCHAR)                           B
      CHARACTER*16 FLNM1, FLNM2                                           B
      LOGICAL*1 CCHAR                                                     B
      INTEGER OUNIT, ERUNIT                                               B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                     B
C  STANDARD INTERACTIVE INPUT UNIT                                        B
      IIUNIT=1                                                            B
C  STANDARD INTERACTIVE OUTPUT UNIT                                       B
      IOUNIT=3                                                            B
C  STANDARD REPORT OUTPUT UNIT                                            B
      OUNIT=6                                                             B
C  STANDARD FILE INPUT UNIT                                               B
      INUNIT=5                                                            B
C  STANDARD ERROR OUTPUT UNIT                                             B
      ERUNIT=3                                                            B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                     B
      IMODE=1                                                             B
      CCHAR=.TRUE.                                                        B
C     OPEN(IIUNIT,FILE='CON')                                             T
C     OPEN(IOUNIT,FILE='CON')                                             T
10    WRITE(*,FMT='(//,'' Enter input file name:'',/)')                   T
           READ(*,ERR=10,FMT='(A)')FLNM1                                  T
           IF(FLNM1.EQ.'CON')GO TO 25                                     T
           IF(FLNM1.EQ.'con')GO TO 25                                     T
              DO 15 I=1,16                                                B
               IF(FLNM1(I:I).EQ.'.')GO TO 25                              B
15             CONTINUE                                                   B
              DO 20 I=1,16                                                B
                 IF(FLNM1(I:I).EQ.' ')THEN                                B
                  IE = I+3                                                B
                  FLNM1(I:IE) = '.dat'                                    B
                  GO TO 25                                                B
                 END IF                                                   B
20             CONTINUE                                                   B
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1        B
35    WRITE(*,FMT='(//,'' Enter output file name:'',/)')                  T 
           READ(*,ERR=35,FMT='(A)')FLNM2                                  T
           IF(FLNM2.EQ.'CON')GO TO 60                                     T
           IF(FLNM2.EQ.'con')GO TO 60                                     T
              DO 40 I=1,16                                                B
               IF(FLNM2(I:I).EQ.'.')GO TO 60                              B
40             CONTINUE                                                   B
              DO 50 I=1,16                                                B
                 IF(FLNM2(I:I).EQ.' ')THEN                                B
                  IE = I+3                                                B
                  FLNM2(I:IE) = '.out'                                    B
                  GO TO 60                                                B
                 END IF                                                   B
50             CONTINUE                                                   B
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2      T
          OPEN(OUNIT,FILE=FLNM2)                                          B
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                            B
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                        B
      RETURN                                                              B
      END                                                                 B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
