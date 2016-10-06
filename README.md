# pymcfsimplex
  Python Wrapper for MCFSimplex - efficient solving of Minimum Cost Flow Problems
  
## 1. What?
pyMCFsimplex is a Python-Wrapper for the C++ MCFSimplex Solver Class from the Operations Research Group at the University of Pisa:

http://www.di.unipi.it/optimize/Software/MCF.html#MCFSimplex

MCFSimplex is a piece of software hat solves big sized Minimum Cost Flow Problems very fast through the (primal or dual) network simplex algorithm. See also this link for a comparison: http://bolyai.cs.elte.hu/egres/tr/egres-13-04.ps

## 2. How?
pyMCFSimplex was being made through SWIG.

## 3. Who?
The authors of MCFSimplex are Alessandro Bertolini and Antonio Frangioni from the Operations Research Group at the Dipartimento di Informatica of the University of Pisa.

pyMCFSimplex is brought to you by Johannes from the [G#.Blog](http://www.sommer-forst.de/blog)

Feel free to contact me: info(at)sommer-forst.de

## 4.Usage
Loading a Minimum Cost Flow Problem instance from DIMACS format and solve it

Here is a first start. "sample.dmx" must be in the same location of your python script. With these lines of code you can parse a minimum cost flow problem in DIMACS file format and solve it.

```
from pyMCFSimplex import * 
print "pyMCFSimplex Version '%s' successfully imported." % version() 
mcf = MCFSimplex() 
print "MCFSimplex Class successfully instantiated." 

FILENAME = 'sample.dmx' 
print "Loading network from DIMACS file %s.." % FILENAME 
f = open(FILENAME,'r') 
inputStr = f.read() 
f.close() 
mcf.LoadDMX(inputStr)

print "Setting time.." 
mcf.SetMCFTime() 
print "Solving problem.." 
mcf.SolveMCF() 

if mcf.MCFGetStatus() == 0: 
  print "Optimal solution: %s" %mcf.MCFGetFO() 
  print "Time elapsed: %s sec " %(mcf.TimeMCF()) 
else: 
  print "Problem unfeasible!" 

print "Time elapsed: %s sec " %(mcf.TimeMCF()) 
