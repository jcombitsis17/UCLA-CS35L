#!/usr/bin/python

import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, filename, filestr):
        if filename == "-" or filename == "-i":
            self.lines = filestr
        else:
            f = open(filename, 'r')
            self.lines = f.readlines()
            f.close()

    def chooseline(self):
        return random.choice(self.lines)

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE
       %prog -i LO-HI [OPTION]...

Output randomly selected lines from FILE."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-i", "--input-range",
                      action="store", type="string", dest="irange", 
                      default=None,
                      help="treat each number LO through HI as an input line")
    parser.add_option("-n", "--head-count",
                      action="store",  dest="numlines", 
                      default=None,
                      help="output at most NUMLINES lines")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat",
                      default=False,
                      help="output lines can be repeated")
    options, args = parser.parse_args(sys.argv[1:])
    
    if sys.stdin.isatty() and ((len(args) == 0 and options.irange == None) or (len(args) == 1 and args[0] == "-")):
        return
    try:
        input_file = args[0]
        file_lines = ""
    except IndexError:
        if options.irange == None:
            input_file = "-"
        else:
            input_file = "-i"
            file_lines = [""]
    if input_file == "-":
        file_lines = sys.stdin.readlines()
    try:
        generator = randline(filename=input_file, filestr=file_lines)
    except IOError:
        parser.error("Error: possible extra operand or incorrect filename")

    # -i --input-range
    if options.irange == None:
        irange = "0-"+str(len(generator.lines))
    else:
        irange = str(options.irange)
    try:
        range_lo = int(irange[:irange.index("-")])
        range_hi = int(irange[irange.index("-")+1:])
        if range_lo > range_hi:
            raise ValueError
    except ValueError:
        parser.error("Invalid input-range format")
    if options.irange != None:
        generator.lines = []
        for i in range(range_lo, range_hi+1):
            generator.lines.append(str(i)+"\n")

    # -n --head-count
    try:
        numlines = int(options.numlines)
    except:
        if options.numlines == None:
            numlines = len(generator.lines)
        else:
            parser.error("invalid NUMLINES: {0}".
                         format(options.numlines))
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))

    if len(args) != 0 and len(args) != 1:
        parser.error("wrong number of operands")

    # -r --repeat
    repeat = options.repeat
    if not repeat:
        try:
            count = 1
            while len(generator.lines) > 0 and count <= numlines:
                rand = generator.chooseline()
                sys.stdout.write(rand)
                del generator.lines[generator.lines.index(rand)]
                count += 1
        except IOError:
            parser.error("I/O error")
    else:
        try:
            if options.numlines == None:
                numlines = -1
            count = 0
            while count != numlines:
                sys.stdout.write(generator.chooseline())
                count += 1
        except IOError:
            parser.error("I/O error")

if __name__ == "__main__":
    main()
