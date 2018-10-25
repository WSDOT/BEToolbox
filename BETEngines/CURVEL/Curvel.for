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

                                                                        00000100
C                                                                       00000200
C         CURVEL - BRIDGE ELEVATIONS PROGRAM                            00000300
C                                                                       00000400
C         - NEAL CAMPBELL -- PC CONVERSION 3/28/86                      00000500
C                                                                       00000600
C     T   - TERRY BEEBE  - VMS CONVERSION JANUARY 1990                     T
C     A   - S.G.R 10/28/91     FIXED BUG IN INTER MODE                     A
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C     Robert S. Lotze - Recompiled/Reconfigured for Win32 dll conversion 9-11/99

C Note from Robert: All sections that end with:
C	"CLOSE(OUNIT)
C	 CLOSE(INUNIT)
C      END"  
C  were originally "STOP / END" lines.  But 'stop' is a very ungraceful way
C  to end the program in that it doesn't close files which are then needed by the
C  GUI.  So I now explicitely close the files and end the program with an "END".

C The following 9 lines added by Robert for Win32 dll conversion:
C Robert's setup for a DLL call:
C Fortran Code  establishing subroutine
	SUBROUTINE Curvel (MIN, MOUT)

C Specify that the routine name is to be made available to callers of the
C DLL and that the external name should not have any prefix or suffix
!DEC$ ATTRIBUTES STDCALL,DLLEXPORT :: Curvel
C!DEC$ ATTRIBUTES DLLIMPORT :: Curvel
!DEC$ ATTRIBUTES ALIAS:'Curvel' :: Curvel
C
      IMPLICIT REAL*8 (A-H,O-Z)                                         00000700
C The following 6 lines added by Robert for Win32 dll conversion:
CInput argument	
	CHARACTER*254,INTENT(IN) :: MIN
CInput argument
	CHARACTER*254,INTENT(IN) :: MOUT
!DEC$ ATTRIBUTES REFERENCE :: MIN
!DEC$ ATTRIBUTES REFERENCE :: MOUT

      DIMENSION OF(2,3),IST(5)                                          00000800
      REAL*8     M3,M2,J9                                               00000900
      INTEGER*4 Q9,F5,H5,IMODE                                          00001000
      CHARACTER*8 REVDAT, TIME                                          0T001100
      CHARACTER*11 DATE                                                  T
      CHARACTER*23 DATIME                                                T
      CHARACTER*1 ANS                                                   00001200
      CHARACTER*1 CARCNT                                                00001300
      INTEGER*4 OUNIT, ERUNIT                                           00001400
      COMMON /DATA/  B7,B8,B9,OF,OF1                                    00001500
      COMMON /SUPERX/  B5,D9,E1,Y1,G8,G9                                00001600
      COMMON /PARMIN/ IOUNIT, INUNIT, OUNIT, ERUNIT, IMODE              00001700
      COMMON /SKEWX/J9,Z4,F5,K                                          00001800
      COMMON /REV/ REVDAT, TIME, DATE                                   00001900


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC      00002000
      REVDAT = ' 3/28/86'                                               00002100
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC      00002200
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      00002300
C     OUNIT = (6 FOR FILE OUTPUT)                                       00002400
C             (3 FOR FILE OUTPUT)                                       0T002500
C             (6 FOR SCREEN OUTPUT (INTERACTIVE MAINFRAME))             00002600
C     ERUNIT = (3 FOR FILE OUTPUT)                                      0T002700
C              (6 FOR MAINFRAME)                                        00002800
C     INUNIT = (5 FOR FILE INPUT)                                       00002900
C              (1 FOR INTERACTIVE ON PC)                                00003000
C              (5 FOR INTERACTIVE ON MAINFRAME)                         00003100
C     IOUNIT = (3 FOR PC)                                               00003200
C              (6 FOR MAINFRAME)                                        00003300
C     IMODE  = (0 FOR BATCH)                                            00003400
C              (1 FOR INTERACTIVE)                                      00003500
C     CARCNT = ('1' FOR BATCH)                                          00003600
C              (' ' FOR INTERACTIVE)                                    00003700
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      00003800
C                                                                       00003900

C  STANDARD INTERACTIVE INPUT UNIT                                      00049400
C      IIUNIT=5                                                          00049500
C  STANDARD INTERACTIVE OUTPUT UNIT                                     00049600
C     IOUNIT=1                                                          00049700
C  STANDARD REPORT OUTPUT UNIT                                          00049800
C      OUNIT=3                                                           00049900
C  STANDARD FILE INPUT UNIT                                             00050000
C      INUNIT=5                                                          00050100
C  STANDARD ERROR OUTPUT UNIT                                           00050200
C      ERUNIT=3                                                          00050300
CCCCCCCCCCCCCCCC

