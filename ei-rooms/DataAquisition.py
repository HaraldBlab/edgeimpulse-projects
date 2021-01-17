import csv
import json
import time

# CSV settings
# header: date and time,humidity,temperature
# values are collected every 30 seconds

sample = 2
# input path
# sample 1
if (sample == 1):
    CSV_PATH = 'D:/Users/Harald/Downloads/sensate/sensate-data-export.CSV'
# sample 2
else:
    CSV_PATH = 'D:/Users/Harald/Downloads/sensate/sensate-data-export-2.CSV'

# output path
# 5 minutes sample
#JSON_PATH = 'D:/Users/Harald/Documents/EdgeImpulse/ei-rooms/5min/'
# 30 minutes sample
JSON_PATH = 'D:/Users/Harald/Documents/EdgeImpulse/ei-rooms/30min/'
# 60 minutes sample
#JSON_PATH = 'D:/Users/Harald/Documents/EdgeImpulse/ei-rooms/60min/'

# EdgeImpulse settings (still some kind of magic)
# sampling
interval_size = 30  # ms (should be 3000ms)
# features
# value related to the feature generator window size
# # 300 ms 10 samples
# # 1000 ms 33 samples
# # 200 ms 9 samples
# useful values from data
# # 2 values a minute 
# # 120 values an hour
# feature_samples = 10 # 5 minutes   
feature_samples = 60    # 30 minutes
# feature_samples = 120    # 60 minutes

def write_json(json_list, json_path):
    value_list = []
    for start in range(1,len(json_list)):
        humidity = json_list[start]['humidity']
        temperature = json_list[start]['temperature']
        value_list.append([float(humidity), float(temperature)])
    emptySignature = ''.join(['0'] * 64)
    data = {
        "protected": {
            "ver": "v1",
            "alg": "none",
            "iat": time.time() # epoch time, seconds since 1970
        },
        "signature": emptySignature,
        "payload": {
            "device_type": "SENSATE-ESP8266",
            "interval_ms": interval_size,
            "sensors": [
                { "name": "humidity", "units": "%RH" },
                { "name": "temperature", "units": "Cel" }
            ],
            "values": value_list
        }
    }
    with open(json_path, 'w') as output:
        json.dump(data, output)

def write_jsons(json_list, json_path, prefix):
    for start in  range(1, len(json_list), feature_samples):
        # TODO: only export full samples
        addon = json_list[start]['date and time']
        json_file = json_path + prefix + addon + ".json"
        print("writing", prefix, "to", json_file)
        write_json(json_list[start:start+feature_samples+1], json_file)

with open(CSV_PATH, 'r') as csv_file:
    csv_file = csv.DictReader(csv_file)
    json_list = list(csv_file)

print("Running sample", sample, "from", CSV_PATH)

# TODO: use a multiplier of feature_samples here
# sample 1
if (sample == 1):
    sleeping = json_list[20:380]
    cooking = json_list[400:500]
    living = json_list[520:860]
# sample 2
else:
    sleeping = None
    cooking = json_list[20:800]
    living = json_list[820:1370]

if (sleeping != None):
    write_jsons(sleeping, JSON_PATH, "sleepig")
write_jsons(cooking, JSON_PATH, "cooking")
write_jsons(living, JSON_PATH, "living")