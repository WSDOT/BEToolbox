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

C********************************************************************          
C     PROGRAM TO COMPUTE THE ULTIMATE AXIAL FORCE AND BIAXIAL BENDING           
C     MOMENT FOR REINFORCED CONCRETE COLUMN OF ARBITRARY CROSS-SECTION          
C                                                                               
C     REVISED FOR TO BYPASS ERROR 209 (DIVIDE BY ZERO) 12.07.77                 
C                                                                               
C     REVISED FOR INTERACTIVE                          07.00.80                 
C     REVISED FOR DESIGN AND CHECK                     01.00.81                 
C     REVISED FOR PRINT FORMAT                         11.23.81                 
C     REVISED FOR CMS                                  08.02.82                 
C     CHANGE SEISMIC LOADS TO UNREDUCED ARS                                     
C     ASK FOR Z FACTOR, CREATE INPUT FOR FOOT PROG     03.24.83                 
C     FOOT FILE BUG - WEIGHT OF COLUMN FOR MULTICOLUMN                          
C     BENT PINNED AT BOTTOM ADDED TO DEAD LOAD TWICE   06.22.83                 
C     REVISED FOR KEY DESIGN                           10.03.83                 
C     MADE YIELD A SUBROUTINE - RENUMBERED PROGRAM     10.28.83                 
C     ADDED TOP STEEL AND COST VALUES TO FOOT FILE     12.09.83                 
C     ADDED NOTE: SERVICE LOADS NOT USED IN DESIGN MODE                         
C     AND CORRECTED SPELLING ERROR IN LOAD.            11.05.84                 
C     CHANGED MOMNET MAGNIFICATION AND BUCKLING CRITERA FROM                    
C     "PCA" METHOD TO "AASHTO" METHOD                                           
C     INCREASE PHI BY 1.3 FOR SEISMIC ONLY (I.E. 1.0 TO 1.2)                    
C     ELIMINATE MOMENT MAG FOR SEISMIC LOADS                                    
C     CHANGE MAX & MIN SPACING FOR LATERAL REINFORCE.  11.26.85                 
C     UPDATED FOOTING COST DATA TO 1985 AVERAGES       04.01.86                 
C*********************************************************************          
C                                                                               
C                             P63925                                            
C                                                                               
C                  MODIFIED FOR USE AT W.S.D.O.T.                               
C                               BY                                              
C                           DAVE PALMER                                         
C                          APRIL 4, 1988                                        
C                                                                               
C                                                                               
C     REVISION .A. BY DAVE PALMER APRIL 4, 1988                                 
C            CHANGES MADE FOR USE AT W.S.D.O.T.                                 
C                                                                               
C     REVISION .B. BY DAVE PALMER JANUARY, 1989                                 
C            CHANGES WERE MADE TO THE CODE AS REQUESTED BY NEAL                 
C            CAMPBELL OF BRIDGE.                                                
C                                                                               
C     REVISION .C. BY TERRY BEEBE MARCH, 1989                                   
C            CHANGES WERE MADE TO THE CODE AS REQUESTED BY                      
C            MIKE SZRAMEK                                                       
C                                                                               
C     REVISION .D. BY NEAL J. CAMPBELL JAN, 1990
C            CONVERSION TO VMS
C
C     REVUSION .E. BY TERRY BEEBE MARCH 1990
C            CHANGED PLOT FILE FORMAT
C
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C
C IGP=0   ANALYSIS                                                              
C IGP=1   DESIGN                                                                
C IGP=2   CHECK                                                                 
C***********************************************************************        
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      REAL*8 MXSC,  MYSC,  MSC,  PSC                                            
      REAL*8 MXFC,  MYFC,  MFC,  PFC                                            
      REAL*8 MXSF,  MYSF,  MSF,  PSF                                            
      REAL*8 MXFF,  MYFF,  MFF,  PFF                                            
      REAL*8 MXFCI, MYFCI, MFCI, PFCI                                           
      REAL*8 MAGY, MAGX, MAG, NO                                                
      REAL*8 KX, KY                                                             
      CHARACTER*40 HEAD, HEAD2                                          .D.
      CHARACTER*11 RNDATE                                               .D.
      CHARACTER*8 NHTIME, PRNT                                          .D.
      REAL*4 GR,CA                                                              
C***********************************************************************        
      DIMENSION LNODE(5),RP(10),RE(7),DI(7),P(10,7),                            
     *  UMX(10,7),UMY(10,7),UM(10,7),                                           
     *  X(100),Y(100),AC(100),ACX(100),ACY(100),                                
     *  XS(200),YS(200),IAS(200),AS(200),                                       
     *  R(5),RS(5),LSNODE(50),                                                  
     *  X1(50),Y1(50),X2(50),Y2(50),REK(22),IPB(7),                             
     *  MYSC(22),  MXSC(22),  MSC(22),  PSC(22),                                
     *  MYFC(21),  MXFC(21),  MFC(21),  PFC(21),                                
     *  MYSF(21),  MXSF(21),  MSF(21),  PSF(21),                                
     *  MYFF(21),  MXFF(21),  MFF(21),  PFF(21),                                
     *  MYFCI(21), MXFCI(21), MFCI(21), PFCI(21), UMR(21),                      
     *  MAGY(21),  MAGX(21), PCYC(21), PCXC(21), EIX(21), EIY(21),              
     *  GR(22),CA(22),                                         
     *  IBOMB(22),PHI(22),P1(10,7),UM1(10,7),DATE(1),IERR(22),                  
     *  BDIA(18),ABAR(18)                                                       
C                                                                               
      COMMON/VALUE/SRA,CRA,D,DP                                                 
C                                                                               
      DATA GR(1),GR(2),GR(3),GR(4),GR(5),GR(6),GR(7),GR(8),GR(9),               
     *GR(10),GR(11),GR(12),GR(13),GR(14),GR(15),GR(16),GR(17),GR(18),           
     *GR(19),GR(20),GR(21),GR(22)/                                              
     *'IH ','IH ','IH ','IP ','IP ','IP ','II ','III','III','III',              
     *'IV ','IV ','IV ','V  ','VI ','VI ','VI ','VII','VII',                    
     *'AL1','AL2','***'/                                                        
      DATA CA(1),CA(2),CA(3),CA(4),CA(5),CA(6),CA(7),CA(8),CA(9),               
     *CA(10),CA(11),CA(12),CA(13),CA(14),CA(15),CA(16),CA(17),CA(18),           
     *CA(19),CA(20),CA(21),CA(22)/                                              
     *'1','2','3','1','2','3',' ','1','2','3','1','2','3',' ',                  
     *'1','2','3','1','2',' ',' ','*'/                                          
      INTEGER YES                                                               
      DATA YES,N/'Y','N'/                                                       
C                                                                               
C***********************************************************************        
C                  CURRENT VERSION AND DATE                                     
C***********************************************************************        
      DATA    DATEV/'11.26.85'/                                                 
      IVERS = 1                                                                 
      DATA    DATEF/'12.09.83'/                                                 
      IVERF = 2                                                                 
C                                                                               
      CALL PARM  (RNDATE,NHTIME)                                        .D.     
      NPROB=0                                                                   
C                                                                               
      BDIA(3)  = 0.375                                                          
      BDIA(4)  = 0.500                                                          
      BDIA(5)  = 0.625                                                          
      BDIA(6)  = 0.750                                                          
      BDIA(7)  = 0.875                                                          
      BDIA(8)  = 1.000                                                          
      BDIA(9)  = 1.128                                                          
      BDIA(10) = 1.270                                                          
      BDIA(11) = 1.410                                                          
      BDIA(14) = 1.693                                                          
      BDIA(18) = 2.257                                                          
C                                                                               
      ABAR(3)  =  .11                                                           
      ABAR(4)  =  .20                                                           
      ABAR(5)  =  .31                                                           
      ABAR(6)  =  .44                                                           
      ABAR(7)  =  .60                                                           
      ABAR(8)  =  .79                                                           
      ABAR(9)  = 1.00                                                           
      ABAR(10) = 1.27                                                           
      ABAR(11) = 1.56                                                           
      ABAR(14) = 2.25                                                           
      ABAR(18) = 4.00                                                           
C                                                                               
    5 NL=21                                                                     
      NLOAD = 0                                                                 
      NWRITE = 0                                                                
      TOLP=1.0E-04                                                              
      TOLE=1.0E-02                                                              
      NITRN=200                                                                 
      PRNT='NO '                                                        .D.
      PHI(20)=1.0                                                               
      PHI(21)=1.0                                                               
      PHI(22)=1.0                                                               
      FC=0.0                                                                    
      E=0.0                                                                     
      EO=0.0                                                                    
      SY=0.0                                                                    
      NC =0                                                                     
      DSO=0.0                                                                   
      CGS=0.0                                                                   
      CSP=0.0                                                                   
      IBENT=0                                                                   
      IPILE=0                                                                   
C                                                                               
C       THE REFERENCE AXES OF THE COLUMN CROSS-SECTION ARE TAKEN AS             
C       THE PRINCIPAL AXES OF THE SECTION.  ALL COORDINATES OF THE              
C       CROSS-SECTIONAL DATA ARE REFERENCED TO THE CENTROID OF THE              
C       SECTION (I.E., THE ORIGIN IS AT THE CENTROID).  THE PROGRAM             
C       MAY BE USED TO ANALYZE, DESIGN OR CHECK A PARTICULAR COLUMN.            
C       FOR THE DESIGN OPTION, IGP = 1, THE PROGRAM ITERATES                    
C       TO FIND THE CONTROLLING GROUP LOADING AND AMOUNT OF STEEL               
C       REQUIRED FOR THE CONTROLLING GROUP LOADING.  THE COLUMN                 
C       IS DESIGNED FOR ULTIMATE AXIAL FORCE AND ULTIMATE BIAXIAL               
C       MOMENT.                                                                 
C                                                                               
                                                                                
      READ (5,5000,ERR=999) HEAD                                        .B.     
 5000 FORMAT (A)                                                        .D.     
      READ (5,*) ICON,IPILE                                             .B.     
C5010 FORMAT (2I10)                                                             
C                                                                               
      NLOOP=1                                                                   
      IF(IPILE.EQ.1) TOLP=1.0E-04                                               
      IF(IPILE.EQ.1) TOLE=1.0E-02                                               
      IF(ICON.EQ.2) NNODE=4                                                     
      IF(ICON.EQ.3.OR.ICON.EQ.8) NNODE=8                                        
      IF(ICON.EQ.4.OR.ICON.EQ.9) NNODE=40                                       
      IF(ICON.EQ.6)              NNODE=6                                        
      IF(ICON.EQ.1) READ(5,*) NLOOP,NNODE                               .B.     
C5020 FORMAT (2I10)                                                     .B.     
C                                                                               
      READ(5,*)  IREBAR, NSLOOP, NBAR                                   .B.     
C5030 FORMAT (I10)                                                      .B.     
C     READ(5,5040)  NSLOOP,NBAR                                         .B.     
C5040 FORMAT (2I10)                                                     .B.     
C                                                                               
      IF(IREBAR.EQ.2.AND.NSLOOP.LT.2) WRITE(6,6000)                             
 6000 FORMAT(//7X,                                                              
     *   '*** ERROR: NO. OF INTERSECTING LOOPS MUST BE 2 OR MORE'/              
     *7X,'           EXECUTION TERMINATED . . .')                               
      IF(IREBAR.EQ.2.AND.NSLOOP.LT.2) STOP                                      
C                                                                               
      IF(IREBAR.NE.2) GO TO 10                                                  
      NCHECK=(NBAR+1)/2 *2                                                      
      NBAR=NBAR/2                                                               
      NBAR=NBAR*2                                                               
      IF(NBAR.NE.NCHECK) WRITE(6,6010) NBAR                                     
 6010 FORMAT(//7X,                                                              
     *   '*** ERROR: THE TOTAL NUMBER OF BARS MUST BE EVEN.'/                   
     *7X,'           TOTAL NUMBER OF BARS SET EQUAL TO ',I3/                    
     *7X,'           EXECUTION CONTINUES . . .')                                
C                                                                               
   10 READ(5,*) IPLOT                                                   .B.     
C5050 FORMAT(I10)                                                       .B.     
      IF(ICON.LT.6.OR.IPLOT.EQ.0) GO TO 20                                      
      IPLOT=1                                                                   
      WRITE(6,6020)                                                             
 6020 FORMAT(//7X,                                                              
     *   'WHEN CONCRETE TYPE >= 6 THEN PLOT TYPE IS SET EQUAL TO 1'/            
     *7X,'EXECUTION CONTINUES . . .')                                           
C                                                                               
   20 READ(5,*) IGP,RMIN,RMAX                                           .C.     
C5060 FORMAT (I10,2F10.1,I10,3F10.2)                                    .B.     
C                                                                               
      IF(RMIN.EQ.0) RMIN=1.0                                                    
      IF(RMAX.EQ.0) RMAX=6.0                                                    
      RMIN1=RMIN/100.                                                           
      RMAX1=RMAX/100.                                                           
C                                                                               
      NAS=NSLOOP                                                                
      IF(IREBAR.EQ.1) NAS=NBAR                                                  
      MNODE=NNODE+NLOOP                                                         
C                                                                               
      NPAGE=1                                                                   
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
C                                                                               
      WRITE(6,6030) IVERS,DATEV                                                 
 6030 FORMAT (//,                                                               
     *7X,'************************************************************',        
     *   '************'/                                                        
     *7X,'*                YIELD PROGRAM - VERSION',I3,4X,A8,'        ',        
     *   '        *'/                                                           
     *7X,'************************************************************',        
     *   '************')                                                        
