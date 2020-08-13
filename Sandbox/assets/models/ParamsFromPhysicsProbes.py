import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
objFile = "C:/Users/mart_/Desktop/dev/Hazel/Sandbox/assets/models/Raft.obj"
f = open(objFile)

## SET OBJECT PARAMETERS
density = 0.5 # water = 1.0.
## PROBE VOLUME

Probes = list()
for line in f:
    if line[:3] == "fl ":
        i1 = line.find(" ") + 1
        i2 = line.find(" ", i1+1)
        i3 = line.find(" ", i2+1)
        try:
            i4 = line.find(" ", i3+1)
        except:
            i4 = -1
        probe = (float(line[i1:i2]), float(line[i2:i3]), float(line[i3:i4]))
        Probes.append(np.asarray(probe))
        
def DistanceMatrix(vec):
    mat = np.zeros((len(vec),len(vec)))
    for i in range(0, len(vec)):
        mat[i][i] = float("nan")
        for j in range(0, i):
            mat[i][j] = np.sqrt(np.sum((vec[i] - vec[j]) ** 2))
            mat[j][i] = mat[i][j]
    return mat

R = DistanceMatrix(Probes)
V = list()
for i in range(0, len(Probes)):
    probe_radius = np.nanmin(R[i])  / 2
    probe_radius = 1
    probe_volume = 4 * np.pi * (probe_radius**3) / 3
    V.append(probe_volume)
    
Vtot = sum(V)
M = 0
CoM = np.asarray([0.0,0.0,0.0])
for i in range(0, len(Probes)):
    CoM += V[i] * Probes[i] * density
    M += V[i] * density
    
CoM /= M
# OFFSET EVERYTHING BY CENTER OF MASS
with open(objFile, 'r') as obj:
    lines = obj.read().splitlines()

ix = -1
for i in range(0, len(lines)):
    line = lines[i]
    if line[:3] == "fl ":
        ix += 1
        i1 = line.find(" ") + 1
        i2 = line.find(" ", i1+1)
        i3 = line.find(" ", i2+1)
        try:
            i4 = line.find(" ", i3+1)
        except:
            i4 = -1
        p = [float(line[i1:i2]), float(line[i2:i3]), float(line[i3:i4])] - CoM
        Probes[ix] = p
        temp = "fl {:.6} {:.6} {:.6}".format(p[0], p[1], p[2])
        print(line)
        print(temp)
        print("\n")
        lines[i] = temp
    
    elif line[:2] == "v ":
        i1 = line.find(" ") + 1
        i2 = line.find(" ", i1+1)
        i3 = line.find(" ", i2+1)
        v0 = float(line[i1:i2]) - CoM[0]
        v1 = float(line[i2:i3]) - CoM[1]
        v2 = float(line[i3:-1]) - CoM[2]
        temp = "v {:.6} {:.6} {:.6}".format(v0, v1, v2)
        lines[i] = temp
    
    
f = open(objFile[0:objFile.rfind(".")] + 'Offset.obj', 'w').write('\n'.join(lines))
del(f)
I = np.asarray([[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0]])
i = np.asarray([[1.0, 0.0, 0.0],[0.0, 1.0, 0.0], [0.0, 0.0, 1.0]])
for j in range(0, len(Probes)):
    P = [Probes[j] - CoM]
    
    m = V[j] * density
    I += m * ((np.dot(P[0], P[0]))*i - np.transpose(P) * P)

with open(objFile[0:objFile.rfind(".")] + 'Offset.obj', 'r') as obj:
    lines = obj.read().splitlines()
    
ix = 0
for i in range(0, len(lines)):
    
    line = lines[i]
    if line[:3] == "fl ":
        print(ix, line)
        p = Probes[ix];
        _temp = ( "fl {:.6} {:.6} {:.6} {:.6} {:.6}".format(p[0], p[1], p[2], V[ix], density))
        lines[i] = _temp
        ix+=1
f = open(objFile[0:objFile.rfind(".")] + 'Weighted.obj', 'w').write('\n'.join(lines))
del(f)

print("Moment of inertia matrix:")
for l in I:
    print("{:.6f}\t{:.6f}\t{:.6}".format(l[0], l[1], l[2]))
print("\nTotal mass is \n\t{:.5f} kg\n".format(Vtot * density))
print("Center of Mass:\n\t{:.5f},\t{:.5f},\t{:.5f}\n".format(CoM[0], CoM[1], CoM[2]))

print("\nglm::mat3 I = glm::mat3({:.6f},{:.6f},{:.6f},{:.6f},{:.6f},{:.6f},{:.6f},{:.6f},{:.6f});".format(I[0][0],I[1][0],I[2][0], I[0][1],I[1][1],I[2][1], I[0][2],I[1][2],I[2][2]))
print("float M = {:.6f};".format(M))