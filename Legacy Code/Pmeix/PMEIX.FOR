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

C    PMEIX                                                              PME00010
C     THIS PROGRAM WAS RECEIVED BY THE WSDOT FROM THE FHWA AND          PME00010
C     INSTALLED JUNE, 1985                                              PME00010
C                                                                       PME00010
C     ADDITIONAL DOCUMENTATION AND QUESTIONS OF A GENERAL NATURE MAY    PME00010
C     BE ADDRESSED TO:                                                  PME00010
C                                                                       PME00010
C          MR. C. T. CHANG                                              PME00010
C          FHWA/TFHRC (HRT-10)                                          PME00010
C          6300 GEORGETOWN PIKE                                         PME00010
C          MCLEAN, VA 22101-2296                                        PME00010
C          PHONE:  (703) 285-2357                                       PME00010
C                                                                       PME00010
C     PROBEMS MAY BE ADDRESSED TO:                                      PME00010
C                                                                       PME00010
C          STUART KOPPERMAN                                             PME00010
C          ONSITE SUPPORT SERVICES GROUP                                PME00010
C          FHWA/TFHRC (FAIRBANK BLDG, RM G-16)                          PME00010
C          6300 GEORGETOWN PIKE                                         PME00010
C          MCLEAN, VA 22101-2296                                        PME00010
C          PHONE:  (703) 285-2486                                       PME00010
C                                                                       PME00010
C     THIS PROGRAM GIVES A SET OF VALUES OF -EI- FOR VARIOUS VALUES     PME00020
C     OF MOMENTS COMBINED WITH AXIAL LOADS RANGING FROM ZERO TO ANY     PME00030
C     LOAD LESS THAN THE SQUASH LOAD.                                   PME00040
C                                                                       PME00050
C     THE PROGRAM CAN TREAT SQUARE, RECTANGULAR OR CIRCULAR SHAPES      PME00060
C     OF CONCRETE WITH REINFORCEMENT OF ANY GRADE.. CIRCULAR SHAPES     PME00070
C     CAN BE SPECIFIED AS WITH OR WITHOUT A STEEL SHELL, WITH OR        PME00080
C     WITHOUT A TUBULAR STEEL CORE (NO CONCRETE IN CORE).               PME00090
C                                                                       PME00100
C     ---- ALL UNITS SHALL BE INPUT IN INCHES AND KIPS ----             PME00110
C                                                                       PME00120
C     INPUT FORMATS ARE AS FOLLOWS:                                     PME00130
C                                                                       PME00140
C     1. FORMAT(8A10). THIS LINE IS FOR IDENTIFICATION OF THE           PME00150
C     PROBLEM.  WILL READ ANY CHARACTER IN THE FIRST 80 COLUMNS.        PME00160
C                                                                       PME00170
C     2. FORMAT(2I5). THIS LINE IS FOR SHAPE IDENTIFICATION AND         PME00180
C     NUMBER OF LOAD CASES.                                             PME00190
C           SHAPE IDENTIFICATION:                                       PME00200
C               1=RECTANGULAR                                           PME00210
C              10=CIRCULAR WITHOUT SHELL OR CORE                        PME00220
C              20=CIRCULAR WITH SHELL, WITHOUT CORE                     PME00230
C              30=CIRCULAR WITH SHELL AND CORE                          PME00240
C     THE NUMBER OF LOAD CASES SHALL NOT EXCEED TEN (10).               PME00250
C                                                                       PME00260
C     3. FORMAT(F10.2). THIS LINE IS FOR THE APPLIED AXIAL LOADS.       PME00270
C     ONE (1) LINE FOR EACH LOAD CASE.                                  PME00280
C                                                                       PME00290
C     4. FORMAT(4F10.2). THIS LINE IS FOR THE COMPRESSIVE STRENGTH      PME00300
C     OF THE CONCRETE, YIELD STRENGTH OF THE REINFORCEMENT, YIELD       PME00310
C     STRENGTH OF THE SHELL OR CORE STEEL, AND THE MODULUS OF           PME00320
C     ELASTICITY OF THE STEEL.                                          PME00330
C                                                                       PME00340
C     5. FORMAT(5F10.2). THIS LINE IS FOR THE WIDTH OF THE SECTION      PME00350
C     (SPECIFY AS 0.0 IF CIRCULAR), DEPTH OF THE SECTION (EXTERNAL      PME00360
C     DIAMETER IF CIRCULAR), EXTERNAL DIAMETER OF THE INNER TUBE,       PME00370
C     THICKNESS OF THE OUTER SHELL, AND THE THICKNESS OF THE INNER      PME00380
C     TUBE                                                              PME00390
C                                                                       PME00400
C     6. FORMAT(2I5,F10.2). THIS LINE IS FOR THE NUMBER OF REBARS,      PME00410
C     THE NUMBER OF ROWS OF REBARS, AND THE CONCRETE COVER FROM THE     PME00420
C     CENTER OF THE REBAR TO THE EDGE OF THE CONCRETE.                  PME00430
C                                                                       PME00440
C     7. FORMAT(F10.2)  THIS LINE IS FOR THE AREA OF REINFORCEMENT      PME00450
C     IN EACH ROW, STARTING FROM THE TOP ROW OF THE SECTION.            PME00460
C     ONE LINE FOR EACH ROW OF REBAR.  THE NUMBER OF ROWS SHALL         PME00470
C     NOT EXCEED FIFTY (50).                                            PME00480
C                                                                       PME00490
C     8. FORMAT(F10.2). THIS LINE IS REQUIRED ONLY IF THE SECTION       PME00500
C     IS RECTANGULAR OR SQUARE.  IT IS FOR THE DISTANCE FROM THE        PME00510
C     CENTROIDAL AXIS TO EACH ROW OF REBAR, STARTING FROM THE TOP       PME00520
C     ROW.  VALUES ARE POSITIVE (+) IF THE ROW IS ABOVE THE AXIS,       PME00530
C     AND NEGATIVE (-) IF THE ROW IS BELOW THE AXIS.                    PME00540
C     ONE LINE FOR EACH ROW.                                            PME00550
C                                                                       PME00560
C    REVISION .A. MADE BY PAUL WOCKEN JULY, 1985                        PME00560
C             FIXED PROBLEMS WITH REPORT HEADER                         PME00560
C                                                                       PME00560
C    REVISION .B. CONVERTED FOR PC BY NEAL CAMPBELL MAR, 1986           .B.00565
C                                                                       .T.
C    REVISION .T. CONVERTED TO VMS BY TERRY BEEBE FEBRUARY 1990         .T.
C                                                                       .T.
C    RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C
C    RICHARD BRICE - 08/08/93 - REVISED FORMAT SPECS FOR DIAMETER AND
C                               EI OUTPUT.
C                                                                       PME00570
      COMMON/ONE/PCON,AMCON,PSTEEL,FFC,PSHELL,AMSHEL ,AC,PSHT,LL(50)    PME00580
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME00590
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME00600
      COMMON/FOUR/KA,II,KK,PCAP,RCS,ROS,RT,RTS,DIF,ASH(100),IJ,PCT,NBARSPME00610
      COMMON/FIVE/P(10),AS(50),XS(50),SSN(50),SSS(50),AXS(50),BARFY,STR PME00620
      COMMON/SIX/X(100),CSN(100),CSS(100),ACC(100),ATOT(100),SSH(100)   PME00630
      COMMON/SEVEN/RSSH(100),XX(100),RXX(100),AM(50),XXS(100),RXXS(100) PME00640
      COMMON/EIGHT/TA(100),RTA(100),TACC(100),RACC(100),RX(100),ES,EC,IKPME00650
      COMMON/NINE/RASH(100),RATOT(100),RTOT(100),RTACC(100),TOT(100)    PME00660
      CHARACTER*8 REVDAT, TIME                                          .T.
      CHARACTER*11 DATE                                                 .T.
      CHARACTER*23 DATIME                                               .T.
      CHARACTER*80 TITLE                                                .T.
      REAL*8    ANAME(10)                                               .A.00670
      DIMENSION DDNA(2)                                                 PME00680
      REAL NDNA                                                         PME00720
