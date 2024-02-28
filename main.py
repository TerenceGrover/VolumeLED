import serial
import time
import subprocess


ser = serial.Serial('/dev/cu.usbmodem24201', 9600, timeout=1)
time.sleep(2) 

def set_volume(volume_level):
    """Set the system volume level on macOS. Volume level should be between 0 to 100."""
    subprocess.run(["osascript", "-e", f"set volume output volume {volume_level}"])

def get_current_volume():
    """Get the current system volume level on macOS."""
    result = subprocess.run(["osascript", "-e", "output volume of (get volume settings)"], capture_output=True, text=True)
    return int(result.stdout.strip())

prev_volume_level = get_current_volume()

while True:
    try:
        data = ser.readline().decode().strip()
        if data:
            volumeLevel = int(data) / 255.0
            volumeLevel = round(volumeLevel * 100)
            
            # Update the volume only if the change is significant (like more than 5%)
            if abs(volumeLevel - prev_volume_level) > 5:
                set_volume(volumeLevel)
                prev_volume_level = volumeLevel
    except Exception as e:
        print(f"Error: {e}")
