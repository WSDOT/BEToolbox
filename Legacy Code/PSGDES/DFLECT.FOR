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


      SUBROUTINE DFLECT
C             PRESTRESSED GIRDER DEFLECTION PROGRAM--THIS PROGRAM WAS
C             WRITTEN IN JULY 1966 IN FORTRAN BY T.L. MCLAIN UNDER
C             THE SUPERVISION OF W.B. HORN OF THE WASHINGTON STATE
C             HIGHWAY DEPARTMENT BRIDGE DIVISION.  IT WAS CONVERTED
C             IN 1968 FROM FORTRAN II TO 360 FORTRAN.  REVISIONS WERE
C             MADE IN JULY 1969 BY RANDY RAHN.
C
C             CONVERTED TO PC AND UPDATED BY NEAL CAMPBELL IN AUG 86
C
C     REVISION LOG:
C
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/27/93
C
      INTEGER*4 OUNIT,ERUNIT,TUNIT
      CHARACTER*1 CARCNT
      CHARACTER*8 TIME,REVDAT                                           .C.
      CHARACTER*11 DATE                                                 .C. 
      COMMON /PARMIN/ IRS,IDEF,IPICK,OUNIT,ERUNIT,INUNIT,TUNIT,K2
      COMMON /REV/ TIME,REVDAT,CARCNT,DATE                              .C.
C
C
C     FREE FORMAT READ ROUTINES
C
      GO TO 300
C PREVENT RERUN WHEN PGM EXECUTED FROM PREDES PGM
 3300 IF(IRS.NE.3)GO TO 420
  300 READ(TUNIT,END=420,ERR=8200,FMT=*)LNCODE
      BACKSPACE TUNIT
      IF (LNCODE.EQ.3) GO TO 320
      IF (LNCODE.EQ.4) GO TO 340
      GO TO 400
  310 READ(TUNIT,END=8000,ERR=8200,FMT=*)LNCODE
      BACKSPACE TUNIT
      IF (LNCODE.EQ.5) GO TO 360
      GO TO 400
  320 READ(TUNIT,END=400,ERR=400,FMT=*)LNCODE,WG,YBG,
     1                                 AG,ZTG,XIG,YTG
      GO TO 300
  340 READ(TUNIT,END=400,ERR=400,FMT=*)LNCODE,NOPBM,KSER,XL,AS,USS,E,
     1FCL,RISE,XNS,XNH,CSI,YBC,GTS,S,SBXML,WS,WC,T1,FCUG,KSTY
      GO TO 310
  360 READ(TUNIT,END=400,ERR=400,FMT=*)LNCODE,XHARP,FCUS,PHI
      GO TO 22