C                                                                               
      IF(ICON.EQ.1) WRITE(6,6031) ICON                                          
      IF(ICON.EQ.2) WRITE(6,6032) ICON                                          
      IF(ICON.EQ.3) WRITE(6,6033) ICON                                          
      IF(ICON.EQ.4) WRITE(6,6034) ICON                                          
      IF(ICON.EQ.6) WRITE(6,6035) ICON                                          
      IF(ICON.EQ.8) WRITE(6,6036) ICON                                          
      IF(ICON.EQ.9) WRITE(6,6037) ICON                                          
 6031 FORMAT(//7X,'COLUMN TYPE                   =',I3,'  (MANUAL)')            
 6032 FORMAT(//7X,'COLUMN TYPE                   =',I3,'  (RECTANGULAR)         
     * '  )                                                                     
 6033 FORMAT(//7X,'COLUMN TYPE                   =',I3,'  (RECTANGULAR',        
     *' W/FILLETS)')                                                            
 6034 FORMAT(//7X,'COLUMN TYPE                   =',I3,'  (RECTANGULAR',        
     *' W/SEMI-CIRCULAR ENDS)')                                                 
 6035 FORMAT(//7X,'COLUMN TYPE                   =',I3,'  (HEXAGONAL)')         
 6036 FORMAT(//7X,'COLUMN TYPE                   =',I3,'  (OCTAGONAL)')         
 6037 FORMAT(//7X,'COLUMN TYPE                   =',I3,'  (CIRCULAR)')          
C***********************************************************************        
      WRITE(6,6040) NLOOP,NNODE                                                 
 6040 FORMAT(7X,'CONCRETE LOOPS                =',I3,/                          
     *7X,'TOTAL NO. OF CONCRETE COORD.  =',I3)                                  
C***********************************************************************        
      IF(IREBAR.EQ.1) WRITE(6,6041) IREBAR                                      
      IF(IREBAR.EQ.2) WRITE(6,6042) IREBAR                                      
      IF(IREBAR.EQ.3) WRITE(6,6043) IREBAR                                      
      IF(IREBAR.EQ.4) WRITE(6,6044) IREBAR                                      
      IF(IREBAR.EQ.5) WRITE(6,6045) IREBAR                                      
C                                                                               
 6041 FORMAT(7X,'STEEL REBAR PATTERN           =',I3,'  (MANUAL)')              
 6042 FORMAT(7X,'STEEL REBAR PATTERN           =',I3,'  (INTERSECTING',         
     *' LOOPS)')                                                                
 6043 FORMAT(7X,'STEEL REBAR PATTERN           =',I3,'  (SEMI-CIRCULAR',        
     *' ENDS)')                                                                 
 6044 FORMAT(7X,'STEEL REBAR PATTERN           =',I3,'  (CONCENTRIC',           
     *' LOOPS)')                                                                
 6045 FORMAT(7X,'STEEL REBAR PATTERN           =',I3,'  (ROWS)')                
      IF(IREBAR.NE.5) WRITE(6,6046) NSLOOP,NBAR                                 
      IF(IREBAR.EQ.5) WRITE(6,6047) NSLOOP,NBAR                                 
C                                                                               
 6046 FORMAT(7X,'NUMBER OF STEEL REBAR LOOPS   =',I3,/                          
     *7X,'TOTAL NO. OF STEEL REBARS     =',I3)                                  
 6047 FORMAT(7X,'NUMBER OF STEEL REBAR ROWS    =',I3,/                          
     *7X,'TOTAL NO. OF STEEL REBARS     =',I3)                                  
C***********************************************************************        
      IF(IPLOT.EQ.0) WRITE(6,6050) IPLOT                                        
      IF(IPLOT.EQ.1) WRITE(6,6051) IPLOT                                        
      IF(IPLOT.EQ.2) WRITE(6,6052) IPLOT                                        
      IF(IPLOT.EQ.3) WRITE(6,6053) IPLOT                                        
      IF(IPLOT.EQ.4) WRITE(6,6054) IPLOT                                        
C                                                                               
 6050 FORMAT(7X,'PLOT TYPE                     =',I3,'  (NO PLOT)')             
 6051 FORMAT(7X,'PLOT TYPE                     =',I3,'  (PN VS. MN)')           
 6052 FORMAT(7X,'PLOT TYPE                     =',I3,'  (PN VS. MNX)')          
 6053 FORMAT(7X,'PLOT TYPE                     =',I3,'  (PN VS. MNY)')          
 6054 FORMAT(7X,'PLOT TYPE                     =',I3,'  (MNY VS. MNX)')         
C***********************************************************************        
      IF(IGP.EQ.0) WRITE(6,6060) IGP                                            
      IF(IGP.EQ.1) WRITE(6,6061) IGP                                            
      IF(IGP.EQ.2) WRITE(6,6062) IGP                                            
C                                                                               
 6060 FORMAT(7X,'DESIGN TYPE                   =',I3,'  (ANALYSIS)')            
 6061 FORMAT(7X,'DESIGN TYPE                   =',I3,'  (DESIGN)')              
 6062 FORMAT(7X,'DESIGN TYPE                   =',I3,'  (CHECK)')               
C***********************************************************************        
      WRITE(6,6070) RMIN,RMAX                                                   
 6070 FORMAT(7X,'PERCENT STEEL LIMITS          =',3X,'  (',F4.2,                
     *' % MIN.  ',F4.2,' % MAX.)')                                              
C                                                                               
C     IF(IREBAR.NE.2.AND.IREBAR.NE.4) GO TO 30                                  
      IF(IGP.EQ.0)                    GO TO 30                                  
C                                                                               
C     WRITE(6,6080) NC,DSO,CGS,CSP                                      .C.     
C6080 FORMAT(///7X,                                                     .C.     
C    *   '*************'/                                               .C.     
C    *7X,'* BENT DATA *'/                                               .C.     
C    *7X,'*****************************************************',       .C.     
C    *   '********************'//                                       .C.     
C    *7X,'NUMBER OF COLUMNS IN BENT                  =',3X,I3/          .C.     
C    *7X,'OUT TO OUT DISTANCE (DIAMETER) OF SPIRAL   =',F6.2,' (INC',   .C.     
C    * 'HES)'/                                                          .C.     
C    *7X,'DISTANCE FROM TOP COLUMN PLASTIC HINGE TO'/                   .C.     
C    *7X,'CENTER OF GRAVITY OF THE SUPERSTRUCTURE    =',F6.2,' (FEET)'/ .C.     
C    *7X,'CENTER TO CENTER SPACING OF COLUMNS        =',F6.2,' (FEET)') .C.     
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
   30 READ (5,*) FC,E,EO,SY                                             .B.     
C5070 FORMAT (4F10.0)                                                   .B.     
C                                                                               
      IF(FC.NE.0.) GO TO 31                                                     
      FC=3250.                                                                  
   31 IF(E.NE.0.) GO TO 32                                                      
      E=29000000.                                                               
   32 IF(EO.NE.0.) GO TO 33                                                     
      EO=.003                                                                   
   33 IF(SY.NE.0.) GO TO 34                                                     
      SY=60000.                                                                 
C                                                                               
   34 EC = 63681. * DSQRT(FC)                                           .C.     
      RM = E/EC                                                                 
      WRITE(6,6110) FC,EC,E,EO,SY                                               
 6110 FORMAT (///7X,                                                            
     *   '******************************'/                                      
     *7X,'* MATERIAL PROPERTIES  (PSI) *'/                                      
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'ULTIMATE CONCRETE COMPRESSIVE STRESS - FC = ',F9.0/                   
     *7X,'YOUNG S MODULUS FOR CONCRETE         - EC = ',F9.0/                   
     *7X,'YOUNG S MODULUS FOR STEEL BARS       - ES = ',F9.0/                   
     *7X,'ULTIMATE CONCRETE COMPRESSIVE STRAIN - EO = ',5X,F4.3/                
     *7X,'YIELDING STRESS FOR STEEL BARS       - FY = ',F9.0)                   
C                                                                               
      EO1=EO                                                                    
C                                                                               
C *********  ADJUST MATERIAL PROPS TO FT.-K UNITS ************                  
C                                                                               
      FC  = FC*.144                                                             
      FC1 = FC/144.                                                             
      E   = E*.144                                                              
      SY  = SY*.144                                                             
      SY1 = SY/144.                                                             
      EC  = EC *.144                                                            
C                                                                               
C                                                                               
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
C                                                                               
      CALL CONCY(ICON,NLOOP,LNODE,NNODE,MNODE,X,Y,HX,HY)                        
      ARAT=HX/HY                                                                
C     IF(ARAT.GE.3) NITRN=100                                                   
C     IF(ICON.EQ.4) NITRN=100                                                   
C                                                                               
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
C                                                                               
      CALL REBAR(IREBAR,NSLOOP,LSNODE,NBAR,NAS,XS,YS,AS,IAS,                    
     *X1,Y1,X2,Y2,RS,SPACE,BAREA,HX,HY,NNODE,MNODE,NLOOP,                       
     *X,Y,ICON,DSO,ABAR,IBAR)                                                   
C                                                                               
      IF(IREBAR.EQ.1.OR.IREBAR.EQ.3.OR.IREBAR.EQ.5) GO TO 40                    
C                                                                               
      P2 = DSO                                                                  
      IF(IREBAR.EQ.2) P2 = ((NSLOOP - 1) * SPACE) + DSO                         
      IF(P2+4.0.LT.HX)  WRITE(6,6120)                                           
      IF(DSO+4.0.LT.HY) WRITE(6,6125)                                           
 6120 FORMAT(//7X,'***WARNING:  THE CONCRETE COVER IS GREATER THAN 2 ',         
     *'INCHES '/                                                                
     *7X,'IN THE X-DIRECTION')                                                  
 6125 FORMAT(//7X,'***WARNING:  THE CONCRETE COVER IS GREATER THAN 2 ',         
     *'INCHES '/                                                                
     *7X,'IN THE Y-DIRECTION')                                                  
C                                                                               
      IF(IGP.EQ.2.AND.IREBAR.EQ.4.AND.2.0*RS(1).GT.DSO) WRITE(6,6130)           
      IF(IGP.EQ.2.AND.IREBAR.EQ.4.AND.2.0*RS(1).GT.DSO) STOP                    
      IF(IGP.EQ.2.AND.IREBAR.EQ.2.AND.2.0*RS(1).GT.DSO) WRITE(6,6130)           
      IF(IGP.EQ.2.AND.IREBAR.EQ.2.AND.2.0*RS(1).GT.DSO) STOP                    
 6130 FORMAT(//7X,'***ERROR: THE STEEL REBARS ARE OUTSIDE THE SPIRAL ',         
     *'REINFORCEMENT !'/                                                        
     *7X,'          EXECUTION TERMINATED . . .'/)                               
C                                                                               
   40 CXMAX = 0.0                                                               
      CYMAX = 0.0                                                               
C                                                                               
      DO 50 I=1,MNODE                                                           
      X(I)=X(I)/12.                                                             
      IF(X(I).GT.CXMAX) CXMAX = X(I)                                            
      Y(I)=Y(I)/12.                                                             
      IF(Y(I).GT.CYMAX) CYMAX = Y(I)                                            
   50 CONTINUE                                                                  
C                                                                               
      SXMAX = 0.0                                                               
      SYMAX = 0.0                                                               
      DO 60 I=1,NBAR                                                            
      XS(I)=XS(I)/12.                                                           
      IF(XS(I).GT.SXMAX) SXMAX = XS(I)                                          
      YS(I)=YS(I)/12.                                                           
      IF(YS(I).GT.SYMAX) SYMAX = YS(I)                                          
   60 CONTINUE                                                                  
      DO 70 I=1,NAS                                                             
      AS(I)=AS(I)/144.                                                          
   70 CONTINUE                                                                  
C                                                                               
      IF(SXMAX.GE.CXMAX) WRITE(6,6140)                                          
      IF(SXMAX.GE.CXMAX) STOP                                                   
      IF(SYMAX.GE.CYMAX) WRITE(6,6140)                                          
      IF(SYMAX.GE.CYMAX) STOP                                                   
 6140 FORMAT(//7X,'***ERROR: ONE OR MORE REBARS ARE OUTSIDE THE COLUMN',        
     *' DIMENSIONS !'/                                                          
     *7X,'EXECUTION TERMINATED . . . '/)                                        
C***********************************************************************        
C                 CALCULATE THE CONCRETE AREA                                   
C***********************************************************************        
      ACT=0.                                                                    
      NI=1                                                                      
      XI=0.                                                                     
      YI=0.                                                                     
      DO 90 L=1,NLOOP                                                           
      NF=NI+LNODE(L)-1                                                          
      DO 80 I=NI,NF                                                             
      I1=I+1                                                                    
      CALL AREA (X(I),Y(I),X(I1),Y(I1),AC(I),ACX(I),ACY(I))                     
      ACT=ACT+AC(I)                                                             
   80 CONTINUE                                                                  
      NF=NF+1                                                                   
C***********************************************************************        
C                  CALCULATE MOMENT OF INERTIA                                  
C***********************************************************************        
      CALL INERT (NI,NF,X,Y,XIN,YIN)                                            
C                                                                               
      XI=XI+XIN                                                                 
      YI=YI+YIN                                                                 
      AC(NF)=0.                                                                 
      ACX(NF)=0.                                                                
      ACY(NF)=0.                                                                
      NI=NF+1                                                                   
   90 CONTINUE                                                                  
C***********************************************************************        
C                    CALCULATE STEEL AREA                                       
C***********************************************************************        
      AST=0.                                                                    
      DO 100 J=1,NBAR                                                           
      JS=IAS(J)                                                                 
      AST=AST+AS(JS)                                                            
  100 CONTINUE                                                                  
      RATI=AST/ACT                                                              
C                                                                               
      IF(IGP.NE.1) GO TO 140                                                    
      RAT2=RATI                                                                 
      IF(RATI.GE.RMIN1.AND.RATI.LE.RMAX1) GO TO 140                             
C                                                                               
      SRATIO=RMIN + .01                                                         
      IF(RATI.LT.RMIN1) GO TO 110                                               
      WRITE(6,6150) RMAX,SRATIO                                                 
 6150 FORMAT( //7X,' ERROR ... THE INPUT PERCENT STEEL IS GREATER',             
     *' THAN ',F4.2,' %'/7X,                                                    
     *' PERCENT STEEL ASSUMED TO BE ',F4.2,' %  - EXECUTION CONTINUES.')        
      GO TO 120                                                                 
C                                                                               
  110 WRITE(6,6155) RMIN,SRATIO                                                 
 6155 FORMAT( //7X,' ERROR ... THE INPUT PERCENT STEEL IS LESS',                
     *' THAN ',F4.2,' %'/7X,                                                    
     *' PERCENT STEEL ASSUMED TO BE ',F4.2,' %  - EXECUTION CONTINUES.')        
C                                                                               
  120 RATI=RMIN1 + .0001                                                        
      RAT2=RATI                                                                 
      AST=RATI*ACT                                                              
      SAREA=AST/NBAR                                                            
      DO 130 I=1,NAS                                                            
      AS(I)=SAREA                                                               
      IF(IGP.EQ.1) IAS(I) = 1                                                   
  130 CONTINUE                                                                  
C                                                                               
  140 POC=.85*FC*(ACT-AST)+AST*SY                                               
      AST1=AST*144.                                                             
      RATI1=RATI*100.                                                           
C***********************************************************************        
C              FIND MOMENT OF INERTIA OF STEEL REBARS                           
C***********************************************************************        
      XIS = 0.0                                                                 
      YIS = 0.0                                                                 
      DO 145 I = 1,NBAR                                                         
      IS = IAS(I)                                                               
      XIS = XIS + AS(IS) * YS(I)**2                                             
      YIS = YIS + AS(IS) * XS(I)**2                                             
  145 CONTINUE                                                                  
C                                                                               
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      WRITE(6,6160) ACT,POC,AST1,RATI1,YI,XI,YIS,XIS                            
 6160 FORMAT (//7X,                                                             
     *   '**************************'/                                          
     *7X,'* INITIAL REFERENCE DATA *'/                                          
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'TOTAL AREA OF THE SECTION              AG =',F10.2,' FT**2'/          
     *7X,'NOMINAL AXIAL LOAD STRENGTH            PO =',F10.2,' KIPS'/           
     *7X,'TOTAL REINFORCEMENT AREA              AST =',F10.2,' IN**2'/          
     *7X,'PERCENT STEEL                             =',F10.2,' %'//             
     *7X,'GROSS MOMENT OF INERTIA ABOUT Y-AXIS  IYC =',                         
     *    F10.2,' FT**4'/                                                       
     *7X,'GROSS MOMENT OF INERTIA ABOUT X-AXIS  IXC =',                         
     *    F10.2,' FT**4'//                                                      
     *7X,'STEEL MOMENT OF INERTIA ABOUT Y-AXIS  IYS =',                         
     *    F10.4,' FT**4'/                                                       
     *7X,'STEEL MOMENT OF INERTIA ABOUT X-AXIS  IXS =',                         
     *    F10.4,' FT**4')                                                       
      IF(IGP.EQ.2) GO TO 300                                                    
      IF(IGP.EQ.1) GO TO 510                                                    
C                                                                               
C***********************************************************************        
C                    ANALYSIS IGP = 0                                           
C***********************************************************************        
      NP = 10                                                                   
      IF(ICON.GE.6.AND.IPLOT.LT.4) GO TO 170                                    
      NE=7                                                                      
      RE(1)=89.999                                                              
      RE(2)=75.                                                                 
      RE(3)=60.                                                                 
      RE(4)=45.                                                                 
      RE(5)=30.                                                                 
      RE(6)=15.                                                                 
      RE(7)=.001                                                                
      GO TO 180                                                                 
C                                                                               
  170 NE=1                                                                      
      RE(1)=89.999                                                              
C                                                                               
  180 IF(IPILE.EQ.1) GO TO 190                                                  
      RP(1) =.60                                                                
      RP(2) =.50                                                                
      RP(3) =.40                                                                
      RP(4) =.30                                                                
      RP(5) =.20                                                                
      RP(6) =.10                                                                
      RP(7) =.05                                                                
      RP(8) = 0.0                                                               
      RP(9) = -.05                                                              
      RP(10)= -.10                                                              
      GO TO 200                                                                 
C                                                                               
  190 RP(1) =.50                                                                
      RP(2) =.40                                                                
      RP(3) =.30                                                                
      RP(4) =.20                                                                
      RP(5) =.12                                                                
      RP(6) =.075                                                               
      RP(7) =.050                                                               
      RP(8) =.025                                                               
      RP(9) = 0.0                                                               
      RP(10)= -.05                                                              
C                                                                               
  200 DO 210 J = 1,NE                                                           
      DO 205 I = 1,NP                                                           
      P(I,J)   = 10.E15                                                         
      UM(I,J)  = 10.E15                                                         
      UMX(I,J) = 10.E15                                                         
      UMY(I,J) = 10.E15                                                         
      CALL MOMENT (PU, PUX, PUY, RDRP, RDRE, LNODE, X, Y, NLOOP,                
     * NBAR, XS, YS, AS, IAS, RP(I), RE(J), SY, FC, EO, E,                      
     * ACT, AST, ITRN, NITRN, TOLP, TOLE, AC, ACX, ACY)                         
      IF(RDRP.GT.TOLP.OR.RDRE.GT.TOLE) GO TO 205                                
      P(I,J)   = PU                                                             
      UMX(I,J) = PUX                                                            
      UMY(I,J) = PUY                                                            
      UM(I,J)  = DSQRT(PUX*PUX+PUY*PUY)                                         
  205 CONTINUE                                                                  
  210 CONTINUE                                                                  
C                                                                               
      CALL PBAL(P,UM,NP,NE,POC,IPB)                                             
      IF(IPILE.EQ.1) CALL PILE(P,UMX,UMY,UM,IPB,FC,ACT,NP,NE,RE,MB)             
C **********************************************************************        
C          ANALYSIS PRINT NOMINAL MOMENTS - IGP=0                               
C **********************************************************************        
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      IF(IPILE.EQ.1)  WRITE(6,6170) AST1                                        
      IF(IPILE.EQ.0)  WRITE(6,6180) AST1                                        
      DO 240 J = 1,NE                                                           
C                                                                               
      IF(J.EQ.4) CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                            
      IF(J.EQ.4.AND.IPILE.EQ.1) WRITE(6,6170) AST1                              
      IF(J.EQ.4.AND.IPILE.EQ.0) WRITE(6,6180) AST1                              
C                                                                               
      DO 230 I = 1,NP                                                           
      IF(I.GE.2) GO TO 220                                                      
      WRITE(6,6190) RE(J),UMY(I,J),UMX(I,J),UM(I,J),P(I,J),                     
     *              IPB(J)                                                      
 6190 FORMAT (/,8X,F10.1,2X,4F10.0,I10)                                         
      GO TO 230                                                                 
C                                                                               
  220 WRITE(6,6200) UMY(I,J),UMX(I,J),UM(I,J),P(I,J)                            
 6200 FORMAT (20X,4F10.0)                                                       
C                                                                               
  230 CONTINUE                                                                  
  240 CONTINUE                                                                  
C                                                                               
 6170 FORMAT(//7X,'NOMINAL STRENGTH VALUES MODIFIED BY PHI (KIP-FT)'/           
     *7X,'(PHI VARIES FROM .70 TO .90)           AST = ',F6.2,' SQ IN'//        
     *7X,'     ANGLE       TRANS      LONG      COMB     AXIAL',                
     *   '  P-BALANCE'/                                                         
     *7X,'                  MNY       MNX        MN        PN ',                
     *   '   (APPROX)')                                                         
 6180 FORMAT (///7X,                                                            
     *   '***************************************',                             
     *   '*********************'/                                               
     *7X,'* NOMINAL STRENGTH VALUES  (KIP-FT)    ',                             
     *   ' AST = ',F6.2,' SQ IN *'/                                             
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'     ANGLE       TRANS      LONG      COMB     AXIAL',                
     *   '  P-BALANCE'/                                                         
     *7X,'                  MNY       MNX        MN        PN ',                
     *   '  (APPROX)')                                                          
C                                                                               
      IF(IPLOT.GE.1)CALL PLOTS(RE,RP,P,UMX,UMY,UM,POC,NP,NE,IPLOT,              
     *AST1,FC1,SY1,HEAD,HEAD2,NPROB,IGP,EO,IPILE,MB)                            
C                                                                               
      STOP                                                                      
C***********************************************************************        
C                       READ IN GROUP LOADINGS                                  
C***********************************************************************        
300   CALL LOAD( MYSC,  MXSC,  MSC,  PSC,                               .D.        
     *           MYFC,  MXFC,  MFC,  PFC,                                       
     *           MYSF,  MXSF,  MSF,  PSF,                                       
     *           MYFF,  MXFF,  MFF,  PFF,                                       
     *           MYFCI, MXFCI, MFCI, PFCI,                                      
     * FC, POC, ICODE, PHI, ZLEN, DAY, DAX, DBY, DBX,                           
     * YI, XI, HY, HX, HEAD2, IGP, ITOP, ACT, ITIE,                             
     * ARST, ARSL, PN18, PN19, SY, PRNT, NC, NLOAD, HEAD, IVERF, DATEF,         
     * MAGY, MAGX, PCYC, PCXC, NWRITE, IPIER, DLN, PSN, EQTN, EQLN,             
     * XIS, YIS, KX, KY, SLX, SLY, E, EC, AST, EIX, EIY, PHIMIN,        .B.     
     * RNDATE, NHTIME, NPAGE )                                          .D.
C                                                                               
      IF(IPIER.EQ.1) NC = 1                                                     
      IF(IPIER.EQ.1) CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                        
      IF(IPIER.EQ.1) WRITE(6,6210)                                              
 6210 FORMAT(///,                                                               
     *7X,'WARNING:'//                                                           
     *7X,'THE RATIO OF THE COLUMN CLEAR HEIGHT TO THE MAXIMUM WIDTH'/           
     *7X,'IS LESS THAN 2.5.  THEREFORE, THE SECTION MAY BE A PIER WALL'/        
     *7X,'AND SHOULD BE DESIGNED AS A COLUMN ONLY IN THE LONGITUDINAL'/         
     *7X,'DIRECTION AND AS A SHEAR WALL IN THE TRANSVERSE DIRECTION'/           
     *7X,'(CODE 1.4.11).'//                                                     
     *7X,'THE TRANSVERE MOMENTS ARE SET EQUAL TO ZERO.'/)                       
                                                                                
C***********************************************************************        
C                   GROUP LOADING ITERATION                                     
C***********************************************************************        
  310 DO 340 K=1,NL                                                             
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 340         
      IF(MXFC(K).GT.0.) ANG = DATAN2(MYFC(K),MXFC(K))                           
      IF(MXFC(K).EQ.0.) ANG = 3.14159265359/2.                                  
      IF(ANG.EQ.0) ANG = .00000001                                              
      RE(1) = ANG*180./3.14159265359                                            
      RP(1) = PFC(K) / POC                                                      
C                                                                               
      CALL MOMENT (PU, PUX, PUY, RDRP, RDRE, LNODE, X, Y, NLOOP,                
     * NBAR, XS, YS, AS, IAS, RP(1), RE(1), SY, FC, EO, E,                      
     * ACT, AST, ITRN, NITRN, TOLP, TOLE, AC, ACX, ACY)                         
      P(1,1)   = PU                                                             
      UMX(1,1) = PUX                                                            
      UMY(1,1) = PUY                                                            
      UM(1,1)  = DSQRT(PUX*PUX+PUY*PUY)                                         
C***********************************************************************        
C                OUTPUT FOR COLUMN CHECK - IGP=2                                
C***********************************************************************        
      MYFC(K) = MYFC(K) * PHI(K)                                                
      MXFC(K) = MXFC(K) * PHI(K)                                                
      MFC(K)  = MFC(K)  * PHI(K)                                                
      PFC(K)  = PFC(K)  * PHI(K)                                                
      UM(1,1) = UM(1,1) * PHI(K)                                                
C                                                                               
      IF(K.EQ.1)  CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                           
      IF(K.EQ.1)  WRITE(6,6220) ZLEN,FC1,SY1,RATI1,AST1                         
 6220 FORMAT(///7X,                                                             
     *   '****************************************************'/                
     *7X,'* FACTORED LOADS FOR COLUMN DESIGN CHECK  (KIP-FT) *'/                
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'APPLIED FACTORED MOMENTS ARE MAGNIFIED FOR SLENDERNESS IN ',          
     *   'ACCORDANCE WITH '/                                                    
     *7X,'"AASHTO" (ART 8.16.5)'/                                       .C.     
     *7X,'LENGTH = ',F6.2,' FT    FC  = ',F6.2,' KSI      FY  = ',F5.2,         
     *   ' KSI'/                                                                
     *7X,'STEEL  = ',F6.2,' %     AST = ',F6.2,' SQ IN '//                      
     *7X,'            :------ APPLIED FACTORED ------: CAPACITY'/               
     *7X,'GROUP CASE  TRANS     LONG     COMB    AXIAL (PHI*MN)  PHI',          
     *   '  RATIO'/                                                             
     *7X,'              MY       MX        M        P       MU      ',          
     *   '   MU/M'/)                                                            
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 340         
C                                                                               
      IF(K.EQ.20) WRITE(6,6230)                                                 
 6230 FORMAT(///7X,                                                             
     *   '*****************************************************'/               
     *7X,'* ARBITRARY LOADS FOR COLUMN DESIGN CHECK  (KIP-FT) *'/               
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'ARBITRARY APPLIED FACTORED MOMENTS ARE NOT MAGNIFIED ',               
     *   'FOR SLENDERNESS.'//                                                   
     *7X,'            :----- APPLIED FACTORED -------:',                        
     *   'CAPACITY'/                                                            
     *7X,'GROUP CASE  TRANS     LONG     COMB    AXIAL (PHI*MN)  PHI',          
     *   ' RATIO'/                                                              
     *7X,'              MY       MX        M        P       MU      ',          
     *   '  MU/M'/)                                                             
C                                                                               
      IF(RDRP.LE.TOLP.AND.RDRE.LE.TOLE) GO TO 320                               
      WRITE(6,6240) GR(K),CA(K),MYFC(K),MXFC(K),MFC(K),PFC(K),UM(1,1),          
     * PHI(K)                                                                   
 6240 FORMAT(9X,A3,3X,A1,5F9.0,1X,F4.2/                                         
     *18X,'** UNABLE TO DETERMINE MOMENT CAPACITY FOR THIS LOAD **'/            
     *7X,'LOAD IGNORED - CALL TERRY BEEBE 6-0702'/)                     .A.     
      GO TO 340                                                                 
C                                                                               
  320 RATMN=0.0                                                                 
      IF(MFC(K).GT.0) RATMN=UM(1,1)/MFC(K)                                      
C                                                                               
      IF(MFC(K).GT.UM(1,1)) GO TO 330                                           
      WRITE(6,6250) GR(K),CA(K),MYFC(K),MXFC(K),MFC(K),PFC(K),UM(1,1),          
     *              PHI(K),RATMN                                                
 6250 FORMAT(9X,A3,3X,A1,5F9.0,1X,F4.2,1X,F5.2,' OK')                           
      GO TO 340                                                                 
  330 WRITE(6,6270) GR(K),CA(K),MYFC(K),MXFC(K),MFC(K),PFC(K),UM(1,1),          
     *              PHI(K),RATMN                                                
 6270 FORMAT(9X,A3,3X,A1,5F9.0,1X,F4.2,1X,F5.2,' NG**')                         
  340 CONTINUE                                                                  
C                                                                               
      WRITE(6,6280)                                                             
 6280 FORMAT(/                                                                  
     *7X,'NOTE: THE AXIAL (ARS) UNREDUCED SEISMIC LOADS (P) ARE NOT',           
     *' DIVIDED'/                                                               
     *7X,'BY THE DUCTILITY FACTOR Z'/)                                          
C***********************************************************************        
C           MOMENT MAGNIFICATION AND BUCKLING CALCULATIONS                      
C***********************************************************************        
      IF(MYFC(1).EQ.0.0.AND.MXFC(1).EQ.0.0.AND.PFC(1).EQ.0.0) GO TO 355         
C                                                                               
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      IF(ZLEN.EQ.0) WRITE(6,6285)                                               
 6285 FORMAT(//7X,'NOTE: THE COLUMN LENGTH IS EQUAL TO ZERO, THEREFORE'/        
     *7X,'NO MOMENT MAGNIFICATION AND BUCKLING CALCULATIONS ARE MADE')          
      IF(ZLEN.EQ.0) GO TO 355                                                   
C                                                                               
      WRITE(6,6290) KY, KX, SLY, SLX, YIS, XIS                                  
 6290 FORMAT(/7X,                                                               
     *   '**************************************************'/                  
     *7X,'* MOMENT MAGNIFICATION AND BUCKLING CALCULATIONS *'/                  
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'REFERENCE: "AASHTO" (ART',                                    .C.     
     *   ' 8.16.5)'/                                                            
     *7X,'          (COLUMN ASSUMED TO BE UNBRACED AGAINST SIDESWAY.)'//        
     *7X,'MAGY   = MOMENT MAGNIFACTION FACTOR ABOUT Y-AXIS'/                    
     *7X,'MAGX   = MOMENT MAGNIFACTION FACTOR ABOUT X-AXIS'//                   
     *7X,'PCY    = CRITICAL BUCKLING LOAD ABOUT Y-AXIS'/                        
     *7X,'PCX    = CRITICAL BUCKLING LOAD ABOUT X-AXIS'//                       
     *7X,'KY     = EFFECTIVE LENGTH FACTOR ABOUT Y-AXIS = ',F6.2,/      .C.     
     *7X,'KX     = EFFECTIVE LENGTH FACTOR ABOUT X-AXIS = ',F6.2,//     .C.     
     *7X,'KY*L/R = SLENDERNESS RATIO ABOUT Y-AXIS       = ',F4.0/       .C.     
     *7X,'KX*L/R = SLENDERNESS RATIO ABOUT X-AXIS       = ',F4.0,//     .C.     
     *7X,'IYS    = STEEL MOMENT OF INERTIA ABOUT Y-AXIS = ',F8.4,       .D.     
     *   ' FT**4'/                                                              
     *7X,'IXS    = STEEL MOMENT OF INERTIA ABOUT X-AXIS = ',F8.4,       .D.     
     *   ' FT**4')                                                              
C                                                                               
      WRITE(6,6291)                                                             
 6291 FORMAT(//7X,                                                              
     *   '             MOMENT             CRACKED         ',                    
     *   '     CRITICAL'/                                                       
     *7X '       :-MAGNIFICATION-:--TRANSFORMED SECTION--:',                    
     *   '-----BUCKLING----:  AXIAL'/                                           
     *7X,'GR  CA  TRAN  LONG  COMB        E*IY        E*IX',                    
     *   '     TRAN     LONG   LOAD'/                                           
     *7X,'        MAGY  MAGX   MAG                        ',                    
     *   '      PCY      PCX      P')                                           
C                                                                               
C                                                                               
      DO 350 K = 1,19                                                           
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 350         
      UMAG = DSQRT( (MYFC(K)/MAGY(K))**2 + (MXFC(K)/MAGX(K))**2 )               
      IF(UMAG.GT.0.0) MAG  = MFC(K)/UMAG                                        
      IF(UMAG.EQ.0.0) MAG  = 1.0                                                
C                                                                               
      BD = .75                                                                  
      IF(K.EQ.3.OR.K.EQ.6.OR.K.EQ.10.OR.K.EQ.13.OR.K.GE.17) BD = 1.00           
      IF(ITOP.EQ.0) PP = PFC(K) - (1.3 * BD * ACT * ZLEN * .150)                
      IF(ITOP.EQ.1) PP = PFC(K)                                                 
C                                                                               
      WRITE(6,6300) GR(K), CA(K), MAGY(K), MAGX(K), MAG, EIY(K), EIX(K),        
     *              PCYC(K), PCXC(K), PP                                        
 6300 FORMAT(7X,A3,1X,A1,1X,3F6.3,2F12.0,2F9.0,F8.0)                            
C                                                                               
      IF(ITIE.EQ.0) PN = POC * PHI(K) * .85                                     
      IF(ITIE.EQ.1) PN = POC * PHI(K) * .80                                     
      IF(ITIE.EQ.0.AND.PFC(K).GT.PN) WRITE(6,6301) PFC(K),PN                    
      IF(ITIE.EQ.1.AND.PFC(K).GT.PN) WRITE(6,6302) PFC(K),PN                    
 6301 FORMAT(7X,                                                                
     *   '*** ERROR:  P > .85*PHI*PO  ',F10.0,                                  
     *   ' > ',F10.0/)                                                          
C                                                                               
 6302 FORMAT(7X,                                                                
     *   '*** ERROR:  P > .80*PHI*PO  ',F10.0,                                  
     *   ' > ',F10.0/)                                                          
C                                                                               
      IF(PP.GE.PCYC(K)) WRITE(6,6303)                                           
      IF(PP.GE.PCXC(K)) WRITE(6,6304)                                           
 6303 FORMAT(7X,                                                                
     *   '*** ERROR: AXIAL LOAD (P) IS GREATER THAN ',                          
     *   'CRITICAL BUCKLING LOAD (PCY).'/)                                      
C                                                                               
 6304 FORMAT(7X,                                                                
     *   '*** ERROR: AXIAL LOAD (P) IS GREATER THAN ',                          
     *   'CRITICAL BUCKLING LOAD (PCX).'/)                                      
  350 CONTINUE                                                                  
C                                                                               
      IF(SLX.GT.100.0) WRITE(6,6293) SLX                                        
 6293 FORMAT(/7X,'*** WARNING KL/R = ',F4.0,' ABOUT THE X-AXIS ',               
     *   'IS GREATER THAN 100.'/                                                
     *11X,'REDESIGN THE COLUMN WITH A LARGER CROSS-SECTION OR'/                 
     *11X,'PERFORM A SPECIAL SECOND ORDER ANALYSIS AS DEFINED '/                
     *11X,'IN ART 8.16.5.1.1 OF AASHTO.')                                       
C                                                                               
      IF(SLY.GT.100.0) WRITE(6,6294) SLY                                        
 6294 FORMAT(/7X,'*** WARNING KL/R = ',F4.0,' ABOUT THE Y-AXIS ',               
     *   'IS GREATER THAN 100.'/                                                
     *11X,'REDESIGN THE COLUMN WITH A LARGER CROSS-SECTION OR'/                 
     *11X,'PERFORM A SPECIAL SECOND ORDER ANALYSIS AS DEFINED '/                
     *11X,'IN ART 8.16.5.1.1 OF AASHTO.')                                       
C                                                                               
C                                                                               
C***********************************************************************        
C               CHECK COLUMN AT SERVICE LEVEL                                   
C***********************************************************************        
  355 CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      CALL STRESS(X, Y, MNODE, XS, YS, AS, NBAR,                                
     * MYSC, MXSC, MSC, PSC,                                                    
     * HX, HY, STOTAL, EC, RM)                                                  
      IF(ITOP.EQ.1.AND.DBX.GT.0.98.AND.IPIER.EQ.1)  GO TO 356                   
      IF(ITOP.EQ.1.AND.DBY.LE.0.98.AND.DBX.LE.0.98) GO TO 375                   
      IF(ITOP.EQ.1.AND.DBY.GT.0.98.AND.DBX.LE.0.98) GO TO 375                   
      IF(ITOP.EQ.1.AND.DBY.LT.0.98.AND.DBX.GT.0.98) GO TO 375                   
C***********************************************************************        
C           PRINT SERVICE  LOADS FOR FOOTING DESIGN                             
C***********************************************************************        
  356 CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      WRITE(6,6310)                                                             
 6310 FORMAT(///7X,                                                             
     *   '***************************************************'/                 
     *7X,'* SERVICE LOADS FOR FOOTING DESIGN (UNITS KIP-FT) *'/                 
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'GROUP   CASE       TRANS     LONG       COMB     AXIAL'/              
     *7X,'                     MY       MX          M         P'/)              
C                                                                               
      DO 360 K=1,21                                                             
      IF(K.GE.18.AND.K.LE.19) GO TO 360                                         
      IF(MYSF(K).EQ.0.0.AND.MXSF(K).EQ.0.0.AND.PSF(K).EQ.0.0) GO TO 360         
      IF(K.GE.4.AND.K.LE.6)   GO TO 360                                         
      WRITE(6,6320) GR(K), CA(K), MYSF(K), MXSF(K), MSF(K), PSF(K)              
 6320 FORMAT(9X,A3,6X,A1,3X,4F10.0)                                             
  360 CONTINUE                                                                  
C***********************************************************************        
C           PRINT FACTORED LOADS FOR FOOTING DESIGN                             
C***********************************************************************        
      WRITE(6,6330)                                                             
 6330 FORMAT(///7X,                                                             
     *   '****************************************************'/                
     *7X,'* FACTORED LOADS FOR FOOTING DESIGN (UNITS KIP-FT) *'/                
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'GROUP   CASE       TRANS     LONG       COMB     AXIAL'/              
     *7X,'                     MY       MX          M         P'/)              
C                                                                               
      DO 370 K=1,21                                                             
      IF(K.GE.18.AND.K.LE.19) GO TO 370                                         
      IF(MYFF(K).EQ.0.0.AND.MXFF(K).EQ.0.0.AND.PFF(K).EQ.0.0) GO TO 370         
      WRITE(6,6320) GR(K), CA(K), MYFF(K), MXFF(K), MFF(K), PFF(K)              
  370 CONTINUE                                                                  
C                                                                               
C***********************************************************************        
C                  SET THE ANGLE VALUES  - RE(J)                                
C***********************************************************************        
  375 POC=.85*FC*(ACT-AST)+AST*SY                                               
      IF(IPIER.EQ.1) GO TO 385                                                  
      IF(ICON.GE.6.AND.IPLOT.LT.4) GO TO 380                                    
      NE=7                                                                      
      RE(1)=89.999                                                              
      RE(2)=75.0                                                                
      RE(3)=60.0                                                                
      RE(4)=45.0                                                                
      RE(5)=30.0                                                                
      RE(6)=15.0                                                                
      RE(7)=  .001                                                              
      GO TO 390                                                                 
C***********************************************************************        
C         CIRCULAR CROSS-SECTIONS ONLY - IPLOT=1,2,3  ICON=6,8,9                
C***********************************************************************        
  380 NE=1                                                                      
      RE(1)=89.999                                                              
      GO TO 390                                                                 
  385 NE=1                                                                      
      RE(1)=0.0                                                                 
C***********************************************************************        
C                  SET THE P/POC VALUES  - RP(I)                                
C***********************************************************************        
  390 NP=10                                                                     
      RP(1)=.60                                                                 
      RP(2)=.50                                                                 
      RP(3)=.40                                                                 
      RP(4)=.30                                                                 
      RP(5)=.20                                                                 
      RP(6)=.10                                                                 
      RP(7)= 0.                                                                 
      RP(8)=-.05                                                                
      RP(9)=-.10                                                                
C***********************************************************************        
C                  INSERT DEAD LOAD   P/POC VALUE                               
C***********************************************************************        
      DLR=DLN/POC                                                               
      M=0                                                                       
      DO 400 I=1,NP                                                             
      IF(RP(I).GT.DLR) GO TO 400                                                
      M=I                                                                       
      IDL = I                                                                   
      GO TO 410                                                                 
  400 CONTINUE                                                                  
C                                                                               
      IF(M.EQ.0) GO TO 430                                                      
  410 M1=NP-M                                                                   
      DO 420 JP=1,M1                                                            
      RP(11-JP)=RP(10-JP)                                                       
  420 CONTINUE                                                                  
      RP(M)=DLR                                                                 
C                                                                               
C***********************************************************************        
C             FIND PROBABLE MOMENTS - (1.3 X NONMINAL)                          
C***********************************************************************        
  430 DO 450 J = 1,NE                                                           
      DO 440 I = 1,NP                                                           
      P(I,J)   = 10.E15                                                         
      UMX(I,J) = 10.E15                                                         
      UMY(I,J) = 10.E15                                                         
      UM(I,J)  = 10.E15                                                         
C                                                                               
      CALL MOMENT (PU, PUX, PUY, RDRP, RDRE, LNODE, X, Y, NLOOP,                
     * NBAR, XS, YS, AS, IAS, RP(I), RE(J), SY, FC, EO, E,                      
     * ACT, AST, ITRN, NITRN, TOLP, TOLE, AC, ACX, ACY)                         
C                                                                               
      IF(RDRP.GT.TOLP.AND.RDRE.GT.TOLE) GO TO 435                               
      FAC = 1.3                                                                 
      IF(EO.GT.0.0031) FAC = 1.0                                                
      P(I,J)   = PU                                                             
      UMX(I,J) = FAC * PUX                                                      
      UMY(I,J) = FAC * PUY                                                      
      UM(I,J)  = DSQRT(UMX(I,J)**2+UMY(I,J)**2)                                 
C                                                                               
  435 UM1(I,J)=UM(I,J)                                                          
      P1(I,J) =P(I,J)                                                           
C                                                                               
  440 CONTINUE                                                                  
  450 CONTINUE                                                                  
      IF(IBENT.EQ.1) GO TO 480                                                  
C                                                                               
      CALL PBAL(P,UM,NP,NE,POC,IPB)                                             
C **********************************************************************        
C                      PRINT PROBABLE MOMENTS - IGP=2                           
C **********************************************************************        
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      IF(EO.LT.0.0031) WRITE(6,6340)                                            
 6340 FORMAT(///7X,                                                             
     *   '***********************************************************'/         
     *7X,'* PROBABLE PLASTIC MOMENT (1.3 X NOMINAL MOMENT) (KIP-FT) *'/         
     *7X,'*****************************************************',               
     *   '********************')                                                
C                                                                               
      IF(EO.GT.0.0031) WRITE(6,6350)                                            
 6350 FORMAT(///7X,                                                             
     *   '*****************************************************',               
     *   '*******'/                                                             
     *7X,'* PROBABLE MOMENT (USING INPUT MATERIAL PROPERTIES)  ',               
     *'(KIP-FT) *'/                                                             
     *7X,'*****************************************************',               
     *   '********************')                                                
C                                                                               
      WRITE(6,6360) FC1,SY1,EO,AST1                                             
 6360 FORMAT(//,                                                                
C    *7X,'CODE (8.16.4.4) AND MEMO TO DESIGNERS 15-10'//                .C.     
     *7X,'FC  = ',F6.2,' KSI              FY  = ',F6.2,' KSI'/                  
     *7X,'EO  = ',F6.4,' IN/IN            AST = ',F6.2,' SQ IN'//               
     *7X,'     ANGLE       TRANS      LONG      COMB     AXIAL',                
     *   '  P-BALANCE'/                                                         
     *7X,'                  MPY       MPX        MP        P  ',                
     *   '   (APPROX)')                                                         
C                                                                               
      DO 470 J=1,NE                                                             
      IF(J.EQ.4) CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                            
      IF(J.EQ.4) WRITE(6,6370) AST1                                             
 6370 FORMAT (///7X,                                                            
     *   '******************************************',                          
     *   '*********************'/                                               
     *7X,'* PROBABLE PLASTIC MOMENT VALUES  (KIP-FT)',                          
     *   ' AST = ',F6.2,' SQ IN *'/                                             
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'     ANGLE       TRANS      LONG      COMB     AXIAL',                
     *   '  P-BALANCE'/                                                         
     *7X,'                  MPY       MPX        MP        PN ',                
     *   '  (APPROX)')                                                          
C                                                                               
      DO 460 I=1,NP                                                             
      IF(I.GE.2) GO TO 455                                                      
      WRITE(6,6380) RE(J),UMY(I,J),UMX(I,J),UM(I,J),P(I,J),                     
     *              IPB(J)                                                      
 6380 FORMAT (/,8X,F10.1,2X,4F10.0,I10)                                         
      GO TO 460                                                                 
C                                                                               
  455 WRITE(6,6381)       UMY(I,J),UMX(I,J),UM(I,J),P(I,J)                      
 6381 FORMAT (20X,4F10.0)                                                       
C                                                                               
  460 CONTINUE                                                                  
  470 CONTINUE                                                                  
C***********************************************************************        
C                 PLASTIC HINGING ANALYSIS                                      
C***********************************************************************        
  480 NP = 10                                                                   
C     IF(DLN.EQ.0) WRITE(6,6382)                                                
C6382 FORMAT(//7X,'** NOTE: THE AXIAL DEAD LOAD EQUALS ZERO. ',                 
C    *   'THEREFORE,'/                                                          
C    *7X,'NO PLASTIC HINGING ANALYSIS IS PERFORMED.')                           
C     IF(DLN.EQ.0) GO TO 500                                                    
C                                                                               
C     IF(ZLEN.EQ.0) WRITE(6,6383)                                               
C6383 FORMAT(//7X,'** NOTE: THE COLUMN LENGTH EQUALS ZERO. ',                   
C    *   'THEREFORE,'/                                                          
C    *7X,'NO PLASTIC HINGING ANALYSIS IS PERFORMED.')                           
C     IF(ZLEN.EQ.0.OR.NC.EQ.0) WRITE(2,6387)                                    
C6387 FORMAT(//'    ')                                                          
C     IF(ZLEN.EQ.0.AND.NC.GT.1) WRITE(2,6388)                                   
C6388 FORMAT(/////'    ')                                                       
C     IF(ZLEN.EQ.0) GO TO 500                                                   
C                                                                               
C     IF(NC.GT.1.AND.CSP.EQ.0) WRITE(6,6384)                                    
C6384 FORMAT(//7X,'** NOTE: THE COLUMN SPACING EQUALS ZERO. ',                  
C    *   'THEREFORE,'/                                                          
C    *7X,'NO PLASTIC HINGING ANALYSIS IS PERFORMED.')                           
C     IF(NC.GT.1.AND.CSP.EQ.0) GO TO 500                                        
C                                                                               
C     IF(NC.EQ.0) WRITE(6,6385)                                                 
C6385 FORMAT(//7X,'** NOTE: THE NUMBER OF COLUMNS EQUALS ZERO.'/                
C    *7X,'THEREFORE, NO PLASTIC HINGING ANALYSIS IS PERFORMED.')                
C     IF(NC.EQ.0) GO TO 500                                                     
C                                                                               
C     IF(IREBAR.EQ.2.AND.DSO.EQ.0) WRITE(6,6386)                                
C     IF(IREBAR.EQ.4.AND.DSO.EQ.0) WRITE(6,6386)                                
C6386 FORMAT(//7X,'** NOTE: THE OUT TO OUT DISTANCE (DIAMETER) OF THE'/         
C    *7X,'SPIRAL EQUALS ZERO.  THEREFORE, NO PLASTIC HINGING'/                  
C    *7X,'ANALYSIS IS PERFORMED.')                                              
C     IF(IREBAR.EQ.2.AND.DSO.EQ.0) GO TO 500                                    
C     IF(IREBAR.EQ.4.AND.DSO.EQ.0) GO TO 500                                    
C                                                                               
C     IF(NC.GE.1) CALL BENT ( NPAGE,HEAD,RNDATE,                                
C    *NHTIME,NC, CSP, ZLEN, CGS, NP, DLN, P1, UM1, ICON, DBY, DBX,              
C    *DAY, DAX, ACT, FC, IREBAR, SPACE, RS, DSO, NSLOOP, HX, HY, ITOP,          
C    *ARST, ARSL, PN18, PN19, PRNT, IDL, ITIE, VMAXP, UMAXP, PMAXP,             
C    *VMINP, PMINP, BDIA, IBAR, IPIER)                                          
C **********************************************************************        
C                       SHEAR KEY DESIGN                                        
C **********************************************************************        
C 490 IF(ITOP.EQ.1.AND.DBX.GT.0.98.AND.DBY.GT.0.98) GO TO 495                   
C     IF(ITOP.EQ.1.AND.DBX.LE.0.98.AND.DBY.GT.0.98) GO TO 495                   
C     IF(ITOP.EQ.1.AND.DBX.GT.0.98.AND.DBY.LE.0.98) GO TO 495                   
C     GO TO 500                                                                 
C 495 CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
C     CALL KEY (MFF, PFF, VMAXP, UMAXP, PMAXP, ARST, ARSL, EQTN, EQLN,          
C    * FC, SY, ZLEN, VMINP, PMINP, DLN, PSN, DBY, DBX, MYFF, MXFF,              
C    * ACT, ITOP)                                                               
C **********************************************************************        
C                         CALL PLOTS                                            
C **********************************************************************        
  500 NP = 10                                                                   
      IF(IBENT.EQ.0.AND.IPLOT.GE.1)CALL PLOTS(RE,RP,P,UMX,UMY,UM,POC,NP,        
     *NE,IPLOT,AST1,FC1,SY1,HEAD,HEAD2,NPROB,IGP,EO,IPILE,MB)                   
      IBENT = 1                                                                 
C***********************************************************************        
C                RETURN AND CHECK FOR ANOTHER SET OF LOADS                      
C***********************************************************************        
      GO TO 300                                                                 
C                                                                               
C***********************************************************************        
C                      DESIGN  IGP=1                                            
C***********************************************************************        
 510  NCOUNT = 0                                                                
      MCOUNT = 1                                                                
      LMAX   = 0                                                                
      XIS = 0.0                                                                 
      YIS = 0.0                                                                 
      DO 515 I = 1,NBAR                                                         
      IS = IAS(I)                                                               
      XIS = XIS + AS(IS) * YS(I)**2                                             
      YIS = YIS + AS(IS) * XS(I)**2                                             
  515 CONTINUE                                                                  
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      CALL LOAD( MYSC,  MXSC,  MSC,  PSC,                                       
     *           MYFC,  MXFC,  MFC,  PFC,                                       
     *           MYSF,  MXSF,  MSF,  PSF,                                       
     *           MYFF,  MXFF,  MFF,  PFF,                                       
     *           MYFCI, MXFCI, MFCI, PFCI,                                      
     * FC, POC, ICODE, PHI, ZLEN, DAY, DAX, DBY, DBX,                           
     * YI, XI, HY, HX, HEAD2, IGP, ITOP, ACT, ITIE,                             
     * ARST, ARSL, PN18, PN19, SY, PRNT, NC, NLOAD, HEAD, IVERS, DATEV,         
     * MAGY, MAGX, PCYC, PCXC, NWRITE, IPIER, DLN, PSN, EQTN, EQLN,             
     * XIS, YIS, KX, KY, SLX, SLY, E, EC, AST, EIX, EIY, PHIMIN,        .D.
     * RNDATE, NHTIME, NPAGE )                                          .D.
C                                                                               
      IF(IPIER.EQ.1) CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                        
      IF(IPIER.EQ.1) WRITE(6,6210)                                              
C***********************************************************************        
C                   GROUP LOADING ITERATION                                     
C***********************************************************************        
 520  RATM = 0.0                                                                
      NCODE  = 0                                                                
      DO 700 K=1,NL                                                             
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 700         
      IF(MXFC(K).GT.0.) ANG = DATAN2(MYFC(K),MXFC(K))                           
      IF(MXFC(K).EQ.0.) ANG = 3.14159265359/2.                                  
      IF(ANG.EQ.0) ANG = .00000001                                              
      RE(1) = ANG*180./3.14159265359                                            
      RP(1) = PFC(K) / POC                                                      
C                                                                               
      RAT2 = AST/ACT                                                            
      IERR(K)  = 0                                                              
      IBOMB(K) = 0                                                              
      ITER     = 0                                                              
  525 ITER = ITER + 1                                                           
      IF(ITER.GT.25) GO TO 640                                                  
C                                                                               
      CALL MOMENT (PU, PUX, PUY, RDRP, RDRE, LNODE, X, Y, NLOOP,                
     * NBAR, XS, YS, AS, IAS, RP(1), RE(1), SY, FC, EO, E,                      
     * ACT, AST, ITRN, NITRN, TOLP, TOLE, AC, ACX, ACY)                         
C                                                                               
      P(1,1)   = PU                                                             
      UMX(1,1) = PUX                                                            
      UMY(1,1) = PUY                                                            
      UM(1,1)  = DSQRT(PUX*PUX+PUY*PUY)                                         
      UMR(K)=UM(1,1)                                                            
C                                                                               
C     WRITE(6,7000)K,NCOUNT,ITRN,ITER,RE(1), MFC(K),UM(1,1),RAT0,               
C    *  RAT1,RAT2,RATM                                                          
C7000 FORMAT(/' K,NCOUNT,ITRN,ITER,RE(1), MFC(K),UM(1,1),RAT0,',                
C    *  'RAT1,RAT2,RATM'/                                                       
C    *4I4,F9.5,2F9.1,4F9.5)                                                     
C                                                                               
      IF(RDRP.LE.TOLP.AND.RDRE.LE.TOLE) GO TO 530                               
      IBOMB(K) = 1                                                              
      AST=ACT*RATM                                                              
      SAREA=AST/NBAR                                                            
      DO 529 I=1,NAS                                                            
      AS(I)=SAREA                                                               
      IAS(I) = 1                                                                
  529 CONTINUE                                                                  
      GO TO 700                                                                 
C                                                                               
                                                                                
  530 IF(MFC(K).GE.(0.99*UM(1,1)).AND.MFC(K).LE.UM(1,1)) GO TO 630              
C                                                                               
      IF(NCODE.GT.0.AND.ITER.EQ.1.AND.MFC(K).LE.UM(1,1))  GO TO 630             
C                                                                               
      IF(ITER.EQ.2) GO TO 550                                                   
      IF(ITER.GE.3) GO TO 560                                                   
C***********************************************************************        
C               ITER = 1  (SET MAX OR MIN STEEL RATIO)                          
C***********************************************************************        
      RAT0=RAT2                                                                 
C     IF(NCOUNT.GE.1) RAT0 = RATM                                               
      IF(MFC(K).GT.UM(1,1)) GO TO 540                                           
      RAT1=RMIN1                                                                
      RAT2=RMIN1                                                                
      NCODE=1                                                                   
      GO TO  610                                                                
  540 RAT1=RMAX1                                                                
      RAT2=RMAX1                                                                
      NCODE=2                                                                   
      GO TO 610                                                                 
C***********************************************************************        
C               ITER = 2  (CHECK MAX OR MIN STEEL RATIO)                        
C***********************************************************************        
  550 IF(RAT2.EQ.RMAX1.AND.MFC(K).GT.UM(1,1)) GO TO 552                         
      GO TO 555                                                                 
C                                                                               
  552 WRITE(6,6390) GR(K),CA(K),RMAX                                            
 6390 FORMAT(//7X,' *** ERROR:'/                                                
     *7X,'LOAD GROUP ',A3,' CASE ',A1,' REQUIRES MORE THAN ',F4.2,' %',         
     *   ' STEEL.'/                                                             
     *7X,'REDESIGN THE COLUMN WITH A LARGER CROSS-SECTION.'/                    
     *7X,'EXECUTION TERMINATED . . . ')                                         
      STOP                                                                      
C                                                                               
  555 IF(RAT2.EQ.RMIN1.AND.MFC(K).LT.UM(1,1)) GO TO  630                        
      GO TO 600                                                                 
C***********************************************************************        
C            ITER >= 3 (FIND NEW STEEL RATIO AND AREA OF STEEL)                 
C***********************************************************************        
  560 IF(NCODE.EQ.1) GO TO 580                                                  
C *** NCODE=2 ***                                                               
      IF(MFC(K).GT.UM(1,1)) GO TO 570                                           
      RAT1=RAT2                                                                 
      GO TO 600                                                                 
  570 RAT0=RAT2                                                                 
      GO TO 600                                                                 
C *** NCODE=1 ****                                                              
  580 IF(MFC(K).GT.UM(1,1)) GO TO 590                                           
      RAT0=RAT2                                                                 
      GO TO 600                                                                 
  590 RAT1=RAT2                                                                 
C                                                                               
  600 RAT2=(RAT0+RAT1)/2                                                        
C                                                                               
  610 AST=ACT*RAT2                                                              
      POC = .85*FC*(ACT-AST)+AST*SY                                             
      RP(1) = PFC(K) / POC                                                      
      SAREA=AST/NBAR                                                            
      DO 620 I=1,NAS                                                            
      AS(I)=SAREA                                                               
      IAS(I) = 1                                                                
  620 CONTINUE                                                                  
      GO TO 525                                                                 
C                                                                               
C***********************************************************************        
C                      FIND MAX STEEL RATIO                                     
C***********************************************************************        
  630 IF(RAT2.LT.RATM) GO TO 700                                                
      RATM=RAT2                                                                 
      LMAX=K                                                                    
      GO TO 700                                                                 
C                                                                               
  640 IERR(K) = 1                                                               
      AST=ACT*RATM                                                              
      SAREA=AST/NBAR                                                            
      DO 645 I=1,NAS                                                            
      AS(I)=SAREA                                                               
      IAS(I) = 1                                                                
  645 CONTINUE                                                                  
C                                                                               
  700 CONTINUE                                                                  
      IF (MCOUNT.GT.1) GO TO 725                                                
      MCOUNT = MCOUNT + 1                                                       
      IF (IGP.EQ.1) WRITE (6,6402)                                              
 6402 FORMAT(//7X,'ARBITRARY SERVICE LOADS ARE ONLY USED TO MAKE A'/            
     *7X,'WORKING STRESS ANALYSIS IN THE CHECK MODE.')                          
  725 IF (RATM.EQ.0.AND.IGP.EQ.1) STOP                                          
C 700 WRITE(6,7010) K, IBOMB(K), RDRP, RDRE                                     
C7010 FORMAT(' K, IBOMB(K), RDRP, RDRE', 2I5,2F15.8)                            
C                                                                               
      POC = .85*FC*(ACT-AST)+AST*SY                                             
      NCOUNT=NCOUNT+1                                                           
      IF(NCOUNT.LT.10) GO TO 710                                                
C                                                                               
      ASTI = RATI * ACT * 144.                                                  
      ASTR = RAT2 * ACT * 144.                                                  
                                                                                
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      WRITE(6,6400) ASTI,ASTR                                                   
 6400 FORMAT(//7X,'*** WARNING ***'//                                           
     *7X,'AFTER 10 INTERATIONS THE INITIAL STEEL AREA IS NOT'/                  
     *7X,'WITHIN 1% OR THE REQUIRED STEEL AREA.'//                              
     *7X,'INITIAL  STEEL AREA = ',F10.4,' IN**2'/                               
     *7X,'REQUIRED STEEL AREA = ',F10.4,' IN**2'//                              
     *7X,'CALL TERRY BEEBE 6-0702'/                                     .A.     
     *7X,'EXECUTION CONTINUES . . .')                                           
      GO TO 730                                                                 
C                                                                               
  710 IF(RATI.GE.0.99*RATM.AND.RATI.LE.1.09*RATM) GO TO 730                     
C     WRITE(6,8005) NCOUNT,RATI,RATM,AST                                        
C8005 FORMAT(' NCOUNT,RATI,RATM,AST ',I5,2F12.7,F10.4)                          
C                                                                               
      IF(NCOUNT.GE.3) GO TO 711                                                 
      RATI = RATM                                                               
      GO TO 713                                                                 
C                                                                               
  711 RATI=(RATM + RATI) / 2.                                                   
      AST=ACT*RATI                                                              
      POC = .85*FC*(ACT-AST)+AST*SY                                             
      SAREA=AST/NBAR                                                            
      DO 712 I=1,NAS                                                            
      AS(I)=SAREA                                                               
      IAS(I) = 1                                                                
  712 CONTINUE                                                                  
C***********************************************************************        
C              FIND MOMENT OF INERTIA OF STEEL REBARS                           
C***********************************************************************        
  713 XIS = 0.0                                                                 
      YIS = 0.0                                                                 
      DO 705 I = 1,NBAR                                                         
      IS = IAS(I)                                                               
      XIS = XIS + AS(IS) * YS(I)**2                                             
      YIS = YIS + AS(IS) * XS(I)**2                                             
  705 CONTINUE                                                                  
C***********************************************************************        
C              ADJUST LOADS FOR MOMNET MAGNIFACTION AND PHI                     
C***********************************************************************        
      DO 720 K=1,NL                                                             
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 720         
      IF(K.GE.20) GO TO 720                                                     
      MYFC(K)=MYFCI(K)                                                          
      MXFC(K)=MXFCI(K)                                                          
      PFC(K) =PFCI(K)                                                           
C                                                                               
      BD = .75                                                                  
      IF(K.EQ.3.OR.K.EQ.6.OR.K.EQ.10.OR.K.EQ.13.OR.K.GE.17) BD = 1.00           
C                                                                               
      BDY = 0.0                                                                 
      BDX = 0.0                                                                 
      IF(MYFC(K).GT.0) BDY = 1.3 * BD * MYSC(22)/MYFC(K)                        
      IF(MXFC(K).GT.0) BDX = 1.3 * BD * MXSC(22)/MXFC(K)                        
      IF(BDY.GT.1.0) BDY = 1.0                                                  
      IF(BDX.GT.1.0) BDX = 1.0                                                  
C                                                                               
      CALL ADJUST (MYFC(K), MXFC(K), PFC(K), YI, XI, HY, HX, ZLEN,              
     * DAY, DAX, DBY, DBX, POC, ITIE, FC, PHI(K), ACT, K,                       
     * MAGY(K), MAGX(K), PCYC(K), PCXC(K), XIS, YIS, E, EC, KX, KY, AST,        
     * BDX, BDY, EIX(K), EIY(K), SLX, SLY, ITOP, BD, IPIER, PHIMIN)     .B.     
C                                                                               
      MFC(K)=DSQRT(MYFC(K)**2+MXFC(K)**2)                                       
C                                                                               
C                                                                               
  720 CONTINUE                                                                  
C                                                                               
      GO TO 520                                                                 
C***********************************************************************        
C         FINAL DESIGN  -  CONTROLLING GROUP LOADING    IGP=1                   
C***********************************************************************        
  730 AST=ACT*RATM                                                              
      SAREA=AST/NBAR                                                            
      DO 731 I=1,NBAR                                                           
      AS(I)=SAREA                                                               
      IAS(I) = 1                                                                
  731 CONTINUE                                                                  
C                                                                               
      POC = .85*FC*(ACT-AST)+AST*SY                                             
C                                                                               
      DO 735 K=1,NL                                                             
C     IBOMB(K) = 0                                                              
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 735         
      IF(MXFC(K).GT.0.) ANG = DATAN2(MYFC(K),MXFC(K))                           
      IF(MXFC(K).EQ.0.) ANG = 3.14159265359/2.                                  
      IF(ANG.EQ.0) ANG = .00000001                                              
      RE(1) = ANG*180./3.14159265359                                            
      RP(1) = PFC(K) / POC                                                      
C                                                                               
      CALL MOMENT (PU, PUX, PUY, RDRP, RDRE, LNODE, X, Y, NLOOP,                
     * NBAR, XS, YS, AS, IAS, RP(1), RE(1), SY, FC, EO, E,                      
     * ACT, AST, ITRN, NITRN, TOLP, TOLE, AC, ACX, ACY)                         
C                                                                               
      P(1,1)   = PU                                                             
      UMX(1,1) = PUX                                                            
      UMY(1,1) = PUY                                                            
      UM(1,1)  = DSQRT(PUX*PUX+PUY*PUY)                                         
      UMR(K)=UM(1,1)                                                            
      IF(RDRP.GT.TOLP.OR.RDRE.GT.TOLE) IBOMB(K) = 1                             
C     WRITE(6,7010) K, IBOMB(K), RDRP, RDRE                                     
C                                                                               
C     WRITE(6,7015) K,RE(1),RP(1),MFC(K),UM(1,1),PHI(K)                         
C7015 FORMAT(' K,RE(1),RP(1),MFC(K),UM(1,1)'/I5,2F10.5,2F10.1,F10.7)            
C                                                                               
  735 CONTINUE                                                                  
C                                                                               
      RATMP=RATM*100.                                                           
      SAREA = AST/NBAR                                                          
      SAREA1=SAREA*144.                                                         
      AST1=AST*144.                                                             
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      WRITE(6,6410) ZLEN,FC1,SY1,RATMP,AST1                                     
 6410 FORMAT(///7X,                                                             
     *   '**********************************************'/                      
     *7X,'* FACTORED LOADS FOR COLUMN DESIGN  (KIP-FT) *'/                      
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'APPLIED FACTORED MOMENTS ARE MAGNIFIED FOR SLENDERNESS IN ',          
     *   'ACCORDANCE WITH '/                                                    
     *7X,'CALTRANS BRIDGE DESIGN SPECIFICATIONS. (ART 8.16.5)'//                
     *7X,'LENGTH = ',F6.2,' FT    FC  = ',F6.2,' KSI      FY  = ',F5.2,         
     *   ' KSI'/                                                                
     *7X,'STEEL  = ',F6.2,' %     AST = ',F6.2,' SQ IN'//                       
     *7X,'            :------ APPLIED FACTORED ------:',                        
     *   'CAPACITY'/                                                            
     *7X,'GROUP CASE  TRANS     LONG     COMB    AXIAL (PHI*MN)  PHI',          
     *   '  RATIO'/                                                              
     *7X,'              MY       MX        M        P       MU      ',          
     *   '   MU/M'/)                                                             
C                                                                               
      RATCON = 10000.                                                           
      DO 750 K=1,NL                                                             
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 750         
      MYFC(K) = MYFC(K) * PHI(K)                                                
      MXFC(K) = MXFC(K) * PHI(K)                                                
      MFC(K)  = MFC(K)  * PHI(K)                                                
      PFC(K)  = PFC(K)  * PHI(K)                                                
      UMR(K)  = UMR(K)  * PHI(K)                                                
      IF(K.EQ.20) WRITE(6,6420)                                                 
      IF(IBOMB(K).EQ.0.AND.IERR(K).EQ.0) GO TO 740                              
      IF(IBOMB(K).EQ.1) WRITE(6,6430)                                           
     * GR(K),CA(K),MYFC(K),MXFC(K),MFC(K),PFC(K),UMR(K),PHI(K)                  
 6430 FORMAT(9X,A3,3X,A1,5F9.0,1X,F4.2/                                         
     *18X,'** UNABLE TO DETERMINE MOMENT CAPACITY FOR THIS LOAD **'/            
     *7X,'LOAD IGNORED - CALL TERRY BEEBE 6-0702'/)                     .A.     
C                                                                               
      IF(IERR(K).EQ.1) WRITE(6,6435)                                            
     * GR(K),CA(K),MYFC(K),MXFC(K),MFC(K),PFC(K),UMR(K),PHI(K)                  
 6435 FORMAT(9X,A3,3X,A1,5F9.0,1X,F4.2/                                         
     *18X,'**   (M) NOT WITHIN 1% OF (MU) AFTER 25 INTERATIONS  **'/            
     *7X,'LOAD IGNORED - CALL TERRY BEEBE 6-0702'/)                     .A.     
      GO TO 750                                                                 
C                                                                               
  740 RATMN=0.0                                                                 
      IF(MFC(K).GT.0) RATMN=UMR(K)/MFC(K)                                       
      WRITE(6,6440) GR(K),CA(K),MYFC(K),MXFC(K),MFC(K),PFC(K),UMR(K),           
     * PHI(K),RATMN                                                             
 6440 FORMAT(9X,A3,3X,A1,5F9.0,1X,F4.2,1X,F5.2)                         .D.        
      IF(RATMN.EQ.0.0) GO TO 750                                                
      IF(RATMN.LT.RATCON) LMAX   = K                                            
      IF(RATMN.LT.RATCON) RATCON = RATMN                                        
  750 CONTINUE                                                                  
C                                                                               
 6420 FORMAT(///7X,                                                             
     *   '***********************************************'/                     
     *7X,'* ARBITRARY LOADS FOR COLUMN DESIGN  (KIP-FT) *'/                     
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'ARBITRARY APPLIED FACTORED MOMENTS ARE NOT MAGNIFIED ',               
     *   'FOR SLENDERNESS.'//                                                   
     *7X,'            :----- APPLIED FACTORED -------: CAPACITY'/               
     *7X,'GROUP CASE  TRANS     LONG     COMB    AXIAL (PHI*MN)  PHI',          
     *   ' RATIO'/                                                              
     *7X,'              MY       MX        M        P       MU      ',          
     *   '  MU/M'/)                                                             
C                                                                               
      WRITE(6,6450)                                                             
 6450 FORMAT(/                                                                  
     *7X,'NOTE: THE AXIAL (ARS) UNREDUCED SEISMIC LOADS (P) ARE NOT',           
     *' DIVIDED'/                                                               
     *7X,'BY THE DUCTILITY FACTOR Z'/)                                          
                                                                                
C                                                                               
      POC = .85*FC*(ACT-AST)+SY*AST                                             
      K=LMAX                                                                    
      TBARS=AST1/BAREA                                                          
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      WRITE(6,6460) GR(K),CA(K),POC,NBAR,RATMP,SAREA1,AST1,TBARS,BAREA          
 6460 FORMAT(///7X,                                                             
     *   '*****************'/                                                   
     *7X,'* FINAL RESULTS *'/                                                   
     *7X,'*****************************************************',               
     *   '********************'//                                               
     *7X,'CONTROLLING GROUP LOADING   ...',5X,'GROUP ',A3,' CASE ',A1/          
     *7X,'NOMINAL AXIAL LOAD STRENGTH ...',F10.0,'  KIPS'/                      
     *7X,'TOTAL NO. OF BARS INPUT     ...',5X,I5,'  BARS'/                      
     *7X,'PERCENT STEEL REQUIRED      ...',F10.2,'  PERCENT'/                   
     *7X,'ADJUSTED AREA OF EACH BAR   ...',F10.2,'  SQ. IN.'//                  
     *7X,'TOTAL AREA OF STEEL REQ.    ...',F10.2,'  SQ. IN.'/                   
     *7X,'NUMBER OF BARS REQUIRED     ...',F10.1,'  BARS AT',F5.2,              
     *   ' SQ.IN. PER BAR'//                                                    
     *7X,'** NOTE: IF THE BAR SIZE IS CHANGED, THE BAR LOCATIONS WILL'/         
     *7X,'CHANGE AND THE DESIGNER SHOULD CONSIDER ADJUSTING THE RADIUS'/        
     *7X,'OF MAIN STEEL BAR LOOP AND RE-RUN THE PROBLEM.'//                     
     *7X,'** NOTE: THE DESIGNER MUST CHECK TO ENSURE THAT THE BAR ',            
     *   'SPACING LIMITS'/                                                      
     *7X,'OF THE CODE ARE SATISFIED.')                                          
C                                                                               
      IF(RAT2.EQ.RMIN1) WRITE(6,6470) RMIN                                      
 6470 FORMAT (/7X,'** NOTE: ALL GROUP LOADS REQUIRED LESS THAN ',F4.2,          
     *' % STEEL. ')                                                             
C***********************************************************************        
C           MOMENT MAGNIFICATION AND BUCKLING CALCULATIONS                      
C***********************************************************************        
  760 IF(MYFC(1).EQ.0.0.AND.MXFC(1).EQ.0.0.AND.PFC(1).EQ.0.0) GO TO 780         
C                                                                               
      IF(ZLEN.EQ.0) WRITE(6,6475)                                               
 6475 FORMAT(/7X,'** NOTE: THE COLUMN LENGTH IS EQUAL TO ZERO. ',               
     *   'THEREFORE,'/                                                          
     *7X,'NO MOMENT MAGNIFICATION AND BUCKLING CALCULATIONS ARE MADE')          
      IF(ZLEN.EQ.0) GO TO 780                                                   
C                                                                               
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                                       
      WRITE(6,6290) KY, KX, SLY, SLX, YIS, XIS                                  
      WRITE(6,6291)                                                             
C                                                                               
      DO 770 K = 1,19                                                           
      IF(MYFC(K).EQ.0.0.AND.MXFC(K).EQ.0.0.AND.PFC(K).EQ.0.0) GO TO 770         
      UMAG = DSQRT( (MYFC(K)/MAGY(K))**2 + (MXFC(K)/MAGX(K))**2 )               
      IF(UMAG.GT.0.0) MAG  = MFC(K)/UMAG                                        
      IF(UMAG.EQ.0.0) MAG  = 1.0                                                
C                                                                               
      BD = .75                                                                  
      IF(K.EQ.3.OR.K.EQ.6.OR.K.EQ.10.OR.K.EQ.13.OR.K.GE.17) BD = 1.00           
      IF(ITOP.EQ.0) PP = PFC(K) - (1.3 * BD * ACT * ZLEN * .150)                
      IF(ITOP.EQ.1) PP = PFC(K)                                                 
C                                                                               
      WRITE(6,6300) GR(K), CA(K), MAGY(K), MAGX(K), MAG, EIY(K), EIX(K),        
     *              PCYC(K), PCXC(K), PP                                        
C                                                                               
      IF(ITIE.EQ.0) PN = POC * PHI(K) * .85                                     
      IF(ITIE.EQ.1) PN = POC * PHI(K) * .80                                     
      IF(ITIE.EQ.0.AND.PFC(K).GT.PN) WRITE(6,6301) PFC(K),PN                    
      IF(ITIE.EQ.1.AND.PFC(K).GT.PN) WRITE(6,6302) PFC(K),PN                    
C                                                                               
      IF(PP.GE.PCYC(K)) WRITE(6,6303)                                           
      IF(PP.GE.PCXC(K)) WRITE(6,6304)                                           
  770 CONTINUE                                                                  
      IF(SLX.GT.100.0) WRITE(6,6293) SLX                                        
      IF(SLY.GT.100.0) WRITE(6,6294) SLY                                        
C***********************************************************************        
C                RETURN AND CHECK FOR ANOTHER SET OF LOADS                      
C***********************************************************************        
  780 GO TO 510                                                                 
C                                                                               
  999 STOP                                                                      
      END                                                                       
      SUBROUTINE PARM(DATE, TIME)                                       .D.
      CHARACTER*8 TIME                                                  .D.
      INTEGER*4 OUNIT,ERUNIT,TUNIT                                      .D.
      CHARACTER*1 QUES                                                  .D.
      CHARACTER*11 DATE                                                 .D.
      CHARACTER*16 FLNM1, FLNM2                                         .D.
      CHARACTER*23 DATIME                                               .D.
C     OUNIT = (6 FOR FILE OUTPUT)                                       .D.
C     ERUNIT = (* FOR SCREEN OUTPUT)                                    .D.
C     INUNIT = (5 FOR FILE INPUT)                                       .D.
C                                                                       .D.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      .D.
C                                                                       .D.
C                                                                       .D.
      CALL LIB$DATE_TIME (DATIME)                                       .D.
      TIME = DATIME(13:20)                                              .D.
      DATE = DATIME(1:11)                                               .D.
C                                                                       .D.
C                                                                       .D.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      .D.
      OUNIT=6                                                           .D.
      OPUNIT=8                                                          .D.
      INUNIT=5                                                          .D.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   .D.
10    WRITE(*,FMT='(//,'' Do you want a plot ?'',/)')                   .D.
      READ(*,ERR=10,FMT='(A)')QUES                                      .D.
      IF(QUES.NE.'Y'.AND.QUES.NE.'y')GO TO 70                           .D.
20    WRITE(*,FMT='(//,'' Enter plot file name'',/)')                   .D.
      READ(*,ERR=20,FMT='(A)')FLNM1                                     .D.
      DO 30 I=1,16                                                      .D.
      IF(FLNM1(I:I).EQ.'.')GO TO 50                                     .D.
30    CONTINUE                                                          .D.
      DO 40 I=1,16                                                      .D.
      IF(FLNM1(I:I).EQ.' ')THEN                                         .D.
      IE = I+3                                                          .D.
      FLNM1(I:IE) = '.plt'                                              .D.
      GO TO 50                                                          .D.
      END IF                                                            .D.
40    CONTINUE                                                          .D.
50    WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1           .D.
      OPEN (OPUNIT,FILE=FLNM1)                                          .D.    
70    WRITE(*,FMT='(//,'' Enter input file name'',/)')                  .D.
      READ(*,ERR=70,FMT='(A)')FLNM1                                     .D.
      DO 80 I=1,16                                                      .D.
      IF(FLNM1(I:I).EQ.'.')GO TO 100                                    .D.
80    CONTINUE                                                          .D.
      DO 90 I=1,16                                                      .D.
      IF(FLNM1(I:I).EQ.' ')THEN                                         .D.
      IE = I+3                                                          .D.
      FLNM1(I:IE) = '.dat'                                              .D.
      GO TO 100                                                         .D.
      END IF                                                            .D.
90    CONTINUE                                                          .D.
100   WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1           .D.
110   WRITE(*,FMT='(//,'' Enter output file name'',/)')                 .D.
      READ(*,ERR=110,FMT='(A)')FLNM2                                    .D.
      DO 120 I=1,16                                                     .D.
      IF(FLNM2(I:I).EQ.'.')GO TO 140                                    .D.
120   CONTINUE                                                          .D.
      DO 130 I=1,16                                                     .D.
      IF(FLNM2(I:I).EQ.' ')THEN                                         .D.
      IE = I+3                                                          .D.
      FLNM2(I:IE) = '.out'                                              .D.
      GO TO 140                                                         .D.
      END IF                                                            .D.
130   CONTINUE                                                          .D.
140   WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2        .D.
      OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                              .D.
      OPEN (OUNIT,FILE=FLNM2,STATUS='NEW')                              .D.
      RETURN                                                            .D.
      END                                                               .D.
      SUBROUTINE MOMENT(PU, PUX, PUY, RDRP, RDRE, LNODE, X, Y, NLOOP,           
     * NBAR, XS, YS, AS, IAS, RP, RE, SY, FC, EO, E,                            
     * ACT, AST, ITRN, NITRN, TOLP, TOLE, AC, ACX, ACY)                         
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      DIMENSION LNODE(1),X(1),Y(1),ID(150),AC(1),ACX(1),ACY(1),                 
     *  XS(1),YS(1),AS(1),IAS(1),IDS(200),ES(200,3),                            
     *  XA(5,3),YA(5,3),XB(5,3),YB(5,3),                                        
     *  ACA(5,3),ACAX(5,3),ACAY(5,3),ACB(5,3),                                  
     *  ACBX(5,3),ACBY(5,3),ACAB(5,3),ACABX(5,3),ACABY(5,3),                    
     *  F(3),G(3),GX(3),GY(3),T(3)                                              
C                                                                               
      COMMON/VALUE/SRA,CRA,D,DP                                                 
C                                                                               
C     TOLP=1.0E-05                                                              
C     TOLE=1.0E-02                                                              
C     NITRN=100                                                                 
      DI = 0.0                                                                  
C                                                                               
C     WRITE(6,7000) LNODE(1), NLOOP, NBAR, NITRN                                
C7000 FORMAT(' LNODE,NLOOP,NBAR,NITRN ',4I5)                                    
C                                                                               
C     WRITE(6,7005) RP, RE                                                      
C7005 FORMAT(' RP, RE ',2F10.4)                                                 
C                                                                               
C     NF=LNODE(1)                                                               
C     DO 800 I = 1, NF                                                          
C     WRITE(6,7010) X(I), Y(I)                                                  
C7010 FORMAT(' X & Y ',2F10.4)                                                  
C 800 CONTINUE                                                                  
C                                                                               
C     DO 805 I = 1,NBAR                                                         
C     WRITE(6,7020) XS(I), YS(I), AS(I), IAS(I)                                 
C7020 FORMAT(' XS,YS,AS,IAS ',3F10.4,I5)                                        
C 805 CONTINUE                                                                  
C                                                                               
C                                                                               
C                                                                               
      POT=-SY*AST                                                               
      FCP=0.85*FC                                                               
      POC=FCP*(ACT-AST)-POT                                                     
      FRATIO=POT/POC                                                            
      RATIO =FRATIO+0.1                                                         
C***********************************************************************        
C             ITERATION FOR COMPUTING LOAD COMBINATIONS                         
C***********************************************************************        
      RA=RE                                                                     
      RA=RA*3.14159265359/180.                                                  
      REI=RA                                                                    
      D=DI                                                                      
C                                                                               
      KKK=0                                                                     
C     IF (RP.LT.RATIO) RP=RATIO                                                 
      RPUO=RP                                                                   
      IF (RE-45.0) 297,297,298                                                  
  297 IND=-1                                                                    
      EXEYO=DTAN (REI)                                                          
      GO TO 299                                                                 
C                                                                               
  298 IND= 1                                                                    
      EXEYO=DTAN (3.14159265359/2.0-REI)                                        
C                                                                               
  299 ITRN=0                                                                    
C                                                                               
  300 ITRN=ITRN+1                                                               
      CRA=DCOS(RA)                                                              
      SRA=DSIN(RA)                                                              
C***********************************************************************        
C             ID ARRAYS FOR NODES AND STEEL BARS                                
C***********************************************************************        
      YPMIN= 10000.                                                             
      YPMAX=-10000.                                                             
      NI=1                                                                      
      DO 340 L=1,NLOOP                                                          
      NF=NI+LNODE(L)-1                                                          
      DO 330 I=NI,NF                                                            
      ID(I)=0                                                                   
      YP=X(I)*SRA+Y(I)*CRA-D                                                    
      IF (YP.GT.0.) ID(I)=1                                                     
      IF (YP.LE.YPMAX) GO TO 310                                                
      YPMAX=YP                                                                  
      IMAX=I                                                                    
  310 IF (YP.GE.YPMIN) GO TO 330                                                
      YPMIN=YP                                                                  
      IMIN=I                                                                    
  330 CONTINUE                                                                  
      NF=NF+1                                                                   
      ID(NF)=ID(NI)                                                             
      NI=NF+1                                                                   
  340 CONTINUE                                                                  
      DMAX=YPMAX-YPMIN                                                          
      IF ((YPMAX.LE.0.).OR.(YPMIN.GE.0.)) GO TO 655                             
C                                                                               
      YIELDY=DABS(SY*YPMAX/(EO*E))                                              
      DO 350 J=1,NBAR                                                           
      IDS(J)=0                                                                  
      ES(J,2)=0.                                                                
      ES(J,3)=0.                                                                
      XP=XS(J)*CRA-YS(J)*SRA                                                    
      YP=XS(J)*SRA+YS(J)*CRA-D                                                  
      IF (DABS(YP).GE.YIELDY) GO TO 345                                         
      ES(J,2)=-EO*(YPMAX-YP)/(YPMAX*YPMAX)                                      
      ES(J,3)= EO*XP/YPMAX                                                      
  345 IF (YP.GE.YIELDY) IDS(J)=1                                                
      IF (YP.LE.-YIELDY) IDS(J)=-1                                              
      ES(J,1)= EO*YP/YPMAX                                                      
  350 CONTINUE                                                                  
C***********************************************************************        
C                 LOCATE POSITION OF NEUTRAL AXIS                               
C***********************************************************************        
      DP=0.15*YPMAX                                                             
      DO 356 I=1,NLOOP                                                          
      DO 355 J=1,3                                                              
      XA(I,J)=   0.0                                                            
      YA(I,J)=   0.0                                                            
      XB(I,J)=   0.0                                                            
      YB(I,J)=   0.0                                                            
      ACA(I,J)=  0.0                                                            
      ACAX(I,J)= 0.0                                                            
      ACAY(I,J)= 0.0                                                            
      ACB(I,J)=  0.0                                                            
      ACBX(I,J)= 0.0                                                            
      ACBY(I,J)= 0.0                                                            
      ACAB(I,J)= 0.0                                                            
      ACABX(I,J)=0.0                                                            
      ACABY(I,J)=0.0                                                            
  355 CONTINUE                                                                  
  356 CONTINUE                                                                  
      NI=1                                                                      
      DO 390 L=1,NLOOP                                                          
      NF=NI+LNODE(L)-1                                                          
      DO 380 I=NI,NF                                                            
      I1=I+1                                                                    
      IF (ID(I)-ID(I1)) 360,380,370                                             
  360 CALL NEUTR   (XA(L,1),YA(L,1),XA(L,2),YA(L,2),XA(L,3),YA(L,3),            
     *              X(I),Y(I),X(I1),Y(I1))                                      
C                                                                               
      CALL AREA (XA(L,1),YA(L,1),X(I1),Y(I1),ACA(L,1),ACAX(L,1),                
     *           ACAY(L,1))                                                     
C                                                                               
      CALL DIFFR   (ACA(L,2),ACAX(L,2),ACAY(L,2),                               
     *              ACA(L,3),ACAX(L,3),ACAY(L,3),                               
     *              XA(L,1),YA(L,1),XA(L,2),YA(L,2),XA(L,3),YA(L,3),            
     *              X(I1),Y(I1),0.0,0.0,0.0,0.0)                                
      GO TO 380                                                                 
C                                                                               
  370 CALL NEUTR   (XB(L,1),YB(L,1),XB(L,2),YB(L,2),XB(L,3),YB(L,3),            
     *              X(I),Y(I),X(I1),Y(I1))                                      
C                                                                               
      CALL AREA (X(I),Y(I),XB(L,1),YB(L,1),ACB(L,1),ACBX(L,1),ACBY(L,1))        
C                                                                               
      CALL DIFFR   (ACB(L,2),ACBX(L,2),ACBY(L,2),                               
     *              ACB(L,3),ACBX(L,3),ACBY(L,3),                               
     *              X(I ),Y(I ),0.0,0.0,0.0,0.0,                                
     *              XB(L,1),YB(L,1),XB(L,2),YB(L,2),XB(L,3),YB(L,3))            
  380 CONTINUE                                                                  
      NF=NF+1                                                                   
      NI=NF+1                                                                   
C                                                                               
      CALL AREA (XB(L,1),YB(L,1),XA(L,1),YA(L,1),                               
     *           ACAB(L,1),ACABX(L,1),ACABY(L,1))                               
C                                                                               
      CALL DIFFR   (ACAB(L,2),ACABX(L,2),ACABY(L,2),                            
     *              ACAB(L,3),ACABX(L,3),ACABY(L,3),                            
     *              XB(L,1),YB(L,1),XB(L,2),YB(L,2),XB(L,3),YB(L,3),            
     *              XA(L,1),YA(L,1),XA(L,2),YA(L,2),XA(L,3),YA(L,3))            
C                                                                               
  390 CONTINUE                                                                  
C***********************************************************************        
C                       COMPUTE ULTIMATE LOADS                                  
C***********************************************************************        
      PU=0.0                                                                    
      PUX=0.0                                                                   
      PUY=0.0                                                                   
      NI=1                                                                      
      DO 450 L=1,NLOOP                                                          
      NF=NI+LNODE(L)-1                                                          
      DO 440 I=NI,NF                                                            
      I1=I+1                                                                    
      IF ((ID(I).LT.1).OR.(ID(I1).LT.1)) GO TO 440                              
      PU=PU+AC(I)                                                               
      PUX=PUX+ACY(I)                                                            
      PUY=PUY+ACX(I)                                                            
  440 CONTINUE                                                                  
      NF=NF+1                                                                   
      NI=NF+1                                                                   
      PU=PU+ACA(L,1)+ACB(L,1)+ACAB(L,1)                                         
      PUX=PUX+ACAY(L,1)+ACBY(L,1)+ACABY(L,1)                                    
      PUY=PUY+ACAX(L,1)+ACBX(L,1)+ACABX(L,1)                                    
  450 CONTINUE                                                                  
      PU=FCP*PU                                                                 
      PUX=FCP*PUX                                                               
      PUY=FCP*PUY                                                               
C                                                                               
      DO 500 J=1,NBAR                                                           
      JS=IAS(J)                                                                 
      ASJ=AS(JS)                                                                
      IF (IDS(J)) 460,470,480                                                   
  460 FSJ=-SY*ASJ                                                               
      GO TO 490                                                                 
  470 FSJ=E*ES(J,1)*ASJ                                                         
      GO TO 490                                                                 
  480 FSJ=SY*ASJ                                                                
  490 PU=PU+FSJ                                                                 
      PUX=PUX+FSJ*YS(J)                                                         
      PUY=PUY+FSJ*XS(J)                                                         
  500 CONTINUE                                                                  
C***********************************************************************        
C                           CHECK CONVERGENCE                                   
C***********************************************************************        
      RPU=PU/POC                                                                
  520 RDRE=0.                                                                   
      IF (IND) 525,525,527                                                      
  525 EXEY=PUY/PUX                                                              
      THETA=DATAN(EXEY)*180./3.14159265359                                      
      GO TO 532                                                                 
C                                                                               
  527 EXEY=PUX/PUY                                                              
      THETA=DATAN (EXEY) *180./3.14159265359                                    
      THETA=90.-THETA                                                           
  532 RDRE=DABS (EXEY-EXEYO)                                                    
      RDRP=DABS (RPU-RPUO)                                                      
      ANGLE=RA*180./3.14159265359                                               
C                                                                               
C     WRITE(6,6666) ITRN, RE, RP, RDRE, RDRP, PUX, PUY                          
C6666 FORMAT(I3,2F5.1,2F10.7,2F10.2)                                            
C                                                                               
      IF ((RDRE.LE.TOLE).AND.(RDRP.LE.TOLP)) GO TO 700                          
C                                                                               
C***********************************************************************        
C              COMPUTE NEW LOCATION OF NEUTRAL AXIS                             
C***********************************************************************        
      DO 550 I=1,3                                                              
      F(I)=0.                                                                   
      G(I)=0.                                                                   
      GX(I)=0.                                                                  
      GY(I)=0.                                                                  
  550 CONTINUE                                                                  
      DO 600 L=1,NLOOP                                                          
      DO 600 I=2,3                                                              
      F(I)=F(I)+ACA(L,I)+ACB(L,I)+ACAB(L,I)                                     
      GX(I)=GX(I)+ACAY(L,I)+ACBY(L,I)+ACABY(L,I)                                
      GY(I)=GY(I)+ACAX(L,I)+ACBX(L,I)+ACABX(L,I)                                
  600 CONTINUE                                                                  
      DO 610 I=2,3                                                              
      F(I)=FCP*F(I)                                                             
      GX(I)=FCP*GX(I)                                                           
      GY(I)=FCP*GY(I)                                                           
  610 CONTINUE                                                                  
C                                                                               
      DO 620 J=1,NBAR                                                           
      IF (IDS(J).NE.0) GO TO 620                                                
C     IF (ID(J).NE.0) GO TO 620                                                 
      JS=IAS(J)                                                                 
      ASJ=AS(JS)                                                                
      FSDJ=E*ES(J,2)*ASJ                                                        
C     FSDJ=E*ES(L,2)*ASJ                                                        
      FSRAJ=E*ES(J,3)*ASJ                                                       
      F(2)=F(2)+FSDJ                                                            
      F(3)=F(3)+FSRAJ                                                           
      GX(2)=GX(2)+FSDJ*YS(J)                                                    
      GY(2)=GY(2)+FSDJ*XS(J)                                                    
      GX(3)=GX(3)+FSRAJ*YS(J)                                                   
      GY(3)=GY(3)+FSRAJ*XS(J)                                                   
  620 CONTINUE                                                                  
      F(2)=F(2)/POC                                                             
      F(3)=F(3)/POC                                                             
C                                                                               
  630 DRA=0.                                                                    
      DRP=RP-RPU                                                                
      AREI=DABS(RE)                                                             
      IF ((AREI.LT.1.0E-10).OR.(AREI.GE.90.)) GO TO 650                         
      DRE=EXEYO-EXEY                                                            
      IF (IND) 635,635,640                                                      
  635 G(2)=(GY(2)*PUX-GX(2)*PUY)/(PUX*PUX)                                      
      G(3)=(GY(3)*PUX-GX(3)*PUY)/(PUX*PUX)                                      
      GO TO 645                                                                 
  640 G(2)=(GX(2)*PUY-GY(2)*PUX)/(PUY*PUY)                                      
      G(3)=(GX(3)*PUY-GY(3)*PUX)/(PUY*PUY)                                      
C                                                                               
  645 FG=F(2)*G(3)-F(3)*G(2)                                                    
      IF (DABS(FG).LT.1.0E-10) GO TO 650                                        
      DD=(DRP*G(3)-DRE*F(3))/FG                                                 
      DRA=(-DRP*G(2)+DRE*F(2))/FG                                               
      DD=DD/2.0                                                                 
      DRA=DRA/2.0                                                               
C***********************************************************************        
C         CHANGE 'DRA' FOR HIGHLY RECTANGULAR SECTIONS                          
C***********************************************************************        
      IF(ARAT.GE.3.OR.ICON.EQ.4.OR.ICON.EQ.9) DRA=DRA/2.0                       
C                                                                               
      GO TO 690                                                                 
  650 IF (DABS(F(2)).GT.1.0E-15) GO TO 660                                      
  655 KKK=1                                                                     
      GO TO 700                                                                 
  660 DD=DRP/F(2)                                                               
      DD=DD/2.0                                                                 
  690 DMAY=DMAX/4.0                                                             
      IF (DD.GE.DMAY) DD=DMAX/5.                                                
      IF (DD.LE.-DMAY) DD=-DMAX/5.                                              
      D=D+DD                                                                    
      ADRA=DABS(DRA)                                                            
      IF (ADRA.GE.0.1) DRA=DRA*0.1/ADRA                                         
C***********************************************************************        
C           CHANGE 'DRA' FOR HIGHLY RECTANGULAR SECTIONS                        
C***********************************************************************        
      IF (ADRA.GE.0.1.AND.ICON.EQ.4) DRA=DRA/2                                  
      IF (ADRA.GE.0.1.AND.ICON.EQ.9) DRA=DRA/2                                  
      IF (ADRA.GE.0.1.AND.ARAT.GE.3.AND.ICON.NE.4.AND.ICON.NE.9)                
     * DRA=DRA/2                                                                
C                                                                               
      DRA = DRA / 10                                                            
C                                                                               
      RA=RA+DRA                                                                 
C                                                                               
  699 IF (ITRN.LT.NITRN) GO TO 300                                              
C                                                                               
  700 RETURN                                                                    
      END                                                                       
      SUBROUTINE ADJUST(MY,MX,P,YI,XI,HY,HX,ZLEN,DAY,DAX,                       
     *DBY,DBX,POC,ITIE,FC,PHI,ACT,K,MAGY,MAGX,PCYC,PCXC,XIS,YIS,E,EC,           
     *KX,KY,AST,BDX,BDY,EIX,EIY,SLX,SLY,ITOP,BD,IPIER,PHIMIN)                   
C**********************************************************************         
C    CHANGED MOMENT MAGNIFICATION AND BUCKLING CRITERA FROM                     
C    "PCA" METHOD TO "AASHTO" METHOD.                        10.15.85           
C    FIXED (PHI) CALCULATION WHEN (P) < .10*FC*ACT           12.03.87           
C**********************************************************************         
      IMPLICIT REAL*8 (A-H,M,O-Z)                                               
      REAL*8  KX, KY                                                            
      REAL * 4 GR,CA                                                            
      DIMENSION GR(22),CA(22)                                                   
C                                                                               
      DATA GR(1),GR(2),GR(3),GR(4),GR(5),GR(6),GR(7),GR(8),GR(9),               
     *GR(10),GR(11),GR(12),GR(13),GR(14),GR(15),GR(16),GR(17),GR(18),           
     *GR(19),GR(20),GR(21),GR(22)/                                              
     *'IH ','IH ','IH ','IP ','IP ','IP ','II ','III','III','III',              
     *'IV ','IV ','IV ','V  ','VI ','VI ','VI ','VII','VII',                    
     *'AL1','AL2','DL '/                                                        
      DATA CA(1),CA(2),CA(3),CA(4),CA(5),CA(6),CA(7),CA(8),CA(9),               
     *CA(10),CA(11),CA(12),CA(13),CA(14),CA(15),CA(16),CA(17),CA(18),           
     *CA(19),CA(20),CA(21),CA(22)/                                              
     *'1','2','3','1','2','3',' ','1','2','3','1','2','3',' ',                  
     *'1','2','3','1','2',' ',' ',' '/                                          
C                                                                               
      MAGX = 1.0                                                                
      MAGY = 1.0                                                                
C**********************************************************************         
C             CHECK MINIMIMUM ECCENTRICITIES                                    
C**********************************************************************         
      MYMIN =  (.6  + (.03 * HX)) / 12. * P                                     
      MXMIN =  (.6  + (.03 * HY)) / 12. * P                                     
      IF(DABS(MY).LT.MYMIN.AND.IPIER.EQ.0) MY = MYMIN                           
      IF(DABS(MX).LT.MXMIN) MX = MXMIN                                          
C                                                                               
      RAT = AST/ACT                                                             
C                                                                       .A.     
C  FIND PHI FOR THE GROUP VII (EQ) LOAD CASES                           .A.     
C                                                                       .A.     
      IF (PHIMIN .LT. 0.5) PHIMIN = 0.5                                 .B.     
C                                                                               
      IF ( K .NE. 18 .AND. K .NE. 19 ) GO TO 5                          .A.     
         PHI = 0.9 - 4.0 * P / FC / ACT                                 .A.     
         IF ( PHI .LT. PHIMIN ) PHI = PHIMIN                            .B.     
         GO TO 20                                                       .A.     
C                                                                       .A.     
C  FIND PHI FOR THE OTHER LOAD CASES                                    .A.     
C                                                                       .A.     
    5 IF ( ITIE .NE. 0 ) GO TO 10                                       .A.     
         PHI = 0.9 - 1.5 * P /FC / ACT                                  .A.     
         IF ( PHI .LT. 0.75 ) PHI = 0.75                                .A.     
         GO TO 20                                                       .A.     
C                                                                       .A.     
   10 PHI = 0.9 - 2.0 * P / FC / ACT                                    .A.     
      IF ( PHI .LT. 0.7 ) PHI = 0.7                                     .A.     
C                                                                       .A.     
   20 IF ( PHI .GT. 0.9 ) PHI = 0.9                                     .A.     
C                                                                               
   25 IF(ZLEN.EQ.0.0) GO TO 70                                                  
C                                                                               
   26 PO = PHI * POC                                                            
   27 PP = P                                                                    
   28 IF(ITOP.EQ.0) PP = P - (1.3 * BD * ACT * ZLEN * .150)                     
   29 PT=PP/PO                                                                  
C**********************************************************************         
C               FIND CRACKED MOMENT OF INERTIA OF SECTION                       
C**********************************************************************         
   30 EIX = ( (EC * XI / 5.0) + (E * XIS) ) / (1 + BDX)                         
   31 EIY = ( (EC * YI / 5.0) + (E * YIS) ) / (1 + BDY)                         
C**********************************************************************         
C          MODIFY (EI) FOR STEEL GREATER THAN 1%                                
C**********************************************************************         
C     FAC2 = 1.9                                                        .B.     
C     IF(RAT.GT.0.0101) FAC2 = 1.9 -((.8/.07)*(RAT-.01))                .B.     
C**********************************************************************         
C          MODIFY (EI) FOR HIGH AXIAL LOAD LEVEL (PT > 0.6)                     
C**********************************************************************         
C     IF(PT.GT.0.6) FAC2 = FAC2-((PT-.6)*(FAC2-1.0)/.3)                 .B.     
C     EIX = FAC2 * EIX                                                  .B.     
C     EIY = FAC2 * EIY                                                  .B.     
C**********************************************************************         
C                  FIND CRITICAL BUCKLING LOADS                                 
C**********************************************************************         
      PCXC= (3.1415927)**2 * EIX / (KX * ZLEN)**2                       .B.     
      PCYC= (3.1415927)**2 * EIY / (KY * ZLEN)**2                       .B.     
C**********************************************************************         
C          FIND MOMENT MAGNIFACATION (ASSUME SIDESWAY ALLOWED)                  
C**********************************************************************         
C     IF(K.EQ.18.OR.K.EQ.19) GO TO 70                                   .B.     
   32 MAGX=1.0/(1.0-PP/PHI/PCXC)                                        .B.     
   33 MAGY=1.0/(1.0-PP/PHI/PCYC)                                        .B.     
   34 IF(MAGY.LT.0) MAGY = 1.00                                                 
   35 IF(MAGX.LT.0) MAGX = 1.00                                                 
C                                                                               
   70 IF(SLX.LT.22.0) MAGX = 1.0                                                
      IF(SLY.LT.22.0) MAGY = 1.0                                                
      MY=MY*MAGY                                                                
      MX=MX*MAGX                                                                
      MY=MY/PHI                                                                 
      MX=MX/PHI                                                                 
      P=P/PHI                                                                   
C                                                                               
      RETURN                                                                    
      END                                                                       
C                                                                               
      SUBROUTINE NEUTR   (XA,YA,XAD,YAD,XARA,YARA,X,Y,XK,YK)                    
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      COMMON/VALUE/SRA,CRA,D,DP                                                 
C                                                                               
      XX=XK-X                                                                   
      YY=YK-Y                                                                   
      AXX=DABS(XX)                                                              
      AYY=DABS(YY)                                                              
      IF ((AXX.LT.1.0E-15).AND.(AYY.GT.1.0E-15)) GO TO 400                      
      IF ((AXX.GT.1.0E-15).AND.(AYY.LT.1.0E-15)) GO TO 500                      
      IF ((AXX.LT.1.0E-15).AND.(AYY.LT.1.0E-15)) GO TO 600                      
C                                                                               
      DB=XX*SRA+YY*CRA                                                          
      IF (DABS(DB).GT.1.0E-15) GO TO 300                                        
      STOP                                                                      
C                                                                               
  300 XA=X*YK-XK*Y                                                              
      YA=(-XA*SRA+(D+DP)*YY)/DB                                                 
      XA=(XA*CRA+(D+DP)*XX)/DB                                                  
      XAD=0.85*XX/DB                                                            
      YAD=0.85*YY/DB                                                            
      XARA=(-(X*YK-XK*Y)*SRA-XA*(-XX*CRA+YY*SRA))/DB                            
      YARA=(-(X*YK-XK*Y)*CRA-YA*(-XX*CRA+YY*SRA))/DB                            
      RETURN                                                                    
C                                                                               
  400 XA=X                                                                      
      YA=(D+DP-X*SRA)/CRA                                                       
      XAD=0.                                                                    
      YAD=0.85/CRA                                                              
      XARA=0.                                                                   
      YARA=-(X-(D+DP)*SRA)/(CRA*CRA)                                            
      RETURN                                                                    
C                                                                               
  500 XA=(D+DP-Y*CRA)/SRA                                                       
      YA=Y                                                                      
      XAD=0.85/SRA                                                              
      YAD=0.                                                                    
      XARA=(Y-(D+DP)*CRA)/(SRA*SRA)                                             
      YARA=0.                                                                   
      RETURN                                                                    
C                                                                               
  600 XA=X                                                                      
      YA=Y                                                                      
      RETURN                                                                    
      END                                                                       
      SUBROUTINE AREA (X,Y,X1,Y1,A,AX,AY)                                       
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      XX=X-X1                                                                   
      YY=Y+Y1                                                                   
      A=XX*YY/2.0                                                               
      AX=XX*(X*(YY+Y)+X1*(YY+Y1))/6.0                                           
      AY=XX*(YY*YY-Y*Y1)/6.0                                                    
      RETURN                                                                    
      END                                                                       
      SUBROUTINE DIFFR   (AD,AXD,AYD,ARA,AXRA,AYRA,                             
     *                    XA,YA,XAD,YAD,XARA,YARA,                              
     *                    XB,YB,XBD,YBD,XBRA,YBRA)                              
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      XX=XA-XB                                                                  
      YY=YA+YB                                                                  
      XXD=XAD-XBD                                                               
      YYD=YAD+YBD                                                               
      XXRA=XARA-XBRA                                                            
      YYRA=YARA+YBRA                                                            
C                                                                               
      AD=(XXD*YY+XX*YYD)/2.0                                                    
      ARA=(XXRA*YY+XX*YYRA)/2.0                                                 
      AXD=(XXD*(XA*(YA+YY)+XB*(YY+YB))+                                         
     *      XX*(XAD*(YY+YA)+XBD*(YY+YB)+XA*(YYD+YAD)+XB*(YYD+YBD)))/6.0         
      AXRA=(XXRA*(XA*(YA+YY)+XB*(YY+YB))+                                       
     *  XX*(XARA*(YA+YY)+XBRA*(YY+YB)+XA*(YYRA+YARA)+XB*(YYRA+YBRA)))/6.        
      AYD=(XXD*(YY*YY-YA*YB)+XX*(2.0*YY*YYD-YAD*YB-YA*YBD))/6.0                 
      AYRA=(XXRA*(YY*YY-YA*YB)+XX*(2.0*YY*YYRA-YARA*YB-YA*YBRA))/6.0            
      RETURN                                                                    
      END                                                                       
C                                                                               
      SUBROUTINE PBAL (PU,UM,NP,NE,POC,IPB)                                     
      IMPLICIT REAL * 8(A-H,O-Z)                                                
      REAL * 8 M                                                                
      REAL * 4 EPS                                                              
C                                                                               
      DIMENSION PU(10,1),UM(10,1),PUI(10),UMI(10),UMAX(10),IPB(1)               
C                                                                               
      EPS=.1                                                                    
C                                                                               
      DO 10 J=1,NE                                                              
      UMAX(J)=0.0                                                               
   10 CONTINUE                                                                  
C                                                                               
      DO 60 J=1,NE                                                              
      KK=1                                                                      
      DO 30 I=1,NP                                                              
      IF(PU(I,J).GT.10.E14) GO TO 30                                            
      PUI(KK)=PU(I,J)                                                           
      NPP=KK                                                                    
      KK=KK+1                                                                   
      IF(UM(I,J).LE.UMAX(J)) GO TO 30                                           
      UMAX(J)=UM(I,J)                                                           
      MAXI=KK-1                                                                 
   30 CONTINUE                                                                  
C                                                                               
      K=MAXI-1                                                                  
      P = PUI(K)                                                                
      RM=0.0                                                                    
      NINC=((NP-NPP)*25)+45                                                     
      DO 50 L=1,NINC                                                            
      KK=1                                                                      
      DO 40 I=1,NP                                                              
      IF(UM(I,J).GT.10.E14) GO TO 40                                            
      UMI(KK)=UM(I,J)                                                           
      NPP=KK                                                                    
      KK=KK+1                                                                   
   40 CONTINUE                                                                  
      P = P - .005*(POC)                                                        
      CALL DALI (P,PUI,UMI,M,NPP,EPS,IER)                                       
C     WRITE(6,1000) L,P,M,RM                                                    
C     WRITE(9,1000) L,P,M,RM                                                    
C1000 FORMAT(' INC,P,M,RM ',I5,3F10.5)                                          
      IF(M.LT.RM) GO TO 55                                                      
      PBI=P                                                                     
      RM=M                                                                      
   50 CONTINUE                                                                  
   55 IPB(J)=PBI                                                                
   60 CONTINUE                                                                  
C                                                                               
      RETURN                                                                    
      END                                                                       
      SUBROUTINE INERT (NI,NF,X,Y,XI,YI)                                        
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      DIMENSION X(1),Y(1)                                                       
      A=0.                                                                      
      YI=0.                                                                     
      XI=0.                                                                     
      NP=NF-1                                                                   
      DO 10 I=NI,NP                                                             
      J=I+1                                                                     
      XI=XI-(Y(I)**3*(X(J)-X(I))/3.+(Y(J)-Y(I))**3*(X(J)-X(I))/36.+             
     *(2.*Y(I)+Y(J))**2*(Y(J)-Y(I))*(X(J)-X(I))/18.)                            
      YI=YI-(-X(I)**3*(Y(J)-Y(I))/3.-(X(J)-X(I))**3*(Y(J)-Y(I))/36.-            
     *(2.*X(I)+X(J))**2*(X(J)-X(I))*(Y(J)-Y(I))/18.)                            
   10 CONTINUE                                                                  
      RETURN                                                                    
      END                                                                       
      SUBROUTINE BENT (IPAGE,HEAD,RNDATE,NHTIME,                                
     * NC, CSP, ZLEN, CGS, NP, DLN, P1, UM1, ICON, DBY, DBX,                    
     * DAY, DAX, ACT, FC, IREBAR, SPACE, RS, DSO, NSLOOP, HX, HY, ITOP,         
     * ARST, ARSL, PN18, PN19, PRNT, IDL, ITIE, VMAXP, UMAXP, PMAXP,            
     * VMINP, PMINP, BDIA, IBAR, IPIER)                                         
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      REAL * 4 EPS                                                              
      REAL * 8 NO                                                               
      CHARACTER*40 HEAD                                                 .D. 
      CHARACTER*11 RNDATE                                               .D.     
      CHARACTER*8 NHTIME, PRNT                                          .D.     
      DIMENSION PT(15), PB(15), P1(10,7), UM1(10,7), PUI(10), UMI(10),          
     * D(15), UMT(15), UMB(15), V(15), RS(1),                           .D.
     * P(7), UMPY(7), UMPX(7), BDIA(1)                                          
C                                                                               
      DATA YES,NO/'YES',' NO'/                                                  
C                                                                               
C     NC   = NO. OF COLUMNS                                                     
C     CSP  = COLUMN SPACING                                                     
C     ZLEN = COLUMN LENGTH                                                      
C     CGS  = C.G. OF SUPERSTRUCTURE                                             
C     RSO  = OUT OT OUT OF MAIN STEEL BARS                                      
C     ILONG=0 TRANSVERSE                                                        
C     ILONG=1 LONGITUDINAL                                                      
C     NP   = NO. OF PU VS. UM LOADS                                             
C                                                                               
C                                                                               
      IF(ITOP.EQ.1) DLNT=DLN                                                    
      IF(ITOP.EQ.1) DLNB=DLN+(ACT*ZLEN*.150)                                    
      IF(ITOP.EQ.0) DLNT=DLN-(ACT*ZLEN*.150)                                    
      IF(ITOP.EQ.0) DLNB=DLN                                                    
      NITER=20                                                                  
      ITER=0                                                                    
      EPS=.1                                                                    
      NE=1                                                                      
      NCC=NC                                                                    
      ILONG=0                                                                   
      ACT=ACT*144.                                                              
      FC=FC/.144                                                                
C                                                                               
      IF(NC.EQ.1) GO TO 60                                                      
      NCHECK=(NC+1)/2*2                                                         
      IF(NCHECK.NE.NC) GO TO 10                                                 
      ITYPE=0                                                                   
      ND=NC/2                                                                   
      GO TO 20                                                                  
   10 ITYPE=1                                                                   
      ND=(NC-1)/2                                                               
C                                                                               
   20 CG=((NC-1)*CSP)/2                                                         
      D(1)=CG                                                                   
      IF(NC.LE.3) GO TO 40                                                      
      DO 30 N=2,ND                                                              
      D(N)=D(N-1)-CSP                                                           
   30 CONTINUE                                                                  
   40 TI=0.0                                                                    
      DO 50 N=1,ND                                                              
      TI=TI+D(N)**2                                                             
   50 CONTINUE                                                                  
      TI=TI*2                                                                   
C                                                                               
   60 DO 70 N=1,NC                                                              
      PT(N)=DLNT                                                                
      PB(N)=DLNB                                                                
   70 CONTINUE                                                                  
C                                                                               
      K=1                                                                       
      DO 80 I=1,NP                                                              
      IF(P1(I,NE).GT.10.E14) GO TO 80                                           
      PUI(K)=P1(I,NE)                                                           
      NPP=K                                                                     
      K=K+1                                                                     
   80 CONTINUE                                                                  
C                                                                               
      DO 90 N=1,NC                                                              
      UMT(N)=0.0                                                                
      UMB(N)=0.0                                                                
      IF(IPIER.EQ.1.AND.ILONG.EQ.0) V(N)=0.0                                    
   90 CONTINUE                                                                  
C                                                                               
  100 ITER=ITER+1                                                               
      VT=0.0                                                                    
      UMBT=0.0                                                                  
      IF(ILONG.EQ.0.AND.IPIER.EQ.1) GO TO 185                                   
C                                                                               
      DO 180 N=1,NC                                                             
      K=1                                                                       
      DO 110 I=1,NP                                                             
      IF(UM1(I,NE).GT.10.E14) GO TO 110                                         
      UMI(K)=UM1(I,NE)                                                          
      NPP=K                                                                     
      K=K+1                                                                     
  110 CONTINUE                                                                  
      IF(ILONG.EQ.0.AND.NC.EQ.1)     GO TO 120                                  
      IF(ILONG.EQ.0.AND.DAY.GT.0.98) GO TO 120                                  
      IF(ILONG.EQ.1.AND.DAX.GT.0.98) GO TO 120                                  
C                                                                               
C     WRITE(6,3010) ITER, N, PT(N), UMT(N),NPP                                  
C     WRITE(9,3010) ITER, N, PT(N), UMT(N),NPP                                  
C3010 FORMAT(' ITER,N,PT,UMT,NPP',2I5,2F10.0,I5)                                
      CALL DALI (PT(N),PUI,UMI,UMT(N),NPP,EPS,IER)                              
C     WRITE(6,3010) ITER, N, PT(N), UMT(N) , NPP                                
C     WRITE(9,3010) ITER, N, PT(N), UMT(N) , NPP                                
C                                                                               
  120 IF(DBX.GT.0.98.OR.DBY.GT.0.98) GO TO 160                                  
      K=1                                                                       
      DO 140 I=1,NP                                                             
      IF(UM1(I,NE).GT.10.E14) GO TO 140                                         
      UMI(K)=UM1(I,NE)                                                          
      NPP=K                                                                     
      K=K+1                                                                     
  140 CONTINUE                                                                  
C                                                                               
C     WRITE(6,3020) ITER, N, PB(N), UMB(N)                                      
C     WRITE(9,3020) ITER, N, PB(N), UMB(N)                                      
C3020 FORMAT(' ITER,N,PB,UMB',2I5,2F10.0)                                       
      CALL DALI (PB(N),PUI,UMI,UMB(N),NPP,EPS,IER)                              
C     WRITE(6,3020) ITER, N, PB(N), UMB(N)                                      
C     WRITE(9,3020) ITER, N, PB(N), UMB(N)                                      
C                                                                               
      UMBT=UMBT+UMB(N)                                                          
  160 ZLENT=ZLEN                                                                
      IF(NC.EQ.1.AND.ILONG.EQ.0) ZLENT=ZLEN+CGS                                 
      V(N) = (UMT(N)+UMB(N))/ZLENT                                              
      VT=VT+V(N)                                                                
  180 CONTINUE                                                                  
C                                                                               
  185 IF(ILONG.EQ.1) GO TO 205                                                  
      IF(NC.EQ.1) GO TO 200                                                     
      IF(VI.GE.0.9999*VT.AND.VI.LE.1.0001*VT) GO TO 200                         
      VI=VT                                                                     
C                                                                               
      UMOT=VT*(ZLEN+CGS)                                                        
C                                                                               
      DO 190 N=1,ND                                                             
      POT = ((UMOT-UMBT)*D(N))/TI                                               
      PT(N) = DLNT-POT                                                          
      PB(N) = DLNB-POT                                                          
      PT(NC+1-N) = DLNT+POT                                                     
      PB(NC+1-N) = DLNB+POT                                                     
      IF(ITYPE.EQ.1) PT(ND+1) = DLNT                                            
      IF(ITYPE.EQ.1) PB(ND+1) = DLNB                                            
  190 CONTINUE                                                                  
      IF(ITER.LT.NITER) GO TO 100                                               
      WRITE(6,2000)                                                             
 2000 FORMAT(' ** ERROR - LATERAL FORCES DID NOT CONVERGE AFTER 20',            
     *' ITERATIONS.'/                                                           
     *' CALL TERRY BEEBE AT 6-0702.'/                                   .A.     
     *' EXECUTION TERMINATED')                                                  
      CALL EXIT                                                                 
C                                                                               
  200 CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)                                       
      WRITE(6,2020)                                                             
 2020 FORMAT(//7X,'DESIGN ASSUMPTIONS:'//                                       
     *7X,'1. ALL COLUMNS ARE THE SAME I.E., PRISMATIC, EQUAL LENGTH,',          
     *   ' EQUAL SPACING.'/                                                     
     *7X,'2. PLASTIC HINGES FORM AT THE TOP AND BOTTOM OF THE COLUMNS.'/        
     *7X,'3. THE DISTANCE BETWEEN THE PLASTIC HINGES IS TAKEN AS THE',          
     *   ' COLUMN LENGTH.'/                                                     
     *7X,'4. THE OVER-TURNING FORCE ACTS THROUGH THE C.G. OF THE SUPER',        
     *   'STRUCTURE AT A'/                                                      
     *7X,'   DISTANCE ''CGS'' ABOVE THE TOP OF THE COLUMN.'/                    
     *7X,'5. THE COLUMNS ARE NUMBERED FROM LEFT TO RIGHT AND THE OVER',         
     *   'TURNING FORCE'/                                                       
     *7X,'   ACTS TO THE RIGHT.')                                               
C                                                                               
C***********************************************************************        
C              PRINT PLASTIC FORCES FOR TIED COLUMNS ONLY                       
C***********************************************************************        
  205 IF(ITIE.EQ.0) GO TO 210                                                   
C                                                                               
      IF(IPIER.EQ.1.AND.ILONG.EQ.0) GO TO 211                                   
C                                                                               
      DO 207 N = 1,NC                                                           
      IF(N.EQ.3.OR.N.EQ.6.OR.N.EQ.9.OR.N.EQ.12.OR.N.EQ.15)                      
     * CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)                                      
      IF(ILONG.EQ.0) WRITE(6,3010)                                              
 3010 FORMAT(///7X,                                                             
     *    '***********************'/                                            
     *7X,'* TRANSVERSE DIRECTION *'/                                            
     *7X,'*****************************************************',               
     *   '********************')                                                
      IF(ILONG.EQ.1.AND.NCC.EQ.2) CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)           
      IF(ILONG.EQ.1.AND.NCC.EQ.5) CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)           
      IF(ILONG.EQ.1.AND.NCC.EQ.8) CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)           
      IF(ILONG.EQ.1.AND.NCC.EQ.11) CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)          
      IF(ILONG.EQ.1.AND.NCC.EQ.14) CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)          
      IF(ILONG.EQ.1) WRITE(6,3020)                                              
 3020 FORMAT(///7X,                                                             
     *   '**************************'/                                          
     *7X,'* LONGITUDINAL DIRECTION *'/                                          
     *7X,'*****************************************************',               
     *   '********************')                                                
      WRITE(6,3030) N,PT(N),UMT(N),PB(N),UMB(N),V(N)                            
 3030 FORMAT(//,                                                                
     *7X,'LOADS RESULTING FROM PLASTIC HINGING',                                
     *' OF COLUMN NO.',I2//                                                     
     *7X,'TOP OF COLUMN    AXIAL FORCE              P =',F7.0,' KIPS'/          
     *7X,'                 PLASTIC MOMENT          MP =',F7.0,' K-FT'/          
     *7X,'BOTTOM OF COLUMN AXIAL FORCE              P =',F7.0,' KIPS'/          
     *7X,'                 PLASTIC MOMENT          MP =',F7.0,' K-FT'//         
     *7X,'DESIGN SHEAR FORCE:                      VU =',F7.0,' KIPS')          
  207 CONTINUE                                                                  
