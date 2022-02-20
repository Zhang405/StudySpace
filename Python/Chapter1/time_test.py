import time
tick = time.time()
print("tick:", tick)
localtick = time.localtime(tick)
print("localtime:",localtick)
asctime = time.asctime(localtick)
print("asctime:",asctime)


