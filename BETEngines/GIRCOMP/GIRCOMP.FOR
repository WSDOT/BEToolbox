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


C**** COMPOSITE SECTION PROPERTIES***'GIRCOMP(X)'***         *P63076**  B    010
C                                                                          00020
C         THE PROGRAM WAS CONVERTED TO CRJE AUG. 1972                   GS800030
C         CONVERTED BY GERRY SHROPE                                     GS800040
C                                                                       GS800050
C     A - GARY CHRISTOPHER - TSO CONVERSION - REV. 07/31/73             GCA00060
C     B - MARK ANDERSON - PC CONVERSION - REV. 10/01/86                 B
C     T - TERRY BEEBE   - VMS CONVERSION - JANUARY 1990                 T
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C     Robert S. Lotze - Recompiled/Reconfigured for Win32 dll conversion 12/99

C Note from Robert: All lines saying "Goto 666" were added for Win32 dll conversion.
C  These lines were originally "STOP" lines, but since "STOP" does not do a clean
C  shut-down of files, I changed all these to go to lines that closed the files, then
C  exited gracefully from the program with an "End".  


C The following 8 lines added by Robert for Win32 dll conversion:
C Robert's setup for a DLL call:
C Fortran Code  establishing subroutine
	SUBROUTINE GirComp (MYINPUT, MYOUTPUT)

C Specify that the routine name is to be made available to callers of the
C DLL and that the external name should not have any prefix or suffix
!DEC$ ATTRIBUTES STDCALL,DLLEXPORT :: GirComp
!DEC$ ATTRIBUTES ALIAS:'GirComp' :: GirComp
C
C
C The following 6 lines added by Robert for Win32 dll conversion:
CInput argument	
	CHARACTER*254,INTENT(IN) :: MYINPUT
CInput argument
	CHARACTER*254,INTENT(IN) :: MYOUTPUT
!DEC$ ATTRIBUTES REFERENCE :: MYINPUT
!DEC$ ATTRIBUTES REFERENCE :: MYOUTPUT
C                                     
C
      CHARACTER*4 IMODE                                                 B
      CHARACTER*1 CARCNT,CRCNT                                          B
      INTEGER*4 OUNIT, ERUNIT                                           B
      CHARACTER*8 REVDAT, TIME                                          T
      CHARACTER*11 DATE                                                 T
      CHARACTER*23 DATIME                                               T 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      B
C     OUNIT = (6 FOR PC OUTPUT)                                         B
C             (9 FOR MF OUTPUT)                                         B
C     ERUNIT = (3 FOR PC)                                               B
C              (9 FOR MAINFRAME BATCH)                                  B
C     CARCNT = ('1' FOR BATCH)                                          B
C     IMODE = (FREE)                                                    B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      B
C                                                                       B
C                                                                       B
      OUNIT = 6                                                         B
      ERUNIT = 3                                                        B
      IMODE = 'FREE'                                                    B
      CARCNT = '1'                                                      B
      CRCNT = ' '                                                       B
      INUNIT = 5                                                        B

C	Last two parameters added by Robert for Win32 dll conversion:
      CALL PARM(IMODE,OUNIT,ERUNIT,CARCNT,MYINPUT,MYOUTPUT)
C      CALL LIB$DATE_TIME (DATIME)                                      T
	CALL DATE_AND_TIME (DATIME)
      TIME = DATIME(13:20)                                              T
      DATE = DATIME(1:11)                                               T

