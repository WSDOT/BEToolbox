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

C    YLDPLO.FOR
C
C                             P63926
C
C                  MODIFIED FOR USE AT W.S.D.O.T.
C                               BY
C                           PAUL WOCKEN
C                          JULY 18, 1984
C
C     REVISION LOG:
C
C     REVISION .A. BY PAUL WOCKEN JULY, 1984
C            CHANGES MADE FOR USE AT W.S.D.O.T.
C
C     GENERAL DATA ANALYSIS AND PLOTTING PROGRAM                        GEN00010
C     MODIFIED FOR CALCOMP 1038 MODEL 01/24/80 BDEGENPLT1 LEVEL 08      GEN00020
C
C     REVISION .B. BY TERRY BEEBE MARCH, 1990                           .B.
C            CONVERSION TO VMS AND CALCOMP 52224 MODEL                  .B.
C
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C***********************************************************************GEN00030
C     SPECIFICATION STATEMENTS                                          GEN00040
C                                                                       GEN00050
      DIMENSION SBCD(42,10),TBCD(42,10),BCD(42),USE(10),                .B.00060
     *          BYTE(4),VBCD(42),PBCD(42),XR(4),YR(4),XPI(102),YPI(102),GEN00070
     *          CARD(20)                                                GEN00080
      LOGICAL*1 SBCD,VBCD,PBCD,TBCD,USE,BYTE,BCD                        GEN00090
      DATA NPLOTS/0/,NYAX/0/                                            GEN00100
C     EQUIVALENCE (KODE,BYTE)                                           .B.00110
      INTEGER*4 IDUM                                                    .B.
      CHARACTER*8 REVDATE,TIME                                          .B.
      CHARACTER*11 DATE                                                 .B.
      CHARACTER*23 DATIME                                               .B.
      CHARACTER*4  COST,TELE                                            .B.
      CHARACTER*12 NAME                                                 .B.
      CHARACTER*1  KODE                                                 .B.
      CHARACTER*24 OUTP                                                 .B.
      CHARACTER    TXT(42),PTXT(42)                                     .B.
      CHARACTER    STXT(42,10),TTXT(42,10)                              .B.
      INUNIT=5                                                          .B.
      OPEN (INUNIT,FILE='YIELD.PLT',STATUS='OLD')                       .B.
C     CALL PARM                                                         .B.
      CALL LIB$DATE_TIME (DATIME)                                       .B.
      TIME = DATIME(13:20)                                              .B.
      DATE = DATIME(1:11)                                               .B.
C***********************************************************************GEN00130
C     FORMAT STATEMENTS                                                 GEN00140
C                                                                       GEN00150
  501 FORMAT(1X,A4,1(' '),A12,1(' '),A4,1(' '),A24,5F5.1)               .B.00160
  502 FORMAT(1X,A4,1(' '),A12,1(' '),A4,1(' '),A24,5F5.1)               .B.00170
  505 FORMAT(1X,A1,2I1,2(' '),3F8.0,1(' '),42A1,T1,20A4)                .B.00180
  601 FORMAT(1X,1H1)                                                    .B.00190
  602 FORMAT(10(' '),20A4)                                              .B.00200
C                                                                       GEN00210
C***********************************************************************GEN00220
C     READ PLOT SPECIFICATIONS AND INITIALIZE PLOT ROUTINES             GEN00230
C                                                                       GEN00240
       READ(INUNIT,501) COST,NAME,TELE,OUTP,WIDT,HEIT                   .B.00250
       WRITE(6,502) COST,NAME,TELE,OUTP,WIDT,HEIT                       .B.00260
       IF(HEIT.GT.11.0) KCMP=30                                         .B.00270
       IF(HEIT.LE.11.0) KCMP=11                                         .B.00280
       CALL PLOTS(0,0,8)                                                .B.
C      CALL PSTART('YIELDGEN',COST,KCMP,1,NAME,TELE)                    .B.00290
C      CALL WHOM(NAME,TELE)                                             .B.00300
C      CALL WHENCE(OUTP)                                                .B.00310
C                                                                       GEN00320
C***********************************************************************GEN00330
C     READ DETAIL CARD, CONVERT BCD AND BRANCH ON CODE                  .B.00340
C                                                                       GEN00350
   20 READ(INUNIT,505,END=227) KODE,N1,N2,D1,D2,D3,BCD,CARD             .B.00360
      IF(KODE.EQ.'T') WRITE(6,601)                                      .B.00370
      WRITE(6,602) CARD                                                 .B.00380
C                                                                       .B.
C     CONVERT BCD CODE TO TEXT                                          .B.  
C                                                                       .B.
      DO 25 I=1,42                                                      .B.
      IF(BCD(I).EQ.32) THEN                                             .B.
         TXT(I)=' '                                                     .B.
      ELSE IF(BCD(I).EQ.33) THEN                                        .B.  
         TXT(I)='!'                                                     .B.
      ELSE IF(BCD(I).EQ.34) THEN                                        .B. 
         TXT(I)='"'                                                     .B.
      ELSE IF(BCD(I).EQ.35) THEN                                        .B.
         TXT(I)='#'                                                     .B. 
      ELSE IF(BCD(I).EQ.40) THEN                                        .B.
         TXT(I)='('                                                     .B.
      ELSE IF(BCD(I).EQ.41) THEN                                        .B.
         TXT(I)=')'                                                     .B.
      ELSE IF(BCD(I).EQ.42) THEN                                        .B. 
         TXT(I)='*'                                                     .B.
      ELSE IF(BCD(I).EQ.43) THEN                                        .B.  
         TXT(I)='+'                                                     .B.
      ELSE IF(BCD(I).EQ.45) THEN                                        .B.
         TXT(I)='-'                                                     .B.
      ELSE IF(BCD(I).EQ.46) THEN                                        .B.
         TXT(I)='.'                                                     .B.
      ELSE IF(BCD(I).EQ.47) THEN                                        .B.
         TXT(I)='/'                                                     .B.
      ELSE IF(BCD(I).EQ.48) THEN                                        .B. 
         TXT(I)='0'                                                     .B.
      ELSE IF(BCD(I).EQ.49) THEN                                        .B.
         TXT(I)='1'                                                     .B.
      ELSE IF(BCD(I).EQ.50) THEN                                        .B.
         TXT(I)='2'                                                     .B.
      ELSE IF(BCD(I).EQ.51) THEN                                        .B.
         TXT(I)='3'                                                     .B.
      ELSE IF(BCD(I).EQ.52) THEN                                        .B.
         TXT(I)='4'                                                     .B.
      ELSE IF(BCD(I).EQ.53) THEN                                        .B.
         TXT(I)='5'                                                     .B.
      ELSE IF(BCD(I).EQ.54) THEN                                        .B.
         TXT(I)='6'                                                     .B. 
      ELSE IF(BCD(I).EQ.55) THEN                                        .B.
         TXT(I)='7'                                                     .B.
      ELSE IF(BCD(I).EQ.56) THEN                                        .B.
         TXT(I)='8'                                                     .B.
      ELSE IF(BCD(I).EQ.57) THEN                                        .B. 
         TXT(I)='9'                                                     .B. 
      ELSE IF(BCD(I).EQ.58) THEN                                        .B.
         TXT(I)=':'                                                     .B.
      ELSE IF(BCD(I).EQ.59) THEN                                        .B. 
         TXT(I)=';'                                                     .B. 
      ELSE IF(BCD(I).EQ.60) THEN                                        .B. 
         TXT(I)='<'                                                     .B.
      ELSE IF(BCD(I).EQ.61) THEN                                        .B.
         TXT(I)='='                                                     .B.
      ELSE IF(BCD(I).EQ.62) THEN                                        .B. 
         TXT(I)='>'                                                     .B.
      ELSE IF(BCD(I).EQ.64) THEN                                        .B.
         TXT(I)='@'                                                     .B. 
      ELSE IF(BCD(I).EQ.65) THEN                                        .B.
         TXT(I)='A'                                                     .B.
      ELSE IF(BCD(I).EQ.66) THEN                                        .B.
         TXT(I)='B'                                                     .B.
      ELSE IF(BCD(I).EQ.67) THEN                                        .B. 
         TXT(I)='C'                                                     .B.
      ELSE IF(BCD(I).EQ.68) THEN                                        .B.
         TXT(I)='D'                                                     .B.
      ELSE IF(BCD(I).EQ.69) THEN                                        .B.
         TXT(I)='E'                                                     .B. 
      ELSE IF(BCD(I).EQ.70) THEN                                        .B. 
         TXT(I)='F'                                                     .B.
      ELSE IF(BCD(I).EQ.71) THEN                                        .B.
         TXT(I)='G'                                                     .B.
      ELSE IF(BCD(I).EQ.72) THEN                                        .B. 
         TXT(I)='H'                                                     .B.
      ELSE IF(BCD(I).EQ.73) THEN                                        .B. 
         TXT(I)='I'                                                     .B.
      ELSE IF(BCD(I).EQ.74) THEN                                        .B.
         TXT(I)='J'                                                     .B.
      ELSE IF(BCD(I).EQ.75) THEN                                        .B. 
         TXT(I)='K'                                                     .B.
      ELSE IF(BCD(I).EQ.76) THEN                                        .B. 
         TXT(I)='L'                                                     .B.
      ELSE IF(BCD(I).EQ.77) THEN                                        .B.
         TXT(I)='M'                                                     .B.
      ELSE IF(BCD(I).EQ.78) THEN                                        .B.
         TXT(I)='N'                                                     .B.
      ELSE IF(BCD(I).EQ.79) THEN                                        .B.
         TXT(I)='O'                                                     .B.
      ELSE IF(BCD(I).EQ.80) THEN                                        .B.
         TXT(I)='P'                                                     .B.
      ELSE IF(BCD(I).EQ.81) THEN                                        .B.
         TXT(I)='Q'                                                     .B.
      ELSE IF(BCD(I).EQ.82) THEN                                        .B. 
         TXT(I)='R'                                                     .B.
      ELSE IF(BCD(I).EQ.83) THEN                                        .B.
         TXT(I)='S'                                                     .B.
      ELSE IF(BCD(I).EQ.84) THEN                                        .B.
         TXT(I)='T'                                                     .B.
      ELSE IF(BCD(I).EQ.85) THEN                                        .B.
         TXT(I)='U'                                                     .B.
      ELSE IF(BCD(I).EQ.86) THEN                                        .B.
         TXT(I)='V'                                                     .B.
      ELSE IF(BCD(I).EQ.87) THEN                                        .B.  
         TXT(I)='W'                                                     .B.
      ELSE IF(BCD(I).EQ.88) THEN                                        .B.
         TXT(I)='X'                                                     .B.
      ELSE IF(BCD(I).EQ.89) THEN                                        .B.
         TXT(I)='Y'                                                     .B.
      ELSE IF(BCD(I).EQ.90) THEN                                        .B. 
         TXT(I)='Z'                                                     .B. 
      ELSE IF(BCD(I).LT.32) THEN                                        .B.
         TXT(I)=' '                                                     .B.
      ELSE IF(BCD(I).GT.90) THEN                                        .B.
         TXT(I)=' '                                                     .B.
      END IF                                                            .B. 
 25   CONTINUE                                                          .B.
