import subprocess
import time
import datetime
import os

#os.run('make')
output = ""
old_out = ""
#output = os.system('make')
try:
    output = subprocess.check_output("make")
    output = subprocess.run(["make"], stdout=subprocess.PIPE)
    output.stdout
except:
    pass
#(out, err) = proc.communicate()
#(output,err) = proc.communicate()

while True:
    os.system('clear')
    dt = datetime.datetime.today()
    #output = subprocess.Popen(["make"], stdout=subprocess.PIPE, shell=True)
    try:
        output = subprocess.run(["make"], stdout=subprocess.PIPE)
        output = output.stdout
    except:
        pass
    #(out, err) = proc.communicate()
    #(output,err) = proc.communicate()
    if output != old_out:
        print(f'Time is {dt.hour}:{dt.minute}:{dt.second}')
        print(output)
        old_out = output
    else:
        print(f'Time is {dt.hour}:{dt.minute}:{dt.second}')
        print(old_out)

    time.sleep(10)

print(a)