C=======================================================================
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  B
      REVDAT = '01/30/90'                                               T
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  B
C     CARCNT - CARRIAGE CONTROL CHARACTER                               B  01440
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION   ',   T
     1  A11'  TIME ',A8)                                                T
 1001 FORMAT('        *** COMPOSITE GIRDER PROPERTIES (GIRCOMP) ***     B
     1' ,/,8X,'REV ',A8)                                                B
 1002 FORMAT(/,'        ************************************************B
     1************************',//)                                     B
C                                                                       B
C                                                                       B
      XNNNN=0.                                                             00280
    6 READ(INUNIT,ERR=199,END=350,FMT=*)NF                              B
      BACKSPACE INUNIT                                                  B
      IF (NF.EQ.1) THEN                                                 B
           READ(INUNIT,ERR=199,FMT=*)NF,IDEN,AR,DEP,XI                  B
           GOTO 6                                                       B
      ENDIF                                                             B
      IF (NF.EQ.2) THEN                                                 B
           READ(INUNIT,ERR=199,FMT=*)NF,NO,D,TW,WTP,TTP,WBT,            B
     1          TBT,WSL,TSL,G,XN,XNN,XNNN                               B
      ELSE                                                              B
           GO TO 199                                                    B
      ENDIF                                                             B
      NX=1                                                              B
   16 IF (AR)28,29,28                                                   COM00820
C                                                                          00830
C        COMPUTE STEEL AREA                                             COM00840
C                                                                          00850
   29 AS=D*TW+WTP*TTP+WBT*TBT                                           COM00860
C                                                                          00870
C        COMPUTE CG GIR SEC (MOMS ABOUT BOT)                            COM00880
C                                                                          00890
      YB=(D*TW*(D/2.+TBT)+WTP*TTP*(D+TBT+TTP/2.)+WBT*TBT*TBT/2.)/AS     COM00900
C                                                                          00910
C        COMPUTE INERTIA (GIR SEC ONLY)                                 COM00920
C                                                                          00930
      XIO=(D**3*TW+TTP**3*WTP+TBT**3*WBT)/12.                           COM00940
      XIG=XIO+(D*TW*(D/2.+TBT-YB)**2)+(WTP*TTP*(D+TBT+TTP/2.-YB)**2)    COM00950
      XIG=XIG+(WBT*TBT*(TBT/2.-YB)**2)                                  COM00960
C                                                                          00970
C        COMPUTE SECTION MODULI (GIR SEC ONLY)                          COM00980
C                                                                          00990
      YT=D+TTP+TBT-YB                                                   COM01000
      SBOT=XIG/YB                                                       COM01010
      STP=XIG/YT                                                        COM01020
C                                                                          01030
C        COMPUTE Q VALUES (GIR SEC)                                     COM01040
C                                                                          01050
      QTP=WTP*TTP*(YT-TTP/2.)                                           COM01060
      QBT=WBT*TBT*(YB-TBT/2.)                                           COM01070
   65 IF (AR)28,66,28                                                   COM01080
C                                                                          01090
C        COMPUTE CG COMP SEC                                            COM01100
C                                                                          01110
   66 ASLAB=(WSL*TSL/XN)                                                COM01120
      APAD=(WTP*G/XN)                                                   COM01130
      ACOM=ASLAB+APAD+AS                                                COM01140
      YBC=(ASLAB*(TSL/2.+G+TTP+D+TBT)+APAD*(G/2.+D+TTP+TBT)+AS*YB)/ACOM COM01150
      IF (TTP+D+TBT-YBC)21,9,9                                          COM01160
    7 FORMAT (//,'          COMPOSITE GIRDER PROPERTIES  N=',I5//,T10,' B  01170
     1   NEUTRAL AXIS LIES IN SLAB')                                    B  01180
    8 WRITE (OUNIT,7)LXN                                                RFF01190
      GO TO 46                                                          COM01200
C                                                                          01210
C        COMPUTE INERTIA COMP SEC                                       COM01220
C                                                                          01230
    9 XIOC=XIG+(ASLAB*TSL**2+APAD*G**2)/12.+AS*(YBC-YB)**2              COM01240
      XIOC=XIOC+ASLAB*(TSL/2.+G+TTP+D+TBT-YBC)**2                       COM01250
      XIOC=XIOC+APAD*(G/2.+TTP+D+TBT-YBC)**2                            COM01260
C                                                                          01270
C        COMPUTE SECTION MODULI (COMP SEC)                              COM01280
C                                                                          01290
      YTC=TTP+D+TBT-YBC                                                 COM01300
      SBC=XIOC/YBC                                                      COM01310
      STC=XIOC/YTC                                                      COM01320
C                                                                          01330
C        COMPUTE Q VALUES (COMP SEC.)                                   COM01340
C                                                                          01350
      QSL=ASLAB*(TSL/2.+G+YTC)+APAD*(G/2.+YTC)                          COM01360
      QSLC=QSL+WTP*TTP*(YTC-TTP/2.)                                     COM01370
      QBC=WBT*TBT*(YBC-TBT/2.)                                          COM01380
   21 GO TO (12,24),NX                                                  COM01400
   26 FORMAT(A1,T9,'IDENT',I7,' PROB. NO.',I5,///,T11,'BUILT-UP BEAM DIMB  01740
     1ENSIONS'//,T12,' WEB',F7.1,' BY',F7.3,'   TOP FLANGE',F5.0,' BY',FB  01750
     27.3,//,T12,' BOT FLANGE',F5.0,' BY',F7.3,'  SLAB',F6.0,' BY',F5.1,B  01760
     3' G=',F5.1)                                                       B  01770
   12 WRITE(OUNIT,1000)CARCNT,DATE,TIME                                 B
      WRITE(OUNIT,1001)REVDAT                                           B
      WRITE(OUNIT,1002)                                                 B
      WRITE(OUNIT,26)CRCNT,IDEN,NO,D,TW,WTP,TTP,WBT,TBT,WSL,TSL,G       B  01470
      WRITE(OUNIT,25)XIG,SBOT,STP,YB,YT,QBT,QTP,AS                      B
   25 FORMAT(//,T9,'  GIRDER PROPERTIES'//,T12,' I GIRDER=',F10.0,' S BOB  01550
     1T=',F8.0,' S TOP=',F8.0,//,T12,' Y BOT=',F7.2,' Y TOP=',F7.2//,T13B  01560
     2,'Q BOT FLANGE=',F9.1,' Q TOP FLANGE=',F9.1,' STEEL AREA=',F7.1)  B  01570
   24 LXN=XN                                                               01580
      IF (TTP+D+TBT-YBC)8,841,841                                       RFF01590
  841 WRITE(OUNIT,499)LXN,XIOC,SBC,STC,YBC,YTC,QBC,QSL,QSLC             GS801670
  499 FORMAT(//,T10,' COMPOSITE GIRDER PROPERTIES  N=',I5//,T13,'I COMP=B  01680
     1',F10.0,' S BOT=',F8.0,' S TOP=',F8.0,//,T13,'Y BOT=',F7.2,' Y TOPB  01690
     2=',F7.2//,T12,' Q BOT FLANGE=',F9.1,' Q SLAB=',F9.1,' Q SLAB & TOPB  01700
     3 FLANGE=',F9.1)                                                   B  01710
      GO TO 46                                                          GS801720
   34 GO TO (850,24),NX                                                 RFF01730
  850 WRITE(OUNIT,1000)CARCNT,DATE,TIME                                 B
      WRITE(OUNIT,1001)REVDAT                                           B
      WRITE(OUNIT,1002)                                                 B
      WRITE(OUNIT,26)CRCNT,IDEN,NO,D,TW,WTP,TTP,WBT,TBT,WSL,TSL,G       B  01800
      K=2                                                               COM01810
   27 FORMAT(//,T12,' I GIRDER=',F9.0,' S TOP=',F8.0,' S BOT=',F8.0,' STB  01820
     1EEL AREA=',F7.1)                                                  B  01830
      WRITE(OUNIT,27)XIG,STP,SBOT,AS                                    RFF01840
      GO TO 24                                                          RFF01850
C                                                                          01860
C        COMPUTE ROLLED SECTION PROPERTIES                              COM01870
C                                                                          01880
   28 ASLAB=(WSL*TSL/XN)                                                COM01890
      APAD=(WTP*G/XN)                                                   COM01900
      ACOM=ASLAB+APAD+AR                                                COM01910
      YBR=(AR*DEP/2.+ASLAB*(TSL/2.+G+DEP)+APAD*(G/2.+DEP))/ACOM         COM01920
      IF (DEP-YBR)810,30,30                                             RFF01930
   35 WRITE (OUNIT,7)NO                                                 COM01940
      GO TO 46                                                          COM01950
   30 XIR=XI+(ASLAB*TSL**2+APAD*G**2)/12.+AR*(DEP/2.-YBR)**2            COM01960
      XIR=XIR+ASLAB*(TSL/2.+G+DEP-YBR)**2+APAD*(G/2.+DEP-YBR)**2        COM01970
      YTR=DEP-YBR                                                       COM01980
      SBR=XIR/YBR                                                       COM01990
      STR=XIR/YTR                                                       COM02000
      QSLAB=ASLAB*(TSL/2.+G+YTR)+APAD*(G/2.+YTR)                        COM02010
  810 GO TO (860,800),NX                                                RFF02020
   31 FORMAT(A1,T8,' IDENT ',I7,' PROB. NO.',I5,///,T10,' ROLLED BEAM SEB  02030
     1CTION PROPERTIES'//,T12,' AREA=',F6.2,'   DEPTH=',F6.2,'   I=',F8.B  02040
     20)                                                                B  02050
  860 WRITE(OUNIT,1000)CARCNT,DATE,TIME                                 B
      WRITE(OUNIT,1001)REVDAT                                           B
      WRITE(OUNIT,1002)                                                 B
      WRITE (OUNIT,31)CRCNT,IDEN,NO,AR,DEP,XI                           B  02080
  800 LXN=XN                                                            RFF02090
      IF (DEP-YBR)8,870,870                                             GS802100
   32 FORMAT(//,T10,' COMPOSITE GIRDER PROPERTIES  N=',I5,//,T12,' I =',B  02110
     1F8.0,8H S BOT =,F7.1,8H S TOP =,F7.1)                             B  02120
  870 WRITE (OUNIT,32)LXN,XIR,SBR,STR                                   GS802130
   33 FORMAT(//,T12,' Y TOP =',F6.2,8H Y BOT =,F6.2,9H Q SLAB =,F7.1)   B  02140
      WRITE (OUNIT,33)YTR,YBR,QSLAB                                     COM02150
      GO TO 46                                                          COM02160
   46 IXN=XN                                                            RT002170
      IXNN=XNN                                                          RT002180
      IXNNN=XNNN                                                        RT002190
      IXNNNN=XNNNN                                                      RT002200
      IF(IXNN) 47,6,47                                                  B   2210
   47 XN=XNN                                                            COM02220
      XNN=XNNN                                                          COM02230
      XNNN=XNNNN                                                        COM02240
      NX=2                                                              COM02250
      GO TO 65                                                          COM02260
  830 FORMAT(//,T10,' COMPOSITE GIRDER PROPERTIES  N=',I5//,T12,' I COMPB  02270
     1 =',F10.0,' S TOP =',F7.0,' S BOT =',F7.0)                        B  02280
  880 WRITE(OUNIT,830)LXN,XIOC,STC,SBC                                  GS802290
      GO TO 46                                                          RFF02300
  199 WRITE(ERUNIT,299)                                                 GCA02350
  299 FORMAT(//,' ERRORS IN DATA HAVE TERMINATED THIS JOB',//)             02360
  350 GOTO 666                                                             
C	The following two lines added by Robert for Win32 dll conversion:
666	CLOSE(OUNIT)
	CLOSE(INUNIT)
      END                                                                  02390
C                                                                       B
C                                                                       B
C                                                                       B
C                                                                       B
C                                                                       B
C                                                                       B
C	Last two parameters added by Robert for Win32 dll conversion:
      SUBROUTINE PARM(IMODE,OUNIT,ERUNIT,CARCNT,MYINFILE,MYOUTFILE)
C	Size changed from *16 to *254 by Robert for Win32 dll conversion:
      CHARACTER*254 FLNM1, FLNM2                                         
      CHARACTER*1 CARCNT                                                B
      CHARACTER*4 IMODE                                                 B
      INTEGER OUNIT, ERUNIT                                             B
C	This line added by Robert for Win32 dll conversion:
	CHARACTER*254 MYINFILE,MYOUTFILE

C	This block commented out by Robert for Win32 dll conversion:
C------------------------------------------------------------------------
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   B
C  STANDARD REPORT OUTPUT UNIT                                          B
      OUNIT=6                                                           B
C  STANDARD FILE INPUT UNIT                                             B
      INUNIT=5                                                          B
C  STANDARD ERROR OUTPUT UNIT                                           B
      ERUNIT=3                                                          B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   B
C10    WRITE(*,210)                                                      T
C210   FORMAT(//,                                                        B
C     1' Enter input file name:',/)                                      B
C           READ(*,ERR=10,FMT='(A)')FLNM1                                B
C           IF(FLNM1.EQ.'CON')GO TO 25                                   T
C           IF(FLNM1.EQ.'con')GO TO 25                                   T
C              DO 15 I=1,16                                              B
C               IF(FLNM1(I:I).EQ.'.')GO TO 25                            B
C15             CONTINUE                                                 B
C              DO 20 I=1,16                                              B
C                 IF(FLNM1(I:I).EQ.' ')THEN                              B
C                  IE = I+3                                              B
C                  FLNM1(I:IE) = '.dat'                                  B
C                  GO TO 25                                              B
C                 END IF                                                 B
C20             CONTINUE                                                 B
C25         WRITE(*,FMT='(/,'' '',1X,A16,'' used for input '',/)')FLNM1  T
C          CARCNT='1'                                                    B
          IMODE = 'FREE'                                                 B
C35    WRITE(*,235)                                                      B
C235   FORMAT(//,                                                        B
C     1' Enter output file name:',/)                                     B
C           READ(*,ERR=35,FMT='(A)')FLNM2                                B
C           IF(FLNM2.EQ.'CON')GO TO 60                                   T
C           IF(FLNM2.EQ.'con')GO TO 60                                   T
C              DO 40 I=1,16                                              B
C               IF(FLNM2(I:I).EQ.'.')GO TO 60                            B
C40             CONTINUE                                                 B
C              DO 50 I=1,16                                              B
C                 IF(FLNM2(I:I).EQ.' ')THEN                              B
C                  IE = I+3                                              B
C                  FLNM2(I:IE) = '.out'                                  B
C                  GO TO 60                                              B
C                 END IF                                                 B
C50             CONTINUE                                                 B
C60        WRITE(*,FMT='(/,'' '',1X,A16,'' used for output '',/ )')FLNM2 T
C------------------------------------------------------------------------
CThese two lines added by Robert for Win32 dll conversion:
	FLNM1 = MYINFILE
	FLNM2 = MYOUTFILE
          OPEN(OUNIT,FILE=FLNM2)                                        B
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                          B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                      B
      RETURN                                                            B
      END                                                               B