C                                                                       .B.
C     BRANCH ON KODE                                                    .B.
C                                                                       .B.
      IF(KODE.EQ.'B') GO TO 194                                         .B.00390
      IF(KODE.EQ.'C') GO TO 195                                         .B.00400
      IF(KODE.EQ.'E') GO TO 197                                         .B.00410
      IF(KODE.EQ.'H') GO TO 200                                         .B.00420
      IF(KODE.EQ.'L') GO TO 211                                         .B.00430
      IF(KODE.EQ.'N') GO TO 213                                         .B.00440
      IF(KODE.EQ.'P') GO TO 215                                         .B.00450
      IF(KODE.EQ.'R') GO TO 217                                         .B.00460
      IF(KODE.EQ.'S') GO TO 226                                         .B.00470
      IF(KODE.EQ.'T') GO TO 227                                         .B.00480
      IF(KODE.EQ.'X') GO TO 231                                         .B.00490
      IF(KODE.EQ.'Y') GO TO 232                                         .B.00500
      IF(KODE.LT.'0') GO TO 20                                          .B.00510
      IF(KODE.GT.'9') GO TO 20                                          .B.00520
C                                                                       GEN00530
C***********************************************************************GEN00540
C     PLOT AND STORE A DATA POINT                           CODE=NUMERICGEN00550
C                                                                       GEN00560
      XBCD = (D1-XH1)/DXH                                               GEN00570
      YBCD = (D2-YV1)/DYV                                               GEN00580
C     KSM = KODE-240                                                    .B.00590
      IF(KODE.EQ.'0') THEN                                              .B.
         KSM = 0                                                        .B.
      ELSE IF(KODE.EQ.'1') THEN                                         .B.
         KSM = 1                                                        .B.
      ELSE IF(KODE.EQ.'2') THEN                                         .B.
         KSM = 2                                                        .B. 
      ELSE IF(KODE.EQ.'3') THEN                                         .B.
         KSM = 3                                                        .B. 
      ELSE IF(KODE.EQ.'4') THEN                                         .B.
         KSM = 4                                                        .B.
      ELSE IF(KODE.EQ.'5') THEN                                         .B.
         KSM = 5                                                        .B.
      ELSE IF(KODE.EQ.'6') THEN                                         .B.
         KSM = 6                                                        .B.
      ELSE IF(KODE.EQ.'7') THEN                                         .B.
         KSM = 7                                                        .B.
      ELSE IF(KODE.EQ.'8') THEN                                         .B.
         KSM = 8                                                        .B.   
      ELSE IF(KODE.EQ.'9') THEN                                         .B.
         KSM = 9                                                        .B. 
      END IF                                                            .B.
C
C============================================= 
C     CHANGING -ICODE (-1) TO (-2) AFFECTS LINE PLOTTING IN THE
C     FOLLOWING SYMBOL CALL
C         -1 = PEN UP
C         -2 = PEN DOWN
C
C     CALL SYMBOL(XBCD,YBCD,0.08,KSM,0.0,-1)                            .B.00600
      CALL SYMBOL(XBCD,YBCD,0.08,IDUM,KSM,0.0,-1)                       .B.
      NP = NP+1                                                         GEN00610
      XPI(NP) = D1                                                      GEN00620
      YPI(NP) = D2                                                      GEN00630
      GO TO 20                                                          GEN00640
C                                                                       GEN00650
C***********************************************************************GEN00660
C     STORE PLOT BORDER SPECIFICATIONS                      CODE=194 (B)GEN00670
C                                                                       GEN00680
 194  NWTL = N1                                                         GEN00690
      BWID = D1                                                         GEN00700
      GO TO 20                                                          GEN00710
C                                                                       GEN00720
C***********************************************************************GEN00730
C     STORE CALCOMP SYMBOL IDENTIFICATION                   CODE=195 (C)GEN00740
C                                                                       GEN00750
 195  K = N1+1                                                          GEN00760
      DO 1951 I=1,42                                                    GEN00770
      SBCD(I,K) = BCD(I)                                                .B.00780
 1951 STXT(I,K) = TXT(I)                                                .B.
      USE(K) = .TRUE.                                                   GEN00790
      GO TO 20                                                          GEN00800
C                                                                       GEN00810
C***********************************************************************GEN00820
C     DRAW EFFECTIVE DENSITY SCALE                          CODE=197 (E)GEN00830
C                                                                       GEN00840
 197  EZERO = D1                                                        GEN00850
      XED = 0.0                                                         GEN00860
      EDS = D2                                                          GEN00870
      YED = D3                                                          GEN00880
      ED1 = -YED*EDS                                                    GEN00890
      EMIN = 0.0                                                        GEN00900
      EMAX = 1.5*EDS                                                    GEN00910
      XPEN = XPEN-0.10                                                  GEN00920
      CALL PLOT(XPEN-0.20,YED,3)                                        GEN00930
      CALL PLOT(XPEN-0.16,YED,2)                                        GEN00940
      CALL PLOT(XPEN-0.20,YED+1.0,3)                                    GEN00950
      CALL PLOT(XPEN-0.16,YED+1.0,2)                                    GEN00960
      CALL SYMBOL(XPEN,YED-3.01,0.10,'EFFECTIVE DENSITY (PCF)',IDUM,    .B.00970
     *90.0,23)                                                          .B.
      CALL PLOT(XPEN-0.05,YED-1.03,3)                                   GEN00980
      CALL POINT(XPEN-0.05,YED-0.23,0.20)                               GEN00990
      CALL SYMBOL(XPEN,YED-0.11,0.10,'0.00',IDUM,0.0,4)                 .B.01000
      CALL NUMBER(XPEN,YED+0.89,0.10,EDS,I,90.0,2)                      .B.01010
      XPEN = XPEN+0.10                                                  GEN01020
      GO TO 20                                                          GEN01030
C                                                                       GEN01040
C***********************************************************************GEN01050
C     HOLD CONSTANT EMBANKMENT ELEVATION                    CODE=200 (H)GEN01060
C                                                                       GEN01070
 200  XED = D1                                                          GEN01080
      GO TO 20                                                          GEN01090
C                                                                       GEN01100
C***********************************************************************GEN01110
C     DRAW LINE THROUGH ACCUMULATED POINTS                  CODE=211 (L)GEN01120
C                                                                       GEN01130
 211  XPI(NP+1) = XH1                                                   GEN01140
      XPI(NP+2) = DXH                                                   GEN01150
      YPI(NP+1) = YV1                                                   GEN01160
      YPI(NP+2) = DYV                                                   GEN01170
      IF(N1.EQ.4) NED = NED+1                                           GEN01180
      IF(D1.EQ.0.0) D1=XH1                                              GEN01190
      IF(D2.EQ.0.0) D2=XH2                                              GEN01200
      CALL CLINE(XPI,YPI,NP,N1,N2,D1,D2,XED,NED.EQ.1,EZERO,             GEN01210
     *           YMIN,YMAX,EMIN,EMAX,ED1,EDS)                           GEN01220
      NP = 0                                                            GEN01230
      GO TO 20                                                          GEN01240
C                                                                       GEN01250
C***********************************************************************GEN01260
C     PLOT A NOTE                                           CODE=213 (N)GEN01270
C                                                                       GEN01280
 213  SIZE = 0.07+0.035*N1                                              GEN01290
      IF(D2.LT.YBMIN) YBMIN=D2                                          GEN01300
      CALL SYMBOL(D1,D2,SIZE,TXT,IDUM,D3,42)                            .B.01310
      GO TO 20                                                          GEN01320
C                                                                       GEN01330
C***********************************************************************GEN01340
C     CHANGE VERTICAL SCALING PARAMETERS                    CODE=215 (P).B.01350
C                                                                       GEN01360
 215  YV1 = D1                                                          GEN01370
      DYV = D2                                                          GEN01380
      GO TO 20                                                          GEN01390
C                                                                       GEN01400
C***********************************************************************GEN01410
C     DRAW A REFERENCE LINE                                 CODE=217 (R)GEN01420
C                                                                       GEN01430
 217  YR(1) = D2                                                        GEN01440
      YR(2) = D3                                                        GEN01450
      YR(3) = 0.0                                                       GEN01460
      YR(4) = 1.0                                                       GEN01470
      XR(2) = D1                                                        GEN01480
      XR(1) = D1                                                        GEN01490
      XR(3) = XH1                                                       GEN01500
      XR(4) = DXH                                                       GEN01510
      CALL LINT(XR,YR,2,1,0.025,0.025)                                  GEN01520
      XBCD = (D1-XH1)/DXH-0.02                                          GEN01530
      SIZE = 0.07+0.035*N1                                              GEN01540
