C  Note: "MAXLIN" is not used.  Can be deleted.


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


C     BOX GIRDER SECTION PROPERTIES                                     BOX00010
C                                                                          00020
C     A - GARY CHRISTOPHER - TSO CONVERSION - REV. 07/23/73               A00030
C     B - NEAL CAMPBELL - PC CONVERSION - REV. 4/14/86                    B
C     T - TERRY BEEBE   - VMS CONVERSION - JANUARY 1990                   T
C         RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C		Robert Lotze - Recompiled to Win32 dll November 1999

C	Note from Robert: All lines stating "GOTO 666" were added by me.  These were
C	  all originally "STOP" commands.  These did not work for the dll conversion, 
C	  so I've got them all going to commands which close the i/o files and then exit
C	  the program ("STOP" does not close the files).

C Robert's setup for a DLL call:
C The following 12 lines are for dll conversion:
C Fortran Code  establishing subroutine
	SUBROUTINE BoxGir (INPUTFILE, OUTPUTFILE)
C Specify that the routine name is to be made available to callers of the
C DLL and that the external name should not have any prefix or suffix
!DEC$ ATTRIBUTES STDCALL,DLLEXPORT :: BoxGir
!DEC$ ATTRIBUTES ALIAS:'BoxGir' :: BoxGir
CInput argument	
	CHARACTER*254,INTENT(IN) :: INPUTFILE
CInput argument
	CHARACTER*254,INTENT(IN) :: OUTPUTFILE
!DEC$ ATTRIBUTES REFERENCE :: INPUTFILE
!DEC$ ATTRIBUTES REFERENCE :: OUTPUTFILE

C                                                                          00040
      INTEGER*4 ERLIST                                                    B
      LOGICAL*1 CCHAR                                                     A00100
      CHARACTER*1 CARCNT                                                  B
      INTEGER*4 OUNIT, ERUNIT                                             B
      CHARACTER*8 REVDAT, TIME                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T
      CHARACTER*4 IMODE                                                   B

C	The following 2 lines are for Win32 dll conversion:
	CHARACTER*255 INPUT
	CHARACTER*255 OUTPUT

      IMODE='FREE'                                                        B
      NW=9                                                                B
      ERLIST=9                                                            B
      INUNIT=5                                                            B
      CARCNT='1'                                                          B
      CCHAR=.TRUE.                                                        B
      MAXLIN=66                                                           B
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
      REVDAT = ' 4/14/86'                                                 B
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C     IMODE = 'FREE'  NOT USED ANYMORE                                    B
C     OUNIT = (6 FOR FILE OUTPUT)                                         T
C             (9 FOR MAIN FRAME)                                          B
C     ERUNIT = (3 FOR PC)                                                 B
C              (9 FOR MAINFRAME BATCH)                                    B
C     CCHAR = .TRUE. NOT USED ANYMORE                                     B
C     MAXLIN = 66 NOT USED ANYMORE                                        B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C                                                                         B
C                                                                         B
C      CALL LIB$DATE_TIME (DATIME)                                         T
C      TIME = DATIME(13:20)                                                T
C      DATE = DATIME(1:11)                                                 T

C	ENTRY INPUT OUTPUT

C	Modified for Win32 environment:
	CALL DATE_AND_TIME(DATE,TIME)                                                                         B
C                    
C	The following line was modified for Win32 dll conversion...the last two
C	  parameters were added by me (Robert).
      CALL LOAD(IMODE,OUNIT,ERUNIT,CCHAR,MAXLIN,INPUTFILE,OUTPUTFILE)
      ERLIST = ERUNIT                                                     A00310
      NW = OUNIT                                                          A00320
C                                                                         B
C  IF PC VERSION, SET PRINTER TO COMPRESSED. NOTE COMMAND AT END OF       B
C  PROGRAM TO RESET PRINTER TO NORMAL OPERATIONS.                         B
C  ACTUAL CHARACTERS NOT IN MAIN FRAME VERSION                            B
C                                                                         B
C     IF(ERUNIT.EQ.3)WRITE(OUNIT,'('' '')')                             B
CC                                                                       B
      READ(INUNIT,END=591,ERR=591,FMT=*)IDENT,J,NPROB                     B
  601 IF (J-1)2,3,2                                                     BOX00450
    2 WRITE(ERLIST,998)                                                    00460
  600 GOTO 666                                                                
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION    ',    T
     1  A11,'  TIME ',A8)                                                 T
 1001 FORMAT('        *** BOX GIRDER SECTION PROPERTIES ***  REV ',A8)    B
3      WRITE(OUNIT,1000)CARCNT,DATE,TIME                                  B
       WRITE(OUNIT,1001)REVDAT                                            B
      WRITE(NW,996)IDENT                                                BOX00500
