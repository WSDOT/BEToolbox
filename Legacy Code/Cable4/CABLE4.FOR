C                                                                          00010
C     CABLE4 VMS                                                           00020
C                                                                          00030
C  PROGRAM CABLE3 COMPUTES HORIZONTAL AND VERTICAL COMPONENTS OF THRUST    00040
C    ON A SUBMERGED ANCHOR CABLE AS WELL AS HORIZONTAL DISPLACEMENT WITH   00050
C    THE PONTOON FORCE.                                                    00060
C                                                                          00070
C    ADAPTED FROM CABLE2 - DICK SIMKINS - 2/11/80                          00080
C                                                                          00090
C    INCLUDE POINT WHERE SAG HITS BOTTOM - DAVE BOWERS - 10/28/82          00100
C                                                                          00110
C    CONVERTED TO VMS - TERRY BEEBE - FEBRUARY, 1990
C
C    CONVERTED FOR USE ON LACEY V. MURROW - MARK W. ANDERSON, 1/11/91
C
C    INPUT DATA:  INITIAL HORIZONTAL PROJECTION OF CABLE (FEET)            00120
C                 INITIAL VERTICAL PROJECTION OF CABLE   (FEET)            00130
C                 INITIAL HORIZONTAL THRUST ON CABLE     (KIPS)            00140
C                 METALLIC CROSS-SECTION AREA OF CABLE   (SQ IN)           00150
C                 MODULUS OF ELASTICITY OF CABLE         (KSI)             00160
C                 DENSITY OF WATER                       (PCF)             00170
C                 DENSITY OF CABLE                       (PCF) 
C                                                                          00180
      IMPLICIT REAL*8 (A-H,O-Z)                                            00190
      INTEGER*4 PAGE
      CHARACTER*8 TIME 
      CHARACTER*11 DATE
      CHARACTER*23 DATIME
      DIMENSION XLX(30,2),HX(30,2),THX(30,2),TVERT(30)                     00240
C     
C                                                                          00260
      ICARD = 5                                                            00270
      LIST = 3
C                                                                          00300
C     OPEN FILES                                                           00330
C
      CALL PARM
C
C     GET DATE, TIME
C
C    5 CALL LIB$DATE_TIME (DATIME)
    5 CALL DATE_AND_TIME(DATE,TIME)
C      TIME = DATIME(13:20)
C      DATE = DATIME(1:11)
C
C     INITIALIZATION                                                       00340
C                                                                          00350
      PAGE = 1 
      Z = 1.0                                                              00360
      K = 1                                                                00370
      J = 1                                                                00380
      M = 17                                                               00390
      DELTA = 0.25D0                                                       00400
C
C     READ INPUT
C
      INUNIT=5
      OUNIT=6
   10 READ (INUNIT,END=445,ERR=998,FMT=*) XL,H,TH,AREA,E,DH20,DCAB
      WAIR = DCAB/144 * AREA
      WWATER = WAIR - DH20 * AREA / 144                                    00480
      WX = WWATER / 1000                                                   00490
