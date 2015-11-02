#!/usr/bin/env python

"""
Created on Sun Nov 01 10:13:25 2015

Autoverification script for the Encircular problem.
It will create different routes for the robot:
 G - go
 L - turn left
 R - turn right
 
The output expected is either
 YES - the asimptotic circle exists
 NO  - it doesn't

@author: ilya
"""

import sys, os, re
import traceback
import platform
import random

from optparse import OptionParser
from subprocess import Popen, PIPE

###############################################################################
def main():
    # global test result
    result = 0;
    
    parser = OptionParser(usage="usage: %prog [options]")
    parser.add_option("-e", "--encircular", type="string", dest="encircular_exe",
        help="Location of the executable to test")
    parser.add_option("-i", "--input-test", type="string", dest="input_test",
        help="Input file with test instructions", default='')
    parser.add_option("-o", "--output-test", type="string", dest="output_test",
        help="Output file with test instructions", default='')
    parser.add_option("--tests", type="int", dest="total_tests",
        help="Total number of tests to generate", default=100)
    parser.add_option("--length", type="int", dest="instr_length",
        help="Maximum instruction lenght", default=100)

    (options, args) = parser.parse_args()
    print ("options: %s\n" % options);
    
    test_runs = []

    if (options.input_test == ''):
        # generate input strings randomly
        for i in range(0, options.total_tests):
            length = random.randint(0, options.instr_length);
            run = ""
            
            for l in range(0, length):
                p = random.randint(0, 2);
                if (p == 0):
                    run += 'G'
                elif (p == 1):
                    run += 'L'
                else:
                    run += 'R'
                    
            test_runs.append(run)
                        
    else:
        # Get the input strings from the file
        if (os.path.isfile(options.input_test) == False):
            print ("ERROR: input file '%s' doesn't exist\n" % options.input_test);
            return False;
        else:
            input_runs = open(options.input_test);
            for run in input_runs:
                # skip the empty lines
                if (len(run) == 0):
                    continue
                else:
                    test_runs.append(run)

    # Save the instructions, if requested
    if (options.input_test == '' and options.output_test != ''):
        f = open(options.output_test, 'w')
        for run in test_runs:
            f.write(run)
            f.write('\n')
        f.close();        
        
    # Do the run
    i = 0
    for run in test_runs:
        process = Popen([options.encircular_exe, run], stdout=PIPE)
        (output, err) = process.communicate()
        exit_code = process.wait()
        print (" %5d: %s %s\n%s" % (++i, exit_code, run, output))
        
        


###############################################################################
if __name__ == "__main__":
    main()

