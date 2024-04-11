import serial
import sqlite3
import matplotlib.pyplot as plt

# Connect to the database or create it if it doesn't exist
conn = sqlite3.connect('ATC_DB.db')
c = conn.cursor()
ser = serial.Serial('/dev/ttyACM0', 9600)
valuefromarduino = ser.readline().decode('utf-8').strip()

# Create the PMOS table if it doesn't exist
if valuefromarduino == "1" :
    c.execute('''
        CREATE TABLE IF NOT EXISTS PMOS (
            ID INTEGER PRIMARY KEY,
            GVoltage REAL,
            SVoltage REAL,
            DVoltage REAL,
            VGS REAL,
            Current REAL
        )
    ''')
    vgs_values = []
    current_values = []

    while True:
        try:
            data = ser.readline().decode('utf-8').strip()

            # Debugging line to print raw Arduino data
            print(f"Raw Arduino Data: {data}")

            # Split the data based on spaces
            values = data.split()

            # Extract numerical values
            gvoltage_value = float(values[0].split(":")[1])
            svoltage_value = float(values[1].split(":")[1])
            dvoltage_value = float(values[2].split(":")[1])
            vgs_value = float(values[3].split(":")[1])
            current_value = float(values[4].split(":")[1])

            # Store data in the database
            c.execute("INSERT INTO PMOS (GVoltage, SVoltage, DVoltage, VGS, Current) VALUES (?, ?, ?, ?, ?)",
                    (gvoltage_value, svoltage_value, dvoltage_value, vgs_value, current_value))
            conn.commit()

            # Append values for plotting
            vgs_values.append(vgs_value)
            current_values.append(current_value)

        except KeyboardInterrupt:
            print("Data collection stopped by user")
            break

    plt.figure()
    plt.plot(vgs_values, current_values, 'o-')
    plt.xlabel('VGS')
    plt.ylabel('CURRENT')
    plt.title('VGS vs. Current')
    plt.grid(True)
    plt.show()

elif valuefromarduino == "0": 
    # Create the NMOS table if it doesn't exist
    c.execute('''
        CREATE TABLE IF NOT EXISTS NMOS (
            ID INTEGER PRIMARY KEY,
            GVoltage REAL,
            SVoltage REAL,
            DVoltage REAL,
            VGS REAL,
            Current REAL
        )
    ''')
    vgs_values = []
    current_values = []

    while True:
        try:
            data = ser.readline().decode('utf-8').strip()

            # Debugging line to print raw Arduino data
            print(f"Raw Arduino Data: {data}")

            # Split the data based on spaces
            values = data.split()

            # Extract numerical values
            gvoltage_value = float(values[0].split(":")[1])
            svoltage_value = float(values[1].split(":")[1])
            dvoltage_value = float(values[2].split(":")[1])
            vgs_value = float(values[3].split(":")[1])
            current_value = float(values[4].split(":")[1])

            # Store data in the database
            c.execute("INSERT INTO NMOS (GVoltage, SVoltage, DVoltage, VGS, Current) VALUES (?, ?, ?, ?, ?)",
                    (gvoltage_value, svoltage_value, dvoltage_value, vgs_value, current_value))
            conn.commit()

        except KeyboardInterrupt:
            print("Data collection stopped by user")
            break
        
    plt.figure()
    plt.plot(vgs_values, current_values, 'o-')
    plt.xlabel('VGS')
    plt.ylabel('CURRENT')
    plt.title('VGS vs. Current')
    plt.grid(True)
    plt.show()

ser.close()
conn.close()
