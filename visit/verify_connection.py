def handler(signum, frame):
    print("visit: trace canceled")
    sys.exit(1)

if __name__ == "__main__":
    import requests
    import socket
    import sys
    import time
    import signal

    signal.signal(signal.SIGINT, handler)
    TOR_HOST = "localhost"
    TOR_PORT = 9050
    URL = "http://connectivitycheck.gstatic.com/generate_204"
    TRIES = 15
    proxy = {}
    proxy["http"] = proxy["https"] = "socks5h://{}:{}".format(TOR_HOST, TOR_PORT)
    try:
        r = requests.get(URL, verify=False, proxies=proxy, timeout=15)
        if r.status_code == 204:
            print("connection test: success")
            time.sleep(1)
            sys.exit(0)
        else:
            print("connection test: failed to startup in 15 seconds")
    except Exception as e:
        print("connection test: failed to startup in 15 seconds")
    sys.exit(1)
