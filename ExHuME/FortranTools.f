      SUBROUTINE OPENLHE
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)

      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)

      MSTP(161)=21
      OPEN(21,FILE='exhume.init',STATUS='unknown')
      MSTP(162)=22
      OPEN(22,FILE='exhume.evnt',STATUS='unknown')

C...Final Les Houches Event File, obtained by combining above two.
      MSTP(163)=23
      OPEN(23,FILE='exhume.lhe',STATUS='unknown')

      END

      SUBROUTINE WRITELHE
 
C...Double precision and integer declarations.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER PYK,PYCHGE,PYCOMP
 
C...Commonblocks.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      COMMON/PYCTAG/NCT,MCT(4000,2)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT1/MINT(400),VINT(400)
      COMMON/PYINT2/ISET(500),KFPR(500,2),COEF(500,20),ICOL(40,4,2)
      COMMON/PYINT4/MWID(500),WIDS(500,5)
      SAVE /PYJETS/,/PYCTAG/,/PYDAT1/,/PYDAT2/,/PYDAT3/,/PYPARS/,
     &/PYINT1/,/PYINT2/,/PYINT4/
 
C...HEPEUP for output.
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &VTIMUP(MAXNUP),SPINUP(MAXNUP)
      SAVE /HEPEUP/
      
C...Put event in HEPEUP commonblock.
      NUP=N-MINT(84)
      IDPRUP=MINT(1)
      XWGTUP=1D0
      SCALUP=VINT(53)
      AQEDUP=VINT(57)
      AQCDUP=VINT(58)
      DO 180 I=1,NUP
        IDUP(I)=K(I+MINT(84),2)
        IF(I.LE.2) THEN
          ISTUP(I)=-1
          MOTHUP(1,I)=0
          MOTHUP(2,I)=0
        ELSEIF(K(I+4,3).EQ.0) THEN
          ISTUP(I)=1
          MOTHUP(1,I)=1
          MOTHUP(2,I)=2
        ELSE
          ISTUP(I)=1
          MOTHUP(1,I)=K(I+MINT(84),3)-MINT(84)
          MOTHUP(2,I)=0
        ENDIF
        IF(I.GE.3.AND.K(I+MINT(84),3).GT.0)
     &  ISTUP(K(I+MINT(84),3)-MINT(84))=2
        ICOLUP(1,I)=MCT(I+MINT(84),1)
        ICOLUP(2,I)=MCT(I+MINT(84),2)
        DO 170 J=1,5
          PUP(J,I)=P(I+MINT(84),J)
  170   CONTINUE
        VTIMUP(I)=V(I,5)
        SPINUP(I)=9D0
  180 CONTINUE
 
C...Optionally write out event to disk.
      IF(MSTP(162).GT.0) THEN
        WRITE(MSTP(162),5200) NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP
        DO 190 I=1,NUP
          WRITE(MSTP(162),5300) IDUP(I),ISTUP(I),MOTHUP(1,I),
     &    MOTHUP(2,I),ICOLUP(1,I),ICOLUP(2,I),(PUP(J,I),J=1,5),
     &    VTIMUP(I),SPINUP(I)
  190   CONTINUE
      ENDIF
 
C...Error messages and other print formats.
 5100 FORMAT(1X,'Error: no subprocess switched on.'/
     &1X,'Execution stopped.')
 5200 FORMAT(1P,2I6,4E14.6)
 5300 FORMAT(1P,I8,5I5,5E18.10,E14.6,E12.4)
 
      RETURN
      END
