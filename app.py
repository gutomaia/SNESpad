from flask import Flask, request
from twilio import twiml

app = Flask(__name__)


@app.route('/call', methods=['POST'])
def phone_call():
    resp = twiml.TwimlResponse()
    resp.gather(numDigits=1, action="/handle-key", method="POST", timeout=1000)
    return str(resp)


@app.route('/handle-key', methods=['POST'])
def phone_call():
    # The button that the user pressed on the phone keypad.
    digit_pressed = request.form['Digits']

    # Send the button input to the SNES here.

    # TwiML response to infinitely loop to this route.
    resp = twiml.TwimlResponse()
    resp.gather(numDigits=1, action="/handle-key", method="POST", timeout=1000)
    return str(resp)

if __name__ == '__main__':
    app.run()
