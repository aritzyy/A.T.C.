import serial
import sqlite3

# Connect to the database or create it if it doesn't exist
conn = sqlite3.connect('ATC_DB.db')
c = conn.cursor()

# Create the PMOS table if it doesn't exist
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

ser = serial.Serial('COM5', 9600)

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

    except KeyboardInterrupt:
        print("Data collection stopped by user")
        break

ser.close()
conn.close()
