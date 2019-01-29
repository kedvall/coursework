import pybullet as p
import pybullet_data as pd
import keyboard
import time
import math

mf = 150
def inverseKin(xpos, ypos, zpos):
	q2 = -math.acos((xpos * xpos + ypos * ypos - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1 = math.atan2(ypos, xpos) + math.atan2(a2 * math.sin(q2), a1 + a2 * math.cos(q2))
	q3 = math.atan2(ypos,zpos)
	return q1, q2, q3

def pause():
	for i in range(0,100):
		p.stepSimulation()
		time.sleep(1. / 240.)


def stepBackRightForward():
	for cnt in range(77,35,-1):
		p.stepSimulation()
		time.sleep(1. / 240.)
		xpos = xw[cnt]
		ypos = yw[cnt]
		q1, q2, q3 = inverseKin(xpos, ypos, 0)
		p.setJointMotorControl2(quadruped, jointIds[7], p.POSITION_CONTROL,
								jointDirections[7] * -q1 + jointOffsets[7] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[8], p.POSITION_CONTROL,
								jointDirections[8] * q2 + jointOffsets[8], force=mf)
		p.setJointMotorControl2(quadruped, jointIds[6], p.POSITION_CONTROL,
								jointDirections[6] * targetPos + jointOffsets[6] + q3, force=mf)
def stepFrontRightForward():
	for cnt in range(77,35,-1):
		p.stepSimulation()
		time.sleep(1. / 240.)
		xpos = xw[cnt]
		ypos = yw[cnt]
		q1, q2, q3 = inverseKin(xpos, ypos, 0)
		p.setJointMotorControl2(quadruped, jointIds[1], p.POSITION_CONTROL,
								jointDirections[1] * -q1 + jointOffsets[1] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[2], p.POSITION_CONTROL,
								jointDirections[2] * q2 + jointOffsets[2], force=mf)
		p.setJointMotorControl2(quadruped, jointIds[0], p.POSITION_CONTROL,
								jointDirections[0] * targetPos + jointOffsets[0] + q3, force=mf)
def stepBackLeftForward():
	for cnt in range(77,35,-1):
		p.stepSimulation()
		time.sleep(1. / 240.)
		xpos = xw[cnt]
		ypos = yw[cnt]
		q1, q2, q3 = inverseKin(xpos, ypos, 0)
		p.setJointMotorControl2(quadruped, jointIds[10], p.POSITION_CONTROL,
								jointDirections[10] * -q1 + jointOffsets[10] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[11], p.POSITION_CONTROL,
								jointDirections[11] * q2 + jointOffsets[11], force=mf)
		p.setJointMotorControl2(quadruped, jointIds[9], p.POSITION_CONTROL,
								jointDirections[9] * targetPos + jointOffsets[9] + q3, force=mf)
def stepFrontLeftForward():
	for cnt in range(77,35,-1):
		p.stepSimulation()
		time.sleep(1. / 240.)
		xpos = xw[cnt]
		ypos = yw[cnt]
		q1, q2, q3 = inverseKin(xpos, ypos, 0)
		p.setJointMotorControl2(quadruped, jointIds[4], p.POSITION_CONTROL,
								jointDirections[4] * -q1 + jointOffsets[4] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[5], p.POSITION_CONTROL,
								jointDirections[5] * q2 + jointOffsets[5], force=mf)
		p.setJointMotorControl2(quadruped, jointIds[3], p.POSITION_CONTROL,
								jointDirections[3] * targetPos + jointOffsets[3] + q3, force=mf)
def shiftMassForward():
	for cnt in range(350,80,-1):
		p.stepSimulation()
		time.sleep(1. / 240.)
		xpos = xw[math.floor(cnt/10)]
		ypos = yw[math.floor(cnt/10)]
		q1, q2, q3 = inverseKin(xpos, ypos, 0)
		p.setJointMotorControl2(quadruped, jointIds[1], p.POSITION_CONTROL,
								jointDirections[1] * -q1 + jointOffsets[1] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[2], p.POSITION_CONTROL,
								jointDirections[2] * q2 + jointOffsets[2], force=mf)
		p.setJointMotorControl2(quadruped, jointIds[4], p.POSITION_CONTROL,
								jointDirections[4] * -q1 + jointOffsets[4] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[5], p.POSITION_CONTROL,
								jointDirections[5] * q2 + jointOffsets[5], force=mf)
		p.setJointMotorControl2(quadruped, jointIds[10], p.POSITION_CONTROL,
								jointDirections[10] * -q1 + jointOffsets[10] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[11], p.POSITION_CONTROL,
								jointDirections[11] * q2 + jointOffsets[11], force=mf)
		p.setJointMotorControl2(quadruped, jointIds[7], p.POSITION_CONTROL,
								jointDirections[7] * -q1 + jointOffsets[7] - math.pi / 2, force=mf)
		p.setJointMotorControl2(quadruped, jointIds[8], p.POSITION_CONTROL,
								jointDirections[8] * q2 + jointOffsets[8], force=mf)


def home():
	xposf = xw[math.floor(39)]
	yposf = yw[math.floor(39)]
	q2f = -math.acos((xposf * xposf + yposf * yposf - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1f = math.atan2(yposf, xposf) + math.atan2(a2 * math.sin(q2f), a1 + a2 * math.cos(q2f))

	xposb = xw[math.floor(39)]
	yposb = yw[math.floor(39)]
	q2b = -math.acos((xposb * xposb + yposb * yposb - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1b = math.atan2(yposb, xposb) + math.atan2(a2 * math.sin(q2b), a1 + a2 * math.cos(q2b))

	xposf2 = xw[math.floor(8)]
	yposf2 = yw[math.floor(8)]
	q2f2 = -math.acos((xposf2 * xposf2 + yposf2 * yposf2 - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1f2 = math.atan2(yposf2, xposf2) + math.atan2(a2 * math.sin(q2f2), a1 + a2 * math.cos(q2f2))

	xposb2 = xw[math.floor(8)]
	yposb2 = yw[math.floor(8)]
	q2b2 = -math.acos((xposb2 * xposb2 + yposb2 * yposb2 - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1b2 = math.atan2(yposb2, xposb2) + math.atan2(a2 * math.sin(q2b2), a1 + a2 * math.cos(q2b2))
	p.setJointMotorControl2(quadruped, jointIds[1], p.POSITION_CONTROL,
							jointDirections[1] * -q1f2 + jointOffsets[1] - math.pi / 2, force=mf)
	p.setJointMotorControl2(quadruped, jointIds[4], p.POSITION_CONTROL,
							jointDirections[4] * -q1f + jointOffsets[4] - math.pi / 2, force=mf)
	p.setJointMotorControl2(quadruped, jointIds[7], p.POSITION_CONTROL,
							jointDirections[7] * -q1b2 + jointOffsets[7] - math.pi / 2, force=mf)
	p.setJointMotorControl2(quadruped, jointIds[10], p.POSITION_CONTROL,
							jointDirections[10] * -q1b + jointOffsets[10] - math.pi / 2, force=mf)

	p.setJointMotorControl2(quadruped, jointIds[2], p.POSITION_CONTROL,
							jointDirections[2] * q2f2 + jointOffsets[2], force=mf)
	p.setJointMotorControl2(quadruped, jointIds[5], p.POSITION_CONTROL,
							jointDirections[5] * q2f + jointOffsets[5], force=mf)
	p.setJointMotorControl2(quadruped, jointIds[8], p.POSITION_CONTROL,
							jointDirections[8] * q2b2 + jointOffsets[8], force=mf)
	p.setJointMotorControl2(quadruped, jointIds[11], p.POSITION_CONTROL,
							jointDirections[11] * q2b + jointOffsets[11], force=mf)
	pause()
	pause()
	pause()
	pause()

p.connect(p.GUI)
p.setAdditionalSearchPath(pd.getDataPath())
plane = p.loadURDF("plane.urdf")
p.setGravity(0,0,-9.81)
p.setTimeStep(1./500)
#p.setDefaultContactERP(0)
#urdfFlags = p.URDF_USE_SELF_COLLISION+p.URDF_USE_SELF_COLLISION_EXCLUDE_ALL_PARENTS 
urdfFlags = p.URDF_USE_SELF_COLLISION
quadruped = p.loadURDF("laikago/laikago.urdf",[0,0,.5],[0,0.5,0.5,0], flags = urdfFlags,useFixedBase=False)

#enable collision between lower legs

# for j in range (p.getNumJoints(quadruped)):
# 		print(p.getJointInfo(quadruped,j))

#2,5,8 and 11 are the lower legs
lower_legs = [2,5,8,11]
for l0 in lower_legs:
	for l1 in lower_legs:
		if (l1>l0):
			enableCollision = 1
			# print("collision for pair",l0,l1, p.getJointInfo(quadruped,l0)[12],p.getJointInfo(quadruped,l1)[12], "enabled=",enableCollision)
			p.setCollisionFilterPair(quadruped, quadruped, 2,5,enableCollision)

jointIds=[]
paramIds=[]
jointOffsets=[]
jointDirections=[-1,1,1,1,1,1,-1,1,1,1,1,1]
jointAngles=[0,0,0,0,0,0,0,0,0,0,0,0]

for i in range (4):
	jointOffsets.append(0)
	jointOffsets.append(-0.7)
	jointOffsets.append(0.7)

maxForceId = p.addUserDebugParameter("maxForce",0,100,20)

for j in range (p.getNumJoints(quadruped)):
	p.changeDynamics(quadruped,j,linearDamping=0, angularDamping=0)
	info = p.getJointInfo(quadruped,j)
	#print(info)
	jointName = info[1]
	jointType = info[2]
	if (jointType==p.JOINT_PRISMATIC or jointType==p.JOINT_REVOLUTE):
			jointIds.append(j)

		
p.getCameraImage(480,320)
p.setRealTimeSimulation(0)

joints=[]

with open(pd.getDataPath()+"/laikago/data1.txt","r") as filestream:
	for line in filestream:
		print("line=",line)
	maxForce = p.readUserDebugParameter(maxForceId)
	currentline = line.split(",")
	#print (currentline)
	#print("-----")
	frame = currentline[0]
	t = currentline[1]
	#print("frame[",frame,"]")
	joints=currentline[2:14]
	#print("joints=",joints)
	for j in range (12):
		targetPos = float(joints[j])
		p.setJointMotorControl2(quadruped,jointIds[j],p.POSITION_CONTROL,jointDirections[j]*targetPos+jointOffsets[j], force=maxForce)
	p.stepSimulation()
	for lower_leg in lower_legs:
		#print("points for ", quadruped, " link: ", lower_leg)
		pts = p.getContactPoints(quadruped,-1, lower_leg)
		#print("num points=",len(pts))
		#for pt in pts:
		#	print(pt[9])
	time.sleep(1./500.)


for j in range (p.getNumJoints(quadruped)):
	p.changeDynamics(quadruped,j,linearDamping=0, angularDamping=0)
	p.changeDynamics(quadruped, j, lateralFriction=20)
	info = p.getJointInfo(quadruped,j)
	js = p.getJointState(quadruped,j)
	#print(info)
	jointName = info[1]
	jointType = info[2]
	if (jointType==p.JOINT_PRISMATIC or jointType==p.JOINT_REVOLUTE):
			paramIds.append(p.addUserDebugParameter(jointName.decode("utf-8"),-4,4,(js[0]-jointOffsets[j])/jointDirections[j]))

# p.changeDynamics(quadruped,-1,mass=60)
walk_y_2 = [-0.022701,-0.024316,-0.02552,-0.026366,-0.026904,-0.027184,-0.027248,-0.027138,-0.026893,-0.026549,-0.026138,-0.025688,-0.025227,-0.024778,-0.024361,-0.023993,-0.023689,-0.023458,-0.023311,-0.023251,-0.023281,-0.0234,-0.023603,-0.023884,-0.024232,-0.024635,-0.025075,-0.025535,-0.025991,-0.026418,-0.026788,-0.02707,-0.027228,-0.027227,-0.027024,-0.026577,-0.025839,-0.024761,-0.023289,-0.023,-0.019334,-0.017885,-0.016823,-0.015969,-0.015254,-0.014643,-0.014114,-0.013653,-0.013251,-0.0129,-0.012596,-0.012334,-0.012111,-0.011926,-0.011775,-0.011658,-0.011574,-0.011521,-0.0115,-0.011511,-0.011553,-0.011626,-0.011732,-0.011872,-0.012046,-0.012256,-0.012504,-0.012794,-0.013129,-0.013513,-0.013953,-0.014458,-0.01504,-0.015717,-0.016519,-0.017501,-0.018786,-0.020865]
walk_x_2 = [0.0113,-0.0109,-0.0103,-0.0097,-0.0091,-0.0085,-0.0079,-0.0073,-0.0067,-0.0061,-0.0055,-0.0049,-0.0043,-0.0037,-0.0031,-0.0025,-0.0019,-0.0013,-0.0007,-0.0001,0.0005,0.0011,0.0017,0.0023,0.0029,0.0035,0.0041,0.0047,0.0053,0.0059,0.0065,0.0071,0.0077,0.0083,0.0089,0.0095,0.0101,0.0107,0.0113,0.0113,0.0107,0.0101,0.0095,0.0089,0.0083,0.0077,0.0071,0.0065,0.0059,0.0053,0.0047,0.0041,0.0035,0.0029,0.0023,0.0017,0.0011,0.0005,-0.0001,-0.0007,-0.0013,-0.0019,-0.0025,-0.0031,-0.0037,-0.0043,-0.0049,-0.0055,-0.0061,-0.0067,-0.0073,-0.0079,-0.0085,-0.0091,-0.0097,-0.0103,-0.0109,-0.0115]

xw1 = [0.0113,-0.01635,-0.01545,-0.01455,-0.01365,-0.01275,-0.01185,-0.01095,-0.01005,-0.00915,-0.00825,-0.00735,-0.00645,-0.00555,-0.00465,-0.00375,-0.00285,-0.00195,-0.00105,-0.00015,0.00075,0.00165,0.00255,0.00345,0.00435,0.00525,0.00615,0.00705,0.00795,0.00885,0.00975,0.01065,0.01155,0.01245,0.01335,0.01425,0.01515,0.01605,0.01695,0.01695,0.01605,0.01515,0.01425,0.01335,0.01245,0.01155,0.01065,0.00975,0.00885,0.00795,0.00705,0.00615,0.00525,0.00435,0.00345,0.00255,0.00165,0.00075,-0.00015,-0.00105,-0.00195,-0.00285,-0.00375,-0.00465,-0.00555,-0.00645,-0.00735,-0.00825,-0.00915,-0.01005,-0.01095,-0.01185,-0.01275,-0.01365,-0.01455,-0.01545,-0.01635,-0.01725]
xw = [0.0113,-0.01635,-0.01545,-0.01455,-0.01365,-0.01275,-0.01185,-0.01095,-0.01005,-0.00915,-0.00825,-0.00735,-0.00645,-0.00555,-0.00465,-0.00375,-0.00285,-0.00195,-0.00105,-0.00015,0.00075,0.00165,0.00255,0.00345,0.00435,0.00525,0.00615,0.00705,0.00795,0.00885,0.00975,0.01065,0.01155,0.01245,0.01335,0.01425,0.01515,0.01605,0.01695,0.01695,0.01605,0.01515,0.01425,0.01335,0.01245,0.01155,0.01065,0.00975,0.00885,0.00795,0.00705,0.00615,0.00525,0.00435,0.00345,0.00255,0.00165,0.00075,-0.00015,-0.00105,-0.00195,-0.00285,-0.00375,-0.00465,-0.00555,-0.00645,-0.00735,-0.00825,-0.00915,-0.01005,-0.01095,-0.01185,-0.01275,-0.01365,-0.01455,-0.01545,-0.01635,-0.01725]

for idx, num in enumerate(xw1):
	xw[idx] = xw1[idx]/2
yw = [-0.023987,-0.023988,-0.023989,-0.023991,-0.023992,-0.023993,-0.023994,-0.023995,-0.023996,-0.023996,-0.023997,-0.023998,-0.023998,-0.023999,-0.023999,-0.023999,-0.024,-0.024,-0.024,-0.024,-0.024,-0.024,-0.024,-0.023999,-0.023999,-0.023999,-0.023998,-0.023998,-0.023997,-0.023997,-0.023996,-0.023995,-0.023994,-0.023993,-0.023992,-0.023991,-0.02399,-0.023989,-0.023987,-0.023,-0.019334,-0.017885,-0.016823,-0.015969,-0.015254,-0.014643,-0.014114,-0.013653,-0.013251,-0.0129,-0.012596,-0.012334,-0.012111,-0.011926,-0.011775,-0.011658,-0.011574,-0.011521,-0.0115,-0.011511,-0.011553,-0.011626,-0.011732,-0.011872,-0.012046,-0.012256,-0.012504,-0.012794,-0.013129,-0.013513,-0.013953,-0.014458,-0.01504,-0.015717,-0.016519,-0.017501,-0.018786,-0.020865]

a1 = 0.025
a2 = 0.0275
idx = 39
idxb = 39
idx2 = 0
idxb2 = 0
ro=0.0
lo=0.0
home()
while (1):
	p.stepSimulation()
	time.sleep(1. / 240.)

	orient = p.getEulerFromQuaternion(p.getLinkState(quadruped, 0)[1])[1]
	# print(orient)
	# if orient is less than zero, extend right legs more to go straight
	if orient < 0:
		#falling to right
		ro = 1

	if orient >= 0:
		#falling to left
		lo = 1

	if idxb == 0:
		idxb = 77
	idxb -= 0.5
	if idx == 0:
		idx = 77
	idx -= 0.5

	if idxb2 == 0:
		idxb2 = 77
	idxb2 -= 0.5
	if idx2 == 0:
		idx2 = 77
	idx2 -= 0.5

	xposf = xw[math.floor(idx)]
	yposf = yw[math.floor(idx)]
	q2f = -math.acos((xposf * xposf + yposf * yposf - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1f = math.atan2(yposf, xposf) + math.atan2(a2 * math.sin(q2f), a1 + a2 * math.cos(q2f))

	xposb = xw[math.floor(idxb)]
	yposb = yw[math.floor(idxb)]
	q2b = -math.acos((xposb * xposb + yposb * yposb - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1b = math.atan2(yposb, xposb) + math.atan2(a2 * math.sin(q2b), a1 + a2 * math.cos(q2b))

	xposf2 = xw[math.floor(idx2)]
	yposf2 = yw[math.floor(idx2)]
	q2f2 = -math.acos((xposf2 * xposf2 + yposf2 * yposf2 - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1f2 = math.atan2(yposf2, xposf2) + math.atan2(a2 * math.sin(q2f2), a1 + a2 * math.cos(q2f2))

	xposb2 = xw[math.floor(idxb2)]
	yposb2 = yw[math.floor(idxb2)]
	q2b2 = -math.acos((xposb2 * xposb2 + yposb2 * yposb2 - a1 * a1 - a2 * a2) / (2 * a1 * a2))
	q1b2 = math.atan2(yposb2, xposb2) + math.atan2(a2 * math.sin(q2b2), a1 + a2 * math.cos(q2b2))

	for i in range(len(paramIds)):
		c = paramIds[i]
		targetPos = p.readUserDebugParameter(c)
		maxForce = p.readUserDebugParameter(maxForceId)
		if i in (0, 3, 6, 9):
			p.setJointMotorControl2(quadruped, jointIds[i], p.POSITION_CONTROL,
									jointDirections[i] * targetPos + jointOffsets[i] + 0.2, force=mf)

	if keyboard.is_pressed('u'):
		for i in range(len(paramIds)):
			c = paramIds[i]
			targetPos = p.readUserDebugParameter(c)
			maxForce = p.readUserDebugParameter(maxForceId)
			if i in (0,3,6,9):
				p.setJointMotorControl2(quadruped, jointIds[i], p.POSITION_CONTROL,
										jointDirections[i] * targetPos + jointOffsets[i] + 0.2, force=mf)

	if not keyboard.is_pressed('u'):
			stepBackRightForward()
			stepFrontRightForward()
			stepBackLeftForward()
			stepFrontLeftForward()
			shiftMassForward()