C                                                                       PME00690
C     READ INPUT DATA                                                   PME00700
C                                                                       PME00710
C OPEN FILES FOR PC                                                     .B.
C                                                                       .B.
      CALL PARM                                                         .B.
C                                                                       .B.
C                                                                       .B.
      INUNIT=5                                                          .T.
      OUNIT=6                                                           .T.
      CALL LIB$DATE_TIME (DATIME)                                       .T.
      TIME = DATIME(13:20)                                              .T.
      DATE = DATIME(1:11)                                               .T.
      READ (INUNIT,1000)TITLE                                           .T.
1000  FORMAT(A)                                                         .T.
      WRITE(OUNIT,1010)TITLE                                            .T.
1010  FORMAT(4X,A80)                                                    .T.
      WRITE(OUNIT,1015)DATE,TIME                                        .T.
1015  FORMAT(11X,A11,14X,'TIME ',A8,/)                                  .T.  
      READ (INUNIT,1)ISHAPE,NP                                          .T.00740
      DO 1100 J=1,NP                                                    .T.
1100  READ (INUNIT,2)P(J)                                               .T.00750
      READ (INUNIT,3)FC,BARFY,TUBEFY,ES                                 .T.00760
      READ (INUNIT,4)WIDTH,OD,DT,T,TT                                   .T.00770
      IF (ISHAPE.EQ.1) GOTO 18                                          .T.00790
      WRITE (OUNIT,6)OD,T,DT,TT                                         .T.
      GOTO 19                                                           PME00810
C                                                                       PME00820
C     CALL SUBROUTINE SETUP TO COMPUTE THE SQUASH LOAD                  PME00830
C                                                                       PME00840
  18  WRITE (OUNIT,7)WIDTH,OD                                           .T.00850
  19  READ (INUNIT,1200)NBARS,NROWS,COVER                               .T.
1200  FORMAT(2I5,F10.2)                                                 .T.
      DO 1210 JJ=1,NROWS                                                .T.
1210  READ (INUNIT,1220)AS(JJ)                                          .T.
1220  FORMAT(F10.2)                                                     .T.
      DO 1225 JJ=1,NROWS                                                .T.
1225  READ (INUNIT,1230,END=1300)XS(JJ)                                 .T. 
1230  FORMAT(F10.2)                                                     .T.
1300  CALL SETUP                                                        .T.00860
      WRITE (OUNIT,5)NBARS                                              .T.00870
      WRITE (OUNIT,8)NROWS                                              .T.00880
      WRITE (OUNIT,11)COVER                                             .T.00890
      WRITE (OUNIT,9)                                                   .T.00900
      DO 1600 JJ=1,NROWS                                                .T.   
