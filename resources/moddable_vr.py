from multiprocessing import Process, Pipe
import dynamic_loader
import sys, os

class LocalsHolder(object):
    pass

class StateUpdate(object):
    def __init__(self, reload_startup = False, reload_update = False):
        self.reload_startup = reload_startup
        self.reload_update = reload_update

def child(stdin_fileno, 
          output_pipe):
    sys.stdin = os.fdopen(stdin_fileno)
    while True:
        input(">>>")
        output_pipe.send(StateUpdate(False, True))

class ModdableVR(object):
    def __init__(self, startup_file_name, startup_fn_name, update_file_name, update_fn_name, reload_pipe):
        self.startup_file_name = startup_file_name
        self.startup_fn_name = startup_fn_name
        self.update_file_name = update_file_name
        self.update_fn_name = update_fn_name
        self.reload_pipe = reload_pipe

        self.startup_func = None
        self.update_func = None

        self.runtime_locals = LocalsHolder()
        self.load_startup()
        self.load_update()

    def load(self, state_update):
        if state_update.reload_startup:
            self.load_startup()
            self.run_startup()
        if state_update.reload_update:
            self.load_update()

    def run(self):
        self.run_startup()
        while True:
            if self.reload_pipe.poll(0):
                self.load(self.reload_pipe.recv())
            self.run_update()

    def load_startup(self):
        self.startup_func = dynamic_loader.import_as_func(self.startup_file_name, self.startup_fn_name)

    def run_startup(self):
        self.runtime_locals = LocalsHolder()
        self.runtime_locals.__dict__.update(self.startup_func())

    def load_update(self):
        self.update_func = dynamic_loader.import_as_func(self.update_file_name, self.update_fn_name)

    def run_update(self, load_update=False):
        self.update_func(self.runtime_locals)

def parent(reader_process, reader_pipe):
    modvr = ModdableVR("mod_startup", "startup", "mod_update", "update", parent_conn)
    modvr.run()

if __name__ == "__main__":
    parent_conn, child_conn = Pipe()
    reader_process = Process(target = child, args = (sys.stdin.fileno(), child_conn,))
    reader_process.start()
    parent(reader_process, parent_conn)