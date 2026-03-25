import sys
import threading
import time
import queue

class ChaosMonkey:
    """
    A helper class to inject chaos (faults) into a Python application 
    demonstrating system resilience.
    
    Usage:
        chaos = ChaosMonkey(app_window)
        if "--chaos" in sys.argv:
            chaos.arm(delay_sec=5)
    """
    def __init__(self, parent=None):
        self.parent = parent # Optional QT/TK Parent for dialogs
        self.active = False
        
    def arm(self, delay_sec=5, mode="random"):
        """Arm the chaos monkey to strike after delay"""
        print(f"[Chaos] Armed! Strike in {delay_sec} seconds...")
        t = threading.Timer(delay_sec, self._strike, args=[mode])
        t.daemon = True
        t.start()
        
    def _strike(self, mode):
        print(f"[Chaos] Striking with mode: {mode}")
        if mode == "leaks" or "--leak" in sys.argv:
            self._simulate_memory_leak()
        elif mode == "crash" or "--crash" in sys.argv:
            self._simulate_crash()
        elif mode == "freeze" or "--freeze" in sys.argv:
            self._simulate_freeze()
        elif mode == "random":
            # Simple default behavior for demo
            self._simulate_crash()
            
    def _simulate_memory_leak(self):
        print("[Chaos] Initiating Memory Leak (10MB/s)...")
        self.leak_storage = []
        while True:
            self.leak_storage.append(bytearray(1024 * 1024 * 10)) # 10MB
            time.sleep(1)
            
    def _simulate_crash(self):
        print("[Chaos] Initiating Unhandled Exception...")
        raise RuntimeError("Chaos Monkey Test Exception! (Expected Failure)")
        
    def _simulate_freeze(self):
        print("[Chaos] Initiating Main Thread Freeze...")
        time.sleep(9999) # Block forever

# Singleton instance
chaos = ChaosMonkey()

def install_chaos_handlers():
    """Call this at app startup"""
    if "--chaos" in sys.argv:
        try:
            # Try to grab delay from args
            delay = 5
            chaos.arm(delay)
        except:
            pass
