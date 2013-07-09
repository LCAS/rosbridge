#!/usr/bin/env python
from time import time
from random import randint
from urllib2 import urlopen
from re import search

class VersionChecker(object):
  def __init__(self, version):
    self.version = version

  def check(self):
    version = self.version

    # note:
    # The following code for producing the proper Google Analytics request is
    # based off of that provided for PHP in a blog post by Peter van Der Graaf:
    # http://www.vdgraaf.info/google-analytics-without-javascript.html
    # http://www.vdgraaf.info/wp-content/uploads/urchin-image.txt
    #
    # Any bugs are likely due to me and not Peter. -tjay

    utmac='UA-18071-1' #enter the new urchin code
    utmhn='code.google.com' #enter your domain
    utmn=str(randint(1000000000,9999999999)) #random request number
    cookie=str(randint(10000000,99999999)) #random cookie number
    random=str(randint(1000000000,2147483647)) #number under 2147483647
    today=str(int(time())) #today
    referer='0'
    uservar='-' #enter your own user defined variable
    utmp='%2Fp%2Fbrown-ros-pkg%2Fwiki%2FrosbridgeRecVer'

    req = 'http://www.google-analytics.com/__utm.gif?utmwv=1&utmn='+ utmn + '&utmsr=-&utmsc=-&utmul=-&utmje=0&utmfl=-&utmdt=-&utmhn='+ utmhn + '&utmr='+ referer + '&utmp='+ utmp + '&utmac='+ utmac + '&utmcc=__utma%3D'+ cookie + '.'+ random + '.'+ today + '.'+ today + '.'+ today + '.2%3B%2B__utmb%3D'+ cookie + '%3B%2B__utmc%3D'+ cookie + '%3B%2B__utmz%3D'+ cookie + '.'+ today + '.2.2.utmccn%3D(direct)%7Cutmcsr%3D(direct)%7Cutmcmd%3D(none)%3B%2B__utmv%3D'+ cookie + '.'+ uservar + '%3B'

    try:
      f = urlopen(req, timeout=5)
      f.readlines()
      f.close()

      f = urlopen('http://code.google.com/p/brown-ros-pkg/wiki/rosbridgeRecVer', timeout=5)
      line = f.readline()
      while line:
        match = search(r'recommendation: ([0-9]+)',line)
        if match != None:
          recommendation = int(match.group(1))
          if (recommendation > version):
            print "This version of rosbridge is based on svn revision %s, revision %s or greater is highly recommended." % (version, recommendation)
          break

        line = f.readline()
      f.close()

    except:
      print "rosbridge may not have outside Internet connectivity."

    print 