C                                                                               
      GO TO 211                                                                 
C                                                                               
  210 IF(IREBAR.EQ.2.OR.IREBAR.EQ.4)                                            
     *CALL SHEAR (FC, IREBAR, PT, PB, V, UMT, UMB, NC,IPAGE,HEAD,               
     * RNDATE,NHTIME,SPACE, RS, DSO, NSLOOP, HX, HY, ILONG,                     
     * ARST, ARSL, PN18, PN19, CGS, CSP, ZLEN, BDIA, IBAR, IPIER)               
C**********************************************************************         
C            SHEAR KEY DESIGN FOR PINNED COLUMN                                 
C**********************************************************************         
  211 IF(DBY.LE.0.98.AND.DBX.LE.0.98) GO TO 212                                 
      IF(ILONG.EQ.0) VMAXP = 0.0                                                
      IF(ILONG.EQ.0) VMINP = 0.0                                                
      IF(ILONG.EQ.0) UMAXP = 0.0                                                
      IF(ILONG.EQ.0) PMAXP = 0.0                                                
      IF(ILONG.EQ.0) PMINP = 0.0                                                
      IF(DBX.GT.0.98.AND.DBY.LE.0.98.AND.ILONG.EQ.0) GO TO 212                  
      IF(DBY.GT.0.98.AND.DBX.LE.0.98.AND.ILONG.EQ.1) GO TO 212                  
      DO 400 N=1,NC                                                             
      IF(V(N).GT.VMAXP)   VMAXP = V(N)                                          
      IF(UMT(N).GT.UMAXP) UMAXP = UMT(N)                                        
      IF(PB(N).GT.PMAXP)  PMAXP = PB(N)                                         
      IF(PB(N).LT.PMINP)  VMINP = V(N)                                          
      IF(PB(N).LT.PMINP)  PMINP = PB(N)                                         
