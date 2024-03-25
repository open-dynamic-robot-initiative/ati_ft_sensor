# Demo to get ati sensor readings in python

from ati_ft_sensor_cpp import AtiFTSensor

sensor = AtiFTSensor()

sensor.initialize()

while True:
    ft = sensor.getFT()
    print(ft)