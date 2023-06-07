import os
import time
import ipaddress
import wifi
import socketpool
import board
import microcontroller
import terminalio
from digitalio import DigitalInOut, Direction
from adafruit_httpserver.server import HTTPServer
from adafruit_httpserver.request import HTTPRequest
from adafruit_httpserver.response import HTTPResponse
from adafruit_httpserver.methods import HTTPMethod
from adafruit_httpserver.mime_type import MIMEType

#  onboard LED setup
led = DigitalInOut(board.LED)
led.direction = Direction.OUTPUT
led.value = False

#  function to convert celcius to fahrenheit
def c_to_f(temp):
    temp_f = (temp * 9/5) + 32
    return temp_f

#  connect to network
print()
print("Connecting to WiFi")

#  connect to your SSID
wifi.radio.connect(os.getenv('CIRCUITPY_WIFI_SSID'), os.getenv('CIRCUITPY_WIFI_PASSWORD'))

print("Connected to WiFi")
pool = socketpool.SocketPool(wifi.radio)
server = HTTPServer(pool)

# variables for HTML
unit = "F"
#  font for HTML
font_family = "monospace"

#  the HTML script
#  setup as an f string
#  this way, can insert string variables from code.py directly
#  of note, use {{ and }} if something from html *actually* needs to be in brackets
#  i.e. CSS style formatting
dt = 0
def webpage():
    temp = str(c_to_f(microcontroller.cpu.temperature))
    dtstr = returndt()

    html = f"""
    <!DOCTYPE html>
    <html>
    <head>
    <meta http-equiv="Content-type" content="text/html;charset=utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.4/jquery.min.js"></script>
    <style>
    html{{font-family: {font_family}; background-color: lightgrey;
    display:inline-block; margin: 0px auto; text-align: center;}}
      h1{{color: deeppink; width: 200; word-wrap: break-word; padding: 2vh; font-size: 20px;}}
      p{{font-size: 15px; width: 200; word-wrap: break-word;}}
      .button{{font-family: {font_family};display: inline-block;
      background-color: black; border: none;
      border-radius: 4px; color: white; padding: 5px 5px;
      text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}}
      p.dotted {{margin: auto;
      width: 75%; font-size: 15px; text-align: center;}}
    </style>
    </head>
    <body>
    <title>Pico W HTTP Server</title>
    <h1>Pico W HTTP Server</h1>
    <br>
    <p class="dotted">This is a Pico W running an HTTP server with CircuitPython.</p>
    <br>
    <p class="dotted">The current ambient temperature near the Pico W is
    <span style="color: deeppink;"><span id="temp">{temp}</span>°{unit}</span></p><br>
    <p class="dotted">Time since last stopwatch press (s): <span style="color: deeppink;">{dtstr}</span></p><br>
    <p><button class="button" name="LED" value="ON" type="submit">LED ON</button></a></p>
    <p><button class="button" name="LED" value="OFF" type="submit">LED OFF</button></a></p>
    <p><button class="button" name="LED" value="Timer" type="submit">Stopwatch</button></a></p>
    </body>
    <script>
    // LED buttons handler
    $(".button").click((el) => {{ $.post("/led", {{[el.target.name]: el.target.value}}) }});
    // Stopwatch button handler
    $(".button").click((el) => {{ $.post("/stopwatch", {{[el.target.name]: el.target.value}}) }});
    // 1 sec temperature refresher
    setInterval(() => {{
        $.get("/temp", (data) => {{
            $("#temp").text(data);
        }});
    }}, 1000);
    </script>
    </html>
    """
    return html

#  route default static IP
@server.route("/")
def base(request: HTTPRequest):
    #  serve the HTML f string
    #  with content type text/html
    with HTTPResponse(request, content_type=MIMEType.TYPE_HTML) as response:
        response.send(f"{webpage()}")


t1 = 0
# change led state
@server.route("/led", method=HTTPMethod.POST)
def buttonpress(request: HTTPRequest):
    print("button request")
    #  get query params (doesn't handle all cases, use with caution
    query = {x[0] : x[1] for x in [x.split("=") for x in request.body.decode("utf8").split("&")]}
    #  if the led on button was pressed
    if "LED" in query:
        if query["LED"] == "Timer":
            print("stopwatch request")
            t2 = time.monotonic()
            global t1
            global dt
            dt = t2-t1
            t1 = t2
            print(dt)
        else:
            #  turn on or off the onboard LED
            led.value = (query["LED"] == "ON")
    # Acknowledge
    with HTTPResponse(request, content_type=MIMEType.TYPE_HTML) as response:
        response.send(str(dt))


def returndt():
    global dt
    return str(dt)

# get temperature
@server.route("/temp")
def temp(request: HTTPRequest):
    print("Temp request")
    # Read temperature
    temp = str(c_to_f(microcontroller.cpu.temperature))

    with HTTPResponse(request, content_type=MIMEType.TYPE_HTML) as response:
        response.send(temp)

print("Starting server..")
# startup the server
try:
    server.start(str(wifi.radio.ipv4_address))
    print("Listening on http://%s:80" % wifi.radio.ipv4_address)
    #  prints MAC address to REPL
    print("My MAC addr:", [hex(i) for i in wifi.radio.mac_address])
    #  prints IP address to REPL
    print("My IP address is", wifi.radio.ipv4_address)
#  if the server fails to begin, restart the pico w
except OSError:
    time.sleep(5)
    print("Restarting..")
    microcontroller.reset()

while True:
    try:
        #  poll the server for incoming/outgoing requests
        server.poll()
    except Exception as e:
        print(e)
        continue
