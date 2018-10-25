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

C THIS PROGRAM COMPILES SL GROUP I AND LF GROUP LOADS OR ALL SL GROUPS.
C IT ALSO LOOKS FOR MAXIMUMS, MINIMUMS SLIDING, ECENTRICITY, PILE AND
C SPREAD FOOTING LOADS.
C
C REVISION LOG:
C
C     T - TERRY BEEBE - VMS CONVERSION - JANUARY 1990                     T
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C
      REAL BD(4), BE(4), DL(5), LL1(5), CF1(5), E(5)
      REAL LL2(5), LL3(5), CF2(5), CF3(5), LF2(5), LF3(5)
      REAL B(5), SF(5), W(5), WL(5), LF1(5), RST(5), EQ3(5), EQ4(5)
      REAL ICE(5), A(5), XX(10), XT(10)
      REAL MIN(10), MINL(10), MINT(10), MINLT(10)
      REAL MAX(10), MAXL(10), MAXT(10), MAXLT(10)
      REAL G(23,5)
      CHARACTER*40 TITLE
      CHARACTER*6  GP(23), LD(5), MAXCH, MINCH
      CHARACTER*1 CARCNT
      INTEGER OUNIT, ERUNIT
      CHARACTER*8 REVDAT, TIME                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     OUNIT = (6 FOR FILE OUTPUT)
C     ERUNIT = (3 FOR PC)
C              (6 FOR MAINFRAME BATCH)
C     INUNIT = (5 FOR FILE INPUT)
C     CARCNT = ('1' FOR BATCH)
C              (' ' FOR INTERACTIVE)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
      CALL PARM(OUNIT,ERUNIT,CARCNT,INUNIT)
      CALL LIB$DATE_TIME (DATIME)                                         T  
      TIME = DATIME(13:20)                                                T   
      DATE = DATIME(1:11)                                                 T