C	The last two parameters added by Robert for Win32 dll conversion:
      CALL PARM(IMODE,OUNIT,ERUNIT,CARCNT,INUNIT,IOUNIT,MIN,MOUT)
C      CALL LIB$DATE_TIME (DATIME)   
	CALL DATE_AND_TIME (DATIME)                                        
      TIME = DATIME(13:20)                                               T
      DATE = DATIME(1:11)                                                T    
C                                                                       00004400
C                                                                       00004500
C                                                                       00004600
      ANS='N'                                                           00004700
      Q9=0                                                              00004800
      D9=0.0                                                            00004900
      C3=0.0                                                            00005000
      M1=0                                                              00005100
      K=1                                                               00005200
   10 IF (IMODE.EQ.1) GO TO 602                                         00005300
      READ(INUNIT,END=998,ERR=998,FMT=*)S1, S2, A2, A3, A4              00005400
      READ(INUNIT,END=998,ERR=998,FMT=*)Q9                              00005500
C                                                                       00005600
   15 IF(Q9.EQ.0)GO TO 60                                               00005700
      READ(INUNIT,END=998,ERR=998,FMT=*)OF1                             00005800
C                                                                       00005900
      READ(INUNIT,END=998,ERR=998,FMT=*)B7, B8, B9                      00006000
      READ(INUNIT,END=998,ERR=998,FMT=*)OF(1,1), OF(2,1), OF(1,2)       00006100
      READ(INUNIT,END=998,ERR=998,FMT=*)OF(2,2), OF(1,3), OF(2,3)       00006200
      CALL DATAIN                                                       00006300
      GO TO 68                                                          00006400
C                                                                       00006500
60    WRITE (OUNIT,64)  CARCNT,DATE,TIME                                00006600
64    FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION   ',    T
     1 A11,'  TIME ',A8)                                                0T006800
      WRITE (OUNIT,65)REVDAT                                            00006900
