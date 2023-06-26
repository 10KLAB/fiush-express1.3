import requests
import datetime
import sys
from linealization import *

pulses = []
pulses1 = [2, 3, 7]
pulses2 = [8, 18, 50]
pulses3 = [51, 166, 320]
pulses4 = [321, 571, 820]
pulses5 = [821, 911, 1000]
# pulses = [5, 8, 10]
pulse_array = []
ml_array = []
counter = 0
selector = False
pump_ender = False


def main():
    # Keep asking for user input until is valid.
    global selector
    global pump_ender
    global ml_array
    while True:
        try:
            file_name = input("[?] Provide the file name: ")
        except ValueError:
            print("[!] Invalid input, try again.")
            continue
        else:
            break

    while True:
        try:
            ka = float(input("[?] Provide ka value: "))
        except ValueError:
            print("[!] Invalid input, try again.")
            continue
        else:
            break   
    while True:
        try:
            kb = float(input("[?] Provide kb value: "))
        except ValueError:
            print("[!] Invalid input, try again.")
            continue
        else:
            break
    pulses = pulses1
    range = 1     

    with open(file_name, "w") as file:
        while True:
            if range == 1:
                pulses = pulses1
                print("r1")
            if range == 2:
                pulses = pulses2
                print("r2")
            if range == 3:
                pulses = pulses3
            if range == 4:
                pulses = pulses4
            if range == 5:
                pulses = pulses5
            print("hola")
            for pulse in (pulses):
                print("[?] For ml: {}".format(pulse))
                # Keep asking for user input until is valid.
                while True:
                    try:
                        if not selector:
                            pump = int(input("[?] Provide pump id: "))
                            selector = True
                    except ValueError:
                        print("[!] Invalid input, try again.")
                        continue
                    else:
                        break

                pload = { 
                    'name': 16, 
                    'pump1': pump, 
                    'ml1': pulse, 
                    'ka1': ka, 
                    'kb1': kb, 
                    'priority1': 1, 
                    'rotation1': 0 
                }
                
                try:
                    url = 'http://192.168.4.1/preparation'
                    requests.post(url, json=pload)
                except:
                    print("[!] Couldn't make the POST request to {}".format(url))
                    print("[!] Miss me with that shit, beach")
                    sys.exit(1)

                # Keep asking for user input until is valid.
                while True:
                    try:
                        quantity = float(input("[?] Provide quantity (ml): "))
                    except ValueError:
                        print("[!] Invalid input, try again.")
                        continue
                    else:
                        break
                pulse_array.append(pulse)
                ml_array.append(quantity)
                # print(ml_array)
                # counter += 1

                file.write("{}\t{}\n".format(pulse, quantity))
                if len(pulses) == len(ml_array):
                    while True:
                        try:
                            ka = float(input("[?] Provide ka value: "))
                        except ValueError:
                            print("[!] Invalid input, try again.")
                            continue
                        else:
                            break   
                    while True:
                        try:
                            kb = float(input("[?] Provide kb value: "))
                        except ValueError:
                            print("[!] Invalid input, try again.")
                            continue
                        else:
                            break
                    ml_array = []
                    range = range + 1
                if(range>5):
                    sys.exit()


                
print(pulse_array)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit(0)
