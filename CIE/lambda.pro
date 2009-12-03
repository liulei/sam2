	
	l0	=	fltarr(91)
	l1	=	fltarr(91)
	l2	=	fltarr(91)
	l3	=	fltarr(91)
	l4	=	fltarr(91)
	l5	=	fltarr(91)
	l6	=	fltarr(91)
	l7	=	fltarr(91)

	xx	=	findgen(91)
	xx	=	xx/20.+4.

	openr,17,'cie_+0.5.dat'
	readf,17,l7
	close,16

	openw,100,'lambda7.dat'
	writeu,100,l7
	close,100

	openr,16,'cie_0.0.dat'
	readf,16,l6
	close,16
	
	openw,100,'lambda6.dat'
	writeu,100,l6
	close,100

	openr,15,'cie_-0.5.dat'
	readf,15,l5
	close,15
	
	openw,100,'lambda5.dat'
	writeu,100,l5
	close,100

	openr,14,'cie_-1.0.dat'
	readf,14,l4
	close,14

	openw,100,'lambda4.dat'
	writeu,100,l4
	close,100

	openr,13,'cie_-1.5.dat'
	readf,13,l3
	close,13

	openw,100,'lambda3.dat'
	writeu,100,l3
	close,100

	openr,12,'cie_-2.0.dat'
	readf,12,l2
	close,12

	openw,100,'lambda2.dat'
	writeu,100,l2
	close,100

	openr,11,'cie_-3.0.dat'
	readf,11,l1
	close,11

	openw,100,'lambda1.dat'
	writeu,100,l1
	close,100

	openr,10,'cie_NIL.dat'
	readf,10,l0
	close,10

	openw,100,'lambda0.dat'
	writeu,100,l0
	close,100

	plot,xx,l0,xrange=[4.0,8.5],yrange=[-24,-20],xstyle=1,ystyle=1
	oplot,xx,l1
	oplot,xx,l2
	oplot,xx,l3
	oplot,xx,l4
	oplot,xx,l5
	oplot,xx,l6
	oplot,xx,l7

	end
