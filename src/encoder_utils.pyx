import cython

ppr = 1024
ppr_mezzi = 512

#                           _______         _______       
#               Pin1 ______|       |_______|       |______ Pin1
# negative <---         _______         _______         __      --> positive
#               Pin2 __|       |_______|       |_______|   Pin2

		#	new	new	old	old
		#	pin2	pin1	pin2	pin1	Result
		#	----	----	----	----	------
		#	0	0	0	0	no movement
		#	0	0	0	1	+1
		#	0	0	1	0	-1
		#	0	0	1	1	+2  (assume pin1 edges only)
		#	0	1	0	0	-1
		#	0	1	0	1	no movement
		#	0	1	1	0	-2  (assume pin1 edges only)
		#	0	1	1	1	+1
		#	1	0	0	0	+1
		#	1	0	0	1	-2  (assume pin1 edges only)
		#	1	0	1	0	no movement
		#	1	0	1	1	-1
		#	1	1	0	0	+2  (assume pin1 edges only)
		#	1	1	0	1	-1
		#	1	1	1	0	+1
		#	1	1	1	1	no movement

def get_callback(bint old_clk, bint old_dt, bint new_clk, bint new_dt):
    cdef int switchValue
    switchValue = (new_clk<<3)+(new_dt<<2)+(old_clk<<1)+old_dt
    if switchValue==1 or switchValue==7 or switchValue==8 or switchValue==14:
        return 1
    elif switchValue==2 or switchValue==4 or switchValue==11 or switchValue==13:
        return -1
    elif  switchValue==0 or switchValue==5 or switchValue==10 or switchValue==15:
        return 0
    elif switchValue==3 or switchValue==12:
        return 2
    else:
        return -2

def get_new_position(old_position):
    if old_position > ppr_mezzi:  # Normalizziamo l'angolo fra -pi e pi
        return old_position - ppr
    elif old_position <= -ppr_mezzi:
        return old_position + ppr
    else:
        return old_position