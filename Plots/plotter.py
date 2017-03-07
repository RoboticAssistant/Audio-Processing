#! /usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import wave
import sys


spf = wave.open('test.wav','r')

signal = spf.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = spf.getframerate()


#if spf.getnchannels() == 2:
#    print 'Just mono files'
#    sys.exit(0)


Time=np.linspace(0, len(signal)/fs, num=len(signal))

plt.figure(1)
plt.title('Signal Wave...')
plt.plot(Time,signal)
plt.show()