C
C
  400 WRITE(ERUNIT,405)
  405 FORMAT(//,'  DATA IS ERRONEOUS, IN THE WRONG ORDER, OR MISSING',
     1 '--TERMINATING RUN',//)
  420 RETURN
C
C     END OF FREE READ ROUTINES
C     SET UP DEFAULTS
C
22    IF (AS .EQ. 0) AS=0.153
      IF (USS .EQ. 0) USS=270.0
      IF (FCUS.EQ. 0) FCUS=6.0                                          .C.
      IF (FCUG.EQ. 0) FCUG=8.4                                          .C.
      IF (T1 .EQ. 0) T1=120.0                                           .C.
      IF (PHI .EQ. 0) PHI=2.0
      NONC = 0
      CSTR=.70
      IF (KSTY .EQ. 0) CSTR=.75
C
C RETRIEVE GIRDER PROPERTIES
C
      IF(KSER.EQ.2.OR.KSER.EQ.3)GO TO 122                               .C.
      IF(KSER.EQ.42)GO TO 126                                           .C.
      IF(KSER.EQ.50)GO TO 128                                           .C.
      IF(KSER.EQ.58)GO TO 121                                           .C.
      IF(KSER.EQ.73)GO TO 123                                           .C.
      IF(KSER.EQ.53.AND.S.LT.4.08.OR.KSER.EQ.35.AND.S.LT.4.08)THEN      .C.
          WRITE (OUNIT,9908) KSER                                       .C.
          WRITE (ERUNIT,9908) KSER                                      .C.
          STOP                                                          .C.
      END IF                                                            .C.
      IF(KSER.EQ.35)GO TO 1340                                          .C.
      IF(KSER.EQ.53)GO TO 1520                                          .C.
      WRITE (OUNIT,9904)                                                .C.
      WRITE (ERUNIT,9904)                                               .C.
      GO TO 400                                                         .C.
C                       GIRDER TYPE W35DG                               .C.
 1340 NONC = 1                                                          .C.
      BTF = S * 12.                                                     .C.
      IF ( BTF .GT. 78. ) BTF = 78.                                     .C.
      WG = .758 + ( S - 4.083 ) * 0.08                                  .C.
      ADSLAB = ( BTF - 49. ) * 6.                                       .C.
      SLABI = ( BTF - 49. ) * 18.                                       .C.
      YB = ( 682.4 * 21.23 + ADSLAB * 32. ) / ( ADSLAB + 682.4 )        .C.
      AG = 682.4 + ADSLAB                                               .C.
      ADY2G = 682.4 * ( ABS ( YB - 21.23 ) ) ** 2.                      .C.
      ADY2S = ADSLAB * ( ABS ( YB - 32. ) ) ** 2.                       .C.
      ZB = ( 101659.3 + SLABI + ADY2G + ADY2S ) / YB                    .C.
      YT = 35. - YB                                                     .C.
      ZT = ZB * YB / YT                                                 .C.
      GI = ZT * YT                                                      .C.
      GO TO 23                                                          .C.
C                       GIRDER TYPE W53DG                               .C.
 1520 NONC = 1
      BTF = S * 12.
      IF ( BTF .GT. 78. ) BTF = 78.                                     .C.
      WG= .878 + ( S - 4.0833 ) * 0.08                                  .C.
      ADSLAB = ( BTF - 49. ) * 6.                                       .C.
      SLABI = ( BTF - 49. ) * 18.                                       .C.
      YB=( 790.4 * 31.88 + ADSLAB * 50. ) / ( ADSLAB + 790.4)           .C.
      AG= 790.4 + ADSLAB                                                .C.
      ADY2G = 790.4 * ( ABS ( YB - 31.88 ) ) ** 2.                      .C.
      ADY2S = ADSLAB * ( ABS ( YB - 50. ) ) ** 2.
      ZB = ( 295053.1 + SLABI + ADY2G + ADY2S ) / YB                    .C.
      YT = 53. - YB
      ZT = ZB * YB / YT
      GI = ZT * YT
      GO TO 23
C                       GIRDER TYPE W58G                                .C.
  121 WG=0.671                                                          .C.
      ZB=9449.                                                          .C.
      YB=28.00                                                          .C.
      AG=603.5                                                          .C.
      ZT=8821.                                                          .C.
      YT=30.00                                                          .C.
      GI=ZB*YB
      GO TO 23
C                USER DEFINED GIRDER OR BULT TEE (SERIES 2 OR 3)
  122 IF ( KSER .EQ. 3 ) NONC = 1
      YB=YBG
      ZT=ZTG
      GI=XIG
      YT=YTG
      ZB=GI/YB
      GO TO 23
C                       GIRDER TYPE W73G                                .C.
  123 WG=0.830                                                          .C.
      ZB=14342.                                                         .C.
      YB=38.08                                                          .C.
      AG=746.7                                                          .C.
      ZT=15417.                                                         .C.
      YT=35.42                                                          .C.
      GI=YB*ZB
      GO TO 23
C                       GIRDER TYPE W42G                                .C.
  126 WG=0.415                                                          .C.
      ZB=4017.                                                          .C.
      YB=18.94                                                          .C.
      AG=373.3                                                          .C.
      ZT=3300.                                                          .C.
      YT=23.06                                                          .C.
      GI=YB*ZB
      GO TO 23
C                       GIRDER TYPE W50G                                .C.
  128 WG=0.584
      ZB=7232.
      YB=22.81
      AG=525.5
      ZT=6067.
      YT=27.19
      GI=YB*ZB
      GO TO 23
C
C  COMPUTE INITIAL CAMBER
C
23    IXNS=INT(XNS)
      IXNH=INT(XNH)
      IT1=INT(T1)
      GE=63756.*SQRT(FCUG*1000.)/1000.
      SE=63756.*SQRT(FCUS*1000.)/1000.
      PEACH=(CSTR*USS-35.0)*AS
      FS=PEACH*XNS
      FH=PEACH*XNH
      YSCL=(XNS*E+XNH*FCL)/(XNS+XNH)
      YSEND=(XNS*E+XNH*(FCL+RISE))/(XNS+XNH)
      GHT=YT+YB
C SET DEFAULT HARPING DIST.
      IF(XHARP.LE.0)THEN                                                .C.
         IF(GHT.LE.50.)THEN
              XHARP=XL/3.
         ELSE
              XHARP=.4*XL
         END IF
         IF ( NONC .EQ. 1 ) XHARP = 0.4 * XL
      END IF
C
C  PRINT OUT TITLE AND INPUT DATA
C
      WRITE(OUNIT,7075)CARCNT,DATE,TIME
      IF( NONC  .EQ. 1 ) THEN
         WRITE(OUNIT,7070)REVDAT
      ELSE
         WRITE(OUNIT,7078)REVDAT
      END IF
      WRITE(OUNIT,7080)NOPBM
 7075 FORMAT(A1,//,8X,' WASHINGTON STATE DEPT. OF TRANSPORTATION  ',    .C.
     1  A11,'  TIME  ',A8)                                              .C.
 7070 FORMAT('         *** PRESTRESSED NONCOMPOSITE GIRDER DEFLECTIONS *
     1**     REV ',
     2       A8,/)
 7078 FORMAT('         *** PRESTRESSED COMPOSITE GIRDER DEFLECTIONS ***
     1     REV ',
     2       A8,/)
 7080 FORMAT('         PROBLEM NO. ',I3,/)
      WRITE(OUNIT,7081)KSER,FCUG,XL,WG,S,AG
      WRITE(OUNIT,7082)IXNH,GI,IXNS,YB,FCL,YT
      WRITE(OUNIT,7083)E,ZT,RISE,WS,XHARP,WC
      WRITE(OUNIT,7084)AS,SBXML,USS
      IF(NONC.EQ.0)THEN
         WRITE(OUNIT,7085)CSI,IT1,YBC,GTS,FCUS,PHI
         IF(KSTY.EQ.1)WRITE(OUNIT,'(''+'',45X,''STRANDS ARE STRESS RELIE
     1VED.'',///)')
         IF(KSTY.EQ.0)WRITE(OUNIT,'(''+'',45X,''STRANDS ARE LOW RELAXATI
     1ON.'',///)')
      ELSE
         IF(KSTY.EQ.1)WRITE(OUNIT,7086)PHI
         IF(KSTY.EQ.0)WRITE(OUNIT,7087)PHI
      END IF
C INITIAL DL GIRDER DEFL.
      DG=(WG*5.*XL**4)*1728./(384.*GE*GI)
      CLM=(YB-YSCL)*(XNS+XNH)*PEACH
      ENDM=(YB-YSEND)*(XNS+XNH)*PEACH
C INITIAL PRES. DEFL.
      DP=-XL*XL*144*(CLM+((ENDM-CLM)/3.)*4.*XHARP*XHARP/XL/XL)/8./GE/GI
C INITIAL DEFL. AT RELEASE
      EE=DP+DG
C FIND SLAB AND CURB INSTANT DEFLECTIONS
      EES=(5.*WS*XL**4)*1728./(384.*GE*GI)
      EEC=(5.*WC*XL**4)*1728./(384.*GE*CSI)
C
C  OUTPUT AND DEFLECTIONS FOR PRESTRESSED GIRDERS
C
      IF( NONC .EQ. 0 ) THEN
C ADJUST INITIAL DEF. TO TIME OF SLAB POUR
      ETP=EE*(PHI*0.152*ALOG(T1+1.))+EE
C CAMBER AT TIME OF SLAB POUR IS ETP1
      ETP1=-ETP
C FINAL CAMBER ABOVE HORIZ. IS EXTR
      EXTR = - ETP - EES - EEC
C SCREED SETTING IS E
      E=EES+EEC
C EXCESS CAMBER IS ENUM
      ENUM = - ETP - EES
C
C BEGIN TO WRITE OUTPUT
C
      WRITE(OUNIT,9919) IT1,ETP1
      WRITE (OUNIT,9911) EES,EEC
      WRITE(OUNIT,9918) E
      WRITE(OUNIT,9910) EXTR,ENUM
      ELSE
C
C OUTPUT AND DEFLECTIONS FOR NONCOMPOSITE GIRDERS
C
      EXTR=-(EE+EES+EEC)*(PHI+1.)
      EE = -EE
      WRITE(OUNIT,9930)EE,EXTR
      CSI=GI
      END IF
C
C FIND LL DEFLECTION (DLL)
C
      XMLL=SBXML*CSI/YBC
      DLL=5.*XMLL*(XL**2)*144./(48.*GE*CSI)
      WRITE (OUNIT,9913)DLL
      WRITE(OUNIT,9931)
      GO TO 3300
8000  WRITE(ERUNIT,8010)
8010  FORMAT(2X,'INSUFFICIENT DATA ... TERMINATING DEFLECT. ANALYSIS.')
      RETURN
8100  WRITE(ERUNIT,8110)
8110  FORMAT(2X,'FILE EMPTY ... TERMINATING DEFLECTION ANALYSIS.')
      RETURN
8200  WRITE(ERUNIT,8210)
8210  FORMAT(2X,'ERROR IN DATA SET ... TERMINATING DEFLECT. ANALYSIS.')
      RETURN
 9904 FORMAT (20H INVALID GIRDER TYPE)                                  .C.
 9908 FORMAT ('         GIRDER SPACING LESS THAN 4.0'' NOT ALLOWED FOR S
     1ERIES ',I2)
 9910 FORMAT ('         FINAL CAMBER OF GIRDER =',F6.2,' INCHES ABOVE TH
     2E HORIZONTAL POSITION.',//,      '         EXCESS GIRDER CAMBER AL
     3LOWANCE FOR ESTABLISHING THE "A"',/,'         DIMENSION PER BRIDGE
     4 DESIGN MANUAL IS',F5.2,' INCHES',/)
 9911 FORMAT ('         DEFLECTION DUE TO SLAB WT =',5X,F7.2,' INCHES',/
     1       ,'         DEFLECTION DUE TO CURB + ASPH =',F8.2,' INCHES',
     2/,42X,' ------')
 9913 FORMAT ('         APPROXIMATE LIVE LOAD + IMPACT DEFL. IS',F5.2,'
     1 INCHES')                                                         .C.
 9918 FORMAT ('         SET  SCREEDS FOR A TOTAL OF      *',F5.2,' * INC
     1HES CAMBER ("C")'/'         ABOVE GRADE AT C.L. SPAN.',/)
 9919 FORMAT (//,'         "D" - GIRDER CAMBER AT TIME OF SLAB POUR(',I3
     1,' DAYS)=',F6.2,' INCHES',/)
 9930 FORMAT ('         GIRDER CAMBER AT RELEASE =',F6.2,' INCHES ABOVE
     2THE HORIZONTAL POSITION.',
     3  //,'         FINAL CAMBER OF GIRDER =  ',
     4 F6.2,' INCHES ABOVE THE HORIZONTAL POSITION.',/,' ')
 9931 FORMAT (//,'         GIRDER CAMBER IS BASED ON 35 KSI LOSSES,'
     1,/,'             PLAIN CONCRETE WEIGHT OF 155 PCF AND'
     2,/,'             FINAL GIRDER CONCRETE F''c.')                    .C.
 7081 FORMAT(/,9X,'GIRDER TYPE  ',10X,I2,12X,                           .C.
     1'FC (GIRDER)',10X,F4.2,'  KSI',/,9X,
     2'SPAN LENGTH',8X,F6.2,'  FT.',7X,'GIRDER WT.',10X,F5.3,'  K/FT.'
     3,/,9X,'GIRDER SPACING',6X,F5.2,'  FT.',7X,
     4'GIRDER AREA',8X,F6.1,'  IN.**2')
 7082 FORMAT(9X,'NO. HARPED STND',8X,I2,12X,'I (GIRDER)',6X,
     1 F9.0,'  IN.**4',/,9X,'NO. STRAIGHT STND',6X,I2,12X,
     2'Y BOTTOM (GIRDER)',3X,F5.2,'  IN.',/,9X,'FCL',17X,F5.2,'  IN.',
     37X,'Y TOP (GIRDER)',6X,F5.2,'  IN.')
 7083 FORMAT(9X,'E',19X,F5.2,'  IN.',7X,'S TOP (GIRDER)',
     1 4X,F7.1,'  IN.**3',/,9X,'RISE / HARPED STND',2X,F5.2,'  IN.',7X,
     2'WS',18X,F5.3,'  K/FT.',/,9X,'HARP. DIST.',9X,F5.2,
     3'  FT.',7X,'WC',18X,F5.3,'  K/FT.')
 7084 FORMAT(9X,'STRAND AREA',9X,F5.3,
     1'  IN.**2',4X,'SBXML',15X,F5.3,'  KSI',/,9X,
     2'ULTIMATE STRENGTH',3X,F5.1,'  KSI')
 7085 FORMAT('+',45X   ,'I (COMPOSITE)',3X,F9.0,'  IN.**4',/,9X,
     1'DAYS',18X,I3,12X,'Y BOTTOM (COMP)',5X,F5.2,'  IN.',/,9X
     2,'GROSS SLAB THICK',4X,F5.2,'  IN.',7X,
     3'FC (SLAB)',12X,F4.2,'  KSI',/,9X,
     4'CREEP',16X,F4.2)
 7086 FORMAT('+',45X,'CREEP',16X,F4.2,/,9X,'STRANDS ARE STRESS RELIEVED'
     1,///)
 7087 FORMAT('+',45X,'CREEP',16X,F4.2,/,9X,'STRANDS ARE LOW RELAXATION'
     1,///)
      END
C
C
C
C
C
C
C
C
C
C

