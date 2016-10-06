
import sys,os
sys.path.append(os.getcwd() + os.sep + r'..\swig_generated')

from pyMCFSimplex import *
mcf = MCFSimplex()

# Turn a Python list into a C double array
def createDoubleArrayFromList(l):
    d = new_darray(len(l))
    for i in range(0,len(l)):
        darray_set(d,i,l[i])
    return d

def createUIntArrayFromList(l):
    ui = new_uiarray(len(l))
    for i in range(0,len(l)):
        uiarray_set(ui,i,l[i])
    return ui
# Print out some elements of an array
def printelements(a, first, last):
    for i in range(first,last):
        print darray_get(a,i)

f = open(sys.argv[1], 'r')
#f = open('sample.dmx','r')
#f = open('/home/hahne/dev/network_flow/netgen_instances/stndrd42.net','r')
s = f.read()
f.close()

inputStr = string(s)

mcf.LoadDMX(inputStr)

##print "Setting time.."
##mcf.SetMCFTime()
##mcf.SolveMCF()
##print "Optimal solution: %s" %mcf.MCFGetFO()
##print "Time elapsed: %ssec" %mcf.TimeMCF()


'''
c Problem line (nodes, links)
p min 4 5
c
c Node descriptor lines (supply+ or demand-)
n 1 4
n 4 -4
c
c Arc descriptor lines (from, to, minflow, maxflow, cost)
a 1 2 0 4 2
a 1 3 0 2 2
a 2 3 0 2 1
a 2 4 0 3 3
a 3 4 0 5 1
'''

##nmx     = 4
##mmx     = 5
##pn      = 4
##pm      = 5
##pU      = [4,2,2,3,5]
##pC      = [2,2,1,3,1]
##pDfct   = [-4,0,0,4]
##pSn     = [1,1,2,2,3]
##pEn     = [2,3,3,4,4]
##
##mcf.LoadNet(nmx, mmx, pn, pm, createDoubleArrayFromList(pU), createDoubleArrayFromList(pC),
##            createDoubleArrayFromList(pDfct), createUIntArrayFromList(pSn),
##            createUIntArrayFromList(pEn))

print "Network loaded"
print "\n"
print "Nodes:\t%s" %mcf.MCFn()
print "Arcs:\t%s" %mcf.MCFm() 

print "\nSetting time.."
mcf.SetMCFTime()
mcf.SolveMCF()

if mcf.MCFGetStatus() == 0:
    print "Optimal solution: %s" %mcf.MCFGetFO()
    print "Time elapsed: %s sec" %mcf.TimeMCF()
else:
    print "Problem unfeasible!"
    print "Time elapsed: %s sec" %mcf.TimeMCF()

##nmx     = mcf.MCFnmax()
##mmx     = mcf.MCFmmax()
##pn      = mcf.MCFnmax()
##pm      = mcf.MCFmmax()
##
##pU      = []
##caps = new_darray(mmx)
##mcf.MCFUCaps(caps)
##for i in range(0,mmx):
##    pU.append(darray_get(caps,i))
##
##pC      = []
##
##costs = new_darray(mmx)
##mcf.MCFCosts(costs)
##for i in range(0,mmx):
##    pC.append(darray_get(costs,i))
##
##pDfct   = []
##supply = new_darray(nmx)
##mcf.MCFDfcts(supply)
##for i in range(0,nmx):
##    pDfct.append(darray_get(supply,i))
##
##pSn     = []
##pEn     = []
##startNodes = new_uiarray(mmx)
##endNodes = new_uiarray(mmx)
##mcf.MCFArcs(startNodes,endNodes)
##for i in range(0,mmx):
##    pSn.append(uiarray_get(startNodes,i)+1)
##    pEn.append(uiarray_get(endNodes,i)+1)
##
##print "\n"
##length = mcf.MCFm()
##cost_flow = new_darray(length)
##mcf.MCFGetX(cost_flow)
##for i in range(0,length):
##    startNode   = pSn[i]
##    endNode     = pEn[i]
##    flow        = darray_get(cost_flow,i)
##    cost        = pC[i]
##    realCost    = cost * flow
##    if flow > 0:
##        print "Flow on arc %s from node %s to node %s: %s at %s cost = %s" %(i,
##                  startNode,endNode,flow,cost,realCost)
##
##print "arc flow"
##length = mcf.MCFm()
##flow = new_darray(length)
##length = mcf.MCFn()
##nms = new_uiarray(length)
##mcf.MCFGetX(flow,nms)
##for i in range(0,length):
##    print "flow",darray_get(flow,i),"arc",uiarray_get(nms,i)
##    
##print "node potentials"
##length = mcf.MCFn()
##costs = new_darray(length)
##mcf.MCFGetPi(costs,nms)
##for i in range(0,length):
##    print "flow",darray_get(costs,i),"node",i+1
##
##print "reading graph - arcs"
##length = mcf.MCFm()
##startNodes = new_uiarray(length)
##endNodes = new_uiarray(length)
##mcf.MCFArcs(startNodes,endNodes)
##for i in range(0,length):
##    print "Arc %s: start %s end %s" % (i, uiarray_get(startNodes,i)+1,uiarray_get(endNodes,i)+1)
##
##print "reading graph - costs"
##length = mcf.MCFm()
##costs = new_darray(length)
##mcf.MCFCosts(costs)
##for i in range(0,length):
##    print "Arc %s: cost %s" % (i, darray_get(costs,i))
##
##print "reading graph - capacities"
##length = mcf.MCFm()
##caps = new_darray(length)
##mcf.MCFUCaps(caps)
##for i in range(0,length):
##    print "Arc %s: capacities %s" % (i, darray_get(caps,i))
##
##print "reading nodes - supply/demand"
##length = mcf.MCFn()
##supply = new_darray(length)
##mcf.MCFDfcts(supply)
##for i in range(0,length):
##    print "Node %s: demand %s" % (i+1, darray_get(supply,i))