C     WRITE(6,1111) V(N), VMAXP, UMT(N), UMAXP, PB(N), PMAXP, PMINP,            
C    * ILONG, NC                                                                
C1111 FORMAT(' V(N) VMAXP UMT(N) UMAXP PB(N) PMAXP PMINP ILONG NC'/             
C    *7F9.0,2I3)                                                                
  400 CONTINUE                                                                  
C***********************************************************************        
C              FIND PLASTIC MOMENTS AT ALL ANGLES                               
C***********************************************************************        
  212 IF(ILONG.EQ.1) GO TO 300                                                  
      IF(PRNT.EQ.'NO ')  GO TO 290                                      .D.
      IQUIT = 0                                                                 
      IF(NC.GT.1)   GO TO 230                                                   
C***********************************************************************        
C              FIND PLASTIC MOMENTS FOR SINGLE COLUMN BENT                      
C***********************************************************************        
      ANG = 1.832595715                                                         
      DO 220 I = 1,7                                                            
      P(I)    = 0.0                                                             
      UMPX(I) = 0.0                                                             
      UMPY(I) = 0.0                                                             
      J = I                                                                     
      IF(ICON.GE.6)  J = 1                                                      
      IF(IPIER.EQ.1) J = 1                                                      
      ANG     = ANG - .261799388                                                
      IF(UM1(IDL,J).GT.10.E14) GO TO 220                                        
      P(I)    = P1(IDL,J)                                                       
      IF(DBX.GT.0.98.OR.DBY.GT.0.98) GO TO 220                                  
      IF(IPIER.EQ.1)                 GO TO 220                                  
      UMPY(I) = DSIN(ANG) * UM1(IDL,J)                                          
      UMPX(I) = DABS( DCOS(ANG) * UM1(IDL,J) )                                  
  220 CONTINUE                                                                  
C                                                                               
      IF(IPIER.EQ.1.AND.DBX.LE.0.98) UMPX(7) = UM1(IDL,1)                       
C                                                                               
      GO TO 280                                                                 
C***********************************************************************        
C              FIND PLASTIC MOMENTS FOR MULTI-COLUMN BENT                       
C***********************************************************************        
  230 P(1)   = PB(NC)                                                           
      P(7)   = DLNB                                                             
      PINC   = DABS((P(1) - P(7)) / 6.)                                         
      IF(NC.EQ.1) PINC = -PINC                                                  
C                                                                               
      DO 235 I = 2,6                                                            
      P(I) = P(I-1) - PINC                                                      
  235 CONTINUE                                                                  
C                                                                               
      DO 236 I = 1,7                                                            
      UMPX(I) = 0.0                                                             
      UMPY(I) = 0.0                                                             
  236 CONTINUE                                                                  
      IF(DBX.GT.0.98.AND.DBY.GT.0.98) GO TO 280                                 
C                                                                               
  237 DO 260 N = 1,7                                                            
      K=1                                                                       
      DO 240 I=1,NP                                                             
      J = N                                                                     
      IF(ICON.GE.6) J = 1                                                       
      IF(P1(I,J).GT.10.E14) GO TO 240                                           
      PUI(K)=P1(I,J)                                                            
      NPP=K                                                                     
      K=K+1                                                                     
  240 CONTINUE                                                                  
C                                                                               
      K=1                                                                       
      DO 250 I=1,NP                                                             
      J = N                                                                     
      IF(ICON.GE.6) J = 1                                                       
      IF(UM1(I,J).GT.10.E14) GO TO 250                                          
      UMI(K)=UM1(I,J)                                                           
      NPP=K                                                                     
      K=K+1                                                                     
  250 CONTINUE                                                                  
C                                                                               
C     WRITE(6,333)  N,P(N),UMB(N),NPP,IER                                       
C 333 FORMAT(/' N,P(N),UMB(N),NPP,IER'/                                         
C    *I5,2F10.0,2I5)                                                            
      CALL DALI (P(N),PUI,UMI,UMB(N),NPP,EPS,IER)                               
C     WRITE(6,333)  N,P(N),UMB(N),NPP,IER                                       
  260 CONTINUE                                                                  
C                                                                               
      ANG = 1.832595715                                                         
      DO 270 I = 1,7                                                            
      ANG     = ANG - .261799388                                                
      IF(DBY.LE.0.98) UMPY(I) = DSIN(ANG) * UMB(I)                              
      IF(DBX.LE.0.98) UMPX(I) = DABS( DCOS(ANG) * UMB(I) )                      
  270 CONTINUE                                                                  
C                                                                               
  280 WRITE(2,2070)                                                             
     *UMPY(1), UMPY(2), UMPY(3), UMPY(4), UMPY(5), UMPY(6) ,UMPY(7),            
     *UMPX(1), UMPX(2), UMPX(3), UMPX(4), UMPX(5), UMPX(6), UMPX(7),            
     *   P(1),    P(2),    P(3),    P(4),    P(5),    P(6),    P(7)             
 2070 FORMAT(7F10.0/7F10.0/7F10.0)                                              
      IF(IQUIT.EQ.0.AND.NC.EQ.1) GO TO 290                                      
C                                                                               
      IF(IQUIT.EQ.1) GO TO 290                                                  
      IQUIT = 1                                                                 
      NC = 1                                                                    
      GO TO 230                                                                 
C                                                                               
C ************ FIND MOMENTS AND SHEAR IN LONGITUDINAL DIRECTION ******          
C                                                                               
  290 NC=1                                                                      
      NE=7                                                                      
      IF(ICON.EQ.6.OR.ICON.EQ.8.OR.ICON.EQ.9) NE=1                              
      IF(IPIER.EQ.1)                          NE=1                              
      ILONG=1                                                                   
      GO TO 60                                                                  
C                                                                               
  300 NC=NCC                                                                    
      ACT=ACT/144.                                                              
      FC=FC*.144                                                                
      RETURN                                                                    
      END                                                                       
      SUBROUTINE CONCY(ICON,NLOOP,LNODE,NNODE,MNODE,X,Y,HX,HY)                  
C                                                                               
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      DIMENSION LNODE(1),X(1),Y(1)                                              
      INTEGER YES                                                               
      DATA YES,N/'Y','N'/                                                       
C                                                                               
      GO TO (100,200,300,400,600,600,600,600,600),ICON                          
C                                                                               
C***********************************************************************        
C                     MANUAL - CONCRETE                                         
C***********************************************************************        
  100 NI=1                                                                      
      DO 120 L=1,NLOOP                                                          
      READ (5,*) X(NI),Y(NI),LNODE(L)                                   .B.     
      LN=LNODE(L)-1                                                             
      NF=NI+LNODE(L)-1                                                          
      NI=NI+1                                                                   
      READ (5,*)    (X(I),Y(I),I=NI,NF)                                 .B.     
      NF=NF+1                                                                   
      NI=NI-1                                                                   
      X(NF)=X(NI)                                                               
      Y(NF)=Y(NI)                                                               
      NI=NF+1                                                                   
  120 CONTINUE                                                                  
C                                                                               
      WRITE(6,2010)                                                             
      NI=1                                                                      
      DO 130 L=1,NLOOP                                                          
      WRITE(6,2015) L,LNODE(L)                                                  
      NF=NI+LNODE(L)-1                                                          
      WRITE(6,2090) (I,X(I),Y(I),I=NI,NF)                                       
      NI=NF+2                                                                   
  130 CONTINUE                                                                  
                                                                                
      XMAX=0.                                                                   
      YMAX=0.                                                                   
      NF=LNODE(1)                                                               
      DO 150 I=1,NF                                                             
      XM=X(I)                                                                   
      YM=Y(I)                                                                   
      IF(XM.GT.XMAX) XMAX=XM                                                    
      IF(YM.GT.YMAX) YMAX=YM                                                    
  150 CONTINUE                                                                  
      HX=2*XMAX                                                                 
      HY=2*YMAX                                                                 
      GO TO 700                                                                 
C                                                                               
C***********************************************************************        
C                   RECTANGULAR - CONCRETE                                      
C***********************************************************************        
C                                                                               
  200 READ(5,*) HX,HY                                                   .B.     
      IF(HX.GE.HY) GO TO 205                                                    
       WRITE(6,5022)                                                            
 5022 FORMAT(///7X,'*** ERROR: THE LARGEST DIMENSION MUST BE PARALLEL',         
     *' TO THE X-AXIS.'/                                                        
     *7X,'HX MUST BE GREATER THAN OR EQUAL TO HY.'/                             
     *7X,'STANDARD FIX-UP TAKEN; HX AND HY VALUES INTERCHANGED.'/               
     *7X,'EXECUTION CONTINUES . . .')                                           
      HXX=HY                                                                    
      HY =HX                                                                    
      HX =HXX                                                                   
  205 X(1)= HX/2                                                                
      Y(1)= HY/2                                                                
      X(2)=-X(1)                                                                
      Y(2)= Y(1)                                                                
      X(3)=-X(1)                                                                
      Y(3)=-Y(1)                                                                
      X(4)= X(1)                                                                
      Y(4)=-Y(1)                                                                
      X(5)= X(1)                                                                
      Y(5)= Y(1)                                                                
  210 CONTINUE                                                                  
      WRITE(6,2020) HX,HY                                                       
      WRITE(6,2090) (I,X(I),Y(I),I=1,NNODE)                                     
      LNODE(1)=4                                                                
      GO TO 700                                                                 
C                                                                               
C***********************************************************************        
C              RECTANGULAR (WITH FILLETS) - CONCRETE                            
C***********************************************************************        
C                                                                               
  300 READ(5,*) HX,HY,FX,FY                                             .B.     
C                                                                               
      IF(HX.GE.HY) GO TO 305                                                    
      WRITE(6,5022)                                                             
      HXX=HY                                                                    
      HY =HX                                                                    
      HX =HXX                                                                   
C                                                                               
  305 IF(FX.EQ.0.0.OR.FY.EQ.0.0) GO TO 320                                      
      IYV=HY/2                                                                  
      IFY=FY                                                                    
      IXV=HX/2                                                                  
      IFX=FX                                                                    
      IF(IYV.EQ.IFY.AND.IXV.EQ.IFX) GO TO 330                                   
      IF(IYV.EQ.IFY) GO TO 310                                                  
C                                                                               
      NNODE=8                                                                   
      X(1)= HX/2                                                                
      Y(1)= HY/2-FY                                                             
      X(2)= HX/2-FX                                                             
      Y(2)= HY/2                                                                
      X(3)=-X(2)                                                                
      Y(3)= Y(2)                                                                
      X(4)=-X(1)                                                                
      Y(4)= Y(1)                                                                
      X(5)=-X(1)                                                                
      Y(5)=-Y(1)                                                                
      X(6)=-X(2)                                                                
      Y(6)=-Y(2)                                                                
      X(7)= X(2)                                                                
      Y(7)=-Y(2)                                                                
      X(8)= X(1)                                                                
      Y(8)=-Y(1)                                                                
      X(9)= X(1)                                                                
      Y(9)= Y(1)                                                                
      GO TO 340                                                                 
C                                                                               
C***********************************************************************        
C                    ELONGATED HEXAGON  FY=HY/2                                 
C***********************************************************************        
C                                                                               
  310 NNODE=6                                                                   
      X(1)= HX/2                                                                
      Y(1)= 0.0                                                                 
      X(2)= HX/2-FX                                                             
      Y(2)= HY/2                                                                
      X(3)=-X(2)                                                                
      Y(3)= Y(2)                                                                
      X(4)=-X(1)                                                                
      Y(4)= Y(1)                                                                
      X(5)=-X(2)                                                                
      Y(5)=-Y(2)                                                                
      X(6)= X(2)                                                                
      Y(6)=-Y(2)                                                                
      X(7)= X(1)                                                                
      Y(7)= Y(1)                                                                
      GO TO 340                                                                 
C                                                                               
C***********************************************************************        
C                 NO FILLETS - RECTANGULAR SECTION                              
C***********************************************************************        
C                                                                               
  320 NNODE=4                                                                   
      X(1)= HX/2                                                                
      Y(1)= HY/2                                                                
      X(2)=-X(1)                                                                
      Y(2)= Y(1)                                                                
      X(3)=-X(1)                                                                
      Y(3)=-Y(1)                                                                
      X(4)= X(1)                                                                
      Y(4)=-Y(1)                                                                
      X(5)= X(1)                                                                
      Y(5)= Y(1)                                                                
      GO TO 340                                                                 
C                                                                               
C***********************************************************************        
C                           DIAMOND                                             
C***********************************************************************        
C                                                                               
  330 NNODE=4                                                                   
      X(1)= HX/2                                                                
      Y(1)= 0.0                                                                 
      X(2)= 0.0                                                                 
      Y(2)= HY/2                                                                
      X(3)=-X(1)                                                                
      Y(3)= 0.0                                                                 
      X(4)= 0.0                                                                 
      Y(4)=-Y(2)                                                                
      X(5)= X(1)                                                                
      Y(5)= Y(1)                                                                
C                                                                               
  340 WRITE(6,2030) HX,HY,FX,FY                                                 
      WRITE(6,2090) (I,X(I),Y(I),I=1,NNODE)                                     
      LNODE(1)=NNODE                                                            
      GO TO 700                                                                 
C                                                                               
C***********************************************************************        
C                     CIRCULAR ENDS - CONCRETE                                  
C***********************************************************************        
C                                                                               
  400 READ(5,*) HX,HY                                                   .B.     
C                                                                               
      IF(HX.GE.HY) GO TO 405                                                    
      WRITE(6,5022)                                                             
      HXX=HY                                                                    
      HY =HX                                                                    
      HX =HXX                                                                   
C                                                                               
  405 X(1) = (HX-HY)/2                                                          
      Y(1) =-HY/2                                                               
      X(20)= X(1)                                                               
      Y(20)=-Y(1)                                                               
      X(21)=-X(1)                                                               
      Y(21)=-Y(1)                                                               
      X(40)=-X(1)                                                               
      Y(40)= Y(1)                                                               
      ANGLEI=(3.14159)/19                                                       
      ANGLE=-3.14159/2                                                          
      DO 420 I=2,19                                                             
      ANGLE=ANGLE+ANGLEI                                                        
      X(I)=(HY/2)*DCOS(ANGLE)+X(1)                                              
      Y(I)=(HY/2)*DSIN(ANGLE)                                                   
      X(41-I)=-X(I)                                                             
      Y(41-I)= Y(I)                                                             
  420 CONTINUE                                                                  
      X(41)=X(1)                                                                
      Y(41)=Y(1)                                                                
  430 CONTINUE                                                                  
      WRITE(6,2040) HX,HY                                                       
      WRITE(6,2090) (I,X(I),Y(I),I=1,40)                                        
      LNODE(1)=40                                                               
      GO TO 700                                                                 
C                                                                               
C***********************************************************************        
C                 REGULAR HEX,OCT & CIRCLE - CONCRETE                           
C***********************************************************************        
C                                                                               
  600 READ(5,*) HX                                                      .B.     
      R=HX/2                                                                    
      ANG=3.14159/8                                                             
      IF(ICON.EQ.8) R=(HX/2)/DCOS(ANG)                                          
      NI=1                                                                      
      NF=NI+NNODE-1                                                             
      ANGLEI=(2.*3.14159)/NNODE                                                 
      ANGLE=-ANGLEI                                                             
      IF(ICON.EQ.8) ANGLE=-ANGLEI+ANG                                           
      DO 620 I=NI,NF                                                            
      ANGLE=ANGLE+ANGLEI                                                        
      X(I)=R*DCOS(ANGLE)                                                        
      Y(I)=R*DSIN(ANGLE)                                                        
  620 CONTINUE                                                                  
      NF=NF+1                                                                   
      X(NF)=X(NI)                                                               
      Y(NF)=Y(NI)                                                               
      NF=NF-1                                                                   
C                                                                               
      IF(ICON.EQ.6) WRITE(6,2060) HX                                            
      IF(ICON.EQ.8) WRITE(6,2062) HX                                            
      IF(ICON.EQ.9) WRITE(6,2064) HX                                            
      WRITE(6,2090) (I,X(I),Y(I),I=NI,NF)                                       
      LNODE(1)=NNODE                                                            
      HY=HX                                                                     
C                                                                               
  700 CONTINUE                                                                  
      RETURN                                                                    
C                                                                               
 1000 FORMAT (A1)                                                               
 1010 FORMAT (2F10.0)                                                           
 1015 FORMAT (2F10.0,28X,I2)                                                    
 1020 FORMAT (2F10.0)                                                           
 1030 FORMAT (4F10.0)                                                           
 1040 FORMAT (2F10.0)                                                           
 1060 FORMAT (1F10.0)                                                           
C                                                                               
 7010 FORMAT (2F10.2)                                                           
 7015 FORMAT (2F10.2,28X,I2)                                                    
 7020 FORMAT (4F10.2)                                                           
 7030 FORMAT (4F10.2)                                                           
 7040 FORMAT (2F10.2)                                                           
 7060 FORMAT (1F10.2)                                                           