C     CALL SYMBOL(XBCD,YR(1)+0.10,SIZE,TXT,IDUM,90.0,42)                .B.01550
      GO TO 20                                                          GEN01560
C                                                                       GEN01570
C***********************************************************************GEN01580
C     STORE A LINE OF SUBTITLE                              CODE=226 (S)GEN01590
C                                                                       GEN01600
 226  IF(NLT.EQ.10) GO TO 20                                            GEN01610
      NLT = NLT+1                                                       GEN01620
      DO 2261 I=1,42                                                    GEN01630
      TBCD(I,NLT) = BCD(I)                                              .B.01640
 2261 TTXT(I,NLT) = TXT(I)                                              .B. 
      GO TO 20                                                          GEN01650
C                                                                       GEN01660
C***********************************************************************GEN01670
C     END OF CURRENT PLOT/START OF NEW PLOT                 CODE=227 (T)GEN01680
C                                                                       GEN01690
 227  IF(NPLOTS.EQ.0) GO TO 2277                                        GEN01700
C                                                                       GEN01710
C***********************************************************************GEN01720
C     DRAW BORDER AND ANNOTATION                            CURRENT PLOTGEN01730
C                                                                       GEN01740
      CALL PLOT(XPEN,0.0,-3)                                            GEN01750
      IF(NWTL.EQ.0) GO TO 2272                                          GEN01760
      XB1 = -AXLE -(NYAX*0.70) -BWID                                    GEN01770
      XB2 = BWID                                                        GEN01780
      YB1 = AMAX1(YBMIN-BWID,-HTHX-0.005*NWTL)                          GEN01790
      YB2 = AXHT+BWID                                                   GEN01800
      DO 2271 I=1,NWTL                                                  GEN01810
      CALL PLOT(XB1,YB1,3)                                              GEN01820
      CALL PLOT(XB1,YB2,2)                                              GEN01830
      CALL PLOT(XB2,YB2,2)                                              GEN01840
      CALL PLOT(XB2,YB1,2)                                              GEN01850
      CALL PLOT(XB1,YB1,2)                                              GEN01860
      XB1 = XB1-0.005                                                   GEN01870
      YB1 = YB1-0.005                                                   GEN01880
      XB2 = XB2+0.005                                                   GEN01890
 2271 YB2 = YB2+0.005                                                   GEN01900
 2272 CALL CENTER(-AXLE/2.0,AXHT-0.07,0.14,PTXT,0.0,42)                 .B.01910
      YBCD = AXHT-0.28                                                  GEN01920
      IF(NLT.EQ.0) GO TO 2274                                           GEN01930
      DO 2273 I=1,NLT                                                   GEN01940
      CALL CENTER(-AXLE/2.0,YBCD,0.105,TTXT(1,I),0.0,42)                .B.01950
 2273 YBCD = YBCD-0.15                                                  GEN01960
 2274 YBCD = YBCD-0.25                                                  GEN01970
      XBCD = 0.07-AXLE*0.5                                              GEN01980
      DO 2275 I=1,10                                                    GEN01990
      IF(.NOT.USE(I)) GO TO 2275                                        GEN02000
      CALL CENTER(XBCD,YBCD,0.07,STXT(1,I),0.0,42)                      .B.02010
      CALL WHERE(XNOW,YNOW,RFACT)                                       .B.02020
C     CALL SYMBOL(-XNOW-AXLE,YBCD,0.08,I-1,0.0,-1)                      .B.02020
      CALL SYMBOL(-XNOW-AXLE,YBCD,0.08,IDUM,I-1,0.0,-1)                 .B.
      YBCD = YBCD-0.125                                                 GEN02040
 2275 CONTINUE                                                          GEN02050
      IF(EDS.EQ.0.0) GO TO 2276                                         GEN02060
      CALL CENTER(XBCD,YBCD,0.07,'EFFECTIVE DENSITY',0.0,17)            GEN02070
      CALL WHERE(XNOW,YNOW,RFACT)                                       .B.02080
      CALL SYMBOL(-AXLE-XNOW,YBCD,0.08,10,IDUM,0.0,-1)                  .B.02090
 2276 XOFF = AXLE+AMRL+(NYAX*0.7)                                       GEN02100
      NYAX = 0                                                          GEN02110
      CALL PLOT(-XOFF,0.0,-3)                                           GEN02120
      CALL PLOT(WIDT,-10.0,-3)                                          GEN02130
      IF(KODE.EQ.'T') GO TO 2277                                        .B.02140
C                                                                       GEN02150
C***********************************************************************GEN02160
C     CLOSE PLOTTAPE AND TERMINATE EXECUTION                 END OF DATAGEN02170
C                                                                       GEN02180
      CALL PLOT(0.0,0.1,2)                                              GEN02190
      CALL PLOT(-.1,HEIT,3)                                             GEN02200
      CALL PLOT(0.1,HEIT,2)                                             GEN02210
      CALL PLOT(0.0,HEIT,3)                                             GEN02220
      CALL PLOT(0.0,HEIT-0.1,2)                                         GEN02230
      END FILE 6                                                        GEN02240
      CALL PLOT(12.0,0.0,999)                                           .B.
C      CALL PSTOP(3.0,NPLOTS,WIDT/12.0,400.)                            .B.02250
      STOP                                                              GEN02260
C                                                                       GEN02270
C***********************************************************************GEN02280
C     STORE TITLE AND INITIALIZE                       START OF NEW PLOTGEN02290
C                                                                       GEN02300
 2277 DO 2278 I=1,42                                                    GEN02310
      PBCD(I) = BCD(I)                                                  .B.02320
 2278 PTXT(I) = TXT(I)                                                  .B.
      AMRL = D1                                                         GEN02330
      AMRT = D2                                                         GEN02340
      HTHX = D3                                                         GEN02350
      CALL PLOT(0.0,0.1,2)                                              GEN02360
      CALL PLOT(-.1,HEIT,3)                                             GEN02370
      CALL PLOT(0.1,HEIT,2)                                             GEN02380
      CALL PLOT(0.0,HEIT,3)                                             GEN02390
      CALL PLOT(0.0,HEIT-0.1,2)                                         GEN02400
      CALL PLOT(AMRL,HTHX,-3)                                           GEN02410
      XPEN = 0.0                                                        GEN02420
      DO 2279 I=1,10                                                    GEN02430
 2279 USE(I) = .FALSE.                                                  GEN02440
      YBMIN = -0.4                                                      GEN02450
      NLT = 0                                                           GEN02460
      EDS = 0.0                                                         GEN02470
      XH1 = 0.0                                                         GEN02480
      DXH = 1.0                                                         GEN02490
      NWTL = 0                                                          GEN02500
      NP = 0                                                            GEN02510
      NED = 0                                                           GEN02520
      AXLE = 0.0                                                        GEN02530
      AXHT = HEIT-AMRT-HTHX                                             GEN02540
      NPLOTS = NPLOTS+1                                                 GEN02550
      GO TO 20                                                          GEN02560
C                                                                       GEN02570
C***********************************************************************GEN02580
C     DRAW SEGMENT OF X-AXIS                                CODE=231 (X)GEN02590
C                                                                       GEN02600
 231  CALL PLOT(XPEN,0.0,-3)                                            GEN02610
      IF(NP.EQ.0) GO TO 2314                                            GEN02620
      DO 2313 I=1,NP                                                    GEN02630
 2313 XPI(I) = D1-(XH2-XPI(I))/DXH*D3                                   GEN02640
 2314 XPEN = 0.0                                                        GEN02650
      XH1 = D1                                                          GEN02660
      XH2 = D2                                                          GEN02670
      DXH = D3                                                          GEN02680
      CALL PLOT(0.0,0.0,3)                                              GEN02690
      FPN = DXH*(IFIX(XH1/DXH+1))                                       GEN02700
      IF(FPN.LT.XH2) GO TO 2315                                         GEN02710
      FPN = XH1                                                         GEN02720
      GO TO 2318                                                        GEN02730
 2315 XPEN = XPEN+(FPN-XH1)/DXH                                         GEN02740
 2316 CALL PLOT(XPEN,0.05,3)                                            GEN02750
      CALL PLOT(XPEN,-.05,2)                                            GEN02760
      XFPN = XPEN-0.5*WIDF(FPN,0.1,-1)                                  GEN02770
      CALL NUMBER(XFPN,-0.20,0.10,FPN,0.0,-1)                           GEN02780
      XPEN = XPEN+1.0                                                   GEN02790
      FPN = FPN+DXH                                                     GEN02800
      IF(FPN.LT.XH2) GO TO 2316                                         GEN02810
 2318 XPEN = XPEN+(XH2-FPN)/DXH                                         GEN02820
      CALL PLOT(0.025,0.0,3)                                            GEN02830
      CALL PLOT(XPEN-0.025,0.0,2)                                       GEN02840
      XBCD = XPEN/2.0                                                   GEN02850
      YBCD = -0.33                                                      GEN02860
      CALL GRAFIC(XBCD,YBCD,0.105,TXT,42)                               .B.02870
      CALL WHERE(XNOW,YNOW,RFACT)                                       .B.02880
      IF(YNOW.LT.YBMIN) YBMIN=YNOW                                      GEN02890
      AXLE = AXLE+XPEN                                                  GEN02900
      GO TO 20                                                          GEN02910
