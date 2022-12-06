import json
import os
import signal
import sys
import time

from selenium import webdriver
from selenium.webdriver.common.proxy import Proxy, ProxyType

def handler(signum, frame):
    print("visit: trace canceled")
    sys.exit(1)

def visit(url, ext):

    LOAD_TIMEOUT = 15
    TOR_HOST = "localhost"
    TOR_PORT = 9050
    EXT = ext

    proxy = Proxy({
        "proxyType"     : ProxyType.MANUAL,
        "socksProxy"    : "{host}:{port}".format(host=TOR_HOST, port=TOR_PORT)})

    options = webdriver.firefox.options.Options()
    options.headless = True
    options.set_preference("network.proxy.type", 1)
    options.set_preference("network.proxy.socks", TOR_HOST)
    options.set_preference("network.proxy.socks_port", TOR_PORT)
    options.set_preference("network.proxy.socks_remote_dns", True)

    driver = webdriver.Firefox(options=options, proxy=proxy)
    if EXT != "undefended":
        EXT_P = os.path.abspath(os.path.join(os.path.abspath(__file__), "..", "addon", EXT))
        driver.install_addon(EXT_P, temporary=True)

    driver.execute_script("window.location.href=({0}).url;".format(json.dumps({"url":url})))
    time.sleep(LOAD_TIMEOUT)
    driver.close()

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    visit(sys.argv[1], sys.argv[2])
