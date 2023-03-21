import cython

def get_callback(bint old_clk, bint old_dt, bint new_clk, bint new_dt):
    print("callback")
    cdef int switchValue
    switchValue = new_clk<<3+new_dt<<2+old_clk<<1+old_dt
    if switchValue==1 or switchValue==7 or switchValue==8 or switchValue==14:
        returnValue = 1
    elif switchValue==2 or switchValue==4 or switchValue==11 or switchValue==13:
        returnValue = -1
    elif switchValue==3 or switchValue==12:
        returnValue = 2
    elif switchValue==6 or switchValue==9:
        returnValue = -2
    else:
        returnValue =  0
    return returnValue