C                                                                       GEN02920
C********************************************************************** GEN02930
C     DRAW Y-AXIS                                           CODE=232 (Y)GEN02940
C                                                                       GEN02950
C232  YV1 = D1                                                          GEN02960
C     DYV = D2                                                          GEN02970
C     XPEN = XPEN+0.4                                                   GEN02980
C     DO 2321 I=1,42                                                    GEN02990
C     NB = 43-I                                                         GEN03000
C     BYTE(4) = BCD(NB)                                                 GEN03010
C     IF(KODE.NE.64) GO TO 2322                                         GEN03020
C2321 CONTINUE                                                          GEN03030
C2322 CALL AXIS(XPEN,0.0,BCD,NB,D3,90.0,YV1,DYV,10.0)                   GEN03040
C     YMIN = YV1                                                        GEN03050
C     YMAX = AXHT*DYV+YV1                                               GEN03060
C     XPEN = XPEN+0.30                                                  GEN03070
C     NYAX = NYAX + 1                                                   GEN03080
C     GO TO 20                                                          GEN03090
C     END                                                               GEN03100
C********************************************************************** GEN03110
C     DRAW Y-AXIS                                           CODE=232 (Y)GEN03120
C                                                                       GEN03130
 232  YV1 = D1                                                          GEN03140
      DYV = D2                                                          GEN03150
      XPEN = XPEN+0.4                                                   GEN03160
      YFPN=0.0                                                          GEN03170
      FPN=YV1                                                           GEN03180
      NI=(D3+1.0)                                                       GEN03190
      DO 2321 I=1,NI                                                    GEN03200
      CALL NUMBER(.60,YFPN,.10,FPN,90.,-1)                              GEN03210
      YFPN=YFPN+1.0                                                     GEN03220
      FPN=FPN+DYV                                                       GEN03230
 2321 CONTINUE                                                          GEN03240
      YMIN = YV1                                                        GEN03250
      YMAX = AXHT*DYV+YV1                                               GEN03260
      XPEN = XPEN+0.30                                                  GEN03270
      NYAX = NYAX + 1                                                   GEN03280
      GO TO 20                                                          GEN03290
      END                                                               GEN03300
      SUBROUTINE CLINE(XPI,YPI,NP,IDG,NHG,XR1,XR2,XED,ZED,EZ,           GEN03310
     *                 YMIN,YMAX,EMIN,EMAX,ED1,EDS)                     GEN03320
      EXTERNAL YVALUE,EFD                                               GEN03330
      DIMENSION XPI(1),YPI(1),A(9,9),B(9)                               GEN03340
      LOGICAL ZED                                                       GEN03350
C                                                                       GEN03360
C***********************************************************************GEN03370
C                                                                       GEN03380
C     IDG = 0 POINTS ONLY, WITH NO CONNECTING LINES                     GEN03390
C     IDG = 1 POINTS CONNECTED BY STRAIGHT LINES                        GEN03400
C     IDG = 2 POINTS CONNECTED BY DASHED LINES                          GEN03410
C     IDG = 3 POWER SERIES OF DEGREE NHG                                GEN03420
C     IDG = 4 POWER SERIES OF DEGREE NHG + EFFECTIVE DENSITY            GEN03430
C     IDG = 5 POINTS CONNECTED BY SMOOTH CURVE                          GEN03440
C                                                                       GEN03450
C***********************************************************************GEN03460
C                                                                       GEN03470
      IF(NP.EQ.0) RETURN                                                GEN03480
      XZERO = XPI(NP+1)                                                 GEN03490
      YZERO = YPI(NP+1)                                                 GEN03500
      XSCALE = XPI(NP+2)                                                GEN03510
      YSCALE = YPI(NP+2)                                                GEN03520
      IDG = N1                                                          .B.
      IF(IDG.GT.0) GO TO 10                                             GEN03530
      RETURN                                                            GEN03540
   10 IF(IDG.GT.1) GO TO 20                                             GEN03550
C     CALL LINE(XPI,YPI,NP,1,0,1)                                       GEN03560
      CALL LINE(XPI,YPI,NP,1,1,1)                                       .B.
      RETURN                                                            GEN03570
   20 IF(IDG.GT.2) GO TO 30                                             GEN03580
      CALL LINT(XPI,YPI,NP,1,0.04,0.03)                                 GEN03590
      RETURN                                                            GEN03600
   30 IF(IDG.GT.4) GO TO 40                                             GEN03610
      CALL CURFIT(XPI,YPI,NP,A,B,NHG,KS)                                GEN03620
      IF(KS.EQ.1) RETURN                                                GEN03630
      CALL FPLOT(YVALUE,XR1,XR2,XZERO,XSCALE,YMIN,YMAX,YZERO,YSCALE)    GEN03640
      IF(IDG.EQ.3) RETURN                                               GEN03650
      IF(EDS.EQ.0.0) RETURN                                             GEN03660
      CALL SECANT(EZ,XED,ZED,B,NHG)                                     GEN03670
      CALL FPLOT(EFD,XR1,XR2,XZERO,XSCALE,EMIN,EMAX,ED1,EDS)            GEN03680
      XPEN = XR1+0.06*XSCALE                                            GEN03690
      DO 41 I=1,2                                                       GEN03700
      CALL EFD(XPEN,FPN)                                                .A.03710
      YPEN = (FPN-ED1)/EDS                                              GEN03720
      XPEN = (XPEN-XZERO)/XSCALE                                        GEN03730
C     CALL SYMBOL(XPEN,YPEN,0.08,10,0.0,-1)                             .B.03740
      CALL SYMBOL(XPEN,YPEN,0.08,10,IDUM,0.0,-1)                        .B.
      CALL NUMBER(XPEN+0.035,YPEN+0.06,0.07,FPN,90.,-1)                 GEN03750
   41 XPEN = XR2-0.06*XSCALE                                            GEN03760
      RETURN                                                            GEN03770
   40 IF(IDG.GT.5) GO TO 50                                             GEN03780
      XN = (XPI(1) - XZERO)/XSCALE                                      GEN03790
      YN = (YPI(1) - YZERO)/YSCALE                                      GEN03800
      CALL PLOT(XN,YN,3)                                                GEN03810
      CALL FLINE(XPI,YPI,-NP,1,0,0)                                     GEN03820
      RETURN                                                            GEN03830
   50 RETURN                                                            GEN03840
      END                                                               GEN03850
      SUBROUTINE GRAFIC(X,Y,HT,TXT,NBCD)                                .B.03860
      LOGICAL*1 BCD(1),ALPHA,TXT(1)                                     .B.03870
      EQUIVALENCE (ALPHA,BETA)                                          GEN03880
      DATA SLASH/'/'/,BETA/' '/                                         GEN03890
      NB1 = 1                                                           GEN03900
      YPEN = Y                                                          GEN03910
      DO 10 I=1,NBCD                                                    GEN03920
      ALPHA = TXT(I)                                                    .B.03930
      IF(BETA.NE.SLASH) GO TO 10                                        GEN03940
      IF(I.EQ.NB1) GO TO 5                                              GEN03950
      CALL CENTER(X,YPEN,HT,TXT(NB1),0.0,I-NB1)                         .B.03960
    5 YPEN = YPEN-1.40*HT                                               GEN03970
      NB1 = I+1                                                         GEN03980
   10 CONTINUE                                                          GEN03990
      CALL CENTER(X,YPEN,HT,TXT(NB1),0.0,NBCD-NB1+1)                    .B.04000
      RETURN                                                            GEN04010
      END                                                               GEN04020
      SUBROUTINE FPLOT(FUNC,X1,X2,XZERO,XSCALE,Y1,Y2,YZERO,YSCALE)      GEN04030
C                                                                       GEN04040
C     THIS ROUTINE PLOTS A FUNCTION (FUNC) OF X BETWEEN SPECIFIED       GEN04050
C     LIMITS.  IF THE VALUE OF THE FUNCTION IS OUTSIDE THE SPECIFIED Y  GEN04060
C     LIMITS, THE LINE IS NOT DRAWN FOR THAT PORTION.                   GEN04070
C                                                                       GEN04080
      LOGICAL OUT                                                       GEN04090
      OUT = .TRUE.                                                      GEN04100
      DXX = 0.01*XSCALE                                                 GEN04110
      XX = X1                                                           GEN04120
      XPEN = (X1-XZERO)/XSCALE                                          GEN04130
      GO TO 25                                                          GEN04140
   20 XPEN = XPEN+0.01                                                  GEN04150
      XX = XX+DXX                                                       GEN04160
      IF(XX.GT.X2) GO TO 40                                             GEN04170
   25 YY = FUNC(XX)                                                     GEN04180
      IF(YY.LT.Y1.OR.YY.GT.Y2) GO TO 30                                 GEN04190
      YPEN = (YY-YZERO)/YSCALE                                          GEN04200
      IF(OUT) CALL PLOT(XPEN,YPEN,3)                                    GEN04210
      OUT = .FALSE.                                                     GEN04220
      CALL PLOT(XPEN,YPEN,2)                                            GEN04230
      GO TO 20                                                          GEN04240
   30 OUT = .TRUE.                                                      GEN04250
      GO TO 20                                                          GEN04260
   40 RETURN                                                            GEN04270
      END                                                               GEN04280
      SUBROUTINE CURFIT(X,Y,NP,A,B,NT,KS)                               GEN04290
      DIMENSION X(NP),Y(NP),A(NT,NT),B(NT)                              GEN04300
      DO 10 I=1,NT                                                      GEN04310
      B(I) = 0.0                                                        GEN04320
      DO 10 J=1,NT                                                      GEN04330
   10 A(I,J) = 0.0                                                      GEN04340
      A(1,1) = NP                                                       GEN04350
      DO 30 N=1,NP                                                      GEN04360
      B(1) = B(1)+Y(N)                                                  GEN04370
      FX = X(N)                                                         GEN04380
      DO 20 I=2,NT                                                      GEN04390
      B(I) = B(I)+FX*Y(N)                                               GEN04400
      A(1,I) = A(1,I)+FX                                                GEN04410
   20 FX = FX*X(N)                                                      GEN04420
      DO 30 I=2,NT                                                      GEN04430
      A(I,NT) = A(I,NT)+FX                                              GEN04440
   30 FX = FX*X(N)                                                      GEN04450
      DO 40 I=2,NT                                                      GEN04460
      K = I-1                                                           GEN04470
      DO 40 J=2,NT                                                      GEN04480
      L = J-1                                                           GEN04490
   40 A(I,L) = A(K,J)                                                   GEN04500
      CALL SIMQ(A,B,NT,KS)                                              GEN04510
      RETURN                                                            GEN04520
      ENTRY YVALUE(XX, YVAL)                                            .A.04530
      NN = NT                                                           GEN04540
      YVAL = B(NT)                                                      .A.04550
   50 NN = NN-1                                                         GEN04560
      IF(NN.EQ.0) RETURN                                                GEN04570
      YVAL = YVAL*XX+B(NN)                                              .A.04580
      GO TO 50                                                          GEN04590
      END                                                               GEN04600
      SUBROUTINE POINT(X,Y,AH)                                          GEN04610
