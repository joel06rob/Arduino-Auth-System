from arduino.app_utils import Bridge, App
from arduino.app_bricks.web_ui import WebUI
import secrets

ui = WebUI()

generated_token = None
attempts = 0;

MAX_ATTEMPTS = 3;

#Generate a cryptographically secure 3 digit code and transmit code to Arduino
#NOTE: 2 args required for handler from socket msg
#
def generate_code(client, data):

    global generated_token
    print("PYTHON: Button Pressed from JS")
    
    generated_token = secrets.randbelow(900) + 100
    
    state = Bridge.call("getDisplayState")
    print(f"Current State: {state}")
    
    Bridge.call("processAuthCode", generated_token)
    
    state = Bridge.call("getDisplayState")
    print(f"Current State (2): {state}")

    #UI - Send code generated message back to webpage
    ui.send_message("awaiting_code", {})


#Compare user entered code with Python generated code
def validate_code(client, data):

    global attempts
    global generated_token

    #Int conversion
    try:
        entered_code = int(data["code"])

    except Exception:
        ui.send_message("access_denied", {})
        print("ERROR: Could not convert the code to int")
        return

    if generated_token is None:
        ui.send_message("access_denied", {})
        return
    
    #Check if correct code - If code is incorrect 3 times then call lock error to Arduino
    if entered_code == generated_token:
        
        print("PYTHON: Code Matched! Access Granted!")
        Bridge.call("unlockAuth")
        ui.send_message("access_granted", {})

        attempts = 0

    else:
        print("PYTHON: Code Incorrect! Access Denied!")
        ui.send_message("access_denied", {})

        attempts += 1

        if attempts >= MAX_ATTEMPTS:
            Bridge.call("setIdleState", True)
            ui.send_message("lock_auth", {})
            attempts = 0

    generated_token = None


        
#Callable functions from Python -> JS    
ui.on_message("generate_code", generate_code)

ui.on_message("validate_code", validate_code)

App.run()