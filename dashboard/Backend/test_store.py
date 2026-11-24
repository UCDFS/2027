import unittest 
from pathlib import Path

# Support running this file directly and importing it as a package module.
try:
    from .SignalStore import SignalStore, load_definitions
except ImportError:
    from SignalStore import SignalStore, load_definitions

SIGNAL_FILE = Path(__file__).parent / "Signals.toml"

class FakeClock:
    def __init__(self):
        self._t = 100.0

    def __call__(self):
        return self._t


class TestDefinitions(unittest.TestCase):
    def test_all_eleven_signals_load(self):
        defs = load_definitions(SIGNAL_FILE)
        self.assertEqual(len(defs), 11)
        self.assertIn("speed_kph", defs)
        self.assertIn("lap_time_ms", defs)

    def test_every_signal_has_a_sane_range_and_timeout(self):
        for d in load_definitions(SIGNAL_FILE).values():
            self.assertGreater(d.stale_after, 0.0, d.name)
            self.assertLess(d.min, d.max, d.name)

class TestStore(unittest.TestCase):
    def setUp(self):
        self.clock = FakeClock()
        self.store = SignalStore(load_definitions(SIGNAL_FILE), clock=self.clock)

    def test_never_recieved_is_stale_with_no_values(self):
        snap = self.store.snapshot()
        self.assertEqual(snap["speed_kph"], {"v": None, "stale": True})

    def test_fresh_value_is_not_stale(self):
        self.store.update("speed_kph", 42.5)
        snap = self.store.snapshot()
        self.assertEqual(snap["speed_kph"], {"v": 42.5, "stale": False})

    def test_value_goes_stale_after_timeout(self):
        self.store.update("speed_kph", 42.5)
        self.clock._t += 0.4
        self.assertFalse(self.store.snapshot()["speed_kph"]["stale"])
        self.clock._t += 0.2
        snap = self.store.snapshot()["speed_kph"]
        self.assertTrue(snap["stale"])
        self.assertEqual(snap["v"], 42.5) # stale values are still returned, just marked as stale

    def test_new_update_clears_stale(self):
        self.store.update("speed_kph", 10)
        self.clock._t += 5
        self.assertTrue(self.store.snapshot()["speed_kph"]["stale"])
        # A fresh update should clear stale and return the newest value.
        self.store.update("speed_kph", 11)
        snap = self.store.snapshot()
        self.assertFalse(snap["speed_kph"]["stale"])
        self.assertEqual(snap["speed_kph"]["v"], 11)

    def test_signals_go_stale_independantly(self):
        self.store.update("speed_kph", 10)
        self.store.update("lap_time_ms", 400)
        self.clock._t += 1.0
        snap = self.store.snapshot()
        self.assertTrue(snap["speed_kph"]["stale"])
        # This signal has never been received, so it is also stale.
        self.assertTrue(snap["dc_bus_voltage"]["stale"])

    def test_unkown_signal_is_rejected(self):
        with self.assertRaises(KeyError):
            self.store.update("speed_kmh", 10)  # typo 

    def test_snapshot_lists_every_signal(self):
        self.assertEqual(set(self.store.snapshot()), set(self.store.names))


if __name__ == "__main__":
    unittest.main()