C                                                                       GEN04620
C     THIS ROUTINE DRAWS A LINE FROM HERE TO THERE, WITH A POINTER      GEN04630
C     OF LENGTH AH AT THE END. ON RETURN, THE PEN IS AT X,Y AND IS UP.  GEN04640
C                                                                       GEN04650
      CALL WHERE(XNOW,YNOW,RFACT)                                       .B.04660
      PHI = ATAN2(Y-YNOW,X-XNOW)                                        GEN04670
      XP = X-0.8*AH*COS(PHI)                                            GEN04680
      YP = Y-0.8*AH*SIN(PHI)                                            GEN04690
      CALL PLOT(XP,YP,2)                                                GEN04700
      XX = X-AH*COS(PHI+0.1745)                                         GEN04710
      YY = Y-AH*SIN(PHI+0.1745)                                         GEN04720
      CALL PLOT(XX,YY,2)                                                GEN04730
      CALL PLOT(X,Y,2)                                                  GEN04740
      XX = X-AH*COS(PHI-0.1745)                                         GEN04750
      YY = Y-AH*SIN(PHI-0.1745)                                         GEN04760
      CALL PLOT(XX,YY,2)                                                GEN04770
      CALL PLOT(XP,YP,2)                                                GEN04780
      CALL PLOT(X,Y,3)                                                  GEN04790
      RETURN                                                            GEN04800
      END                                                               GEN04810
      SUBROUTINE SECANT(EZERO,XED,ZEFD,B,NT)                            GEN04820
      DIMENSION B(1),D(8)                                               GEN04830
      LOGICAL ZEFD                                                      GEN04840
      IF(.NOT.ZEFD) GO TO 20                                            GEN04850
      KED = 1                                                           GEN04860
      PZERO = YVALUE(EZERO)                                             GEN04870
      ND = NT-1                                                         GEN04880
      NN = NT                                                           GEN04890
      D(ND) = B(NT)                                                     GEN04900
   10 NN = NN-1                                                         GEN04910
      IF(NN.EQ.1) RETURN                                                GEN04920
      D(NN-1) = D(NN)*EZERO+B(NN)                                       GEN04930
      GO TO 10                                                          GEN04940
   20 IF(XED.EQ.0.0) KED = 2                                            GEN04950
      IF(XED.NE.0.0) KED = 3                                            GEN04960
      RETURN                                                            GEN04970
      ENTRY EFD(XX,EFDVAL)                                              .A.04980
      IF(KED.EQ.2) GO TO 40                                             GEN04990
      IF(KED.EQ.3) GO TO 50                                             GEN05000
      NN = ND                                                           GEN05010
      ED = D(NN)                                                        GEN05020
   30 NN = NN-1                                                         GEN05030
      IF(NN.EQ.0) GO TO 35                                              GEN05040
      ED = ED*XX+D(NN)                                                  GEN05050
      GO TO 30                                                          GEN05060
   35 EFDVAL = 144.0*ED                                                 .A.05070
      RETURN                                                            GEN05080
   40 CALL YVALUE (XX, YVAL)                                            .A.05090
      EFDVAL = 144.0*(YVAL-PZERO)/(XX-EZERO)                            .A.05090
      RETURN                                                            GEN05100
   50 CALL YVALUE (XX, YVAL)                                            .A.05110
      EFDVAL = 144.0*(YVAL-PZERO)/(XED-EZERO)                           .A.05110
      RETURN                                                            GEN05120
      END                                                               GEN05130
      SUBROUTINE CENTER(X,Y,HT,TXT,ANGLE,NBCD)                          GEN05140
      LOGICAL*1 BCD(1),ALPHA,TXT(1)                                     .B.05150
      EQUIVALENCE (ALPHA,BETA)                                          GEN05160
      DATA BLANK/' '/,BETA/' '/                                         GEN05170
      NB1 = 1                                                           GEN05180
      NB2 = NBCD                                                        GEN05190
      DO 10 I=1,NBCD                                                    GEN05200
      ALPHA = TXT(I)                                                    GEN05210
      IF(BETA.NE.BLANK) GO TO 20                                        GEN05220
   10 NB1 = NB1+1                                                       GEN05230
      RETURN                                                            GEN05240
   20 DO 30 I=1,NBCD                                                    GEN05250
      ALPHA = TXT(NBCD-I+1)                                             GEN05260
      IF(BETA.NE.BLANK) GO TO 40                                        GEN05270
   30 NB2 = NB2-1                                                       GEN05280
   40 NBYTES = NB2-NB1+1                                                GEN05290
      WIDTH = (.87514*NBYTES-.28571)*HT                                 GEN05300
      PHI = ANGLE*0.01745329                                            GEN05310
      C = COS(PHI)*0.5                                                  GEN05320
      S = SIN(PHI)*0.5                                                  GEN05330
      XPEN = X-WIDTH*C+HT*S                                             GEN05340
      YPEN = Y-WIDTH*S-HT*C                                             GEN05350
      CALL SYMBOL(XPEN,YPEN,HT,TXT(NB1),IDUM,ANGLE,NBYTES)              .B.05360
      RETURN                                                            GEN05370
      END                                                               GEN05380
      SUBROUTINE PSTART(NAME,LC,INCH,NPAPER,PGMR,PHONE)                 GEN05390
C     CALL PSTART(NAME,LC,INCH,NPAPER)                                  GEN05400
C         INITIALIZES A PLOT BUFFER OF 8000 BYTES                       GEN05410
C         IF BLANK PAPER IS USED, INITIALIZES COORDINATES AT RIGHT      GEN05420
C         NAME......PROGRAM NAME (LITERAL UP TO 8 CHARACTERS)           GEN05430
C         LC........LABOR COST CODE (4 CHARACTERS)                      GEN05440
C         INCH......SIZE PLOTTER (11 OR 30)                             GEN05450
C         NPAPER....CODE FOR TYPE OF PAPER                              GEN05460
C                   1..BLANK                                            GEN05470
C                   2..OLIVE GRID                                       GEN05480
C                   3..RED GRID                                         GEN05490
C     CALL WHOM(WHO,PHONE)                                              GEN05500
C         THIS ENTRY POINT IS OPTIONAL                                  GEN05510
C         WHO.......PROGRAMMER NAME (12 CHARACTERS)                     GEN05520
C         PHONE.....EXTENSION NUMBER (4 CHARACTERS)                     GEN05530
C     CALL WHENCE(DOUT)                                                 GEN05540
C         DOUT......OUTPUT DISPOSITION (24 CHARACTERS)                  GEN05550
C         THIS ENTRY POINT IS OPTIONAL.  IF IT IS NOT CALLED, THE       GEN05560
C         MESSAGE 'RETURN VIA BRIDGE BASKET' WILL BE PRINTED.           GEN05570
C     CALL PEN(KPEN)                                                    GEN05580
C         KPEN......PEN NUMBER                                          GEN05590
C         THIS ENTRY POINT IS OPTIONAL.  DEFAULT IS PEN NO. 2           GEN05600
C     CALL PSTOP(DX,NPLOTS,WIDTH,SEC)                                   GEN05610
C         PLOTS MESSAGE 'END OF PLOTS' WITH PROGRAM NAME AND DATE       GEN05620
C         WRITES (ON UNIT 6) MESSAGE TO PLOTTER OPERATOR                GEN05630
C         CLOSES TAPE BY WRITING BLOCK ADDRESS 999                      GEN05640
C         DX........OFFSET TO 'END OF PLOTS' MESSAGE                    GEN05650
C         NPLOTS....NUMBER OF BLOCKS                                    GEN05660
C         WIDTH.....AVERAGE WIDTH PER BLOCK                             GEN05670
C         SEC.......AVERAGE PLOTTING TIME PER BLOCK                     GEN05680
C                                                                       GEN05690
      DIMENSION PGMR(3)                                                 .B.05700
      DIMENSION WHO(3)                                                  .A.06270
      DIMENSION DISP(6)                                                 .A.06340
C                                                                       .A.05710
      REAL*8 VOLNO                                                      .A.06220
      REAL*8 MSG(8)/'END OF P','LOTS',4*' ','REEL NO',' '/              .A.05720
      REAL*8 PAPER(6)/'BLANK',' ','OLIVE GR','ID','RED GRID',' '/ ,NAME GEN05730
      REAL*8 VOL,START(2)/'AT RIGHT','ANYWHERE'/                        GEN05740
      REAL*4 DOUT(6)/'RETU','RN V','IA B','RIDG','E BA','SKET'/         GEN05750
      REAL*8 JNAME                                                      GEN05760
      INTEGER *4 IDUM(3)                                                .A.05770
      DATA NPEN/2/                                                      GEN05770
