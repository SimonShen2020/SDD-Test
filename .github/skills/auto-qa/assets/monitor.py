import threading
import time
import psutil
import logging
import os
import uiautomation as auto
from rapidocr_onnxruntime import RapidOCR

class UniversalMonitor:
    def __init__(self, target_pid=None, log_file="monitor.log"):
        self.pid = target_pid
        self.process = psutil.Process(target_pid) if target_pid else None
        self.running = False
        self.log_file = log_file
        self.logger = self._setup_logger()
        
        # Load local models if available
        assets_dir = os.path.dirname(os.path.abspath(__file__))
        models_dir = os.path.join(assets_dir, "models")
        det_path = os.path.join(models_dir, "ch_PP-OCRv4_det_infer.onnx")
        
        if os.path.exists(det_path):
            self.ocr = RapidOCR(
                det_model_path=os.path.join(models_dir, "ch_PP-OCRv4_det_infer.onnx"),
                cls_model_path=os.path.join(models_dir, "ch_ppocr_mobile_v2.0_cls_infer.onnx"),
                rec_model_path=os.path.join(models_dir, "ch_PP-OCRv4_rec_infer.onnx")
            )
        else:
            self.ocr = RapidOCR()
        
        # Stats
        self.max_cpu = 0
        self.max_mem = 0
        self.errors_detected = 0

    def _setup_logger(self):
        logger = logging.getLogger(f"Monitor-{self.pid}")
        logger.setLevel(logging.INFO)
        if not logger.handlers:
            handler = logging.FileHandler(self.log_file, encoding='utf-8')
            formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
            handler.setFormatter(formatter)
            logger.addHandler(handler)
        return logger

    def attach(self, pid):
        self.pid = pid
        self.process = psutil.Process(pid)
        self.logger.info(f"Attached to PID: {pid}")

    def start(self):
        if not self.process:
            raise ValueError("No process attached. Call attach(pid) first.")
        
        self.running = True
        self.monitor_thread = threading.Thread(target=self._monitor_loop)
        self.monitor_thread.daemon = True
        self.monitor_thread.start()
        self.logger.info("Monitor thread started.")

    def stop(self):
        self.running = False
        if hasattr(self, 'monitor_thread'):
            self.monitor_thread.join(timeout=2)
        
        stats = {
            "max_cpu": self.max_cpu,
            "max_mem_mb": self.max_mem,
            "errors": self.errors_detected
        }
        self.logger.info(f"Monitor stopped. Stats: {stats}")
        return stats

    def _monitor_loop(self):
        # CRITICAL FIX: Initialize COM for background thread UI automation
        with auto.UIAutomationInitializerInThread():
            while self.running and self.process.is_running():
                try:
                    # 1. Process Health
                    start_cpu = time.time()
                    cpu = self.process.cpu_percent(interval=None) # Non-blocking
                    mem = self.process.memory_info().rss / 1024 / 1024
                    
                    self.max_cpu = max(self.max_cpu, cpu)
                    self.max_mem = max(self.max_mem, mem)
                    
                    # 2. Visual State Check (Periodically)
                    self._check_for_error_dialogs()
                    
                    time.sleep(1)
                except psutil.NoSuchProcess:
                    self.logger.error("Process disappeared unexpectedly")
                    self.running = False
                    break
                except Exception as e:
                    self.logger.error(f"Monitor Error: {e}")
                    time.sleep(1)

    def _check_for_error_dialogs(self):
        """
        Scans for top-level windows belonging to the target process
        that match generic 'Error' patterns.
        """
        try:
            root = auto.GetRootControl()
            error_keywords = ["Error", "Exception", "Fatal", "错误", "异常", "警告"]
            
            # Optimization: Filter by ProcessId at the API level if possible, 
            # otherwise iterate top-level windows.
            # auto.GetWindowControl only gets one. We need generic walker.
            
            # Simple heuristic: Check focused window or active window
            # Or iterate children of root (Heavy operation, do carefully)
            for win in root.GetChildren():
                if win.ProcessId == self.pid:
                    # If it's a small window or dialog with error keywords
                    if any(k in win.Name for k in error_keywords):
                        self.logger.error(f"Detected Error Dialog: {win.Name}")
                        self.errors_detected += 1
                        # Optional: Capture Screenshot
                        # win.CaptureToImage(f"error_{time.time()}.png")
        except Exception as e:
            pass # Don't crash monitor due to transient UI issues
