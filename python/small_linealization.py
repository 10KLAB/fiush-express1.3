import numpy as np
import matplotlib.pyplot as plt
# %matplotlib inline


ml= [50, 162, 302]
pulses = [51, 166, 320]


def linealization(pulses_array, minilliters_array):
    cuantity = minilliters_array
    pulses = pulses_array

    range1 = []

    ka_array = []
    kb_array = []
    range_array = [1]

    k_range1 = []

    for i in range(3):
        couple = cuantity[i], pulses[i]
        range1.append(couple)

    ranges = np.array([[range1]])
    # print(ranges[0][0])


    points = np.array(ranges[0][0])
    # get x and y vectors
    x = points[:,0]
    y = points[:,1]
    # calculate polynomial
    z = np.polyfit(x, y, 1)
    a = round(z[0],2)
    b = round(z[1],2)
    ka_array.append(a)
    kb_array.append(b)
        # print("range: ", i, " ka: ", a, " kb: ", b)

    k_range1 = range_array[0], ka_array[0], kb_array[0]

    return(k_range1)

# range_array = []
# ka_array = []
# kb_array = []
# k_range1 = []
# k_range2 = []
# k_range3 = []
# k_range4 = []
k_range1 = linealization(pulses, ml)

print(k_range1)



