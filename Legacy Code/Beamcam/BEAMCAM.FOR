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

C     BEAM CAMBER CALCULATIONS                                          BEA00010
C                                                                          00020
C     A - GARY CHRISTOPHER - TSO CONVERSION - 07/23/73                    A00030
C     B - NEAL CAMPBELL - PC CONVERSION - 04/10/86                        C
C     T - TERRY BEEBE   - VMS CONVERSION - JANUARY 1990
C         RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C                                                                          00040
      IMPLICIT REAL*8 (A-H,O-Z)                                            00050
      INTEGER*4 LIST,ERLIST,ERUNIT,OUNIT                                  C00080
      LOGICAL*1 CCHAR                                                     A00100
      DIMENSION XM(11),XI(11)                                           BEA00130
      DIMENSION R(11),THETA(11)                                         BEA00140
      DIMENSION XN(11),Y(10)                                            BEA00150
      CHARACTER*1 CARCNT                                                  C
      CHARACTER*8 TIME, REVDAT                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T
      CHARACTER*4 IMODE                                                   C
      INUNIT=5                                                            C
      LIST=9                                                              C
      ERLIST=9                                                            C
      CARCNT='1'                                                          C
      IMODE='FREE'                                                        C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        C
C     OUNIT = (6 FOR SCREEN OUTPUT)                                       T
C             (9 FOR PRINTER OUPTPUT)                                     T
C     ERUNIT = (3 FOR FILE OUTPUT)                                        T
C              (9 FOR PRINTER OUTPUT)                                     T
C     INUNIT = (5 FOR FILE INPUT)                                         C
C     CARCNT = ('1' FOR BATCH)                                            C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        C
C                                                                         C
C                                                                         C
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    C
      REVDAT = ' 1/30/90'                                                 C
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    C
C                                                                         C
C                                                                         C
C                                                                          00200
      CALL PARM(IMODE,OUNIT,ERUNIT,CCHAR)                                 A00220
      CALL LIB$DATE_TIME (DATIME)                                         T
      TIME = DATIME(13:20)                                                T 
      DATE = DATIME(1:11)                                                 T
      LIST = OUNIT                                                        A00230
      ERLIST = ERUNIT                                                     A00240
C                                                                          00250
1     READ(INUNIT,END=70  ,ERR=45,FMT=*)IDEN1,IDEN2,NO,XL,XM(1),          C
     1      XM(11),XI(1),XI(11),W,E,NCAN                                  C
      K = 1                                                                00360
C                                                                          00370
      IF (NO .NE. 1) GO TO 999                                             00380
      GO TO 998                                                            00390
