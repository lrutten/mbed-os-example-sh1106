The problem was from the mbed framework.
In `.platformio/packages/framework-mbed/platformio/package_deps/py3/intelhex/compat.py, line 60 change 

~~~~
array_tobytes = getattr(array.array, “tobytes”, array.array.tostring)
~~~~

to

~~~~
array_tobytes = getattr(array.array, “tobytes”, array.array.tobytes) 
~~~~

In Slackware

~~~~
/home/lrutten/.local/lib/python3.9/site-packages/intelhex/compat.py, line 60
~~~~
 