C                                                                               
 2010 FORMAT (//6X,' MANUAL INPUT CROSS-SECTION')                               
 2015 FORMAT (//6X,' LOOP NO.',I2,' HAS',I3,' COORDINATES')                     
 2020 FORMAT(//6X,' RECTANGULAR CROSS-SECTION'//                                
     *,14X,'HX',8X,'HY'/7X,(2F10.2))                                            
 2030 FORMAT(//6X,' RECTANGULAR CROSS-SECTION  (WITH FILLETS)'//                
     *,14X,'HX',8X,'HY',8X,'FX',8X,'FY'/7X,(4F10.2))                            
 2040 FORMAT(//7X,'RECTANGULAR CROSS-SECTION (WITH SEMI-CIRCLE AT ENDS)'        
     *//14X,'HX',8X,'HY'/7X,(2F10.2))                                           
 2060 FORMAT (//6X,' REGULAR HEXAGONAL CROSS-SECTION'//                         
     *,14X,'HX'/7X,(F10.2))                                                     
 2062 FORMAT (//6X,' REGULAR OCTAGNONAL CROSS-SECTION'//                        
     *,14X,'HX'/7X,(F10.2))                                                     
 2064 FORMAT (//6X,' CIRCULAR CROSS-SECTION'//                                  
     *,14X,'HX'/7X,(F10.2))                                                     
 2090 FORMAT (//6X,' CONCRETE COORDINATES  (INCHES)'//                          
     *6X,' COORD',7X,'X',9X,'Y'//(6X,I4,2X,2F10.2))                             
      END                                                                       
      SUBROUTINE DALI (X,ARG,VAL,Y,NDIM,EPS,IER)                                
      IMPLICIT REAL * 8 (A-H,O-Z)                                               
      DIMENSION ARG(1),VAL(1)                                                   
      IER=2                                                                     
      DELT2=0.0                                                                 
      IF(NDIM-1) 9,7,1                                                          
    1 DO 6 J=2,NDIM                                                             
      DELT1=DELT2                                                               
      IEND=J-1                                                                  
      DO 2 I=1,IEND                                                             
      H=ARG(I)-ARG(J)                                                           
      IF(H) 2,13,2                                                              
    2 VAL(J)=(VAL(I)*(X-ARG(J))-VAL(J)*(X-ARG(I)))/H                            
      DELT2=DABS(VAL(J)-VAL(IEND))                                              
      IF(J-2) 6,6,3                                                             
    3 IF(DELT2-EPS) 10,10,4                                                     
    4 IF(J-8) 6,5,5                                                             
    5 IF(DELT2-DELT1) 6,11,11                                                   
    6 CONTINUE                                                                  
    7 J=NDIM                                                                    
    8 Y=VAL(J)                                                                  
    9 RETURN                                                                    
   10 IER=0                                                                     
      GO TO 8                                                                   
   11 IER=1                                                                     
   12 J=IEND                                                                    
      GO TO 8                                                                   
   13 IER=3                                                                     
      GO TO 12                                                                  
      END                                                                       
      SUBROUTINE KEY (MFF, PFF, VMAXP, UMAXP, PMAXP, ARST, ARSL,                
     * EQTN, EQLN, FC, SY, ZLEN, VMINP, PMINP, DLN, PSN,                        
     * DBY, DBX, MYFF, MXFF, ACT, ITOP)                                         
C**********************************************************************         
C                       SHEAR KEY DESIGN                                        
C**********************************************************************         
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      REAL*8 MFF, MYFF, MXFF                                                    
      DIMENSION MFF(1), PFF(1), MYFF(1), MXFF(1)                                
C**********************************************************************         
C               FIND MAX MOMENT, SHEAR, AXIAL LOAD (FAC)                        
C**********************************************************************         
      FC = FC / 144.                                                            
      SY = SY / 144.                                                            
      DLN = DLN + (ACT * ZLEN * .150)                                           
C                                                                               
      UMAXF = 0.0                                                               
      PMAXF = 0.0                                                               
      DO 10 L = 1,21                                                            
      IF(L.GE.18.AND.L.LE.19) GO TO 10                                          
      IF(DBX.GT.0.98.AND.DBY.GT.0.98.AND.MFF(L).GT.UMAXF)  UMAXF=MFF(L)         
      IF(DBX.GT.0.98.AND.DBY.LE.0.98.AND.MXFF(L).GT.UMAXF) UMAXF=MXFF(L)        
      IF(DBY.GT.0.98.AND.DBX.LE.0.98.AND.MYFF(L).GT.UMAXF) UMAXF=MYFF(L)        
C                                                                               
      IF(PFF(L).GT.PMAXF) LM    = L                                             
      IF(PFF(L).GT.PMAXF) PMAXF = PFF(L)                                        
   10 CONTINUE                                                                  
C                                                                               
      VMAXF = UMAXF / ZLEN                                                      
C                                                                               
C**********************************************************************         
C             MAX MOMENT, SHEAR, AXIAL LOAD (ARS)                               
C**********************************************************************         
      IF(DBX.GT.0.98.AND.DBY.LE.0.98) GO TO 20                                  
      IF(DBY.GT.0.98.AND.DBX.LE.0.98) GO TO 30                                  
C***********************************************************************        
C                 PINNED IN BOTH DIRECTIONS                                     
C***********************************************************************        
      UMAXA = ARST                                                              
      IF(ARSL.GT.UMAXA) UMAXA = ARSL                                            
      VMAXA = UMAXA / ZLEN                                                      
      PNL = EQLN + DLN + PSN                                                    
      PNT = EQTN + DLN + PSN                                                    
      IF(EQTN.LT.0) PNT = -EQTN + DLN + PSN                                     
      PMAXA = PNT                                                               
      IF(PNL.GT.PMAXA) PMAXA = PNL                                              
C                                                                               
      PNT = EQTN + DLN + PSN                                                    
      IF(EQTN.GT.0) PNT = -EQTN + DLN + PSN                                     
      PMINA = PNT                                                               
      VMINA = ARST / ZLEN                                                       
      IF(PNL.LT.PMINA) VMINA = ARSL / ZLEN                                      
      IF(PNL.LT.PMINA) PMINA = PNL                                              
      GO TO 40                                                                  
C***********************************************************************        
C            PINNED IN LONGITUDINAL DIRECTION ONLY                              
C***********************************************************************        
   20 UMAXA = ARSL                                                              
      VMAXA = UMAXA / ZLEN                                                      
      PNL = EQLN + DLN + PSN                                                    
      PMAXA = PNL                                                               
C                                                                               
      PMINA = PNL                                                               
      VMINA = ARSL / ZLEN                                                       
      GO TO 40                                                                  
C***********************************************************************        
C            PINNED IN TRANSVERSE DIRECTION ONLY                                
C***********************************************************************        
   30 UMAXA = ARST                                                              
      VMAXA = UMAXA / ZLEN                                                      
      PNT = EQTN + DLN + PSN                                                    
      IF(EQTN.LT.0) PNT = -EQTN + DLN + PSN                                     
      PMAXA = PNT                                                               
C                                                                               
      PNT = EQTN + DLN + PSN                                                    
      IF(EQTN.GT.0) PNT = -EQTN + DLN + PSN                                     
      PMINA = PNT                                                               
      VMINA = ARST / ZLEN                                                       
      GO TO 40                                                                  
C**********************************************************************         
C                FIND CONTROLLING SHEAR AND AXIAL LOAD                          
C**********************************************************************         
   40 VMAX = VMAXA                                                              
      IF(VMAXP.LT.VMAX) VMAX = VMAXP                                            
      IF(VMAXF.GT.VMAX) VMAX = VMAXF                                            
C                                                                               
      NPMAX = 2                                                                 
      PMAX = PMAXA                                                              
      PMIN = PMINA                                                              
      VMIN = VMINA                                                              
      IF(VMAXP.LT.VMAXA) NPMAX = 3                                              
      IF(VMAXP.LT.VMAXA) PMAX = PMAXP                                           
      IF(VMAXP.LT.VMAXA) PMIN = PMINP                                           
      IF(VMAXP.LT.VMAXA) VMIN = VMINP                                           
C                                                                               
      WRITE(6,6000)                                                             
 6000 FORMAT(/7X,                                                               
     *   '**************'/                                                      
     *7X,'* KEY DESIGN *'/                                                      
     *7X,'************************************************************',        
     *   '*************')                                                       
C                                                                               
      IF(NPMAX.EQ.2) WRITE(6,6010) VMAXA, VMAXP                                 
 6010 FORMAT(7X,                                                                
     *   'NOTE:  THE UNREDUCED ELASTIC ARS + DEAD LOAD SHEAR ',                 
     *'(',F7.0,' KIPS)'/                                                        
     *7X,'IS LESS THAN THE PROBABLE PLASTIC SHEAR (',F7.0,' KIPS)'/             
     *7X,'THEREFORE, PLASTIC HINGING DOES NOT CONTROL.'/                        
     *7X,'KEY DESIGN IS BASED ON GROUP LOADS I TO VII.'/                        
     *7X,'(UNREDUCED ARS + DEAD LOAD SHEAR IS USED FOR GROUP VII)')             
C                                                                               
      IF(NPMAX.EQ.3) WRITE(6,6020)                                              
 6020 FORMAT(7X,                                                                
     *   'NOTE:  THE KEY DESIGN IS BASED ON PLASTIC HINGING.')                  
C                                                                               
      IF(DBX.GT.0.98.AND.DBY.LE.0.98) WRITE(6,6025)                             
      IF(DBY.GT.0.98.AND.DBX.LE.0.98) WRITE(6,6025)                             
 6025 FORMAT(//7X,                                                              
     *   'THE KEY IS DESIGNED FOR THE LOADS IN THE DIRECTION OF THE',           
     *   ' "PIN".'/                                                             
     *7X,'THE DESIGNER MUST CHECK THE LOADS IN THE "FIXED" DIRECTION.')         
C**********************************************************************         
C              SHEAR FRICTION (AREA OF KEY AND AREA OF STEEL)                   
C**********************************************************************         
      AK1 = VMAX / (.85 * .2 * FC)                                              
      AK2 = VMAX / (.85 * .8)                                                   
      AKS = AK1                                                                 
      IF(AK2.GT.AKS) AKS = AK2                                                  
      AVF = VMAX / (.85 * SY)                                                   
C                                                                               
      WRITE(6,6030) VMAX, AKS, AVF                                              
 6030 FORMAT(/7X,                                                               
     *   '**********************************'/                                  
     *7X '* SHEAR FRICTION (CODE 8.15.5.4) *'/                                  
     *7X,'************************************************************',        
     *   '*************'/                                                       
     *7X,'MAXIMUM SHEAR            (VU) =',F9.0,' KIPS'/                        
     *7X,'AREA OF KEY   REQUIRED  (ACF) =',F9.2,' SQ IN'/                       
     *7X,'AREA OF STEEL REQUIRED  (AVF) =',F9.2,' SQ IN')                       
C**********************************************************************         
C             FIND STEEL AREA BASED ON MIN AXIAL LOAD (UPLIFT)                  
C**********************************************************************         
      ASTT = 0.0                                                                
      IF(PMIN.GE.0.0) GO TO 50                                                  
      AST  = -PMIN / SY                                                         
      AVFT = VMIN / (.85 * SY)                                                  
      ASTT = AST + AVFT                                                         
C                                                                               
      WRITE(6,6040) PMIN, AST, VMIN, AVFT, ASTT                                 
 6040 FORMAT(/7X,                                                               
     *   '*************************************************'/                   
     *7X,'* MINIMUM (UPLIFT) AXIAL LOAD (CODE 8.16.4.5.3) *'/                   
     *7X,'************************************************************',        
     *   '*************'/                                                       
     *7X,'MINIMUM AXIAL LOAD            =',F9.0,' KIPS'/                        
     *7X,'AREA OF STEEL REQUIRED (AST)  =',F9.2,' SQ IN'//                      
     *7X,'ASSOCIATED SHEAR              =',F9.0,' KIPS'/                        
     *7X,'AREA OF STEEL REQUIRED (AVF)  =',F9.2,' SQ IN'//                      
     *7X,'TOTAL AREA OF STEEL REQUIRED  =',F9.2,' SQ IN'/)                      
C                                                                               
   50 ASMAX = AVF                                                               
      IF(ASTT.GT.ASMAX) ASMAX = ASTT                                            
C**********************************************************************         
C       FIND MINIMUM STEEL AREA (.005 X AREA OF KEY - AKS)                      
C**********************************************************************         
      ASMIN = .005 * AKS                                                        
      IF(ASMIN.GT.ASMAX) ASMAX = ASMIN                                          
C                                                                               
C**********************************************************************         
C             FIND KEY AREA BASED ON MAX COMPRESSIVE AXIAL LOAD                 
C**********************************************************************         
      AKF = ((PMAXF - (.70 * ASMAX * SY)) / (.70 * .85 * FC )) + ASMAX          
C                                                                               
      AKP = ((PMAX  - (ASMAX * SY)) / (.90 * .85 * FC)) + ASMAX                 
C                                                                               
      AKMAX = AKS                                                               
      IF(AKF.GE.AKMAX) AKMAX = AKF                                              
      IF(AKP.GE.AKMAX) AKMAX = AKP                                              
      IF(ASMAX.GT.0.005*AKMAX) GO TO 60                                         
C                                                                               
      C1 = .005 *.70 * SY                                                       
      C2 = .70 * .85 * FC                                                       
      C3 = C1 / C2                                                              
      AKF = PMAXF / C2 / (.995 + C3)                                            
C                                                                               
      C1 = .005 * SY                                                            
      C2 = .90 * .85 * FC                                                       
      C3 = C1 / C2                                                              
      AKP = PMAX / C2 / (.995 + C3)                                             
C                                                                               
      IF(AKF.GE.AKP) AKMAX = AKF                                                
      IF(AKP.GE.AKF) AKMAX = AKP                                                
      ASMAX = .005 * AKMAX                                                      
      ASMIN = ASMAX                                                             
C                                                                               
   60 WRITE(6,6055) ASMIN                                                       
 6055 FORMAT(/7X,                                                               
     *   '*************************************************'/                   
     *7X,'* MINIMUM AREA OF STEEL REQUIRED (CODE 4.4.9.4) *'/                   
     *7X,'************************************************************',        
     *   '*************'/                                                       
     *7X,'MINIMUM STEEL AREA REQUIRED   =',F9.2,' SQ IN')                       
C                                                                               
      WRITE(6,6050) ASMAX, PMAXF, AKF, PMAX, AKP                                
 6050 FORMAT(/7X,                                                               
     *   '****************************************************'/                
     *7X,'* MAXIMUM COMPRESSIVE AXIAL LOAD (CODE 8.16.4.5.2) *'/                
     *7X,'************************************************************',        
     *   '*************'/                                                       
     *7X,'AREA OF STEEL                 =',F9.2,' SQ IN'//                      
     *7X,'MAXIMUM AXIAL LOAD (FACTORED) =',F9.0,' KIPS'/                        
     *7X,'AREA OF KEY REQUIRED          =',F9.2,' SQ IN'//                      
     *7X,'MAXIMUM AXIAL LOAD (SEISMIC)  =',F9.0,' KIPS'/                        
     *7X,'AREA OF KEY REQUIRED          =',F9.2,' SQ IN')                       
C                                                                               
      WRITE(6,6060) AKMAX, ASMAX                                                
 6060 FORMAT(/7X,                                                               
     *   '***********'/                                                         
     *7X,'* SUMMARY *'/                                                         
     *7X,'************************************************************',        
     *   '*************'/                                                       
     *7X,'AREA OF KEY REQUIRED          =',F9.2,' SQ IN'/                       
     *7X,'TOTAL AREA OF STEEL REQUIRED  =',F9.2,' SQ IN')                       
C                                                                               
      FC = FC * 144.                                                            
      SY = SY * 144.                                                            
      DLN = DLN - (ACT * ZLEN * .150)                                           
      RETURN                                                                    
      END                                                                       
      SUBROUTINE LOAD (MYSC, MXSC, MSC, PSC,                                    
     *                 MYFC,  MXFC,  MFC,  PFC,                                 
     *                 MYSF,  MXSF,  MSF,  PSF,                                 
     *                 MYFF,  MXFF,  MFF,  PFF,                                 
     *                 MYFCI, MXFCI, MFCI, PFCI,                                
     * FC, POC, ICODE, PHI, ZLEN, DAY, DAX, DBY, DBX,                           
     * YI, XI, HY, HX, HEAD2, IGP, ITOP, ACT, ITIE,                             
     * ARST, ARSL, PN18, PN19, SY, PRNT, NC, NLOAD, HEAD, IVERF, DATEF,         
     * MAGY, MAGX, PCYC, PCXC, NWRITE, IPIER, DLN, PSN, EQTN, EQLN,             
     * XIS, YIS, KX, KY, SLX, SLY, E, EC, AST, EIX, EIY, PHIMIN,        .B.     
     * RNDATE, NHTIME, NPAGE )                                          .D.
C                                                                               
      IMPLICIT REAL*8 (A-H,L,M,O-Z)                                             
C     REAL*8  LFT,LFL,LFN,MY,MX,M                                               
C     REAL*8  MYS1,MXS1,MYS2,MXS2,MYF1,MXF1,MYF2,MXF2,MAGY,MAGX                 
      REAL*8  KX, KY                                                            
      CHARACTER*40 HEAD, HEAD2                                          .D.
      CHARACTER*11 RNDATE                                               .D.
      CHARACTER*8 NHTIME, FTYPE, PRNT                                   .D.
C                                                                               
      DATA PIN,FIX/'PIN', 'FIX'  /                                      .D.     
C                                                                               
C                                                                               
      DIMENSION MYSC(1), MXSC(1), MSC(1), PSC(1),                               
     *          MYFC(1), MXFC(1), MFC(1), PFC(1),                               
     *          MYSF(1), MXSF(1), MSF(1), PSF(1),                               
     *          MYFF(1), MXFF(1), MFF(1), PFF(1),                               
     *          MYFCI(1),MXFCI(1),MFCI(1),PFCI(1),                              
     *          MNY(22), MNX(22), MN(22), PN(22),                               
     *          MAGY(1), MAGX(1), PCYC(1), PCXC(1),                             
     *          PHI(1), EIX(1), EIY(1)                                  .D.
C                                                                               
      FC1 = FC / .144                                                           
      SY1 = SY / .144                                                           
C                                                                               
      NLOAD = NLOAD + 1                                                         
C                                                                               
      READ (5, 935, END = 999) HEAD2                                    .D.     
935   FORMAT(A40)                                                       .D.    
      READ (5, 940, END = 999) PRNT                                     .B.     
940   FORMAT (A1)                                                       .D. 
      IF (NLOAD .EQ. 1) GO TO 945                                               
          READ (5,950,END=999) FDUMY                                            
          GO TO 952                                                             
  945     READ (5, 950, END = 999) FTYPE                                .B.     
  950     FORMAT (A1)                                                           
          IF (FTYPE .EQ. 's'  .OR. FTYPE .EQ. 'S' ) FTYPE = 'SPREAD'    .D.     
          IF (FTYPE .EQ. 'p'  .OR. FTYPE .EQ. 'P' ) FTYPE = 'PILE'      .D.     
  952 IF (PRNT .EQ. 'Y'  .OR. PRNT .EQ. 'y' ) PRNT = 'YES'              .D.     
      IF (PRNT .EQ. 'N' .OR. PRNT .EQ. 'n') PRNT = 'NO '                .D.     
C                                                                               
C     IF(NLOAD.EQ.1) READ (5,1000,END=999) HEAD2,PRNT,FTYPE             .B.     
C     IF(NLOAD.GT.1) READ (5,1000,END=999) HEAD2,PRNT                   .B.     
 1000 FORMAT(3A8,A6,7X,A3,4X,A6)                                                
      CALL PAGE(NPAGE,HEAD,RNDATE,NHTIME)                               .C.     
      WRITE(6,3000)         HEAD2,PRNT,FTYPE                                    
 3000 FORMAT(//7X,                                                              
     *             '***************************'/                               
     *          7X,'* COLUMN LOAD DATA (K-FT) *'/                               
     *          7X,'******************************************',                
     *             '******************************'//                           
     *          7X,'LOAD NAME:         ',A40/                           .D.     
     *          7X,'FOOTING DATA FILE: ',A3/                                    
     *          7X,'TYPE FOOTING:      ',A6/)                                   
C                                                                               
      READ (5,*,END=900) KX,KY,PERI,ZLEN,ITIE,ITOP,ZZ,PHIMIN            .C.     
C1010 FORMAT(4F5.2,2F10.2,2I5,F10.2)                                    .B.     
C                                                                               
      IPIER = 0                                                                 
      WRAT = ZLEN/(HX/12.)                                                      
      IF(WRAT.LT.2.5) IPIER = 1                                                 
C                                                                               
      IF(ZZ.GE.1) GO TO 10                                                      
      WRITE(6,909)                                                              
  909 FORMAT(//7X,'*** ERROR IN LOAD DATA !'/                                   
     *7X,'THE DUCTILIY AND RISK FACTOR Z MUST BE GREATER THAN OR',              
     *7X,' EQUAL TO 1.0'/                                                       
     *7X,'EXECUTION TERMINATED . . .')                                          
      STOP                                                                      
C***********************************************************************        
C                CHECK FOR ALL LOAD DATA CARDS                                  
C***********************************************************************        
  900 WRITE(6,910)                                                              
  910 FORMAT(//7X,'*** ERROR IN LOAD DATA !'/                                   
     *7X,'ALL LOAD DATA CARDS MUST BE ENTERED EVEN IF BLANK'/                   
     *7X,'EXECUTION TERMINATED . . .')                                          
      STOP                                                                      
C                                                                               
   10 WRITE(6,3010) KX,KY,PERI,ZLEN,ITIE,ITOP,ZZ,PHIMIN                 .C.     
 3010 FORMAT(//7X,                                                              
     *   'EFF. LENGTH FACTORS           COLUMN     STEEL',              .C.     
     *'   LOCATION  DUCTILITY'/                                         .C.     
     *7X,':-----------------:  PERCENT  LENGTH     TIE=1',              .C.     
     *'      TOP=1     FACTOR  MIN'/                                    .C.     
     *7X,'    KX      KY        IMPACT  (FEET)  SPIRAL=0',              .C.     
     *'   BOTTOM=0        (Z)  PHI'//                                   .C.     
     *7X,F6.2,2X,F6.2,5X,F9.2,F8.2,I10,I11,F11.2,F5.2)                          
C     IF(DAY.LE.0.99.AND.DAX.LE.0.99.AND.DBY.LE.0.99.AND.DBX.LE.0.99)   .C.     
C    * GO TO 15                                                         .C.     
C                                                                               
C     WRITE(6,3020)                                                     .C.     
C3020 FORMAT(/' ** ERROR:  THE MOMENT DISTRIBUTION FACTORS MUST BE',    .C.     
C    *' LESS THAN 1.00 **'/                                             .C.     
C    *' EXECUTION TERMINATED . . .')                                    .C.     
C     STOP                                                              .C.     
C**********************************************************************         
C               FIND EFFECTIVE LENGTH ABOUT X-AXIS (KX)                         
C**********************************************************************         
C  15 IF(ZLEN.EQ.0.0) GO TO 340                                         .C.     
C     SCX = EC * XI / ZLEN                                              .C.     
C     IF(DAX.GT.0.0) GO TO 200                                          .C.     
C     GAX = 0.0                                                         .C.     
C     GO TO 210                                                         .C.     
C 200 SAX = (SCX / DAX) - SCX                                           .C.     
C     GAX =  SCX / SAX                                                  .C.     
C                                                                       .C.     
C 210 IF(DBX.GT.0.0) GO TO 220                                          .C.     
C     GBX = 0.0                                                         .C.     
C     GO TO 230                                                                 
C 220 SBX = (SCX / DBX) - SCX                                           .C.     
C     GBX =  SCX / SBX                                                  .C.     
C                                                                       .C.     
C 230 GAVEX = (GAX + GBX) / 2.                                                  
C     IF(GAVEX.LT.2.0)  KX = (20. - GAVEX) / 20. * (1.0 + GAVEX)**.5    .C.     
C     IF(GAVEX.GE.2.0)  KX = 0.9 * (1.0 + GAVEX)**.5                    .C.     
C     IF(KX.GT.2.1)     KX = 2.1                                        .C.     
      RX = (XI / ACT)**.5                                                       
      SLX = KX * ZLEN / RX                                                      
C**********************************************************************         
C               FIND EFFECTIVE LENGTH ABOUT Y-AXIS (KY)                         
C**********************************************************************         
C     SCY = EC * YI / ZLEN                                              .C.     
C     IF(DAY.GT.0.0) GO TO 300                                          .C.     
C     GAY = 0.0                                                         .C.     
C     GO TO 310                                                         .C.     
C 300 SAY = (SCY / DAY) - SCY                                           .C.     
C     GAY =  SCY / SAY                                                  .C.     
C                                                                               
C 310 IF(DBY.GT.0.0) GO TO 320                                          .C.     
C     GBY = 0.0                                                         .C.     
C     GO TO 330                                                         .C.     
C 320 SBY = (SCY / DBY) - SCY                                           .C.     
C     GBY =  SCY / SBY                                                  .C.     
C                                                                               
C 330 GAVEY = (GAY + GBY) / 2.                                          .C.     
C     IF(GAVEY.LT.2.0)  KY = (20. - GAVEY) / 20. * (1.0 + GAVEY)**.5    .C.     
C     IF(GAVEY.GE.2.0)  KY = 0.9 * (1.0 + GAVEY)**.5                    .C.     
C     IF(KY.GT.2.1)     KY = 2.1                                        .C.     
      RY = (YI / ACT)**.5                                                       
      SLY = KY * ZLEN / RY                                                      
C***********************************************************************        
C                        READ GROUP LOADS                                       
C***********************************************************************        
  340 READ (5,*,END=900) DLT,PST,HTT,HLT,HAT,WT,WLT,LFT,HCFT,TT         .B.     
      READ (5,*,END=900) DLL,PSL,HTL,HLL,HAL,WL,WLL,LFL,HCFL,TL         .B.     
      READ (5,*,END=900) DLN,PSN,HTN,HLN,HAN,WN,WLN,LFN,HCFN,TN         .B.     
C1030 FORMAT(F7.0,F8.0,4F7.0,4F6.0)                                     .B.     
C                                                                               
      DDLN = DLN                                                                
C                                                                               
C     READ (5,1040,END=900) PTT,PLT,PAT,PCFT                            .B.     
C     READ (5,1040,END=900) PTL,PLL,PAL,PCFL                            .B.     
C     READ (5,1040,END=900) PTN,PLN,PAN,PCFN                            .B.     
C1040 FORMAT(15X,3F7.0,19X,F6.0)                                        .B.     
      PTT = 0.                                                          .B.     
      PLT = 0.                                                          .B.     
      PAT = 0.                                                          .B.     
      PCFT = 0.                                                         .B.     
      PTL = 0.                                                          .B.     
      PLL = 0.                                                          .B.     
      PAL = 0.                                                          .B.     
      PCFL = 0.                                                         .B.     
      PTN = 0.                                                          .B.     
      PLN = 0.                                                          .B.     
      PAN = 0.                                                          .B.     
      PCFN = 0.                                                         .B.     
C                                                                               
      READ (5,*,END=900) EQTT,EQLT,MYS1,MYS2,MYF1,MYF2                  .B.     
      READ (5,*,END=900) EQTL,EQLL,MXS1,MXS2,MXF1,MXF2                  .B.     
      READ (5,*,END=900) EQTN,EQLN, PS1, PS2, PF1, PF2                  .B.     
C1050 FORMAT(6F10.0)                                                    .B.     
C***********************************************************************        
C                        PRINT GROUP LOADS                                      
C***********************************************************************        
      IDLT = DLT                                                                
      IPST = PST                                                                
      IHTT = HTT                                                                
      IHLT = HLT                                                                
      IHAT = HAT                                                                
      IWT  = WT                                                                 
      IWLT = WLT                                                                
      ILFT = LFT                                                                
      IHCFT= HCFT                                                               
      ITT  = TT                                                                 
C                                                                               
      IDLL = DLL                                                                
      IPSL = PSL                                                                
      IHTL = HTL                                                                
      IHLL = HLL                                                                
      IHAL = HAL                                                                
      IWL  = WL                                                                 
      IWLL = WLL                                                                
      ILFL = LFL                                                                
      IHCFL= HCFL                                                               
      ITL  = TL                                                                 
C                                                                               
      IDLN = DLN                                                                
      IPSN = PSN                                                                
      IHTN = HTN                                                                
      IHLN = HLN                                                                
      IHAN = HAN                                                                
      IWN  = WN                                                                 
      IWLN = WLN                                                                
      ILFN = LFN                                                                
      IHCFN= HCFN                                                               
      ITN  = TN                                                                 
C                                                                               
      IPTT = PTT                                                                
      IPLT = PLT                                                                
      IPAT = PAT                                                                
      IPCFT= PCFT                                                               
C                                                                               
      IPTL = PTL                                                                
      IPLL = PLL                                                                
      IPAL = PAL                                                                
      IPCFL= PCFL                                                               
C                                                                               
      IPTN = PTN                                                                
      IPLN = PLN                                                                
      IPAN = PAN                                                                
      IPCFN= PCFN                                                               
C                                                                               
      IEQTT = EQTT                                                              
      IEQLT = EQLT                                                              
      IMYS1 = MYS1                                                              
      IMYS2 = MYS2                                                              
      IMYF1 = MYF1                                                              
      IMYF2 = MYF2                                                              
C                                                                               
      IEQTL = EQTL                                                              
      IEQLL = EQLL                                                              
      IMXS1 = MXS1                                                              
      IMXS2 = MXS2                                                              
      IMXF1 = MXF1                                                              
      IMXF2 = MXF2                                                              
C                                                                               
      IEQTN = EQTN                                                              
      IEQLN = EQLN                                                              
      IPS1  = PS1                                                               
      IPS2  = PS2                                                               
      IPF1  = PF1                                                               
      IPF2  = PF2                                                               
      WRITE(6,3060) IDLT,IPST,IHTT,IHLT,IHAT,IWT,IWLT,ILFT,IHCFT,ITT,           
     *              IDLL,IPSL,IHTL,IHLL,IHAL,IWL,IWLL,ILFL,IHCFL,ITL,           
     *              IDLN,IPSN,IHTN,IHLN,IHAN,IWN,IWLN,ILFN,IHCFN,ITN    .C.     
C    *              IPTT,IPLT,IPAT,IPCFT,                               .C.     
C    *              IPTL,IPLL,IPAL,IPCFL,                               .C.     
C    *              IPTN,IPLN,IPAN,IPCFN                                .C.     
 3060 FORMAT(//7X,'COLUMN GROUP LOADS - SERVICE (K-FT)'//                       
     *14X,'             :---- LL+IMPACT ---:'/                                  
     *14X,'             CASE 1 CASE 2 CASE 3'/                                  
     *14X,'DEAD     PRE  TRANS   LONG  AXIAL'/                                  
     *14X,'LOAD  STRESS MY-MAX MX-MAX  N-MAX   WIND    WL    LF CF-MY',         
     *'  TEMP'//                                                                
     *7X,'MY  ',I7,I8,4I7,4I6/                                                  
     *7X,'MX  ',I7,I8,4I7,4I6/                                                  
     *7X,'P   ',I7,I8,4I7,4I6)                                          .C.     
C    *7X,'PMY ',15X,3I7,19X,I6/                                         .C.     
C    *7X,'PMX ',15X,3I7,19X,I6/                                         .C.     
C    *7X,'P   ',15X,3I7,19X,I6)                                         .C.     
C                                                                               
      WRITE(6,3070) EQTT,EQLT,MYS1,MYS2,MYF1,MYF2,                              
     *              EQTL,EQLL,MXS1,MXS2,MXF1,MXF2,                              
     *              EQTN,EQLN, PS1, PS2, PF1, PF2                               
 3070 FORMAT(//7X,'COLUMN SEISMIC AND ARBITRARY LOADS (K-FT)'//                 
     *7X,'     (ARS) UNREDUCED SEISMIC     ARBITRARY LOADS',                    
     *'     ARBITRARY LOADS'/                                                   
     *7X,'            CASE 1    CASE 2   SERVICE   SERVICE',                    
     *'  FACTORED  FACTORED'/                                                   
     *7X,'          MAX TRAN  MAX LONG       AL1       AL2',                    
     *'       AL1       AL2'//                                                  
     *7X,'MY TRAN ',6F10.0/                                                     
     *7X,'MX LONG ',6F10.0/                                                     
     *7X,'P AXIAL ',6F10.0)                                                     
C                                                                               
      ENDX=FIX                                                                  
      ENDY=FIX                                                                  
      IF(DBX.GT.0.98) ENDX=PIN                                                  
      IF(DBY.GT.0.98) ENDY=PIN                                                  
      IFIX = 0                                                                  
      IF(ENDX.EQ.FIX.OR.ENDY.EQ.FIX) IFIX = 1                                   
C***********************************************************************        
      IF(PRNT.EQ.'YES'.AND.IGP.EQ.2) GO TO 98                           .D. 
      IF(PRNT.EQ.'YES'.AND.IGP.NE.2)                                    .D. 
     *WRITE(6,3071)                                                             
 3071 FORMAT(/,                                                                 
     *7X,'NOTE:  A FOOTING INPUT DATA FILE WAS NOT CREATED.  A FOOTING'/        
     *7X,'INPUT DATA FILE IS ONLY AVAILABLE IN THE "CHECK" MODE.')              
      GO TO 99                                                                  
C                                                                               
   98 BC = HX / 12.                                                             
      HC = HY / 12.                                                             
C                                                                               
      IF(ITOP.EQ.1.AND.DBX.GT.0.98.AND.IPIER.EQ.1) GO TO 108                    
      IF(ITOP.EQ.1.AND.IFIX.EQ.1) GO TO 107                                     
      IF(ITOP.EQ.0.AND.IFIX.EQ.0) GO TO 107                                     
      GO TO 108                                                                 
C                                                                               
  107 IF(ITOP.EQ.1) WRITE(6,3072)                                               
 3072 FORMAT(//,                                                                
     *7X,'NOTE:  A FOOTING INPUT DATA FILE WAS NOT CREATED.  THE LOADS'/        
     *7X,'ARE NOT LOCATED AT THE BOTTOM OF THE COLUMN.'/)                       
C                                                                               
      IF(ITOP.EQ.0) WRITE(6,3073)                                               
 3073 FORMAT(//,                                                                
     *7X,'NOTE:  A FOOTING INPUT DATA FILE WAS NOT CREATED.  THE LOADS'/        
     *7X,'ARE NOT LOCATED AT THE TOP OF THE COLUMN.'/)                          
      PRNT='NO '                                                        .D.
      GO TO 99                                                                  
C                                                                               
  108 IF(ENDX.EQ.FIX) GO TO 102                                                 
      IF(ITOP.EQ.1) IDLN = (ACT * ZLEN * .150) + IDLN                           
      IDLL = 0                                                                  
      IPSL = 0                                                                  
      IHTL = 0                                                                  
      IHLL = 0                                                                  
      IHAL = 0                                                                  
      IWL  = 0                                                                  
      IWLL = 0                                                                  
      ILFL = 0                                                                  
      ITL  = 0                                                                  
C                                                                               
      IPTL = 0                                                                  
      IPLL = 0                                                                  
      IPAL = 0                                                                  
C                                                                               
      IEQTL = 0                                                                 
      IEQLL = 0                                                                 
      IMXS1 = 0                                                                 
      IMXS2 = 0                                                                 
      IMXF1 = 0                                                                 
      IMXF2 = 0                                                                 
C                                                                               
  102 IF(ENDY.EQ.FIX.AND.IPIER.EQ.0) GO TO 103                                  
      IF(ITOP.EQ.1.AND.ENDX.EQ.FIX) IDLN = (ACT * ZLEN * .150) + IDLN           
      IDLT = 0                                                                  
      IPST = 0                                                                  
      IHTT = 0                                                                  
      IHLT = 0                                                                  
      IHAT = 0                                                                  
      IWT  = 0                                                                  
      IWLT = 0                                                                  
      ILFT = 0                                                                  
      IHCFL= 0                                                                  
      IHCFT= 0                                                                  
      IHCFN= 0                                                                  
      ITT  = 0                                                                  
C                                                                               
      IPTT = 0                                                                  
      IPLT = 0                                                                  
      IPAT = 0                                                                  
      IPCFL= 0                                                                  
      IPCFT= 0                                                                  
      IPCFN= 0                                                                  
C                                                                               
      IEQTT = 0                                                                 
      IEQLT = 0                                                                 
      IMYS1 = 0                                                                 
      IMYS2 = 0                                                                 
      IMYF1 = 0                                                                 
      IMYF2 = 0                                                                 
  103 IF(NWRITE.EQ.1) GO TO 104                                                 
C                                                                               
      NWRITE = 1                                                                
      WRITE(2,3075) HEAD, IVERF, DATEF                                          
 3075 FORMAT(A40,'     VERSION',I3,4X,A8)                               .D. 
      IF(ENDX.EQ.FIX.AND.ENDY.EQ.FIX)                                           
     *WRITE(2,3076) BC,HC,ACT,PERI,ENDX,ENDY,NC,FTYPE,FC1,SY1                   
 3076 FORMAT(4F10.2,2X,A3,2X,A3,8X,I2/                                          
     *'    DESIGN',34X,A6,'     10.00'/                                         
     *2F10.0,'     150.0'/                                                      
     *'                             4         4      3.00         X')           
C                                                                               
      IF(ENDX.EQ.PIN.OR.ENDY.EQ.PIN)                                            
     *WRITE(2,3079) BC,HC,ACT,PERI,ENDX,ENDY,NC,FTYPE,FC1,SY1                   
 3079 FORMAT(4F10.2,2X,A3,2X,A3,8X,I2//                                         
     *'    DESIGN',34X,A6/                                                      
     *2F10.0,'     150.0'/                                                      
     *'                             4         4      3.00         X')           
C                                                                               
      CLR = 6.00                                                                
      IF(FTYPE.EQ.'SPREAD') CLR = 3.00                                  .D.
      WRITE(2,3077) CLR                                                         
 3077 FORMAT(46X,F4.2,'         X'/                                             
     *'         5     12.00     12.00'/                                         
     *'      2.00    120.00    170.00'/                                         
     *'     39.00     24.00    271.00       .41      9.00    521.00')           
C                                                                               
      IF(FTYPE.EQ.'PILE') WRITE(2,3078)                                 .D.
 3078 FORMAT(                                                                   
     *'                   1      12.0      70.0       YES'/)                    
C                                                                               
C                                                                               
C                                                                               
  104 WRITE(2,3081) HEAD2,                                                      
     *              IDLT,IPST,IHTT,IHLT,IHAT,IWT,IWLT,ILFT,IHCFT,ITT,           
     *              IDLL,IPSL,IHTL,IHLL,IHAL,IWL,IWLL,ILFL,IHCFL,ITL,           
     *              IDLN,IPSN,IHTN,IHLN,IHAN,IWN,IWLN,ILFN,IHCFN,ITN            
 3081 FORMAT(A40/                                                       .D.     
     *       I7,I8,4I7,4I6/                                                     
     *       I7,I8,4I7,4I6/                                                     
     *       I7,I8,4I7,4I6)                                                     
C                                                                               
      WRITE(2,3082) IPTT,IPLT,IPAT,IPCFT,                                       
     *              IPTL,IPLL,IPAL,IPCFL,                                       
     *              IPTN,IPLN,IPAN,IPCFN                                        
 3082 FORMAT(15X,3I7,19X,I6/                                                    
     *       15X,3I7,19X,I6/                                                    
     *       15X,3I7,19X,I6)                                                    
C                                                                               
      WRITE(2,3083) IEQTT,IEQLT,IMYS1,IMYS2,IMYF1,IMYF2,                        
     *              IEQTL,IEQLL,IMXS1,IMXS2,IMXF1,IMXF2,                        
     *              IEQTN,IEQLN, IPS1, IPS2, IPF1, IPF2                         
 3083 FORMAT(6I10/6I10/6I10)                                                    
C***********************************************************************        
C               ITIE=0 .... SPIRAL  PHI=.75                                     
C               ITIE=1 .... TIED    PHI=.70                                     
C               N=1    .... SERVICE  LOADS FOR COLUMN                           
C               N=2    .... FACTORED LOADS FOR COLUMN                           
C               N=3    .... SERVICE  LOADS FOR FOOTING                          
C               N=4    .... FACTORED LOADS FOR FOOTING                          
C***********************************************************************        
   99 N=0                                                                       
  100 N=N+1                                                                     
      IF(IGP.EQ.1) N=2                                                          
C                                                                               
      IF(N.EQ.3) GO TO 110                                                      
      GO TO 120                                                                 
C***********************************************************************        
C     REMOVE THE IMPACT LOADS FROM THE LL FOR FOOTING LOADS - N=3,4             
C***********************************************************************        
  110 BD=1.0                                                                    
      PI=PERI/100+1.0                                                           
      HTT=HTT/PI                                                                
      HLT=HLT/PI                                                                
      HAT=HAT/PI                                                                
      HTL=HTL/PI                                                                
      HLL=HLL/PI                                                                
      HAL=HAL/PI                                                                
      HTN=HTN/PI                                                                
      HLN=HLN/PI                                                                
      HAN=HAN/PI                                                                
      PTT=PTT/PI                                                                
      PLT=PLT/PI                                                                
      PAT=PAT/PI                                                                
      PTL=PTL/PI                                                                
      PLL=PLL/PI                                                                
      PAL=PAL/PI                                                                
      PTN=PTN/PI                                                                
      PLN=PLN/PI                                                                
      PAN=PAN/PI                                                                
C                                                                               
  120 IF(N.EQ.1) BD=1.0                                                         
      IF(N.EQ.2) BD=.75                                                         
C                                                                               
      IF(N.EQ.1.OR.N.EQ.3) FAC1=1.0                                             
      IF(N.EQ.1.OR.N.EQ.3) FAC2=1.0                                             
      IF(N.EQ.2.OR.N.EQ.4) FAC1=1.3                                             
      IF(N.EQ.2.OR.N.EQ.4) FAC2=5.0/3.0                                         
C***********************************************************************        
C     ADJUST DEAD LOAD BY WEIGHT OF COLUMN FOR FOOTING LOADS                    
C***********************************************************************        
      IF(ITOP.EQ.1.AND.N.EQ.3.AND.ENDY.EQ.PIN.AND.ENDX.EQ.PIN)                  
     * DLN = DLN + (ACT * ZLEN * .150)                                          
C                                                                               
C     GP IH  CASE 1                                                             
C                                                                               
      MNY(1)=FAC1*(BD*DLT+FAC2*HTT+HCFT)+PST                                    
      MNX(1)=FAC1*(BD*DLL+FAC2*HTL      )+PSL                                   
      PN (1)=FAC1*(BD*DLN+FAC2*HTN      )+PSN                                   
C                                                                               
C     GP IH  CASE 2                                                             
      MNY(2)=FAC1*(BD*DLT+FAC2*HLT+HCFL)+PST                                    
      MNX(2)=FAC1*(BD*DLL+FAC2*HLL     )+PSL                                    
      PN (2)=FAC1*(BD*DLN+FAC2*HLN     )+PSN                                    
C                                                                               
C     GP IH  CASE 3                                                             
      MNY(3)=FAC1*(DLT+FAC2*HAT+HCFN)+PST                                       
      MNX(3)=FAC1*(DLL+FAC2*HAL     )+PSL                                       
      PN (3)=FAC1*(DLN+FAC2*HAN     )+PSN                                       
C                                                                               
      IF(N.EQ.1.OR.N.EQ.3) GO TO 130                                            
C     GP IP  CASE 1                                                             
      MNY(4)=FAC1*(BD*DLT+PTT+PCFT)+PST                                         
      MNX(4)=FAC1*(BD*DLL+PTL     )+PSL                                         
      PN (4)=FAC1*(BD*DLN+PTN     )+PSN                                         
C                                                                               
C     GP IP  CASE 2                                                             
      MNY(5)=FAC1*(BD*DLT+PLT+PCFL)+PST                                         
      MNX(5)=FAC1*(BD*DLL+PLL     )+PSL                                         
      PN (5)=FAC1*(BD*DLN+PLN     )+PSN                                         
C                                                                               
C     GP IP  CASE 3                                                             
      MNY(6)=FAC1*(DLT+PAT+PCFN)+PST                                            
      MNX(6)=FAC1*(DLL+PAL     )+PSL                                            
      PN (6)=FAC1*(DLN+PAN     )+PSN                                            
C                                                                               
  130 IF(N.EQ.1.OR.N.EQ.3) FAC3=1.0/1.25                                        
      IF(N.EQ.2.OR.N.EQ.4) FAC1=1.3                                             
      IF(N.EQ.2.OR.N.EQ.4) FAC3=1.0                                             
C     GP II                                                                     
      MNY(7)=FAC3*(FAC1*(BD*DLT+WT)+PST)                                        
      MNX(7)=FAC3*(FAC1*(BD*DLL+WL)+PSL)                                        
      PN (7)=FAC3*(FAC1*(BD*DLN+WN)+PSN)                                        
C                                                                               
C     GP III  CASE 1                                                            
      MNY(8)=FAC3*(FAC1*(BD*DLT+HTT+HCFT+.3*WT+WLT+LFT)+PST)                    
      MNX(8)=FAC3*(FAC1*(BD*DLL+HTL+     .3*WL+WLL+LFL)+PSL)                    
      PN (8)=FAC3*(FAC1*(BD*DLN+HTN+     .3*WN+WLN+LFN)+PSN)                    
C                                                                               
C     GP III  CASE 2                                                            
      MNY(9)=FAC3*(FAC1*(BD*DLT+HLT+HCFL+.3*WT+WLT+LFT)+PST)                    
      MNX(9)=FAC3*(FAC1*(BD*DLL+HLL+     .3*WL+WLL+LFL)+PSL)                    
      PN (9)=FAC3*(FAC1*(BD*DLN+HLN+     .3*WN+WLN+LFN)+PSN)                    
C                                                                               
C     GP III  CASE 3                                                            
      MNY(10)=FAC3*(FAC1*(DLT+HAT+HCFN+.3*WT+WLT+LFT)+PST)                      
      MNX(10)=FAC3*(FAC1*(DLL+HAL+     .3*WL+WLL+LFL)+PSL)                      
      PN (10)=FAC3*(FAC1*(DLN+HAN+     .3*WN+WLN+LFN)+PSN)                      
C                                                                               
C     GP IV   CASE 1                                                            
      MNY(11)=FAC3*(FAC1*(BD*DLT+HTT+HCFT+TT)+PST)                              
      MNX(11)=FAC3*(FAC1*(BD*DLL+HTL+     TL)+PSL)                              
      PN (11)=FAC3*(FAC1*(BD*DLN+HTN+     TN)+PSN)                              
C                                                                               
C     GP IV   CASE 2                                                            
      MNY(12)=FAC3*(FAC1*(BD*DLT+HLT+HCFL+TT)+PST)                              
      MNX(12)=FAC3*(FAC1*(BD*DLL+HLL+     TL)+PSL)                              
      PN (12)=FAC3*(FAC1*(BD*DLN+HLN+     TN)+PSN)                              
C                                                                               
C     GP IV   CASE 3                                                            
      MNY(13)=FAC3*(FAC1*(DLT+HAT+HCFN+TT)+PST)                                 
      MNX(13)=FAC3*(FAC1*(DLL+HAL+     TL)+PSL)                                 
      PN (13)=FAC3*(FAC1*(DLN+HAN+     TN)+PSN)                                 
C                                                                               
      IF(N.EQ.1.OR.N.EQ.3) FAC3=1.0/1.4                                         
      IF(N.EQ.2.OR.N.EQ.4) FAC1=1.25                                            
C     GP V                                                                      
      MNY(14)=FAC3*(FAC1*(BD*DLT+WT+TT)+PST)                                    
      MNX(14)=FAC3*(FAC1*(BD*DLL+WL+TL)+PSL)                                    
      PN (14)=FAC3*(FAC1*(BD*DLN+WN+TN)+PSN)                                    
C                                                                               
C     GP VI   CASE 1                                                            
      MNY(15)=FAC3*(FAC1*(BD*DLT+HTT+HCFT+.3*WT+WLT+LFT+TT)+PST)                
      MNX(15)=FAC3*(FAC1*(BD*DLL+HTL+     .3*WL+WLL+LFL+TL)+PSL)                
      PN (15)=FAC3*(FAC1*(BD*DLN+HTN+     .3*WN+WLN+LFN+TN)+PSN)                
C                                                                               
C     GP VI   CASE 2                                                            
      MNY(16)=FAC3*(FAC1*(BD*DLT+HLT+HCFL+.3*WT+WLT+LFT+TT)+PST)                
      MNX(16)=FAC3*(FAC1*(BD*DLL+HLL+     .3*WL+WLL+LFL+TL)+PSL)                
      PN (16)=FAC3*(FAC1*(BD*DLN+HLN+     .3*WN+WLN+LFN+TN)+PSN)                
C                                                                               
C     GP VI   CASE 3                                                            
      MNY(17)=FAC3*(FAC1*(DLT+HAT+HCFN+.3*WT+WLT+LFT+TT)+PST)                   
      MNX(17)=FAC3*(FAC1*(DLL+HAL+     .3*WL+WLL+LFL+TL)+PSL)                   
      PN (17)=FAC3*(FAC1*(DLN+HAN+     .3*WN+WLN+LFN+TN)+PSN)                   
C                                                                               
      IF(N.NE.2) GO TO 140                                                      
C     GP VII  CASE 1                                                            
      MNY(18)=DABS(DLT+PST)+DABS(EQTT / ZZ)                                     
      MNX(18)=DABS(DLL+PSL)+DABS(EQTL / ZZ)                                     
      PN (18)=DLN+EQTN+PSN                                                      
      PN18   = PN(18)                                                           
      ARST =        DABS(EQTT) + DABS(DLT+PST)                                  
C                                                                               
C     GP VII  CASE 2                                                            
      MNY(19)=DABS(DLT+PST)+DABS(EQLT / ZZ)                                     
      MNX(19)=DABS(DLL+PSL)+DABS(EQLL / ZZ)                                     
      PN (19)=DLN+EQLN+PSN                                                      
      PN19   = PN(19)                                                           
      ARSL =      DABS(EQLL) + DABS(DLL  + PSL)                                 
C***********************************************************************        
C     CALCULATE SERVICE & FACTORED LOADS FOR COL. AND FOOTING                   
C***********************************************************************        
  140 DO 180 K=1,22                                                             
      IF(K.EQ.4) GO TO 180                                              .C.     
      IF(K.EQ.5) GO TO 180                                              .C.     
      IF(K.EQ.6) GO TO 180                                              .C.     
C                                                                               
      IF(IPIER.EQ.1) MNY(K) = 0.0                                               
C                                                                               
      IF(N.EQ.2) GO TO 150                                                      
      IF(N.EQ.3) GO TO 160                                                      
      IF(N.EQ.4) GO TO 170                                                      
C***********************************************************************        
C                  SERVICE LOADS FOR COLUMN  N=1                                
C***********************************************************************        
      IF(K.GE.4.AND.K.LE.6)   GO TO 180                                         
      IF(K.EQ.18)             GO TO 145                                         
      MYSC(K) = DABS(MNY(K))                                                    
      MXSC(K) = DABS(MNX(K))                                                    
      MSC(K)  = DSQRT(MNY(K)**2+MNX(K)**2)                                      
      PSC(K)  = PN(K)                                                           
      GO TO 180                                                                 
C***********************************************************************        
C                          AL1 SERVICE                                          
C***********************************************************************        
  145 MYSC(20) = DABS(MYS1)                                                     
      IF(IPIER.EQ.1) MYSC(20) = 0.0                                             
      MXSC(20) = DABS(MXS1)                                                     
      MSC (20) = DSQRT(MYS1**2+MXS1**2)                                         
      PSC (20) = PS1                                                            
C***********************************************************************        
C                          AL2 SERVICE                                          
C***********************************************************************        
      MYSC(21) = DABS(MYS2)                                                     
      IF(IPIER.EQ.1) MYSC(21) = 0.0                                             
      MXSC(21) = DABS(MXS2)                                                     
      MSC (21) = DSQRT(MYS2**2+MXS2**2)                                         
      PSC (21) = PS2                                                            
C***********************************************************************        
C          DEAD LOAD ONLY FOR COLUMN STRESS ANALYSIS                            
C***********************************************************************        
      MYSC(22) = DABS(DLT+PST)                                                  
      IF(IPIER.EQ.1) MYSC(22) = 0.0                                             
      MXSC(22) = DABS(DLL+PSL)                                                  
      MSC (22) = DSQRT(MYSC(22)**2+MXSC(22)**2)                                 
      PSC (22) = DLN+PSN                                                        
      GO TO 185                                                                 
C***********************************************************************        
C                   FACTORED LOADS FOR COLUMN  N=2                              
C***********************************************************************        
  150 IF(K.EQ.20) GO TO 155                                                     
      MYFCI(K) = DABS(MNY(K))                                                   
      MXFCI(K) = DABS(MNX(K))                                                   
      MFCI(K)  = DSQRT(MNY(K)**2+MNX(K)**2)                                     
      PFCI(K)  = PN(K)                                                          
C                                                                               
      BD = .75                                                                  
      IF(K.EQ.3.OR.K.EQ.6.OR.K.EQ.10.OR.K.EQ.13.OR.K.GE.17) BD = 1.00           
C                                                                               
      IF(K.GT.1) GO TO 151                                                      
      MYSC(22) = DABS(DLT+PST)                                                  
      IF(IPIER.EQ.1) MYSC(22) = 0.0                                             
      MXSC(22) = DABS(DLL+PSL)                                                  
C                                                                               
  151 BDY = 0.0                                                                 
      BDX = 0.0                                                                 
C                                                                               
      IF(MYFCI(K).GT.0.0) BDY = 1.3 * BD * MYSC(22)/MYFCI(K)                    
      IF(MXFCI(K).GT.0.0) BDX = 1.3 * BD * MXSC(22)/MXFCI(K)                    
      IF(BDY.GT.1.0) BDY = 1.0                                                  
      IF(BDX.GT.1.0) BDX = 1.0                                                  
C                                                                               
      CALL ADJUST(MNY(K),MNX(K),PN(K),YI,XI,HY,HX,                              
     *ZLEN,DAY,DAX,DBY,DBX,POC,ITIE,FC,PHI(K),ACT,K,                            
     *MAGY(K),MAGX(K),PCYC(K),PCXC(K),XIS,YIS,E,EC,KX,KY,AST,BDX,BDY,           
     *EIX(K),EIY(K),SLX,SLY,ITOP,BD,IPIER,PHIMIN)                       .B.     
C                                                                               
      MYFC(K) = DABS(MNY(K))                                                    
      MXFC(K) = DABS(MNX(K))                                                    
      MFC(K)  = DSQRT(MNY(K)**2+MNX(K)**2)                                      
      PFC(K)  = PN(K)                                                           
      GO TO 180                                                                 
C***********************************************************************        
C                          AL1 FACTORED                                         
C***********************************************************************        
  155 MYFC(20) = DABS(MYF1)                                                     
      IF(IPIER.EQ.1) MYFC(20) = 0.0                                             
      MXFC(20) = DABS(MXF1)                                                     
      MFC (20) = DSQRT(MYF1**2+MXF1**2)                                         
      PFC (20) = PF1                                                            
C***********************************************************************        
C                          AL2 FACTORED                                         
C***********************************************************************        
      MYFC(21) = DABS(MYF2)                                                     
      IF(IPIER.EQ.1) MYFC(21) = 0.0                                             
      MXFC(21) = DABS(MXF2)                                                     
      MFC (21) = DSQRT(MYF2**2+MXF2**2)                                         
      PFC (21) = PF2                                                            
      GO TO 185                                                                 
C***********************************************************************        
C                  SERVICE LOADS FOR FOOTING N=3                                
C***********************************************************************        
  160 IF(K.GE.22)           GO TO 185                                           
      IF(K.GE.4.AND.K.LE.6) GO TO 180                                           
      IF(K.GE.18.AND.K.LE.19) GO TO 180                                         
      IF(K.LT.20)  GO TO 161                                                    
      MNY(K) = MYSC(K)                                                          
      MNX(K) = MXSC(K)                                                          
      PN(K)  = PSC(K)                                                           
  161 MYSF(K) = 0.0                                                             
      MXSF(K) = 0.0                                                             
      MSF(K)  = 0.0                                                             
      IF(ENDY.EQ.PIN) GO TO 162                                                 
      MYSF(K) = DABS(MNY(K))                                                    
  162 IF(ENDX.EQ.PIN) GO TO 163                                                 
      MXSF(K) = DABS(MNX(K))                                                    
  163 MSF(K)  = DSQRT(MYSF(K)**2+MXSF(K)**2)                                    
      PSF(K)  = PN(K)                                                           
      GO TO 180                                                                 
C***********************************************************************        
C                   FACTORED LOADS FOR FOOTING N=4                              
C***********************************************************************        
  170 IF(K.GE.22) GO TO 185                                                     
      IF(K.GE.18.AND.K.LE.19) GO TO 180                                         
      IF(K.LT.20)  GO TO 171                                                    
      MNY(K) = MYFC(K)                                                          
      MNX(K) = MXFC(K)                                                          
      PN(K)  = PFC(K)                                                           
  171 MYFF(K) = 0.0                                                             
      MXFF(K) = 0.0                                                             
      MFF(K)  = 0.0                                                             
      IF(ENDY.EQ.PIN) GO TO 172                                                 
      MYFF(K) = DABS(MNY(K))                                                    
  172 IF(ENDX.EQ.PIN) GO TO 173                                                 
      MXFF(K) = DABS(MNX(K))                                                    
  173 MFF(K)  = DSQRT(MYFF(K)**2+MXFF(K)**2)                                    
      PFF(K)  = PN(K)                                                           
  180 CONTINUE                                                                  
C 180 WRITE(6,4001) N,K,MNY(K),MNX(K),PN(K),PCYC(K)                             
C4001 FORMAT(' IN LOAD - N,K,MNY,MNX,PN',2I5,4F10.0)                            
C                                                                               
  185 IF(IGP.EQ.1) RETURN                                                       
      IF(IGP.EQ.2.AND.N.EQ.2.AND.ITOP.EQ.1.AND.DBX.GT.0.98.AND.                 
     *IPIER.EQ.1)                                         GO TO 186             
      IF(IGP.EQ.2.AND.N.EQ.2.AND.ITOP.EQ.1.AND.IFIX.EQ.1) GO TO 190             
  186 IF(N.LT.4) GO TO 100                                                      
  190 DLN = DDLN                                                                
      RETURN                                                                    
C                                                                               
  999 STOP                                                                      
C                                                                               
      END                                                                       
      SUBROUTINE PAGE(IP,TITLE,DATE,NTIME)                                      
C                                                                               
C ************************************************                              
C     PRINT NEW PAGE TITLE                                                      
C ************************************************                              
C                                                                               
      IMPLICIT REAL*8 (A-H,O-Z)                                                 
      CHARACTER*40 TITLE                                                .D.
      CHARACTER*8 NTIME                                                 .D.
      CHARACTER*11 DATE                                                 .D.     
C     DIMENSION TITLE(1)                                                .D.     
C     WRITE(6,1000)(DATE(I),I=1,8),(NTIME(J),J=1,3),(TITLE(M),M=1,5),IP         
C000  FORMAT(1H1/7X,'DATE          :  ',8A1,10X,'TIME : ',I2,':',I2,':',        
C    .  I2//     7X,'PROJECT TITLE :  ',5A8,8X,'PAGE : ',I3)                    
C                                                                               
      WRITE(6,1000)TITLE,NTIME,DATE,IP                                  .D.     
1000  FORMAT(1H1,6X,'TITLE ',A40,1X,A8,1X,A11,1X,'PG.',I3)              .D.     
      IP=IP+1                                                                   
      RETURN                                                                    
      END                                                                       
      SUBROUTINE PILE (P,UMX,UMY,UM,IPB,FC,ACT,NP,NE,RE,MB)                     
      IMPLICIT REAL * 8 (A-H,O-Z)                                               
      DIMENSION P(10,1),UMX(10,1),UMY(10,1),UM(10,1),IPB(1),PUM(7),             
     * UM1(10,7),PMIN(7),RE(7)                                                  
C                                                                               
      PHI=.70                                                                   
      P1 = .10*FC*ACT                                                           
      DO 10 J=1,NE                                                              
      P2 = PHI*IPB(J)                                                           
      IF(P2.LE.P1) PMIN(J)=P2                                                   
      IF(P1.LE.P2) PMIN(J)=P1                                                   
   10 CONTINUE                                                                  
C                                                                               
      EPS=.1                                                                    
      DO 20 J=1,NE                                                              
      DO 15 I=1,NP                                                              
      UM1(I,J)=UM(I,J)                                                          
   15 CONTINUE                                                                  
      CALL DALI(PMIN(J),P,UM1,PUM(J),NP,EPS,IER)                                
      PMIN(J)=PMIN(J)*.70                                                       
      PUM(J) = PUM(J)*.70                                                       
   20 CONTINUE                                                                  
C                                                                               
      DO 50 J=1,NE                                                              
      DO 40 I=1,NP                                                              
      PHI=.70                                                                   
      IF(PHI*P(I,J).GE.PMIN(J)) GO TO 30                                        
      PHI = PHI + ((.90-PHI) * ((PMIN(J)-(PHI*P(I,J)))/PMIN(J)))                
      IF(P(I,J).LE.0.0) PHI=.90                                                 
C 30  WRITE(3,1000) I,PHI,P(I,J),UM(I,J)                                        
C     WRITE(9,1000) I,PHI,P(I,J),UM(I,J)                                        
C1000 FORMAT(' NP,PHI,P,UM ',I5,F10.4,2F10.2)                                   
  30  P(I,J)  =P(I,J)*PHI                                                       
      UMX(I,J)=UMX(I,J)*PHI                                                     
      UMY(I,J)=UMY(I,J)*PHI                                                     
      UM(I,J) =UM(I,J)*PHI                                                      
C     WRITE(3,1000) I,PHI,P(I,J),UM(I,J)                                        
C     WRITE(9,1000) I,PHI,P(I,J),UM(I,J)                                        
  40  CONTINUE                                                                  
  50  CONTINUE                                                                  
      DO 60 J=1,NE                                                              
      IPB(J)=IPB(J)*.70                                                         
  60  CONTINUE                                                                  
C                                                                               
      DO 95 J=1,NE                                                              
      DO 70 I=1,NP                                                              
      IF(P(I,J).GT.PMIN(J)) GO TO 70                                            
      MB=I                                                                      
      GO TO 80                                                                  
  70  CONTINUE                                                                  
C                                                                               
  80  M1=NP-MB                                                                  
      DO 90 JP=1,M1                                                             
      P(11-JP,J)=P(10-JP,J)                                                     
      UM(11-JP,J)=UM(10-JP,J)                                                   
      UMX(11-JP,J)=UMX(10-JP,J)                                                 
      UMY(11-JP,J)=UMY(10-JP,J)                                                 
  90  CONTINUE                                                                  
      P (MB,J)=PMIN(J)                                                          
      UM(MB,J)=PUM(J)                                                           
      ANG=RE(J)*3.14159265359/180.                                              
      UMX(MB,J)=PUM(J)*DCOS(ANG)                                                
      UMY(MB,J)=PUM(J)*DSIN(ANG)                                                
  95  CONTINUE                                                                  
      RETURN                                                                    
      END                                                                       
      SUBROUTINE PLOTS (RE,RP,P,UMX,UMY,UM,POC,NP,NE,                           
     *IPLOT,AST1,FC1,SY1,HEAD,HEAD2,NPROB,IGP,EO,IPILE,MB)                      
      IMPLICIT REAL*8(A-H,O-Z)                                                  
      CHARACTER*40 HEAD, HEAD2
      DIMENSION P(10,1),UMY(10,1),UM(10,1),UMX(10,1),                           
     *RE(1),RP(1),UM1(10,7)                                             .D.
C                                                                               
      ISY1=SY1                                                                  
      ISY1=ISY1+1                                                               
      IPOC=POC                                                                  
C                                                                               
C *** OPTION TO PRODUCE A PLOT OF:  ********                                    
C          1. PN  (NOMINAL AXIAL LOAD) VS. MN  (NOMINAL MOMENT)                 
C          2. PN         "             VS. MNX         "                        
C          3. PN         "             VS. MNY         "                        
C          4. MNY (NOMINAL MOMENT)     VS. MNX         "                        
C                                                                               
      GO TO (100,200,300,400),IPLOT                                             
C                                                                               
  200 DO 210 J=1,NE                                                             
      DO 210 I=1,NP                                                             
      UM1(I,J)=UMX(I,J)                                                         
  210 CONTINUE                                                                  
      GO TO 103                                                                 
C                                                                               
  300 DO 310 J=1,NE                                                             
      DO 310 I=1,NP                                                             
      UM1(I,J)=UMY(I,J)                                                         
  310 CONTINUE                                                                  
      GO TO 103                                                                 
C                                                                               
  100 DO 102 J=1,NE                                                             
      DO 101 I=1,NP                                                             
      UM1(I,J)=UM(I,J)                                                          
  101 CONTINUE                                                                  
  102 CONTINUE                                                                  
  103 CONTINUE                                                                  
      IF(NPROB.EQ.1) GO TO 160                                                  
      IF(NPROB.EQ.2) GO TO 160                                                  
C *** Y & X - AXIS CARD DATA ***                                                
C     READ(5,599) YMIN,YINCR,YLEN,(SNAME(I),I=1,6)                              
C     READ(5,600) XMIN,XMAX,XINCR                                               
      YINCR=0.0                                                                 
      IF(YINCR.GT.0.) GO TO 110                                                 
C                                                                               
      YMAX=0.0                                                                  
      YMIN=0.0                                                                  
      XMAX=0.0                                                                  
      XMIN=0.0                                                                  
      DO 10 J=1,NE                                                              
      DO 10 I=1,NP                                                              
      IF(P(I,J).LT.0.0.AND.IPILE.EQ.1) P(I,J)=0.0                               
      IF(P(I,J).GT.10.E14) GO TO 10                                             
      PIJ=P(I,J)                                                                
      UMIJ=UM1(I,J)                                                             
      IF(PIJ.GT.YMAX)  YMAX=PIJ                                                 
      IF(PIJ.LT.YMIN)  YMIN=PIJ                                                 
      IF(UMIJ.GT.XMAX) XMAX=UMIJ                                                
   10 CONTINUE                                                                  
C                                                                               
      X1=1.0                                                                    
      DIV=1000.                                                                 
      IF (YMAX.LT.5000.) DIV=100.                                               
      IF (YMAX.LT.500.) DIV=10.                                                 
      IF (YMIN.LT.DIV.AND.YMIN.GE.0.0) YMIN=0.0                                 
      IF(YMIN.EQ.0.0) GO TO 11                                                  
      IYMIN=(YMIN/DIV) + DSIGN(X1,YMIN)                                         
      YMIN=IYMIN*DIV                                                            
C                                                                               
   11 YLEN=8.0                                                                  
      YINCR=(YMAX-YMIN)/YLEN                                                    
      INCRY=(YINCR/DIV)+1.                                                      
      YINCR=INCRY*DIV                                                           
      Y=YMIN+(YINCR*7.0)                                                        
      IF(YMAX.LT.Y) YLEN=7.0                                                    
C                                                                               
      XLEN=7.0                                                                  
      DIV=1000.                                                                 
      IF (XMAX.LT.5000.) DIV=100.                                               
      IF (XMAX.LT.500.) DIV=10.                                                 
      XINCR=(XMAX-XMIN)/XLEN                                                    
      INCRX=(XINCR/DIV)+1.                                                      
      XINCR=INCRX*DIV                                                           
      X=XINCR*6.0                                                               
      IF(XMAX.LT.X) XLEN=6.0                                                    
C                                                                               
      XMIN=-.001                                                                
      XMAX=(XLEN*XINCR)+.1                                                      
C                                                                               
  110 CONTINUE                                                                  
C     WRITE(6,601)                                                              
C     WRITE(6,602) YMIN,YINCR,YLEN                                              
C     WRITE(6,603)                                                              
C     WRITE(6,604) XMIN,XMAX,XINCR                                              
C                                                                               
      IF(NPROB.EQ.3) GO TO 115                                                  
C *** WRITE HEADER ********                                                     
C     WRITE(8,810) (SNAME(N),N=1,6)                                             
      WRITE(8,810)                                                              
  115 IF(IPLOT.EQ.2) GO TO 130                                                  
      IF(IPLOT.EQ.3) GO TO 140                                                  
C *** TITLE,Y-AXIS,X-AXIS                                                       
      IF(IGP.EQ.0) WRITE(8,820) HEAD                                            
      IF(IGP.NE.0) WRITE(8,820) HEAD2
      WRITE(8,830) YMIN,YINCR,YLEN                                              
      IF(IGP.EQ.0.AND.IPILE.EQ.0) WRITE(8,840) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.0.AND.IPILE.EQ.1) WRITE(8,851) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.2) WRITE(8,847) XMIN,XMAX,XINCR                                 
      GO TO 150                                                                 
C                                                                               
  130 IF (IGP.EQ.0) WRITE(8,820) HEAD                                           
      IF (IGP.NE.0) WRITE(8,820) HEAD2
      WRITE(8,830) YMIN,YINCR,YLEN                                              
      IF(IGP.EQ.0.AND.IPILE.EQ.0) WRITE(8,842) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.0.AND.IPILE.EQ.1) WRITE(8,852) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.2) WRITE(8,848) XMIN,XMAX,XINCR                                 
      GO TO 150                                                                 
