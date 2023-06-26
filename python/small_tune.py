import requests
import datetime
import sys


counter = 0
selector = False
pump_ender = False

print("[!] Be shure you are connected to the Fiush Machine")
print("[!] Follow the instructions\n")

def main():
    # Keep asking for user input until is valid.
    global selector
    global pump_ender
    while True:
        try:
            pump_number = int(input("[?] Provide the Pump id: "))
        except ValueError:
            print("[!] Invalid input, try again.")
            continue
        else:
            break
    while True:
        try:
            ka = float(input("[?] Provide ka: "))
        except ValueError:
            print("[!] Invalid input, try again.")
            continue
        else:
            break
    while True:
        try:
            kb = float(input("[?] Provide kb: "))
        except ValueError:
            print("[!] Invalid input, try again.")
            continue
        else:
            break
    while True:
        while True:
            try:
                quantity = float(input("[?] Provide quantity (ml): "))
            except ValueError:
                print("[!] Invalid input, try again.")
                continue
            else:
                break
        pload = { 
                    'name': pump_number, 
                    'pump1': pump_number, 
                    'ml1': quantity, 
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
        

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit(0)