C
C  IF PC VERSION SET PRINTER TO COMPRESSED. NOTE COMMAND AT END OF
C  PROGRAM TO RESET PRINTER TO NORMAL OPERATIONS.
C  ACTUAL CHARACTERS NOT IN MAIN FRAME VERSION
C
C      IF(ERUNIT.EQ.3)WRITE(OUNIT,'('' '')')
CCCCCC
C
C
C
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION   ',     T
     1  A11,'  TIME ',A8)                                                 T
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      REVDAT = '01/30/90'                                                 T
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
 1001 FORMAT('        *** GROUP LOADS AND MAXIMUMS (GPLOAD) ***
     1' ,/,8X,'REV ',A8,///)
C
C  INITIALIZE ALL VARIABLES
C
      LD(1)=' P  '
      LD(2)=' ML '
      LD(3)=' MT '
      LD(4)=' HL '
      LD(5)=' HT '
      GP(1)='SL-IA '
      GP(2)='SL-IB '
      GP(3)='SL-IC '
      GP(4)='I-A   '
      GP(5)='I-B   '
      GP(6)='I-C   '
      GP(7)='II    '
      GP(8)='III-A '
      GP(9)='III-B '
      GP(10)='III-C '
      GP(11)='IV-A  '
      GP(12)='IV-B  '
      GP(13)='IV-C  '
      GP(14)='V     '
      GP(15)='VI-A  '
      GP(16)='VI-B  '
      GP(17)='VI-C  '
      GP(18)='VII-3 '
      GP(19)='VII-4 '
      GP(20)='VIII-A'
      GP(21)='VIII-B'
      GP(22)='VIII-C'
      GP(23)='IX    '
      KSL=0
      KLOOP=4
      KGROUP=23
      MAXCH='MAX'
      MINCH='MIN'
      DO 10 I=1,10
      MAX(I)  =-5000000.
      MIN(I)  = 5000000.
      MAXT(I) =-5000000.
10    MINT(I) = 5000000.
C
C  READ PROGRAM PARAMETERS
C
      READ (INUNIT,END=999,ERR=998,FMT=*)BD1,BD2,BE1,
     1                              BE2,R,KFOOT,PFTG,AREA,ZL,ZT
C
C  RESET FOR SERVICE LOADS ONLY
C
      IF(BD1.EQ.0.AND.BD2.EQ.0)THEN
          KSL=1
          BD1=1
          KGROUP=20
          KLOOP=2
          DO 15 I=4,23
              J=I-3
15            GP(J)=GP(I)
      END IF
1010  FORMAT(A40)
C
C  READ DATA FOR ONE PROBLEM
C
20    READ (INUNIT,1010,END=130,ERR=998)TITLE
      READ (INUNIT,END=998,ERR=998,FMT=*)NCL
      DO 30 I=1,5
30    READ(INUNIT,END=998,ERR=998,FMT=*)DL(I),LL1(I),LL2(I),LL3(I),
     1CF1(I),CF2(I),CF3(I),E(I)
      DO 40 I=1,5
40    READ(INUNIT,ERR=998,END=998,FMT=*)B(I),SF(I),W(I),WL(I),
     1              LF1(I),LF2(I),LF3(I),RST(I)
      DO 50 I=1,5
50    READ(INUNIT,ERR=998,END=998,FMT=*)EQ3(I),EQ4(I),ICE(I)
C
C  WRITE OUT DATA FOR ONE PROBLEM
C  HEADINGS FOR PAGE
C
      WRITE (OUNIT,1000)CARCNT, DATE, TIME
      WRITE (OUNIT,1001)REVDAT
      WRITE(OUNIT,1020)NCL,TITLE
1020  FORMAT(' ',/,10X,' LOAD NUMBER',I3,2X,A40)
      IF(KSL.EQ.0)WRITE(OUNIT,1030)BD1,BD2,BE1,BE2,R
      IF(KSL.EQ.1)WRITE(OUNIT,1031)BE1,BE2,R
      IF(KFOOT.EQ.0)WRITE(OUNIT,1050)
      IF(KFOOT.EQ.1)WRITE(OUNIT,1033)PFTG,AREA,ZL,ZT
      IF(KFOOT.EQ.2)WRITE(OUNIT,1036)PFTG,AREA,ZL,ZT
1030  FORMAT(1X,/,10X,' ULTIMATE LOAD GROUPS',
     1          /,10X,' BD1= ',F5.2,' BD2= ',F5.2,' BE1= ',F5.2,
     2            ' BE2= ',F5.2,' R= ',F5.2)
1031  FORMAT(1X,/,10X,' SERVICE LOAD GROUPS',
     1          /,10X,' BE1= ',F5.2,' BE2= ',F5.2,' R= ',F5.2)
1033  FORMAT(1X,/,10X,' P-FTG.= ',F6.1,' NO. PILES= ',F3.0,' ZL= ',
     1               F6.1,' ZT= ',F6.1,/)
1036  FORMAT(1X,/,10X,' P-FTG.= ',F6.1,' AREA= ',F5.1,' ZL= ',F6.1,
     1       ' ZT= ',F6.1,/)
      WRITE(OUNIT,*)'          LOAD      DEAD      LL+I A    LL+I B    L
     1L+I C     CF-A      CF-B      CF-C      EARTH'
      WRITE(OUNIT,1050)
1040  FORMAT(' ',/)
1050  FORMAT(' ')
      DO 55 MM=1,10
      MAXL(MM) =-5000000.
      MINL(MM) = 5000000.
      MAXLT(MM)=-5000000.
55    MINLT(MM)= 5000000.
      DO 60 I=1,5
C
C  WRITING OUT DATA
C
60    WRITE(OUNIT,1060)LD(I),DL(I),LL1(I),LL2(I),LL3(I),CF1(I),CF2(I),
     1           CF3(I),E(I)
      WRITE(OUNIT,1040)
      WRITE(OUNIT,*)'          LOAD         B         SF      WIND    W
     1LIVE      LF-A      LF-B      LF-C       RST'
      WRITE(OUNIT,1050)
      DO 70 I=1,5
70    WRITE(OUNIT,1060)LD(I),B(I),SF(I),W(I),WL(I),LF1(I),LF2(I),LF3(I),
     1RST(I)
      WRITE(OUNIT,1040)
      WRITE(OUNIT,*)'          LOAD       EQ-3      EQ-4       ICE'
      WRITE(OUNIT,1050)
      DO 80 I=1,5
80    WRITE(OUNIT,1070)LD(I),EQ3(I),EQ4(I),ICE(I)
1060  FORMAT(10X,A6,8F10.1)
1070  FORMAT(10X,A6,3F10.1)
      IF(KFOOT.NE.0)DL(1)=DL(1)+PFTG
C
C  END DATA WRITE
C
C  SET BETA VALUES
C
      BE(1)=BE1
      BE(2)=BE2
      BE(3)=BE1
      BE(4)=BE2
      BD(1)=BD1
      BD(2)=BD1
      BD(3)=BD2
      BD(4)=BD2
C
C  START DO LOOP FOR EACH SET OF BETAS
C
      DO 120 J=1,KLOOP
      IF(BD(J).EQ.0..OR.BE(J).EQ.0.)GO TO 120
C
C  START DO LOOP FOR EACH OF FIVE FORCE TYPES- P ML ETC.
C
      DO 100 K=1,5
C
C  ULTIMATE GROUP LOAD COMBINATIONS
C
      IF(KSL.EQ.0)THEN
      G(1,K)=DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)+SF(K)
      G(2,K)=DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)+SF(K)
      G(3,K)=DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)+SF(K)
      G(4,K)=1.3*(BD(J)*DL(K)+1.67*LL1(K)+CF1(K)+BE(J)*E(K)+B(K)
     @+SF(K))
      G(5,K)=1.3*(BD(J)*DL(K)+1.67*LL2(K)+CF2(K)+BE(J)*E(K)+B(K)
     @+SF(K))
      G(6,K)=1.3*(BD(J)*DL(K)+1.67*LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K))
      G(7,K)=1.3*(BD(J)*DL(K)+BE(J)*E(K)+B(K)+SF(K)+W(K))
      G(8,K)=1.3*(BD(J)*DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)+SF(K)
     @+.3*W(K)+WL(K)+LF1(K))
      G(9,K)=1.3*(BD(J)*DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)+SF(K)
     @+.3*W(K)+WL(K)+LF2(K))
      G(10,K)=1.3*(BD(J)*DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF3(K))
      G(11,K)=1.3*(BD(J)*DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)
     @+SF(K)+RST(K))
      G(12,K)=1.3*(BD(J)*DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)
     @+SF(K)+RST(K))
      G(13,K)=1.3*(BD(J)*DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K)+RST(K))
      G(14,K)=1.25*(BD(J)*DL(K)+BE(J)*E(K)+B(K)+SF(K)+W(K)
     @+RST(K))
      G(15,K)=1.25*(BD(J)*DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF1(K)+RST(K))
      G(16,K)=1.25*(BD(J)*DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF2(K)+RST(K))
      G(17,K)=1.25*(BD(J)*DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF3(K)+RST(K))
      EQT3=EQ3(K)
      EQT4=EQ4(K)
      IF(K.EQ.2.OR.K.EQ.3)THEN
          EQT3=EQT3/R
          EQT4=EQT4/R
      END IF
      G(18,K)=DL(K)+E(K)+B(K)+SF(K)+EQT3
      G(19,K)=DL(K)+E(K)+B(K)+SF(K)+EQT4
      G(20,K)=1.3*(BD(J)*DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)
     @+SF(K)+ICE(K))
      G(21,K)=1.3*(BD(J)*DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)
     @+SF(K)+ICE(K))
      G(22,K)=1.3*(BD(J)*DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K)+ICE(K))
      G(23,K)=1.2*(BD(J)*DL(K)+BE(J)*E(K)+B(K)+SF(K)+W(K)+ICE(K))