C                                                                          00500
      WRITE (OUNIT,20)DATE,TIME,PAGE                                       00510
   20 FORMAT('1',/,' CABLE4          ',A11,'  TIME ',A8,15X,'PAGE',I3)
      WRITE (OUNIT,21)
   21 FORMAT('  HORIZ. DISTANCE    DEPTH    THRUST    AREA    E (KSI)  W   00530
     1ATER (PCF)  CABLE(PCF)')      
      WRITE (OUNIT,30) XL,H,TH,AREA,E,DH20,DCAB                            00540
   30 FORMAT( 5X,F8.2,7X,F6.2,3X,F7.2,3X,F5.2,5X,F6.0,5X,F6.1,6X,F6.2/)    00550
C                                                                          00560
      Z = Z + 1.0                                                          00570
      XLIN = XL                                                            00580
      HIN = H                                                              00590
      XLX(1,1) = XL                                                        00600
      HX(1,1) = H                                                          00610
      XLC = DSQRT(XL**2 + H**2)                                            00620
      W = WX * XLC                                                         00630
   40 BX = W / (2.0 * TH * XL)                                             00640
      AX = (H - BX * XL**2) / XL                                           00650
      XLENG = XL + H**2 / (2.0 * XL) - H**4 / (8.0 * XL**3) + H**6 / (16   00660
     1   .0 * XL**5) + (XL**2 / 6.0 - H**2 / 4.0 + 5.0 * H**4 / (16.0 *    00670
     2   XL**5)) * XL * BX**2                                              00680
      XDLENG = (TH / (AREA * E)) * (XL + H**2 / XL + BX**2 * XL**3 / 3.)   00690
      XUL = XLENG - XDLENG                                                 00700
      IF (K - 10) 50,50,60                                                 00710
   50 W = WX * XUL                                                         00720
      K = K + 1                                                            00730
      GO TO 40                                                             00740
   60 K = 1                                                                00750
C                                                                          00760
      WRITE (OUNIT,70) XUL
   70 FORMAT(' UNSTRETCHED CABLE LENGTH IN FEET ',F10.4,//)                00780
      WRITE (OUNIT,80)                                                     00820
   80 FORMAT('   HORIZ. DIST.   - - - - - - - -  THRUST  - - - - - - - - DB00830
     1    SAG       CHORD      CABLE      CABLE     HORIZ.'/             DB00840
     2       18X,'HORIZ.    VERT. TOP  VERT. BOT    CABLE                DB00850
     3ANGLE     ANG. TOP   ANG. BOT  SPG. CST.'/                         DB00860
     4       5X,'(FEET)       (KIPS)      (KIPS)     (KIPS)     (KIPS)   DB00870
     5 (FEET)     (DEG)      (DEG)      (DEG)     (K/FT.)')              DB00880
C                                                                          00890
   90 WRITE (OUNIT,100)                                                    00900
  100 FORMAT(' ')                                                          00910
      DO 230 I = 1,M                                                       00920
         A = XL * (XL**2 /6.0 - H**2 / 4.0)                                00930
         B = W * XL**2 / (6.0 * E * AREA)                                  00940
         C=XL + H**2 / (2.0 * XL) - H**4 / (8.0 * XL**3) + H**6 / (16.0    00950
     1   * XL**5) - XUL                                                    00960
         D = -W * (1.0 + H**2 / XL**2) / (2.0 * E * AREA)                  00970
         P = B / A                                                         00980
         Q = C / A                                                         00990
         R = D / A                                                         01000
         ALFA = (3.0 * Q - P**2) / 3.0                                     01010
         BETA = (2.0 * P**3 - 9.0 * P * Q + 27.0 * R) / 27.0               01020
         L = 1                                                             01030
         IF (ALFA) 110,120,130                                             01040
  110    X1 = (-ALFA)**0.5                                                 01050
         GO TO 140                                                         01060
  120    X1 = -BETA**(1.0 / 3.0)                                           01070
         GO TO 140                                                         01080
  130    X1 = -BETA / ALFA                                                 01090
  140    FX1 = X1**3 + ALFA * X1 + BETA                                    01100
         X1 = X1 - FX1 / (3.0 * X1**2 + ALFA)                              01110
         IF (L - 20) 150,150,160                                           01120
  150    L = L + 1                                                         01130
         GO TO 140                                                         01140
  160    BX = X1 - P / 3.0                                                 01150
         AX = (H - BX * XL**2) / XL                                        01160
         THX(I,J) = W / (2.0 * BX * XL)                                    01170
         SPCNST = (THX(I,J) - THX(I-1,J)) / DELTA
         IF (I.EQ.1) SPCNST = 0
         ANGT = DATAN(AX + 2.0 * BX * XL)                                  01180
         ANGB = DATAN(AX)                                                  01190
         DANGT = DATAND(AX + 2.0 * BX * XL)                                01180
         DANGB = DATAND(AX)                                                01190
         TVT = THX(I,J) * DSIN(ANGT) / DCOS(ANGT)                          01200
         TVB = THX(I,J) * DSIN(ANGB) / DCOS(ANGB)                          01210
         TCABL = (TVT**2 + THX(I,J)**2)**0.5                               01220
         SAG = WX * XL * DSQRT(XL**2 + H**2) / (8 * THX(I,J))              01230
         CHDA = DATAND(H / XLX(I,J))
C        LOW POINT OF CABLE                                                01240
C        XLOW=XLX(I,J)/2.0+(THX(I,J)/WX)*(H/XLX(I,J))                    DB01250
C        YLOW=(WX*XLOW*(XLX(I,J)-XLOW))/(2.0*THX(I,J))+XLOW*(H/XLX(I,J)) DB01260
C        IF (H-XLOW) 170,190,190                                         DB01270
C 170    XLOO=XLOW                                                       DB01280
C 180    XLOO=XLOO-10.0                                                  DB01290
C        YLOO=(WX*XLOO*(XLX(I,J)-XLOO))/(2.0*THX(I,J))+XLOO*(H/XLX(I,J)) DB01300
C        IF (H-YLOO) 180,190,190                                         DB01310
C 190    CONTINUE                                                        DB01320
         WRITE (OUNIT,200) XLX(I,J),THX(I,J),TVT,TVB,TCABL,SAG,CHDA,       01330
     1   DANGT,DANGB,SPCNST                                                01340
  200    FORMAT(' ',3X,F8.3,4X,F8.2,3X,F8.2,3X,F8.2,3X,F8.2,3X,F6.2,       01350
     1          3X,F8.2,3X,F8.2,3X,F8.2,3X,F8.2)                           01360
         N = I + 1                                                         01370
         IF (K - 1) 210,210,220                                            01380
  210    XL = XL + DELTA                                                   01390
         XLX(N,J) = XL                                                     01400
         HX(N,J) = H                                                       01410
         GO TO 230                                                         01420
  220    XL = XL - DELTA                                                   01430
         XLX(N,J) = XL                                                     01440
         HX(N,J) = H                                                       01450
  230    CONTINUE                                                          01460
      K = K + 1                                                            01470
      J = J + 1                                                            01480
      XL = XLIN                                                            01490
      H = HIN                                                              01500
      XLX(1,2) = XL                                                        01510
      HX(1,2) = H                                                          01520
      IF (K - 2) 90,90,240                                                 01530
C                                                                          01540
  240 H = HIN                                                              01550
      XL = XLIN                                                            01560
      K = 1                                                                01570
      J = 1                                                                01580
      M = 17                                                               01590
C                                                                          01600
      PAGE = PAGE + 1
      WRITE (OUNIT,20)DATE,TIME,PAGE
      WRITE (OUNIT,21)
      WRITE (OUNIT,30)XL,H,TH,AREA,E,DH20,DCAB
C                                                                          01630
      WRITE (OUNIT,260)                                                    01640
  260 FORMAT(6X,'DEPTH       - - - - - - - - THRUST  - - - - - - - -     DB01650
     1 SAG       CHORD      CABLE      CABLE      VERT.'/                DB01660
     2  18X,'HORIZ.    VERT. TOP  VERT. BOT   CABLE                ANGLE DB01670
     3     ANG. TOP   ANG.BOT   SPG. CST.'/                              DB01680
     4  6X,'(FEET)      (KIPS)      (KIPS)     (KIPS)    (KIPS)    (FEET DB01690
     5)     (DEG)      (DEG)      (DEG)     (K/FT.)')                    DB01700
C                                                                          01710
  270 WRITE (OUNIT,100)                                                    01720
      DO 400 I = 1,M                                                       01730
         A = XL * (XL**2 / 6.0 - H**2 / 4.0)                               01740
         B = W * XL**2 / (6.0 * E * AREA)                                  01750
         C=XL + H**2 / (2.0 * XL) - H**4 / (8.0 * XL**3) + H**6 / (16.0    01760
     1   * XL**5) - XUL                                                    01770
         D = -W * (1.0 + H**2 / XL**2) / (2.0 * E * AREA)                  01780
         P = B / A                                                         01790
         Q = C / A                                                         01800
         R = D / A                                                         01810
         ALFA = (3.0 * Q - P**2) / 3.0                                     01820
         BETA = (2.0 * P**3 - 9.0 * P * Q + 27.0 * R) / 27.0               01830
         L = 1                                                             01840
         IF (ALFA) 280,290,300                                             01850
  280    X1 = (-ALFA)**0.5                                                 01860
         GO TO 310                                                         01870
  290    X1 = (-BETA)**(1.0 / 3.0)                                         01880
         GO TO 310                                                         01890
  300    X1 = -BETA / ALFA                                                 01900
  310    FX1 = X1**3 + ALFA * X1 + BETA                                    01910
         X1 = X1 - FX1 / (3.0 * X1**2 + ALFA)                              01920
         IF (L - 20) 320,320,330                                           01930
  320    L = L + 1                                                         01940
         GO TO 310                                                         01950
  330    BX = X1 - P / 3.0                                                 01960
         AX = (H - BX * XL**2) / XL                                        01970
         THX(I,J) = W / (2.0 * BX * XL)                                    01980
         ANGT = DATAN(AX + 2.0 * BX * XL)                                  01990
         ANGB = DATAN(AX)                                                  02000
         DANGT = DATAND(AX + 2.0 * BX * XL)                                01990
         DANGB = DATAND(AX)                                                02000
         TVERT(I) = THX(I,J) * DSIN(ANGT) / DCOS(ANGT)                     02010
         SPGCST = (TVERT(I) - TVERT(I-1)) / DELTA
         IF (I.EQ.1) SPGCST = 0.0
         TVB = THX(I,J) * DSIN(ANGB) / DCOS(ANGB)                          02020
         TCABL = (TVERT(I)**2 + THX(I,J)**2)**0.5
         SAG = WX * XL * DSQRT(XL**2 + H**2) / (8 * THX(I,J))              02030
         CHDA = DATAND(HX(I,J) / XL)
C        LOW POINT OF CABLE                                                02040
C        XLOW=XL/2.0+(THX(I,J)/WX)*(HX(I,J)/XL)                          DB02050
C        YLOW=(WX*XLOW*(XL-XLOW))/(2.0*THX(I,J))+XLOW*(HX(I,J)/XL)       DB02060
C        IF (HX(I,J)-XLOW) 340,360,360                                   DB02070
C 340    XLOO=XLOW                                                       DB02080
C 350    XLOO=XLOO-10.0                                                  DB02090
C        YLOO=(WX*XLOO*(XL-XLOO))/(2.0*THX(I,J))+XLOO*(HX(I,J)/XL)       DB02100
C        IF (HX(I,J)-YLOO) 350,360,360                                   DB02110
C 360    CONTINUE                                                        DB02120
         WRITE (OUNIT,370) HX(I,J),THX(I,J),TVERT(I),TVB,TCABL,SAG,CHDA,
     1   DANGT,DANGB,SPGCST               
  370    FORMAT(' ',3X,F8.3,4X,F8.2,3(3X,F8.2),3X,F6.2,3X,F8.2,3X,F8.2,  DB02140
     1          3X,F8.2,3X,F8.2)                                         DB02150
C                                                                          02160
         N = I + 1                                                         02170
         IF (K - 1) 380,380,390                                            02180
  380    H = H + DELTA                                                     02190
         XLX(N,J) = XL                                                     02200
         HX(N,J) = H                                                       02210
         GO TO 400                                                         02220
  390    H = H - DELTA                                                     02230
         XLX(N,J) = XL                                                     02240
         HX(N,J) = H                                                       02250
         M = 22                                                            02260
  400    CONTINUE                                                          02270
      K = K + 1                                                            02280
      J = J + 1                                                            02290
      XL = XLIN                                                            02300
      H = HIN                                                              02310
      XLX(1,2) = XL                                                        02320
      HX(1,2) = H                                                          02330
      IF (K - 2) 270,270,410                                               02340
  410 GO TO 5                                                              02350
  445 GOTO 450                                                             02380
  998 WRITE(*,*)'   READ ERROR, CHECK YOUR DATA FILE.'
  450 STOP                                                                 02390
      END                                                                  02400
C
C
C
C
      SUBROUTINE PARM
      CHARACTER*16 FLNM1, FLNM2
      INTEGER OUNIT
	INTEGER INUNIT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  STANDARD REPORT OUTPUT UNIT
      OUNIT=6
C  STANDARD FILE INPUT UNIT
      INUNIT=5
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
10    WRITE(*,110)
110   FORMAT(//,
     1' Enter input file name:',/)
           READ(*,ERR=10,FMT='(A)')FLNM1
              DO 15 I=1,16
               IF(FLNM1(I:I).EQ.'.')GOTO 25
15             CONTINUE
              DO 20 I=1,16
               IF(FLNM1(I:I).EQ.' ')THEN
                 IE = I+3
                 FLNM1(I:IE) = '.dat'
                 GOTO 25
               END IF
20            CONTINUE
25        WRITE(*,FMT='(/,'' '',1X,A16,'' used for input '',//)')FLNM1
35    WRITE(*,135)
135   FORMAT(//,
     1' Enter output file name:',/)
           READ(*,ERR=35,FMT='(A)')FLNM2
              DO 40 I=1,16
               IF(FLNM2(I:I).EQ.'.')GOTO 60
40             CONTINUE
              DO 50 I=1,16
                 IF(FLNM2(I:I).EQ.' ')THEN
                  IE = I+3
                  FLNM2(I:IE) = '.out'
                  GOTO 60
                 END IF
50            CONTINUE
60       WRITE(*,FMT='(/,'' '',1X,A16,'' used for output '',//)')FLNM2
         OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')
         OPEN(OUNIT,FILE=FLNM2,STATUS='REPLACE')
      RETURN
      END
C
C
C     