C                                                                         A00510
C                                                                          00540
  100 WRITE(NW,995)                                                     BOX00650
      WRITE(NW,993)                                                     BOX00660
  150 N=0                                                               BOX00680
 4    READ(INUNIT,END=591,ERR=591,FMT=*)                                  B
     1            NPRB,D,T,TN,W,ST,SB,FT,FB,EL,CL,BL,ER,CR,BR              00730
C                                                                       GS600740
   40 IF (TN-1.)42,41,42                                                BOX00750
   41 TN=2                                                              BOX00760
      T=T*.5                                                            BOX00770
C     COMPUTE AREA IN SQ FT                                             BOX00780
   42 AST=(ST/12.)*W                                                    BOX00790
      ASB=(SB/12.)*W                                                    BOX00800
      AT=TN*(T/12.)*(D-(ST/12.+SB/12.))                                 BOX00810
      AFT=(TN-1.)*(FT/12.)**2.                                          BOX00820
      AFB=(TN-1.)*(FB/12.)**2.                                          BOX00830
      ACL=CL*EL/12.                                                     BOX00840
      ABL=0.5*EL*(BL-(CL/12.))                                          BOX00850
      ACR=CR*ER/12.                                                     BOX00860
      ABR=0.5*ER*(BR-(CR/12.))                                          BOX00870
      ATOT=AST+ASB+AT+AFT+AFB+ACL+ABL+ACR+ABR                           BOX00880
C     Y FROM BOTTOM OF BOX IN FT                                        BOX00890
      YST=D-ST/24.                                                      BOX00900
      YSB=SB/24.                                                        BOX00910
      YT=((D-(ST/12.+SB/12.))/2.)+SB/12.                                BOX00920
      YFT=D-ST/12.-(FT/36.)                                             BOX00930
      YFB=(SB+(FB/3.))/12.                                              BOX00940
      YCL=D-CL/24.                                                      BOX00950
      YBL=D-CL/12.-((BL-CL/12.)/3.)                                     BOX00960
      YCR=D-CR/24.                                                      BOX00970
      YBR=D-CR/12.-((BR-CR/12.)/3.)                                     BOX00980
C     COMPUTE Q IN FT 3RD                                               BOX00990
      QST=AST*YST                                                       BOX01000
      QSB=ASB*YSB                                                       BOX01010
      QT=AT*YT                                                          BOX01020
      QFT=AFT*YFT                                                       BOX01030
      QFB=AFB*YFB                                                       BOX01040
      QCL=ACL*YCL                                                       BOX01050
      QBL=ABL*YBL                                                       BOX01060
      QCR=ACR*YCR                                                       BOX01070
      QBR=ABR*YBR                                                       BOX01080
      QTOT=QST+QSB+QT+QFT+QFB+QCL+QBL+QCR+QBR                           BOX01090
C     I ABOUT BOTTOM OF BOX IN FT 4TH                                   BOX01100
      XIST=QST*YST                                                      BOX01110
      XISB=QSB*YSB                                                      BOX01120
      XIT=QT*YT                                                         BOX01130
      XIFT=QFT*YFT                                                      BOX01140
      XIFB=QFB*YFB                                                      BOX01150
      XICL=QCL*YCL                                                      BOX01160
      XIBL=QBL*YBL                                                      BOX01170
      XICR=QCR*YCR                                                      BOX01180
      XIBR=QBR*YBR                                                      BOX01190
      XXIST=(W*((ST/12.)**3.))/12.                                      BOX01200
      XXISB=(W*((SB/12.)**3.))/12.                                      BOX01210
      XXIT=((T/12.)*TN*((D-(ST/12.+SB/12.))**3.))/12.                   BOX01220
      XXIFT=(2.*(TN-1.)*((FT/12.)**4.))/36.                             BOX01230
      XXIFB=(2.*(TN-1.)*((FB/12.)**4.))/36.                             BOX01240
      XXICL=(EL*((CL/12.)**3.))/12.                                     BOX01250
      XXIBL=(EL*((BL-CL/12.)**3.))/36.                                  BOX01260
      XXICR=(ER*((CR/12.)**3.))/12.                                     BOX01270
      XXIBR=(ER*((BR-CR/12.)**3.))/36.                                  BOX01280
      XITOT=XIST+XISB+XIT+XIFT+XIFB+XICL+XIBL+XICR+XIBR+XXIST+XXISB+XXITBOX01290
     1+XXIFT+XXIFB+XXICL+XXIBL+XXICR+XXIBR                              BOX01300
      YCGB=QTOT/ATOT                                                    BOX01310
      YCGT=D-YCGB                                                       BOX01320
      XXI=XITOT-(QTOT*YCGB)                                             BOX01330
      WT=ATOT*0.160                                                     BOX01340
      ZT=XXI/YCGT                                                       BBA01350
      ZB=XXI/YCGB                                                       BBA01360
      IN=TN                                                             BOX01370
      WRITE(NW,992)NPRB,D,T,IN,W,ST,SB,EL,CL,ER,CR,XXI,YCGT,YCGB,ATOT,WTBBA01460
     1,ZT,ZB                                                            BBA01470
  450 N=N+1                                                             BOX01490
      IF (NPROB-N)6,6,4                                                 BOX01500
    6 WRITE(NW,994)IDENT                                                BOX01510
