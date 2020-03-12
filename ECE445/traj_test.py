import odrive
import time
import sys
import math
from collections import OrderedDict 

path = OrderedDict() 
path[330] = 0
path[335] = 100
path[340] = 200
path[200] = 250
path[100] = 275
path[000] = 330
path[-50] = 200
path[-100] = 100
path[-75] = 0


hip_level = -100 #90 deg = 3900
leg_level = 2100
zero_pos_hip = 3800
zero_pos_knee = 0
serial = 3666335E3037
a1 = 180;
a2 = 180;
knee_ratio = 6.25
hip_ratio = 84/11
rad_to_cpr = (2000/(2*math.pi))


##### Ranges
knee_lower = -5500
knee_upper = 0
hip_lower = -3900
hip_upper = 3900


od = odrive.find_any()
# od = odrive.find(serial)
print("Init done")


knee = od.axis0.controller # Knee
hip = od.axis1.controller # Hip

input("Waiting...")

knee.move_to_pos(0)
hip.move_to_pos(0)

safe_mode = False
fast = True
xpos = 0
ypos = -200


# for x_coord, y_coord in path.items():
# 	print("X: ",x_coord,", Y: ",y_coord)


while (True):


	for x_coord, y_coord in path.items():
		print("X: ",x_coord,", Y: ",y_coord)
		xpos = x_coord
		ypos = y_coord

		# print("Using X: " + str(xpos) + ", Y: " + str(ypos))
		try:
			q2 = -math.acos((xpos*xpos + ypos*ypos - a1*a1 - a2*a2)/(2*a1*a2))
			q1 = math.atan2(ypos, xpos) + math.atan2(a2*math.sin(q2), a1 + a2*math.cos(q2)) - q2
		except ValueError:
			print("Error")
			continue
		print("Generated q2: " + str(q2) + ", q1: " + str(q1))

		knee_setpoint = q2*rad_to_cpr*knee_ratio + zero_pos_knee
		hip_setpoint = -q1*rad_to_cpr*hip_ratio + zero_pos_hip

		print("Knee setpoint: " + str(knee_setpoint))
		print("Hip setpoint: " + str(hip_setpoint))

		if knee_setpoint > knee_upper:
			print("Setpoint out of range (knee)")
			continue
		elif knee_setpoint < knee_lower:
			print("Setpoint out of range (knee)")
			continue
		elif hip_setpoint > hip_upper:
			print("Setpoint out of range (hip)")
			continue
		elif hip_setpoint < hip_lower:
			print("Setpoint out of range (hip)")
			continue

		if safe_mode:
			safe_input = input("Continue? Y / N: ")

			if safe_input.lower() != "y":
				print("Resetting\n")
				continue

		if fast:
			knee.pos_setpoint = int(knee_setpoint)
			hip.pos_setpoint = int(hip_setpoint)
		else:
			knee.move_to_pos(int(knee_setpoint))
			hip.move_to_pos(int(hip_setpoint))

		time.sleep(1)



sys.exit()


'''
Knee
-3300 to 2100 (pos is cw)

Hip
-3900 3800 (pos is cw)
'''