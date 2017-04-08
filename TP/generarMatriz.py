#!/usr/bin/env python

import os
import sys
from libmatriz import *

n = int(sys.argv[1])
m = 1
if len(sys.argv) > 2:
	m = int(sys.argv[2])	# m = cant. de vectores b a generar a partir de la matriz nxn
generar_Colley(n, m)