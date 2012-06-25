#Ply Cropping Software
#Currently calibrated to work on the Radiohead - House of Cards video
#Written by: Nathan Smith

def readFile(inputFile, outputFile, translateVector=(), cropVector=(), taperBaseRatio = 0, swap=False):
	inputData = open(inputFile)
	points = []
	inHeader = True
	ptCounter = 0
	while(inHeader):
		line = inputData.readline()
		if "end_header" in line:
			inHeader = False
	for x in inputData:
		newCoords = [float(y) for y in x.split()]
		if cropVector:
			newCoords = filter_box_crop(newCoords, cropVector[0], cropVector[1], cropVector[2], cropVector[3], cropVector[4], cropVector[5])
		if newCoords:
			if translateVector:
				newCoords = filter_translate(newCoords,translateVector)
			if  taperBaseRatio:
				newCoords = filter_anti_tapering(newCoords, taperBaseRatio, cropVector[4], cropVector[5])
			if swap:
				newCoords = filter_swap(newCoords)
			ptCounter = ptCounter + 1
			points.append(newCoords)
	inputData.close()
	print str(ptCounter) + " Points used"
	outputDump = open(outputFile, 'w')
	outputDump.write("ply\n")
	outputDump.write("format ascii 1.0\n")
	outputDump.write("element vertex %d\n" % ptCounter)
	outputDump.write("property float x\n")
	outputDump.write("property float y\n")
	outputDump.write("property float z\n")
	outputDump.write("end_header\n")
	for x in points:
		outputDump.write("%d %d %d\n" % (x[0], x[1], x[2]))
	outputDump.close()


def filter_translate(coord, tVector):
	return (float(coord[0]) + tVector[0], float(coord[1]) + tVector[1], float(coord[2]) + tVector[2])
	
def filter_box_crop(coord, minX, maxX, minY, maxY, minZ, maxZ):
	if  minX < coord[0] < maxX and minY < coord[1] < maxY and minZ < coord[2] < maxZ:
		return coord
	else:
		return None

def filter_swap(coord):
	x = coord[0]
	y = coord[1]
	z = coord[2]
	coord[0] = x
	coord[1] = z
	coord[2] = -y
	return coord 
	
def filter_anti_tapering(coord, center,  minZ, maxZ, topBaseRatio):
	boxHeight = maxZ - minZ
	pointHeight = coord[2] - minZ
	prop = (boxHeight - pointHeight)/boxHeight
	xDistFromCenter = coord[0] - center[0]
	yDistFromCenter = coord[1] - center[1]


infileloc = r"./plys"
outfileloc =r"./out"
beginFrame = 570
endFrame = 860

for x in range(beginFrame, endFrame + 1):
	inputFile = "%s/%d.csv.ply" % (infileloc, x)
	outputFile = "%s/%d.csv.ply" % (outfileloc, x)
	print "reading " + inputFile
	readFile(inputFile, outputFile,(),(-250,250,-250,250,-150,00),0, True)