C                                                                               
  140 IF (IGP.EQ.0) WRITE(8,820) HEAD                                           
      IF (IGP.NE.0) WRITE(8,820) HEAD2
      WRITE(8,830) YMIN,YINCR,YLEN                                              
      IF(IGP.EQ.0.AND.IPILE.EQ.0) WRITE(8,843) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.0.AND.IPILE.EQ.1) WRITE(8,853) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.2) WRITE(8,849) XMIN,XMAX,XINCR                                 
C                                                                               
  150 IF(IPILE.EQ.0) WRITE(8,850)                                               
      IF(IPILE.EQ.1) WRITE(8,950)                                               
C *** DRAW VERTICAL GRID LINES ***                                              
      YMAX=YMIN + (YLEN*YINCR)                                                  
      X=-XINCR                                                                  
      YV=((XMAX-XMIN)/XINCR)+1.0                                                
      NV=YV                                                                     
      DO 20 I=1,NV                                                              
      X=X+XINCR                                                                 
      WRITE(8,750)X,YMIN                                                        
      WRITE(8,750)X,YMAX                                                        
      WRITE(8,751)                                                              
   20 CONTINUE                                                                  
C *** DRAW HORIZONTAL GRID LINES ***                                            
      Y=YMIN                                                                    
      NH=YLEN                                                                   
      DO 30 I=1,NH                                                              
      Y=Y+YINCR                                                                 
      WRITE(8,750)XMIN,Y                                                        
      WRITE(8,750)XMAX,Y                                                        
      WRITE(8,751)                                                              
   30 CONTINUE                                                                  
C *** WRITE SUBTITLE FOR AST,POC,FC,SY************                              
      IF(IPILE.EQ.1) GO TO 155                                                  
      WRITE(8,880) AST1,IPOC                                                    
      WRITE(8,881) FC1,ISY1,EO                                                  
      GO TO 160                                                                 
  155 IFC1=FC1*1000.                                                            
      IFC1=IFC1+1                                                               
      ISY1=ISY1*1000                                                            
      WRITE(8,882) IFC1,ISY1                                                    
C                                                                               
C *** PLOT OF PN VS. MN ***                                                     
  160 IF(IPILE.EQ.1) GO TO 165                                                  
      DO 50 J=1,NE                                                              
      N1=J-1                                                                    
      WRITE(8,890) N1,RE(J)                                                     
      DO 40 I=1,NP                                                              
      UMIJ=UM1(I,J)                                                             
      PIJ=P(I,J)                                                                
      IF(PIJ.GT.10.E14) GO TO 40                                                
      K=I-1                                                                     
      IF(K.EQ.0) GO TO 35                                                       
      PKJ=P(K,J)                                                                
      IF(PIJ.EQ.PKJ) GO TO 40                                                   
C *** PLOT THE Y AND X DATA POINTS ***                                          
   35 WRITE(8,900) N1,UMIJ,PIJ                                                  
C                                                                               
   40 CONTINUE                                                                  
C *** DRAW A LINE THROUGH THE DATA POINTS ***                                   
      WRITE(8,910)                                                              
   50 CONTINUE                                                                  
      IPLOT=0                                                                   
      RETURN                                                                    
C *** PLOT OF PN VS. MN ***  FOR A PILE INCLUDING PHI. (IPILE=1)                
  165 DO 195 J=1,NE                                                             
      N1=3                                                                      
C     WRITE(8,890) N1,RE(J)                                                     
      NI=1                                                                      
      NF=MB                                                                     
  170 DO 190 I=NI,NF                                                            
      UMIJ=UM1(I,J)                                                             
      PIJ=P(I,J)                                                                
      IF(PIJ.GT.10.E14) GO TO 190                                               
      K=I-1                                                                     
      IF(K.EQ.0) GO TO 185                                                      
      PKJ=P(K,J)                                                                
      IF(PIJ.EQ.PKJ) GO TO 190                                                  
C *** PLOT THE Y AND X DATA POINTS ***                                          
  185 WRITE(8,900) N1,UMIJ,PIJ                                                  
C                                                                               
  190 CONTINUE                                                                  
C *** DRAW A LINE THROUGH THE DATA POINTS ***                                   
      WRITE(8,910)                                                              
      IF(NF.EQ.NP) GO TO 195                                                    
      NI=MB                                                                     
      NF=NP                                                                     
      GO TO 170                                                                 
  195 CONTINUE                                                                  
      RETURN                                                                    
C                                                                               
  599 FORMAT(3F10.0,6A2)                                                        
  600 FORMAT(3F10.0)                                                            
  601 FORMAT(1H1,' PLOT - SCALE PARAMETERS',//,                         DAP     
     *'  YMIN (K)     YINCR (K/INCH)     YLEN (INCH)',/)                DAP     
  602 FORMAT(F10.2,9(' '),F10.2,6(' '),F10.2)                           .C. 
  603 FORMAT(//' XMIN (K-FT)     XMAX (K-FT)     XINCR (K-FT/INCH)'/)           
  604 FORMAT(F12.4,6(' '),F10.2,12(' '),F10.2)                          .C.      
  605 FORMAT(1H1,' PLOT - SCALE PARAMETERS',//,                         DAP     
     *' YMIN (K-FT)     YINCR (K-FT/INCH)     YLEN (INCH)',/)           DAP     
  810 FORMAT(' B430 ------------ 9756 TS STRUCTURES, GP=T     8.5',     .E.            
     *'  11.0')                                                         .E.        
  820 FORMAT(' T         .40     .40     .50 ',A40)                     .E.
C 820 FORMAT(' T         .40     .40     .50  PN VS MN')                .E.     
  822 FORMAT(' T         .40     .40     .50  PN VS MNX')               .E.     
  823 FORMAT(' T         .40     .40     .50  PN VS MNY')               .E.     
  830 FORMAT(' Y    ',3F8.1)                                            .E.    
  840 FORMAT(' X    ',F8.4,F8.1,F8.1,' MN - NOMINAL MOMENT  KIP-FT')    .E.         
  842 FORMAT(' X    ',F8.4,F8.1,F8.1,' MNX - NOMINAL MOMENT  KIP-FT')   .E.         
  843 FORMAT(' X    ',F8.4,F8.1,F8.1,' MNY - NOMINAL MOMENT  KIP-FT')   .E.         
  847 FORMAT(' X    ',F8.4,F8.1,F8.1,' MP - PROBABLE MOMENT  KIP-FT')   .E.         
  848 FORMAT(' X    ',F8.4,F8.1,F8.1,' MPX - PROBABLE MOMENT KIP-FT')   .E.         
  849 FORMAT(' X    ',F8.4,F8.1,F8.1,' MPY - PROBABLE MOMENT KIP-FT')   .E.         
  850 FORMAT(' N1       -.28     2.0     90. PN - NOMINAL AXIAL LOAD',  .E.      
     *2(' '),'KIPS')                                                    .E.               
  950 FORMAT(' N1        -.28     2.0     90. NOMINAL AXIAL LOAD',      .E.          
     *' MODIFIED BY PHI - KIPS')                                        .E.        
  851 FORMAT(' X    ',F8.4,F8.1,F8.1,' NOMINAL MOMENT MODIFIED BY PHI', .E.         
     *' - KIP FT')                                                      .E.        
  852 FORMAT(' X    ',F8.4,F8.1,F8.1,' MNX - MODIFIED BY PHI - KIP FT') .E.         
  853 FORMAT(' X    ',F8.4,F8.1,F8.1,' MNY - MODIFIED BY PHI - KIP FT') .E.         
C *** GRID LINES ***                                                    .E.        
  750 FORMAT(' 3',4(' '),2F8.1)                                         .E.        
  751 FORMAT(' L1')                                                     .E.         
C *** WRITE SUBTITLE ***                                                .E.        
  880 FORMAT(' S                              AST =',F7.2,' SQ IN',     .E.      
     *'    PMAX = ',I7,' K')                                            .E.      
  881 FORMAT(' S                              FC =',F5.2,' KSI',        .E.      
     *' FY =',I3,' KSI  EO = ',F5.3)                                    .E.        
  882 FORMAT(' S                              FC =',I5,' PSI     ',     .E.         
     *' FY =',I6,' PSI')                                                        
C                                                                               
C *** SYMBOL IDENTIFICATION ***                                                 
  890 FORMAT(' C',I1,29(' '),'ANGLE =',F5.1,' DEG')                     .E.                 
C                                                                               
C *** PLOT THE Y AND X DATA POINTS ***                                          
  900 FORMAT(' ',I1,4(' '),2F8.1)                                       .E.           
C                                                                               
C *** DRAW A CURVE BETWEEN DATA POINTS ***                                      
  910 FORMAT(' L5')                                                     .E.         
C                                                                               
C *** NEW PLOT ***  MNY VS. MNX  ***                                            
  400 CONTINUE                                                                  
C     READ (5,599) YMIN,YINCR,YLEN,(SNAME(N), N=1,6)                            
C     READ (5,600) XMIN,XMAX,XINCR                                              
      YINCR = 0.0                                                               
      IF(YINCR.GT.0.) GO TO 410                                                 
C                                                                               
      YMAX=0.0                                                                  
      YMIN=0.0                                                                  
      XMAX=0.0                                                                  
      XMIN=0.0                                                                  
      DO 60 I=1,NP                                                              
      DO 60 J=1,NE                                                              
      UMYIJ=UMY(I,J)                                                            
      IF(UMYIJ.GT.10.E14) GO TO 60                                              
      UMXIJ=UMX(I,J)                                                            
      IF(UMYIJ.GT.YMAX) YMAX=UMYIJ                                              
      IF(UMXIJ.GT.XMAX) XMAX=UMXIJ                                              
   60 CONTINUE                                                                  
C                                                                               
      DIV=1000.                                                                 
      IF (YMAX.LT.5000.) DIV=100.                                               
      IF (YMAX.LT.500.) DIV=10.                                                 
      YLEN=8.0                                                                  
      YINCR=(YMAX-YMIN)/YLEN                                                    
      INCRY=(YINCR/DIV)+1.0                                                     
      YINCR=INCRY*DIV                                                           
C                                                                               
      DIV=1000.                                                                 
      IF (XMAX.LT.5000.) DIV=100.                                               
      IF (XMAX.LT.500.) DIV=10.                                                 
      XLEN=7.0                                                                  
      XINCR=(XMAX-XMIN)/XLEN                                                    
      INCRX=(XINCR/DIV)+1.0                                                     
      XINCR=INCRX*DIV                                                           
C                                                                               
      IF(YINCR.GT.XINCR) XINCR=YINCR                                            
      IF(XINCR.GT.YINCR) YINCR=XINCR                                            
C                                                                               
      Y=YINCR*7.0                                                               
      IF(YMAX.LT.Y) YLEN=7.0                                                    
      X=XINCR*6.0                                                               
      IF(XMAX.LT.X) XLEN=6.0                                                    
C                                                                               
      XMAX=(XLEN*XINCR)+.1                                                      
      XMIN=-.001                                                                
  410 CONTINUE                                                                  
C 410 WRITE(6,605)                                                              
C     WRITE(6,602) YMIN,YINCR,YLEN                                              
C     WRITE(6,603)                                                              
C     WRITE(6,604) XMIN,XMAX,XINCR                                              
      IF(NPROB.EQ.3) GO TO 415                                                  
C***********  WRITE HEADER CARD *********                                       
C     WRITE(8,810) (SNAME(N), N=1,6)                                            
      WRITE(8,810)                                                              
C *** WRITE TITLE AND Y AND X AXIS DATA ***                                     
  415 IF(IGP.EQ.0) WRITE(8,824) HEAD                                            
      IF(IGP.NE.0) WRITE(8,824) HEAD2                                           
      WRITE(8,834) YMIN,YINCR,YLEN                                              
      IF(IGP.EQ.0.AND.IPILE.EQ.0) WRITE(8,844) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.0.AND.IPILE.EQ.1) WRITE(8,856) XMIN,XMAX,XINCR                  
      IF(IGP.EQ.2) WRITE(8,854) XMIN,XMAX,XINCR                                 
      IF(IGP.EQ.0.AND.IPILE.EQ.0) WRITE(8,845)                                  
      IF(IGP.EQ.0.AND.IPILE.EQ.1) WRITE(8,857)                                  
      IF(IGP.EQ.2) WRITE(8,855)                                                 