C                                                                       GEN05780
C 699 FORMAT('11DOT STRUCTURES        TRBD ',A4,'L     ',A8,7X,         .A.05790
C    *       'TS BRIDGE   12TH & I',                                    .A.05800
C    */      '22TS BRIDGE 12TH & I',9X,'(916) 445-9756',7X,             .A.05810
C    *       'YIELD PLOT',                                              .A.05820
C    */      '33',I2,4X,6X,A8,A7,I3,' FT    ',I6,5X,I2,':',I2,7X,       .A.05830
C    *       'YIELD',                                                   .A.05840
C    */      '44',A8)                                                   .A.05850
C 699 FORMAT(1H1,19X,A6,59(1H*)/20X,A6,58X,1H*                          GEN05860
C    */    20X, A6,9X,40HINSTRUCTIONS TO CALCOMP PLOTTER OPERATOR,9X,1H*GEN05870
C    */    20X, A6,58X,1H*/20X,A6,59(1H*)/20X,A6,58X,1H*                GEN05880
C    */    20X, A6,21X,10HUSE NUMBER,I2,4H PEN,21X,1H*/20X,A6,58X,1H*   GEN05890
C    */    20X, A6,9X,10HPLOT WIDTH,15X,2H= ,I2,5H INCH,15X,1H*         GEN05900
C    */    20X, A6,9X,13HTYPE OF PAPER,12X,2H= ,2A8, 6X,1H*             GEN05910
C    */    20X, A6,9X, 9HSTART PEN,16X,2H= , A8,14X,1H*                 GEN05920
C    */    20X, A6,9X,13HPLOTTING TIME,12X,1H=, I3,8H MINUTES,12X,1H*   GEN05930
C    */    20X, A6,9X,15HNUMBER OF PLOTS,10X,2H= ,I2,20X,1H*            GEN05940
C    */    20X, A6,9X,15HLENGTH OF PAPER,10X,1H=,I3,5H FEET,15X,1H*     GEN05950
C    */    20X, A6,9X,18HLAST BLOCK ADDRESS,6X, 6H = 999,19X,1H*        GEN05960
C    */    20X, A6,58X,1H*/20X,A6,59(1H*)/20X,A6,58X,1H*                GEN05970
C    */    20X, A6, 9X,13HSUBMITTED BY ,3A4,5X,     6HEXT 5-,A4,9X,1H*  GEN05980
C    */    20X, A6,58X,1H*                                              GEN05990
C    */    20X, A6,17X, 7HPROGRAM ,11(1H.),A6,17X,1H*                   GEN06000
C    */    20X, A6,17X,19HLABOR COST CODE....,A4,1HL,17X,1H*            GEN06010
C    */    20X, A6,17X,18HREEL NUMBER.......,A6,17X,1H*                 GEN06020
C    */    20X, A6,58X,1H*/20X,A6,17X,6A4,17X,1H*                       GEN06030
C    */    20X, A6,58X,1H*/20X,A6,59(1H*))                              GEN06040
C                                                                       GEN06050
C     CALL DATIME(MSG(5),IDUM)                                          .B.06060
C     MSG(3) = NAME                                                     .B.06070
C     CALL PLOTS ARGUMENTS NO MEANING IN SUBROUTINE 01/24/80 REVISION   GEN06080
      CALL PLOTS(0,0,8)                                                 .B.06090
      KS = 1                                                            GEN06110
C     IP2 = 2*NPAPER                                                    .B.06120
C     IP1 = IP2-1                                                       .B.06130
C     IF(NPAPER.NE.1) RETURN                                            .B.06140
C     CALL PLOT(0.0,-FLOAT(INCH+1), 3) DELETED 01/24/80 1038 MODEL REV  GEN06150
C     CALL WHERE(XNOW,YNOW,RFACT)         "       "       "    "    "   .B.06160
C     CALL PLOT (XNOW,YNOW,4) & KS=2      "       "       "    "    "   GEN06170
      KS = 1                                                            GEN06180
      RETURN                                                            GEN06190
C                                                                       GEN06200
      ENTRY PTAPE(VOLNO)                                                GEN06210
      VOLNO = VOL                                                       GEN06230
      RETURN                                                            GEN06240
C                                                                       GEN06250
      ENTRY WHOM(WHO,EXT)                                               GEN06260
      DO 15 I=1,3                                                       GEN06280
   15 PGMR(I) = WHO(I)                                                  GEN06290
      PHONE = EXT                                                       GEN06300
      RETURN                                                            GEN06310
C                                                                       GEN06320
      ENTRY WHENCE(DISP)                                                GEN06330
      DO 20 I=1,6                                                       GEN06350
   20 DOUT(I)=DISP(I)                                                   GEN06360
      RETURN                                                            GEN06370
C                                                                       GEN06380
      ENTRY PEN(KPEN)                                                   GEN06390
      NPEN = KPEN                                                       GEN06400
      RETURN                                                            GEN06410
C                                                                       GEN06420
      ENTRY PSTOP(DX,NPLOTS,WIDTH,SEC)                                  GEN06430
      MSG(8) = VOL                                                      GEN06440
      CALL WHERE(XNOW,YNOW,RFACT)                                       .B.06450
      CALL PLOT(DX+XNOW,0.0,-3)                                         GEN06460
      CALL SYMBOL(0.0,10.0,0.14,MSG,IDUM,270.,62)                       .B.06470
      CALL PLOT(0.0,0.0,999)                                            GEN06480
      NF    = NPLOTS*WIDTH+2                                            GEN06490
      NT = NPLOTS*SEC/60.                                               GEN06500
      NT1= NT/60                                                        GEN06510
C     CALL GTNAM(JNAME)                                                 .A.06520
C     WRITE(6,699)                                                      .A.06530
C    * (VOL,I=1,7),NPEN,VOL,VOL,INCH,VOL,PAPER(IP1),PAPER(IP2),VOL,     GEN06540
C    * START(KS),VOL,NT,VOL,NPLOTS,VOL,NF,(VOL,I=1,5),PGMR,PHONE,       GEN06550
C    * VOL,VOL,NAME,VOL,LC,(VOL,I=1,4),DOUT,VOL,VOL                     GEN06560
C    * LC,JNAME,INCH,PAPER(IP1),PAPER(IP2),NF,NPLOTS,NT1,NT,START(KS)   .A.06570
      RETURN                                                            GEN06580
      END                                                               GEN06590
      SUBROUTINE LINT(X,Y,N,INC,DSD,DSU)                                GEN06600
      DIMENSION X(1),Y(1)                                               GEN06610
      IF(N.LE.0.OR.DSD+DSU.LE.0.) RETURN                                GEN06620
      S = 0.                                                            GEN06630
      IPEN = 3                                                          GEN06640
      I = N*INC+INC+1                                                   GEN06650
      VSCALE = Y(I)                                                     GEN06660
      HSCALE = X(I)                                                     GEN06670
      I = I-INC                                                         GEN06680
      XZERO = X(I)                                                      GEN06690
      YZERO = Y(I)                                                      GEN06700
      IEND = I-2*INC                                                    GEN06710
      DO 90 I=1,IEND,INC                                                GEN06720
      XPEN = (X(I)-XZERO)/HSCALE                                        GEN06730
      YPEN = (Y(I)-YZERO)/VSCALE                                        GEN06740
      CALL PLOT(XPEN,YPEN,IPEN)                                         GEN06750
      J = I+INC                                                         GEN06760
      DX = (X(J)-X(I))/HSCALE                                           GEN06770
      DY = (Y(J)-Y(I))/VSCALE                                           GEN06780
      DS = SQRT(DX*DX+DY*DY)                                            GEN06790
      DX = DX/DS                                                        GEN06800
      DY = DY/DS                                                        GEN06810
      IF(S.LT.DS) GO TO 5                                               GEN06820
      S = S-DS                                                          GEN06830
      GO TO 90                                                          GEN06840
    5 XPEN = XPEN+S*DX                                                  GEN06850
      YPEN = YPEN+S*DY                                                  GEN06860
      CALL PLOT(XPEN,YPEN,IPEN)                                         GEN06870
   10 IF(IPEN.EQ.2) GO TO 30                                            GEN06880
      IPEN = 2                                                          GEN06890
      S = S+DSD                                                         GEN06900
      IF(S.LT.DS) GO TO 20                                              GEN06910
      S = S-DS                                                          GEN06920
      GO TO 90                                                          GEN06930
   20 XPEN = XPEN+DSD*DX                                                GEN06940
      YPEN = YPEN+DSD*DY                                                GEN06950
      GO TO 50                                                          GEN06960
   30 IPEN = 3                                                          GEN06970
      S = S+DSU                                                         GEN06980
      IF(S.LT.DS) GO TO 40                                              GEN06990
      S = S-DS                                                          GEN07000
      GO TO 90                                                          GEN07010
   40 XPEN = XPEN+DSU*DX                                                GEN07020
      YPEN = YPEN+DSU*DY                                                GEN07030
   50 CALL PLOT(XPEN,YPEN,IPEN)                                         GEN07040
      GO TO 10                                                          GEN07050
   90 CONTINUE                                                          GEN07060
      IF(IPEN.EQ.3) RETURN                                              GEN07070
      XPEN = (X(J)-XZERO)/HSCALE                                        GEN07080
      YPEN = (Y(J)-YZERO)/VSCALE                                        GEN07090
      CALL PLOT(XPEN,YPEN,2)                                            GEN07100
      RETURN                                                            GEN07110
      END                                                               GEN07120
      FUNCTION WIDF(FPN,HT,NDEC)                                        GEN07130
C        PURPOSE - TO CALCULATE THE WIDTH REQUIRED BY THE               GEN07140
C                  CALCOMP PLOTTER TO PLOT A REAL NUMBER                GEN07150
C                                                                       GEN07160
C        PARAMETERS - FPN IS THE NAME OF THE NUMBER                     GEN07170
C                   - HT IS THE HEIGHT OF THE CHARACTERS                GEN07180
C                   - NDEC IS THE NUMBER OF PLACES TO THE               GEN07190
C                     RIGHT OF THE DECIMAL POINT                        GEN07200
C                                                                       GEN07210
      N = FPN * 1.000001                                                GEN07220
      L = 0                                                             GEN07230
      IF(FPN) 2,3,1                                                     GEN07240
    1 IF(FPN-1) 4,3,3                                                   GEN07250
    2 L = L+1                                                           GEN07260
    3 L = L+1                                                           GEN07270
      N = N/10                                                          GEN07280
      IF(N) 3,4,3                                                       GEN07290
    4 WIDF = (0.87514*(L+NDEC+1) - 0.28571)*HT                          GEN07300
      RETURN                                                            GEN07310
      END                                                               GEN07320