1600  WRITE (OUNIT,10)JJ,AS(JJ),XS(JJ)                                  .T.00910
      WRITE (OUNIT,12)FC                                                .T.00920
      WRITE (OUNIT,13)BARFY                                             .T.00930
      WRITE (OUNIT,14)TUBEFY                                            .T.00940
      WRITE (OUNIT,15)ES                                                .T.00950
      WRITE (OUNIT,16)EC                                                .T.00960
      WRITE (OUNIT,17)PCAP                                              .T.00970
C                                                                       PME00980
C     CHECK WHETHER OR NOT THE SQUASH LOAD IS LARGER THAN THE AXIAL LOADPME00990
C                                                                       PME01000
      DO 500 J=1,NP                                                     PME01010
      IF (PCAP-P(J)) 430,26,26                                          PME01020
  26  WRITE (OUNIT,30)P(J)                                              .T.01030
      WRITE (OUNIT,21)                                                  .T.01040
      WRITE (OUNIT,23)                                                  .T.01050
C                                                                       PME01060
C     SET UP INITIAL VALUE FOR PHI                                      PME01070
      PHI=.000001                                                       PME01080
C                                                                       PME01090
C                                                                       PME01100
C                                                                       PME01110
  35  NSWTCH=0                                                          PME01120
      DDNA(1)=0.0                                                       PME01130
      DDNA(2)=OD                                                        PME01140
C                                                                       PME01150
C                                                                       PME01160
C                                                                       PME01170
      DNA=OD                                                            PME01180
C                                                                       PME01190
C     CALL CSTRESS TO COMPUTE CONCRETE STRESS FROM STRESS STRAIN CURVE  PME01200
C                                                                       PME01210
  40  CALL CSTRES                                                       PME01220
      PCON=0.0                                                          PME01230
      IF (DNA-OD) 33,33,55                                              PME01240
  33  K=INT(DNA/DEL)                                                    PME01250
      GO TO 481                                                         PME01260
  55  K=60                                                              PME01270
 481  CONTINUE                                                          PME01280
      DO 90 I=1,K                                                       PME01290
      XX(I)=X(I)+RO-DNA                                                 PME01300
      IF (ISHAPE.EQ.1) GOTO 85                                          PME01310
      XXS(I)=XX(I)*XX(I)                                                PME01320
      IF (RINT.GE.ABS(XX(I))) GOTO 47                                   PME01330
      ACC(I)=0.0                                                        PME01340
      GOTO 44                                                           PME01350
  47  ACC(I)=2.*SQRT(RINTS-XXS(I))*DEL                                  PME01360
  44  PCON=PCON+CSS(I)*ACC(I)                                           PME01370
      GOTO 90                                                           PME01380
  85  PCON=PCON+AC*CSS(I)                                               PME01390
  90  CONTINUE                                                          PME01400
C                                                                       PME01410
C     CALL SUBROUTINE STEELP TO COMPUTE LOAD TAKEN BY REBARS            PME01420
C                                                                       PME01430
      CALL STEELP                                                       PME01440
      PTOT=PCON+PSTEEL                                                  PME01450
      IF (ISHAPE.EQ.1) GOTO 86                                          PME01460
      IF (ISHAPE.EQ.10) GOTO 86                                         PME01470
C                                                                       PME01480
C     CALL SUBROUTINE SHEELP TO COMPUTE LOAD TAKEN BY STEEL SHELL       PME01490
C                                                                       PME01500
      CALL SHELLP                                                       PME01510
      PTOT=PTOT+PSHELL                                                  PME01520
      IF (ISHAPE.EQ.20) GOTO 86                                         PME01530
C                                                                       PME01540
C     CALL SUBROUTINE COREP TO COMPUTE LOAD TAKEN BY CORE SHELL         PME01550
C                                                                       PME01560
      CALL COREP                                                        PME01570
      PTOT=PTOT+PCORE                                                   PME01580
  86  IF(PTOT .GT. P(J)) GOTO 89                                        PME01590
      DDNA(1)=DNA                                                       PME01600
      IF(NSWTCH .EQ. 1)GOTO 92                                          PME01610
      DDNA(2)=5.0*DNA                                                   PME01620
      GOTO 92                                                           PME01630
  89  DDNA(2)=DNA                                                       PME01640
      NSWTCH=1                                                          PME01650
C                                                                       PME01660
C     COMPUTE NEW NEUTRAL AXIS                                          PME01670
C                                                                       PME01680
  92  NDNA=(DDNA(1)+DDNA(2))/2.0                                        PME01690
      IF(ABS(DNA-NDNA) .LE. .0001) GOTO 50                              PME01700
      DNA=NDNA                                                          PME01710
      GOTO 40                                                           PME01720
  50  AMCON=0.0                                                         PME01730
      DO 100 I=1,K                                                      PME01740
      IF(ISHAPE.EQ.1) GOTO 65                                           PME01750
      AMCON=AMCON+CSS(I)*ACC(I)*(XX(I))                                 PME01760
      GOTO 100                                                          PME01770
  65  AMCON=AMCON+AC*CSS(I)*(XX(I))                                     PME01780
 100  CONTINUE                                                          PME01790
      AMSTEE =0.0                                                       PME01800
      DO 200 I=1,NROWS                                                  PME01810
  200 AMSTEE =AMSTEE +SSS(I)*AS(I)*XS(I)                                PME01820
      AMTOT=AMCON+AMSTEE                                                PME01830
      IF (ISHAPE.EQ.1) GOTO 110                                         PME01840
      IF (ISHAPE.EQ.10) GOTO 110                                        PME01850
      CALL SHELLM                                                       PME01860
      AMTOT=AMTOT+AMSHEL                                                PME01870
      IF (ISHAPE.EQ.20) GOTO 110                                        PME01880
      CALL COREM                                                        PME01890
      AMTOT=AMTOT+AMCORE                                                PME01900
