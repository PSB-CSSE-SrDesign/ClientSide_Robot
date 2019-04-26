from blinkstick import blinkstick
bstick = blinkstick.find_first()
for x in range(8):
    bstick.set_color(0, x, 0, 0, 0)