C     CONTOUR PROGRAM                                                   GEN07330
C FLINE      V097       09-12-67                                        GEN07340
C SUBROUTINE FLINE  COPYRIGHT 1967 CALIF COMPUTER PRODUCTS              GEN07350
C.....CALL FLINE (XARRAY, YARRAY, NPTS, INC, LINTYP, INTEQ)             GEN07360
C.....                                                                  GEN07370
C.....  XARRAY  IS THE NAME OF THE ARRAY CONTAINING THE VARIABLES TO BE GEN07380
C.....          PLOTTED AS THE ABSCISSAS.                               GEN07390
C.....  YARRAY  IS THE NAME OF THE ARRAY CONTAINING THE VARIABLES TO BE GEN07400
C.....          PLOTTED AS THE ORDINATES.                               GEN07410
C.....  NPTS    IS THE NUMBER OF DATA POINTS TO BE PLOTTED...           GEN07420
C.....          IF NPTS IS LESS THAN 0, A SMOOTH CURVE IS DRAWN THROUGH GEN07430
C.....                                  THE POINTS.                     GEN07440
C.....          IF NPTS IS GREATER THAN 0, A STRAIGHT LINE IS DRAWN     GEN07450
C.....                                  CONNECTING DATA POINTS.         GEN07460
C.....  INC     IS THE INCREMENT BETWEEN ELEMENTS IN THE ARRAY.         GEN07470
C.....          INC IS GREATER THAN 1 IF THE VALUES TO BE PLOTTED ARE   GEN07480
C.....          IN A MIXED ARRAY.                                       GEN07490
C.....  LINTYP  CONTROLS THE TYPE OF PLOT PRODUCED...                   GEN07500
C.....          IF LINTYP=0   A LINE IS PLOTTED CONNECTING SUCCESSIVE   GEN07510
C.....                        DATA POINTS.                              GEN07520
C.....          IF LINTYP=1   A LINE PLOT WITH A SYMBOL AT EACH DATA    GEN07530
C.....                        POINT IS PRODUCED.                        GEN07540
C.....          IF LINTYP=2   A LINE PLOT WITH A SYMBOL AT EVERY 2ND    GEN07550
C.....                        DATA POINT IS PRODUCED.                   GEN07560
C.....          IF LINTYP=N   A LINE PLOT WITH A SYMBOL AT EVERY NTH    GEN07570
C.....                        DATA POINT IS PRODUCED.                   GEN07580
C.....          IF LINTYP=-N  A SYMBOL APPEARS AT EVERY NTH DATA POINT, GEN07590
C.....                        NO CONNECTING LINES ARE PLOTTED.          GEN07600
C.....  INTEQ   IS THE INTEGER EQUIVALENT OF THE SYMBOL TO BE PLOTTED ATGEN07610
C.....          A DATA POINT.                                           GEN07620
C.....                                                                  GEN07630
C                                                                       GEN07640
      SUBROUTINE FLINE (X,Y,NN,K,J,L)                                   GEN07650
      DIMENSION X(2),Y(2)                                               GEN07660
      N=IABS(NN)                                                        GEN07670
      KK=K                                                              GEN07680
      LMIN=N*KK+1                                                       GEN07690
      LDX =LMIN+KK                                                      GEN07700
      NL  =LMIN-KK                                                      GEN07710
      XMIN=X(LMIN)                                                      GEN07720
      YMIN=Y(LMIN)                                                      GEN07730
      DX=X(LDX)                                                         GEN07740
      DY=Y(LDX)                                                         GEN07750
      CALL WHERE(XN,YN,DF)                                              GEN07760
      DF= AMAX1 ( ABS ((X( 1)-XMIN)/DX-XN), ABS ((Y( 1)-YMIN)/DY-YN))   GEN07770
      DL= AMAX1 ( ABS ((X(NL)-XMIN)/DX-XN), ABS ((Y(NL)-YMIN)/DY-YN))   GEN07780
      IC=3                                                              GEN07790
      IS=-1                                                             GEN07800
      NT= IABS (J)                                                      GEN07810
      IF(J.EQ.0)NT=1                                                    GEN07820
      IF(DF.GT.DL)GO TO 4                                               GEN07830
      NF=1                                                              GEN07840
      NA=NT                                                             GEN07850
      GO TO 5                                                           GEN07860
   4  KK=-KK                                                            GEN07870
      NF=NL                                                             GEN07880
      NA=((N-1)/NT)*NT+NT-N+1                                           GEN07890
   5  IF  (J) 6,7,8                                                     GEN07900
   6  ICA=3                                                             GEN07910
      ISA=-1                                                            GEN07920
      LSW=1                                                             GEN07930
      GO TO 9                                                           GEN07940
   7  NA=LDX                                                            GEN07950
   8  ICA=2                                                             GEN07960
      ISA=-2                                                            GEN07970
      LSW=0                                                             GEN07980
    9 XN1=(X(NF)-XMIN)/DX                                               GEN07990
      YN1=(Y(NF)-YMIN)/DY                                               GEN08000
      NF=NF+KK                                                          GEN08010
      IF(NN.GT.0)GO TO 25                                               GEN08020
      X0=XN1                                                            GEN08030
      Y0=YN1                                                            GEN08040
      LP=KK*(N-1)+1                                                     GEN08050
      NLP=LP-KK                                                         GEN08060
      U=(X(NF)-X(1))/DX                                                 GEN08070
      V=(Y(NF)-Y(1))/DY                                                 GEN08080
      U1=(X(LP)-X(NLP))/DX                                              GEN08090
      V1=(Y(LP)-Y(NLP))/DY                                              GEN08100
      SU=U                                                              GEN08110
      SV=V                                                              GEN08120
      IF(X(1).EQ.X(LP).AND.Y(1).EQ.Y(LP))GO TO 25                       GEN08130
      NFP=NLP-KK                                                        GEN08140
      SU=(X(NLP)-X(NFP))/DX                                             GEN08150
      SV=(Y(NLP)-Y(NFP))/DY                                             GEN08160
      CALL REFLEX (U1,V1,SU,SV)                                         GEN08170
      NFP=NF+KK                                                         GEN08180
      U1=(X(NFP)-X(NF))/DX                                              GEN08190
      V1=(Y(NFP)-Y(NF))/DY                                              GEN08200
      CALL REFLEX (U,V,U1,V1)                                           GEN08210
   25 DO 23 I=1,N                                                       GEN08220
        XN=XN1                                                          GEN08230
        YN=YN1                                                          GEN08240
      IF(N.LE.I)GO TO 11                                                GEN08250
          XN1=(X(NF)-XMIN)/DX                                           GEN08260
          YN1=(Y(NF)-YMIN)/DY                                           GEN08270
   11   NW=NA-NT                                                        GEN08280
      IF(NW.LT.0.AND.LSW.NE.0)GO TO 17                                  GEN08290
            IF (NN) 16,24,15                                            GEN08300
   15         CALL PLOT (XN,YN,IC)                                      GEN08310
              GO TO 20                                                  GEN08320
   16 IF(IC.NE.2)GO TO 15                                               GEN08330
   17 IF(N.GT.I)GO TO 18                                                GEN08340
      U2=SU                                                             GEN08350
      V2=SV                                                             GEN08360
      GO TO 19                                                          GEN08370
   18 U2=XN1-XN                                                         GEN08380
      V2=YN1-YN                                                         GEN08390
   19 CALL FIT4(X0,Y0,XN,YN,U1,V1,U2,V2)                                GEN08400
      U1=U                                                              GEN08410
      V1=V                                                              GEN08420
      U=U2                                                              GEN08430
      V=V2                                                              GEN08440
              X0=XN                                                     GEN08450
              Y0=YN                                                     GEN08460
   20       NA=NA+1                                                     GEN08470
      IF(NW.NE.0)GO TO 22                                               GEN08480
C             CALL SYMBOL (XN,YN,0.08,L,0.0,-1)                         GEN08480
              CALL SYMBOL (XN,YN,0.08,L,IDUM,0.0,-1)                    .B.
              NA=1                                                      GEN08500
   22   IC=ICA                                                          GEN08510
   23   NF=NF+KK                                                        GEN08520
   24 RETURN                                                            GEN08530
      END                                                               GEN08540
      SUBROUTINE REFLEX (VX1,VY1,VX2,VY2)                               GEN08550
C REFLEX  COPYRIGHT 1967 CALIFORNIA COMPUTER PRODUCTS                   GEN08560
CALL REFLEX (VX2,VY2,VX3,VY3)     GORDON KNOPES     3/28/67             GEN08570
C                                                                       GEN08580
C                                                                       GEN08590
      PS=VY1*VY1                                                        GEN08600
      DS=VX1*VX1                                                        GEN08610
      SS=DS+PS                                                          GEN08620
      DS=DS-PS                                                          GEN08630
      PS=2.0*VX1*VY1                                                    GEN08640
      TEMP=(PS*VY2+VX2*DS)/SS                                           GEN08650
      VY2=(PS*VX2-VY2*DS)/SS                                            GEN08660
      VX2=TEMP                                                          GEN08670
      RETURN                                                            GEN08680
      END                                                               GEN08690
      SUBROUTINE FIT4(PX1,PY1,PX2,PY2,VECX1,VECY1,VECX3,VECY3)          GEN08700