C                                                                          00400
 1001 WRITE( LIST,1002)                                                    00410
 1002 FORMAT(/,7X,' END OF BEAM CAMBER RUN')                               00420
      GO TO 1                                                              00430
  999 WRITE (ERLIST,997)                                                   00440
  997 FORMAT(' ','INPUT NOT IN CORRECT SEQUENCE',//)                       00450
      STOP                                                                C
C                                                                          00470
C     CARCNT - CARRIAGE CONTROL CHARACTER                                 A00480
 2000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION   ',     T
     1  A11,'  TIME ',A8)                                                 C
 2001 FORMAT('        *** BEAM CAMBER ***                                 C
     1' ,/,8X,'REV ',A8,///)                                              C
  998  WRITE(LIST,2000)CARCNT,DATE,TIME                                   C
       WRITE(LIST,2001)REVDAT                                             C
      WRITE(LIST,4)IDEN1,IDEN2                                             00510
    4 FORMAT(' ',7X,'IDENT.',I5,' BEAM NO.',I3,/)                          00520
C                                                                          00530
      IF (XL)996,42,996                                                 BEA00540
   42 THET2=0.                                                          BEA00550
      THET1=0.                                                          BEA00560
      THETA(11)=0.                                                      BEA00570
      GO TO 27                                                          BEA00580
C                                                                          00590
 996  READ(INUNIT,END=45,ERR=45,FMT=*)NO,(XI(MM),MM=2,10)                 C
C                                                                          00640
      IF (NO-2)999,41,999                                               BEA00650
   41 IF (W)8,6,8                                                       BEA00660
C                                                                          00670
   6  READ(INUNIT,END=45,ERR=45,FMT=*)NO,(XM(MM),MM=2,10)                 C
C                                                                          00720
      IF (NO-3)999,13,999                                               BEA00730
    8 C=.1                                                              BEA00740
C                                                                          00750
      DO 10 J=2,10                                                      BEA00760
      XM(J)=XM(1)+C*(XM(11)-XM(1))+W*C*XL*(XL-C*XL)/2.                  BEA00770
   10 C=C+.1                                                            BEA00780
C                                                                          00790
   13 DO 14 I=1,11                                                      BEA00800
   14 XN(I)=-XM(I)/(E*XI(I)*1.728)                                      BEA00810
C                                                                          00820
      R(1)=(XL*(3.5*XN(1)+3.*XN(2)-.5*XN(3)))*.1                        BEA00830
      R(11)=XL*.1*(3.5*XN(11)+3.*XN(10)-.5*XN(9))                       BEA00840
C                                                                          00850
      DO 15 I=2,10                                                      BEA00860
   15 R(I)=XL*.1*(XN(I-1)+10.*XN(I)+XN(I+1))                            BEA00870
C                                                                          00880
      THETA(1)=R(1)                                                     BEA00890
C                                                                          00900
      DO 16 I=2,10                                                      BEA00910
   16 THETA(I)=THETA(I-1)+R(I)                                          BEA00920
C                                                                          00930
      Y(1)=XL*THETA(1)*1.2                                              BEA00940
C                                                                          00950
      DO 17 I=2,10                                                      BEA00960
   17 Y(I)=Y(I-1)+THETA(I)*1.2*XL                                       BEA00970
C                                                                          00980
      C=.1                                                              BEA00990
C                                                                          01000
      DO 18 I=1,9                                                       BEA01010
      Y(I)=Y(I)-C*Y(10)                                                 BEA01020
   18 C=C+.1                                                            BEA01030
C                                                                          01040
      THETA(1)=Y(1)/(XL*1.2)-R(1)                                       BEA01050
      THET2=-THETA(1)                                                   BEA01060
      THETA(11)=R(11)-Y(9)/(XL*1.2)                                     BEA01070
   27 IF (NCAN)999,32,29                                                BEA01080
   29 NCAN=NCAN-1                                                       BEA01090
C                                                                          01100
      READ(INUNIT,END=45,ERR=45,FMT=*)NO,XLL,XM2,XM1,XM0,XI2,XI1,XI0      C
C                                                                          01200
      IF (NO-5)43,30,999                                                BEA01210
   43 XONE=-1.                                                          BEA01220
      GO TO 21                                                          BEA01230
   30 THET2=THETA(11)                                                   BEA01240
      XONE=1.                                                           BEA01250
   21 XN2=-XM2/(E*XI2*1.728)                                            BEA01260
      XN1=-XM1/(E*XI1*1.728)                                            BEA01270
      XN0=-XM0/(E*XI0*1.728)                                            BEA01280
      RE=(3.*XN2-.5*XN1)*XLL/3.                                         BEA01290
      R0=(3.5*XN0+3.*XN1-.5*XN2)*XLL/3.                                 BEA01300
      R2=(10.*XN2+XN1)*XLL/3.                                           BEA01310
      R1=(XN2+10.*XN1+XN0)*XLL/3.                                       BEA01320
      DEL1=(THET2+R0)*XLL*4.                                            BEA01330
      DEL2=DEL1+(THET2+R0+R1)*XLL*4.                                    BEA01340
      DEL3=DEL2+(THET2+R0+R1+R2)*XLL*4.                                 BEA01350
      THETE=XONE*(THET2+R0+R1+R2+RE)                                    BEA01360
      GO TO (22,28),K                                                   BEA01370
C                                                                          01380
   22 WRITE (LIST,25)                                                      01390
   25 FORMAT('        END SLOPE     YEND     Y2/3     Y1/3     LENGTH',/  01400
     1)
      K=2                                                               BEA01410
   28 WRITE (LIST,26)THETE,DEL3,DEL2,DEL1,XLL                              01420
   26 FORMAT (' ',7X,F10.8,2X,F7.3,2X,F7.3,2X,F7.3,2X,F7.2,/)              01430
      GO TO 27                                                          BEA01440
   32 IF (XL)999,1,40                                                   BEA01450
   40 WRITE (LIST,33)THETA(1),THETA(11)                                    01460
   33 FORMAT('        SLOPE LEFT = ',F10.8,'  SLOPE RIGHT = ',F10.8,/)     01470
      WRITE (LIST,35)                                                      01480
   35 FORMAT (52H                     .1       .2       .3       .4  )  BEA01490
      WRITE (LIST,36)Y(1),Y(2),Y(3),Y(4)                                   01500
   36 FORMAT (' ',7X,F18.3,F9.3,F9.3,F9.3)                                 01510
      WRITE (LIST,37)                                                      01520
   37 FORMAT (52H            .5       .6       .7       .8       .9  )  BEA01530
      WRITE (LIST,38)Y(5),Y(6),Y(7),Y(8),Y(9)                              01540
   38 FORMAT (' ',7X,F9.3,F9.3,F9.3,F9.3,F9.3,7H INCHES,////)              01550
      GO TO 1001                                                        BEA01560
C                                                                          01570
   45 WRITE (*,*)'  INPUT ERROR!!  CHECK YOUR INPUT FILE!!!'              T01580
   70 STOP                                                                A01610
      END                                                               BEA01620
C                                                                         C
C                                                                         C
C                                                                         C
C                                                                         C
C                                                                         C
C                                                                         C
      SUBROUTINE PARM(IMODE,OUNIT,ERUNIT,CCHAR)                           C
      CHARACTER*16 FLNM1, FLNM2                                           C
      CHARACTER*4 IMODE                                                   C
      LOGICAL*1 CCHAR                                                     C
      INTEGER OUNIT, ERUNIT                                               C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                     C
C  STANDARD INTERACTIVE INPUT UNIT                                        C
      IIUNIT=1                                                            C
C  STANDARD INTERACTIVE OUTPUT UNIT                                       C
      IOUNIT=3                                                            C
C  STANDARD REPORT OUTPUT UNIT                                            C
      OUNIT=6                                                             C
C  STANDARD FILE INPUT UNIT                                               C
      INUNIT=5                                                            C
C  STANDARD ERROR OUTPUT UNIT                                             C
      ERUNIT=3                                                            C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                     C
      IMODE='FREE'                                                        C
      CCHAR=.TRUE.                                                        C
10    WRITE(*,FMT='(//,'' Enter input file name'',/)')                    T
           READ(*,ERR=10,FMT='(A)')FLNM1                                  T
           IF(FLNM1.EQ.'CON')GO TO 25                                     T
           IF(FLNM1.EQ.'con')GO TO 25                                     T
              DO 15 I=1,16                                                C
               IF(FLNM1(I:I).EQ.'.')GO TO 25                              C
15             CONTINUE                                                   C
              DO 20 I=1,16                                                C
                 IF(FLNM1(I:I).EQ.' ')THEN                                C
                  IE = I+3                                                C
                  FLNM1(I:IE) = '.dat'                                    C
                  GO TO 25                                                C
                 END IF                                                   C
20             CONTINUE                                                   C
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1        T
35    WRITE(*,FMT='(//,'' Enter output file name'',/)')                   T
           READ(*,ERR=35,FMT='(A)')FLNM2                                  T
           IF(FLNM2.EQ.'CON')GO TO 60                                     T
C          IF(FLNM2.EQ.'con')GO TO 60                                     T
              DO 40 I=1,16                                                C
               IF(FLNM2(I:I).EQ.'.')GO TO 60                              C
40             CONTINUE                                                   C
              DO 50 I=1,16                                                C
                 IF(FLNM2(I:I).EQ.' ')THEN                                C
                  IE = I+3                                                C
                  FLNM2(I:IE) = '.out'                                    C
                  GO TO 60                                                C
                 END IF                                                   C
50             CONTINUE                                                   C
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2      T
          OPEN(OUNIT,FILE=FLNM2)                                          C
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                            C
C     IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                        C
C     IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                        C
C     IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                        C
      RETURN                                                              C
      END                                                                 C
C                                                                         C