65    FORMAT('        *** CURVEL (VERTICAL CURVE CALCULATIONS) ***      00007000
     1' ,/,8X,'REV ',A8,///)                                            00007100
C                                                                       00007200
   68 M3=S1                                                             00007300
      M2=S2                                                             00007400
      S1=S1/100.0                                                       00007500
      S2=S2/100.0                                                       00007600
      A5=A4+A3/2                                                        00007700
      CALL CONVT(A4,IST)                                                00007800
C                                                                       00007900
      A4=ROUNDS(A4)                                                     00008000
      WRITE(OUNIT,70)                                                   00008100
   70 FORMAT(//,' ',24X,'VERTICAL CURVE DATA')                          00008200
      WRITE(OUNIT,80) IST,A2                                            00008300
   80 FORMAT('   P.I. AT STA. ',I4,'+',2I1,'.',2I1,8X,'P.I. ELEV. = ',F900008400
     1.3)                                                               00008500
      WRITE(OUNIT,90) A3,M3,M2                                          00008600
   90 FORMAT('   LENGTH =',F9.2,' FT.',4X,'G1 =',F9.5,' %',4X,'G2 =',F9.00008700
     15,' %'//)                                                         00008800
C                                                                       00008900
      A9=A2-A3/2*S1                                                     00009000
      B1=S1                                                             00009100
      Y2=A2+A3/2*S2                                                     00009200
      IF(A3.EQ.0.0) GO TO 100                                           00009300
      GO TO 110                                                         00009400
C                                                                       00009500
  100 B2=0.0                                                            00009600
      GO TO 120                                                         00009700
  110 B2=(Y2-B1*A3-A9)/(A3**2)                                          00009800
  120 S1=S1*100.0                                                       00009900
      S2=S2*100.0                                                       00010000
C                                                                       00010100
      WRITE(OUNIT,130)                                                  00010200
  130 FORMAT(' ',15X,'PROFILE',14X,'OFFSET',15X,'CROWN   CROWN')        00010300
      WRITE(OUNIT,140)                                                  00010400
  140 FORMAT(' ',15X,'GRADE',16X,'RT. OF',15X,'SLOPE   SLOPE')          00010500
      WRITE(OUNIT,150)                                                  00010600
  150 FORMAT('  STATION',7X,'ELEVATION    GRADE   CROWN    ELEVATION   L00010700
     *EFT    RIGHT'/)                                                   00010800
C                                                                       00010900
      B3=A4-A3/2                                                        00011000
      B5=B3                                                             00011100
      GO TO 500                                                         00011200
  160 Y1=B2*B5*B5+B1*B5+A9                                              00011300
      C7=Y1                                                             00011400
      B6=(2*B2*B5+B1)*100.0                                             00011500
      B5=B5+A4-A3/2                                                     00011600
  190 CALL CONVT(B5,IST)                                                00011700
      IF (Q9.EQ.1 .OR. ANS.EQ.'Y')GO TO 200                             00011800
      WRITE(OUNIT,170) IST,C7,B6                                        00011900
      GO TO 210                                                         00012000
  200 E1=Y1                                                             00012100
      B5=Z1                                                             00012200
      CALL SUPER                                                        00012300
C                                                                       00012400
  165 WRITE(OUNIT,170) IST,C7,B6,D9,Y1,G8,G9                            00012500
  170 FORMAT(' ',I4,'+',2I1,'.',2I1,4X,F9.3,2X,F8.4,1X,F9.4,1X,F9.3,2X, 00012600
     1F7.4,2X,F7.4/)                                                    00012700
  210 IF(M1.EQ.0) GO TO 300                                             00012800
      B5=C4                                                             00012900
      IF((B5+C3/2).LT.C6)GO TO 220                                      00013000
      M1=0                                                              00013100
      GO TO 210                                                         00013200
C                                                                       00013300
  220 IF(K.EQ.1)GO TO 230                                               00013400
      B5=Z4                                                             00013500
  230 B5=B5+C3                                                          00013600
      Z1=B5                                                             00013700
      C4=B5                                                             00013800
      GO TO 350                                                         00013900
  300 IF(Q9.EQ.0.AND.ANS.NE.'Y')GO TO 320                               00014000
C                                                                       00014100
      IF (IMODE.EQ.1) GO TO 650                                         00014200
      READ (INUNIT,FMT=*,END=2000,ERR=998)B5, S9                        00014300
  305 D9=S9                                                             00014400
      IF(B5.EQ.992)  GO TO 310                                          00014500
      GO TO 350                                                         00014600
  310 F5=0                                                              00014700
      GO TO 350                                                         00014800
C                                                                       00014900
  320 IF (IMODE.EQ.1) GO TO 660                                         00015000
      READ(INUNIT,FMT=*,END=2000,ERR=998)B5                             00015100
      GO TO 360                                                         00015200
  350 Z2=D9                                                             00015300
  360 IF(B5.EQ.0)  GO TO 2000                                           00015400
      IF(B5.EQ.1)  GO TO 380                                            00015500
      J9=B5                                                             00015600
  370 IF(B5.EQ.1)  GO TO 380                                            00015700
      GO TO 385                                                         00015800
  380 B5=J9                                                             00015900
  385 IF(B5.EQ.999.)GO TO 390                                           00016000
      IF(B5.EQ.991.)GO TO 450                                           00016100
      IF(B5.EQ.992.)GO TO 430                                           00016200
      IF(B5.EQ.993.)GO TO 420                                           00016300
      GO TO 495                                                         00016400
  390 K=1                                                               00016500
      S1=D9                                                             00016600
      D9=0.0                                                            00016700
      READ(INUNIT,FMT=*,END=998,ERR=998)S2, A2, A3, A4, Q9              00016800
      GO TO 15                                                          00016900
  420 F5=1                                                              00017000
  430 K=1                                                               00017100
      N5=B5                                                             00017200
      CALL  SKEW(0)                                                     00017300
      H5=0                                                              00017400
      GO TO 490                                                         00017500
  450 IF(Q9.EQ.1 .OR. ANS.EQ.'Y') GO TO 460                             00017600
      IF (IMODE.EQ.1) GO TO 670                                         00017700
      BACKSPACE INUNIT                                                  00017800
      READ(INUNIT,FMT=*,END=998,ERR=998)DUMY, C1, C6, C8, DD9           00017900
  455 D9=DD9                                                            00018000
C                                                                       00018100
      GO TO  470                                                        00018200
  460 IF (IMODE.EQ.1) GO TO 670                                         00018300
      BACKSPACE INUNIT                                                  00018400
      READ(INUNIT,FMT=*,END=998,ERR=998)DUMY, C1, C6, C8, DD9           00018500
  465 D9=DD9                                                            00018600
  470 M1=1                                                              00018700
C                                                                       00018800
      B5=C1                                                             00018900
      C3=(C6-C1)/C8                                                     00019000
  490 IF(H5.EQ.0) GO TO 499                                             00019100
  495 IF(K-2) 500,496,496                                               00019200
  496 CALL SKEW(1)                                                      00019300
  499 H5=1                                                              00019400
  500 Z1=B5                                                             00019500
      C4=B5                                                             00019600
      IF((B5-B3).GT.0.0) GO TO 540                                      00019700
      Y1=A9+(B5-B3)*S1/100.000                                          00019800
      B6=S1                                                             00019900
      C7=Y1                                                             00020000
      GO TO 190                                                         00020100
C                                                                       00020200
  540 IF((B5-A5).GT.0.0) GO TO 550                                      00020300
      B5=B5-B3                                                          00020400
      GO TO 160                                                         00020500
  550 Y1=Y2+(B5-A5)*S2/100.000                                          00020600
      C7=Y1                                                             00020700
      B6=S2                                                             00020800
      GO TO 190                                                         00020900
C                                                                       00021000
C     TSO CONVERSATIONAL ROUTINES (PROMPT MESSAGES)                     00021100
C	Note from Robert: Don't quite know which lines of this are called
C	  from other lines (through GOTO's) (if any), so instead of commenting
C	  out this entire section, I'm just going to comment out all "WRITE(*"
C	  lines.
C                                                                       00021200
C  602 WRITE(*,604)                                                      0T021300
  602 WRITE(*,604)
  604 FORMAT(' ',' ENTER INTERSECTION GRADIENTS,G1 AND G2, IN PERCENT'  0T021400
     1,/)
C      READ(*,FMT=*,ERR=602)S1,S2                                        00021500
C  606 WRITE(*,608)                                                      0T021600
C  608 FORMAT(' ',' ENTER P.I. ELEV. IN FT., CURVE LENGTH IN FT.',/)     0T021700
C      READ(*,FMT=*,ERR=606)A2,A3                                        00021800
C  610 WRITE(*,612)                                                      0T021900
C  612 FORMAT(' ',' ENTER INPUT STATION OF P.I., IN FEET',/)             0T022000
C      READ(*,FMT=*,ERR=610)A4                                           00022100
C  614 WRITE(*,616)                                                      0T022200
C  616 FORMAT(' ',' DO YOU WISH TO CORRECT FOR SUPERELEV.?(YES OR NO)',/)0T022300
C      READ(*,FMT='(A)',ERR=614) ANS                                     A 022400
C      IF (ANS.EQ.'y') ANS = 'Y'                                         00022500
C      IF (ANS.EQ.'n') ANS = 'N'                                         00022600
C      IF (ANS.EQ.'N') GO TO 646                                         00022700
C      IF (ANS.EQ.'Y') GO TO 618                                         00022800
C     GO TO 614                                                         00022900
C  618 WRITE(*,620)                                                      0T023000
C  620 FORMAT(' ',' ENTER HORIZ. DIST. RT. FROM CROWN TO PROFILE GRADE   0T023100
C     1 PT.(FT)',/)                                                      0T023200
C      READ(*,FMT=*,ERR=618)OF1                                          00023300
C  622 WRITE(*,624)                                                      0T023400
C  624 FORMAT(' ',' TYPE SUPERELEV. BREAK POINT STAS. 1, 2, AND 3 (FT)',  T
C     1 /)                                                                T 
C      READ(*,FMT=*,ERR=622)B7, B8, B9                                   00023600
C  626 WRITE(*,628)                                                      0T023700
C  628 FORMAT(' ',' ENTER SLOPES LEFT OF CROWN PT. AT STAS. 1, 2, AND 3 (0T023800
C     1FT/FT)',/)                                                        0T023900
C      READ(*,FMT=*,ERR=626)OF(1,1), OF(2,1), OF(1,2)                    00024000
C  640 WRITE(*,645)                                                      0T024100
C  645 FORMAT(' ',' ENTER SLOPES RIGHT OF CROWN PT. AT STAS. 1,2 AND 3 (F0T024200
C     1T/FT)',/)                                                         0T024300
C      READ(*,FMT=*,ERR=640)OF(2,2), OF(1,3), OF(2,3)                    00024400
  646 CALL DATAIN                                                       00024500
      GO TO 68                                                          00024600
C                                                                       00024700
C  650 WRITE(*,655)                                                      0T024800
  650 WRITE(*,655)
  655 FORMAT(' ',' ENTER STA. AND OFFSET RIGHT (FT)',/)                 0T024900
C      READ(*,FMT=*,ERR=650)B5,S9                                        00025000
C      IF(B5.EQ.999.)GO TO 2000                                          00025100
C      GO TO 305                                                         00025200
C                                                                       00025300
C  660 WRITE(*,665)                                                      0T025400
  660 WRITE(*,665)
  665 FORMAT(' ',' ENTER STA. (FT.)',/)                                 0T025500
C      READ(*,FMT=*,ERR=660)B5                                           00025600
C      IF(B5.EQ.999.)GO TO 2000                                          00025700
C      GO TO 360                                                         00025800
C                                                                       00025900
C  670 WRITE(*,675)                                                      0T026000
  670 WRITE(ERUNIT,*)
  675 FORMAT(' ',' ENTER BEGINNING STA., ENDING STA., NO. OF SPACES, AND0T026100
     1'/,' OFFSET DIST. RT. OF C. L. (ALL IN FT.)',/)                   0T026200
C      READ(*,FMT=*,ERR=670)C1,C6,C8,DD9                                 00026300
C      GO TO 465                                                         00026400
C                                                                       00026500
C     END OF CONVERSATIONAL ROUTINES                                    00026600
C                                                                       00026700
C 998  WRITE(*,*)'   INPUT ERROR!!!  CHECK YOUR INPUT FILE!!'            0T026800
998   WRITE(ERUNIT,*)'   INPUT ERROR!!!  CHECK YOUR INPUT FILE!!'            
 
C     The following two lines added by Robert for Win32 dll conversion.
 2000	CLOSE(OUNIT)
	CLOSE(INUNIT)
C	CLOSE(ERUNIT)
C	CLOSE(IOUNIT)
      END                                                               00027000
C
C
C
C
      DOUBLE PRECISION FUNCTION   ROUNDS(STA)                           00027100
C                                                                       00027200
      REAL*8 STA                                                        00027300
C                                                                       00027400
      ROUNDS=IDINT(STA*100.0D0+0.5D0)/100.0D0                           00027500
C                                                                       00027600
      RETURN                                                            00027700
      END                                                               00027800
C
C
C
C
      SUBROUTINE  CONVT(STA,IST)                                        00027900
C                                                                       00028000
C     THIS SUBROUTINE CONVERTS STA TO INTEGER ARRAY READY FOR PRINT.    00028100
C                                                                       00028200
      INTEGER*4 IST(5)                                                  00028300
      REAL*8 STA,ST                                                     00028400
C                                                                       00028500
   10 STA =STA + 0.00500                                                00028600
      IST(1)=STA/100.                                                   00028700
      ST=DABS(STA)                                                      00028800
      J=IST(1)*10                                                       00028900
      J= IABS(J)                                                        00029000
      IST(2)=ST/10.-J                                                   00029100
C                                                                       00029200
      J=(J+IST(2))*10                                                   00029300
      IST(3)= ST - J                                                    00029400
C                                                                       00029500
      J=(J+IST(3))*10                                                   00029600
      IST(4)=ST *10.-J                                                  00029700
C                                                                       00029800
      J=(J+IST(4))*10                                                   00029900
      IST(5) =ST * 100. -J                                              00030000
C                                                                       00030100
      STA= STA-.005                                                     00030200
      RETURN                                                            00030300
      END                                                               00030400
C
C
C
C
      SUBROUTINE DATAIN                                                 00030500
C                                                                       00030600
C     THIS SUBROUTINE PRINTS SUPERELEVATION DATA.                       00030700
C                                                                       00030800
      IMPLICIT REAL*8 (A-H,O-Z)                                         00030900
      DIMENSION OF(2,3)                                                 00031000
      INTEGER*4 OUNIT,IMODE,ERUNIT                                      00031100
      CHARACTER*8 REVDAT, TIME                                          0T031200
      CHARACTER*11 DATE                                                  T
      CHARACTER*23 DATIME                                                T
      COMMON /DATA/  B7,B8,B9,OF,OF1                                    00031300
      COMMON /PARMIN/ IOUNIT, INUNIT, OUNIT, ERUNIT, IMODE              00031400
      COMMON /REV/ REVDAT, TIME, DATE                                   0T031500
C                                                                       00031600
      WRITE (OUNIT,64)  CARCNT,DATE,TIME                                00031700
64    FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION  '     0T031800
     1 A11,'  TIME ',A8)                                                0T031900
      WRITE (OUNIT,65)REVDAT                                            00032000
65    FORMAT('        *** CURVEL (VERTICAL CURVE CALCULATIONS) ***      00032100
     1' ,/,8X,'REV ',A8,///)                                            00032200
      WRITE(OUNIT,15)                                                   00032300
   15 FORMAT(' ',25X,'SUPERELEVATION DATA',/,' ',27X,'1',14X,'2',14X,'3'00032400
     1)                                                                 00032500
      WRITE(OUNIT,20) B7,B8,B9                                          00032600
   20 FORMAT('   STATION',11X,F10.3,5X,F10.3,5X,F10.3)                  00032700
      WRITE(OUNIT,30) OF(1,1),OF(2,1),OF(1,2)                           00032800
   30 FORMAT('   SLOPE LEFT',10X,F8.3,7X,F8.3,7X,F8.3)                  00032900
      WRITE(OUNIT,40) OF(2,2),OF(1,3),OF(2,3)                           00033000
   40 FORMAT('   SLOPE RIGHT',9X,F8.3,7X,F8.3,7X,F8.3)                  00033100
      WRITE(OUNIT,50) OF1                                               00033200
   50 FORMAT('   THE PROFILE GRADE POINT IS ',F10.4,' FT. RIGHT OF CROWN00033300
     1POINT')                                                           00033400
      RETURN                                                            00033500
      END                                                               00033600
C
C
C
C
      SUBROUTINE SUPER                                                  00033700
C                                                                       00033800
      IMPLICIT REAL*8 (A-H,O-Z)                                         00033900
      DIMENSION OF(2,3)                                                 00034000
      COMMON /SUPERX/ B5,D9,E1,Y1,G8,G9                                 00034100
      COMMON /DATA/  B7,B8,B9,OF,OF1                                    00034200
C                                                                       00034300
C                                                                       00034400
   10 IF(B5.LT.B7) GO TO 100                                            00034500
      IF(B5.GE.B9) GO TO 110                                            00034600
      IF(B5.GT.B8) GO TO 80                                             00034700
      G9=((OF(1,3)-OF(2,2))/(B8-B7))*(B5-B7)                            00034800
      G9=G9+OF(2,2)                                                     00034900
      G8=((OF(2,1)-OF(1,1))/(B8-B7))*(B5-B7)                            00035000
      G8=G8+OF(1,1)                                                     00035100
      GO TO 120                                                         00035200
C                                                                       00035300
   80 G9=((OF(2,3)-OF(1,3))/(B9-B8))*(B5-B8)                            00035400
      G9=G9+OF(1,3)                                                     00035500
      G8=((OF(1,2)-OF(2,1))/(B9-B8))*(B5-B8)                            00035600
      G8=G8+OF(2,1)                                                     00035700
      GO TO 120                                                         00035800
C                                                                       00035900
  100 G9=OF(2,2)                                                        00036000
      G8=OF(1,1)                                                        00036100
      GO TO 120                                                         00036200
C                                                                       00036300
  110 G9=OF(2,3)                                                        00036400
      G8=OF(1,2)                                                        00036500
C                                                                       00036600
  120 IF(OF1.LT.0.0) GO TO 130                                          00036700
      OF2=(-OF1)*G9                                                     00036800
      GO TO 140                                                         00036900
  130 OF2=OF1*G8                                                        00037000
  140 IF(D9.LT.0.0) GO TO 150                                           00037100
      C9=E1+OF2+G9*D9                                                   00037200
      GO TO 160                                                         00037300
C                                                                       00037400
  150 C9=E1+OF2-G8*D9                                                   00037500
  160 Y1=C9                                                             00037600
      RETURN                                                            00037700
      END                                                               00037800
C
C
C
C
      SUBROUTINE SKEW(NM)                                               00037900
C                                                                       00038000
C     THIS ROUTINE CORRECTS STATION FOR SKEW ANGLE.                     00038100
C                                                                       00038200
      IMPLICIT REAL*8 (A-H,O-Z)                                         00038300
      DIMENSION OF(2,3),IST(5)                                          00038400
      INTEGER*4 N5,D1,D2,D3,F5                                          00038500
      INTEGER*4 OUNIT,IMODE,ERUNIT                                      00038600
      REAL*8 J9,OF3                                                     00038700
      COMMON /SKEWX/J9,Z4,F5,K                                          00038800
      COMMON /SUPERX/ B5,D9,E1,Y1,G8,G9                                 00038900
      COMMON /DATA/  B7,B8,B9,OF,OF1                                    00039000
      COMMON /PARMIN/ IOUNIT, INUNIT, OUNIT, ERUNIT, IMODE              00039100
      NR=5                                                              00039200
      IF(NM.EQ.1) GO TO 65                                              00039300
      IF(K.EQ.2)GO TO 66                                                00039400
      B5=D9                                                             00039500
      J9=B5                                                             00039600
      IF (IMODE.EQ.1) GO TO 680                                         00039700
      BACKSPACE INUNIT                                                  00039800
      READ(INUNIT,ERR=998,END=998,FMT=*)DUMY,DUMY, S9, D1, D2,          00039900
     1                                  D3, R, OF3                      00040000
   05 D9=S9                                                             00040100
      WRITE(OUNIT,10)                                                   00040200
   10 FORMAT(/,' SKEW DATA FOR STATIONS FOLLOWING: ')                   00040300
      WRITE(OUNIT,20) D1,D2,D3,R,OF3                                    00040400
   20 FORMAT(' SKEW ANG.= ',I4,1X,I2,1X,I2,' RAD. TO L = ',F9.2,        00040500
     1' OFFSET, CROWN TO L = ',F6.2)                                    00040600
      IF(D1+D2+D3.EQ.0)GO TO 100                                        00040700
      D=IABS(D1)+IABS(D2)/60.0+IABS(D3)/3600.0                          00040800
      IF(D1.LT.0) GO TO 50                                              00040900
      IF(D2.LT.0) GO TO 50                                              00041000
      IF(D3.LT.0) GO TO 50                                              00041100
      GO TO 60                                                          00041200
C                                                                       00041300
C     TSO CONVERSATIONAL ROUTINES (PROMPT MESSAGES)                     00041400
C                                                                       00041500
C	Note from Robert: Commenting this out since it is definitely all
C	  related to the console:
C  680 WRITE(*,685)                                                      0T041600
  680 WRITE(*,685)
  685 FORMAT(' ',' ENTER STATION(FT), OFFSET(FT), SKEW ANGLE(DEG.,MIN.,S0T041700
     *EC.),',/,' ','RADIUS TO L LINE(FT), OFFSET DISTANCE CROWN TO L(FT)0T041800
     *',/)                                                              0T041900
      J9 = B5                                                           00042000
C      READ (INUNIT,FMT=*,ERR=680)B5, S9, D1, D2, D3, R, OF3             00042100
C      GO TO 05                                                          00042200
C                                                                       00042300
C     END OF CONVERSATIONAL ROUTINES                                    00042400
C                                                                       00042500
   50 D=-D                                                              00042600
   60 D=D*1.745329251994329577D-02                                      00042700
C                                                                       00042800
      V3=D                                                              00042900
   65 D=DABS(D)                                                         00043000
   66 IF(F5.EQ.1) GO TO 70                                              00043100
      GO TO 120                                                         00043200
   70 V2=D9                                                             00043300
      IF(R.EQ.0.0) GO TO 90                                             00043400
      IF(DABS(R-V2).LE.DABS(R)) GO TO 80                                00043500
      D=3.141592653589793238D0 -D                                       00043600
   80 RO=DSQRT(R*R+V2*V2-2*DABS(V2)*DABS(R)*DCOS(D))*R/DABS(R)          00043700
      D9=(OF3-RO+R)                                                     00043800
      GO TO 120                                                         00043900
   90 D9=(OF3+V2*DCOS(D))                                               00044000
      GO TO 200                                                         00044100
  100 D=0.0D0                                                           00044200
      C=0.0                                                             00044300
      V2=(OF3-D9)*(-1)                                                  00044400
      GO TO 210                                                         00044500
  120 IF(R .EQ.0.0) GO TO 195                                           00044600
      RO=R-D9+OF3                                                       00044700
      IF(F5.EQ.1) GO TO 160                                             00044800
      AO= DASIN(R*DSIN(D)/RO)                                           00044900
      IF(DABS(RO).GT.DABS(R)) GO TO 140                                 00045000
      PO=AO-D                                                           00045100
      GO TO 150                                                         00045200
  140 PO=D-AO                                                           00045300
  150 V2=DSIN(PO)*DABS(RO)/DSIN(D)                                      00045400
      IF(RO.GT.R) GO TO 155                                             00045500
      GO TO 160                                                         00045600
  155 V2=V2*(-1)                                                        00045700
  160 IF(V2.EQ.0) GO TO 170                                             00045800
C                                                                       00045900
      PO= DASIN(DABS(V2)*DSIN(D)/DABS(RO))*V2/DABS(V2)*(-1)*V3/DABS(V3) 00046000
      GO TO 180                                                         00046100
  170 PO=0                                                              00046200
  180 C= DABS(R)*PO                                                     00046300
      GO TO 210                                                         00046400
C                                                                       00046500
  195 V2=((OF3-D9)/DCOS(D))*(-1)                                        00046600
  200 C=V2*DSIN(D)*V3/DABS(V3)*(-1)                                     00046700
  210 CALL CONVT(B5,IST)                                                00046800
      WRITE(OUNIT,220) IST,V2                                           00046900
  220 FORMAT(' ',I4,'+',2I1,'.',2I1,' IS STA. ON L LINE, SKEW DIST.=',F800047000
     1.3,' ,OFFSET PT. STA. IS')                                        00047100
      Z4=B5                                                             00047200
      B5=B5+C                                                           00047300
      IF(D.EQ.0.0D0) GO TO 250                                          00047400
      D=V3                                                              00047500
      K=2                                                               00047600
      GO TO 260                                                         00047700
  250 K=1                                                               00047800
  260 RETURN                                                            00047900
  998 WRITE(ERUNIT,*)'   INPUT ERROR!!!  CHECK YOUR INPUT FILE!!!'           
C  998 WRITE(*,*)'   INPUT ERROR!!!  CHECK YOUR INPUT FILE!!!'           0T048000

C	The following two lines added by Robert for Win32 dll conversion.
 	CLOSE(OUNIT)
	CLOSE(INUNIT)
C	CLOSE(ERUNIT)
C	CLOSE(IOUNIT)
	END
C                                                                       00048300
C                                                                       00048400
C                                                                       00048500
C                                                                       00048600
C                                                                       00048700
C     The last two parameters added by Robert for Win32 dll conversion:
      SUBROUTINE PARM(IMODE,OUNIT,ERUNIT,CARCNT,INUNIT,IOUNIT,SIN,SOUT) 
      CHARACTER*254 FLNM1, FLNM2                                        00049000
      CHARACTER*1 CARCNT, QUES                                          00049100

C	Changed by Robert from INTEGER to INTEGER*4:
      INTEGER*4 OUNIT, ERUNIT
	                                       
C	The following two variables added by Robert for Win32 dll conversion:
	CHARACTER*254 SIN,SOUT

C	The following two declarations added by Robert to make sure that
C	  these are declared and no garbage is passed to a function as 
C	  a result of not declaring them.
C	---When these are declared, the program will not work.  Go figure.
C	REAL*4 IMODE
C	REAL*8 INUNIT,IOUNIT

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00049300
C  STANDARD INTERACTIVE INPUT UNIT                                      00049400
C      IIUNIT=1                                                          00049500
      IIUNIT=5                                                          00049500
C  STANDARD INTERACTIVE OUTPUT UNIT                                     00049600
      IOUNIT=1                                                          00049700
C  STANDARD REPORT OUTPUT UNIT                                          00049800
      OUNIT=3                                                           00049900
C  STANDARD FILE INPUT UNIT                                             00050000
      INUNIT=5                                                          00050100
C  STANDARD ERROR OUTPUT UNIT                                           00050200
      ERUNIT=3                                                          00050300
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00050400
      IKEY=0                                                            00050500
C     OPEN(IIUNIT,FILE='CON')                                           0T050600
C     OPEN(IOUNIT,FILE='CON')                                           0T050700

C	The following line added by Robert for win32 dll conversion:
	IMODE = 0
	INUNIT=5

C The following section commented out by Robert for win32 dll conversion
C===============================================================================

C ASK IF ITERACTIVE FOR INPUT OR NOT                                    00050800
C      IMODE=1                                                           00050900
C     CARCNT=' '                                                        00051000
C      INUNIT=1                                                          00051100
C      OUNIT=3                                                           00051200
C
C5     WRITE(*,FMT='(//,'' Do you want to run interactive? (y/n):'',/)')  T
C      READ (*,ERR=5,FMT='(A)')QUES                                      0T051600
C      IF(QUES.EQ.'n') QUES='N'                                          00051700
C          IF(QUES.EQ.'N') THEN                                          00051800
C           IMODE = 0                                                    00051900
C           INUNIT=5                                                     00052000
C10    WRITE(*,FMT='(//,'' Enter input file name:'',/)')                  T
C           READ(*,ERR=10,FMT='(A)')FLNM1                                0T052400
C           IF(FLNM1.EQ.'CON')GO TO 25                                   0T052500
C           IF(FLNM1.EQ.'con')GO TO 25                                   0T052600
C              DO 15 I=1,16                                              00052700
C               IF(FLNM1(I:I).EQ.'.')GO TO 25                            00052800
C15             CONTINUE                                                 00052900
C              DO 20 I=1,16                                              00053000
C                 IF(FLNM1(I:I).EQ.' ')THEN                              00053100
C                  IE = I+3                                              00053200
C                  FLNM1(I:IE) = '.dat'                                  00053300
C                  GO TO 25                                              00053400
C                 END IF                                                 00053500
C20             CONTINUE                                                 00053600
C25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1      0T053700
C           IKEY=1                                                       00053800
C           END IF                                                       00053900
CC  ASK IF INTERACTIVE FOR OUTPUT OR NOT                                 00054000
C30    WRITE(*,FMT='(//,'' Do you want a print out? (yes/no)'',/)')       T
C      READ(*,ERR=30,FMT='(A)')QUES                                      0T054400
C      IF(QUES.EQ.'y') QUES='Y'                                          00054500
C         IF(QUES.EQ.'Y') THEN                                           00054600
C          OUNIT=6                                                       00054700
C          CARCNT='1'                                                    00054800
C35    WRITE(*,FMT='(//,'' Enter output file name:'',/)')                 T
C           READ(*,ERR=35,FMT='(A)')FLNM2                                0T055200
C           IF(FLNM2.EQ.'CON')GO TO 60                                   0T055300
C           IF(FLNM2.EQ.'con')GO TO 60                                   0T055400
C              DO 40 I=1,16                                              00055500
C               IF(FLNM2(I:I).EQ.'.')GO TO 60                            00055600
C40             CONTINUE                                                 00055700
C              DO 50 I=1,16                                              00055800
C                 IF(FLNM2(I:I).EQ.' ')THEN                              00055900
C                  IE = I+3                                              00056000
C                  FLNM2(I:IE) = '.out'                                  00056100
C                  GO TO 60                                              00056200
C                 END IF                                                 00056300
C50             CONTINUE                                                 00056400
C60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2    0T056500
C          OPEN(OUNIT,FILE=FLNM2)                                        00056600
C          END IF                                                        00056700
C          IF(IKEY.EQ.1)OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')             00056800
C      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                      00056900
C      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                      00057000
C      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                      00057100
C================================================================================
C
C	The following four lines by Robert for Win32 dll conversion:
	FLNM1 = SIN
	FLNM2 = SOUT
          OPEN(OUNIT,FILE=FLNM2)                                         
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD') 
      RETURN                                                            00057200
      END                                                               00057300
C                                                                       00057400
C                                                                       00057500
C                                                                       00057600