C                                                                       PME01910
C     COMPUTE EI-VALUE AND PRINT COMPUTATION RESULT                     PME01920
C                                                                       PME01930
  110 EI=AMTOT/PHI                                                      PME01940
      CSNMAX=PHI*(DNA-T)                                                PME01950
      WRITE(OUNIT,400)AMTOT,EI,PHI,CSNMAX,DNA                           .T.01960
      IF (CSNMAX.GT..0038) GOTO 500                                     PME01970
      GOTO 450                                                          PME01980
  430 WRITE(OUNIT,440)                                                  .T.01990
      GOTO 500                                                          PME02000
  450 IF (PHI.GT..00005) GOTO 460                                       PME02010
      PHI=PHI+.000004                                                   PME02020
      GOTO 35                                                           PME02030
  460 PHI=PHI+.00003                                                    PME02040
      GOTO 35                                                           PME02050
  500 CONTINUE                                                          PME02060
  1   FORMAT (2I5)                                                      PME02070
  2   FORMAT (F10.2)                                                    PME02080
  3   FORMAT (4F10.2)                                                   PME02090
  4   FORMAT (5F10.2)                                                   PME02100
  5   FORMAT(5X,'NO. OF REBARS              ',I5)                       PME02110
C CHANGED DIAMETER TO FROM F5.2 TO F8.2  RAB 8/8/93
  6   FORMAT (5X,'SHAPE : CIRCULAR',/,5X,'DIAMETER  ',7X,F8.2,/,5X,    .B.02120
     *'SHELL THICKNESS     ',F5.2,/,5X,'CORE TUBE O.D.      ',          .B.02130
     *F5.2,/,5X,'CORE TUBE THICKNESS ',F5.2,/)                          PME02140
  7   FORMAT (5X,'SHAPE : RECTANGULAR',/,5X,'WIDTH               ',     PME02150
     *F5.2,5X,'DEPTH                ',F5.2,/)                           PME02160
  8   FORMAT (5X,'ROWS OF REBARS             ',I5)                      PME02170
  9   FORMAT(5X,'LAYER',11X,'AREA',7X,'ORDINATE')                       PME02180
 10   FORMAT(5X,I5,5X,F10.2,5X,F10.2,/)                                 PME02190
  11  FORMAT (5X,'COVER',/,5X,'(BAR CENTER TO CONCR EDGE)',             PME02200
     *3X,F3.1,/)                                                        PME02210
  12     FORMAT (5X,'CONCRETE CYLINDER STRENGTH    ',F10.2,'KSI')       PME02220
  13  FORMAT (5X,'REBARS YIELD STRENGTH         ',F10.2,'KSI')          PME02230
  14  FORMAT (5X,'SHELL/TUBE YIELD STRENGTH',5X,F10.2,'KSI')            PME02240
  15  FORMAT (5X,'MODULUS OF ELAST. OF STEEL    ',F10.2,'KSI')          PME02250
  16  FORMAT (5X,'MODULUS OF ELAST. OF CONCR    ',F10.2,'KSI')          PME02260
  17  FORMAT (5X,'SQUASH LOAD CAPACITY          ',F10.2,'KPS',//)       PME02270
  20  FORMAT(A8)                                                        .A.02280
  22  FORMAT(A8)                                                        .A.02290
  30  FORMAT('1',4X,'AXIAL LOAD = ',F10.2,2X,'KIPS')                    PME02300
  21  FORMAT(/13X,'MOMENT        EI         PHI   MAX STR   N AXIS')    PME02310
  23  FORMAT(13X,'IN KIPS     KIP-IN2               IN/IN       IN')    PME02320
  400 FORMAT(10X,F9.1,2X,E12.6,2X,F7.6,2X,F7.5,2X,F6.2)                 PME02330
  440 FORMAT(5X,'APPLIED LOAD MORE THAN SQUASH LOAD')                   PME02340
      STOP                                                              PME02350
      END                                                               PME02360
C
C
C
      SUBROUTINE SETUP                                                  PME02370
C                                                                       PME02380
C     THIS SUBROUTINE IS USED TO COMPUTE THE SQUASH LOAD PCAP OF MEMBERSPME02390
C                                                                       PME02400
      COMMON/ONE/PCON,AMCON,PSTEEL,FFC,PSHELL,AMSHEL ,AC,PSHT,LL(50)    PME02410
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME02420
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME02430
      COMMON/FOUR/KA,II,KK,PCAP,RCS,ROS,RT,RTS,DIF,ASH(100),IJ,PCT,NBARSPME02440
      COMMON/FIVE/P(10),AS(50),XS(50),SSN(50),SSS(50),AXS(50),BARFY,STR PME02450
      COMMON/SIX/X(100),CSN(100),CSS(100),ACC(100),ATOT(100),SSH(100)   PME02460
                                                                        PME02470
      COMMON/SEVEN/RSSH(100),XX(100),RXX(100),AM(50),XXS(100),RXXS(100) PME02480
      COMMON/EIGHT/TA(100),RTA(100),TACC(100),RACC(100),RX(100),ES,EC,IKPME02490
      COMMON/NINE/RASH(100),RATOT(100),RTOT(100),RTACC(100),TOT(100)    PME02500
      FACT=.7854                                                        PME02510
      EC=57.5*SQRT(FC*1000.)                                            PME02520
      STR=1.7*FC/EC                                                     PME02530
      FFC=FC                                                            PME02540
      DEL=OD/60.                                                        PME02550
      RO=.5*OD                                                          PME02560
      IF(ISHAPE.EQ.1) GOTO 4                                            PME02590
      ANBARS=NBARS                                                      PME02600
      LA=INT(ANBARS/2.)                                                 PME02610
      LB=2*LA                                                           PME02620
      NA=INT(ANBARS/4.)                                                 PME02630
      NB=4*NA                                                           PME02640
      ANGLE=6.2832/ANBARS                                               PME02650
      RS=RO-COVER-T                                                     PME02660
      DO 200 JJ=1,NROWS                                                 PME02670
      AJJ=JJ                                                            PME02680
      IF ( ANBARS-LB ) 7,6,7                                            PME02690
  7   AM(JJ)=(ANBARS+1.)/4.-AJJ/2.                                      PME02700
      GOTO 200                                                          PME02710
    6 IF(NBARS.EQ.NB) GOTO 100                                          PME02720
      AM(JJ)=ANBARS/4.-AJJ+.5                                           PME02730
      GOTO 200                                                          PME02740
  100 AM(JJ)=ANBARS/4.-AJJ+1.                                           PME02750
  200 XS(JJ)=RS*SIN(ANGLE*AM(JJ))                                       PME02760
  4   ARS=0.0                                                           PME02790
C                                                                       PME02800
C     COMPUTE THE TOTAL AREA OF REBARS                                  PME02810
C                                                                       PME02820
      DO 5 I=1,NROWS                                                    PME02830
  5   ARS=ARS+AS(I)                                                     PME02840
      IF(ISHAPE.EQ.1)GOTO 101                                           PME02850
      D=OD-T-T                                                          PME02860
      ROS=RO*RO                                                         PME02870
      RINT=.5*D                                                         PME02880
      RINTS=RINT*RINT                                                   PME02890
      ARC=FACT*D*D-ARS                                                  PME02900
C                                                                       PME02910
C     COMPUTE THE MAX. LOAD TAKEN BY WHOLE SECTION                      PME02920
C                                                                       PME02930
      A=FACT*((OD**2)-(D**2))                                           PME02940
      IF(ISHAPE.EQ.10) GOTO 103                                         PME02950
      IF(ISHAPE .EQ. 20) GOTO 103                                       PME02960
      RT=.5*DT                                                          PME02970
      DC=DT-TT-TT                                                       PME02980
      RC=.5*DC                                                          PME02990
      RTS=RT*RT                                                         PME03000
      RCS=RC*RC                                                         PME03010
      Z=RO+RT                                                           PME03020
      ATUBE=FACT*((DT**2)-(DC**2))                                      PME03030
      AIN=FACT*DT*DT                                                    PME03040
      PCAP=FFC*(ARC-AIN)+BARFY*ARS+TUBEFY*(A+ATUBE)                     PME03050
      DIF=RO-RT                                                         PME03060
      GOTO 104                                                          PME03070
  103 PCAP=FFC*ARC+BARFY*ARS+TUBEFY*A                                   PME03080
      GOTO 104                                                          PME03090
  101 AC=WIDTH*DEL                                                      PME03100
      ARC=WIDTH*OD-ARS                                                  PME03110
      PCAP=FFC*ARC+BARFY*ARS                                            PME03120
  1   FORMAT(2I5,F10.2)                                                 PME03130
  2   FORMAT(F10.2)                                                     PME03140
  3   FORMAT(F10.2)                                                     PME03150
  104 RETURN                                                            PME03160
      END                                                               PME03170
      SUBROUTINE CSTRES                                                 PME03180
C                                                                       PME03190
C     THIS SUBROUTINE IS WRITTEN TO COMPUTE THE STRESS OF CONCRETE FROM PME03200
C     STRESS STRAIN CURVE                                               PME03210
C                                                                       PME03220
      COMMON/ONE/PCON,AMCON,PSTEEL,FFC,PSHELL,AMSHEL ,AC,PSHT,LL(50)    PME03230
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME03240
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME03250
      COMMON/FIVE/P(10),AS(50),XS(50),SSN(50),SSS(50),AXS(50),BARFY,STR PME03260
      COMMON/SIX/X(100),CSN(100),CSS(100),ACC(100),ATOT(100),SSH(100)   PME03270
      N=INT(DNA/DEL)                                                    PME03280
      C=DNA-(FLOAT(N))*DEL                                              PME03290
      F=DEL-C                                                           PME03300
      K=60                                                              PME03310
      IF(DNA-OD) 10,10,20                                               PME03320
  10  X(1)=C+.5*DEL                                                     PME03330
      GOTO 60                                                           PME03340
  20  X(1)=(DNA-OD)+.5*DEL                                              PME03350
  60  DO 90 I=1,K                                                       PME03360
      IF (I.GT.1) X(I)=X(1)+(FLOAT(I-1))*DEL                            PME03370
      CSN(I)=X(I)*PHI                                                   PME03380
      IF (CSN(I)-STR) 70,70,80                                          PME03390
  70  CSS(I)=FFC*(2.*CSN(I)/STR-(CSN(I)/STR)**2)                        PME03400
      GOTO 90                                                           PME03410
  80  CSS(I)=FFC*(.85+.15*(.0038-CSN(I))/(.0038-STR))                   PME03420
  90  CONTINUE                                                          PME03430
      RETURN                                                            PME03440
      END                                                               PME03450
      SUBROUTINE STEELP                                                 PME03460
C                                                                       PME03470
C     THIS SUBROUTINE IS WRITTEN TO COMPUTE THE LOAD TAKEN BY REBARS    PME03480
C                                                                       PME03490
      COMMON/ONE/PCON,AMCON,PSTEEL,FFC,PSHELL,AMSHEL ,AC,PSHT,LL(50)    PME03500
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME03510
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME03520
      COMMON/FIVE/P(10),AS(50),XS(50),SSN(50),SSS(50),AXS(50),BARFY,STR PME03530
      COMMON/EIGHT/TA(100),RTA(100),TACC(100),RACC(100),RX(100),ES,EC,IKPME03540
      PSTEEL=0.0                                                        PME03550
      DO 100 JJ=1,NROWS                                                 PME03560
      AXS(JJ)=XS(JJ)+DNA-RO                                             PME03570
      SSN(JJ)=PHI*AXS(JJ)                                               PME03580
      SSS(JJ)=SSN(JJ)*ES                                                PME03590
      IF (SSS(JJ).GT.BARFY) SSS(JJ)=BARFY                               PME03600
      IF (SSS(JJ).LT.-BARFY) SSS(JJ)=-BARFY                             PME03610
      PSTEEL=PSTEEL+AS(JJ)*SSS(JJ)                                      PME03620
 100  CONTINUE                                                          PME03630
      RETURN                                                            PME03640
      END                                                               PME03650
      SUBROUTINE SHELLP                                                 PME03660
C                                                                       PME03670
C     THIS SUBROUTINE IS USED TO COMPUTE THE LOAD TAKEN BY THE STEEL    PME03680
C     SHELL                                                             PME03690
C                                                                       PME03700
      COMMON/ONE/PCON,AMCON,PSTEEL,FFC,PSHELL,AMSHEL ,AC,PSHT,LL(50)    PME03710
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME03720
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME03730
      COMMON/FOUR/KA,II,KK,PCAP,RCS,ROS,RT,RTS,DIF,ASH(100),IJ,PCT,NBARSPME03740
      COMMON/FIVE/P(10),AS(50),XS(50),SSN(50),SSS(50),AXS(50),BARFY,STR PME03750
      COMMON/SIX/X(100),CSN(100),CSS(100),ACC(100),ATOT(100),SSH(100)   PME03760
      COMMON/SEVEN/RSSH(100),XX(100),RXX(100),AM(50),XXS(100),RXXS(100) PME03770
      COMMON/EIGHT/TA(100),RTA(100),TACC(100),RACC(100),RX(100),ES,EC,IKPME03780
      COMMON/NINE/RASH(100),RATOT(100),RTOT(100),RTACC(100),TOT(100)    PME03790
      DIMENSION SM(100)                                                 PME03800
      PSHT=0.0                                                          PME03810
      PSHELL=0.0                                                        PME03820
      DO 103 I=1,K                                                      PME03830
      IF (RO.GT.ABS(XX(I))) GOTO 110                                    PME03840
      ASH(I)=0.0                                                        PME03850
      GOTO 106                                                          PME03860
  110 ATOT(I)=2.*SQRT(ROS-XXS(I))*DEL                                   PME03870
      ASH(I)=ATOT(I)-ACC(I)                                             PME03880
  106 SSH(I)=ES*CSN(I)                                                  PME03890
      IF (SSH(I).GT.TUBEFY) SSH(I)=TUBEFY                               PME03900
      IF (SSH(I).LT.-TUBEFY) SSH(I)=-TUBEFY                             PME03910
  103 PSHELL=PSHELL+ASH(I)*SSH(I)                                       PME03920
      IF (DNA-OD) 107,105,105                                           PME03930
  107 KA=59-K                                                           PME03940
      DO 104 I=1,KA                                                     PME03950
      RX(I)=X(I)-C+F                                                    PME03960
      RXX(I)=RO-DNA-RX(I)                                               PME03970
      RXXS(I)=RXX(I)*RXX(I)                                             PME03980
      IF (RINT.GT.ABS(RXX(I))) GOTO 47                                  PME03990
      RACC(I)=0.0                                                       PME04000
      GOTO 44                                                           PME04010
  47  RACC(I)=2.*SQRT(RINTS-RXXS(I))*DEL                                PME04020
  44  IF (RO.GT.ABS(RXX(I))) GOTO 48                                    PME04030
      RASH(I)=0.0                                                       PME04040
      GOTO 104                                                          PME04050
  48  RATOT(I)=2.*SQRT(ROS-RXXS(I))*DEL                                 PME04060
      RASH(I)=RATOT(I)-RACC(I)                                          PME04070
      SM(I)=RX(I)*PHI                                                   PME04080
C                                                                       PME04090
C     COMPUTE THE TENSILE FORCE TAKEN BY THE STEEL SHELL                PME04100
C                                                                       PME04110
      RSSH(I)=ES*SM(I)                                                  PME04120
      IF (RSSH(I).GT.TUBEFY) RSSH(I)=TUBEFY                             PME04130
      IF (RSSH(I).LT.-TUBEFY) RSSH(I)=-TUBEFY                           PME04140
  104 PSHT=PSHT+RASH(I)*RSSH(I)                                         PME04150
      G=F*PHI*ES*T*DEL                                                  PME04160
      PSHELL=PSHELL-PSHT-G                                              PME04170
  105 RETURN                                                            PME04180
      END                                                               PME04190
      SUBROUTINE SHELLM                                                 PME04200
C                                                                       PME04210
C     THIS SUBROUTINE IS USED TO CALCULATE THE BENDING MOMENT TAKEN BY  PME04220
C     THE STEEL SHELL                                                   PME04230
C                                                                       PME04240
      COMMON/ONE/PCON,AMCON,PSTEEL,FFC,PSHELL,AMSHEL ,AC,PSHT,LL(50)    PME04250
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME04260
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME04270
      COMMON/FOUR/KA,II,KK,PCAP,RCS,ROS,RT,RTS,DIF,ASH(100),IJ,PCT,NBARSPME04280
      COMMON/FIVE/P(10),AS(50),XS(50),SSN(50),SSS(50),AXS(50),BARFY,STR PME04290
      COMMON/SIX/X(100),CSN(100),CSS(100),ACC(100),ATOT(100),SSH(100)   PME04300
      COMMON/SEVEN/RSSH(100),XX(100),RXX(100),AM(50),XXS(100),RXXS(100) PME04310
      COMMON/EIGHT/TA(100),RTA(100),TACC(100),RACC(100),RX(100),ES,EC,IKPME04320
      COMMON/NINE/RASH(100),RATOT(100),RTOT(100),RTACC(100),TOT(100)    PME04330
      AMSHEL =0.0                                                       PME04340
      AMSHT=0.0                                                         PME04350
      DO 300 I=1,K                                                      PME04360
  300 AMSHEL =AMSHEL +SSH(I)*ASH(I)*XX(I)                               PME04370
      IF (DNA-OD) 400,502,600                                           PME04380
  400 DO 501 I=1,KA                                                     PME04390
  501 AMSHT=AMSHT+RSSH(I)*RASH(I)*(RX(I)+DNA-RO)                        PME04400
  502 AMSHEL =AMSHEL +AMSHT+G*(F/2.+DNA-RO)                             PME04410
  600 RETURN                                                            PME04420
      END                                                               PME04430
      SUBROUTINE COREP                                                  PME04440
C                                                                       PME04450
C     THIS SUBROUTINE IS WRITTEN TO COMPUTE THE LOAD TAKEN BY THE STEEL PME04460
C     CORE                                                              PME04470
      COMMON/ONE/PCON,AMCON,PSTEEL,FFC,PSHELL,AMSHEL ,AC,PSHT,LL(50)    PME04480
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME04490
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME04500
      COMMON/FOUR/KA,II,KK,PCAP,RCS,ROS,RT,RTS,DIF,ASH(100),IJ,PCT,NBARSPME04510
      COMMON/FIVE/P(10),AS(50),XS(50),SSN(50),SSS(50),AXS(50),BARFY,STR PME04520
      COMMON/SIX/X(100),CSN(100),CSS(100),ACC(100),ATOT(100),SSH(100)   PME04530
      COMMON/SEVEN/RSSH(100),XX(100),RXX(100),AM(50),XXS(100),RXXS(100) PME04540
      COMMON/EIGHT/TA(100),RTA(100),TACC(100),RACC(100),RX(100),ES,EC,IKPME04550
      COMMON/NINE/RASH(100),RATOT(100),RTOT(100),RTACC(100),TOT(100)    PME04560
      COMMON/TEN/NN,S(100),CST(100),CB(100),H(100),RCX(100)             PME04570
      COMMON/ELEVEN/CX(100),SN(100),CS(100),B(100),BS(100)              PME04580
      CDEL=DT/20.                                                       PME04590
      NN=INT(DNA/CDEL)                                                  PME04600
      CC=DNA-FLOAT(NN)*CDEL                                             PME04610
      CF=CDEL-CC                                                        PME04620
      IF(DNA-Z)10,10,20                                                 PME04630
  10  CX(1)=CC+.5*CDEL                                                  PME04640
      GOTO 600                                                          PME04650
  20  CX(1)=(DNA-Z)+.5*CDEL                                             PME04660
 600  DO 90 I=1,100                                                     PME04670
      IF(I.GT.1) CX(I)=CX(1)+(I-1)*CDEL                                 PME04680
      SN(I)=CX(I)*PHI                                                   PME04690
      CST(I)=ES*SN(I)                                                   PME04700
      IF(CST(I).GT.TUBEFY) CST(I)=TUBEFY                                PME04710
      IF(CST(I).LT.-TUBEFY) CST(I)=-TUBEFY                              PME04720
  90  CONTINUE                                                          PME04730
      PCT=0.0                                                           PME04740
      PCORE=0.0                                                         PME04750
      IF(DNA-Z) 1,1,5                                                   PME04760
  1   IF(DNA-DIF) 80,80,4                                               PME04770
  4   IK=INT((DNA-DIF)/CDEL)                                            PME04780
      GOTO 6                                                            PME04790
  5   IK=20                                                             PME04800
  6   DO 70 I=1,IK                                                      PME04810
      CB(I)=CX(I)+RO-DNA                                                PME04820
      CS(I)=CB(I)*CB(I)                                                 PME04830
      IF(RT-ABS(CB(I))) 15,15,25                                        PME04840
  15  TOT(I)=0.0                                                        PME04850
C                                                                       PME04860
C     COMPUTE THE TOTAL AREA TO TAKEN THE COMPRESSION FORCE             PME04870
C                                                                       PME04880
      GOTO 30                                                           PME04890
  25  TOT(I)=2.*SQRT(RTS-CS(I))*CDEL                                    PME04900
  30  IF (RC-ABS(CB(I))) 40,40,50                                       PME04910
  40  TACC(I)=0.0                                                       PME04920
      GOTO 60                                                           PME04930
  50  TACC(I)=2.*SQRT(RCS-CS(I))*CDEL                                   PME04940
  60  TA(I)=TOT(I)-TACC(I)                                              PME04950
  70  PCORE=PCORE+TA(I)*CST(I)                                          PME04960
      IF(DNA-Z) 80,200,200                                              PME04970
  80  KK=INT((Z-DNA)/CDEL)                                              PME04980
      IF(DNA-DIF) 95,100,100                                            PME04990
  95  RCX(1)=DIF-DNA+.5*CDEL                                            PME05000
      KK=20                                                             PME05010
      GOTO 96                                                           PME05020
  100 RCX(1)=CF+.5*CDEL                                                 PME05030
      KK=INT((Z-DNA)/CDEL)                                              PME05040
  96  DO 180 I=1,KK                                                     PME05050
      IF(I.GT.1) RCX(I)=RCX(1)+(I-1)*CDEL                               PME05060
      B(I)=RO-DNA-RCX(I)                                                PME05070
      BS(I)=B(I)*B(I)                                                   PME05080
      IF(RT-ABS(B(I))) 120,120,130                                      PME05090
  120 RTOT(I)=0.0                                                       PME05100
      GOTO 140                                                          PME05110
  130 RTOT(I)=2.*SQRT(RTS-BS(I))*CDEL                                   PME05120
C                                                                       PME05130
C     COMPUTE THE TOTAL AREA TO TAKE TENSION                            PME05140
C                                                                       PME05150
  140 IF(RC-ABS(B(I))) 150,150,160                                      PME05160
  150 RTACC(I)=0.0                                                      PME05170
      GOTO 170                                                          PME05180
  160 RTACC(I)=2.*SQRT(RCS-BS(I))*CDEL                                  PME05190
  170 RTA(I)=RTOT(I)-RTACC(I)                                           PME05200
      H(I)=CST(I)*RCX(I)/CX(I)                                          PME05210
      IF(H(I).GT.TUBEFY) H(I)=TUBEFY                                    PME05220
      IF(H(I).LT.-TUBEFY) H(I)=-TUBEFY                                  PME05230
  180 PCT=PCT+RTA(I)*H(I)                                               PME05240
      PCORE=PCORE-PCT                                                   PME05250
  200 RETURN                                                            PME05260
      END                                                               PME05270
      SUBROUTINE COREM                                                  PME05280
C                                                                       PME05290
C     THIS SUBROUTINE IS USED TO COMPUTE THE BENDING MOMENT TAKEN BY THEPME05300
C     CORE SHELL                                                        PME05310
      COMMON/TWO/PCORE,AMCORE,ISHAPE,WIDTH,OD,RO,RC,DC,DT,T,TT,Z,COVER  PME05320
      COMMON/THREE/NROWS,NP,FC,K,DEL,PHI,RINT,RINTS,DNA,C,F,G,TUBEFY    PME05330
      COMMON/FOUR/KA,II,KK,PCAP,RCS,ROS,RT,RTS,DIF,ASH(100),IJ,PCT,NBARSPME05340
      COMMON/EIGHT/TA(100),RTA(100),TACC(100),RACC(100),RX(100),ES,EC,IKPME05350
      COMMON/NINE/RASH(100),RATOT(100),RTOT(100),RTACC(100),TOT(100)    PME05360
      COMMON/TEN/NN,S(100),CST(100),CB(100),H(100),RCX(100)             PME05370
      COMMON/ELEVEN/CX(100),SN(100),CS(100),B(100),BS(100)              PME05380
      AMCORE=0.0                                                        PME05390
      AMCT=0.0                                                          PME05400
      IF(DNA-Z) 1,1,4                                                   PME05410
  1   IF (DNA-DIF) 6,6,4                                                PME05420
  4   DO 5 I=1,IK                                                       PME05430
  5   AMCORE=AMCORE+CST(I)*TA(I)*CB(I)                                  PME05440
      IF(DNA-Z) 6,9,9                                                   PME05450
  6   DO 7 I=1,KK                                                       PME05460
  7   AMCT=AMCT+H(I)*RTA(I)*(RCX(I)+DNA-RO)                             PME05470
      AMCORE=AMCORE+AMCT                                                PME05480
  9   RETURN                                                            PME05490
      END                                                               PME05500
C                                                                       .B.
C
C
C
C
C
C
      SUBROUTINE PARM
      CHARACTER*16 FLNM1, FLNM2
      INTEGER OUNIT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  STANDARD INTERACTIVE INPUT UNIT
      IIUNIT=1
C  STANDARD INTERACTIVE OUTPUT UNIT
      IOUNIT=3
C  STANDARD REPORT OUTPUT UNIT
      OUNIT=6
C  STANDARD FILE INPUT UNIT
      INUNIT=5
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     OPEN(IIUNIT,FILE='CON')                                           .T.
C     OPEN(IOUNIT,FILE='CON')                                           .T.
10    WRITE(*,210)                                                      .T.
210   FORMAT(//,
     1' Enter input file name:',/)                                      .T. 
           READ(*,ERR=10,FMT='(A)')FLNM1                                .T.
           IF(FLNM1.EQ.'CON')GO TO 25                                   .T.
           IF(FLNM1.EQ.'con')GO TO 25                                   .T.
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
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1      .T.
35    WRITE(*,235)                                                      .T.    
235   FORMAT(//,                   
     1' Enter output file name:',/)                                     .T.
           READ(*,ERR=35,FMT='(A)')FLNM2                                .T.
           IF(FLNM2.EQ.'CON')GO TO 60                                   .T. 
           IF(FLNM2.EQ.'con')GO TO 60                                   .T. 
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
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2    .T.
          OPEN(OUNIT,FILE=FLNM2)
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6
      RETURN
      END