C *** DRAW VERTICLE GRID LINES ***                                              
      YMAX=YMIN + (YLEN*YINCR)                                                  
      X=-XINCR                                                                  
      YV=((XMAX-XMIN)/XINCR)+1.0                                                
      NV=YV                                                                     
      DO 70 I=1,NV                                                              
      X=X+XINCR                                                                 
      WRITE(8,750)X,YMIN                                                        
      WRITE(8,750)X,YMAX                                                        
      WRITE(8,751)                                                              
   70 CONTINUE                                                                  
C *** DRAW HOIRIZINTAL GRID LINES ***                                           
      Y=YMIN                                                                    
      NH=YLEN                                                                   
      DO 80 I=1,NH                                                              
      Y=Y+YINCR                                                                 
      WRITE(8,750)XMIN,Y                                                        
      WRITE(8,750)XMAX,Y                                                        
      WRITE(8,751)                                                              
   80 CONTINUE                                                                  
C *** PLOT OF MUX VS. MUY ***                                                   
      WRITE(8,880) AST1,IPOC                                                    
      WRITE(8,881) FC1,ISY1,EO                                                  
      DO 95 I=1,NP                                                              
      N1=I-1                                                                    
      RPI=RP(I)                                                                 
      IP=RPI*POC                                                                
      WRITE(8,894) N1,IP                                                        
      DO 90 J=1,NE                                                              
      UMXIJ=UMX(I,J)                                                            
      UMYIJ=UMY(I,J)                                                            
      IF(UMXIJ.GT.10.E14) GO TO 90                                              
      K=J-1                                                                     
      IF(K.EQ.0) GO TO 85                                                       
      UMXIK=UMX(I,K)                                                            
      IF(UMXIJ.EQ.UMXIK) GO TO 90                                               
   85 WRITE(8,900) N1,UMXIJ,UMYIJ                                               
   90 CONTINUE                                                                  
      WRITE(8,910)                                                              
   95 CONTINUE                                                                  
      IPLOT=0                                                                   
      RETURN                                                                    
  824 FORMAT('T         .40     .40     .50 ',A40)                      .D.  
C 824 FORMAT('T         .40     .40     .50 MNY VS MNX')                        
  834 FORMAT('Y    ',3F8.1)                                                     
  844 FORMAT('X    ',F8.4,F8.1,F8.1,' MNX - NOMINAL MOMENT  KIP-FT')            
  845 FORMAT('N1       -.28     2.0     90. MY - N0MINAL MOMENT',2X,            
     1'KIP-FT')                                                                 
  854 FORMAT('X    ',F8.4,F8.1,F8.1,' MPX - PROBABLE MOMENT  ',                 
     1'KIP-FT')                                                                 
  855 FORMAT('N1       -.28     2.0     90. MPY - PROBABLE MOMENT',             
     *'  KIP-FT')                                                               
  856 FORMAT('X    ',F8.4,F8.1,F8.1,' MNX - MODIFIED BY PHI  ',                 
     1'KIP-FT')                                                                 
  857 FORMAT('N1       -.28     2.0     90. MNY - MODIFIED BY PHI',             
     *'  KIP-FT')                                                               
  894 FORMAT('C',I1,28(' '),'P =',I7,' K')                                     
      END                                                                       
      SUBROUTINE REBAR (IREBAR,NSLOOP,LSNODE,NBAR,NAS,XS,YS,AS,IAS,             
     *X1,Y1,X2,Y2,RS,SPACE,BAREA,HX,HY,NNODE,MNODE,NLOOP,                       
     *XC,YC,ICON,DSO,ABAR,IBAR)                                                 
C                                                                               
      IMPLICIT REAL*8(A-H,O-Z)                                                  
C                                                                               
      DIMENSION LSNODE(1),XS(1),YS(1),AS(1),IAS(1),X1(1),Y1(1),                 
     *X2(1),Y2(1),RS(1),NNODE(1),XC(1),YC(1),ABAR(1)                            
C                                                                               
      DO 5 J=1,NBAR                                                             
      AS(J)=0.0                                                                 
    5 CONTINUE                                                                  
C                                                                               
      DO 10 J=1,NBAR                                                            
      IAS(J)=0.0                                                                
   10 CONTINUE                                                                  
C                                                                               
      GO TO (100,200,300,400,500),IREBAR                                        
C                                                                               
C***********************************************************************        
C                     MANUAL - STEEL REBARS                                     
C***********************************************************************        
C                                                                               
  100 ITOT = 0                                                                  
      NI=1                                                                      
      DO 160 L=1,NSLOOP                                                         
      READ (5,*)    XS(NI),YS(NI),AS(L),LSNODE(L)                       .B.     
      ITOT = ITOT + LSNODE(L)                                                   
      NF=NI+LSNODE(L)-1                                                         
      NI=NI+1                                                                   
      READ (5,*) (XS(J),YS(J),J=NI,NF)                                  .B.     
      NI=NI-1                                                                   
C                                                                               
      DO 150 J=NI,NF                                                            
      IAS(J)=L                                                                  
  150 CONTINUE                                                                  
      NI=NF+1                                                                   
  160 CONTINUE                                                                  
C                                                                               
  166 NI=1                                                                      
      DO 180 L=1,NSLOOP                                                         
      NF=NI+LSNODE(L)-1                                                         
      WRITE(6,2012) L,AS(L),LSNODE(L)                                           
C                                                                               
      IF(ITOT.NE.NBAR) GO TO 610                                                
                                                                                
      WRITE(6,2090) (J,XS(J),YS(J),J=NI,NF)                                     
      NI=NF+1                                                                   
  180 CONTINUE                                                                  
      GO TO 600                                                                 
C                                                                               
C***********************************************************************        
C                INTERSECTING LOOPS - STEEL REBAR                               
C***********************************************************************        
C                                                                               
  200 READ (5,*) RS(1),SPACE,AS(1)                                      .B.     
C                                                                               
      IF(DSO.EQ.0.0) DSO=2*RS(1)                                                
      IF(SPACE.LE.0.75*DSO) GO TO 210                                           
      WRITE(6,5024)                                                             
5024  FORMAT(//7X,                                                              
     *   '*** WARNING: FOR INTERSECTING STEEL LOOPS  ***'/                      
     *7X,'THE LOOP SPACING IS GREATER THAN 0.75 TIMES THE SPIRAL',              
     *   ' DIAMETER.')                                                          
C                                                                               
  210 S1=SPACE/2                                                                
      ANG=S1/RS(1)                                                              
      ANGLE1=DACOS(ANG)                                                        
      P1=(2*3.1415927)-2*ANGLE1                                                 
      P2=(3.1415927)-2*ANGLE1                                                   
      PT=2*P1+2*P2*(NSLOOP-2)                                                   
      THETA=PT/NBAR                                                             
      XBAR1=P2/THETA                                                            
      LBAR1=XBAR1                                                               
      XBAR11=LBAR1 + .50                                                        
      IF(XBAR1.GE.XBAR11) LBAR1 = LBAR1 + 1                                     
      LBAR2=NBAR/2-LBAR1*(NSLOOP-2)                                             
      NI=1                                                                      
      NSLRP=NSLOOP-1                                                            
      DO 250 L=1,NSLRP                                                          
      TRANS=(2*L-NSLOOP-1)*SPACE/2                                              
      LBAR=LBAR1                                                                
      ANG1=P2/LBAR                                                              
      IF(L.EQ.1.OR.L.EQ.NSLOOP)LBAR=LBAR2                                       
      IF(L.EQ.1.OR.L.EQ.NSLOOP)ANG1=P1/LBAR                                     
      DO 240 KL=1,LBAR                                                          
      ANG=ANGLE1+ANG1*(KL-1)                                                    
      XS(NI)=RS(1)*DCOS(ANG)+TRANS                                              
      YS(NI)=RS(1)*DSIN(ANG)                                                    
      XS(NI+1)= -XS(NI)                                                         
      YS(NI+1)= -YS(NI)                                                         
      NI=NI+2                                                                   
  240 CONTINUE                                                                  
  250 CONTINUE                                                                  
C                                                                               
      DO 270 J=1,NBAR                                                           
      IAS(J)=1                                                                  
  270 CONTINUE                                                                  
C                                                                               
      WRITE(6,2020) RS(1),SPACE,AS(1)                                           
      WRITE(6,2090) (J,XS(J),YS(J),J=1,NBAR)                                    
      GO TO 600                                                                 
C                                                                               
C***********************************************************************        
C                  SEMI-CIRCULAR ENDS - STEEL REBARS                            
C***********************************************************************        
C                                                                               
  300 READ(5,*) RS(1),SPACE,AS(1)                                       .B.     
      P1=RS(1)*3.1415927                                                        
      P2=SPACE                                                                  
      PT=2*(P1+P2)                                                              
      LBAR1=(NBAR-4)*(P1/PT)                                                    
      LBAR2=((NBAR-4)-2*(LBAR1))/2                                              
      ANGLE=-3.1415927/2                                                        
      ANGLEI=3.1415927/(LBAR1+1)                                                
      XS(1)=SPACE/2                                                             
      YS(1)=-RS(1)                                                              
      XS(2)=-XS(1)                                                              
      YS(2)= YS(1)                                                              
      NF=(2*LBAR1)+1                                                            
C                                                                               
      DO 310 J=3,NF,2                                                           
      ANGLE=ANGLE+ANGLEI                                                        
      XS(J)=RS(1)*DCOS(ANGLE)+XS(1)                                             
      YS(J)=RS(1)*DSIN(ANGLE)                                                   
      XS(J+1)=-XS(J)                                                            
      YS(J+1)= YS(J)                                                            
  310 CONTINUE                                                                  
C                                                                               
      XS(NF+2)= XS(1)                                                           
      YS(NF+2)=-YS(1)                                                           
      XS(NF+3)=-XS(1)                                                           
      YS(NF+3)=-YS(1)                                                           
      XINCR=P2/(LBAR2+1)                                                        
      NF=NF+4                                                                   
C                                                                               
      DO 320 J=NF,NBAR,2                                                        
      XS(J)=XS(J-2)-XINCR                                                       
      YS(J)=YS(J-2)                                                             
      XS(J+1)= XS(J)                                                            
      YS(J+1)=-YS(J)                                                            
  320 CONTINUE                                                                  
C                                                                               
      DO 330 J=1,NBAR                                                           
      IAS(J)=1                                                                  
  330 CONTINUE                                                                  
C                                                                               
      WRITE(6,2030) RS(1),SPACE,AS(1)                                           
      WRITE(6,2090) (J,XS(J),YS(J),J=1,NBAR)                                    
      GO TO 600                                                                 
C                                                                               
C***********************************************************************        
C             CONCENTRIC LOOPS - STEEL REBARS                                   
C***********************************************************************        
C                                                                               
  400 ITOT = 0                                                                  
      DO 420 L=1,NSLOOP                                                         
      READ (5,*) RS(L),AS(L),LSNODE(L)                                  .B.     
      IF(NSLOOP.EQ.1.AND.LSNODE(L).EQ.0) LSNODE(L) = NBAR                       
      ITOT = ITOT + LSNODE(L)                                                   
  420 CONTINUE                                                                  
C                                                                               
      NI=1                                                                      
      DO 440 L=1,NSLOOP                                                         
      NF=NI+LSNODE(L)-1                                                         
      ANGLEI=(2.*3.14159)/LSNODE(L)                                             
      ANGLE=-ANGLEI                                                             
C                                                                               
      DO 430 J=NI,NF                                                            
      ANGLE=ANGLE+ANGLEI                                                        
      XS(J)=RS(L)*DCOS(ANGLE)                                                   
      YS(J)=RS(L)*DSIN(ANGLE)                                                   
      IAS(J)=L                                                                  
  430 CONTINUE                                                                  
C                                                                               
      NI=NF+1                                                                   
  440 CONTINUE                                                                  
C                                                                               
      WRITE(6,2040) (L,RS(L),AS(L),LSNODE(L),L=1,NSLOOP)                        
C                                                                               
      IF(ITOT.NE.NBAR) GO TO 610                                                
C                                                                               
      WRITE(6,2090) (J,XS(J),YS(J),J=1,NBAR)                                    
      GO TO 600                                                                 
C                                                                               
C***********************************************************************        
C                      ROWS - STEEL REBAR                                       
C***********************************************************************        
C                                                                               
  500 ITOT = 0                                                                  
      DO 510 L=1,NSLOOP                                                         
      READ  (5,*) X1(L),Y1(L),X2(L),Y2(L),AS(L),LSNODE(L)               .B.     
      ITOT = ITOT + LSNODE(L)                                                   
      IF(AS(L).EQ.0) AS(L)=AS(L-1)                                              
      IF(LSNODE(L).EQ.0) LSNODE(L)=LSNODE(L-1)                                  
  510 CONTINUE                                                                  
C                                                                               
      NI=1                                                                      
      DO 540 L=1,NSLOOP                                                         
      NF=NI+LSNODE(L)-1                                                         
      XINCR = DABS(X1(L) - X2(L)) / (LSNODE(L) - 1)                             
      YINCR = DABS(Y1(L) - Y2(L)) / (LSNODE(L) - 1)                             
      IF(X1(L).GT.X2(L)) XINCR=-XINCR                                           
      IF(Y1(L).GT.Y2(L)) YINCR=-YINCR                                           
      XS(NI) = X1(L)                                                            
      YS(NI) = Y1(L)                                                            
      IAS(NI) = L                                                               
      NI = NI + 1                                                               
C                                                                               
      DO 530 K=NI,NF                                                            
      XS(K) = XS(K-1) + XINCR                                                   
      YS(K) = YS(K-1) + YINCR                                                   
      IAS(K)= L                                                                 
  530 CONTINUE                                                                  
C                                                                               
      NI=NF+1                                                                   
  540 CONTINUE                                                                  
C                                                                               
      WRITE(6,2050)                                                             
      WRITE(6,2051)                                                             
      DO 570 L=1,NSLOOP                                                         
      WRITE(6,2052) X1(L),Y1(L),X2(L),Y2(L),AS(L),LSNODE(L),L                   
  570 CONTINUE                                                                  
C                                                                               
      IF(ITOT.NE.NBAR) GO TO 610                                                
C                                                                               
      WRITE(6,2090) (J,XS(J),YS(J),J=1,NBAR)                                    
C                                                                               
  600 BAREA=AS(1)                                                               
      IBAR = 1                                                                  
      IBAR = 1                                                                  
      DO 605 I = 3,18                                                           
      IF(I.EQ.12.OR.I.EQ.13.OR.I.EQ.15.OR.I.EQ.16.OR.I.EQ.17) GO TO 605         
      IF(AS(1).GT.0.999*ABAR(I).AND.AS(1).LT.1.001*ABAR(I)) IBAR = I            
      IF(AS(1).GT.1.999*ABAR(I).AND.AS(1).LT.2.001*ABAR(I)) IBAR = I            
C     WRITE(6,4000) AS(1), ABAR(I), IBAR                                        
C4000 FORMAT(' AS(1), ABAR(I), IBAR ',2F15.10,I3)                               
  605 CONTINUE                                                                  
