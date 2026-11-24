"""SignalStore : The latest value of every signal, and a stale flag
    
    Nothing about the comms protocol used nor is DBC needed
    Anything that produces data( simulator, CAN reader, UDP reciever)
    just calls update(name, value)
    Anything that consumes data (WebSocket Server, alert rules, looger) calls snapshot()"""

import threading
import time
import tomllib
from dataclasses import dataclass
from pathlib import Path

@dataclass(frozen=True)
class SignalDef:
    name : str
    unit : str
    stale_after : float
    min : float
    max : float

def load_definitions(path):
    """Load signal definitions from a TOML file"""
    with open(path, "rb") as f:
        data = tomllib.load(f)
    defs = {}
    for name, d in data["signals"].items():
        defs[name] = SignalDef(
            name=name,
            unit=d.get("unit", ""),
            stale_after=float(d["stale_after"]),
            min=d.get("min"),
            max=d.get("max"),
        )
    return defs

class SignalStore:
    def __init__(self, definitions, clock=time.monotonic):
        self._defs = dict(definitions)
        self._clock = clock # injectable so tests don't have to wait real time
        self._lock = threading.Lock() # a CAN reader thread and a WebSocket server thread will be accessing this
        self._values = {}


    @property
    def names(self):
        return list(self._defs)

    def update(self, name, value):
        """Record a new value, Unknown names are rejected so typos show up in the logs"""
        if name not in self._defs:
            raise KeyError(f"Unknown signal name: {name!r}")
        with self._lock:
            self._values[name] = (value, self._clock())

    def snapshot(self):
        """"Return {name: {"v" : value or None, "stale" : True/False} } for every signal
            A signal that has never been recieved is stale with v = none"""

        now = self._clock()
        out = {}
        with self._lock:
            for name, d in self._defs.items():
                if name not in self._values:
                    out[name] = {"v": None, "stale": True}
                    continue
                value, t = self._values[name]
                out[name] = {"v": value, "stale": (now - t) > d.stale_after}

        return out
    