C                                                                         B
C  IF PC VERSION, RESET PRINTER TO NORMAL. NOTE COMMAND AT BEG. OF        B
C  PROGRAM TO SET PRINTER TO COMPRESSED.                                  B
C  ACTUAL CHARACTERS NOT IN MAIN FRAME VERSION                            B
C                                                                         B
C     IF(ERUNIT.EQ.3)WRITE(OUNIT,'('' '')')                             B
C                                                                         B
C                                                                       GS600370
      GOTO 666
  998 FORMAT(/,' INCORRECT HEADER CARD, 1 IN COLUMN 5')                    01590
  996 FORMAT(' ','       IDENT NO. ',I4,//)                               B01610
  995 FORMAT('        PROB DPTH   T   NO  WIDTH  ST   SB   EL   CL   ER   B01620
     1  CR     I     YT   YB   AREA     W/FT     ZT      ZB')             B01630
  994 FORMAT('        END IDENT NO. ',I4)                                 B01640
  993 FORMAT('        NO   (FT)  (IN)     (FT)  (IN) (IN) (FT) (IN) (FT)  B01650
     1 (IN) (FT**4) (FT) (FT) (FT**2)   (K/F)  (FT**3) (FT**3)',/)        B01660
  992 FORMAT(' ',5X,I4,F7.2,F6.2,I3,2F6.2,5F5.2,F8.2,2F5.2,4F8.2,/)       B01670
C  591 WRITE(*,593)                                                        A01690
  591 WRITE(OUNIT,593)                                                      
  593 FORMAT(/,'  ERRONEOUS DATA HAS BEEN ENTERED',//)                    A01700

C	The following three lines added by Robert for Win32 dll conversion:
  666	CLOSE(OUNIT)
	CLOSE(INUNIT)
      END                                                                  
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C     The last two parameters of this subroutine added by Robert for Win32 dll conversion:
      SUBROUTINE LOAD(IMODE,OUNIT,ERUNIT,CCHAR,MAXLIN,INFILE,OUTFILE)
	
C	The following two variables were originally set to "*16"
      CHARACTER*254 FLNM1, FLNM2                                           B
      CHARACTER*4 IMODE                                                   B
      LOGICAL*1 CCHAR                                                     B
      INTEGER OUNIT, ERUNIT  
C	The following two variables added by Robert for Win32 dll Conversion
      CHARACTER*254 INFILE,OUTFILE
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
      MAXLIN=66                                                           B
      IMODE='FREE'                                                        B
      CCHAR=.TRUE.                                                        T

C	The following section commented out by Robert for Win32 dll conversion:
C============================================================================
C     OPEN(IIUNIT,FILE='CON')                                             T
C     OPEN(IOUNIT,FILE='CON')                                             B
C10    WRITE(*,FMT='(//,'' Enter input file name:'',/)')                   T
CC           READ(*,ERR=10,FMT='(A)')FLNM1                                  T
C          IF(FLNM1.EQ.'CON')GO TO 25                                     T
C           IF(FLNM1.EQ.'con')GO TO 25                                     T
C              DO 15 I=1,16                                                B
C               IF(FLNM1(I:I).EQ.'.')GO TO 25                              B
C15             CONTINUE                                                   B
C              DO 20 I=1,16                                                B
C                 IF(FLNM1(I:I).EQ.' ')THEN                                B
C                  IE = I+3                                                B
C                  FLNM1(I:IE) = '.dat'                                    B
C                  GO TO 25                                                B
C                 END IF                                                   B
C20             CONTINUE                                                   B
C25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1        T
C35    WRITE(*,FMT='(//,'' Enter output file name:'',/)')                  T
C           READ(*,ERR=35,FMT='(A)')FLNM2                                  T
C           IF(FLNM2.EQ.'CON')GO TO 60                                     T
C           IF(FLNM2.EQ.'con')GO TO 60                                     T
C              DO 40 I=1,16                                                B
C               IF(FLNM2(I:I).EQ.'.')GO TO 60                              B
C40             CONTINUE                                                   B
C              DO 50 I=1,16                                                B
C                 IF(FLNM2(I:I).EQ.' ')THEN                                B
C                  IE = I+3                                                B
C                  FLNM2(I:IE) = '.out'                                    B
C                  GO TO 60                                                B
C                 END IF                                                   B
C50             CONTINUE                                                   B
C60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2      T
C============================================================================
C	The following two setting of varables added by Robert for Win32 dll conversion
	FLNM1 = INFILE
	FLNM2 = OUTFILE
          OPEN(OUNIT,FILE=FLNM2)                                          B
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                              B
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                        B
      RETURN                                                              B
      END                                                                 B
C                                                                         B
C                                                                         B