C
C SERVICE LOAD GROUPS
C
      ELSE
      G(1,K)=DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)+SF(K)
      G(2,K)=DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)+SF(K)
      G(3,K)=DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)+SF(K)
      G(4,K)=DL(K)+E(K)+B(K)+SF(K)+W(K)
      G(5,K)=DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)+SF(K)
     @+.3*W(K)+WL(K)+LF1(K)
      G(6,K)=DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)+SF(K)
     @+.3*W(K)+WL(K)+LF2(K)
      G(7,K)=DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF3(K)
      G(8,K)=DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)
     @+SF(K)+RST(K)
      G(9,K)=DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)
     @+SF(K)+RST(K)
      G(10,K)=DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K)+RST(K)
      G(11,K)=DL(K)+E(K)+B(K)+SF(K)+W(K)
     @+RST(K)
      G(12,K)=DL(K)+LL1(K)+CF1(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF1(K)+RST(K)
      G(13,K)=DL(K)+LL2(K)+CF2(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF2(K)+RST(K)
      G(14,K)=DL(K)+LL3(K)+CF3(K)+BE(J)*E(K)+B(K)
     @+SF(K)+.3*W(K)+WL(K)+LF3(K)+RST(K)
      EQT3=EQ3(K)
      EQT4=EQ4(K)
      IF(K.EQ.2.OR.K.EQ.3)THEN
          EQT3=EQT3/R
          EQT4=EQT4/R
      END IF
      G(15,K)=DL(K)+E(K)+B(K)+SF(K)+EQT3
      G(16,K)=DL(K)+E(K)+B(K)+SF(K)+EQT4
      G(17,K)=DL(K)+LL1(K)+CF1(K)+E(K)+B(K)
     @+SF(K)+ICE(K)
      G(18,K)=DL(K)+LL2(K)+CF2(K)+E(K)+B(K)
     @+SF(K)+ICE(K)
      G(19,K)=DL(K)+LL3(K)+CF3(K)+E(K)+B(K)
     @+SF(K)+ICE(K)
      G(20,K)=DL(K)+E(K)+B(K)+SF(K)+W(K)+ICE(K)
      END IF
C
C  FIND AND KEEP PARAMETERS FOR MAXIMUM ABSOLUTE LOAD TYPES.
C
      DO 95 M=1,KGROUP
         IF(G(M,K).GT.MAX(K)) MAX(K) =G(M,K)
         IF(G(M,K).GT.MAXL(K))MAXL(K)=G(M,K)
         IF(G(M,K).LT.MIN(K)) MIN(K) =G(M,K)
 95      IF(G(M,K).LT.MINL(K))MINL(K)=G(M,K)
100   CONTINUE
C
C   PRINT TITLE FOR GROUP LOAD LISTING.
C
      WRITE (OUNIT,1000)CARCNT, DATE, TIME
      WRITE (OUNIT,1001)REVDAT
      IF(KSL.EQ.0)WRITE(OUNIT,1080) BD(J), BE(J), R, NCL, TITLE
      IF(KSL.EQ.1)WRITE(OUNIT,1081) BE(J), R, NCL, TITLE
1080  FORMAT(' ',/,10X,' BD= ',F5.2,' BE= ',F5.2,' R= ',F5.2,' LOAD NO.
     1',I2,2X,A40,/)
1081  FORMAT(' ',/,10X,' BE= ',F5.2,' R= ',F5.2,' LOAD NO. ',
     1       I2,2X,A40,/)
C  FOR NO FOOTING:
      IF(KFOOT.EQ.0)THEN
      WRITE(OUNIT,*)'          GROUP        P         ML        MT
     1  HL        HT'
      WRITE(OUNIT,1050)
      END IF
C  FOR PILE FOOTING:
      IF(KFOOT.EQ.1)THEN
      WRITE(OUNIT,1083) PFTG, AREA, ZL, ZT
1083  FORMAT(10X,' P-FTG.= ',F6.1,' NO. PILES= ',F3.0,' ZL= ',F6.1,
     1 ' ZT= ',F6.1,//)
      WRITE(OUNIT,*)'          GROUP        P         ML        MT
     1HL      HT     ML/P    MT/P    PMAX    PMIN'
      WRITE(OUNIT,1050)
      END IF
C  FOR SPREAD FOOTING:
      IF(KFOOT.EQ.2)THEN
      WRITE(OUNIT,1086) PFTG, AREA, ZL, ZT
1086  FORMAT(10X,' P-FTG.= ',F6.1,' AREA= ',F5.1,' ZL= ',F6.1,' ZT= '
     1,F6.1,//)
      WRITE(OUNIT,*)'          GROUP        P         ML        MT
     1HL      HT     ML/P    MT/P    HR/P    SMAX'
      WRITE(OUNIT,1050)
      END IF
C  STRESS TESTS FOR ECCENTRICITY ETC.
      DO 115 N=1,KGROUP
      XX1=ABS(G(N,1))
      XX2=ABS(G(N,2))
      XX3=ABS(G(N,3))
      XX4=G(N,4)
      XX5=G(N,5)
      IF(KFOOT.GT.0)THEN
      XX(6)=XX2/XX1
      XX(7)=XX3/XX1
      XT(6)=XX(6)
      XT(7)=XX(7)
C
C  ADJUST ECCENTRICITY FOR ULTIMATE LOADS BACK TO SL FROM ULTIMATE.
C
           IF(N.GE.4.AND.KSL.EQ.0)THEN
           XT(6)=XX(6)/2.
           XT(7)=XX(7)/2.
           END IF
      XX(8)=(XX4*XX4+XX5*XX5)**0.5/XX1
      XT(8)=XX(8)
      XX(9)=XX1/AREA+XX2/ZL+XX3/ZT
      XX(10)=XX1/AREA-XX2/ZL-XX3/ZT
      XT(9)=XX(9)
      XT(10)=XX(10)
C
C  ADJUST PILE LOADS BACK TO PILE CAPACITY FROM ULTIMATE.
C
           IF(KFOOT.EQ.1.AND.KSL.EQ.0)THEN
              IF(N.GE.4.AND.N.LE.13)XT(9)=XX(9)/1.5
              IF(N.GE.4.AND.N.LE.13)XT(10)=XX(10)/1.5
              IF(N.GE.14)XT(9) =XX(9) /1.75
              IF(N.GE.14)XT(10)=XX(10)/1.75
           END IF
C
C  ADJUST SPREAD FOOTING PRESSURES BACK TO SERVICE LOADS FROM ULT.
C
           IF(KFOOT.EQ.2.AND.N.GE.4.AND.KSL.EQ.0)THEN
              XT(9) =XX(9) /2.
              XT(10)=XX(10)/2.
           END IF
C
C  ADJUST FOOTING SERVICE LOADS BACK TO GROUP 1.
C
           IF(KSL.EQ.1.AND.KFOOT.NE.0)THEN
             IF(N.GE.4.AND.N.LE.10)THEN
             XT(9)=XX(9)/1.25
             XT(10)=XX(10)/1.25
             END IF
             IF(N.EQ.15.OR.N.EQ.16)THEN
             XT(9)=XX(9)/1.33
             XT(10)=XX(10)/1.33
             END IF
             IF(N.GE.11.AND.N.LE.14.OR.N.GE.17.AND.N.LE.19)THEN
             XT(9)=XX(9)/1.4
             XT(10)=XX(10)/1.4
             END IF
             IF(N.EQ.20)THEN
             XT(9)=XX(9)/1.5
             XT(10)=XX(10)/1.50
             END IF
           END IF
C
C  DELETE NEGATIVE GROUND PRESSURES.
C
           IF(KFOOT.EQ.2.AND.XX(10).LT.0.)THEN
           XX(9)=-50000000.
           XT(9)=-50000000.
           END IF
      END IF
C
C  WRITE OUTPUT.
C  FOR NO FOOTING
C
      IF(KFOOT.EQ.0)WRITE(OUNIT,1090)GP(N),(G(N,II),II=1,5)
1090  FORMAT(11X,A6,5F10.1,3F10.3)
C  FOR PILE FOOTINGS
      IF(KFOOT.EQ.1)WRITE(OUNIT,1093)GP(N),(G(N,II),II=1,5)
     1                                ,XX(6),XX(7),XX(9),XX(10)
1093  FORMAT(11X,A6,3F10.1,2F8.1,2F8.3,2F8.1)
C  FOR SPREAD FOOTINGS
      IF(KFOOT.EQ.2)WRITE(OUNIT,1096)GP(N),(G(N,II),II=1,5)
     1                            ,(XX(II),II=6,9)
1096  FORMAT(11X,A6,3F10.1,2F8.1,3F8.3,2X,F6.2)
      IF(KFOOT.EQ.0)GO TO 115
C
C FIND MAXIMUM STRESS CASES
C
      DO 110 II=6,10
         IF(XT(II).GT.MAXT(II))THEN
            MAXT(II)=XT(II)
            MAX(II) =XX(II)
         END IF
         IF(XT(II).GT.MAXLT(II))THEN
            MAXLT(II)=XT(II)
            MAXL(II) =XX(II)
         END IF
         IF(XT(II).LT.MINT(II))THEN
            MINT(II)=XT(II)
            MIN(II) =XX(II)
         END IF
         IF(XT(II).LT.MINLT(II))THEN
            MINLT(II)=XT(II)
            MINL(II) =XX(II)
         END IF
110   CONTINUE
115   CONTINUE
120   CONTINUE
C
C  WRITE OUTPUT.
C  FOR NO FOOTING
C
      WRITE(OUNIT,'(/,10X,'' MAXIMUMS AND MINIMUMS FOR LOAD'',I3,/,'' ''
     1)')NCL
      IF(KFOOT.EQ.0)THEN
         WRITE(OUNIT,1090)MAXCH,(MAXL(II),II=1,5)
         WRITE(OUNIT,1090)MINCH,(MINL(II),II=1,5)
      END IF
C  FOR PILE FOOTINGS
      IF(KFOOT.EQ.1)THEN
         WRITE(OUNIT,1093)MAXCH,(MAXL(II),II=1,7),MAXL(9),MAXL(10)
         WRITE(OUNIT,1093)MINCH,(MINL(II),II=1,7),MINL(9),MINL(10)
      END IF
C  FOR SPREAD FOOTINGS
      IF(KFOOT.EQ.2)THEN
         WRITE(OUNIT,1096)MAXCH,(MAXL(II),II=1,9)
         WRITE(OUNIT,1096)MINCH,(MINL(II),II=1,9)
      END IF
      GO TO 20
C
C  WRITE OUTPUT.
C  FOR NO FOOTING
C
 130  WRITE(OUNIT,'(/,10X,'' MAXIMUMS AND MINIMUMS FOR ALL LOADS THIS RU
     1N'',/,'' '')')
      IF(KFOOT.EQ.0)THEN
         WRITE(OUNIT,1090)MAXCH,(MAX(II),II=1,5)
         WRITE(OUNIT,1090)MINCH,(MIN(II),II=1,5)
      END IF
C  FOR PILE FOOTINGS
      IF(KFOOT.EQ.1)THEN
         WRITE(OUNIT,1093)MAXCH,(MAX(II),II=1,7),MAX(9),MAX(10)
         WRITE(OUNIT,1093)MINCH,(MIN(II),II=1,7),MIN(9),MIN(10)
      END IF
C  FOR SPREAD FOOTINGS
      IF(KFOOT.EQ.2)THEN
         WRITE(OUNIT,1096)MAXCH,(MAX(II),II=1,9)
         WRITE(OUNIT,1096)MINCH,(MIN(II),II=1,9)
      END IF
C  IF PC VERSION RESET PRINTER TO NORMAL. NOTE COMMAND AT BEG. OF
C  PROGRAM TO SET PRINTER TO COMPRESSED.
C  ACTUAL CHARACTERS NOT IN MAIN FRAME VERSION
C
C      IF(ERUNIT.EQ.3)WRITE(OUNIT,'('' '')')
C
      GO TO 999
998   WRITE(*,*)'INPUT ERROR!!  CHECK YOUR DATA SET!!'                    T  
 
999   STOP
      END
C
C
C
C
C
C
      SUBROUTINE PARM(OUNIT,ERUNIT,CARCNT,INUNIT)
      CHARACTER*16 FLNM1, FLNM2
      CHARACTER*1 CARCNT
      INTEGER OUNIT, ERUNIT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  STANDARD INTERACTIVE INPUT UNIT
      IIUNIT=1
C  STANDARD INTERACTIVE OUTPUT UNIT
      IOUNIT=3
C  STANDARD REPORT OUTPUT UNIT
      OUNIT=6
C  STANDARD FILE INPUT UNIT
      INUNIT=5
C  STANDARD ERROR OUTPUT UNIT
      ERUNIT=3
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C      OPEN(IIUNIT,FILE='CON')                                            T
C      OPEN(IOUNIT,FILE='CON')                                            T
10    WRITE(*,210)                                                        T
210   FORMAT(//,
     1' Enter input file name:',/)
           READ(*,ERR=10,FMT='(A)')FLNM1                                  T
           IF(FLNM1.EQ.'CON')GO TO 25                                     T 
           IF(FLNM1.EQ.'con')GO TO 25                                     T 
              DO 15 I=1,16
               IF(FLNM1(I:I).EQ.'.')GO TO 25
15             CONTINUE
              DO 20 I=1,16
                 IF(FLNM1(I:I).EQ.' ')THEN
                  IE = I+3
                  FLNM1(I:IE) = '.dat'
                  GO TO 25
                 END IF
20             CONTINUE
25         WRITE(IOUNIT,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1   T
          CARCNT='1'
35    WRITE(*,235)                                                        T
235   FORMAT(//,
     1' Enter output file name:',/)
           READ(*,ERR=35,FMT='(A)')FLNM2                                  T
           IF(FLNM2.EQ.'CON')GO TO 60                                     T
           IF(FLNM2.EQ.'con')GO TO 60                                     T  
              DO 40 I=1,16
               IF(FLNM2(I:I).EQ.'.')GO TO 60
40             CONTINUE
              DO 50 I=1,16
                 IF(FLNM2(I:I).EQ.' ')THEN
                  IE = I+3
                  FLNM2(I:IE) = '.out'
                  GO TO 60
                 END IF
50             CONTINUE
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2      T
          OPEN(OUNIT,FILE=FLNM2)
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6
      RETURN
      END
C
C