C                                                                               
      IF(IBAR.NE.1) WRITE(6,3000) IBAR                                          
 3000 FORMAT(//7X,'THE MAIN LONGITUDINAL STEEL IS ASSUMED TO BE ',              
     *'# ',I2,' BARS.'//)                                                       
C                                                                               
      IF(IBAR.EQ.1) WRITE(6,3010)                                               
 3010 FORMAT(//7X,                                                              
     *'*** WARNING A NON-STANDARD STEEL AREA HAS BEEN USED.'//)                 
C                                                                               
      RETURN                                                                    
C                                                                               
  610 WRITE(6,6000) NBAR, ITOT                                                  
 6000 FORMAT(//4X,                                                              
     *   '** ERROR: TOTAL NUMBER OF STEEL BARS (',I3,') NOT EQUAL TO'/          
     *7X,'TO TOTAL NUMBER OF STEEL BARS IN THIS SECTION (',I3,')!'/             
     *7X,'CORRECT INPUT DATA AND RERUN.  EXECUTION TERMINATED . . .')           
      STOP                                                                      
C                                                                               
 1000 FORMAT(A1)                                                                
C1005 FORMAT(2F10.2,20X,1F10.0,I10)                                     .B.     
C1010 FORMAT(2F10.0,20X,1F10.0)                                         .B.     
C1012 FORMAT(2F10.0)                                                    .B.     
C1014 FORMAT(2F10.0,20X,2F10.0)                                         .B.     
C1030 FORMAT(2F10.0,20X,1F10.0)                                         .B.     
C1040 FORMAT(1F10.0,30X,F10.0,I10)                                      .B.     
C1050 FORMAT(5F10.0,I10)                                                .B.     
C                                                                               
 2010 FORMAT(//6X,' MANUAL INPUT PATTERN - STEEL REBARS'//                      
     *6X,'  BAR AREA'/6X,(F10.2))                                               
 2012 FORMAT(//6X,' MANUAL INPUT PATTERN - STEEL REBARS'//                      
     *6X,' GROUP  BAR AREA     NBARS'/6X,(I4,2X,1F10.2,I8))                     
 2020 FORMAT(//6X,' INTERSECTING LOOP PATTERN'//                                
     *6X,'    RADIUS   SPACING      AREA'/6X,(3F10.2))                          
 2030 FORMAT(//6X,' SEMI-CIRCULAR AT EACH END'//                                
     *6X,'    RADIUS   SPACING      AREA'/6X,(3F10.2))                          
 2040 FORMAT(//6X,' CONCENTRIC CIRCLE PATTERN'//                                
     *6X,' LOOP    RADIUS      AREA      BARS'/(6X,I5,2F10.2,I10))              
 2050 FORMAT(//6X,' ROW PATTERN')                                               
 2051 FORMAT(//,14X,                                                            
     *'X1',8X,'Y1',8X,'X2',8X,'Y2',6X,'AREA',6X,'BARS',7X,'ROW'/)               
 2052 FORMAT(6X,5F10.2,2I10)                                                    
 2090 FORMAT(//6X,' STEEL REBAR COORDINATES  (INCHES)'//                        
     *6X,' COORD',7X,'X',9X,'Y'//(6X,I4,2X,2F10.2))                             
 2091 FORMAT(//6X,' STEEL REBAR COORDINATES  (INCHES)'//                        
     *6X,' COORD',7X,'X',9X,'Y',8X,'AREA'//(6X,I4,2X,3F10.2))                   
      END                                                                       
      SUBROUTINE SHEAR (FC,IREBAR,PT,PB,V,UMT,UMB,NC,IPAGE,HEAD,                
     * RNDATE,NHTIME,SPACE, RS, DSO, NSLOOP, HX, HY, ILONG,                     
     * ARST, ARSL, PN18, PN19, CGS, CSP, ZLEN, BDIA, IBAR, IPIER)               
      IMPLICIT REAL*8(A-H,O-Z)                                                  
C                                                                               
      CHARACTER*40 HEAD                                                 .D. 
      CHARACTER*11 RNDATE                                               .D.     
      CHARACTER*8 NHTIME                                                .D.     
      DIMENSION RS(1), PT(1), PB(1), V(1), UMT(1), UMB(1), BDIA(1)              
C     DIMENSION HEAD(1)                                                 .D.     
C                                                                               
C VS   = VOLUME  OF SPIRAL                                                      
C AC   = AREA OF CORE                                                           
C AG   = GROSS AREA BASSED ON 2" OF COVER                                       
C RMAX = MAX VOLUMETRIC RATIO                                                   
C SPC  = SPACING FOR CONFINEMENT                                                
C SPS  = SPACING FOR MAX SHEAR                                                  
C SPM  = SPACING FOR MIN SHEAR                                                  
C ILONG= 0 TRANSVERE ANALYSIS                                                   
C ILONG= 1 LONGITUDINAL ANALYSIS                                                
C UNITS  LBS-INCHES                                                             
C                                                                               
      AV4 = .20                                                                 
      AV5 = .31                                                                 
      AV6 = .44                                                                 
      AV7 = .60                                                                 
      AV8 = .79                                                                 
      IF(ILONG.EQ.1) GO TO 70                                                   
      RSO=DSO/2                                                                 
C                                                                               
C ******** AREA OF CONCRETE CORE - CONC. LOOPS - IREBAR = 4 ************        
C                                                                               
      AC =  3.1415927 * (RSO)**2                                                
      AG =  3.1415927 * (RSO+2.0)**2                                            
      P1 = 0.0                                                                  
      IF(IREBAR.EQ.4) GO TO 10                                                  
C                                                                               
C ******** AREA OF CONCRETE CORE - INTER. LOOPS - IREBAR = 2 ***********        
C                                                                               
      P1 = (NSLOOP-1) * SPACE                                                   
      PT4 = (2 * P1) + (3.1415927 * (DSO -  .56))                               
      PT5 = (2 * P1) + (3.1415927 * (DSO -  .69))                               
      PT6 = (2 * P1) + (3.1415927 * (DSO -  .88))                               
      PT7 = (2 * P1) + (3.1415927 * (DSO - 1.00))                               
      PT8 = (2 * P1) + (3.1415927 * (DSO - 1.13))                               
      AC  = AC + (P1 * DSO)                                                     
      AG  = AG + (P1 * (DSO+4.0))                                               
      GO TO 30                                                                  
C                                                                               
C ********* VOL OF STEEL - CONCENTRIC LOOPS - IREBAR = 4 ***************        
C                                                                               
   10 VST4 =  3.1415927 * (DSO -  .56) * AV4                                    
      VST5 =  3.1415927 * (DSO -  .69) * AV5                                    
      VST6 =  3.1415927 * (DSO -  .88) * AV6                                    
      VST7 =  3.1415927 * (DSO - 1.00) * AV7                                    
      VST8 =  3.1415927 * (DSO - 1.13) * AV8                                    
      GO TO 60                                                                  
C                                                                               
C ******* VOL OF STEEL - INTERSECTING LOOPS - IREBAR = 2 ***************        
C                                                                               
   30 VST4 = PT4 * AV4                                                          
      VST5 = PT5 * AV5                                                          
      VST6 = PT6 * AV6                                                          
      VST7 = PT7 * AV7                                                          
      VST8 = PT8 * AV8                                                          
C                                                                               
   60 FC1=FC/1000                                                               
      FYH=60.                                                                   
      AG1=AG/144.                                                               
      AC1=AC/144.                                                               
      BT =       DSO                                                            
      DT = .80 * DSO                                                            
      IF(IREBAR.EQ.2) DT = .80 * (P1 + DSO)                                     
C                                                                               
      BL =       DSO                                                            
      IF(IREBAR.EQ.2) BL =       (P1 + DSO)                                     
      DL = .80 * DSO                                                            
      WRITE(6,3000) NC,DSO,CGS,CSP,FC1,FYH,AG1,AC1,BT,DT,BL,DL                  
C                                                                               
 3000 FORMAT(/7X,'DETERMINATION OF COLUMN LATERAL REINFORCEMENT'//              
     *7X,'NUMBER OF COLUMNS IN BENT                 =',I5/                      
     *7X,'OUT TO OUT DISTANCE (DIAMETER) OF'/                                   
     *7X,'LATERAL REINFORCEMENT                     =',F7.2,' IN'/              
     *7X,'DISTANCE FROM TOP COLUMN PLASTIC HINGE '/                             
     *7X,'TO CENTER OF GRAVITY OF SUPERSTURCTURE    =',F7.2,' FT'/              
     *7X,'CENTER TO CENTER SPACING OF COLUMNS       =',F7.2,' FT'/              
     *7X,'COMPRESSIVE STRENGTH OF CONCRETE    - FC  =',F7.2,' KSI'/             
     *7X,'YIELD STRENGTH OF REINFORCEMENT     - FY  =',F7.2,' KSI'/             
     *7X,'CONCRETE GROSS AREA (2" COVER)      - AG  =',F7.2,' FT**2'/           
     *7X,'CONCRETE CORE AREA                  - AC  =',F7.2,' FT**2'/           
     *7X,'TRANSVERSE DIRECTION                - BW  =',F7.2,' IN'/              
     *7X,'TRANSVERSE DIRECTION                - D   =',F7.2,' IN'/              
     *7X,'LONGITUDINAL DIRECTION              - BW  =',F7.2,' IN'/              
     *7X,'LONGITUDINAL DIRECTION              - D   =',F7.2,' IN')              
                                                                                
      WRITE(6,3002)                                                             
 3002 FORMAT(//7X,                                                              
     *   'AMOUNT OF REINFORCEMENT CONFORMS TO THE FOLLOWING',                   
     *' BRIDGE DESIGN '/                                                        
     *7X,'SPECIFICATIONS:'//                                                    
     *7X,'CASE A - CONFINEMENT REINFORCEMENT    -  (CODE 8.18.2.2)'/            
     *8X,'        PS1 = .45 (AG/AC - 1) FC/FY (.5 + 1.25 P/(FC * AG))'/         
     *8X,'        PS2 =             .12 FC/FY (.5 + 1.25 P/(FC * AG))'/         
     *7X,'         PS3 = .45 (AG/AC - 1) FC/FY'//                               
     *7X,'CASE B - APPLIED SHEAR REINFORCEMENT  -  (CODE 8.16.6)'/              
     *7X,'         VU <= PHI * VN                (8-46)'/                       
     *7X,'         VN  = VC + VS                 (8-47)'/                       
     *7X,'         VC  = 2 * SQRT(FC) * BW * D   (8-51)'/                       
     *7X,'         VS  = (AV * FY * D) / S       (8-53)'//                      
     *7X,'CASE C - MINIMUM SHEAR REINFORCEMENT  -  (CODE 8.19.1.1)'/            
     *7X,'         AV = (50 * BW * S)/FY         (8-63)')                       
C                                                                               
                                                                                
                                                                                
C                                                                               
C ************* EFFECTIVE SHEAR AREA - (AV) ****************************        
C                                                                               
   70 AV4=2*AV4                                                                 
      AV5=2*AV5                                                                 
      AV6=2*AV6                                                                 
      AV7=2*AV7                                                                 
      AV8=2*AV8                                                                 
      IF(ILONG.EQ.0)  GO TO 72                                                  
      IF(IREBAR.EQ.4) GO TO 72                                                  
      AV4=NSLOOP*AV4                                                            
      AV5=NSLOOP*AV5                                                            
      AV6=NSLOOP*AV6                                                            
      AV7=NSLOOP*AV7                                                            
      AV8=NSLOOP*AV8                                                            
C                                                                               
   72 B = DSO                                                                   
      IF(IREBAR.EQ.2.AND.ILONG.EQ.1) B = (P1 + DSO)                             
C                                                                               
      D = .80 * (P1 + DSO)                                                      
      IF(IREBAR.EQ.2.AND.ILONG.EQ.1) D = .80 * DSO                              
C                                                                               
      DO 95 N=1,NC                                                              
      VU = V(N)                                                                 
      VN = VU / .85                                                             
      VC = 2 * DSQRT(FC) * B * D / 1000.                                        
      PTN = PT(N) * 1000.                                                       
      IF(PTN.LT.0.0) PTN = 0.0                                                  
      PBS = PTN/AC                                                              
      IF(PBS.GE.0.1*FC) GO TO 73                                                
      VC= (PBS/(.1*FC))*VC                                                      
C                                                                               
   73 VS = VN - VC                                                              
      IF(VS.LT.0) VS = 0.0                                                      
      VCF = .85 * VC                                                            
      VSF = .85 * VS                                                            
C                                                                               
      IF(ILONG.EQ.0.AND.IPIER.EQ.1) GO TO 96                                    
C                                                                               
      CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)                                       
      IF(ILONG.EQ.0) WRITE(6,3010)                                              
      IF(ILONG.EQ.1) WRITE(6,3020)                                              
      WRITE(6,3030) N,PT(N),UMT(N),PB(N),UMB(N),VU,VCF,VSF                      
 3030 FORMAT(//,                                                                
     *7X,'LOADS RESULTING FROM PLASTIC HINGING',                                
     *' OF COLUMN NO.',I2//                                                     
     *7X,'TOP OF COLUMN    AXIAL FORCE              P =',F7.0,' KIPS'/          
     *7X,'                 PLASTIC MOMENT          MP =',F7.0,' K-FT'/          
     *7X,'BOTTOM OF COLUMN AXIAL FORCE              P =',F7.0,' KIPS'/          
     *7X,'                 PLASTIC MOMENT          MP =',F7.0,' K-FT'//         
     *7X,'DESIGN SHEAR FORCE:                      VU =',F7.0,' KIPS'/          
     *7X,'CONCRETE FACTORED SHEAR STRENGTH:  .85 * VC =',F7.0,' KIPS'/          
     *7X,'STEEL    FACTORED SHEAR STRENGTH:  .85 * VS =',F7.0,' KIPS')          
C                                                                               
      VSMAX = 8.0 * DSQRT(FC) * B * D / 1000.                                   
      IF(VS.GE.VSMAX) WRITE(6,3040) VS,VSMAX                                    
 3040 FORMAT(/,                                                                 
     *7X,'*************************************'/                               
     *7X,'*** ERROR: VS > 8 * SQRT(FC) * B * D '/                               
     *7X,'*************************************'//                              
     *7X,F10.0,' KIPS  > ',F10.0,' KIPS (CODE 8.16.6.3.9)')                     
C                                                                               
      IF(VC.LT.2.*DSQRT(FC)*B*D/1000) WRITE(6,3050)                             
 3050 FORMAT(/7X,'NOTE: THE AVERAGE COMPRESSIVE STRESS (P/AC) IS',              
     *' LESS THAN .10 * FC.'/                                                   
     *7X,'THEREFORE, VC IS LESS THAN 2 * SQRT(FC) * B * D  ',                   
     *'(CODE 8.16.6.11.4(A))')                                                  
C                                                                               
C  **************** CONFINEMENT - ASSHTO ART. 8.18.2.2) ****************        
C                                                                               
      PBN=PB(N) * 1000.                                                         
      IF(PBN.LT.0.0) PBN=0.0                                                    
      FAC1 = (FC/60000.)*(0.5+((1.25*PBN)/(FC*AG)))                             
      FAC2 = .45*((AG/AC)-1)                                                    
C                                                                               
      RS1 = FAC2*FAC1                                                           
      RS2 = .12*FAC1                                                            
      RS3 = FAC2*(FC/60000.)                                                    
C                                                                               
      IF(RS1.GT.RS2)     RMAX=RS1                                               
      IF(RS2.GT.RS1)     RMAX=RS2                                               
      IF(RS3.GT.RMAX)    RMAX=RS3                                               
C                                                                               
      IF(RMAX.EQ.RS1)       NRS=1                                               
      IF(RMAX.EQ.RS2)       NRS=2                                               
      IF(RMAX.EQ.RS3)       NRS=3                                               
C                                                                               
      SPC4 = VST4/(AC*RMAX)                                                     
      SPC5 = VST5/(AC*RMAX)                                                     
      SPC6 = VST6/(AC*RMAX)                                                     
      SPC7 = VST7/(AC*RMAX)                                                     
      SPC8 = VST8/(AC*RMAX)                                                     
C                                                                               
C **************** SHEAR STRESS - ASSHTO ART. 8.16.6.1) ****************        
C                                                                               
      SPS4=10.E10                                                               
      SPS5=10.E10                                                               
      SPS6=10.E10                                                               
      SPS7=10.E10                                                               
      SPS8=10.E10                                                               
      SPM4=10.E10                                                               
      SPM5=10.E10                                                               
      SPM6=10.E10                                                               
      SPM7=10.E10                                                               
      SPM8=10.E10                                                               
C                                                                               
      IF(VU.LT.VCF) GO TO 80                                                    
      FAC = 60. * D / VS                                                        
      SPS4 = AV4*FAC                                                            
      SPS5 = AV5*FAC                                                            
      SPS6 = AV6*FAC                                                            
      SPS7 = AV7*FAC                                                            
      SPS8 = AV8*FAC                                                            
C                                                                               
C *********** MINIMUM SHEAR - ASSHTO ART. 8.16.19.1 ********************        
C                                                                               
   80 IF(VU.LT.0.5*VCF) GO TO 90                                                
      FACC = 60000./(50.*B)                                                     
      SPM4=AV4*FACC                                                             
      SPM5=AV5*FACC                                                             
      SPM6=AV6*FACC                                                             
      SPM7=AV7*FACC                                                             
      SPM8=AV8*FACC                                                             
C                                                                               
C ************************* RESULTS ************************************        
C                                                                               
   90 WRITE(6,3060)SPC4,SPS4,SPM4,RMAX,NRS,                                     
     *             SPC5,SPS5,SPM5,RMAX,NRS,                                     
     *             SPC6,SPS6,SPM6,RMAX,NRS,                                     
     *             SPC7,SPS7,SPM7,RMAX,NRS,                                     
     *             SPC8,SPS8,SPM8,RMAX,NRS                                      
C                                                                               
      IF(VU.LE.VCF) WRITE(6,3070) VU,VCF                                        
C                                                                               
      IF(SPM4.EQ.10.E10) WRITE(6,3080) VU,VCF                                   
C                                                                               
   95 CONTINUE                                                                  
C                                                                               
      IF(ILONG.EQ.0.AND.DABS(ARST).LT.UMT(NC).AND.UMB(NC).EQ.0)                 
     * WRITE(6,3090) ARST, UMT(NC)                                              
      IF(ILONG.EQ.1.AND.DABS(ARSL).LT.UMT(NC).AND.UMB(NC).EQ.0)                 
     * WRITE(6,3090) ARSL, UMT(NC)                                              
C                                                                               
      IF(ILONG.EQ.0.AND.DABS(ARST).LT.UMB(NC)) WRITE(6,3090)ARST,UMB(NC)        
      IF(ILONG.EQ.1.AND.DABS(ARSL).LT.UMB(NC)) WRITE(6,3090)ARSL,UMB(NC)        
 3090 FORMAT(//7X,                                                              
     *  'NOTE:'//                                                               
     *7X,'THE UNREDUCED ELASTIC ARS + DEAD LOAD MOMENT ',                       
     *'(',F7.0,' K-FT)'/                                                        
     *7X,'IS LESS THAN THE PROBABLE PLASTIC MOMENT (',F7.0,' K-FT)'/            
     *7X,'THEREFORE, PLASTIC HINGING DOES NOT CONTROL.'/                        
     *7X,'(CODE 8.16.6.11.1)'//                                                 
     *7X,'THE DESIGNER MUST CHECK THE SHEAR REINFORCEMENT (CASE B)'/            
     *7X,'FOR GROUP LOADS I TO VII'/                                            
     *7X,'(USE UNREDUCED ELASTIC ARS + DEAD LOAD SHEAR FOR GROUP VII)')         
C                                                                               
   96 SPCY = HY * 12. / 5.0                                                     
      IF(IBAR.NE.1) SPCB = 6.0 * BDIA(IBAR)                                     
      IF(IBAR.EQ.1) SPCB = 8.0                                                  
      IF(SPCY.GE.SPCB) SPCM = SPCB                                              
      IF(SPCB.GE.SPCY) SPCM = SPCY                                              
      IF(SPCM.GE.8.0)  SPCM = 8.0                                               
C******* SHEAR CAPACITY                       (MAX SPACING) ************        
      VCF = .85 * 2 * DSQRT(FC) * B * D / 1000.                                 
      FAC = .85 * 60. * D                                                       
      VSS4= AV4 * FAC / SPCM  + VCF                                             
      VSS5= AV5 * FAC / SPCM  + VCF                                             
      VSS6= AV6 * FAC / SPCM  + VCF                                             
      VSS7= AV7 * FAC / SPCM  + VCF                                             
      VSS8= AV8 * FAC / SPCM  + VCF                                             
C****** SHEAR CAPACITY (1.5 * 1.00" AGGREGATE) (MIN SPACING) ***********        
      VS14= AV4 * FAC / 2.000 + VCF                                             
      VS15= AV5 * FAC / 2.125 + VCF                                             
      VS16= AV6 * FAC / 2.250 + VCF                                             
      VS17= AV8 * FAC / 2.375 + VCF                                             
      VS18= AV8 * FAC / 2.500 + VCF                                             
C                                                                               
      VMAX = .85 * 10.0 * DSQRT(FC) * B * D / 1000.                             
C                                                                               
      IF(VSS4.GT.VMAX) VSS4 = VMAX                                              
      IF(VSS5.GT.VMAX) VSS5 = VMAX                                              
      IF(VSS6.GT.VMAX) VSS6 = VMAX                                              
      IF(VSS7.GT.VMAX) VSS7 = VMAX                                              
      IF(VSS8.GT.VMAX) VSS8 = VMAX                                              
      IF(VS14.GT.VMAX) VS14 = VMAX                                              
      IF(VS15.GT.VMAX) VS15 = VMAX                                              
      IF(VS16.GT.VMAX) VS16 = VMAX                                              
      IF(VS17.GT.VMAX) VS17 = VMAX                                              
      IF(VS18.GT.VMAX) VS18 = VMAX                                              
C                                                                               
      CALL PAGE(IPAGE,HEAD,RNDATE,NHTIME)                                       
      IF(ILONG.EQ.0.AND.IREBAR.EQ.2) WRITE(6,3010)                              
      IF(ILONG.EQ.1.AND.IREBAR.EQ.2) WRITE(6,3020)                              
C                                                                               
      WRITE(6,3092)                                                             
 3092 FORMAT(//                                                                 
     *7X,'THE MAXIMUM SPACING OF LATERAL REINFORCEMENT SHALL NOT ',             
     *   'EXCEED ONE-FIFTH'/                                                    
     *7X 'OF THE LEAST DIMENSION OF THE COLUMN, 6 TIMES THE NOMINAL ',          
     *   'DIAMETER OF THE'/                                                     
     *7X,'LONGITUDINAL REINFORCEMENT, OR 8 INCHES. (8.21.1.1)'//                
     *7X,'THE MINIMUM SPACING SHALL NOT BE LESS THAN 1.5 BAR ',                 
     *   'DIAMETERS,'/                                                          
     *7X,'1.5 TIMES THE MAXIMUM SIZE COARSE AGGREGATE, OR 1.5 INCHES.',         
     *   ' (8.21.1)')                                                           
C                                                                               
      WRITE(6,3093) SPCM,VSS4,VS14, SPCM,VSS5,VS15,                             
     *              SPCM,VSS6,VS16, SPCM,VSS7,VS17,                             
     *              SPCM,VSS8,VS18                                              
 3093 FORMAT(//7X,                                                              
     *'FACTORED SHEAR STRENGTH  .85 * (VS + VC) BASED ON MAXIMUM AND ',         
     *'MINIMUM '/                                                               
     *7X,'CENTER TO CENTER SPACING (PITCH) OF LATERAL REINFORCEMENT:'//         
     *7X,'                                1.0" AGG          '/                  
     *7X,' BAR          MAX     SHEAR          MIN     SHEAR'/                  
     *7X,'SIZE        PITCH  STRENGTH        PITCH  STRENGTH'/                  
     *7X,'             (IN)    (KIPS)         (IN)    (KIPS)'/                  
     *7X,'  4 ',8X,F5.3,F10.0,8X,'2.000',F10.0/                                 
     *7X,'  5 ',8X,F5.3,F10.0,8X,'2.125',F10.0/                                 
     *7X,'  6 ',8X,F5.3,F10.0,8X,'2.250',F10.0/                                 
     *7X,'  7 ',8X,F5.3,F10.0,8X,'2.375',F10.0/                                 
     *7X,'  8 ',8X,F5.3,F10.0,8X,'2.500',F10.0)                                 
C                                                                               
      IF(IBAR.EQ.1) WRITE(6,3094)                                               
 3094 FORMAT(//,                                                                
     *7X,'*** WARNING THE MAIN LONGITUDINAL BARS ARE A NON-STANDARD ',          
     *   'SIZE.'/                                                               
     *7X,'    THE DESIGNER MUST CHECK THE MAX LATERAL BAR SPACING ',            
     *       'BASED ON'/                                                        
     *7X,'    6 TIMES THE NOMINAL BAR DIAMETER OF THE MAIN ',                   
     *       'LONGITUDINAL BARS.')                                              
C                                                                               
      PAC = .10 * FC * AC / 1000.                                               
      WRITE(6,3095) VCF,PAC,VCF,VMAX                                            
 3095 FORMAT(//,                                                                
     *7X,'NOTE:'//                                                              
     *7X,'THE ABOVE FACTORED SHEAR STRENGTH VALUES ARE BASED ON A'/             
     *7X,'FACTORED CONCRETE SHEAR STRENGTH (.85 * VC)',                         
     *' OF',F5.0,' KIPS.'//                                                     
     *7X,'IF THE APPLIED AXIAL LOAD IS LESS THAN ',F5.0,' KIPS,'/               
     *7X,'THEN THE FACTORED CONCRETE SHEAR STRENGTH DECREASES FROM'/            
     *7X,F5.0,' KIPS TO 0. KIPS AND THE VALUES IN THE ABOVE TABLE'/             
     *7X,'MUST BE APPROPRIATELY ADJUSTED.'//                                    
     *7X,'IF THE DESIGNER SELECTS A LATERAL BAR SIZE DIFFERENT THAN'/           
     *7X,'THE ONE ASSUMED, HE SHOULD CONSIDER ADJUSTING THE RADIUS OF'/         
     *7X,'THE MAIN STEEL BAR LOOP AND RE-RUN THE PROBLEM.'//                    
     *7X,'MAXIMUM SHEAR CAPACITY OF THE SECTION:'/                              
     *7X,'VMAX = .85 * 10 * SQRT(FC) * B * D = ',F6.0,' KIPS'/)                 
C                                                                               
 3010 FORMAT(///7X,                                                             
     *    '***********************'/                                            
     *7X,'* TRANSVERSE DIRECTION *'/                                            
     *7X,'*****************************************************',               
     *   '********************')                                                
 3020 FORMAT(///7X,                                                             
     *   '**************************'/                                          
     *7X,'* LONGITUDINAL DIRECTION *'/                                          
     *7X,'*****************************************************',               
     *   '********************')                                                
C                                                                               
C                                                                               
 3060 FORMAT(/7X,'MAXIMUM CENTER TO CENTER SPACING (PITCH) OF',                 
     *' LATERAL REINFORCEMENT. (IN)'//                                          
     *8X,' BAR    CASE A         CASE B       CASE C',11X,'CONFINEMENT'/        
     *8X,' SIZE   CONFINEMENT    APP SHEAR    MIN SHEAR      RATIO',            
     *'  EQUATION'/                                                             
     *8X,'  4  ',6X,F7.3,6X,F7.3,6X,F7.3,6X,F7.5,5X,I1/                         
     *8X,'  5  ',6X,F7.3,6X,F7.3,6X,F7.3,6X,F7.5,5X,I1/                         
     *8X,'  6  ',6X,F7.3,6X,F7.3,6X,F7.3,6X,F7.5,5X,I1/                         
     *8X,'  7  ',6X,F7.3,6X,F7.3,6X,F7.3,6X,F7.5,5X,I1/                         
     *8X,'  8  ',6X,F7.3,6X,F7.3,6X,F7.3,6X,F7.5,5X,I1//)                       
C                                                                               
 3070 FORMAT(/,                                                                 
     *7X,'NOTE: VU',F6.0,' KIPS, IS LESS THAN (.85 * VC)',F6.0,' KIPS.'/        
     *7X,'THEREFORE, NO REINFORCEMENT FOR APPLIED SHEAR (CASE B) IS',           
     *' REQUIRED.')                                                             
C                                                                               
 3080 FORMAT(/7X,'NOTE: VU',F6.0,' KIPS, IS LESS THAN ONE HALF ',               
     *'(.85 * VC)',F6.0,' KIPS.'/                                               
     *7X,'THEREFORE, NO REINFORCEMENT FOR MINIMUM SHEAR (CASE C) IS',           
     *' REQUIRED.')                                                             
C                                                                               
  100 RETURN                                                                    
      END                                                                       
      SUBROUTINE STRESS(X,Y, NCP, RX, RY, RS, NB, MYSC, MXSC, MSC, PSC,         
     * HX, HY, STOTAL, EC, RM)                                                  
C      TSO DATA SET 'TR.BDE.FORT(STRESS)'         05/20/81                      
C      PAN DATA SET BDEROJWCOL                    05/20/81                      
C **********************************************************************        
C         GENERAL COLUMN ANALYSIS - MODIFIED FOR YIELD PROGGRAM                 
C                    (OLD PROGRAM BDE013)                                       
C *********************************************************************         
C MOD VER   DATE     CHANGE                            BY     PANVALET          
C  X   X   APR 81   COPY ORIG BDE013 PANFILE           JHG   BDE013V15          
C  1   1   APR 81   NEW PANFILE -- STRIP OUT EXCESS    SBM   BDEROJWCOL         
C *********************************************************************         
C                                                                               
C     X(I)=X-COORDINATE OF CONCRETE POINT I.                                    
C     Y(I)=Y-COORDINATE OF CONCRETE POINT I.                                    
C     PMY=MOMENT ABOUT THE Y-AXIS IN KIP FEET.                                  
C     PMX=MOMEN  ABOUT THE X-AXIS IN KIP FEET.                                  
C     P=AXIAL LOAD IN KIPS.                                                     
C     XBAR=X-DISTANCE TO MOMENT CENTER.                                         
C     YBAR=Y-DISTANCE TO MOMENT CENTER.                                         
C     BNCOM=MODULAR RATIO COMPRESSION STEEL.                                    
C     BNTEN=MODULAR RATIO TENSION STEEL.                                        
C     LOADN=USER LOAD NUMBER                                                    
C     SSMIN=MAXIMUM TENSILE STRESS IN THE REINFORCING STEEL                     
C     SLX(I)=X-COORDINATE OF TENSION RESTEEL                                    
C     SLY(I)=Y-COORDINATE OF TENSION RESTEEL                                    
C     ITM=SUBSCRIPT ASSIGNED TO BAR COORDINATE WHERE MAX. TENSION OCCURS        
C     SSMAX=MAXIMUM COMPRESSION STRESS IN THE REINFORCING STEEL                 
C     SHX(I)=X-COORDINATE OF COMPRESSION RESTEEL                                
C     SHY(I)=Y-COORDINATE OF COMPRESSION RESTEEL                                
C     IALLOW=FALLOW=COMPUTED ALLOWABLE CONCRETE STRESS                          
C     SMAX(I)=                                                                  
C     SMIN(I)=                                                                  
C                                                                               
      IMPLICIT REAL * 8  (A-H,O-Z)                                              
      REAL * 8  MYSC,MXSC,MSC                                                   
C                                                                               
      DIMENSION X(1),Y(1), CX(100), CY(100), XA(100), YA(100),                  
     *CC(100), CS(100), CHX(100), MYSC(1), MXSC(1), MSC(1), PSC(1),             
     *CHY(100), RS(300), RX(300), RY(300), SMIN(300), SMAX(300),                
     *SLX(300), SLY(300), SHX(300), SHY(300)                                    
C                                                                               
      MP=NB                                                                     
      NP=NCP                                                                    
      STOTAL = STOTAL * 144.                                                    
      IALLST =  0                                                               
      BNCOM  =  RM                                                              
      BNTEN  =  RM                                                              
      NRESLT =  0                                                               
C                                                                               
      XBAR = HX/2                                                               
      YBAR = HY/2                                                               
C *** CHANGE CONC. COORD TO CLOCKWISE AND TRANSFER FROM CENTROID ******         
      DO 10 I=1,NCP                                                             
      L = NCP+1-I                                                               
      CX(I) = (X(L)*12.) + XBAR                                                 
      CY(I) = (Y(L)*12.) + YBAR                                                 
   10 CONTINUE                                                                  
      DO 20 I=1,NCP                                                             
      X(I) = CX(I)                                                              
      Y(I) = CY(I)                                                              
   20 CONTINUE                                                                  
C ********* CHANGE STEEL COORD FROM CENTROID ************************           
      DO 30 I=1,NB                                                              
      RX(I) = (RX(I)*12.) + XBAR                                                
      RY(I) = (RY(I)*12.) + YBAR                                                
      IF(RS(I).EQ.0.0)  RS(I)=RS(I-1)                                           
   30 CONTINUE                                                                  
C ************ CHANGE UNITS FROM FEET**2 TO INCHES**2  **************           
      DO 35 I=1,NB                                                              
      RS(I) = RS(I) * 144.                                                      
   35 CONTINUE                                                                  
C                                                                               
      EC = EC/.144                                                              
C                                                                               
C     WRITE(6,3000)    NCP,NB,HX,HY,STOTAL                                      
C3000 FORMAT(' NCP,NB,HX,HY,STOTAL',3I5,3F10.2)                                 
C                                                                               
C     WRITE(6,3010)     (I,X(I),Y(I),I=1,NCP)                                   
C3010 FORMAT(' I,X,Y',I5,2F10.2)                                                
C                                                                               
C     WRITE(6,3020) (I,RX(I),RY(I),RS(I),I=1,NB)                                
C3020 FORMAT(' I,RX,RY,RS',I5,3F10.2)                                           
C                                                                               
C     WRITE(6,3030)  (I,MYSC(I),MXSC(I),PSC(I),I=1,NL)                          
C3030 FORMAT(' I,MYSC,MXSC,PSC',I5,3F10.2)                                      
C                                                                               
      NCP=NCP-1                                                                 
C                                                                               
      DO 40 I=1,NP                                                              
      XA(I) =0.                                                                 
      YA(I) =0.                                                                 
      CC(I) =0.                                                                 
      CS(I) =0.                                                                 
      CHX(I) =0.                                                                
      CHY(I) =0.                                                                
   40 CONTINUE                                                                  
C                                                                               
      DO 50 I=1,NB                                                              
      SMIN(I) =0.                                                               
      SMAX(I) =0.                                                               
      SLX(I) =0.                                                                
      SLY(I) =0.                                                                
      SHX(I) =0.                                                                
      SHY(I) =0.                                                                
   50 CONTINUE                                                                  
C                                                                               
      DO 2000 K=1,22                                                            
      IF(K.GE.4.AND.K.LE.6)   GO TO 2000                                        
      IF(K.GE.18.AND.K.LE.19) GO TO 2000                                        
      IF(MYSC(K).EQ.0.0.AND.MXSC(K).EQ.0.0.AND.PSC(K).EQ.0.0) GO TO 2000        
      IF(MYSC(K).EQ.0.0) MYSC(K)=.01                                            
      IF(MXSC(K).EQ.0.0) MXSC(K)=.01                                            
      IF(PSC(K).EQ.0.0)  PSC(K) =.01                                            
      PMY = MYSC(K)                                                             
      PMX = -MXSC(K)                                                            
      PXY = MSC(K)                                                              
      P   = PSC(K)                                                              
      LOADN = K                                                                 
C                                                                               
  800 EX = 12. *PMY/ P                                                          
      XP =XBAR + EX                                                             
      EY = 12. *PMX/ P                                                          
      YP =YBAR - EY                                                             
      XF = 1.                                                                   
      YF = 1.                                                                   
      XC = 0.                                                                   
      YC = 0.                                                                   
      ITERAT=0                                                                  
      AXIAL=0.0                                                                 
      BR=0.0                                                                    
 1040 BM = BNCOM -1.                                                            
 1100 IF (PMX -PMY ) 1110,1120,1120                                             
 1110 XNA = -9999.                                                              
      KS =2                                                                     
      PBR = -10000.                                                             
      GO TO 1200                                                                
C                                                                               
 1120 YNA = 9999.                                                               
      KS =1                                                                     
      PBR = 10000.                                                              
C                                                                               
 1200 AS =0.                                                                    
      AXS =0.                                                                   
      AYS =0.                                                                   
      PIXS =0.                                                                  
      PIYS =0.                                                                  
      PIXYS =0.                                                                 
      AC =0.                                                                    
      AXC =0.                                                                   
      AYC =0.                                                                   
      PIXC =0.                                                                  
      PIYC =0.                                                                  
      PIXYC =0.                                                                 
      A =0.                                                                     
      AX =0.                                                                    
      AY =0.                                                                    
      PIY =0.                                                                   
      PIX =0.                                                                   
      PIXY =0.                                                                  
C                                                                               
      CALL CONCS (X, Y, XA, YA, YF, XF, AXIAL, XC, YC, NP,                      
     * AC, AXC, AYC, PIXC, PIYC, PIXYC, BR, ITERAT, KS)                         
C                                                                               
      IF (ITERAT) 1310,1301,1310                                                
 1301 CTOTAL = AC                                                               
C                                                                               
 1310 CALL STEEL (                                                              
     * AS, AXS, AYS, PIXS, PIYS, PIXYS,                                         
     * AC, AXC, AYC, PIXC, PIYC, PIXYC,                                         
     * A,  AX,  AY,  PIX,  PIY,  PIXY,                                          
     *     XP,  YP,  PIXI, PIYI,                                                
     *               PMXI, PMYI,                                                
     * P, YF, XF, AXIAL, XC, YC, BNTEN, BM, RX, RY, RS, KS, MP, ITERAT)         
C                                                                               
 1325 XF = PMXI/PIXI                                                            
      YF = PMYI/PIYI                                                            
      AXIAL = P/A                                                               
      GO TO (1340,1330), KS                                                     
 1330 AR = XF/YF                                                                
      BR = -(XF*YC +AXIAL)/YF +XC                                               
      GO TO 1350                                                                
 1340 AR = YF/XF                                                                
      BR = (YF *(-XC)+AXIAL)/XF +YC                                             
 1350 DELTA =DABS(PBR-BR)                                                       
      IF (DELTA-0.1) 1400,1400,1360                                             
 1360 ITERAT = ITERAT + 1                                                       
      PBR = BR                                                                  
      IF (ITERAT-15) 1200,1400,1400                                             
C                                                                               
 1400 DO 1525 N = 1,NCP                                                         
      GO TO (1520,1510), KS                                                     
 1510 XCS = AR*Y(N) +BR                                                         
      CS(N) = 1000. *YF *(X(N)-XCS)                                             
      GO TO 1525                                                                
 1520 YCS = AR*X(N) +BR                                                         
      CS(N) = 1000. *XF *(YCS-Y(N))                                             
 1525 CONTINUE                                                                  
C                                                                               
      IH =1                                                                     
      IL =1                                                                     
      HIGH = CS(1)                                                              
      CLOW =HIGH                                                                
C                                                                               
      DO 1550 N =1,NCP                                                          
      IF (HIGH -CS(N)) 1530,1535,1535                                           
 1530 HIGH = CS(N)                                                              
      IH = N                                                                    
 1535 IF (CS(N) -CLOW) 1540,1550,1550                                           
 1540 CLOW = CS(N)                                                              
      IL = N                                                                    
 1550 CONTINUE                                                                  
C                                                                               
      IHM=1                                                                     
      DO 1570 N =1,NCP                                                          
      IF (CS(N)) 1570,1570,1555                                                 
 1555 CC(IHM) =CS(N)                                                            
      CHX(IHM) = X(N)                                                           
      CHY(IHM) = Y(N)                                                           
      IHM = IHM + 1                                                             
 1570 CONTINUE                                                                  
C                                                                               
      ICC = IHM-1                                                               
 1601 SSMAX =0.                                                                 
      SSMIN =0.                                                                 
      IT = 1                                                                    
      IC=1                                                                      
      ITM =0                                                                    
      ICM =0                                                                    
      GO TO (1702,1602), KS                                                     
C                                                                               
 1602 DO 1690 M =1,MP                                                           
      XNA = (XF *(RY(M)-YC) -AXIAL)/YF +XC                                      
      XSS = AR*RY(M) +BR                                                        
C                                                                               
      IF (RX(M) -XNA) 1610,1610,1650                                            
 1610 SMIN(IT) = BNTEN *YF *(RX(M)-XSS)                                         
 1630 SMIN(IT) = SMIN(IT) * (-1000.)                                            
      IF (SSMIN -SMIN(IT)) 1640,1640,1645                                       
 1640 SSMIN = SMIN(IT)                                                          
      ITM = IT                                                                  
 1645 SLX(IT) = RX(M)                                                           
      SLY(IT) = RY(M)                                                           
      IT = IT + 1                                                               
      GO TO 1680                                                                
 1650 SMAX(IC) = 1000.*BNCOM*YF *(RX(M)-XSS)                                    
 1655 IF (SSMAX -SMAX(IC)) 1660,1660,1665                                       
 1660 SSMAX = SMAX(IC)                                                          
      ICM = IC                                                                  
 1665 SHX(IC) = RX(M)                                                           
      SHY(IC) = RY(M)                                                           
      IC = IC + 1                                                               
 1680 GO TO (1790,1690), KS                                                     
 1690 CONTINUE                                                                  
C                                                                               
      GO TO 1800                                                                
 1702 DO 1790 M =1,MP                                                           
      YNA = (YF *(RX(M)-XC) +AXIAL) /XF +YC                                     
      YSS = AR*RX(M) +BR                                                        
      IF (RY(M) -YNA) 1710,1750,1750                                            
 1710 SMAX(IC) = 1000.*BNCOM*XF* (YSS-RY(M))                                    
      GO TO 1655                                                                
 1750 SMIN(IT) = BNTEN*XF *(YSS-RY(M))                                          
 1765 GO TO 1630                                                                
 1790 CONTINUE                                                                  
C                                                                               
 1800 IST = IT-1                                                                
      ISC = IC-1                                                                
C                                                                               
      CALL ALLOW (STOTAL, CTOTAL, CLOW, HIGH, FALLOW)                           
C                                                                               
      CALL RESULT (X, Y, P, PMX, PMY, PXY, XBAR, YBAR, BNCOM, BNTEN,            
     * CC, CHX, CHY, SSMAX, SSMIN, SMAX, SMIN, SHX, SHY, SLX, SLY,              
     * HIGH, FALLOW, LOADN, IALLST, ICC, IST, ISC, IH, ICM, ITM, NRESLT,        
     * EC, RM)                                                                  
C                                                                               
      PIXYS =0.                                                                 
      AS =0.                                                                    
      AXS =0.                                                                   
      AYS =0.                                                                   
      PIXS =0.                                                                  
      PIYS =0.                                                                  
      AC =0.                                                                    
      AXC =0.                                                                   
      AYC =0.                                                                   
      PIXC =0.                                                                  
      PIYC =0.                                                                  
      PIXYC =0.                                                                 
      A =0.                                                                     
      AX =0.                                                                    
      AY =0.                                                                    
      PIY =0.                                                                   
      PIX =0.                                                                   
      PIXY =0.                                                                  
      ITERAT =0                                                                 
 2000 CONTINUE                                                                  
C *** CHANGE CONC. COORD TO COUNTER-CLOCKWISE AND TRANSFER TO CENTROID *        
      NCP=NCP+1                                                                 
      DO 2010 I=1,NCP                                                           
      L = NCP+1-I                                                               
      CX(I) = (X(L) - XBAR)/12.                                                 
      CY(I) = (Y(L) - YBAR)/12.                                                 
 2010 CONTINUE                                                                  
      DO 2020 I=1,NCP                                                           
      X(I) = CX(I)                                                              
      Y(I) = CY(I)                                                              
 2020 CONTINUE                                                                  
C ********* CHANGE STEEL COORD TO CENTROID ************************             
      DO 2030 I=1,NB                                                            
      RX(I) = (RX(I) - XBAR)/12.                                                
      RY(I) = (RY(I) - YBAR)/12.                                                
 2030 CONTINUE                                                                  
C ************ CHANGE UNITS FROM INCHES**2 TO FEET**2  **************           
      DO 2040 I=1,NB                                                            
      RS(I) = RS(I) / 144.                                                      
 2040 CONTINUE                                                                  
      EC = EC*.144                                                              
      RETURN                                                                    
      END                                                                       
C **********************************************************************        
      SUBROUTINE STEEL                                                          
     *(AS, AXS, AYS, PIXS, PIYS, PIXYS,                                         
     * AC, AXC, AYC, PIXC, PIYC, PIXYC,                                         
     * A,  AX,  AY,  PIX,  PIY,  PIXY,                                          
     *     XP,  YP,  PIXI, PIYI,                                                
     *               PMXI, PMYI,                                                
     * P, YF, XF, AXIAL, XC, YC, BNTEN, BM, RX, RY, RS, KS, MP, ITERAT)         
C                                                                               
C     FORTRAN  ROY IMBSEN      MAY 1968                                         
C     FORTRAN  JIMME WING                                                       
C     FORTRAN  STEVE MCBRIDE   MAY 1981                                         
C                                                                               
      IMPLICIT REAL * 8 (A-H,O-Z)                                               
C                                                                               
      DIMENSION  RS(1), RX(1), RY(1)                                            
C                                                                               
  100 GO TO (105,150), KS                                                       
  105 M=1                                                                       
  106 IF (ITERAT) 110,140,110                                                   
  110 YNA = (YF *(RX(M)-XC) +AXIAL) /XF +YC                                     
      IF (RY(M) -YNA) 140,120,120                                               
  120 FACTOR = BNTEN *RS(M)                                                     
  125 AS = AS+FACTOR                                                            
      AXS = AXS +RX(M)*FACTOR                                                   
      AYS = AYS +RY(M)*FACTOR                                                   
      PIXS = PIXS +RY(M)*RY(M)*FACTOR                                           
      PIYS = PIYS +RX(M)*RX(M)*FACTOR                                           
      PIXYS = PIXYS +RX(M)*RY(M)*FACTOR                                         
      GO TO (130,155), KS                                                       
  130 M=M+1                                                                     
      IF (M.LE.MP) GO TO 106                                                    
      GO TO 170                                                                 
  140 FACTOR = BM *RS(M)                                                        
      GO TO 125                                                                 
  150 M=1                                                                       
  151 IF (ITERAT) 154,140,154                                                   
  154 XNA = (XF *(RY(M)-YC) -AXIAL) /YF +XC                                     
      IF (RX(M) -XNA) 120,120,140                                               
  155 M=M+1                                                                     
      IF (M.LE.MP) GO TO 151                                                    
C                                                                               
  170 A    = AC +AS                                                             
      AX   = AXC +AXS                                                           
      AY   = AYC +AYS                                                           
      PIX  = PIXC +PIXS                                                         
      PIY  = PIYC +PIYS                                                         
      PIXY = PIXYC +PIXYS                                                       
C                                                                               
  190 XC = AX/A                                                                 
      YC = AY/A                                                                 
      PIXO = PIX -A*YC*YC                                                       
      PIYO = PIY -A*XC*XC                                                       
      PIXYO = PIXY -A*XC*YC                                                     
      PMXO = P *(YC-YP)                                                         
      PMYO = P *(XP-XC)                                                         
  195 PIXI = PIXO -PIXYO*PIXYO/PIYO                                             
      PIYI = PIYO -PIXYO*PIXYO/PIXO                                             
      PMXI = PMXO +PMYO*PIXYO/PIYO                                              
      PMYI = PMYO +PMXO*PIXYO/PIXO                                              
      RETURN                                                                    
      END                                                                       
C **********************************************************************        
      SUBROUTINE CONCS (X, Y, XA, YA, YF, XF, AXIAL, XC, YC, NP,                
     * AC, AXC, AYC, PIXC, PIYC, PIXYC, BR, ITERAT, KS)                         
C                                                                               
C     FORTRAN  ROY IMBSEN    MAY 1968                                           
C     FORTRAN  JIMMIE WING                                                      
C     FORTRAN  STEVE MCBRIDE MAY 1981                                           
C                                                                               
      IMPLICIT REAL * 8 (A-H,O-Z)                                               
C                                                                               
      DIMENSION X(1), Y(1), XA(1), YA(1)                                        
C                                                                               
C                                                                               
C                                                                               
      J = 1                                                                     
      L = 1                                                                     
      I = 0                                                                     
      LPT = 0                                                                   
      IF (ITERAT) 55,100,55                                                     
   55 GO TO (60,70), KS                                                         
   60 YNA = (YF* (100.-XC) +AXIAL) / XF +YC                                     
      SNA =.01*(YNA-BR)                                                         
      GO TO 100                                                                 
   70 XNA = (XF* (100.-YC) -AXIAL) / YF +XC                                     
      SNA = 100. / (XNA -BR)                                                    
      SN2 = 10.**10                                                             
      SNA = DMIN1(SNA,SN2)                                                      
      GO TO 100                                                                 
  100 DO 900  N =1,NP                                                           
      IF (ITERAT) 200,110,200                                                   
  110 IF (N -1) 120,500,120                                                     
  120 IF (K -1) 300,300,130                                                     
  130 K = 2                                                                     
      XW = X(N)                                                                 
      YW = Y(N)                                                                 
      I = I+1                                                                   
      XA(I) = X(N)                                                              
      YA(I) = Y(N)                                                              
C                                                                               
  140 AC =AC-(XL *YW  -XW *YL) /2.                                              
      AXC =AXC-XL**2 *(YW -YL)/2. -(XW -XL) *(YW -YL) *(2. *XL +XW) /6.         
      AYC =AYC+YL**2 *(XW -XL)/2. +(YW -YL) *(XW -XL) *(2. *YL +YW) /6.         
      PIXC = PIXC +YL**3 *(XW -XL)/3. +(YW -YL)**3 *(XW -XL)/36.                
     1       +(2.*YL +YW)**2 *(YW -YL) *(XW -XL)/18.                            
      PIYC = PIYC -XL**3 *(YW -YL)/3. -(XW -XL)**3 *(YW -YL)/36.                
     1       -(2.*XL +XW)**2 *(XW -XL) *(YW -YL)/18.                            
      PIXYC = PIXYC -XL**2 *(YW -YL) *(YW +YL)/4.                               
     1              -(XW -XL)**2 *(YW -YL)**2 /72.                              
     2              -(XW -XL) *(YW -YL) *(2.*XL +XW) *(YL +2.*YW)/18.           
      XL = XW                                                                   
      YL = YW                                                                   
      IF (LPT) 990,150,990                                                      
  150 IF (L -2) 900,155,900                                                     
  155 L = 1                                                                     
      GO TO 130                                                                 
  200 GO TO (210,220), KS                                                       
  210 YNA = (YF* (X(N)-XC) +AXIAL) / XF  +YC                                    
      IF (Y(N) -YNA) 110,110,600                                                
  220 XNA = (XF* (Y(N)-YC) -AXIAL) / YF  +XC                                    
      IF (X(N) -XNA) 600,110,110                                                
  300 IF (X(N) -X(N-1)) 320,310,320                                             
  310 XX = X(N)                                                                 
      SDATA = 10.**10                                                           
      GO TO 350                                                                 
  320 SDATA = (Y(N) -Y(N-1)) / (X(N) -X(N-1))                                   
      GO TO (330,340), KS                                                       
  330 XX = (SDATA *X(N) -Y(N) +BR) / (SDATA -SNA)                               
      GO TO 350                                                                 
  340 XX = (SDATA *X(N) -Y(N) -SNA *BR) / (SDATA -SNA)                          
      GO TO 350                                                                 
  350 IF (Y(N) -Y(N-1)) 370,360,370                                             
  360 YY = Y(N)                                                                 
      GO TO 395                                                                 
  370 GO TO (380,390), KS                                                       
  380 YY = SNA *XX +BR                                                          
      GO TO 395                                                                 
  390 YY = SNA * (XX -BR)                                                       
  395 GO TO 400                                                                 
  400 IF (J -1) 440,420,440                                                     
  420 XL = XX                                                                   
      YL = YY                                                                   
      XFIRST = XX                                                               
      YFIRST = YY                                                               
      J = 2                                                                     
      GO TO 130                                                                 
  440 XW = XX                                                                   
      YW = YY                                                                   
      IF (K -1) 450,460,450                                                     
  450 L = 1                                                                     
      K = 1                                                                     
      GO TO 140                                                                 
  460 L = 2                                                                     
      GO TO 140                                                                 
  500 XL = X(1)                                                                 
      YL = Y(1)                                                                 
      XA(1) = X(1)                                                              
      YA(1) = Y(1)                                                              
      I = 1                                                                     
      XFIRST =X(1)                                                              
      YFIRST =Y(1)                                                              
      J = 2                                                                     
      K = 2                                                                     
      GO TO 900                                                                 
  600 IF (J -1) 610,610,620                                                     
  610 K = 1                                                                     
      GO TO 900                                                                 
  620 IF (K -1) 610,610,300                                                     
  900 CONTINUE                                                                  
      IF (X(NP) -XFIRST) 920,910,920                                            
  910 IF (Y(NP) -YFIRST) 920,990,920                                            
  920 XW = XFIRST                                                               
      YW = YFIRST                                                               
      LPT = 1                                                                   
      GO TO 140                                                                 
C                                                                               
  990 RETURN                                                                    
      END                                                                       
C **********************************************************************        
      SUBROUTINE ALLOW (STOTAL, CTOTAL, CLOW, HIGH, FALLOW)                     
C                                                                               
C     FORTRAN  ROY IMBSEN     MAY 1968                                          
C     FORTRAN  JIMMIE WING                                                      
C     FORTRAN  STEVE MCBRIDE  MAY 1981                                          
C                                                                               
C                                                                               
      IMPLICIT REAL * 8 (A-H,O-Z)                                               
C                                                                               
      F = 100. *STOTAL/CTOTAL                                                   
      T=2963.7075 -(1430.6968 -(552.6238 -(114.1669                             
     1      -(11.6949 -0.4652*F)*F)*F)*F)*F                                     
C   THIS FORMULA WAS REWRITTEN ON APRIL 23, 1984        BY L. SHENG             
C     T=2963.7075 -1430.6968*F +552.6238*F**2 -114.1669*F**3                    
C    1+11.6949*F**4 -0.4652*F**5                                                
      R=T/1300.0                                                                
      RATIO = CLOW/HIGH                                                         
      IF (RATIO) 50,70,70                                                       
   50 SUM = R +RATIO                                                            
      IF (SUM) 60,60,70                                                         
   60 FALLOW=1300.                                                              
      GO TO 100                                                                 
   70 C = 0.8*(731.25 + (240.*F)) / (1.+0.09*F)                                 
      DCRACK =(1300. -C) *C/(C+T)                                               
      IF (RATIO) 80,90,90                                                       
   80 CRACK = C+DCRACK                                                          
      FALLOW = CRACK +(CRACK -1300.)*RATIO/R                                    
      GO TO 100                                                                 
   90 FALLOW = C +(1.-RATIO)*DCRACK                                             
  100 RETURN                                                                    
      END                                                                       
C **********************************************************************        
      SUBROUTINE RESULT(X, Y, P,PMX, PMY, PXY, XBAR, YBAR, BNCOM, BNTEN,        
     * CC, CHX, CHY, SSMAX, SSMIN, SMAX, SMIN, SHX, SHY, SLX, SLY,              
     * HIGH, FALLOW, LOADN, IALLST, ICC, IST, ISC, IH, ICM, ITM, NRESLT,        
     * EC, RM)                                                                  
C                                                                               
C     FORTRAN  ROY IMBSEN     MAY 1968                                          
C     FORTRAN  STEVE MCBRIDE  MAY 1981                                          
C                                                                               
      IMPLICIT REAL * 8 (A-H,O-Z)                                               
      REAL * 4 GR,CA                                                            
C                                                                               
      DIMENSION X(1), Y(1), CC(1), CHX(1), CHY(1),                              
     * SMIN(1), SMAX(1),                                                        
     * SLX(1), SLY(1), SHX(1), SHY(1), GR(22), CA(22)                           
C                                                                               
      DATA GR(1),GR(2),GR(3),GR(4),GR(5),GR(6),GR(7),GR(8),GR(9),               
     *GR(10),GR(11),GR(12),GR(13),GR(14),GR(15),GR(16),GR(17),GR(18),           
     *GR(19),GR(20),GR(21),GR(22)/                                              
     *'IH ','IH ','IH ','IP ','IP ','IP ','II ','III','III','III',              
     *'IV ','IV ','IV ','V  ','VI ','VI ','VI ','VII','VII',                    
     *'AL1','AL2','DL '/                                                        
      DATA CA(1),CA(2),CA(3),CA(4),CA(5),CA(6),CA(7),CA(8),CA(9),               
     *CA(10),CA(11),CA(12),CA(13),CA(14),CA(15),CA(16),CA(17),CA(18),           
     *CA(19),CA(20),CA(21),CA(22)/                                              
     *'1','2','3','1','2','3',' ','1','2','3','1','2','3',' ',                  
     *'1','2','3','1','2',' ',' ',' '/                                          
C                                                                               
      PMX = -PMX                                                                
      GO TO 500                                                                 
C                                                                               
  130 WRITE(6,135)                                                              
  135 FORMAT(1H1,                                                               
     1                         /// T5,'/----------------LOADING DATA----        
     2------------/', 4X, '/---------RESULTS  (UNITS POUNDS & INCHES)---        
     3------/', / T8, 'MOMENTS     AXIAL   MOMENT CENTER   MODULI', 8X,         
     4'MAXIMUM     ALLOWABLE    MAXIMUM STEEL STRESSES' / T8,                   
     5'(KIP FT)    FORCE    COORDINATES    RATIO', 9X,                          
     6'CONCRETE    CONCRETE     COMPRESSION    TENSION' / T20,                  
     7'(KIPS)    (INCHES)', 21X, 'STRESS', 6X, 'STRESS' )                       
C                                                                               
  400 IALLOW=FALLOW                                                             
      NTEN=BNTEN                                                                
      NCOM=BNCOM                                                                
C                                                                               
C     CHECK FOR TENSION STEEL                                                   
C                                                                               
      IF(SSMIN) 420, 410, 420                                                   
C                                                                               
C     NO TENSION STEEL                                                          
C                                                                               
  410 WRITE(6, 415) LOADN, PMY, P, XBAR, NCOM, HIGH,                            
     1IALLOW, SSMAX, PMX, YBAR,  NTEN, X(IH), SHX(ICM), Y(IH), SHY(ICM)         
  415 FORMAT(1H0, 'LOAD NO.', I3,/                                              
     1T5, 'MY', F9.0, 3X, F6.0,                                                 
     14X, 'X', 2X, F6.1, 5X, 'N''', I3, 10X, F6.0, 7X, I4, 9X, F8.0, /          
     2T5, 'MX', F9.0, 13X, 'Y', 2X, F6.1, 5X, 'N', I4, 7X, 'X', F8.2,           
     318X, 'X', F8.2, / T56, 'Y', F8.2, 18X, 'Y', F8.2)                         
      GO TO 450                                                                 
C                                                                               
C     TENSION STEEL                                                             
C                                                                               
  420 WRITE(6, 425) LOADN, PMY, P, XBAR, NCOM, HIGH,                            
     1IALLOW, SSMAX, SSMIN, PMX, YBAR, NTEN, X(IH), SHX(ICM), SLX(ITM),         
     2Y(IH), SHY(ICM), SLY(ITM)                                                 
  425 FORMAT(1H0, 'LOAD N0.', I3, 2X,/                                          
     *T6, 'MY', F9.0, 3X, F6.0,                                                 
     *4X, 'X', 2X, F6.1, 5X, 'N''', I3, 10X, F6.0, 7X, I4, 9X, F8.0,            
     *4X, F8.0, / T6, 'MX', F9.0, 13X, 'Y', 2X, F6.1, 5X, 'N', I4, 7X,          
     *'X',F8.2,18X,'X',F8.2,3X,'X',F8.2 / T56, 'Y', F8.2, 18X, 'Y',             
     *F8.2, 3X, 'Y', F8.2)                                                      
  450 IF(IALLST) 900,900,460                                                    
C                                                                               
C     PRINT OUT ALL STRESSES                                                    
C                                                                               
  460 WRITE(6,462)                                                              
  462 FORMAT(1H1,10X,//                                                         
     1'COLUMN ANALYSIS', 13X, 'ALL STRESSES', / 6X, 4A4, A2, T70,               
     2'PAGE', I2, ////   1X, 'CONCRETE COMPRESSION STRESSES')                   
      WRITE(6,464) LOADN                                                        
  464 FORMAT(1H , 'LOAD NO.', I4, /  4(18X, 'STRESS'), / 5X,                    
     14('X', 7X, 'Y', 6X, 'PSI', 6X))                                           
      WRITE (6,470) (CHX(I), CHY(I), CC(I), I=1,ICC)                            
  470 FORMAT (4(F8.2, F8.2, F8.0))                                              
C                                                                               
C     CHECK FOR BARS IN COMPRESSION                                             
C                                                                               
      IF(ISC)475,480,475                                                        
  475 WRITE(6,477)                                                              
  477 FORMAT(1H , 'COMPRESSION STEEL STRESSES')                                 
      WRITE(6,464) LOADN                                                        
      WRITE(6,470) (SHX(I), SHY(I), SMAX(I), I=1,ISC)                           
C                                                                               
C     CHECK FOR BARS IN TENSION                                                 
C                                                                               
  480 IF(IST)485,490,485                                                        
  485 WRITE(6,487)                                                              
  487 FORMAT(1H , 'TENSION STEEL STRESSES')                                     
      WRITE(6,464) LOADN                                                        
      WRITE(6,470) (SLX(I), SLY(I), SMIN(I), I=1,IST)                           
  490 CONTINUE                                                                  
C                                                                               
  500 K=LOADN                                                                   
      IF(NRESLT.EQ.1) GO TO 520                                                 
      WRITE(6,510) EC,RM                                                        
  510 FORMAT(//,                                                                
     *7X,'COLUMN SERVICE LOADS (K-FT) AND MAXIMUM WORKING STRESSES',            
     *   ' (PSI)'//                                                             
     *7X,'YOUNG S MODULUS FOR CONCRETE - EC =',F9.0,' PSI'/                     
     *7X,'MODULAR RATIO  (ES/EC)       -  N =',F4.1//                           
     *7X,'            :---- SERVICE LOADS (K-FT) ----:---- MAX STRESS ('        
     *  ,'PSI) ----:'/                                                          
     *7X,'            TRANS     LONG     COMB    AXIAL     CONC    STE',        
     *   'EL    STEEL'/                                                         
     *7X,'GP  CASE      MY       MX        M        P      COMP      ',         
     *   'TEN     COMP'//)                                                      
C                                                                               
      NRESLT = 1                                                                
C                                                                               
  520 WRITE(6,530) GR(K),CA(K),PMY,PMX,PXY,P,HIGH,SSMIN,SSMAX                   
  530 FORMAT(8X,A3,3X,A1,1X,7F9.0)                                              
  900 RETURN                                                                    
      END                                                                       