C FIT4    COPYRIGHT 1967 CALIFORNIA COMPUTER PRODUCTS                   GEN08710
CALL FIT4(X1,Y1,X2,Y2,VX1,VY1,VX3,VY3)     GORDON KNOPES     3/28/67    GEN08720
C                                                                       GEN08730
C                                                                       GEN08740
      X1=PX1                                                            GEN08750
      Y1=PY1                                                            GEN08760
      CALL WHERE (X,Y,D)                                                GEN08770
      IF(X1-X)2,1,2                                                     GEN08780
    1 IF(Y1-Y)2,3,2                                                     GEN08790
    3 IF(VECX1-VX2)5,4,5                                                GEN08800
    4 IF(VECY1-VY2)5,6,5                                                GEN08810
    2 CALL PLOT (PX1,PY1,3)                                             GEN08820
    5 VX3=PX2-X1                                                        GEN08830
      VY3=PY2-Y1                                                        GEN08840
      VX2=VECX1                                                         GEN08850
      VY2=VECY1                                                         GEN08860
      D2=VX2*VX2+VY2*VY2                                                GEN08870
      T=1.                                                              GEN08880
      GO TO 7                                                           GEN08890
    6 T=0.                                                              GEN08900
      VX2=VX3                                                           GEN08910
      VY2=VY3                                                           GEN08920
      VX3=VECX3                                                         GEN08930
      VY3=VECY3                                                         GEN08940
      D2=D3                                                             GEN08950
      UX1=UX2                                                           GEN08960
      UY1=UY2                                                           GEN08970
    7 D3=VX3*VX3+VY3*VY3                                                GEN08980
      UX2=D2*VX3+D3*VX2                                                 GEN08990
      UY2=D2*VY3+D3*VY2                                                 GEN09000
      DV=1.0/SQRT (UX2*UX2+UY2*UY2)                                     GEN09010
      UX2=DV*UX2                                                        GEN09020
      UY2=DV*UY2                                                        GEN09030
      IF(T)6,8,6                                                        GEN09040
    8 D=ABS(UX1*VX2+UY1*VY2)                                            GEN09050
      D1=D                                                              GEN09060
      UUX1=D*UX1                                                        GEN09070
      UUY1=D*UY1                                                        GEN09080
      D=ABS(UX2*VX2+UY2*VY2)                                            GEN09090
      UUX2=D*UX2                                                        GEN09100
      UUY2=D*UY2                                                        GEN09110
      D=D+D1                                                            GEN09120
      AX=UUX2+UUX1-VX2-VX2                                              GEN09130
      BX=VX2-UUX1-AX                                                    GEN09140
      AY=UUY2+UUY1-VY2-VY2                                              GEN09150
      BY=VY2-UUY1-AY                                                    GEN09160
      N=10.*D+1.                                                        GEN09170
      D=1./FLOAT (N)                                                    GEN09180
      DO 9 I=1,N                                                        GEN09190
      T=T+D                                                             GEN09200
      X=((AX*T+BX)*T+UUX1)*T+X1                                         GEN09210
      Y=((AY*T+BY)*T+UUY1)*T+Y1                                         GEN09220
    9 CALL PLOT (X,Y,2)                                                 GEN09230
      RETURN                                                            GEN09240
      END                                                               GEN09250
      SUBROUTINE SIMQ(A,B,N,KS)
C
C     ADAPTED FOR WASHINGTON STATE D.O.T. FROM IBM SYSTEM/360
C     SCIENTIFIC SUBROUTINE PACKAGE BY GEORGE SPENCER, NOVEMBER, 1981.
C
C     PURPOSE
C        OBTAIN SOLUTION OF A SET OF SIMULTANEOUS LINEAR EQUATIONS,
C        AX = B
C
C     USAGE
C        CALL SIMQ(A,B,N,KS)
C
C     DESCRIPTION OF PARAMETERS
C        A - MATRIX OF COEFFICIENTS STORED COLUMNWISE.  THESE ARE
C            DESTROYED IN THE COMPUTATION.  THE SIZE OF MATRIX A IS
C            N BY N.
C        B - VECTOR OF ORIGINAL CONSTANTS (LENGTH N).  THESE ARE
C            REPLACED BY FINAL SOLUTION VALUES, VECTOR X.
C        N - NUMBER OF EQUATIONS AND VARIABLES.  N MUST BE .GT. ONE.
C        KS - OUTPUT DIGIT
C             0 FOR A NORMAL SOLUTION
C             1 FOR A SINGULAR SET OF EQUATIONS
C
C     REMARKS
C        MATRIX A MUST BE GENERAL.
C        IF MATRIX IS SINGULAR, SOLUTION VALUES ARE MEANINGLESS.
C        AN ALTERNATIVE SOLUTION MAY BE OLTAINED BY USING MATRIX
C        INVERSION (MINV) AND MATRIX PRODUCT (GMPRD).
C
C     SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
C        NONE
C
C     METHOD
C        METHOD OF SOLUTION IS BY ELIMINATION USING LARGEST PIVOTAL
C        DIVISOR.  EACH STAGE OF ELIMINATION CONSISTS OF INTERCHANGING
C        ROWS WHEN NECESSARY TO AVOID DIVISION BY ZERO OR SMALL
C        ELEMENTS.
C        THE FORWARD SOLUTION TO OBTAIN VARIABLE N IS DONE IN
C        N STAGES.  THE BACK SOLUTION FOR THE OTHER VARIABLES IS
C        CALCULATED BY SUCCESSIVE SUBSTITUTIONS.  FINAL SOLUTION
C        VALUES ARE DEVELOPED IN VECTOR B, WITH VARIABLE 1 IN B(1),
C        VARIABLE 2 IN B(2),..., VARIABLE N IN B(N).
C        IF NO PIVOT CAN BE FOUND EXCEEDING A TOLERANCE OF 0.0,
C        THE MATRIX IS CONSIDERED SINGULAR AND KS IS SET TO 1.  THIS
C        TOLERANCE CAN BE MODIFIED BY REPLACING THE FIRST STATEMENT.
C
      REAL*4  A(1),B(1)
C
C     FORWARD SOLUTION
C
      TOL = 0.0
      KS = 0
      JJ = -N
      DO 65 J = 1,N
      JY = J + 1
      JJ = JJ + N + 1
      BIGA = 0.0
      IT = JJ - J
      DO 30 I=J,N
C
C     SEARCH FOR MAXIMUM COEFFICIENT IN COLUMN
C
      IJ = IT + I
      IF(ABS(BIGA) - ABS(A(IJ))) 20,30,30
   20 BIGA = A(IJ)
      IMAX = I
   30 CONTINUE
C
C     TEST FOR PIVOT LESS THAN TOLERANCE (SINGULAR MATRIX)
C
      IF(ABS(BIGA) - TOL) 35,35,40
   35 KS = 1
      WRITE(6,37)
   37 FORMAT('-','***  KS=1, MATRIX IS SINGULAR, SOLUTION VALUES ',
     1   'MEANINGLESS')
      RETURN
C
C     INTERCHANGE ROWS IF NECESSARY
C
   40 I1 = J + N*(J-2)
      IT = IMAX - J
      DO 50 K=J,N
      I1 = I1 + N
      I2 = I1 + IT
      SAVE = A(I1)
      A(I1) = A(I2)
      A(I2) = SAVE
C
C     DIVIDE EQUATION BY LEADING COEFFICIENT
C
   50 A(I1) = A(I1)/BIGA
      SAVE = B(IMAX)
      B(IMAX) = B(J)
      B(J) = SAVE/BIGA
C
C     ELIMINATE NEXT VARIABLE
C
      IF(J-N) 55,70,55
   55 IQS = N*(J - 1)
      DO 65 IX = JY,N
      IXJ = IQS + IX
      IT = J - IX
      DO 60 JX = JY,N
      IXJX = N*(JX - 1) + IX
      JJX = IXJX + IT
   60 A(IXJX) = A(IXJX) - (A(IXJ) * A(JJX))
   65 B(IX) = B(IX) - (B(J)*A(IXJ))
C
C     BACK SOLUTION
C
   70 NY = N - 1
      IT = N*N
      DO 80 J = 1,NY
      IA = IT - J
      IB = N - J
      IC = N
      DO 80 K = 1,J
      B(IB) = B(IB) - A(IA)*B(IC)
      IA = IA -N
   80 IC = IC - 1
      RETURN
      END

C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   
C
C
      SUBROUTINE PARM
      CHARACTER*8 TIME
      CHARACTER*16 FLNM1, FLNM2
      INTEGER*4 INUNIT, ERUNIT, OUNIT
C     INUNIT = (5 FOR FILE INPUT)
C     ERUNIT = (* FOR SCREEN OUTPUT)
C     OUNIT  = (8 FOR FILE OUTPUT)
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      OUNIT  = 8
      INUNIT = 5
C
10    WRITE(*,FMT='(//,'' Enter name of plot file created in YIELD
     * Program'',/)')
      READ(*,ERR=10,FMT='(A)')FLNM1
      DO 20 I=1,16
      IF(FLNM1(I:I).EQ.'.')GO TO 50
20    CONTINUE
      DO 40 I=1,16
      IF(FLNM1(I:I).EQ.' ')THEN
      IE = I + 3
      FLNM1(I:IE) = '.PLT'
      GO TO 50
      END IF
40    CONTINUE
50    WRITE(*,FMT='(/,1X,A16,'' used for plot input '',//)')FLNM1
      OPEN (INUNIT,FILE=FLNM1,STATUS='OLD')
70    WRITE(*,FMT='(//,'' Enter name of output file'',/)')
      READ(*,ERR=70,FMT='(A)')FLNM2
      DO 80 I=1,16
      IF(FLNM2(I:I).EQ.'.')GO TO 100
80    CONTINUE
      DO 90 I=1,16
      IF(FLNM2(I:I).EQ.' ')THEN
      IE = I + 3
      FLNM2(I:IE) = '.PLO'
      GO TO 100
      END IF
90    CONTINUE
100   WRITE(*,FMT='(/,1X,A16,'' used for output '',//)')FLNM2
      OPEN (OUNIT,FILE=FLNM2,STATUS='NEW')
      RETURN
      END   
